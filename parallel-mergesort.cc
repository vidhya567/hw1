/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.hh"
void merge(keytype* a,int p,int q,int r){

int size1 = q-p+1;
int size2 = r-q;
keytype* temp1 = newKeys (size1);
keytype* temp2 = newKeys (size2);
// printf("p is,%d\n",p);
// printf("q is,%d\n",q);
// printf("r is %d\n",r);

memcpy (temp1, a+p, size1 * sizeof (keytype));
memcpy (temp2, a+q+1, size1 * sizeof (keytype));

int m = 0;
int n = 0;
int index = p;

while(m<size1 && n<size2){
      if(temp1[m]<temp2[n]){
        a[index] = temp1[m];
        m++;
      }
      else{
        a[index] = temp2[n];
        n++;
      }
      index++;
  }
  while(m<size1)
    a[index++] = temp1[m++];

  while(n<size2)
    a[index++] = temp2[n++];

}

void mergeSort(keytype* a,int left,int right){

int mid ;

if(left<right){
	mid = left+(right-left)/2;
  // printf("mid,%d\n",mid);
	mergeSort(a,left,mid);
	mergeSort(a,mid+1,right);
	merge(a,left,mid,right);
  }
}
void
parallelSort (int N, keytype* A)
{
  /* Lucky you, you get to start from scratch */
  mergeSort(A, 0, N-1);
}

/* eof */
