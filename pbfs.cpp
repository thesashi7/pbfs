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

using namespace std;

// short-cut for declaring a graph
typedef std::map<int, std::list<int> > adjacency_list;
typedef std::map<int, int > order_list;

void read_adjacency_list(adjacency_list&, char*);
void read_order_list(order_list&, char*);
std::vector<vector<int> > breadth_first_search_2(adjacency_list&, int, map<int, int>& );
std::map<int, int> breadth_first_search(adjacency_list&, int, map<int, int>& );
void print_graph( adjacency_list );
void print_orders( map<int, int>);
bool verify_2(vector<vector<int> >, vector<int>);
bool verify(map<int, int>, map<int, int>);


int main(int argc, char* argv[]) {
    
    if(argc != 3) {
        std::cout << "usage:  ./bfs  input_graph  src" << std::endl;
        return -1;
    }
    char* inputfile = argv[1];
    int src = atoi( argv[2] );
    int num_threads = atoi( argv[3] );
    
    adjacency_list graph;
    order_list order;
    
    read_adjacency_list(graph, inputfile);
    if( graph.find(src) == graph.end() ) {
        cout << "error: non-existant source" << endl;
        return -1;
    }
    print_graph(graph);
    map<int, int> vert_orders;
    
    map<int, int> node_level = breadth_first_search(graph, src, vert_orders);
    
    print_orders(vert_orders );
    
    // GENERATE ORDER FILE AND PRINT RUN-TIME AND NUMBER OF THREADS HERE
    
    return 0;
}


bool verify_2(vector<vector<int> > level_list, vector<int> order)
{
    bool is_correct;
    int level_i;
    is_correct = true;
    level_i = 0;
    
    for(int i=0; i < level_list.size(); i++)
    {
        vector<int> level;
        level = level_list[i];
        int cur_size;
        cur_size = level.size();
        if(level_i > order.size())
            return false;
        while(level_i < order.size())
        {
            int node = order[level_i];
            if (std::find(level.begin(), level.end(),node) == level.end())
            {
                return false;
            }
            level_i++;
        }
    }
    if(level_i != order.size())
        is_correct = false;
    return is_correct;
}

bool verify(map<int, int> node_level_s, map<int, int> node_level_p)
{
    bool is_correct;
    is_correct = true;
    
    if(node_level_s.size() == node_level_p.size())
    {
        for(int i=0; i<node_level_s.size(); i++)
        {
            if(node_level_s[i] != node_level_p[i])
                return false;
        }
    }
    else is_correct = false;
    return is_correct;
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

/* read an order list */
void read_order_list(order_list &g, char* orderfile) {
    
    ifstream infile;
    infile.open(orderfile);
    string line;
    
    int vertex, level;
    
    // for each line, get the vertex and the level
    while( getline(infile, line) ) {
        vertex = atoi( line.substr(0,1).c_str() );
        level = atoi( line.substr(2,3).c_str() );
        g.insert(make_pair(vertex, level));
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
std::vector<vector<int> > breadth_first_search_2( adjacency_list& graph, int src, map<int, int>& vert_orders) {
    std::vector<vector<int> > level_list;
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
        vector<int> level;
        queue<int> temp_queue;
        while(next_verts.size()>0)
        {
            int cur_node = next_verts.front();
            next_verts.pop();
            level.push_back(cur_node);
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
                        temp_queue.push(*it);
                        visited[(*it)-1] = true;
                    }
                }
            }
        }
        level_list.push_back(level);
        next_verts = temp_queue;
    }
    
    return level_list;
}

// perform breadth-first search
map<int, int> breadth_first_search( adjacency_list& graph, int src, map<int, int>& vert_orders) {
    map<int, int> node_level;
    
    // initialize visited and queue
    bool* visited = new bool[graph.size()];
    for(int i=0; i<graph.size(); i++)
        visited[i]=false;
    queue<int> next_verts;
    
    /*
     breadth-first search
     */
    int order = 1;
    int lev = 0;
    next_verts.push(src);
    visited[src-1] = true;
    while(next_verts.size()>0)
    {
        vector<int> level;
        queue<int> temp_queue;
        while(next_verts.size()>0)
        {
            int cur_node = next_verts.front();
            next_verts.pop();
            node_level.insert(std::make_pair(cur_node, lev));	
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
                        temp_queue.push(*it);
                        visited[(*it)-1] = true;
                    }
                }	
            }
        }
        next_verts = temp_queue;
        lev++;
        
    }
    
    return node_level;
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















