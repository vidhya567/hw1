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
keytype* parallel_Merge(keytype* a,keytype* b,int m,int n,int base){
  int searchIndex = m/2;
  keytype searchTerm = a[serachIndex];
  int foundIndex = binarySearch(b,searchTerm,n);
  int a2_size = m-searchindex;
  int b2_size = n-foundindex;
  keytype* A1 = newCopy(searchIndex,a);
  keytype* B1 = newCopy(foundIndex,b);
  keytype* A2 = newKeys(a2_size2);
  keytype* B2 = newKeys(b2_size2);
  memcpy (A2, a+searchIndex, size1 * sizeof (keytype));
  memcpy (B2, b+foundIndex , size1 * sizeof (keytype));
  keytype* c1 = parallel_Merge(A1,B1,searchIndex,foundIndex,base+1);
  keytype* c2 = parallel_Merge(A2,B2,a2_size,b2_size2,base+1);
  keytype* result = combine(c1,c2);
  return result;
}
int binarySearch(keytype* b,keytype searchTerm,int size){
    int low = 0;
    int high = size;
    int mid = size/2;
    while(low<high){
          mid = low + (high-low)/2;
          if(b[mid]>=searchTerm){
            high = mid;
          }
          else if(b[mid]<searchTerm){
            low = mid+1;
          }
      return mid;
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
