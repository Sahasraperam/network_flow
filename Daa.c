#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 1000

// A queue structure for BFS
typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

// Queue operations
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(Queue* q) {
    return q->front == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_VERTICES - 1)
        return; // Queue is full
    
    if (q->front == -1)
        q->front = 0;
    
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q))
        return -1;
    
    int item = q->items[q->front];
    
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    
    return item;
}

// Edmonds-Karp algorithm implementation
int edmonds_karp(int graph[MAX_VERTICES][MAX_VERTICES], int n, int source, int sink) {
    int residual_graph[MAX_VERTICES][MAX_VERTICES];
    int parent[MAX_VERTICES];
    int max_flow = 0;
    int u, v;
    
    // Create residual graph
    for (u = 0; u < n; u++) {
        for (v = 0; v < n; v++) {
            residual_graph[u][v] = graph[u][v];
        }
    }
    
    // Augment the flow while there is a path from source to sink
    while (true) {
        // Use BFS to find an augmenting path
        bool visited[MAX_VERTICES] = {false};
        Queue q;
        initQueue(&q);
        
        enqueue(&q, source);
        visited[source] = true;
        parent[source] = -1;
        
        // BFS loop
        bool path_found = false;
        while (!isEmpty(&q) && !path_found) {
            u = dequeue(&q);
            
            for (v = 0; v < n; v++) {
                if (!visited[v] && residual_graph[u][v] > 0) {
                    enqueue(&q, v);
                    parent[v] = u;
                    visited[v] = true;
                    
                    if (v == sink) {
                        path_found = true;
                        break;
                    }
                }
            }
        }
        
        // If no augmenting path was found, we're done
        if (!visited[sink])
            break;
        
        // Find the minimum residual capacity along the augmenting path
        int path_flow = INT_MAX;
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            if (residual_graph[u][v] < path_flow)
                path_flow = residual_graph[u][v];
        }
        
        // Update residual capacities of the edges and reverse edges along the path
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }
        
        // Add path flow to overall flow
        max_flow += path_flow;
    }
    
    return max_flow;
}

int main() {
    int graph[MAX_VERTICES][MAX_VERTICES] = {0};
    int n = 6; // Number of vertices
    
    // Add edges with capacities
    graph[0][1] = 16;
    graph[0][2] = 13;
    graph[1][2] = 10;
    graph[1][3] = 12;
    graph[2][1] = 4;
    graph[2][4] = 14;
    graph[3][2] = 9;
    graph[3][5] = 20;
    graph[4][3] = 7;
    graph[4][5] = 4;
    
    int source = 0;
    int sink = 5;
    
    int max_flow = edmonds_karp(graph, n, source, sink);
    
    printf("The maximum possible flow is %d\n", max_flow);
    
    return 0;
}
