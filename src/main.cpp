#include "graph.cpp"
#include "debug.cpp"
#include<iostream>

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    freopen("error.txt", "w", stderr);

    int nodes,edges;
    std::cin>>nodes>>edges;

    SafeFlowGraph graph(nodes);
    int a,b; double c;
    for(int i = 0 ; i < edges ; i++){
        std::cin>>a>>b>>c;
        graph.adj[a].push_back({b,c});
        graph.radj[b].push_back({a,c});
    }
    
    graph.init();
    auto res = graph.computeSafePaths();
    for(auto x: res){
        std::cout << x.flow << " " << x.u << " " << x.v << " " << x.l << " " << x.r << "\n";
    } 

return 0;
}
