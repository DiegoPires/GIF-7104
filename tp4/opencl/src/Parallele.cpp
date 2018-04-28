#pragma once

#include "../lib/lodepng.h"
#include <iostream>
#include <fstream>

#include "../lib/Chrono.hpp"
#include "../PACC/Tokenizer.hpp"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.hpp>
#endif

#include <sys/stat.h>

#include "Shared.cpp"

#define COMPUTE_KERNEL_FILENAME ("src/mykernel.cl")

unsigned int count2 = 307200;       // 1D array of image data 640*480

using namespace std;

const char *getErrorString(cl_int error)
{
    switch(error){
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

            // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

            // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

static int LoadTextFromFile(const char *file_name, char **result_string, size_t *string_len)
{
    char cCurrentPath[1024];
    getcwd(cCurrentPath, sizeof(cCurrentPath));
    cout << cCurrentPath;
    int fd;
    unsigned file_len;
    struct stat file_status;
    int ret;
    *string_len = 0;
    fd = open(file_name, O_RDONLY);
    if (fd == -1){
        printf("Error opening file %s\n", file_name);
        return -1;
    }
    ret = fstat(fd, &file_status);
    if (ret){
        printf("Error reading status for file %s\n", file_name);
        return -1;
    }
    file_len = (unsigned)file_status.st_size;
    *result_string = (char*)calloc(file_len + 1, sizeof(char));
    ret = (int)read(fd, *result_string, file_len);
    if (!ret){
        printf("Error reading from file %s\n", file_name);
        return -1;
    }
    close(fd);
    *string_len = file_len;
    return 0;
}

Chrono executerParallele(string iFilename, string iOutFilename, string noyau){

// Convulation stuff
    ifstream lConfig;

    lConfig.open(noyau);
    if (!lConfig.is_open()) {
        cerr << "Le fichier noyau fourni (" << noyau << ") est invalide." << endl;
        exit(1);
    }

    PACC::Tokenizer lTok(lConfig);
    lTok.setDelimiters(" \n","");

    string lToken;
    lTok.getNextToken(lToken);

    int lK = atoi(lToken.c_str());
    int lHalfK = lK/2;

    // cout << "Taille du noyau: " <<  lK << endl;

    //Lecture du filtre
    double* lFilter;
    lFilter = new double[lK * lK];

    Chrono lChrono(true);

    for (int i = 0; i < lK; i++) {
        for (int j = 0; j < lK; j++) {
            lTok.getNextToken(lToken);
            lFilter[i*lK+j] = atof(lToken.c_str());
        }
    }

    //Lecture de l'image
    //Variables à remplir
    unsigned int lWidth, lHeight;
    vector<unsigned char> lImage; //Les pixels bruts
    //Appeler lodepng

    lChrono.pause();
    decode(iFilename.c_str(), lImage, lWidth, lHeight);

    lChrono.resume();



    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen(COMPUTE_KERNEL_FILENAME, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1,
                          &device_id, &ret_num_devices);

    if (ret != CL_SUCCESS){
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }else{
        cout << "";
    }

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    if (!command_queue)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create memory buffers on the device for each vector
    //cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
    //                                  LIST_SIZE * sizeof(int), NULL, &ret);
    //cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
    //                                  LIST_SIZE * sizeof(int), NULL, &ret);
    //cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
    //                                  LIST_SIZE * sizeof(int), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    //ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
    //                           LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    //ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
    //                           LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

    char *source = 0;
    size_t length = 0;
    printf("Loading kernel source from file '%s'...\n", "mykernel.cl");
    ret = LoadTextFromFile(COMPUTE_KERNEL_FILENAME, &source, &length);
    if (!source || ret)
    {
        printf("Error: Failed to load kernel source!\n");
        return EXIT_FAILURE;
    }

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1,
                                                   (const char **)&source, NULL, &ret);

    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    if (ret != CL_SUCCESS)
    {
        size_t len;
        char buffer[3072000];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "filter", &ret);
    if (!kernel || ret != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
    cl_uint native_double_width;
    clGetDeviceInfo(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &native_double_width, NULL);

    if(native_double_width == 0){
        printf("No double precision support.\n");
    }

    cl_mem bufferlImage = clCreateBuffer(context,  CL_MEM_READ_WRITE, sizeof(int) * lWidth * lHeight * 4, NULL, NULL);
    cl_mem bufferlHalfK = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * lHalfK * 10, NULL, NULL);
    cl_mem bufferlWidth = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * lWidth * 10, NULL, NULL);
    //cl_mem bufferlFilter = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_uint) * native_double_width, NULL, NULL);
    cl_mem bufferlFilter = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * lK * lK * 10, NULL, NULL);


    if (!bufferlImage || !bufferlHalfK || !bufferlWidth || !bufferlFilter)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }

    /*
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                 &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
                              LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

    // Display the result to the screen
    //for(i = 0; i < LIST_SIZE; i++)
    //    printf("%d + %d = %d\n", A[i], B[i], C[i]);
    */

    // Write our data set into the input array in device memory
    //

    int size = sizeof(int) * lWidth * lHeight * 14;
    cout << "Size" << size << "\n";

    ret = clEnqueueWriteBuffer(command_queue, bufferlImage, CL_TRUE, 0, size, lImage.data(), 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        cout << "Error: Failed to write to source array &Image!" << getErrorString(ret) << "\n";
        exit(1);
    }
    ret = clEnqueueWriteBuffer(command_queue, bufferlHalfK, CL_TRUE, 0, sizeof(int) * lHalfK * 10, &lHalfK, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array &HalfK!\n");
        exit(1);
    }
    ret = clEnqueueWriteBuffer(command_queue, bufferlWidth, CL_TRUE, 0, sizeof(int) * lWidth * 10, &lWidth, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array &lWidth!\n");
        exit(1);
    }
    ret = clEnqueueWriteBuffer(command_queue, bufferlFilter, CL_TRUE, 0, sizeof(int) * lK * lK * 10, &lFilter, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array &Filter!\n");
        exit(1);
    }

    ret = 0;
    ret |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferlImage);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferlHalfK);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferlWidth);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferlFilter);
    if (ret != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", ret);
        exit(1);
    }

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    //ret = clReleaseMemObject(a_mem_obj);
    //ret = clReleaseMemObject(b_mem_obj);
    //ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    //free(A);
    //free(B);
    //free(C);
    return 0;

    lChrono.pause();



    return lChrono;

}

