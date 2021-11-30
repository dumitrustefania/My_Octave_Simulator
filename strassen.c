// Copyright 2021 Dumitru Bianca Stefania

#include "my_octave.h"

// Multiply 2 matrices using the Strassen algo.
// First, check if the 2 matrices exist and can be multiplied.
// Then, use the strassen() funtion and add the
// resulted matrix to the matrices array.
void strassen_multiply(pack *my_pack, int *allocated)
{	int ***matrices = (*my_pack).matrices;
	dim *dimensions = (*my_pack).dimensions;
	int size = (*my_pack).size;

	int index1, index2;
	scanf("%d%d", &index1, &index2);

	if (!check_existence(index1, size) || !check_existence(index2, size))
		return;

	if (dimensions[index1].cols != dimensions[index2].lines) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int **matrix_a = matrices[index1];
	int **matrix_b = matrices[index2];
	int lines = dimensions[index1].lines;

	// use strassen() function that outputs the multiplied matrices
	int **matrix_c = strassen(matrix_a, matrix_b, lines, &*my_pack);

	(*my_pack).size = size;
	add_matrix_to_array(matrix_c, lines, lines, &*my_pack, &*allocated);
}

// The adorable Strassen algo., which uses divide et impera
// and performs the multiplication in O(n^(log2(7))) time,
// faster than the basic O(n^3).
int **strassen(int **matrix_a, int **matrix_b, int lines, pack *my_pack)
{	// allocate space for the resulting matrix
	int **matrix_c = alloc_matrix(lines, lines, &*my_pack);

	//base case: if the matrices have 1x1 size, multiply them as numbers
	if (lines == 1) {
		matrix_c[0][0] = matrix_a[0][0] * matrix_b[0][0];
		return matrix_c;
	}
	int half = lines / 2;
	// allocate space for the arrays that will help with the cropping
	int *crop_line1 = (int *)malloc(half * sizeof(int));
	int *crop_line2 = (int *)malloc(half * sizeof(int));
	if (!crop_line1 || !crop_line2) {
		free_all(&*my_pack); error();
	}
	for (int i = 0; i < half ; i++) {
		crop_line1[i] = i;
		crop_line2[i] = half + i;
	}
	// create the 4 divisions of the initial matrices using the crop function
	int ***A = (int ***)malloc(4 * sizeof(int **));
	int ***B = (int ***)malloc(4 * sizeof(int **));
	A[0] = crop(matrix_a, half, half, crop_line1, crop_line1, &*my_pack);
	A[1] = crop(matrix_a, half, half, crop_line1, crop_line2, &*my_pack);
	A[2] = crop(matrix_a, half, half, crop_line2, crop_line1, &*my_pack);
	A[3] = crop(matrix_a, half, half, crop_line2, crop_line2, &*my_pack);
	B[0] = crop(matrix_b, half, half, crop_line1, crop_line1, &*my_pack);
	B[1] = crop(matrix_b, half, half, crop_line1, crop_line2, &*my_pack);
	B[2] = crop(matrix_b, half, half, crop_line2, crop_line1, &*my_pack);
	B[3] = crop(matrix_b, half, half, crop_line2, crop_line2, &*my_pack);
	// calculate necessary sums & differences for the next step
	int ***S = (int ***)malloc(14 * sizeof(int **));
	S[0] = sum_dif(A[0], A[3], '+', half, &*my_pack);
	S[1] = sum_dif(B[0], B[3], '+', half, &*my_pack);
	S[2] = sum_dif(A[2], A[3], '+', half, &*my_pack);
	S[3] = sum_dif(B[1], B[3], '-', half, &*my_pack);
	S[4] = sum_dif(B[2], B[0], '-', half, &*my_pack);
	S[5] = sum_dif(A[0], A[1], '+', half, &*my_pack);
	S[6] = sum_dif(A[2], A[0], '-', half, &*my_pack);
	S[7] = sum_dif(B[0], B[1], '+', half, &*my_pack);
	S[8] = sum_dif(A[1], A[3], '-', half, &*my_pack);
	S[9] = sum_dif(B[2], B[3], '+', half, &*my_pack);
	// find recursively the necessary matrices to calculate C0 -> C3
	int ***M = (int ***)malloc(7 * sizeof(int **));
	M[0] = strassen(S[0], S[1], half, &*my_pack);
	M[1] = strassen(S[2], B[0], half, &*my_pack);
	M[2] = strassen(A[0], S[3], half, &*my_pack);
	M[3] = strassen(A[3], S[4], half, &*my_pack);
	M[4] = strassen(S[5], B[3], half, &*my_pack);
	M[5] = strassen(S[6], S[7], half, &*my_pack);
	M[6] = strassen(S[8], S[9], half, &*my_pack);
	// calculate other sums for the C1 -> C3 matrices
	S[10] = sum_dif(M[0], M[3], '+', half, &*my_pack);
	S[11] = sum_dif(S[10], M[4], '-', half, &*my_pack);
	S[12] = sum_dif(M[0], M[1], '-', half, &*my_pack);
	S[13] = sum_dif(S[12], M[2], '+', half, &*my_pack);
	// calculate the C0 -> C3 matrices
	int ***C = (int ***)malloc(4 * sizeof(int **));
	C[0] = sum_dif(S[11], M[6], '+', half, &*my_pack);
	C[1] = sum_dif(M[2], M[4], '+', half, &*my_pack);
	C[2] = sum_dif(M[1], M[3], '+', half, &*my_pack);
	C[3] = sum_dif(S[13], M[5], '+', half, &*my_pack);

	// put together C0 -> C1 matrices to form the final resulting matrix
	for (int i = 0; i < lines ; i++)
		for (int j = 0; j < lines; j++) {
			if (i < half && j < half)
				matrix_c[i][j] = C[0][i][j] % MOD;
			else if (i < half)
				matrix_c[i][j] = C[1][i][j - (half)] % MOD;
			else if (j < half)
				matrix_c[i][j] = C[2][i - (half)][j] % MOD;
			else
				matrix_c[i][j] = C[3][i - (half)][j - (half)] % MOD;
			if (matrix_c[i][j] < 0)
				matrix_c[i][j] += MOD;
		}
	// free all the memory that was allocated
	free_strassen(&A, &B, &C, &S, &M, half); free(crop_line1); free(crop_line2);
	return matrix_c;
}

// Calculate the sum or difference of 2 matrices.
int **sum_dif(int **matrix1, int **matrix2,
			  char plus_minus, int lines, pack *my_pack)
{	// allocate space for the resulting matrix
	int **matrix_sum = alloc_matrix(lines, lines, &*my_pack);
	// add/sbubtract the 2 matrices
	for (int i = 0; i < lines; i++)
		for (int j = 0; j < lines; j++) {
			if (plus_minus == '+')
				matrix_sum[i][j] = (matrix1[i][j] + matrix2[i][j]) % MOD;
			if (plus_minus == '-')
				matrix_sum[i][j] = (matrix1[i][j] - matrix2[i][j]) % MOD;
			if (matrix_sum[i][j] < 0)
				matrix_sum[i][j] += MOD;
		}
	return matrix_sum;
}

// Free a matrix when having a pointer to it.
void free_basic_matrix(int ***matrix, int lines)
{	// free all its lines
	for (int i = 0; i < lines; i++)
		free((*matrix)[i]);
	// free the matrix
	free(*matrix);
}

// Free all the memory allocated for the auxiliary matrices in Strassen algo.
void free_strassen(int ****A, int ****B, int ****C,
				   int ****S, int ****M, int half)
{
	for (int i = 0; i < 14; i++) {
		if (i < 4) {
			free_basic_matrix(&(*A)[i], half);
			free_basic_matrix(&(*B)[i], half);
			free_basic_matrix(&(*C)[i], half);
		}
		if (i < 7)
			free_basic_matrix(&(*M)[i], half);
		free_basic_matrix(&(*S)[i], half);
	}
	free(*A); free(*B); free(*C);
	free(*S); free(*M);
}
