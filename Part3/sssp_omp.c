#include <stdio.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

const int numNodes = 16;
#define THREAD_NUM 4

int minDistance(int dist[numNodes], bool done[numNodes])
{

    int min = INT_MAX;
    int minNode;
    for (int i = 0; i < numNodes; i++)
    {
        int v = i;
        if (dist[v] <= min && done[v] == false)
        {
            min = dist[v];
            minNode = v;
        }
    }

    return minNode;
}

void printSolution(int dist[])
{
    printf("Node \t\t Distance from Source\n");
    for (int i = 1; i < numNodes; i++)
    {
        printf("%d \t\t\t\t %d\n", i, dist[i]);
    }
}

void dijkstra(int graph[numNodes][numNodes], int src, int dist[numNodes], bool done[numNodes]){

    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < numNodes; i++)
    {
        dist[i] = INT_MAX;
    }

    dist[src] = 0;

    for (int i = 0; i < numNodes - 1; i++)
    {
        int u = minDistance(dist, done);
        done[u] = true;
        int distNum = dist[u];

        #pragma omp parallel firstprivate(distNum)
        #pragma omp for
        for (int j = 0; j < numNodes; j++)
        {
            int v = j;
            int alt = distNum + graph[u][v];
            if (done[v] == false && distNum != INT_MAX && alt < dist[v] && graph[u][v])
            {
                dist[v] = alt;
            }
        }
    }
}

int main()
{

    FILE *ptr;
    int num = 0;
    clock_t start, end;
    double cpu_time_used;

    int graph[numNodes][numNodes];
    int data[numNodes * numNodes];
    int dist[numNodes];
    bool done[numNodes];
    memset(done, false, sizeof done);

    if (numNodes == 6)
    {
        ptr = fopen("0.txt", "r");
    }
    else if (numNodes == 8)
    {
        ptr = fopen("1.txt", "r");
    }
    else if (numNodes == 16)
    {
        ptr = fopen("2.txt", "r");
    }
    else if (numNodes == 64)
    {
        ptr = fopen("3.txt", "r");
    }
    else if (numNodes == 128)
    {
        ptr = fopen("4.txt", "r");
    }
    else if (numNodes == 256)
    {
        ptr = fopen("5.txt", "r");
    }
    else if (numNodes == 384)
    {
        ptr = fopen("6.txt", "r");
    }
    else if (numNodes == 512)
    {
        ptr = fopen("7.txt", "r");
    }
    if (NULL == ptr)
    {
        printf("Unable to open file\n");
        exit(1);
    }

    int i = 0;
    fscanf(ptr, "%d", &num);
    while (!feof(ptr))
    {
        data[i] = num;
        i++;
        fscanf(ptr, "%d", &num);
    }

    i = 0;
    int j, k = 0;
    for (i = 0; i < numNodes; i++)
    {
        for (j = 0; j < numNodes; j++)
        {
            graph[i][j] = data[k++];
        }
    }

    fclose(ptr);

    if (numNodes < 64)
    {
        omp_set_num_threads(1);
    }
    else if (numNodes < 128)
    {
        omp_set_num_threads(2);
    }
    else
    {
        omp_set_num_threads(THREAD_NUM);
    }


    start = clock();
    dijkstra(graph, 0, dist, done);
    end = clock();
    printSolution(dist);
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken in seconds: %f\n", cpu_time_used);
    return 0;
}