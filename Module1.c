/*

If your professor asks:

"What does Module 1 do?"

Say:

Module 1 performs network modeling. It takes Access Point and connection information, calculates the health score of each AP, detects problems such as weak signal, high latency and interference, and stores the network data in network.txt for Module 2.

In one line:

Module 1 = Input → AP Health → Problem Detection → Network Data → network.txt.

*/

#include <stdio.h>

#define MAX_AP 20
#define MAX_EDGES 50

struct AP
{
    int id;
    float signal;
    float bandwidth;
    float latency;
    float interference;
    int users;
    float health;
};

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

void runModule1()
{
    struct AP ap[MAX_AP];
    struct Edge edge[MAX_EDGES];

    int n;
    int e;
    int i;

    FILE *file;

    printf("       MODULE 1: NETWORK MODELING\n");
    printf("========================================\n");

    printf("\nEnter number of Access Points: ");
    scanf("%d", &n);

    if (n < 2 || n > MAX_AP)
    {
        printf("Invalid number of APs.\n");
        return;
    }

    /* Enter AP information */

    for (i = 0; i < n; i++)
    {
        ap[i].id = i + 1;

        printf("\n------ AP%d ------\n", i + 1);

        printf("Signal strength (0-100): ");
        scanf("%f", &ap[i].signal);

        printf("Bandwidth (0-100): ");
        scanf("%f", &ap[i].bandwidth);

        printf("Latency (ms): ");
        scanf("%f", &ap[i].latency);

        printf("Interference (0-100): ");
        scanf("%f", &ap[i].interference);

        printf("Number of users: ");
        scanf("%d", &ap[i].users);

        /* Simple health calculation */

        ap[i].health =
            (ap[i].signal * 0.30) +
            (ap[i].bandwidth * 0.20) +
            ((100 - ap[i].latency) * 0.20) +
            ((100 - ap[i].interference) * 0.20) +
            ((100 - ap[i].users) * 0.10);

        if (ap[i].health < 0)
            ap[i].health = 0;

        if (ap[i].health > 100)
            ap[i].health = 100;
    }

    /* Enter connections */

    printf("\nEnter number of WiFi connections: ");
    scanf("%d", &e);

    if (e < 1 || e > MAX_EDGES)
    {
        printf("Invalid number of connections.\n");
        return;
    }

    for (i = 0; i < e; i++)
    {
        printf("\n------ Connection %d ------\n", i + 1);

        printf("First AP: ");
        scanf("%d", &edge[i].ap1);

        printf("Second AP: ");
        scanf("%d", &edge[i].ap2);

        printf("Distance: ");
        scanf("%f", &edge[i].distance);

        printf("Latency: ");
        scanf("%f", &edge[i].latency);

        printf("Interference (0-100): ");
        scanf("%f", &edge[i].interference);

        printf("Signal (0-100): ");
        scanf("%f", &edge[i].signal);

        printf("User load: ");
        scanf("%f", &edge[i].users);

        edge[i].cost = 0;
    }

    /* Display AP report */

    printf("\n========================================\n");
    printf("          AP HEALTH REPORT\n");
    printf("========================================\n");

    for (i = 0; i < n; i++)
    {
        printf("\nAP%d", ap[i].id);
        printf("\nSignal       : %.1f", ap[i].signal);
        printf("\nBandwidth    : %.1f", ap[i].bandwidth);
        printf("\nLatency      : %.1f ms", ap[i].latency);
        printf("\nInterference : %.1f", ap[i].interference);
        printf("\nUsers        : %d", ap[i].users);
        printf("\nHealth Score : %.1f", ap[i].health);

        if (ap[i].health >= 70)
            printf("\nStatus       : GOOD\n");
        else if (ap[i].health >= 40)
            printf("\nStatus       : WARNING\n");
        else
            printf("\nStatus       : CRITICAL\n");
    }

    /* Problem detection */

    printf("\n========================================\n");
    printf("          PROBLEM DETECTION\n");
    printf("========================================\n");

    for (i = 0; i < n; i++)
    {
        printf("AP%d: ", ap[i].id);

        if (ap[i].signal < 50)
            printf("Weak Signal ");

        if (ap[i].latency > 50)
            printf("High Latency ");

        if (ap[i].interference > 60)
            printf("High Interference ");

        if (ap[i].users > 50)
            printf("High User Load ");

        if (ap[i].signal >= 50 &&
            ap[i].latency <= 50 &&
            ap[i].interference <= 60 &&
            ap[i].users <= 50)
        {
            printf("No major problem");
        }

        printf("\n");
    }

    /*
       Save data into network.txt
    */

    file = fopen("network.txt", "w");

    if (file == NULL)
    {
        printf("\nError creating network.txt\n");
        return;
    }

    fprintf(file, "%d %d\n", n, e);

    for (i = 0; i < n; i++)
    {
        fprintf(file, "%d %.2f %.2f %.2f %.2f %d %.2f\n",
                ap[i].id,
                ap[i].signal,
                ap[i].bandwidth,
                ap[i].latency,
                ap[i].interference,
                ap[i].users,
                ap[i].health);
    }

    for (i = 0; i < e; i++)
    {
        fprintf(file, "%d %d %.2f %.2f %.2f %.2f %.2f\n",
                edge[i].ap1,
                edge[i].ap2,
                edge[i].distance,
                edge[i].latency,
                edge[i].interference,
                edge[i].signal,
                edge[i].users);
    }

    fclose(file);

    printf("\nData saved successfully in network.txt\n");
    printf("\nMODULE 1 COMPLETED.\n");
}

/*              **************************
Why did we choose 30% for signal?

Because for our simplified project health score, we decided signal should have the highest influence. A weak signal directly affects WiFi connectivity, so we gave it the largest weight.

But this is important for your viva:

30% is not a universal WiFi standard. It is a design assumption/weight chosen for our simulation.

If your professor asks "Why exactly 30%?", say:

“We assigned 30% to signal strength as a project-defined weighting because signal quality is one of the major factors affecting WiFi connectivity. The weights are configurable and can be changed based on real network data.”

Even better for your project

Since your professor said the project is too basic, we can make this more scientific later by not arbitrarily choosing 30%. We could derive the weights using:

network performance data,
expert-defined priorities,
or a scoring/normalization method.

That would make your project much stronger academically.
*/