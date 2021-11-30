// Copyright 2021 Dumitru Bianca Stefania

#include "my_octave.h"

// Allocate space for a matrix based on its no. of lines & cols.
// Check if malloc() failed. If yes, free everything and exit the program.
int **alloc_matrix(int lines, int cols, pack *my_pack)
{
	int **matrix = (int **)malloc(lines * sizeof(int *));
	// check if malloc() failed
	if (!matrix) {
		free_all(&*my_pack);
		error();
	}

	for (int i = 0; i < lines; i++) {
		// allocate space for every line
		matrix[i] = malloc(cols * sizeof(int));

		// check if malloc() failed
		if (!matrix[i]) {
			// free the memory allocated for the previous lines
			for (int j = 0; j < lines; j++)
				free(matrix[j]);
			// free the matrix
			free(matrix);
			// free all the matrices allocated previously
			free_all(&*my_pack);
			error();
		}
	}

	return matrix;
}

// Allocate a matrix and read values for the matrix from input.
int **read_matrix(int *lines, int *cols, pack *my_pack)
{
	scanf("%d%d", lines, cols);

	int **matrix = alloc_matrix(*lines, *cols, &*my_pack);

	for (int i = 0; i < *lines; i++)
		for (int j = 0; j < *cols; j++)
			scanf("%d", &matrix[i][j]);
	return matrix;
}

// Add a new matrix to the array of matrices, also memorizing
// its dimensions for further use.
void add_matrix_to_array(int **matrix, int lines,
						 int cols, pack *my_pack, int *allocated)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	matrices[size] = matrix;
	dimensions[size].lines = lines;
	dimensions[size].cols = cols;
	// increase the size of the matrices array
	size++;
	// if the size of the array becomes equal to the allocated space for the
	// array, then realloc the array by doubling the allocated space
	if (size == *allocated) {
		*allocated *= 2;
		realloc_arrays(&*my_pack, *allocated);
	}
	(*my_pack).size = size;
}

// Free matrix at given index.
void free_matrix_from_array(pack *my_pack, int index)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;

	int lines = dimensions[index].lines;
	// free all the lines of the matrix
	for (int i = 0; i < lines; i++)
		free(matrices[index][i]);
	//free the matrix itself
	free(matrices[index]);
}

// Realloc the size of the matrices and dimensions arrays
void realloc_arrays(pack *my_pack, int allocated)
{
	int ***tmp = (int ***)realloc
			((*my_pack).matrices, allocated * sizeof(int **));
	dim *tmp2 = (dim *)realloc((*my_pack).dimensions, allocated * sizeof(dim));

	// check if realloc() failed
	if (!tmp || !tmp2) {
		free_all(&*my_pack);
		error();
	}

	(*my_pack).matrices = tmp;
	(*my_pack).dimensions = tmp2;
}

// Check if there is any matrix at given index in array.
int check_existence(int index, int size)
{
	if (index >= size || index < 0) {
		printf("No matrix with the given index\n");
		return 0;
	}
	return 1;
}

// Output error message and exit program if malloc or realloc failed.
void error(void)
{
	fprintf(stderr, "malloc() failed\n");
	exit(-1);
}

// Calculate the sum of all elements in a matrix modulo 10007.
int calc_sum(int **matrix, dim dimension)
{
	int sum = 0;
	for (int i = 0; i < dimension.lines; i++)
		for (int j = 0; j < dimension.cols; j++) {
			sum += matrix[i][j];
			sum %= MOD;
		}
	if (sum < 0)
		sum += MOD;
	return sum;
}

// Crop a matrix when given the array of lines and cols to be kept.
int **crop(int **matrix, int lines, int cols,
		   int *crop_lines, int *crop_cols, pack *my_pack)
{	// allocate space for the cropped matrix
	int **cropped_matrix = alloc_matrix(lines, cols, &*my_pack);

	for (int i = 0; i < lines; i++)
		for (int j = 0; j < cols; j++)
			cropped_matrix[i][j] = matrix[crop_lines[i]][crop_cols[j]];

	return cropped_matrix;
}
