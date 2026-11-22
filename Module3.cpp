#include <iostream>
#include <cstdio>
using namespace std;

#define MAX_AP 20
#define INF 1000000000

// Find the unvisited node with minimum distance
int findMinimum(double distance[], bool visited[], int n)
{
    double minimum = INF;
    int index = -1;

    for (int i = 0; i < n; i++)
    {
        if (!visited[i] && distance[i] < minimum)
        {
            minimum = distance[i];
            index = i;
        }
    }

    return index;
}

// Dijkstra's Algorithm
void dijkstra(double graph[MAX_AP][MAX_AP], int n,
              int source, int destination)
{
    double distance[MAX_AP];
    bool visited[MAX_AP] = {false};
    int previous[MAX_AP];

    for (int i = 0; i < n; i++)
    {
        distance[i] = INF;
        previous[i] = -1;
    }

    distance[source] = 0;

    for (int i = 0; i < n; i++)
    {
        int u = findMinimum(distance, visited, n);

        if (u == -1)
            break;

        visited[u] = true;

        for (int v = 0; v < n; v++)
        {
            if (graph[u][v] > 0 && !visited[v])
            {
                double newDistance = distance[u] + graph[u][v];

                if (newDistance < distance[v])
                {
                    distance[v] = newDistance;
                    previous[v] = u;
                }
            }
        }
    }

    if (distance[destination] == INF)
    {
        cout << "\nNo route available between these APs.\n";
        return;
    }

    // Print shortest path
    int path[MAX_AP];
    int count = 0;

    int current = destination;

    while (current != -1)
    {
        path[count++] = current;
        current = previous[current];
    }

    cout << "\nShortest Route: ";

    for (int i = count - 1; i >= 0; i--)
    {
        cout << "AP" << path[i] + 1;

        if (i != 0)
            cout << " -> ";
    }

    printf("\nTotal Route Cost: %.2f\n", distance[destination]);
}


// Module 3
extern "C" void runModule3()
{
    FILE *file = fopen("optimized.txt", "r");

    if (file == NULL)
    {
        cout << "\nError: optimized.txt not found!\n";
        cout << "Please run Module 2 first.\n";
        return;
    }

    int n;

    if (fscanf(file, "%d", &n) != 1 || n < 2 || n > MAX_AP)
    {
        cout << "\nInvalid network data!\n";
        fclose(file);
        return;
    }

    double graph[MAX_AP][MAX_AP] = {0};

    int from, to;
    double cost;

    // Read optimized connections
    for (int i = 0; i < n - 1; i++)
    {
        if (fscanf(file, "%d %d %lf", &from, &to, &cost) != 3)
        {
            cout << "\nInvalid connection data!\n";
            fclose(file);
            return;
        }

        if (from < 1 || from > n ||
            to < 1 || to > n ||
            from == to || cost <= 0)
        {
            cout << "\nInvalid connection found!\n";
            fclose(file);
            return;
        }

        from--;
        to--;

        graph[from][to] = cost;
        graph[to][from] = cost;
    }

    fclose(file);

    cout << "\n====================================";
    cout << "\n ROUTING AND PERFORMANCE ANALYSIS";
    cout << "\n====================================";

    cout << "\nTotal Access Points: " << n;
    cout << "\nOptimized Connections: " << n - 1;

    int source, destination;

    cout << "\n\nEnter Source AP (1-" << n << "): ";
    cin >> source;

    cout << "Enter Destination AP (1-" << n << "): ";
    cin >> destination;

    if (cin.fail() || source < 1 || source > n ||
        destination < 1 || destination > n ||
        source == destination)
    {
        cout << "\nInvalid AP selection!\n";
        return;
    }

    source--;
    destination--;

    // Route Analysis
    cout << "\n--- ROUTE ANALYSIS ---";

    dijkstra(graph, n, source, destination);

    // Link Failure Simulation
    cout << "\n\n--- LINK FAILURE SIMULATION ---";

    int ap1, ap2;

    cout << "\nEnter first AP of the link to fail: ";
    cin >> ap1;

    cout << "Enter second AP of the link to fail: ";
    cin >> ap2;

    if (cin.fail() || ap1 < 1 || ap1 > n ||
        ap2 < 1 || ap2 > n || ap1 == ap2)
    {
        cout << "\nInvalid AP selection!\n";
        return;
    }

    ap1--;
    ap2--;

    if (graph[ap1][ap2] == 0)
    {
        cout << "\nThese APs are not directly connected!\n";
        return;
    }

    // Temporarily remove connection
    double failedCost = graph[ap1][ap2];

    graph[ap1][ap2] = 0;
    graph[ap2][ap1] = 0;

    cout << "\nConnection between AP" << ap1 + 1
         << " and AP" << ap2 + 1 << " has failed.";

    cout << "\nChecking alternative route...\n";

    dijkstra(graph, n, source, destination);

    // Restore connection
    graph[ap1][ap2] = failedCost;
    graph[ap2][ap1] = failedCost;

    cout << "\n====================================\n";
    cout << "Module 3 Completed Successfully!\n";
    cout << "====================================\n";
}
/*
What is Dijkstra's Algorithm?

Dijkstra's Algorithm finds the lowest-cost path from a source vertex 
to other vertices in a weighted graph, provided the edge weights are non-negative.

In your project:

Vertices: Access Points.

Edges: Optimized WiFi connections.

Edge weights: The calculated costs from Module 2.

Source: The AP where the route starts.

Destination: The AP where the route ends.

Prim's Algorithm selects the network connections in Module 2; 
Dijkstra finds a route through those connections in Module 3.

4. Important limitations in your current Module 3

Before your final demonstration, there are four things worth improving:

Decimal precision: Your code converts costs to integers instead of preserving 
floating-point values.

Input validation: It should verify that source and destination AP numbers are 
between 1 and n.

Failure simulation: Despite the broader project plan, this code does not yet 
simulate an AP or link failure.

Performance comparison: It displays basic network information, but does not yet 
compare original and optimized latency, interference or cost.

5. Viva-ready answer

Question: Explain Module 3 of your project.

“Module 3 performs routing and performance analysis. It reads the optimized 
network generated by Module 2, represents the connections using an adjacency matrix, 
and asks the user for a source and destination AP. It applies Dijkstra's Algorithm to 
find the minimum-cost route, reconstructs the path using the previous array, and 
displays the route, total route cost and basic network information.”
*/