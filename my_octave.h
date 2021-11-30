// # Copyright 2021 Dumitru Bianca Stefania
#include <stdio.h>
#include <stdlib.h>
#pragma once

#define MOD 10007

// for keeping an array with the dimensions of matrices
typedef struct{
	int lines, cols;
} dim;

// for easily refering to the whole pack of matrices in the program
typedef struct{
	int ***matrices;
	dim *dimensions;
	int size;
} pack;

// functions that allocate or free memory
int **alloc_matrix(int lines, int cols, pack *my_pack);
void free_matrix_from_array(pack *my_pack, int index);
void realloc_arrays(pack *my_pack, int allocated);

// helper functions to avoid duplicate code
int check_existence(int index, int size);
void error(void);
int calc_sum(int **matrix, dim dimension);
int **crop(int **matrix, int lines, int cols,
		   int *crop_lines, int *crop_cols, pack *my_pack);
int **read_matrix(int *lines, int *cols, pack *my_pack);
void add_matrix_to_array(int **matrix, int lines,
						 int cols, pack *my_pack, int *allocated);

// functions for the main commands from input
void load_matrix(pack *my_pack, int *allocated);
void print_dimensions(pack my_pack);
void print_matrix(pack my_pack);
void redimension_matrix(pack *my_pack);
void multiply_matrices(pack *my_pack, int *allocated);
void order_matrices(pack *my_pack);
void transpose_matrix(pack *my_pack);
void free_matrix(pack *my_pack, int *allocated);
void strassen_multiply(pack *my_pack, int *allocated);
void free_all(pack *my_pack);

// functions for Strassen
int **strassen(int **matrix_a, int **matrix_b, int lines, pack *my_pack);
int **sum_dif(int **matrix1, int **matrix2,
			  char plus_minus, int lines, pack *my_pack);
void free_basic_matrix(int ***matrix, int lines);
void free_strassen(int ****A, int ****B, int ****C,
				   int ****S, int ****M, int half);
