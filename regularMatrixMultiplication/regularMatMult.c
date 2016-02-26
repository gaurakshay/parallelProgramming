/*
 * Program  :   regularMatMult.c
 * Purpose  :   This program generates two matrices
 *              with random number generator populating it.
 *              Then, straightforward matrix multiplication
 *              is used to calculate the results.
 * Author   :   Akshay Gaur
 * Date     :   2016/02/20
 * Note     :   Since the program uses clock_gettime() function
 *              this should be run on *nix based systems.
 *              Compile this program with the -lrt flag to
 *              get access to the clock_gettime function.
 *
 */

#include <stdio.h>      // For print and stuff.
#include <stdlib.h>
#include <time.h>       // For the clock_gettime function.

// Method to populate the matrix with random numbers using rand().
// Parameters passed:   1. number of rows in the matrix
//                      2. number of columns in the matrix
//                      3. the matrix itself.
void randomizeArray(int row, int col, int mat[row][col]) {
    int i, j;
    srand(time(NULL));
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            mat[i][j] = rand() % 10;
        }
    }

    return;
}


// Method to initialize a matrix with zeros.
// Parameters passed:   1. number of rows in the matrix
//                      2. number of columns in the matrix
//                      3.  the matrix itself.
void initializeArray(int row, int col, int mat[row][col]) {
    int i, j;
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            mat[i][j] = 0;
        }
    }

    return;
}


// Method to print an array of the size that is passed
// along in the parameters.
void printArray(int row, int col, int mat[row][col]) {
    int i, j;
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            printf("[%d][%d]: %d ", i, j, mat[i][j]);
        }
        printf("\n");
    }
    
    return;
}


// The main method
void main () {
    int row1 = 16;
    int col1 = row1;
    int row2 = row1;  // This should be equal to col1 variable.
    int col2 = row1;
    int mat1[row1][col1];
    int mat2[row2][col2];
    int result[row1][col2];
    int i, j, k;
    //clock_t begin, end;
    struct timespec start, end;

    // Populating the two matrices to be multiplied with random numbers.
    printf("Populating arrays of size %d.\n", row1);
    randomizeArray(row1, col1, mat1);
    printf("Array #1 generated.\n");
    randomizeArray(row2, col2, mat2);
    printf("Array #2 generated.\n");
    // Initialize the array that will store the results.
    initializeArray(row1, col2, result);
    printf("Resultant array initialized.\n");

    // Print the arrays that will be multiplied.
    //printf("Printing array #1\n");
    //printArray(row1, col1, mat1);
    //printf("Printing array #2\n");
    //printArray(row2, col2, mat2);
    
    printf("Multipying the two arrays...\n");

    // Start the clock to measure the performance.
    //begin = clock();
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Multiplying the two arrays.
    for(i = 0; i < row1; i++) {
        for(j = 0; j < col2; j++) {
            for(k = 0; k < row2; k++) {
                result[i][j] = result[i][j] + (mat1[i][k] * mat2[k][j]);
            }
        }
    }

    // Stop the clock and measure the time.
    //end = clock();
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("\nTime taken in seconds to calculated the result: %f\n",
            ( (end.tv_sec-start.tv_sec) + ((end.tv_nsec-start.tv_nsec) / 1000000000.0) ) );

    // Print the resulting array.
    //printf("Printing array result\n");
    //printArray(row1, col2, result);

    return;
}
