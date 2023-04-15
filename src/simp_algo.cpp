/* 
 * Use: 
 * <exec>     => Generates safe and complete
 * <exec>  u  => Generates unitigs
 * <exex>  e  => Generates extended unitigs
 */


#include<cstdio>
#include<climits>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include<cfloat>

using namespace std;

#define pld pair<long,double>
#define pdl pair<double,long>
#define pll pair<long,long>
#define pdd pair<double,double>
#define p3ld pair<pll,pld>
#define pddll pair<pdd,pll>
#define ipld list<pld>::iterator

class Graph{

	public:
	       	vector<list<pld>> adj,rAdj;
		list<long> topOrder;
		vector<double> fIn;
		vector<double> fOut;

	Graph(int n=1){
		adj.resize(n);
		rAdj.resize(n);
		fIn.resize(n,0);
		fOut.resize(n,0);
	}
};

int existEdge(Graph &G,long x, long y){
	list<pld> :: iterator it;
	for(it= G.adj[x].begin(); it!= G.adj[x].end(); it++)
		if((*it).first ==y) break;
	
	if(it== G.adj[x].end()) return 0;
	else			return 1;
}


void revReachTopOrder(Graph &G,long root,vector<int> &vis){
	list<pld> :: iterator it;

	for(it=G.rAdj[root].begin();it!=G.rAdj[root].end();it++){
		if(!vis[(*it).first]){
			vis[(*it).first]=1;
			revReachTopOrder(G,(*it).first,vis);
		}
	}	
	G.topOrder.push_back(root);
}

/*
void comp_topOrder(Graph &G){
	vector<long> indeg;
	queue<long> q;

	G.topOrder.clear();
	indeg.resize(G.adj.size());

	for(int i=0;i<G.adj.size();i++){
		indeg[i]=G.rAdj[i].size();
		if(!indeg[i]){
		       	G.topOrder.push_back(i);
			q.push(i);
		}
	}

	//printf("Q %lu\n",q.size());
	while(!q.empty()){
		long t = q.front(); 
		q.pop();
		list<pld> :: iterator it;
		for(it=G.adj[t].begin();it!=G.adj[t].end();it++){
			indeg[(*it).first]--;
			if(!indeg[(*it).first]){
				G.topOrder.push_back((*it).first);
				q.push((*it).first);
			}
		}
	}
	//printf("Top Ord %lu \n",G.topOrder.size());
}*/

void comp_topOrder(Graph &G){
	vector<int> vis;
	G.topOrder.clear();
	vis.resize(G.adj.size(),0);
	

	for(int i=0;i<G.adj.size();i++){
		if(G.adj[i].size()==0)
			revReachTopOrder(G,i,vis);
	}
}


void print_topOrder(Graph &G){
	list<long> :: iterator it;
	for(it=G.topOrder.begin();it!=G.topOrder.end();it++)
		printf("%ld ",*it);
	printf("\n");
}

void compute_safePaths(Graph &G, list<pair<list<ipld>,pld> > &paths, list<pair<list<ipld>,pld> > &safePaths){
	list<pld> :: iterator edgit;	
	list<ipld>:: iterator itp,itp2;
	list<pair<list<ipld>,pld> >:: iterator itps;
	long x,y;
	double f;
	int prnt = 0;
	list<ipld> path;

	if(prnt) printf("Safe and Complete paths\n");

	for(itps=paths.begin(); itps!=paths.end();itps++){

		if(prnt){ 
			printf("N %ld %ld", (long) (*itps).second.second, 
					(*itps).second.first);
			for(itp=(*itps).first.begin();itp!=(*itps).first.end();itp++)
				printf(" %ld", (**itp).first);
			printf("\n");
		}

		itp = itp2 = (*itps).first.begin();
		x = (*itps).second.first;
		y = (**itp).first; 
		f = (**itp).second;
	        path.clear();
		path.push_back(*itp);	
		itp2++;

		while(true ){  
			
			while(itp2!=(*itps).first.end() && f+(**itp2).second-G.fOut[y] > 0){ // Right extendable
				f-= G.fOut[y]-(**itp2).second;
				y = (**itp2).first;
				path.push_back(*itp2);
				if(prnt) printf("Move Right l %ld r %ld df %lf\n", x,y, f+(**itp2).second-G.fOut[y]);
				itp2++;	
			} 
			
			if(itp!=itp2){  // Safe path > 1 edge
                               	safePaths.push_back(pair<list<ipld>,pld>(path,pld(x,f)));
				if(prnt) printf("S %ld %ld f%ld\n",x,y,(long) f);
			}

				
			if(itp2!=(*itps).first.end()){
				f-= G.fOut[y]-(**itp2).second;
				y = (**itp2).first;
				path.push_back(*itp2);
				if(prnt) printf("Move Right l %ld r %ld df %lf\n", x,y, f);
				itp2++;
				
				while(f-(**itp).second+G.fIn[(**itp).first] <= 0){ // UnSafe flow
					f+= G.fIn[(**itp).first]-(**itp).second; 
					if(prnt) printf("Move Left l %ld r %ld df %lf\n", (**itp).first,y, f);
					path.pop_front();		
					itp++;
				}
		
				x = (**itp).first;
				f+= G.fIn[(**itp).first]-(**itp).second; 
				path.pop_front();		
				if(prnt) printf("Move Left l %ld r %ld df %lf\n", x,y, f);
				itp++;
			} else break;

		}
	}
	
}

void extend_unitigs(Graph &G, list<pair<list<ipld>,pld> > &eUnitigs){
	
	list<pld> :: iterator edgit;	
	list<ipld>:: iterator itp;
	list<pair<list<ipld>,pld> >:: iterator itu;
	int prnt = 0;

	if(prnt) printf("Extended unitigs\n");

	for(itu=eUnitigs.begin(); itu!=eUnitigs.end();itu++){

		long x,y;
		itp = (*itu).first.end();
		itp--;       

		x = (**itp).first;
		while(G.adj[x].size()==1 ){  // Right extension
			(*itu).first.push_back(G.adj[x].begin());
			x = G.adj[x].front().first;
		}

		x = (*itu).second.first;
		while(G.rAdj[x].size()==1){ // Left extension
			y = G.rAdj[x].front().first;	
			edgit=G.adj[y].begin();
			while((*edgit).first!= x) edgit++;

			(*itu).first.push_front(edgit);
			(*itu).second.first = y;
			x = y;
		}
	}




}

void compute_unitigs(Graph &G, list<pair<list<ipld>,pld> > &unitigs){
	vector<long> secV = vector<long>(G.adj.size(),0);
	list<long>:: iterator it;
	list<pld> :: iterator edgit;	
	int prnt = 0;
	if(prnt) printf("Unitigs\n");

	for(it=G.topOrder.begin(); it!=G.topOrder.end();it++){
		long x = *it;
		if(prnt) printf("Node %ld\n",x);
		for(edgit=G.adj[x].begin(); edgit!= G.adj[x].end(); edgit++){
			long y= (*edgit).first;
			if(prnt) printf("Edge %ld %ld\n",x,y);
			if(G.adj[y].size()!=1 || G.rAdj[y].size()!=1 || secV[y]!=0){
				if(secV[x]==0){
					pld pth = pld(x,(*edgit).second);
					list<ipld> path;
					path.push_back(edgit);
					unitigs.push_back(pair<list<ipld>,pld>(path,pth));		
				}

			} else {
				// Begining of a unitig
				if(prnt) printf("Begin Unitig\n");
				secV[y]=1;
			
				pld pth = pld(x,(*edgit).second);
				list<ipld> path;
			
				ipld curr = edgit;
				while(true){
					path.push_back(curr);

					if(G.adj[(*curr).first].size()!=1 || G.rAdj[(*curr).first].size()!= 1) 
						break;
					secV[(*curr).first]=1;
					curr = G.adj[(*curr).first].begin(); 
				}
			
				unitigs.push_back(pair<list<ipld>,pld>(path,pth));		
			}			
		}
	}


		

}

	
void print_paths(Graph &G, list<pair<list<ipld>,pld> > &paths){
//	printf("%lu paths:\n", paths.size());
	
	list<pair<list<ipld>,pld> > :: iterator it;
	list<ipld> :: iterator it2;

	for(it=paths.begin();it!=paths.end();it++){
		if((*it).first.size()!=1){
		printf("%ld %ld", (long) (*it).second.second, (*it).second.first);
		for(it2=(*it).first.begin();it2!=(*it).first.end();it2++)
			printf(" %ld", (**it2).first);
		printf("\n");
		}
	}
}





void updateG(Graph &G, list<ipld> &path, double val){
	list<ipld> :: iterator it;
	for(it=path.begin();it!=path.end();it++)
		(**it).second+= val;
}

void path_decomp(Graph &G, list<pair<list<ipld>,pld> > &paths){
	list<long> sources;
	list<pld> :: iterator edgit,edgit2;	
	double min=DBL_MAX;
	int prnt = 0;
	for(long i=0;i<G.adj.size();i++){
		if(G.rAdj[i].size()==0){ // Source node
			for(edgit=G.adj[i].begin(); edgit!= G.adj[i].end(); edgit++){
				while((*edgit).second!=0){// Source edge
					pld pth = pld(i,DBL_MAX);
					list<ipld> path;
					
					ipld curr = edgit;
					while(true){
						if(pth.second > (*curr).second) 
							pth.second = (*curr).second;
						path.push_back(curr);

						if(G.adj[(*curr).first].size()==0) break;
						
						edgit2=G.adj[(*curr).first].begin(); 
						while((*edgit2).second== 0) edgit2++;
						curr = edgit2;
					}
					paths.push_back(pair<list<ipld>,pld>(path,pth));
					if(prnt) printf("Path from %ld of %lf weight and length %lu added\n", 
							pth.first,pth.second, path.size());
					updateG(G,path,-1*pth.second);					
				}
			}
		}
	}

	list<pair<list<ipld>,pld> > :: iterator it;
	for(it=paths.begin();it!=paths.end();it++)
		updateG(G,(*it).first,(*it).second.second);
}


int main(int argc, char *argv[]){
	char line[30000];
	long n,m,graphC=0;
	int prnt =0;
	//int t=10;

	int flag=0; // safe paths
	if(argc==2){
		if(argv[1][0]=='u') flag=1;// unitigs	
		if(argv[1][0]=='e') flag=2;// extended unitigs	
	}

	while(scanf("%[^\n]",line) > 0){
		if(flag<=2) printf("%s\n",line);
		graphC++;
		scanf("%ld",&n);
		m=0;

		Graph G= Graph(n);
		list<pair<list<ipld>,pld > > paths;
		list<pair<list<ipld>,pld > > results;
		long a,b;
		double c;

		while(scanf("%ld %ld %lf\n",&a,&b,&c)> 0){	
			G.adj[a].push_back(pld(b,c));
			G.rAdj[b].push_back(pld(a,c));	
			G.fIn[b]+= c;
			G.fOut[a]+= c;
			m++;
		}

		if(prnt) printf("N%ld M%ld\n",n,m);
		
		if(flag){
			comp_topOrder(G);
			if(prnt) print_topOrder(G);
		
			compute_unitigs(G,results);
			if(flag==2)
				extend_unitigs(G,results);
		}else{
			path_decomp(G,paths);
			if(prnt) print_paths(G,paths);

			compute_safePaths(G,paths,results);
		}	 
	        print_paths(G,results);	
	
	//	printf("Orginal paths %ld, Final paths %ld, diff %ld\n", org,fin, org-fin );
	//	printf("Weight of Orginal paths %ld, Weight of Final paths %ld, diff %ld\n\n", wOrg,wFin, wOrg-wFin );
	}

	//if(flag){
	//	printf("\n\nNumber of Graphs processed: %ld\n", graphC);
	//	printf("Total Original Paths %ld, Total final Paths %ld, max diff %ld\n", tOrg, tFin, maxd);
	//	printf("Total Weight of Original Paths %ld, Total weight of final paths %ld, max diff %ld\n", wtOrg, wtFin, wMaxd);
	//}

	return 0;

	}