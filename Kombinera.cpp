#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge {
    int to, capacity, flow, rev;
};

// Flödesalgoritm
class MaxFlow {
public:
    MaxFlow(int vertices) : adj(vertices), level(vertices), start(vertices) {}

    void addEdge(int u, int v, int capacity) {
        Edge a = {v, capacity, 0, (int) adj[v].size()};
        Edge b = {u, 0, 0, (int) adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    int edmondsKarp(int s, int t) {
        int maxFlow = 0;

        while (bfs(s, t)) {
            fill(start.begin(), start.end(), 0);
            while (int flow = sendFlow(s, t, INF))
                maxFlow += flow;
        }

        return maxFlow;
    }

    vector<vector<Edge>>& getGraph() {
        return adj;
    }

private:
    vector<vector<Edge>> adj;
    vector<int> level, start;
    const int INF = numeric_limits<int>::max();

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;

        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& e : adj[u]) {
                if (level[e.to] < 0 && e.flow < e.capacity) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] >= 0;
    }

    int sendFlow(int u, int t, int flow) {
        if (u == t) return flow;

        for (; start[u] < adj[u].size(); ++start[u]) {
            Edge &e = adj[u][start[u]];

            if (level[e.to] == level[u] + 1 && e.flow < e.capacity) {
                int currFlow = min(flow, e.capacity - e.flow);
                int tempFlow = sendFlow(e.to, t, currFlow);

                if (tempFlow > 0) {
                    e.flow += tempFlow;
                    adj[e.to][e.rev].flow -= tempFlow;
                    return tempFlow;
                }
            }
        }

        return 0;
    }
};

// Funktion för att lösa matchningsproblemet
void solveBipartiteMatching() {
    int x, y, e;
    cin >> x >> y >> e;

    int totalVertices = x + y + 2;
    int source = 0;
    int sink = totalVertices - 1;

    MaxFlow maxFlow(totalVertices);

    // Läs in kanterna
    for (int i = 0; i < e; ++i) {
        int a, b;
        cin >> a >> b;
        maxFlow.addEdge(a, b, 1);  // Kapacitet 1 för matchning
    }

    // Lägg till kanter från källa till mängden X
    for (int i = 1; i <= x; ++i) {
        maxFlow.addEdge(source, i, 1);
    }

    // Lägg till kanter från mängden Y till sänka
    for (int i = x + 1; i <= x + y; ++i) {
        maxFlow.addEdge(i, sink, 1);
    }

    // Kör Edmonds-Karp för max-flöde
    maxFlow.edmondsKarp(source, sink);

    vector<pair<int, int>> matching;
    vector<vector<Edge>>& adj = maxFlow.getGraph();

    for (int u = 1; u <= x; ++u) {
        for (const auto& e : adj[u]) {
            if (e.flow > 0 && e.capacity > 0) {
                matching.emplace_back(u, e.to);
            }
        }
    }
    cout << x << " " << y << "\n";
    cout << matching.size() << "\n";
    for (const auto& edge : matching) {
        cout << edge.first << " " << edge.second << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    solveBipartiteMatching();

    return 0;
}
