#include <stdio.h>
#include <math.h>
#include <time.h>

// 0.000040
// 0.000003

const int numElements = 8;

void PrintResults(int arr[numElements])
{//
    for (int i = 0; i < numElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void scanParallel(int numbersout[numElements])
{
    for (long t = 0; t < log2(numElements) - 1; t++)
    {
        long k = pow(2, t + 1);
        long p = pow(2, t);
        for (long i = 0; i < numElements - 1; i += pow(2, t + 1))
        {
            numbersout[i + k - 1] = numbersout[i + p - 1] + numbersout[i + k - 1];
        }
    }
    
    numbersout[numElements - 1] = 0;
    for (long t = log2(numElements) - 1; t >= 0; t--)
    {
        long k = pow(2, t + 1);
        long p = pow(2, t);
        for (long i = 0; i < numElements - 1; i += pow(2, t + 1))
        {
            long temp = numbersout[i + p - 1];
            numbersout[i + p - 1] = numbersout[i + k - 1];
            numbersout[i + k - 1] = temp + numbersout[i + k - 1];
        }
    }
}

int main()
{
    int numbers[numElements];
    int numbersout[numElements];
    clock_t start, end;
    double cpu_time_used;

    for (int i = 0; i < numElements; i++){
        numbers[i] = rand() % 10;
        numbersout[i] = numbers[i];
    }

    printf("Input array: \n");
    PrintResults(numbers);
    printf("");
    printf("Output array: \n");
    start = clock();
    scanParallel(numbersout);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    PrintResults(numbersout);
    printf("Time taken in seconds: %f\n", cpu_time_used);
}