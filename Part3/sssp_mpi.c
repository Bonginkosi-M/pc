#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int Vertices;
int MAXIMUM = 999999;

void Dijkstra(int numVerts, int source, int *Weights, int *dist, MPI_Comm comm)
{

    int numV;
    int firstVertex;
    int lastVertex;
    int localMinPair[2], globalMinPair[2];

    int u, uDistance;
    int num_procs, myRank;

    MPI_Comm_size(comm, &num_procs);
    MPI_Comm_rank(comm, &myRank);

    numV = numVerts / num_procs;
    firstVertex = myRank * numV;
    lastVertex = firstVertex + numV - 1;

    for (int j = 0; j < numV; j++)
    {
        dist[j] = Weights[source * numV + j];
    }

    int *visited = new int[numV];
    for (int j = 0; j < numV; j++)
    {
        visited[j] = 1;
    }
    if (source >= firstVertex && source <= lastVertex)
    {
        visited[source - firstVertex] = 0;
    }

    for (int i = 0; i < numVerts; i++)
    {

        localMinPair[0] = MAXIMUM;
        localMinPair[1] = -1;

        for (int j = 0; j < numV; j++)
        {

            if (visited[j] && dist[j] < localMinPair[0])
            {
                localMinPair[0] = dist[j];
                localMinPair[1] = firstVertex + j;
            }
        }

        MPI_Allreduce(localMinPair, globalMinPair, 1, MPI_2INT, MPI_MINLOC, comm);

        uDistance = globalMinPair[0];
        u = globalMinPair[1];

        if (u == localMinPair[1])
        {
            visited[u - firstVertex] = 0;
        }

        for (int j = 0; j < numV; j++)
        {

            if (visited[j] && ((uDistance + Weights[u * numV + j]) < dist[j]))
            {
                dist[j] = uDistance + Weights[u * numV + j];
            }
        }
    }
}

int main(int argc, char *argv[])
{

    // ThreadVerts is number of vertices for each thread/proc
    int Rank, num_procs, ThreadVerts, RootNode;
    // int *dist;

    // int *Temp;
    vector<vector<int>> Board;

    fstream Graph;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    if (Rank == 0)
    {

        if (argc != 2)
        {
            cout << "Only 2 arguments expected" << endl;
            exit(0);
        }
        RootNode = atoi(argv[1]);

        Graph.open("graphs.txt");
        Graph >> Vertices;
        Board.resize(Vertices, vector<int>(Vertices, 0));

        for (int i = 0; i < Vertices; i++)
        {

            for (int j = 0; j < Vertices; j++)
            {

                Graph >> Board[i][j];
            }
        }
    }

    auto start = chrono::high_resolution_clock::now(); // start the timing
    MPI_Bcast(&RootNode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *distance = new int[Vertices];
    dist = new int[Vertices];
    Temp = new int[Vertices * Vertices];

    ThreadVerts = Vertices / num_procs;

    int *Weight = new int[ThreadVerts * Vertices];
    int *LDis = new int[ThreadVerts * Vertices];

    if (Rank == 0)
    {

        for (int i = 0; i < num_procs; i++)
        {

            for (int j = 0; j < Vertices; j++)
            {

                for (int k = 0; k < ThreadVerts; k++)
                {
                    Temp[i * Vertices * ThreadVerts + j * ThreadVerts + k] = Board[j][i * ThreadVerts + k];
                }
            }
        }
    }

    MPI_Scatter(Temp, ThreadVerts * Vertices, MPI_INT, Weight, ThreadVerts * Vertices, MPI_INT, 0, MPI_COMM_WORLD);
    Dijkstra(Vertices, RootNode, Weight, LDis, MPI_COMM_WORLD);
    MPI_Gather(LDis, ThreadVerts, MPI_INT, distance, ThreadVerts, MPI_INT, 0, MPI_COMM_WORLD);
    auto finish = chrono::high_resolution_clock::now(); // stop the timing

    if (Rank == 0)
    {
        long long timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();

        cout << "MPI Parallel time taken for execution: " << timeTaken << " microseconds" << endl;
    }

    MPI_Finalize();
    return 0;
}