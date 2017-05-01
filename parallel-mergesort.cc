/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"
#include <omp.h>


int TotalThreads ;
int LeastProcesses;
int binarySearch(keytype* b,keytype searchTerm,int startIndex,int endIndex){
    int low = startIndex;
    int high = MAX(startIndex,endIndex+1);
    int mid;
    while(low<high){
          mid = low + (high-low)/2;
          if(b[mid]>=searchTerm){
            high = mid;
          }
          else if(b[mid]<searchTerm){
            low = mid+1;
          }
        }
      return high;
}
void merge(keytype* a,keytype *sorted,int p,int q,int w,int r,int copyStart){

int size1 = q-p+1;
int size2 = r-w+1;
keytype* temp1 = newKeys (size1);
keytype* temp2 = newKeys (size2);
// printf("p is,%d\n",p);
// printf("q is,%d\n",q);
// printf("r is %d\n",r);
// printf("w is %d\n",w);
// printf("merge\n");

memcpy (temp1, a+p, size1 * sizeof (keytype));
memcpy (temp2, a+w, size2 * sizeof (keytype));

int m = 0;
int n = 0;
int index = copyStart;

while(m<size1 && n<size2){
      if(temp1[m]<temp2[n]){
        sorted[index] = temp1[m];
        m++;
      }
      else{
        sorted[index] = temp2[n];
        n++;
      }
      index++;
  }
  while(m<size1)
    sorted[index++] = temp1[m++];

  while(n<size2)
    sorted[index++] = temp2[n++];

}


void paMerge(keytype *a,keytype *sorted,int aStart,int aEnd,int bStart,int bEnd,int copy){

	int asize = aEnd-aStart+1;
  int bsize = bEnd-bStart+1;
  int size = asize+bsize;

  if(size<=3){
		merge(a,sorted,aStart,aEnd,bStart,bEnd,copy);
	}
	else{

						if(asize<bsize){
							int exchange ;

							exchange = aStart;
							aStart = bStart;
							bStart = exchange;

							exchange = aEnd;
							aEnd = bEnd;
							bEnd = exchange;

							exchange = asize;
							asize = bsize;
							bsize = exchange;

						}

						if(asize == 0){
							return;
						}
					  else{
												int searchIndex = (aStart+aEnd)/2;
											  keytype searchTerm = a[searchIndex];
											  // printf ("Search term %lu\n",searchTerm);
											  int foundIndex = binarySearch(a,searchTerm,bStart,bEnd);
												int copyAt = copy+(searchIndex-aStart)+(foundIndex-bStart)+1;
												sorted[copyAt-1] = searchTerm;
											  paMerge(a,sorted,aStart,searchIndex-1,bStart,foundIndex-1,copy);
											  paMerge(a,sorted,searchIndex+1,aEnd,foundIndex,bEnd,copyAt);

								}
	}


}

void mergeSort(keytype* a,keytype* temp,int left,int right){

int mid ;

if(left<right){
	mid = left+(right-left)/2;
  // printf("mid,%d\n",mid);
	#pragma omp task
	{
	mergeSort(a,temp,left,mid);
	}
	mergeSort(a,temp,mid+1,right);
	#pragma taskwait
	// merge(a,left,mid,mid+1,right);
  paMerge(a,temp,left,mid,mid+1,right,left);
	memcpy(a + left, temp + left, (right - left + 1) * sizeof(keytype));
  }
}

void
parallelSort (int N, keytype* A)
{
  /* Lucky you, you get to start from scratch */
 #pragma omp parallel
  {
  	TotalThreads = omp_get_num_threads();
  	LeastProcesses = N/TotalThreads;
    #pragma omp master
    {
      printf("Threads spawned: %d\n", TotalThreads);
    }

	  #pragma omp single
    {
      mergeSort(A,sortedArray, 0, N-1); 
    }
  }
//   keytype* sortedArray = newKeys(N);
//   mergeSort(A,sortedArray, 0, N-1);
}

/* eof */
