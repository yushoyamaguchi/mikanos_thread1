#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../app_thread.hpp"


#define NUM_ITEMS 10

struct pair_int{
  int front;
  int back;
};


void mergeSort( int array_size);
void m_sort(struct pair_int* p);
void m_sort_first(int64_t int_p);
void merge(int left, int mid, int right);

int numbers[NUM_ITEMS];
int temp[NUM_ITEMS];



extern "C" int main()
{
  int i;

  numbers[0]=100;
  numbers[1]=67;
  numbers[2]=11;
  numbers[3]=166;
  numbers[4]=3;
  numbers[5]=10;
  numbers[6]=35;
  numbers[7]=99;
  numbers[8]=44;
  numbers[9]=51;


  //perform merge sort on array
  mergeSort(NUM_ITEMS);

  printf("Done with sort.\n");

  for (i = 0; i < NUM_ITEMS; i++)
    printf("%i\n", numbers[i]);

  return 0;
}

void test_func(int64_t arg1){
    SyscallLogString(kWarn,"from user thread test func\n");
    printf("p=%ld\n",arg1);

    SyscallExit(0);
}



void mergeSort(int array_size)
{
  int mid=(array_size-1)/2;
  struct pair_int p1={0,mid};
  struct pair_int p2={mid+1,array_size-1};
  int64_t int_p1=reinterpret_cast<uint64_t>(&p1);
  int64_t int_p2=reinterpret_cast<uint64_t>(&p2);
  app_thread_t t1,t2;
  void (*f)(int64_t);
  f=&m_sort_first;
  app_thread_create(&t1,(void*)f,int_p1);
  app_thread_create(&t2,(void*)f,int_p2);
  app_thread_join(&t1);
  app_thread_join(&t2);
  merge(0,mid+1,array_size-1);
}

void m_sort_first(int64_t int_p)
{
  struct pair_int* p=reinterpret_cast<struct pair_int*>(int_p);
  int mid,left,right;
  left=p->front;
  right=p->back;

  if (right > left)
  {
    mid = (right + left) / 2;
    struct pair_int p1={left,mid};
    struct pair_int p2={mid+1,right};
    m_sort(&p1);
    m_sort(&p2);

    merge(left, mid+1, right);
  }
  SyscallExit(0);
}



void m_sort(struct pair_int* p)
{
  int mid,left,right;
  left=p->front;
  right=p->back;

  if (right > left)
  {
    mid = (right + left) / 2;
    struct pair_int p1={left,mid};
    struct pair_int p2={mid+1,right};
    m_sort(&p1);
    m_sort(&p2);

    merge(left, mid+1, right);
  }
}




void merge(int left, int mid, int right)
{
  int i, left_end, num_elements, tmp_pos;

  left_end = mid - 1;
  tmp_pos = left;
  num_elements = right - left + 1;

  while ((left <= left_end) && (mid <= right))
  {
    if (numbers[left] <= numbers[mid])
    {
      temp[tmp_pos] = numbers[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;
    }
    else
    {
      temp[tmp_pos] = numbers[mid];
      tmp_pos = tmp_pos + 1;
      mid = mid + 1;
    }
  }

  while (left <= left_end)
  {
    temp[tmp_pos] = numbers[left];
    left = left + 1;
    tmp_pos = tmp_pos + 1;
  }
  while (mid <= right)
  {
    temp[tmp_pos] = numbers[mid];
    mid = mid + 1;
    tmp_pos = tmp_pos + 1;
  }

  for (i=0; i <= num_elements; i++)
  {
    numbers[right] = temp[right];
    right = right - 1;
  }
}
