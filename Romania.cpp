#include <stdio.h>
#include <iostream>
#include <list>
#include <stack>

//The number of vertices stored in our graph
int vertexCount = 0;
//A 2D array of edges in the format graph[vertexA][vertexB] = distance between vertexA and vertexB
int** graph;
//The output logging file. Don't worry about this one
FILE* output;

/*
AddEdge: Add an edge to the graph from vertex u to vertex v.

Arugments:
    int u: One of the vertices that form the edge
    int v: The other vertice that forms the edge
    int dist: The distance between u and v

Adds an edge to the graph from vertex u to vertex v with the given distance dist.
The graph is represented a a 2D array of integers, where each integer is a distance.
When an edge is added from u to v an edge from v to u is also saved in memory.

Returns void: No value returned
*/
void AddEdge(int u, int v, int dist)
{
    graph[u][v] = dist;
    graph[v][u] = dist;
}

/*
ReadGraphFile: Read graph data in from a file to populate our graph structure.

Arguments:
    char* fname: The name of the file to read

Reads graph data in from a file to populate our graph structure. The format of the file
defines a number of vertices at the top of the file, and edges for the graph for each
line afterwards in the format (U, V, DIST).

Returns bool: Returns true if the file was opened and read successfuly, and false if the file could not be read
*/
bool ReadGraphFile(char* fname)
{
    FILE* file = fopen(fname, "r");
    if (file)
    {
        rewind(file);
        int a, b, d;
        
        fscanf(file, "%d", &vertexCount);
        //fscanf_s(file, "%d", &vertexCount);

        graph = new int*[vertexCount];
        for (int u = 0; u < vertexCount; u++)
        {
            graph[u] = new int[vertexCount];

            for (int v = 0; v < vertexCount; v++)
            {
                graph[u][v] = 0;
            }
        }

        while (fscanf(file, "%d %d %d", &a, &b, &d) != EOF)
//        while (fscanf_s(file, "%d %d %d", &a, &b, &d) != EOF)
        {
            AddEdge(a, b, d);
        }

        fclose(file);

        return true;
    }
    else
    {
        printf("Failed to read file...\n");
        return false;
    }
}

/*
LogVertex: Prints the current vertex traveled to and saves the output to a file

Arguments:
    int vert: The vertex that you are currently visiting

In order for us to grade you, you must print the vertex you are travelling to in your algorithm.
This function makes it easier for you to print the proper data. It also saves the data to a file.

Returns void: No values returned
*/
void LogVertex(int vert)
{
    printf("%d\n", vert);
    fprintf(output, "%d\n", vert);
}

/*
SetartLogging: Opens the logging file for later writing

Arguments:
    None

Pretty much just a wrapper for fopen so that we make sure you are writing your logging data to the right file.

Returns void: No values returned
*/
void StartLogging()
{
    output = fopen("log.txt", "w");
}

/*
EndLogging: Closes the logging file properly

Arguments:
    None

Pretty much just a wrapper for fclose so that we make sure you close the right file.

Returns void: No values returned
*/
void EndLogging()
{
    fclose(output);
}

bool bfs(int start, int end);
bool dfs(int start, int end);
bool ids(int start, int end);
bool recursive_dfs(int start, int end, int limit);

int main(int argc, char** argv)
{
    //Setup
    //-----------------------------------
    if (argc == 4)
    {
        if (!ReadGraphFile(argv[1]))
            return 2;
    }
    else
    {
        printf("No file argument given...\n");
        return 1;
    }
    StartLogging();
    //-------------------------------------

    int start = std::stoi(argv[2]);
    int end = std::stoi(argv[3]);

    //Your code
    #if defined(IMPLENTING_BFS)
    bfs(start, end);
    #elif defined(IMPLEMENTING_DFS)
    dfs(start, end);
    #elif defined(IMPLENTING_IDS)
    ids(start, end);
    #endif


    //Cleanup
    //-------------------------------------
    EndLogging();
    //-------------------------------------

    return 0;
}

bool bfs(int start, int end)
{
    int visited[vertexCount];
    for(int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    std::list<int> queue;

    visited[start] = true;
    queue.push_back(start);

    while(!queue.empty())
    {
        int currentNode = queue.front();
        queue.pop_front();

        LogVertex(currentNode);

        if(currentNode == end)
        {
            return true;
        }

        for(int i = 0; i < vertexCount; i++)
        {
            int distance = graph[currentNode][i];
            if(distance != 0 && !visited[i])
            {

                visited[i] = true;
                queue.push_back(i);
            }
        }
    }

    return false;
}

bool dfs(int start, int end)
{
    bool visited[vertexCount];
    for(int i = 0; i < vertexCount; i++)
    {
        visited[i] = false;
    }

    std::stack<int> stack;

    visited[start] = true;
    stack.push(start);

    while(!stack.empty())
    {
        int currentNode = stack.top();
        stack.pop();

        LogVertex(currentNode);

        if(currentNode == end)
        {
            return true;
        }

        for(int i = 0; i < vertexCount; i++)
        {
            int distance = graph[currentNode][i];
            if(distance != 0 && !visited[i])
            {
                visited[i] = true;
                stack.push(i);
            }
        }
    }

    return false;
}

bool ids(int start, int end)
{
    for(int i = 0; i < vertexCount; i++)
    {
        if(recursive_dfs(start, end, i))
        {
            return true;
        }
    }

    return false;
}

bool recursive_dfs(int start, int end, int limit)
{
    LogVertex(start);

    if(start == end)
    {
        return true;
    }

    if(limit <= 0)
    {
        return false;
    }

    for(int i = 0; i < vertexCount; i++)
    {
        int distance = graph[start][i];
        if(distance != 0)
        {
            if(recursive_dfs(i, end, limit - 1))
            {
                return true;
            }
        }
    }

    return false;
}
