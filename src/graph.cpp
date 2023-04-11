#include <vector>
#include<queue>
#include<algorithm>
#include "levelancestors.cpp"

class Graph{
public:
    int numberOfNodes;
    std::vector<std::vector<std::pair<int, double>>> adj, radj;
    std::vector<int>topoAdj, topoRadj;    
};

struct OptimalSafePath {
    double flow;
    int l, r;
    int u, v;
};

class SafeFlowGraph : public Graph{
private:
    void dfs(int v, std::vector<int>& ans ,
     std::vector<std::vector<std::pair<int, double>>>&graph, std::vector<bool>& visited) {
        visited[v] = true;
        for (auto u : graph[v]) {
            if(visited[u.first] == false){        
                dfs(u.first,ans,graph, visited);
            }
        }
        ans.push_back(v);
    }

public:
    std::vector<double> totalFlowIn, totalFlowOut;
    std::vector<std::vector<int>> Fi, Fo;
    std::vector<int>ci, co;
    int source , sink;
    std::vector<std::pair<double,int>> maximumIncomingFlow, maximumOutgoingFlow;
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
        maximumIncomingFlow.resize(n,{0,0});
        maximumOutgoingFlow.resize(n,{0,0});        
        source = -1 ;
        sink = -1 ;
    }


    void init(){

        // Calculating totalFlowIn and totalFlowOut
        for(int u = 0 ; u < numberOfNodes ; u++){
            for(auto v : adj[u]){
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
                        double edgeValue = v.second;
                        if(maximumIncomingFlow[v.first].first == edgeValue && maximumIncomingFlow[v.first].second == 1){
                            Fi[p].push_back(v.first);
                            double totalInputFlow = totalFlowIn[v.first];
                            double reduce_by = totalInputFlow - edgeValue;
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
                        double edgeValue = v.second;
                        if(maximumOutgoingFlow[v.first].first == edgeValue && maximumOutgoingFlow[v.first].second == 1){
                            Fo[p].push_back(v.first);
                            double totalInputFlow = totalFlowOut[v.first];
                            double reduce_by = totalInputFlow - edgeValue;
                            co[v.first] = co[p]-reduce_by;
                            q.push(v.first);                    
                        }
                    } 
                }

            }
        }        
        

        LAi = new LevelAncestors(Fi);
        LAo = new LevelAncestors(Fo);        
    }

    OptimalSafePath getPath(int u, int v, double edgeFlow){
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
                double safeFlow = edgeFlow + ci[u] + co[v] - ci[ans_u] - co[ans_v];
                OptimalSafePath optimalSafePath;
                optimalSafePath.flow = safeFlow;
                optimalSafePath.l = ans_u;
                optimalSafePath.r = ans_v;
                optimalSafePath.u = u;
                optimalSafePath.v = v;
                return optimalSafePath;
    }

    std::vector<OptimalSafePath> computeSafePaths() {
        std::vector<OptimalSafePath> result;
        for(int u = 0; u < numberOfNodes; u++){
            for(auto v: adj[u]){
                double edgeFlow = v.second;
                // finding left maximal
                int left , right ;
                OptimalSafePath path = getPath(u,v.first,v.second);                
                result.push_back(path);
                left = path.l;
                right = path.r;

                while(left!= v.first  ){
                    int x = LAo->get(right,0);
                    if(x == right){break;}
                    edgeFlow = v.second+co[v.first]-co[right];
                    int temp = LAo->level[right]-1;
                    right = LAo->get(right,temp);
                    path = getPath(left,right,edgeFlow);
                    left = path.l;
                    right = path.r;
                    result.push_back(path);
                }
            }
        }
        return result;
    }


};