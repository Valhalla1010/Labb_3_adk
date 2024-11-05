#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int to, capacity, flow, rev;
};

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int V, s, t, E;
    cin >> V >> s >> t >> E;

    MaxFlow maxFlow(V + 1);

    for (int i = 0; i < E; ++i) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        maxFlow.addEdge(u, v, capacity);
    }

    int maxFlowValue = maxFlow.edmondsKarp(s, t);

    // Skriv ut antal hörn, källa och sänka samt maximalt flöde
    cout << V << "\n" << s << " " << t << " " << maxFlowValue << "\n";

    vector<vector<Edge>>& adj = maxFlow.getGraph();
    int edgeCount = 0;

    // Räkna kanter med positivt flöde
    for (int u = 1; u <= V; ++u) {
        for (const auto &e : adj[u]) {
            if (e.flow > 0 && e.capacity > 0) {
                edgeCount++;
            }
        }
    }

    cout << edgeCount << "\n";

    // Skriv ut varje kant med positivt flöde
    for (int u = 1; u <= V; ++u) {
        for (const auto &e : adj[u]) {
            if (e.flow > 0 && e.capacity > 0) {
                cout << u << " " << e.to << " " << e.flow << "\n";
            }
        }
    }

    return 0;
}
