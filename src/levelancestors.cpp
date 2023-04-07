#include<vector>
#include<cmath>
#include<iostream>

class LevelAncestors{
public:
    int n ;
    std::vector<int>level;
    std::vector<std::vector<int>>memo;
    std::vector<std::vector<int>>* g = NULL;
    std::vector<int>* topo;
    LevelAncestors(std::vector<std::vector<int>>& graph, std::vector<int>& t){
        int size = graph.size();
        g = &graph;
        topo = &t;
        n = size;
        level.resize(size);
        memo.resize(size, std::vector<int>(30,-1));
        precalculate();
    }

    void dfs(int u , int p , int lg, std::vector<bool>& vis){
        memo[u][0] = p;
        vis[u] = true;        
        for (int v : (*g)[u]){
            if (vis[v]==false)
            {
                level[v] = level[u] + 1;
                dfs(v, u,lg,vis);
            }
        }    
    }

    void precalculate(){
        int lg = (int)ceil(log2(n)); 
        std::vector<bool>vis(n,false);
        for(int i = 0 ; i < n ; i++){
            int node = (*topo)[i];
            if(vis[node] == false){
                dfs(node,-1,lg,vis);
            }
        }
        
        for(int i = 1 ; i<= lg ; i++){
            for(int j = 0 ; j < n ; j++){
                int temp = memo[j][i-1];
                if(temp == -1){
                    memo[j][i] = -1;
                }else{
                    memo[j][i] = memo[memo[j][i-1]][i-1];
                }
            }
        }



    }

    int get(int u, int d){ 
        d = level[u]-d;

        if(d < 0){ return -1 ;}
        if(d == 0){return u;}

        int lg = (int)ceil(log2(n));

        for (int i = lg; i >= 0; i--){
            int temp = d-(1LL<<i);
            if (temp >= 0){
                u = memo[u][i];                  
                d = d - (1<<i);
            }
        }
        return u;
    }
};