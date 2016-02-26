#include <stdio.h>      //For print and stuff
#include <math.h>       //For finding square root of the number of the process.
#include <stdlib.h>     //For malloc'ing the arrays.
#include <mpi.h>        //MPI library.
#include <time.h>       //For random number generator.


// Allocate memory to the arrays of the size passed as argumens.
double * createArray(int rows, int cols) {
    double *mat;
    // Memory assignment is of the type : malloc(sizeof(dataype) * rows * colums)
    // This way, the members will be references as:
    // matrix[x*columns + j] so that x*columns forwards the pointer
    // to the xth row and then j takes  the pointer to the jth element in
    // that row.
    mat = malloc(sizeof(double) * rows * cols);
    
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
    printf("\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("[%d][%d] = %6.6f  ", i, j, mat[i*cols + j]);
        }
        printf("\n");
    }

    return;
}


// The main method
int main() {
    int n = 1024;                 //Size of matrices
    int p;                      //no of processes
    int q;                      //Square root of number of processes
    MPI_Comm row_communicator;  //for communication accross row
    MPI_Comm col_communicator;  //for comm across col
    int my_overall_rank;        //the rank of the process
    int row_id;                 //Curr. row id of the proc which is rank/q
    int col_id;                 //Curr. col id f the proc which is rank%q
    int my_row_id;              //Rank of the proc among the rows. (row index)
    int my_col_id;              //Rank of the proc among the cols. (col index)
    int sub_array_size;         //The size of the submatrix for each proc.
    double *mat1;               //Declare matrix A.
    double *mat2;               //Declare matrix B.
    double *result;             //Declare matrix C.
    double *loc_mat1;
    double *loc_mat2;
    double *loc_result;
    int i, j, k;                //counters.
    int dest, src;              //The dest and the source to send the data to
    int step;                   //number of steps to mult the matrices.
    int u;

    //Create and initialize the arrays.
    mat1    = createArray(n, n);
    mat2    = createArray(n, n);
    result  = createArray(n, n);

    randomizeArray(mat1, n, n);
    randomizeArray(mat2, n, n);
    initializeArray(result, n, n);

    //Initialize MPI.
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_overall_rank);

    q               = (int) sqrt((double)p);
    row_id          = my_overall_rank/q;
    col_id          = my_overall_rank%q;
    sub_array_size  = n/q;

    //Set up the row and column comms.
    MPI_Comm_split(MPI_COMM_WORLD, row_id, my_overall_rank, &row_communicator);
    MPI_Comm_split(MPI_COMM_WORLD, col_id, my_overall_rank, &col_communicator);

    MPI_Comm_rank(row_communicator, &my_row_id); //get the row index of proc.
    MPI_Comm_rank(col_communicator, &my_col_id); //get the col index of proc.

    if( (my_row_id - 1) % q == -1) {
        dest = q - 1;
    }
    else {
        dest = (my_row_id - 1) % q;
    }
    src  = (my_row_id + 1) % q;

    //Initialize the array for the current process.
    loc_mat1 = createArray(sub_array_size, sub_array_size);
    loc_mat2 = createArray(sub_array_size, sub_array_size);
    loc_result = createArray(sub_array_size, sub_array_size);
    initializeArray(loc_mat1, sub_array_size, sub_array_size);
    //test = temp_mat2;
    initializeArray(loc_mat2, sub_array_size, sub_array_size);
    initializeArray(loc_result, sub_array_size, sub_array_size);
    //printf("initialization successful\n");
    //Set up submatrix for this process:
    for(i = 0; i < sub_array_size; i++) {
        for(j = 0; j < sub_array_size; j++) {
            loc_mat1[i*sub_array_size + j] = mat1[((my_row_id*sub_array_size)+i)*n + ((my_col_id*sub_array_size)+j)];
            loc_mat2[i*sub_array_size + j] = mat2[((my_row_id*sub_array_size)+i)*n + ((my_col_id*sub_array_size)+j)];
        }
    }
    
    for(step = 0; step < q; step++) {
        u = (my_row_id + step) % q;
        MPI_Bcast(loc_mat1, sub_array_size*sub_array_size, MPI_DOUBLE, my_row_id, row_communicator);

        for(i=0; i<sub_array_size; i++) {
            for(j=0; j<sub_array_size; j++) {
                for(k=0; k<sub_array_size; k++) {
                    loc_result[i*sub_array_size + j] = loc_result[i*sub_array_size + j]
                                + (loc_mat1[i*sub_array_size + k] * loc_mat2[k*sub_array_size + j]);
                }
            }
        }
        if(src%2 == 0) {
            MPI_Send(loc_mat2, sub_array_size*sub_array_size, MPI_DOUBLE, dest, 0, col_communicator);
            MPI_Recv(loc_mat2, sub_array_size*sub_array_size, MPI_DOUBLE, src, 0, col_communicator, MPI_STATUS_IGNORE);
        }
        else {
            MPI_Recv(loc_mat2, sub_array_size*sub_array_size, MPI_DOUBLE, src, 0, col_communicator, MPI_STATUS_IGNORE);
            MPI_Send(loc_mat2, sub_array_size*sub_array_size, MPI_DOUBLE, dest, 0, col_communicator);
        }
    }
    
    free(mat1);
    free(loc_mat1);
    free(loc_mat2);
    free(loc_result);
    free(mat2);
    free(result);
    MPI_Finalize();

    if(my_overall_rank == 0) {
        printf("The End.\n");
    }
    return 0;
}
