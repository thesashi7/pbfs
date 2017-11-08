#include<iostream>
#include<map>
//#include<vector>
#include<list>
#include<cstdlib>
#include<fstream>
#include<string>
#include<queue>
#include<map>
#include <sys/time.h>

using namespace std;

// short-cut for declaring a graph
typedef std::map<int, std::list<int> > adjacency_list;
typedef unsigned long long uint64; //for timing

void read_adjacency_list(adjacency_list&, char*);
void breadth_first_search(adjacency_list&, int, map<int, int>& );
void print_graph( adjacency_list );
void print_orders( map<int, int>);
uint64 getTimeMs64();

int main(int argc, char* argv[]) {

	if(argc != 3) {
		std::cout << "usage:  ./bfs  input_graph  src" << std::endl;
		return -1;
	}
	char* inputfile = argv[1];
	int src = atoi( argv[2] );

	adjacency_list graph;
	read_adjacency_list(graph, inputfile);
	if( graph.find(src) == graph.end() ) {
		cout << "error: non-existant source" << endl;
		return -1;
	}
 	print_graph(graph);
	map<int, int> vert_orders;

	breadth_first_search(graph, src, vert_orders);

	print_orders(vert_orders );

	return 0;
}

/* read an adjacency list */
void read_adjacency_list(adjacency_list &g, char* inputfile) {

	std::ifstream infile;
	infile.open(inputfile);
	std::string line;

	int v1,v2;
	std::list<int> adjacent_verts;	

	// for each line, get numbers after a space
	while( getline(infile, line) ) {
		adjacent_verts.clear();
		int a=0;
		int b=0;
		int v=0;
		string v_str;
		bool v1_found=false;
		for(int i=0; i<line.length(); i++) {			
			if( line[i]==' ') { // if the vertex has neighbors
				b=i;
				v_str = line.substr(a,b);
				v = atoi(v_str.c_str() ); // convert variable v_str to a c-string
				if( v1_found == false) {
					v1 = v;
					v1_found=true;
				}
				else
					adjacent_verts.push_back(v);
				a=b+1;
			}
		}
		v_str = line.substr(a,b);
		v=atoi(v_str.c_str() ); // convert variable v_str to a c-string
		if( v1_found == false) 
				v1 = v;
		else
			adjacent_verts.push_back(v);	
		g.insert( std::make_pair( v1, adjacent_verts ) );

	}

	infile.close();

	return;
}

void print_graph( adjacency_list g ) {

	adjacency_list::iterator g_itr;
	list<int> row;
	list<int>::iterator row_itr;
	for( g_itr = g.begin(); g_itr != g.end(); ++g_itr ) {

		int v = (*g_itr).first;
		cout << v << ": ";

		row = (*g_itr).second;
		for( row_itr = row.begin(); row_itr != row.end(); ++row_itr ) {
			cout << *row_itr << " ";
		}
		cout << endl;
	}
	cout << endl;

	return;
}

// perform breadth-first search
void breadth_first_search( adjacency_list& graph, int src, map<int, int>& vert_orders) {

    uint64 time_start, time_stop, runtime;
    time_start = getTimeMs64();
    
  // initialize visited and queue
	bool* visited = new bool[graph.size()];
	for(int i=0; i<graph.size(); i++) 
		visited[i]=false;
	queue<int> next_verts;

	/*
		breadth-first search
  */
	int order = 1;
	next_verts.push(src);
	visited[src-1] = true; 
	while(next_verts.size()>0)
	{
		int cur_node = next_verts.front();
		next_verts.pop();
  	vert_orders.insert(std::make_pair(cur_node, order++));
    list<int>nb;
		map<int, std::list<int> >::iterator it = graph.find(cur_node);
		if(it != graph.end())
		{
		 	nb = it->second;
			std::list<int>::iterator it;
			for (it = nb.begin(); it != nb.end(); ++it)
			{
					if(visited[(*it)-1] == false)
					{
						next_verts.push(*it);
						visited[(*it)-1] = true;
					}
			}

		}
	}

    time_stop = getTimeMs64();
    runtime = (time_stop - time_start) / 10;
    cout << "The runtime is: " << runtime << " ms" << endl;
    
	return;
}

// print the final orders
void print_orders( map<int, int> vert_orders ) {
	cout << "vert_id\torder" << endl;
	map<int, int>::iterator map_itr;
	for(map_itr = vert_orders.begin(); map_itr!=vert_orders.end(); ++map_itr) {
		cout << (*map_itr).first << "\t" << (*map_itr).second << endl;
	}
	return;
}

/* Get system time */
uint64 getTimeMs64()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64 ret = tv.tv_usec;
    // convert micro secs (10^-6) to millisecs (10^-3)
    ret /= 1000;
    
    // add seconds (10^0) after converting to millisecs (10^-3)
    ret += (tv.tv_sec*1000);
    return ret;
}














