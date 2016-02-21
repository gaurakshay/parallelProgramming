/*
 * Program: regularMatMult.c
 * Purpose: This program generates two matrices
 *          with random number generator populating it.
 *          Then, straightforward matrix multiplication
 *          is used to calculate the results.
 * Author:  Akshay Gaur
 * Date:    2016/02/20
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Method to populate the matrix with random numbers using rand().
// Parameters passed:   1. number of rows in the matrix
//                      2. number of columns in the matrix
//                      3. the matrix itself.
void randomizeArray(int row, int col, int mat[row][col]) {
    srand(time(NULL));
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
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
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            mat[i][j] = 0;
        }
    }

    return;
}


// Method to print an array of the size that is passed
// along in the parameters.
void printArray(int row, int col, int mat[row][col]) {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            printf("[%d][%d]: %d ", i, j, mat[i][j]);
        }
        printf("\n");
    }
    
    return;
}


// The main method
void main () {
    int row1 = 10;
    int col1 = 12;
    int row2 = 12;  // This should be equal to col1 variable.
    int col2 = 10;
    int mat1[row1][col1];
    int mat2[row2][col2];
    int result[row1][col2];

    // Populating the two matrices to be multiplied with random numbers.
    randomizeArray(row1, col1, mat1);
    randomizeArray(row2, col2, mat2);
    // Initialize the array that will store the results.
    initializeArray(row1, col2, result);

    // Print the arrays that will be multiplied.
    printf("Printing array #1\n");
    printArray(row1, col1, mat1);
    printf("Printing array #2\n");
    printArray(row2, col2, mat2);

    // Multiplying the two arrays.
    for(int i = 0; i < row1; i++) {
        for(int j = 0; j < col2; j++) {
            for(int k = 0; k < row2; k++) {
                result[i][j] = result[i][j] + (mat1[i][k] * mat2[k][j]);
            }
        }
    }

    // Print the resulting array.
    printf("Printing array result\n");
    printArray(row1, col2, result);

    return;
}
