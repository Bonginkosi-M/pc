#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

const int numNodes = 16;

int minDistance(int dist[], bool done[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < numNodes; v++){
        if (done[v] == false && dist[v] <= min){
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

void printSolution(int dist[])
{
    printf("Node \t\t Distance from Source\n");
    for (int i = 1; i < numNodes; i++){
        printf("%d \t\t\t\t %d\n", i, dist[i]);
    }
}

void dijkstra(int graph[numNodes][numNodes], int src, int dist[numNodes])
{
    bool done[numNodes];
    for (int i = 0; i < numNodes; i++){
        dist[i] = INT_MAX, done[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < numNodes - 1; count++)
    {
        int u = minDistance(dist, done);

        done[u] = true;

        for (int v = 0; v < numNodes; v++){
            if (!done[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]){
                dist[v] = dist[u] + graph[u][v];
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

    start = clock();
    dijkstra(graph, 0, dist);
    end = clock();
    printSolution(dist);
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken in seconds: %f\n", cpu_time_used);
    return 0;
}