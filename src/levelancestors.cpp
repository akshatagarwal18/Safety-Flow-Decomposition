#include<vector>
#include<cmath>
#include<iostream>

class LevelAncestors{
public:
    int n ;
    std::vector<int>level;
    std::vector<std::vector<int>>memo;
    std::vector<std::vector<int>>* g = NULL;
    std::vector<int>indegree;
    std::vector<int>ordering;
    LevelAncestors(std::vector<std::vector<int>>& graph){
        int size = graph.size();
        g = &graph;
        n = size;
        level.resize(size);
        memo.resize(size, std::vector<int>(30,-1));
        indegree.resize(n,0);
        for(int i = 0 ; i < n ; i++){
            for(auto v : (*g)[i]){
                indegree[v]++;
            }
        }
        forOrdering();       
    }

    void dfs(int u , int p , std::vector<bool>& vis){
        memo[u][0] = p;
        vis[u] = true;
        ordering.push_back(u);        
        for (int v : (*g)[u]){
            if (vis[v]==false)
            {
                level[v] = level[u] + 1;
                dfs(v, u,vis);
            }
        }    
    }


    void forOrdering(){
        int lg = (int)ceil(log2(n)); 
        for(int i = 0 ; i < n ; i++){
            if(indegree[i] == 0){
                ordering.clear();
                std::vector<bool>vis(n,false);
                dfs(i,-1,vis);
                reverse(ordering.begin(), ordering.end());
                for(int i = 1 ; i<= lg ; i++){
                    for(int j = 0 ; j < ordering.size() ; j++){
                        int node = ordering[j];
                        int temp = memo[node][i-1];
                            if(temp == -1){
                                memo[node][i] = -1;
                            }else{
                                memo[node][i] = memo[memo[node][i-1]][i-1];
                            }
            }
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