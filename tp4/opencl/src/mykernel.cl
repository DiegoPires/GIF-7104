__kernel void filter(
__global const uchar *lImage,
__global const int *lHalfK,
__global const int *lWidth,
__global const int *lFilter) {

    /*
    int fy, fx;
    //Variables temporaires pour les canaux de l'image
    double lR, lG, lB;
    for(int x = lHalfK; x < (int)lWidth - lHalfK; x++)
    {
        for (int y = lHalfK; y < (int)lHeight - lHalfK; y++)
        {
            lR = 0.;
            lG = 0.;
            lB = 0.;
            for (int j = -lHalfK; j <= lHalfK; j++) {
                fy = j + lHalfK;
                for (int i = -lHalfK; i <= lHalfK; i++) {
                    fx = i + lHalfK;
                    //R[x + i, y + j] = Im[x + i, y + j].R * Filter[i, j]
                    lR += double(lImage[(y + j)*lWidth*4 + (x + i)*4]) * lFilter[fx + fy*lK];
                    lG += double(lImage[(y + j)*lWidth*4 + (x + i)*4 + 1]) * lFilter[fx + fy*lK];
                    lB += double(lImage[(y + j)*lWidth*4 + (x + i)*4 + 2]) * lFilter[fx + fy*lK];
                }
            }
            //Placer le résultat dans l'image.
            lImage[y*lWidth*4 + x*4] = (unsigned char)lR;
            lImage[y*lWidth*4 + x*4 + 1] = (unsigned char)lG;
            lImage[y*lWidth*4 + x*4 + 2] = (unsigned char)lB;
        }
    }
    */

}