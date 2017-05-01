#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <omp.h>
#include "sort.hh"

int TotalThreads ;
int LeastProcesses;
int binarySearch(keytype* b,keytype searchTerm,int startIndex,int endIndex){
    int low = startIndex;
    int high = endIndex;
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
int compare (const void* a, const void* b)
{
  keytype ka = *(const keytype *)a;
  keytype kb = *(const keytype *)b;
  if (ka < kb)
    return -1;
  else if (ka == kb)
    return 0;
  else
    return 1;
}
void merge(keytype* a,keytype *sorted,int p,int q,int w,int r,int copyStart){

int size1 = q-p+1;
int size2 = r-w+1;
int m = p;
int n = w;
int index = copyStart;

while(m<=q && n<=r){
      if(a[m]<a[n]){
        sorted[index] = a[m];
        m++;
      }
      else{
        sorted[index] = a[n];
        n++;
      }
      index++;
  }
  while(m<=q)
    sorted[index++] = a[m++];

  while(n<=r)
    sorted[index++] = a[n++];

}


void paMerge(keytype *a,keytype *sorted,int aStart,int aEnd,int bStart,int bEnd,int copy){

	int asize = aEnd-aStart+1;
  int bsize = bEnd-bStart+1;
  int size = asize+bsize;

  if(size<=LeastProcesses){
		merge(a,sorted,aStart,aEnd,bStart,bEnd,copy);
	}
	else{

					
		int searchIndex = (aStart+aEnd)/2;
		keytype searchTerm = a[searchIndex];
		// printf ("Search term %lu\n",searchTerm);
		int foundIndex = binarySearch(a,searchTerm,bStart,bEnd);
		int copyAt = copy+(searchIndex-aStart)+(foundIndex-bStart)+1;
		sorted[copyAt-1] = searchTerm;
		#pragma omp task
		{
			paMerge(a,sorted,aStart,searchIndex-1,bStart,foundIndex-1,copy);
		}
		paMerge(a,sorted,searchIndex+1,aEnd,foundIndex,bEnd,copyAt);
		#pragma omp taskwait
								
	}


}

void mergeSort(keytype* a,keytype* sortedArray,int left,int right){

int mid ;
 if(left<right){
	mid = left+(right-left)/2;
#pragma omp task
	{
  // printf("mid,%d\n",mid);
	mergeSort(a,sortedArray,left,mid);
	}
	mergeSort(a,sortedArray,mid+1,right);
	// merge(a,left,mid,mid+1,right);
	#pragma taskwait
	paMerge(a,sortedArray,left,mid,mid+1,right,left);
	memcpy(a + left, sortedArray + left, (right - left + 1) * sizeof(keytype));
  }
}
void parallelSort (int N, keytype* A) 
{
	keytype* sortedArray = new keytype[N]; 

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
}
