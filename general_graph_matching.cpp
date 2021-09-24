// C++ Implementation of Edmonds Blossoms' Maximum Matching Algorithm
// It takes Nnumber of vertices and edges, and all the edges of the graph as input
// and gives the maximum matching edges as output
// O(E V^2)
#include <iostream>
#include <cstring>
using namespace std;
#define M 500 // max number of vertices
struct StructEdge
{
    int v;
    StructEdge *n;
};
typedef StructEdge *Edge;
class Blossom
{
    StructEdge pool[M * M * 2];
    Edge top = pool, adj[M];
    int V, E, qh, qt;
    int match[M], q[M], father[M], base[M];
    bool inq[M], inb[M], ed[M][M];
public:
Blossom(int V, int E) : V(V), E(E) {}
    void addEdge(int u, int v)
    {
        if (!ed[u - 1][v - 1])
        {
            top->v = v, top->n = adj[u], adj[u] = top++;
            top->v = u, top->n = adj[v], adj[v] = top++;
            ed[u - 1][v - 1] = ed[v - 1][u - 1] = true;
        }
    }
    // utility function for blossom contraction
    int LCA(int root, int u, int v)
    {
        static bool inp[M];
        memset(inp, 0, sizeof(inp));
        while (1)
        {
            inp[u = base[u]] = true;
            if (u == root)
                break;
            u = father[match[u]];
        }
        while (1)
        {
            if (inp[v = base[v]])
                return v;
            else
                v = father[match[v]];
        }
    }
    void mark_blossom(int lca, int u)
    {
        while (base[u] != lca)
        {
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = father[v];
            if (base[u] != lca)
                father[u] = v;
        }
    }
    void blossom_contraction(int s, int u, int v)
    {
        int lca = LCA(s, u, v);
        memset(inb, 0, sizeof(inb));
        mark_blossom(lca, u);
        mark_blossom(lca, v);
        if (base[u] != lca)
            father[u] = v;
        if (base[v] != lca)
            father[v] = u;
        for (int u = 0; u < V; u++)
            if (inb[base[u]])
            {
                base[u] = lca;
                if (!inq[u])
                    inq[q[++qt] = u] = true;
            }
    }
    int find_augmenting_path(int s)
    {
        memset(inq, 0, sizeof(inq));
        memset(father, -1, sizeof(father));
        for (int i = 0; i < V; i++)
            base[i] = i;
        inq[q[qh = qt = 0] = s] = true;
        while (qh <= qt)
        {
            int u = q[qh++];
            for (Edge e = adj[u]; e; e = e->n)
            {
                int v = e->v;
                if (base[u] != base[v] && match[u] != v)
                    if ((v == s) || (match[v] != -1 && father[match[v]] != -1))
                        blossom_contraction(s, u, v);
                    else if (father[v] == -1)
                    {
                        father[v] = u;
                        if (match[v] == -1)
                            return v;
                        else if (!inq[match[v]])
                            inq[q[++qt] = match[v]] = true;
                    }
            }
        }
        return -1;
    }
    int augment_path(int s, int t)
    {
        int u = t, v, w;
        while (u != -1)
        {
            v = father[u];
            w = match[v];
            match[v] = u;
            match[u] = v;
            u = w;
        }
        return t != -1;
    }
    int edmondsBlossomAlgorithm()
    { // Converted recursive algorithm to iterative version for simplicity
        int match_counts = 0;
        memset(match, -1, sizeof(match));
        for (int u = 0; u < V; u++)
            if (match[u] == -1)
                match_counts += augment_path(u, find_augmenting_path(u));
        return match_counts;
    }
    void printMatching()
    {
        for (int i = 0; i < V; i++)
            if (i < match[i])
                cout << i + 1 << " " << match[i] + 1 << "\n";
    }

};
int main()
{
    int u, v;
    int V, E;
    cin >> V >> E;
    Blossom bm(V, E);
    while (E--)
    {
        cin >> u >> v;
        bm.addEdge(u - 1, v - 1);
    }
    int res = bm.edmondsBlossomAlgorithm();
    if(!res)
        cout << "No Matching found\n";
    else
    {
        cout << "Total Matching = " << res << "\n";
        bm.printMatching();
    }
    return 0;   
}
