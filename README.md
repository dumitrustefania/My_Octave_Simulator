// Copyright 2021 Dumitru Bianca Stefania

Computer Programming

Homework #2 - My Octave simulator

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

General description and notes:

*) All the arrays and matrices used are dinamically alocated in memory,
using malloc(), realloc() and free() functions from the stdlib.h library.

*) I defined 2 data types: "dim" (that stores 2 ints - the no. of lines and
columns of a matrix) and "pack" (that stores the array of matrices, the 
array of matrix dimensions and the number of matrices present in the array)

*) I always give my_pack (the bundle of matrices, dimensions and their size)
as parameter in the functions because I have to check whether malloc() or 
realloc() fails. In case it does, I have to free all the memory I allocated, 
so I need to have acces to the arrays.

*) In the main() function, I initially allocate space for 10 elements 
in the matrices and dimensions arrays

*) There are 10 possible commands that can be recieved from input:

- 'L' - Loads a matrix with the given sizes and values into the memory
- 'D' - Prints the no. of lines & columns of the matrix at the given index
- 'P' - Prints the values present in the matrix at the given index
- 'C' - Resizes the matrix based on the given instructions
- 'M' - Multiplies two matrices, adding the resulted matrix into memory
- 'O' - Sorts the matrices in ascending order, based on the 
		sum of their values
- 'T' - Transposes the matrix at the given index
- 'F' - Frees the matrix at a given index
- 'S' - Multiplies two square matrices using the Strassen algorithm
- 'Q' - Frees all the memory alocated and stops the program

*) Strassen is used for multiplying matrices whose size can be written
as 2 ^ n. The 2 matrices are split into 4 smaller matrices and certain 
operations are performed recursively until reaching 1x1 matrices.
Because at each step there are 7 matrices that have to be recursively
coputed with Strassen, the complexity becomes O(n ^ log2(7)).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Commands:

1) 'L' - Load matrix
	- allocates dinamically the necessary memory for the matrix
	- reads the values for the matrix and stores them
	- adds the new matrix and its dimensions to the specific arrays

2) 'D' - Print dimensions
	- outputs the dimension of the matrix at given index

3) 'P' - Print matrix
	- outputs the values stored in the matrix at given index

4) 'C' - Resize matrix
	- recieves the number of lines and columns to be used and their indices
	- allocates the necessary memory for the new resized matrix
	- fills the new matrix with the values specified by the indices given
	- frees the old matrix from memory and replaces it with the new one

5) 'M' - Multiply matrices
	- allocates the necessary memory for the resulting matrix
	- multiplies 2 matrices in O(n^3) time, using the basic algorithm
	- adds the new matrix and its dimensions to the specific arrays

6) 'O' - Order matrices
	- allocates space for an array of sums
	- computes the sum for each matrix
	- uses insertion sort to order the matrices in ascending order

7) 'T' - Transpose matrix
	- allocates the necessary memory for the transposed matrix
	- fills the new matrix with transposed values from initial matrix
	- frees the old matrix from memory and replaces it with its transpose

8) 'F' - Free matrix
	- frees memory for the matrix at given index
	- moves back with one position all the other matrices after it
	- *if the no. of matrices stored in the array becomes less than half 
		the allocated space for the array, reallocates the space of the
		array to half its current size

9) 'S' - Multiply matrices using the Strassen algorithm
	- allocates space for the computed resulting matrix
	- alocates space and initialises the values for 2 arrays to help
		with the next step
	- reuses the crop() function to create the 4 halved matrices for each
		initial matrix
	- computes necessary sums for the next step
	- recursively calls the strassen function to compute the 7 matrices needed
	- computes the 4 matrices that will be put together to form the final
		resulting matrix of the multiplication
	- frees all the memory allocated for the algorithm

10) 'Q' - Free memory
	- frees the memory for all the matrices in the array
	- frees the 2 arrays that stored the matrices and their dimensions
	- stops the program
