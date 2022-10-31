#include <stdio.h>
#include <math.h>
#include <time.h>

const int numElements = 8;

void PrintResults(int arr[numElements])
{//
    for (int i = 0; i < numElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void scanSerial(int numbers[numElements], int numbersout[numElements])
{
    int i = 0;
    numbersout[0] = numbers[0];
    for (i = 1; i < numElements; i++)
    {
        numbersout[i] = numbers[i] + numbersout[i - 1];
    }
}

int main(){
    int numbers[numElements];
    int numbersout[numElements];
    clock_t start, end;
    double cpu_time_used;

    for (int i = 0; i < numElements; i++){
        numbers[i] = rand() % 10;
    }
    printf("Input array: \n");
    PrintResults(numbers);
    printf("");
    printf("Output array: \n");
    start = clock();
    scanSerial(numbers, numbersout);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    PrintResults(numbersout);
    printf("Time taken in seconds: %f\n", cpu_time_used);
}