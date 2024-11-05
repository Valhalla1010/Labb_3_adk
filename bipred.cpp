//
// Created by davii on 10/31/2024.
//
/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder iostreams i C++.
 * Anmärkning: scanf/printf-rutinerna från C-exemplet går att använda
 * även i C++ och är ofta märkbart snabbare än cin/cout som det här
 * programmet använder.
 *
 * Author: Per Austrin
 */

#include <iostream>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
std::vector<std::list<int>> graph;
std::vector<std::pair<int, int>> matchFLow;
int x, y, e;


void readBipartiteGraph() {
    // Läs antal hörn och kanter
    cin >> x >> y >> e;

    graph.resize(x + y + 2);


    // Läs in kanterna
    for (int i = 0; i < e; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
    }
    for (int i = 1; i <= x; ++i) {
        graph[0].push_back(i);
    }

    for (int i = x + 1; i <= x + y; ++i) {
        graph[i].push_back(x + y + 1);
    }
}


void writeFlowGraph() {
    int v = x + y + 2;
    int s = 1;
    int t = v;

    // Skriv ut antal hörn och kanter samt källa och sänka
    cout << v << "\n" << s << " " << t << "\n" << x + y + e << "\n";
    for (int i = 0; i < graph.size(); ++i) {
        int u = i + 1;
        int c = 1;
        for (int b: graph[i]) {
            b += 1;
            // Kant från u till v med kapacitet c
            cout << u << " " << b << " " << c << "\n";
        }
    }
    // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
    cout.flush();

    // Debugutskrift
    cerr << "Skickade iväg flödesgrafen\n";
}


void readMaxFlowSolution() {
    int v, s, t, f;
    // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
    // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
    // skickade iväg)
    cin >> v >> s >> t >> f >> e;
    matchFLow.clear();
    for (int i = 0; i < e; ++i) {
        int u, v, flow;
        // Flöde f från u till v
        cin >> u >> v >> flow;
        if (flow > 0 && u >= 2 && u <= x + 1 && v >= x + 2 && v <= x + y + 1) {
            matchFLow.emplace_back(u - 1, v - 1);
        }
    }
}


void writeBipMatchSolution() {
    //int x = 17, y = 4711, maxMatch = 0;

    // Skriv ut antal hörn och storleken på matchningen
    cout << x << " " << y << "\n" << matchFLow.size() << "\n";

    for (const auto &edge: matchFLow) {
        // Kant mellan a och b ingår i vår matchningslösning

        cout << edge.first << " " << edge.second<< "\n";
    }

}


int main(void) {
    // Två trick för att göra cin/cout lite snabbare.
    // Se http://kattis.csc.kth.se/doc/iostreamio
    std::ios::sync_with_stdio(false);
    cin.tie(0);

    readBipartiteGraph();

    writeFlowGraph();

    readMaxFlowSolution();

    writeBipMatchSolution();

    // debugutskrift
    cerr << "Bipred avslutar\n";
    return 0;
}