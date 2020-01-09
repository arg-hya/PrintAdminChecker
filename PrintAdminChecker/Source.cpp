
#include "stdafx.h"

#include <iostream>

// A utility function ot print an array of size n
void printArray(int arr[], int n)
{
	int i;
	for (i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

void insertionSort1(int arr[], int n)
{
	for (int i = 1; i < n; i++)
	{
		int key = arr[i];
		for (int j = i - 1; j >= 0 ; j--)
		{
			if (key < arr[j])
			{
				int temp = arr[j];
				arr[j] = key;
				arr[j + 1] = temp;
			}
		}
	}
}

/* Driver program to test insertion sort */
int main()
{
	int arr[] = { 12, 11, 13, 5, 6 };
	int n = sizeof(arr) / sizeof(arr[0]);

	insertionSort1(arr, n);
	printArray(arr, n);

	return 0;
}