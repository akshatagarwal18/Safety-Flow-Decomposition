/**
 *     AC Trie based compression to remove duplicates, prefixes and suffixes.
 *
 *     Usage Graph input format, where (a-z) are long integers 
 *        # graph p  (less than 300 chars)
 *        a b c d e f
 *        (x, y)(v, w)
 *        # graph q
 *        ....
 *
 * 	Use arg 'f' for the following graph format, 
 * 	where abcefg are long int and d1.d2 are double
 *	# graph p   (less than 300 chars)
 *	d1.00 a b c 
 *      d2.00 e f g 
 *
 * 	./a.out  arg < input
 * 	arg: a => All details, stats for each graph and commulative
 * 	     s => Only commulative stats
 *
 */


#include<cstdio>
#include<list>
#include<queue>

#define pll pair<long,long> 
#define plA pair<long,acTrie>

using namespace std;

long fin,wFin,flag;

struct acTrie {
	long val;
	long wt;
	list<pll> indices;
	list<plA> children;
	acTrie *fail;
	int isFail;
}; 


void printAll(acTrie root, list<long> *string,int flag){
	
//printf("printAll v%ld ind%lu ch%lu\n",root.val,root.indices.size(),root.children.size());	
	if(root.children.empty()){
		if(root.isFail) return;
		list<long> :: iterator itn;
		list<pll> :: iterator itp;
		fin++;
		wFin+= (*string).size();
		if(flag<2){
			if(flag==-1) printf("%ld ",root.wt);
			
			itn=(*string).begin();
			printf("%ld ",*itn); itn++;
			while(itn!=(*string).end()){
				printf("%ld ",*itn);
				itn++;
			}
			printf("\n");
		
			if(flag!=-1){
				for(itp=root.indices.begin();itp!=root.indices.end();itp++)
					printf("(%ld,%ld)",(*itp).first,(*itp).second);
				printf("\n");
			}
		}
	}else{
		list<plA> :: iterator itc;

		for(itc=root.children.begin();itc!=root.children.end();itc++){
			(*string).push_back((*itc).first);
			printAll((*itc).second,string,flag);
			(*string).pop_back();
		}
	}

}

void add_Fail(acTrie *root){
	(*root).fail = root;
	
	acTrie *src,*tar;
	queue<acTrie *> q;
	q.push(root);

	list<plA> :: iterator its,itt;

	while(!q.empty()){
		src = q.front(); 
		q.pop();

		for(its=(*src).children.begin(); its!=(*src).children.end();its++){
			tar=src;
			(*its).second.fail=NULL;
			while(tar!=root){
				tar = (*tar).fail;
				
				for(itt=(*tar).children.begin();
			            itt!=(*tar).children.end();itt++){
					if((*its).first==(*itt).first){
						(*its).second.fail=&((*itt).second);
						(*itt).second.isFail=1;
					        tar=root;	
						break;
					}
				}
				
				

			}
			if((*its).second.fail==NULL) 
				(*its).second.fail=root;
			q.push(&((*its).second));
		}

	
	}
	
}


int main(int argc, char *argv[]){
   	// freopen("output.txt", "r", stdin);
    // freopen("acTrie_output.txt", "w", stdout);
	char line[30000];
	//int t=10;
	list<long> nodes;
	list<pll> indices;
	long graphC=0;
	long org, tOrg=0, tFin=0, maxd=0;
	long wOrg, wtOrg=0, wtFin=0, wMaxd=0;
	

	flag=0;
	if(argc==2){
		if(argv[1][0]=='a') flag=1;// all paths + stats
		if(argv[1][0]=='s') flag=2;// stats only	
		if(argv[1][0]=='f') flag=-1;// stats only	
	}

	while(scanf("%[^\n]",line) > 0){
		if(flag<2) printf("%s path = 100\n",line);
		org=wOrg=fin=wFin=0;
		graphC++;

		acTrie root, *curr;
		list<plA>::iterator it;

		while(1){
	        	curr = &root;	
			nodes.clear();
			indices.clear();
			long tmp,tmp2;
			long tmpf;
			char test;

			if(flag==-1){
				if(scanf("%ld",&tmpf)<=0) break;
			}


			while(scanf("%ld",&tmp)> 0){	
			//	printf("TEST C\n");
				nodes.push_back(tmp);
				for(it=(*curr).children.begin(); it!=(*curr).children.end();it++){
					if((*it).first == tmp){
						curr=&((*it).second); 
						break;	
					}
				}

				if(it==(*curr).children.end()){
					acTrie acn;
					acn.val= tmp;
					acn.isFail=0;
					(*curr).children.push_back(plA(tmp,acn));
					curr= &((*curr).children.back().second);
				}
				scanf("%c",&test);			
				if(test=='\n' || test=='\r') break;
			}
			
			if(nodes.size()==0) break;
			
			if(flag==-1){
				(*curr).wt=tmpf;				
			}
			else{
				while(scanf("(%ld, %ld)", &tmp,&tmp2)!=0) {
					(*curr).indices.push_back(pll(tmp,tmp2));
				}
			}
			org++;
			wOrg+= nodes.size();
			//printf("%lu nodes, %lu indices\n",
			//	nodes.size(),(*curr).indices.size());	

		}
		
		add_Fail(&root);
		list<long> str;
		printAll(root,&str,flag);	
		if(maxd<org-fin) maxd=org-fin;
		if(wMaxd<wOrg-wFin) wMaxd=wOrg-wFin;
		tOrg+=org;
		wtOrg+= wOrg;
		tFin+=fin;
		wtFin+=wFin;
		if(flag==1){
			printf("Orginal paths %ld, Final paths %ld, diff %ld\n", org,fin, org-fin );
			printf("Weight of Orginal paths %ld, Weight of Final paths %ld, diff %ld\n\n", wOrg,wFin, wOrg-wFin );
		}
		//if(!(--t)) return 0;
		
		
	}

	if(flag>0){
		printf("\n\nNumber of Graphs processed: %ld\n", graphC);
		printf("Total Original Paths %ld, Total final Paths %ld, max diff %ld\n", tOrg, tFin, maxd);
		printf("Total Weight of Original Paths %ld, Total weight of final paths %ld, max diff %ld\n", wtOrg, wtFin, wMaxd);
	}

	return 0;
}
