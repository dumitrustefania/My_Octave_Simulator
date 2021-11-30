// Copyright 2021 Dumitru Bianca Stefania

#include "my_octave.h"

int main(void)
{
	int allocated = 10;
	pack my_pack;
	// allocate initial space for 10 elem. in the matrices and dimensions array
	my_pack.matrices = (int ***)malloc(allocated * sizeof(int **));
	my_pack.dimensions = (dim *)malloc(allocated * sizeof(dim));
	my_pack.size = 0;

	// check if malloc() failed
	if (!my_pack.matrices || !my_pack.dimensions)
		error();

	while (1) {
		char task;
		scanf("%c", &task);

		if (task == 'L') {
			load_matrix(&my_pack, &allocated);
		} else if (task == 'D') {
			print_dimensions(my_pack);
		} else if (task == 'P') {
			print_matrix(my_pack);
		} else if (task == 'M') {
			multiply_matrices(&my_pack, &allocated);
		} else if (task == 'F') {
			free_matrix(&my_pack, &allocated);
		} else if (task == 'T') {
			transpose_matrix(&my_pack);
		} else if (task == 'C') {
			redimension_matrix(&my_pack);
		} else if (task == 'O') {
			order_matrices(&my_pack);
		} else if (task == 'S') {
			strassen_multiply(&my_pack, &allocated);
		} else if (task == 'Q') {
			free_all(&my_pack);
			return 0;
		} else if (task >= 'A' && task <= 'Z') {
			printf("Unrecognized command\n");
		}
	}

	return 0;
}

// Read the matrix and add it to the matrix array.
void load_matrix(pack *my_pack, int *allocated)
{
	int lines, cols;
	int **matrix = read_matrix(&lines, &cols, &*my_pack);

	add_matrix_to_array(matrix, lines, cols, &*my_pack, &*allocated);
}

// Print no. of lines & cols of a matrix.
void print_dimensions(pack my_pack)
{
	dim *dimensions = my_pack.dimensions;
	int size = my_pack.size;

	int index;
	scanf("%d", &index);
	if (!check_existence(index, size))
		return;

	int lines = dimensions[index].lines;
	int cols = dimensions[index].cols;
	printf("%d %d\n", lines, cols);
}

// Print a matrix when given its index.
void print_matrix(pack my_pack)
{	int ***matrices = my_pack.matrices;
	dim *dimensions = my_pack.dimensions;
	int size = my_pack.size;

	int index;
	scanf("%d", &index);

	if (!check_existence(index, size))
		return;

	int **matrix = matrices[index];
	int lines = dimensions[index].lines;
	int cols = dimensions[index].cols;

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < cols; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

// Multiply 2 matrices using the basic multiplication algo.
// First, check if the 2 matrices exist and can be multiplied.
// Then, allocate a 3rd matrix, perform the multiplication
// and add the new matrix to the matrix array
void multiply_matrices(pack *my_pack, int *allocated)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	int index1, index2, sum;
	scanf("%d%d", &index1, &index2);

	if (!check_existence(index1, size) || !check_existence(index2, size))
		return;

	if (dimensions[index1].cols != dimensions[index2].lines) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int **matrix1 = matrices[index1];
	int **matrix2 = matrices[index2];
	int lines1 = dimensions[index1].lines;
	int lines2 = dimensions[index2].lines;
	int cols2 = dimensions[index2].cols;

	// allocate space for the resulted matrix from multiplication
	int **matrix_multiply = alloc_matrix(lines1, cols2, &*my_pack);

	// perform O(n^3) multiplication algorithm, with every
	// element being calculated modulo 10007
	for (int i = 0; i < lines1; i++)
		for (int j = 0; j < cols2; j++) {
			sum = 0;
			for (int k = 0; k < lines2; k++)
				sum += (matrix1[i][k] * matrix2[k][j]) % MOD;
			sum %= MOD;
			if (sum < 0)
				sum += MOD;
			matrix_multiply[i][j] = sum;
		}

	(*my_pack).size = size;
	add_matrix_to_array(matrix_multiply, lines1, cols2, &*my_pack, &*allocated);
}

// Transpose a matrix function.
// Allocate a 2nd matrix to calculate the transpose,
// then replace the given matrix with its transpose
void transpose_matrix(pack *my_pack)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	int index;
	scanf("%d", &index);
	if (!check_existence(index, size))
		return;

	int lines = dimensions[index].lines;
	int cols = dimensions[index].cols;
	int **matrix = matrices[index];

	// allocate space for calculating the transpose
	int **transpose = alloc_matrix(cols, lines, &*my_pack);

	// calculate transpose
	for (int i = 0; i < lines; i++)
		for (int j = 0; j < cols; j++)
			transpose[j][i] = matrix[i][j];

	// free the old matrix and replace it with its transpose
	free_matrix_from_array(&*my_pack, index);
	matrices[index] = transpose;
	dimensions[index].lines = cols;
	dimensions[index].cols = lines;
}

// Order the matrices in the array in ascending order
// based on the sum of their elements.
// Calculate the sum of elements for every matrix, then
// perform insertion sort.
void order_matrices(pack *my_pack)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	// allocate space for the sums array and calculate the sum
	// of all elements modulo 10007
	int *sum = (int *)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		sum[i] = calc_sum(matrices[i], dimensions[i]);

	// perform insertion sort and swap the matrices,
	// their dimensions and the sum of their elements
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++) {
			if (sum[i] > sum[j]) {
				int **aux = matrices[i];
				matrices[i] = matrices[j];
				matrices[j] = aux;

				dim auxx = dimensions[i];
				dimensions[i] = dimensions[j];
				dimensions[j] = auxx;

				int aux_ = sum[i];
				sum[i] = sum[j];
				sum[j] = aux_;
			}
		}
	free(sum);
}

// Redimension the matrix, keeping the desired lines and cols
// from the input in the given order.
void redimension_matrix(pack *my_pack)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	int index, lines, cols;
	scanf("%d", &index);
	if (!check_existence(index, size))
		return;

	// allocate space for the array of lines to be cropped, then read from input
	scanf("%d", &lines);
	int *crop_lines = (int *)malloc(lines * sizeof(int));
	if (!crop_lines) {
		free_all(&*my_pack);
		error();
	}
	for (int i = 0; i < lines; i++)
		scanf("%d", &crop_lines[i]);

	// allocate space for the array of cols to be cropped, then read from input
	scanf("%d", &cols);
	int *crop_cols = (int *)malloc(cols * sizeof(int));
	if (!crop_cols) {
		free_all(&*my_pack);
		error();
	}
	for (int i = 0; i < cols; i++)
		scanf("%d", &crop_cols[i]);

	// use crop() function to receive the desired redimensionated matrix
	int **cropped_matrix = crop(matrices[index], lines, cols,
								crop_lines, crop_cols, &*my_pack);

	// free the old matrix and replace it with the redimensionated one
	free_matrix_from_array(&*my_pack, index);
	matrices[index] = cropped_matrix;
	dimensions[index].lines = lines;
	dimensions[index].cols = cols;
	free(crop_cols);
	free(crop_lines);
}

// Free a matrix from memory and eliminate it from the matrix array,
// then move back with one position all the other matrices after it.
void free_matrix(pack *my_pack, int *allocated)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	int index;
	scanf("%d", &index);
	if (!check_existence(index, size))
		return;

	free_matrix_from_array(&*my_pack, index);

	// reposition the matrices in the array after the elimination
	for (int i = index + 1; i < size; i++) {
		matrices[i - 1] = matrices[i];
		dimensions[i - 1] = dimensions[i];
	}
	size--;
	(*my_pack).size = size;

	// if the no. of matrices in the array is less than half the allocated space
	// for the array, realloc the space of the array to half its current size
	if ((*my_pack).size * 2 < *allocated) {
		*allocated /= 2;
		realloc_arrays(&*my_pack, *allocated);
	}
}

// Free the space for all the matrices in the array, then for the array itself
void free_all(pack *my_pack)
{
	int size = (*my_pack).size;

	// free each matrix from the array
	for (int k = 0; k < size; k++)
		free_matrix_from_array(&*my_pack, k);

	// free the matrix array and the dimensions array
	free((*my_pack).matrices);
	free((*my_pack).dimensions);
}
