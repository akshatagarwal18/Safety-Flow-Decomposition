#include <vector>
#include<queue>
#include<algorithm>
#include "levelancestors.cpp"
using namespace std;

class Graph{
public:
    int numberOfNodes;
    std::vector<std::vector<std::pair<int, int>>> adj, radj;
    std::vector<int>topoAdj, topoRadj;    
};

struct OptimalSafePath {
    int flow;
    int l, r;
    int u, v;
    std::vector<int>completePath;
};

class SafeFlowGraph : public Graph{
private:
    void dfs(int v, std::vector<int>& ans ,
     std::vector<std::vector<std::pair<int, int>>>&graph, std::vector<bool>& visited) {
        visited[v] = true;
        for (auto u : graph[v]) {
            if(visited[u.first] == false){        
                dfs(u.first,ans,graph, visited);
            }
        }
        ans.push_back(v);
    }

public:
    std::vector<int> totalFlowIn, totalFlowOut;
    std::vector<std::vector<int>> Fi, Fo;
    std::vector<int>ci, co;
    int source , sink;
    vector<vector<int>>gr;
    std::vector<std::pair<int,int>> maximumIncomingFlow, maximumOutgoingFlow;
    LevelAncestors* LAi = NULL ;
    LevelAncestors* LAo = NULL;

    SafeFlowGraph(int n)
    {
        numberOfNodes = n;
        adj.resize(n);
        radj.resize(n);
        totalFlowIn.resize(n);
        totalFlowOut.resize(n);
        ci.resize(n);
        co.resize(n);
        Fi.resize(n);
        Fo.resize(n);
        gr.resize(n,vector<int>(n));
        maximumIncomingFlow.resize(n,{0,0});
        maximumOutgoingFlow.resize(n,{0,0});        
        source = -1 ;
        sink = -1 ;
    }


    void init(){

        // Calculating totalFlowIn and totalFlowOut
        for(int u = 0 ; u < numberOfNodes ; u++){
            for(auto v : adj[u]){
                gr[u][v.first]=v.second;
                totalFlowIn[v.first]+=v.second;
                totalFlowOut[u]+=v.second;              
            }
        }

        // Finding source and sink
        for(int i = 0 ; i < numberOfNodes; i++){
            if(totalFlowIn[i] == 0){
                source=i;
            }
            if(totalFlowOut[i] == 0){
                sink=i;
            }
        }

        // Calculating maximumIncomingFlow and maximumOutgoingFlow
        for(int u = 0 ; u < numberOfNodes ; u++){
            for(auto v : adj[u]){
                if(v.second > maximumIncomingFlow[v.first].first){
                    maximumIncomingFlow[v.first].first = v.second;
                    maximumIncomingFlow[v.first].second = 1;
                } else if(v.second == maximumIncomingFlow[v.first].first){
                    maximumIncomingFlow[v.first].second+=1;                    
                }
            }
        }

        for(int u = 0 ; u < numberOfNodes ; u++){
            for(auto v : radj[u]){
                if(v.second > maximumOutgoingFlow[v.first].first){
                    maximumOutgoingFlow[v.first].first = v.second;
                    maximumOutgoingFlow[v.first].second = 1;
                } else if(v.second == maximumOutgoingFlow[v.first].first){
                    maximumOutgoingFlow[v.first].second+=1;                    
                }                
            }
        }

        // Topological sort of adj and radj
        std::vector<bool>visited(numberOfNodes,false);
        dfs(source,topoAdj,adj,visited);
        visited = std::vector<bool>(numberOfNodes,false);
        dfs(sink,topoRadj,radj,visited);
        std::reverse(topoAdj.begin(),topoAdj.end());
        std::reverse(topoRadj.begin(),topoRadj.end());

        // Generating Fi and Fo
        // and Ci and Co
        
        std::vector<bool>vis(numberOfNodes,false);
        for(int i = 0 ; i < numberOfNodes ; i++){
            int node = topoAdj[i];
            if(vis[node] == false){
                std::queue<int>q;
                q.push(node); 
                ci[node]=0;
                while(!q.empty()){
                    int p = q.front();
                    q.pop();
                    vis[p]=true;
                    for(auto v : adj[p]){
                        int edgeValue = v.second;
                        if(maximumIncomingFlow[v.first].first == edgeValue && maximumIncomingFlow[v.first].second == 1){
                            Fi[p].push_back(v.first);
                            int totalInputFlow = totalFlowIn[v.first];
                            int reduce_by = totalInputFlow - edgeValue;
                            ci[v.first] = ci[p]-reduce_by;
                            q.push(v.first);                    
                        }
                    } 
                }

            }
        }
        vis = std::vector<bool>(numberOfNodes,false);
        for(int i = 0 ; i < numberOfNodes ; i++){
            int node = topoRadj[i];
            if(vis[node] == false){
                std::queue<int>q;
                q.push(node);
                co[node]=0;
                while(!q.empty()){
                    int p = q.front();
                    q.pop();
                    vis[p]=true;
                    for(auto v : radj[p]){
                        int edgeValue = v.second;
                        if(maximumOutgoingFlow[v.first].first == edgeValue && maximumOutgoingFlow[v.first].second == 1){
                            Fo[p].push_back(v.first);
                            int totalInputFlow = totalFlowOut[v.first];
                            int reduce_by = totalInputFlow - edgeValue;
                            co[v.first] = co[p]-reduce_by;
                            q.push(v.first);                    
                        }
                    } 
                }

            }
        }        
        
    //     for(int i = 0 ; i < numberOfNodes ; i++){
    //     cout<<i<<": "<<ci[i]<<endl;
    //  } cout<<endl;

        LAi = new LevelAncestors(Fi);
        LAo = new LevelAncestors(Fo);
        int temp = 0;
        // for(int i = 1 ; i<= numberOfNodes ; i++){
        //     temp = LAi->get(14,i);
        //     cerr<<temp<<" ";
        // }         cerr<<endl;
    }
     
     
    OptimalSafePath getPath(int u, int v, int edgeFlow){
        int leftMaxLo = 0, leftMaxHi = LAi->level[u];
                int ans_u  ;
                while(leftMaxLo <= leftMaxHi){
                    int leftMaxMid = (leftMaxLo + leftMaxHi)/2;
                    int midNode = LAi->get(u,leftMaxMid);
                    if(ci[midNode] < ci[u] + edgeFlow) {
                        ans_u = midNode;
                        leftMaxHi = leftMaxMid-1;

                    } else {
                        leftMaxLo = leftMaxMid + 1;
                    }
                }
                // finding right maximal
                int rightMaxLo = 0, rightMaxHi = LAo->level[v];
                int ans_v;
                while(rightMaxLo <= rightMaxHi){
                    int rightMaxMid = (rightMaxLo + rightMaxHi)/2;
                    int midNode = LAo->get(v,rightMaxMid);
                    if(co[midNode] < edgeFlow + ci[u] + co[v] - ci[ans_u]){
                        ans_v = midNode;
                        rightMaxHi = rightMaxMid-1;
                    } else {
                        rightMaxLo = rightMaxMid + 1;
                    }
                }
                int safeFlow = edgeFlow + ci[u] + co[v] - ci[ans_u] - co[ans_v];
                OptimalSafePath optimalSafePath;
                optimalSafePath.flow = safeFlow;
                optimalSafePath.l = ans_u;
                optimalSafePath.r = ans_v;
                optimalSafePath.u = u;
                optimalSafePath.v = v;
                return optimalSafePath;
    }



    bool isRightExtendible(int flow, int v){
        int temp1 = totalFlowOut[v];
        int temp2 = maximumOutgoingFlow[v].first;
        int temp3 = temp1 - temp2 ;
        int rem = flow - temp3;
        if(rem > 0){return true;}
        return false;
    }
    
    bool isLeftExtendible(int flow, int u){
        return false;
    }

    std::vector<OptimalSafePath> computeSafePaths(int flag) {
        std::vector<OptimalSafePath> result;
        
        for(int u = 0; u < numberOfNodes; u++){
            for(auto v: adj[u]){
                int edgeFlow = v.second;
                // finding left maximal
                int left , right ;

                // Checking if the this edge is unique maximum incoming edge or not
                int temp1 = u ;
                int temp2 = v.first;
                if(maximumIncomingFlow[temp2].second == 1 && maximumIncomingFlow[temp2].first == edgeFlow){
                    
                    if(Fi[temp2].size() == 0){
                        //cout<<v.first<<endl;
                        int l = 0,r = LAi->level[u],mid, x;
                        while(l<=r){
                            mid = (l+r)/2;
                            int xx = LAi->get(u,mid);
                            if(ci[xx] < ci[u] + edgeFlow){
                                x = xx;
                                r = mid-1;
                            }else{
                                l = mid+1;
                            }
                        }

                        OptimalSafePath path;
                        path.u = u;
                        path.v = v.first;
                        path.l = x ;
                        path.r = v.first;
                        path.flow = edgeFlow - ci[x] + ci[u];
                        if( !(path.l == u && path.r  == v.first)){
                        if(flag){complete(path);}
                        result.push_back(path);
                        }
                        // // int lev = ;
                        int cur = u ;
                        int last = x ;
                        
                        // cout<<cur<<" "<<last<<endl; 
                         while(LAi->level[last]){
                            int u1 = LAi->get(cur,(LAi->level[cur]-1));
                            l = 0; r = LAi->level[u1]; 
                            int y;
                            int flow = gr[u1][cur];
                            while(l<=r){
                                mid = (l+r)/2;
                                int yy = LAi->get(u1,mid);
                                if(ci[yy] < ci[u1] + flow){
                                    y = yy;
                                    r = mid-1;
                                }else{
                                    l = mid+1;
                                }
                            }
                            path.u = u1;
                            path.v = cur;
                            path.l = y;
                            path.r = cur;
                            path.flow = flow - ci[y] + ci[u1];
                            if( !(path.l == u1 && path.r  == cur)){
                                if(flag){complete(path);}
                                result.push_back(path);
                            }
                            cur = u1;
                            last = y;
                                                    // cout<<cur<<" "<<last<<endl;
                         }                          
                    }
                    continue;
                }















                OptimalSafePath path = getPath(u,v.first,v.second);                
                
                left = path.l;
                right = path.r;
                if(flag){complete(path);}
                if(!(left == u && right == v.first)){
                result.push_back(path);
                }
                while(left!= v.first  ){
                    int x = LAo->get(right,0);
                    if(x == right){break;}

                    
                    int temp = LAo->level[right]-1;
                    right = LAo->get(right,temp);
                    edgeFlow = v.second+co[v.first]-co[right];
                    if(edgeFlow <= 0){break;}
                    path = getPath(u,right,edgeFlow);
                    left = path.l;
                    right = path.r;
                    path.u = u ;
                    path.v = v.first;
                    
                    if(left == u && right  == v.first){continue;}
                    if(flag){complete(path);}
                    result.push_back(path);
                }
            }
        }
        return result;
    }

    void complete(OptimalSafePath& path){
        int left = path.l;
        int right = path.r;
        int temp = path.u;
        std::vector<int>v1;
        v1.push_back(temp);        
        while(temp!= left){
            int temp1 = LAi->get(temp,LAi->level[temp]-1);
            v1.push_back(temp1);            
            temp = temp1; 
        } reverse(v1.begin(),v1.end());
        v1.push_back(path.v);
        temp = path.v ;        
        while(temp!=right){
            int temp1 = LAo->get(temp,LAo->level[temp]-1);
            v1.push_back(temp1);            
            temp = temp1;            
        }  
        path.completePath = v1;        
    }
    


};