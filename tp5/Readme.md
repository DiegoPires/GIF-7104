# Red-Black Gauss-Seidel solution for 2D vector with OpenMP

This is a simple solution for the Red-Black Gauss-Seidel solution for a 2D vector parallized with OpenMP

### Prerequisities

You must have OpenMP installed to run this code

### Usage

```shell
main <params>
```

```shell
-h       help

-d       size of the matrix

-f       path of the initialization matrix - Ex: "src/input/init100.txt"

-d       dimension of the matrix

-s       threshold indicating when to stop the process

-i       between how many iterations we should generate an output

-c       how many process to use in the parallelisation

-t       creates a new file with the dimension provided and overwrite the -f parameter with the file created

-p       ignores everything and makes an performance test
```