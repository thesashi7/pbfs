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
void read_graph(adjacency_list&, char*);
void read_order_list(order_list&, char*);
std::map<int, int> breadth_first_search(adjacency_list&, int );
void print_graph( adjacency_list );
void print_orders( map<int, int>);
bool verify(map<int, int>, map<int, int>);
std::vector<std::string> split(string, char);

int main(int argc, char* argv[]) {

    if(argc < 3) {
        std::cout << "usage:  ./bfs  input_graph  src" << std::endl;
        return -1;
    }
    char* inputfile = argv[1];
    int src = atoi( argv[2] );
    char* orderfile = argv[3];

    adjacency_list graph;
    order_list order;
    read_adjacency_list(graph, inputfile);
  //print_graph(graph);
    if( graph.find(src) == graph.end() ) {
        cout << "error: non-existant source" << endl;
        return -1;
    }
    read_order_list(order, orderfile);
    map<int, int> vert_orders;
    map<int, int> node_level = breadth_first_search(graph, src);
   // print_orders(node_level);
    // Verification and OUTPUT GETS PRINTED HERE
    if(verify(node_level, order))
    {
        std::cout<<"True\n";
    }
    else
    {
        std::cout<<"False\n";
    }
    return 0;
}

bool verify(map<int, int> node_level_s, map<int, int> node_level_p)
{
    bool is_correct;
    is_correct = true;
    if(node_level_s.size() == node_level_p.size())
    {
      for(std::map<int,int>::iterator iter = node_level_s.begin();
          iter != node_level_s.end(); ++iter)
      {
        int k =  iter->first;
        if(iter->second != node_level_p[k])
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

    int v1;
    std::list<int> adjacent_verts;

    // for each line, get numbers after a space
    while( getline(infile, line) ) {
        adjacent_verts.clear();
        vector<string> splited = split(line,' ');
        v1 = atoi( splited[0].c_str() );
        for(int j=1; j<splited.size(); j++)
        {
          adjacent_verts.push_back(atoi(splited[j].c_str()));
        }
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
      vector<string> splited = split(line,' ');
      vertex = atoi( splited[0].c_str() );
      level = atoi( splited[1].c_str() );
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
map<int, int> breadth_first_search( adjacency_list& graph, int src) {
    map<int, int> node_level;

    // initialize visited and queue
    map<int, bool>visited;
    queue<int> next_verts;

    /*
     breadth-first search
     */
    int order = 1;
    int lev = 0;
    next_verts.push(src);
    visited.insert(std::make_pair(src, lev));
    while(next_verts.size()>0)
    {
        vector<int> level;
        queue<int> temp_queue;
        while(next_verts.size()>0)
        {
            int cur_node = next_verts.front();
            next_verts.pop();
            node_level.insert(std::make_pair(cur_node, lev));
            list<int>nb;
            map<int, std::list<int> >::iterator it = graph.find(cur_node);
            if(it != graph.end())
            {
                nb = it->second;
                std::list<int>::iterator it;
                for (it = nb.begin(); it != nb.end(); ++it)
                {
                    if(visited.find(*it) == visited.end())
                    {
                        temp_queue.push(*it);
                        visited.insert(std::make_pair(*it, 1));
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

vector<string> split(string str, char delimiter) {
  std::vector<std::string> splited;
  std::size_t pos = 0, found;
  while((found = str.find_first_of(delimiter, pos)) != std::string::npos) {
     splited.push_back(str.substr(pos, found - pos));
     pos = found+1;
  }
  splited.push_back(str.substr(pos));

  return splited;
}
