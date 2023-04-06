#include<vector>
#include<cmath>
#include<iostream>

class LevelAncestors{
public:
    int n ;
    std::vector<int>level;
    std::vector<std::vector<int>>memo;
    std::vector<std::vector<int>>* g = NULL;
    LevelAncestors(std::vector<std::vector<int>>& graph){
        int size = graph.size();
        g = &graph;
        n = size;
        level.resize(size);
        memo.resize(size, std::vector<int>(30));
        precalculate();
    }

    void dfs(int u , int p , int lg, std::vector<bool>& vis){
        memo[u][0] = p;
        vis[u] = true;
        for (int i = 1; i <= lg; i++)
        memo[u][i] = memo[memo[u][i - 1]][i - 1];
        for (int v : (*g)[u]){
            if (v != p)
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
            if(vis[i] == false){
                dfs(i,i,lg,vis);
            }
        }
    }

    int get(int u, int d){  
        // std::cout<<level[u]<<" "<<d<<std::endl;  
        d = level[u]-d;
        // std::cout<<d<<std::endl;
        //std::cout<<"lg"<<std::endl;
        if(d < 0){ return -1 ;}

        int lg = (int)ceil(log2(n)); 
        //std::cout<<lg<<std::endl;
        for (int i = lg; i >= 0; i--){
            if ((level[u] - (1<<i)) >= 0){
                
                u = memo[u][i];
                 std::cout<<u<<std::endl;
                d = d - (1<<i);
            }
        }
        return u;
    }
};