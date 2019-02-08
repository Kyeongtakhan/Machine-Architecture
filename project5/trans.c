/*Kyeongtak Han
   5492834  han00127
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
 #include <stdio.h>
 #include "cachelab.h"

 int is_transpose(int M, int N, int A[N][M], int B[M][N]);


 /*
  * transpose_submit - This is the solution transpose function that you
  *     will be graded on for Part B of the assignment. Do not change
  *     the description string "Transpose submission", as the driver
  *     searches for that string to identify the transpose function to
  *     be graded.
  */
 char transpose_submit_desc[] = "Transpose submission";
 void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
/*
  There are three sub functions to deal with seperately 32 * 32 matrix, 64 *64,
   and 61 * 67 matix.
*/
     int i,j,k,l;  // This variables represent the row, col, block_row, block_col
     int temp0, temp1, temp2, temp3;           //*This variables used to optimize temperal locality and
    int temp4, temp5, temp6, temp7;            //                     storages which is capable of storing value. Only need to 4 temp variables.
                                               //

     if(M == 32){
      // j represent column of matrix, i represent row of matrix
     	for (j = 0; j < M; j += 8) {
            for (i = 0; i < N; i += 8) {
                // k represent block_col, l repsent block_row
                for (k = i; k < i + 8; ++k) {
                     for (l = j; l < j + 8; ++l) {
                         // If it's diagonals, stores value in temp
                         if (k == l)
                            temp0 = A[k][l];
                         else
                            B[l][k] = A[k][l];
                      }
                 //THis is diagonal, so just stored same value with A's block diagonal. the diagonals of the block move to same position as the matrix B.
                 if (i == j)
                     B[k][k] = temp0;
             }
         }
     }
   }
    /*
       Instead of blocking by 4, first I'm blocking this as 8 * 8 then when I access the first block, I block again by 4 * 4.  First block - 64 * 64 -> 8 * 8,  sencond block -  8 * 8 ->  4 * 4. to configurate the matrix that has no diagonals.
       The key point of this is that I just do as same as the 32 * 32 when it has the diagonals. The other part is that the 2nd block of A transposes  to 2nd block of B
       then stores transposed value in temp. Finally move stroed value to 3rd block of B then The third block of A transpose again to 2nd block.
    */
     else if( M == 64) {
       // This first and 2nd for loop will be iterated by 8 times. As I mentioned above, First I block the matrix as 8 * 8.
       // i - first_block_row, j - first_block-col, k - second_block_row, l - second_block_col

       for(i = 0 ; i < 8 ; i++) {
           for(j = 0 ; j < 8; j++) {
               for(k = i*8 ; k < i*8 + 4 ; k++) {
                   // This is the first quadrant matrix that include the diagonals. This severed as the 32 * 32
                   for(l = j*8 ; l < j*8 + 4 ; l++) { // 1
                       if (k == l)
                           temp0 = A[k][l] ;
                       else
                           B[l][k] = A[k][l] ;
                   }
                   if(i == j)
                     B[k][k] = temp0 ;
                   }
                   // This for loop handles the part that didn't include the diagonals.
                   // trnaspose the 2nd block of A to 2nd block of B
                   for(k = i*8; k < i*8 + 4; k++) {  // 2
                       temp0 = A[k][j*8 + 4] ;
                       temp1 = A[k][j*8 + 5] ;
                       temp2 = A[k][j*8 + 6] ;
                       temp3 = A[k][j*8 + 7] ;
                       B[j*8][k + 4] = temp0 ;
                       B[j*8 + 1][k + 4] = temp1 ;
                       B[j*8 + 2][k + 4] = temp2 ;
                       B[j*8 + 3][k + 4] = temp3 ;
                   }
                   // stores the 2nd block of transposed value in temp variables. Then move to third block of B in a row.
                   for(k = 0 ; k < 4 ; k++) {        // 3
                       temp0 = B[j*8 + k][i*8 + 4] ;
                       temp1 = B[j*8 + k][i*8 + 5] ;
                       temp2 = B[j*8 + k][i*8 + 6] ;
                       temp3 = B[j*8 + k][i*8 + 7] ;
                       temp4 = A[i*8 +4][j* 8 + k];
                       temp5 = A[i*8 +5][j* 8 + k];
                       temp6 = A[i*8 +6][j* 8 + k];
                       temp7 = A[i*8 +7][j* 8 + k];
                       B[j*8 + k][i *8 +4] = temp4;
                       B[j*8 + k][i *8 +5] = temp5;
                       B[j*8 + k][i *8 +6] = temp6;
                       B[j*8 + k][i *8 +7] = temp7;
                       B[j*8 + k + 4][i*8] = temp0 ;
                       B[j*8 + k + 4][i*8 + 1] = temp1 ;
                       B[j*8 + k + 4][i*8 + 2] = temp2 ;
                       B[j*8 + k + 4][i*8 + 3] = temp3 ;
                   }

                    // This is fourth block of the matrix that includes the diagonals. This severed as the 32 * 32
                   for(k = i*8 + 4 ; k < i*8 + 8; k++){   //4
                       for(l = j*8 + 4 ; l < j*8 + 8; l++){
                           if(k == l)
                               temp0 = A[k][l] ;
                           else
                               B[l][k] = A[k][l] ;
                       }
                       if(i == j)
                           B[k][k] = temp0 ;
                 }
           }
        }
     }
     /*
     This is for 61 * 67 matrix. blocking by 16 * 16, then check the diagonals, if diagonals, just do same as the I did previous.
     However, the key point is that why we minum i and j first, the number of diagonals is fixed. Thus, to find the diagonal, I need to minus block size in each row and column.
     Check the block's size not to be over the overal matrix size in for loop. If the indices are not over the matrix size, keep checking the diagonals. If not transpose as usual.
     In the inner most loop, it will check the maximun range of the matrix. If the block is over the size of row, it will break when it realizes. It's same as the checking the diagonal. If it's over the size of column, automately stop transpose.
      i - row, j - column k -block_row, l - blcok-column
     */
    else{
      // blocking by block_row
      for(i = 0; i < N; i += 16) {
        for(j = 0; j < M; j += 16) {

      		for(k = i; (k < i + 16); k++) {
              if(k < N){
		            	for(l = j; (l < j + 16); l++) {
                 	    if (l < M){
 			     	                if ((k - i) == (l - j))
                            	    temp0 = A[k][l];
			                      else
                           	    B[l][k] = A[k][l];
                      }
                  }
               }
               else{
                 break;
               }
                   // Check the diagonals
          for (l = j; (l < j + 16); l++) {
		           if(l < M){
                 	  if ((k - i) == (l - j))
                          B[l][k] = temp0;
               }else{
                 break;
               }
          }
        }
      }
     }
   }
}

 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
 char trans_desc[] = "Simple r w-wise scan transpose";
 void trans(int M, int N, int A[N][M], int B[M][N]) {
     int i, j, tmp;
     for (i = 0; i < N; i++) {
         for (j = 0; j < M; j++) {
             tmp = A[i][j];
             B[j][i] = tmp;
         }
     }
 }

 /*
  * registerFunctions - This function registers your transpose
  *     functions with the driver.  At runtime, the driver will
  *     evaluate each of the registered functions and summarize their
  *     performance. This is a handy way to experiment with different
  *     transpose strategies.
  */
 void registerFunctions() {
     /* Register your solution function */
     registerTransFunction(transpose_submit, transpose_submit_desc);
     /* Register any additional transpose functions */
 //  registerTransFunction(trans, trans_desc);
 }

 /*
  * is_transpose - This helper function checks if B is the transpose of
  *     A. You can check the correctness of your transpose by calling
  *     it before returning from the transpose function.
  */
 int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
     int i, j;
     for (i = 0; i < N; i++) {
         for (j = 0; j < M; ++j) {
             if (A[i][j] != B[j][i]) {
                 return 0;
             }
         }
     }
     return 1;
 }
