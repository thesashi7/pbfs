#include<iostream>
#include<map>
//#include<vector>
#include<list>
#include<cstdlib>
#include<fstream>
#include<string>
#include<queue>
#include<map>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <mutex>

using namespace std;

// short-cut for declaring a graph
typedef std::map<int, std::list<int> > adjacency_list;

void read_adjacency_list(adjacency_list&, char*);
std::map<int, int> parallel_breadth_first_search(adjacency_list&, int, map<int, int>& );
void print_graph( adjacency_list );
void print_orders( map<int, int>);
void generate_order_file(map<int, int>);


int main(int argc, char* argv[]) {
    
    if(argc != 4) {
        std::cout << "usage:  ./bfs  input_graph  src num_threads" << std::endl;
        return -1;
    }
    char* inputfile = argv[1];
    int src = atoi( argv[2] );
    int num_threads = atoi( argv[3] );
    
    omp_set_num_threads(num_threads);
    
    adjacency_list graph;
    order_list order;
    
    read_adjacency_list(graph, inputfile);
    if( graph.find(src) == graph.end() ) {
        cout << "error: non-existant source" << endl;
        return -1;
    }
    map<int, int> vert_orders;
    
    map<int, int> node_level = parallel_breadth_first_search(graph, src, vert_orders);
    
    generate_order_file(node_level);
    
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

// perform parallel breadth-first search
map<int, int> parallel_breadth_first_search( adjacency_list& graph, int src, map<int, int>& vert_orders) {
    map<int, int> node_level;
    
    mutex l;
    mutex k;
    
    // initialize visited and queue
    bool* visited = new bool[graph.size()];
    #pragma omp parallel for
    for(int i=0; i<graph.size(); i++)
        visited[i]=false;
    queue<int> next_verts;
    
    /*
     parallel breadth-first search
     */
    int order = 1;
    int lev = 0;
    next_verts.push(src);
    visited[src-1] = true;
    while(next_verts.size()>0)
    {
        vector<int> level;
        queue<int> temp_queue;
        #pragma omp critical
        while(next_verts.size()>0)
        {
            l.lock();
            int cur_node = next_verts.front();
            next_verts.pop();
            l.unlock();
            
            node_level.insert(std::make_pair(cur_node, lev));	
            vert_orders.insert(std::make_pair(cur_node, order++));
            list<int>nb;
            map<int, std::list<int> >::iterator it = graph.find(cur_node);
            
            if(it != graph.end())
            {
                l.lock();
                nb = it->second;
                l.unlock();
                
                std::list<int>::iterator it;
                for (it = nb.begin(); it != nb.end(); ++it)
                {
                    l.lock();
                    if(visited[(*it)-1] == false)
                    {
                        k.lock();
                        temp_queue.push(*it);
                        k.unlock();
                        k.lock();
                        visited[(*it)-1] = true;
                        k.unlock();
                    }
                    l.unlock();
                }	
            }
        }
        #pragma omp barrier
        next_verts = temp_queue;
        lev++;
        
    }
    
    return node_level;
}

void generate_order_file(map<int, int> m) {
    ofstream ofs;
    ofs.open("pbfs_order.txt", ofstream::out);
    
    
    for(map<int, int>::iterator iter = m.begin(); iter != m.end(); ++iter)
        ofs << iter->first << " " << iter->second << "\n";
    
    ofs.close();
    
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














