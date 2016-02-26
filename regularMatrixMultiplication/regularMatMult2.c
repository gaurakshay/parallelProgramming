/*
 * Program  :   regularMatMult2.c
 *
 * Purpose  :   This program generates two matrices
 *              with random number generator populating it.
 *              Then, straightforward matrix multiplication
 *              is used to calculate the results.
 *
 * Author   :   Akshay Gaur
 *
 * Date     :   2016/02/20
 *
 * Note     :   The program now uses malloc to assign memory
 *              to the arrays since using large arrays in Stack
 *              causes Segmentation Fault (please refer to 
 *              regularMatMult.c for regular array initialization)
 *
 *              Also, since the program uses clock_gettime() function
 *              this should be run on *nix based systems.
 *              Compile this program with the -lrt flag to
 *              get access to the clock_gettime function.
 *
 */

#include <stdio.h>      // For print and stuff.
#include <stdlib.h>     // For malloc.
#include <time.h>       // For the clock_gettime function.

// Allocate memory to the arrays of the size passed as argumens.
double * createArray(int rows, int cols) {
    double *mat;
    // Memory assignment is of the type : malloc(sizeof(dataype) * rows * colums)
    // This way, the members will be references as:
    // matrix[x*columns + j] so that x*columns forwards the pointer
    // to the xth row and then j takes  the pointer to the jth element in
    // that row.
    mat = malloc(sizeof(double *) * rows * cols);
    
    return mat;
}


// Method to populate the matrix with random numbers using rand().
// Parameters passed:   1. number of rows in the matrix
//                      2. number of columns in the matrix
//                      3. the pointer to matrix.
void randomizeArray(double *mat, int rows, int cols) {
    int i, j;
    srand(time(NULL));
    double randNum;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mat[i*cols + j] = ( ( (double)rand() / (double)RAND_MAX ) * 2.0 ) - 1.0;
        }
    }

    return;
}


// Method to initialize a matrix with zeros.
// Parameters passed:   1. number of rows in the matrix
//                      2. number of columns in the matrix
//                      3.  the matrix itself.
void initializeArray(double *mat, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mat[i*cols + j] = 0.0;
        }
    }

    return;
}


// Method to print an array of the size that is passed
// along in the parameters.
void printArray(double *mat, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("[%d][%d] = %6.6f  ", i, j, mat[i*cols + j]);
        }
        printf("\n");
    }

    return;
}


// The main method
void main () {
    int row1 = 4096;
    int col1 = row1;
    int row2 = row1;  // This should be equal to col1 variable.
    int col2 = row1;
    double *mat1;
    double *mat2;
    double *result;
    int i, j, k;
    struct timespec start, end;

    // Allocate memory to the matrices.
    printf("Allocating memory to the matrices.\n");
    mat1 = createArray(row1, col1);
    printf("Memory allocated to matrix #1 (%dx%d).\n", row1, col1);
    mat2 = createArray(row2, col2);
    printf("Memory allocated to matrix #2 (%dx%d).\n", row2, col1);
    result = createArray(row1, col2);
    printf("Memory allocated to matrix which will store the results (%dx%d).\n", row1, col2);

    // Populating the two matrices to be multiplied with random numbers.
    printf("Populating matrices.\n");
    randomizeArray(mat1, row1, col1);
    printf("Matrix #1 randomized.\n");
    randomizeArray(mat2, row2, col2);
    printf("Matrix #2 randomized.\n");
    // Initialize the array that will store the results.
    initializeArray(result, row1, col2);
    printf("Resultant matrix initialized.\n");

    // Print the arrays that will be multiplied. (uncomment if you want to verify
    // the results.)
    //printf("Printing matrix #1...\n");
    //printArray(mat1, row1, col1);
    //printf("Printing matrix #2...\n");
    //printArray(mat2, row2, col2);
    
    printf("Multipying the two matrices...\n");

    // Start the clock to measure the performance.
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Multiplying the two arrays.
    for(i = 0; i < row1; i++) {
        for(j = 0; j < col2; j++) {
            for(k = 0; k < row2; k++) {
                result[i*col2 + j] = result[i*col2 + j] + ( mat1[i*col1 + k] * mat2[k*col2 + j] );
            }
        }
    }

    // Stop the clock and measure the time.
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("\nTime taken in seconds to calculated the result: %f\n",
            ( (end.tv_sec-start.tv_sec) + ((end.tv_nsec-start.tv_nsec) / 1000000000.0) ) );

    // Print the resulting array.
    //printf("Printing resulting matrix...\n");
    //printArray(result, row1, col2);
    
    // Free the memory allocated to the matrices.
    printf("Freeing the memory allocated for the matrices.\n");
    free(mat1);
    free(mat2);
    free(result);
    printf("END OF PROGRAM.\n");

    return;
}
