#include "graph.cpp"
#include "debug.cpp"
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
using namespace std;


vector<string> split(string &s, string &delimiters){
    vector<string> res;
    int pos, prev = 0;
    while((pos = s.find_first_of(delimiters,prev)) != string::npos){
        if(pos > prev) {
            res.emplace_back(s,prev,pos-prev);
        }
        prev = pos+1;
    }
    if(prev < s.size()) res.emplace_back(s,prev,(int)s.size() - prev);
    return res;
}

int main(int argc , char* argv[]){
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    // freopen("error.txt", "w", stderr);
    if(argc != 4){
        cout<<"Expected parameters < input > < output > < flag > "<<endl;
        exit(1);
    }
    std::ifstream fin;
    fin.open(argv[1]);
    string line;
    std::ofstream fout;
    fout.open(argv[2]);
    
    int flag = stoi(argv[3]);

    if(fin.is_open() == false){
        cout<<"Error while opening the input file"<<endl;
        exit(1);
    }
    if(fout.is_open() == false){
        cout<<"Error while opening the output file"<<endl;
        exit(1);
    }
    // debug(argv[0]);
    // debug(argv[1]);
    // debug(argv[2]);
    // debug(argv[3]);

    
    int nodes,edges;
    int counter = 0 ;
    SafeFlowGraph graph(1);
    string space = " ";
    string graphName = "";
    string prev;
    while(getline(fin,line)){
        if(line[0] == '#'){
        //   fout<<line<<endl;
          prev = graphName;
          graphName = line ;
          getline(fin,line);
          if(counter > 0){ 
            graph.init();
            auto res = graph.computeSafePaths(flag);
            int epsilon = 1e-7;
            fout << prev << endl;
            
            for(auto x: res){
                if(x.flow <= 0 ){continue;}
                if(flag == true){
                    fout <<x.flow <<" ";
                    for(int i = 0 ; i < x.completePath.size() ; i++){
                        if((i+1) == (x.completePath.size())){fout<<x.completePath[i]<<"\n";}
                        else{fout<<x.completePath[i]<<" ";}
                    }
                }else{
                    fout << x.flow <<" "<< x.u <<" "<<x.v<<" "<<x.l<<" "<<x.r<<endl;
                }
            } 
          }
          nodes = stoi(line);
          graph = SafeFlowGraph(nodes);          
          counter++;                
        }else{
          auto v = split(line,space);
          int a = stoi(v[0]);
          int b = stoi(v[1]);
          int c = stod(v[2]);
          graph.adj[a].push_back({b,c});
          graph.radj[b].push_back({a,c});
        }
    }
    graph.init();
    auto res = graph.computeSafePaths(flag);
    fout << graphName << endl;
    int epsilon = 1e-7;
    for(auto x: res){
        if(x.flow <= 0 ){continue;}
        if(flag == true){
            fout <<x.flow <<" ";
            for(int i = 0 ; i < x.completePath.size() ; i++){
                if((i+1) == (x.completePath.size())){fout<<x.completePath[i]<<"\n";}
                else{fout<<x.completePath[i]<<" ";}
            }
        }else{
            fout << x.flow <<" "<< x.u <<" "<<x.v<<" "<<x.l<<" "<<x.r<<endl;
        }
    } 


    fin.close();
    fout.close();   

return 0;
}
