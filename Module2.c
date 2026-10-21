/*
If your professor asks:

"What is the purpose of Module 2?"

Say:

“Module 2 calculates a weighted cost for each WiFi link using distance,
latency, interference, signal strength and user load. 
It then applies Prim's Minimum Spanning Tree algorithm to 
select the minimum-cost connections required to keep all access 
points connected while removing unnecessary links.”

And if asked "Why Prim?":

“Prim's algorithm is used to construct a Minimum Spanning Tree,
which connects all APs with minimum total edge cost and avoids unnecessary connections.”

One important limitation to remember: your current cost formula uses the 
raw values directly, so the different parameters are not normalized to a 
common scale. For a stronger final project, this is one of the things I'd improve.
*/

#include <stdio.h>

#define MAX_AP 20
#define MAX_EDGES 50

struct Edge
{
    int ap1;
    int ap2;

    float distance;
    float latency;
    float interference;
    float signal;
    float users;

    float cost;
};

void runModule2()
{
    struct Edge edge[MAX_EDGES];

    float graph[MAX_AP][MAX_AP];

    int n;
    int e;

    int i;
    int j;

    FILE *file;
    FILE *output;

    /* Open network data */

    file = fopen("network.txt", "r");

    if (file == NULL)
    {
        printf("\nPlease run Module 1 first.\n");
        return;
    }

    fscanf(file, "%d %d", &n, &e);

    /* Skip AP information */

    float temp1;
    float temp2;
    float temp3;
    float temp4;
    int temp5;
    float temp6;
    int tempID;

    for (i = 0; i < n; i++)
    {
        fscanf(file, "%d %f %f %f %f %d %f",
               &tempID,
               &temp1,
               &temp2,
               &temp3,
               &temp4,
               &temp5,
               &temp6);
    }

    /* Read connections */

    for (i = 0; i < e; i++)
    {
        fscanf(file, "%d %d %f %f %f %f %f",
               &edge[i].ap1,
               &edge[i].ap2,
               &edge[i].distance,
               &edge[i].latency,
               &edge[i].interference,
               &edge[i].signal,
               &edge[i].users);
    }

    fclose(file);

    /* Initialize graph */

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            graph[i][j] = 0;
        }
    }

    /* Calculate link costs */

    printf("\n========================================\n");
    printf("       MODULE 2: TOPOLOGY OPTIMIZATION\n");
    printf("========================================\n");

    for (i = 0; i < e; i++)
    {
        /*
           Simple weighted cost

           Distance       20%
           Latency        30%
           Interference   25%
           Signal         10%
           Users          15%
        */

        edge[i].cost =
            (edge[i].distance * 0.20) +
            (edge[i].latency * 0.30) +
            (edge[i].interference * 0.25) +
            ((100 - edge[i].signal) * 0.10) +
            (edge[i].users * 0.15);

        graph[edge[i].ap1 - 1][edge[i].ap2 - 1]
            = edge[i].cost;

        graph[edge[i].ap2 - 1][edge[i].ap1 - 1]
            = edge[i].cost;

        printf("\nAP%d <--> AP%d",
               edge[i].ap1,
               edge[i].ap2);

        printf("   Cost = %.2f",
               edge[i].cost);
    }

    /* Prim's Algorithm */

    int selected[MAX_AP];

    for (i = 0; i < n; i++)
    {
        selected[i] = 0;
    }

    selected[0] = 1;

    output = fopen("optimized.txt", "w");

    if (output == NULL)
    {
        printf("\nError creating optimized.txt\n");
        return;
    }

    fprintf(output, "%d\n", n);

    float totalCost = 0;

    printf("\n\n========================================\n");
    printf("       OPTIMIZED CONNECTIONS\n");
    printf("========================================\n");

    for (i = 0; i < n - 1; i++)
    {
        /*
           Start with no minimum connection
        */

        float minimum = 0;

        int from = -1;
        int to = -1;

        for (j = 0; j < n; j++)
        {
            if (selected[j] == 1)
            {
                int k;

                for (k = 0; k < n; k++)
                {
                    /*
                       graph[j][k] != 0 means
                       a connection exists
                    */

                    if (selected[k] == 0 &&
                        graph[j][k] != 0)
                    {
                        if (from == -1 ||
                            graph[j][k] < minimum)
                        {
                            minimum = graph[j][k];

                            from = j;
                            to = k;
                        }
                    }
                }
            }
        }

        if (from == -1)
        {
            printf("\nNetwork is not fully connected.\n");
            fclose(output);
            return;
        }

        selected[to] = 1;

        printf("\nAP%d <--> AP%d   Cost = %.2f",
               from + 1,
               to + 1,
               minimum);

        fprintf(output, "%d %d %.2f\n",
                from + 1,
                to + 1,
                minimum);

        totalCost += minimum;
    }

    fclose(output);

    printf("\n\nOriginal connections : %d", e);

    printf("\nOptimized connections: %d",
           n - 1);

    printf("\nConnections removed  : %d",
           e - (n - 1));

    printf("\nTotal optimized cost : %.2f\n",
           totalCost);

    printf("\nOptimized data saved in optimized.txt\n");

    printf("\nMODULE 2 COMPLETED.\n");
}

/*
The project assigns weights:

Parameter	Weight
Distance	20%
Latency	30%
Interference	25%
Signal penalty	10%
Users	15%
Total	100%
Important!

These percentages are project-defined weights, not universal WiFi standards.

The idea is:

Higher cost = less desirable connection.*/

/*
Why (100 - signal)?

Suppose signal is:

90

That's good.

We don't want good signal to increase the cost.

Therefore:

100 - 90 = 10

Small penalty.

But if signal is:

30

Then:

100 - 30 = 70

Large penalty.

So:

Strong signal → low penalty → lower cost
Weak signal → high penalty → higher cost

This is called a signal penalty in our simplified model.
*/

/*
AP1 → index 0
AP2 → index 1
AP3 → index 2

That's why:

ap1 - 1

is used.
*/

/*
Why do we store it twice?
graph[edge[i].ap2 - 1][edge[i].ap1 - 1]
    = edge[i].cost;

Because the WiFi connection is considered bidirectional.

If:

AP1 ↔ AP2

then:

AP1 → AP2

and

AP2 → AP1

both exist.

So the matrix stores:

       AP1    AP2
AP1     0     20.5
AP2    20.5    0
*/

/*
Display selected connection
printf("\nAP%d <--> AP%d   Cost = %.2f",
       from + 1,
       to + 1,
       minimum);

If:

from = 0
to = 2
minimum = 15.50

It prints:

AP1 <--> AP3   Cost = 15.50

Again +1 converts C array index back to human AP number.
*/