#include <pthread.h>

// Create other necessary functions here

#define NUM_THREADS 8

typedef struct matrix_data{
    int N;
    int* matA;
    int* matB;
    int* output;
}matrix_data;

typedef struct thread_args{
    // thread config
    int work_start; 
    int work_end;   
    // data  
    matrix_data matrix;
}thread_args;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;



void *RMM(void *arg)    // Without locking
{
  thread_args *args = (thread_args *)arg;

  int work_start  = args->work_start;
  int work_end    = args->work_end;

  int N           = args->matrix.N;
  int* matA       = args->matrix.matA;
  int* matB       = args->matrix.matB;
  int* output     = args->matrix.output;
  
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int iter = 0; iter < N; iter+=2){
      for(int colB = work_start; colB < work_end; colB++){
        
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
  return NULL;
}


 


// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
  assert( N>=4 and N == ( N &~ (N-1)));

  // Clearing the output array. 
  for(int i = 0; i<(N>>1)*(N>>1); i++){
    output[i] = 0;
  }

  
  matrix_data matrix;

  matrix.N      = N;
  matrix.matA   = matA;
  matrix.matB   = matB;
  matrix.output = output;
  

  

  // Distributing the work onto multiple threads.
  pthread_t child_tids[NUM_THREADS];

  thread_args work_ranges[NUM_THREADS];
  int curr_start  = 0;
  int offset      = N/NUM_THREADS;
  for(int i = 0; i<NUM_THREADS; i++){
    work_ranges[i].work_start = curr_start;
    work_ranges[i].work_end   = curr_start + offset;
    work_ranges[i].matrix     = matrix;                 //Assigning data pointers

    curr_start = curr_start + offset;
  }  

  work_ranges[NUM_THREADS - 1].work_end = N;          // Handles the boundary case wherein we do not have N/10 as a integer multiple.


  for(int i = 0; i< NUM_THREADS; i++){
    pthread_create(&child_tids[i], NULL, RMM, &work_ranges[i]);
  }

  for(int i = 0; i< NUM_THREADS; i++){
    pthread_join(child_tids[i], NULL);
  }    


}


/********************************************************************************************************************************/

//void *RMM(void *arg)    // with locking //not for submission.  // has issues.
// {
//   thread_args *args = (thread_args *)arg;
//
//   int work_start  = args->work_start;
//   int work_end    = args->work_end;
//
//   int N           = args->matrix.N;
//   int* matA       = args->matrix.matA;
//   int* matB       = args->matrix.matB;
//   int* output     = args->matrix.output;
//  
//   for(int rowA = 0; rowA < N; rowA +=2) {
//     for(int iter = 0; iter < N; iter+=2){
//       int thread_sum = 0;
//       int indexC = 0;
//       for(int colB = work_start; colB < work_end; colB++){
//    
//         int rowC = rowA >> 1;
//         int colC = colB >> 1;
//         indexC = rowC*(N>>1) + colC;
//
//         thread_sum += matA[rowA*N      + iter    ] * matB[iter*N     + colB];
//         thread_sum += matA[rowA*N      + (iter+1)] * matB[(iter+1)*N + colB];
//         thread_sum += matA[(rowA+1)*N  + iter    ] * matB[iter*N     + colB];
//         thread_sum += matA[(rowA+1)*N  + (iter+1)] * matB[(iter+1)*N + colB];
//
//        
//       }
//
//       pthread_mutex_lock(&lock);
//       output[indexC] += thread_sum ;
//       pthread_mutex_unlock(&lock);
//     }
//   }
//   return NULL;
// }




