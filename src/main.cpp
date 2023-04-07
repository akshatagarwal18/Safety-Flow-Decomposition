#include "graph.cpp"
#include "debug.cpp"
#include<iostream>

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    freopen("error.txt", "w", stderr);

    int nodes,edges;
    std::cin>>nodes>>edges;

    std::cerr<<nodes<<" "<<edges<<std::endl;

    SafeFlowGraph graph(nodes);
    int a,b; double c;
    for(int i = 0 ; i < edges ; i++){
        std::cin>>a>>b>>c;
        graph.adj[a].push_back({b,c});
        graph.radj[b].push_back({a,c});
    }
    
    graph.init();
 
    debug(graph.co);
    debug(graph.ci);
    debug(graph.LAi->get(4,1));
    debug(graph.LAi->get(4,2));
    debug(graph.LAi->memo);
     debug(graph.LAo->memo);

return 0;
}
