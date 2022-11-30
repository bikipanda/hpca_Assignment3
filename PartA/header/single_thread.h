//There are two parts to this file. One is Baseline and the other is the optimised one.
//Please uncomment the one you would want to run.




//void singleThread(int N, int *matA, int *matB, int *output)  // Baseline
//{
//  assert( N>=4 and N == ( N &~ (N-1)));
//  for(int rowA = 0; rowA < N; rowA +=2) {
//    for(int colB = 0; colB < N; colB += 2){
//      int sum = 0;
//      for(int iter = 0; iter < N; iter++) 
//      {
//        sum += matA[rowA * N + iter] * matB[iter * N + colB];
//        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
//      }
//
//      // compute output indices
//      int rowC = rowA>>1;
//      int colC = colB>>1;
//      int indexC = rowC * (N>>1) + colC;
//      output[indexC] = sum;
//    }
//  }
//}







 void singleThread(int N, int *matA, int *matB, int *output)  // Optimised
 {
   // Clearing the output array. 
   for(int i = 0; i<(N>>1)*(N>>1); i++){
     output[i] = 0;
   }

   assert( N>=4 and N == ( N &~ (N-1)));
  
   for(int rowA = 0; rowA < N; rowA +=2) {
     for(int iter = 0; iter < N; iter+=2){
       for(int colB = 0; colB < N; colB++){
         int rowC = rowA >> 1;
         int colC = colB >> 1;
         int indexC = rowC*(N>>1) + colC;
         int sum = output[indexC];

         sum += matA[rowA*N      + iter    ] * matB[iter*N     + colB];
         sum += matA[rowA*N      + (iter+1)] * matB[(iter+1)*N + colB];
         sum += matA[(rowA+1)*N  + iter    ] * matB[iter*N     + colB];
         sum += matA[(rowA+1)*N  + (iter+1)] * matB[(iter+1)*N + colB];

         output[indexC] = sum;
       }
     }
   }
 }
