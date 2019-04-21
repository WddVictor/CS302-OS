#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <sys/time.h>

#define MAX 1000000
#define RANDNUM_MAX 999
#define random(x) (rand()%x)

void merge(int *,int,int,int);
void merge_sort(int*,int,int);
void generate_random_array(int *);
void display(int *);


int main()
{
	int i;
	int a[MAX] = {0};
	struct timeval start, end;
	double delta;
	srand((int)time(0));
	
	//mark the start time
	gettimeofday(&start, NULL);

	generate_random_array(a);
	// display(a);
	merge_sort(a,0,MAX-1);
    // display(a);
    //mark the end time
	gettimeofday(&end, NULL);
	delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
         end.tv_usec - start.tv_usec) / 1.e6;

	printf("Time cost: %f\n",delta);
	return 0;
}

void merge(int *p,int low,int mid,int high)
{
	int i,k;
	int* temp = (int*)malloc((high-low+1)*sizeof(int));
	int start1 = low;
	int end1 = mid;
	int start2 = mid+1;
	int end2 = high;

	for (k = 0;start1<=end1 && start2<=end2;k++)
	{
		if (p[start1]<p[start2])
			temp[k] = p[start1++];
		else
			temp[k] = p[start2++];
	}

	while (start1<=end1)
		temp[k++] = p[start1++];

	while (start2<=end2)
		temp[k++] = p[start2++];

	for (i = 0;i<high-low+1;i++)
		p[low+i] = temp[i];

	free(temp);
}


void merge_sort(int*p,int low,int high)
{
	int mid = (low+high)/2;

	if (low <high)
	{
		merge_sort(p,low,mid);
		merge_sort(p,mid+1,high);
		merge(p,low,mid,high);
	}
}

void display(int *p){
	for(int i =0;i<MAX;i++){
		printf("%d ", p[i]);
	}
	printf("\n");
}

void generate_random_array(int *p){
	srand((int)time(0));	
	for (int i = 0; i < MAX; i++) 
        p[i] = random(RANDNUM_MAX);
}
 
