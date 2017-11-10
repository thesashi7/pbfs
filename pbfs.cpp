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
#include <sys/time.h>

using namespace std;

// short-cut for declaring a graph
typedef std::map<int, std::list<int> > adjacency_list;
typedef unsigned long long uint64; //for timing

void read_adjacency_list(adjacency_list&, char*);
std::map<int, int> parallel_breadth_first_search(adjacency_list&, int);
void print_graph( adjacency_list );
void print_orders( map<int, int>);
void generate_order_file(map<int, int>);
uint64 getTimeMs64();
std::vector<std::string> split(string, char);

int main(int argc, char* argv[]) {

    if(argc != 4) {
        std::cout << "usage:  ./pbfs  input_graph  src num_threads" << std::endl;
        return -1;
    }
    char* inputfile = argv[1];
    int src = atoi( argv[2] );
    int num_threads = atoi( argv[3] );

    omp_set_num_threads(num_threads);

    adjacency_list graph;

    read_adjacency_list(graph, inputfile);
    if( graph.find(src) == graph.end() ) {
        cout << "error: non-existant source" << endl;
        return -1;
    }
    uint64 time_start, time_stop, runtime;
    time_start = getTimeMs64();
    map<int, int> node_level = parallel_breadth_first_search(graph, src);
    time_stop = getTimeMs64();
    runtime = (time_stop - time_start) ;
    cout << "The runtime is: " << runtime << " ms" << endl;

    cout << "The number of threads is: " << num_threads << endl;
    generate_order_file(node_level);

    return 0;
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
map<int, int> parallel_breadth_first_search( adjacency_list& graph, int src) {
 map<int, int> node_level;
    omp_lock_t l;
    omp_lock_t k;
    omp_lock_t m;
    omp_lock_t n;
    omp_init_lock(&l);
    omp_init_lock(&k);
    omp_init_lock(&m);
    omp_init_lock(&n);
    // initialize visited and queue
    map<int, bool>visited;
    queue<int> next_verts;

    /*
     parallel breadth-first search
     */
    int order = 1;
    int lev = 0;
    next_verts.push(src);
    visited.insert(std::make_pair(src, lev));
    while(next_verts.size()>0)
    {
        vector<int> level;
        vector<int> temp_list;
        while(next_verts.size() > 0)
        {
          temp_list.push_back(next_verts.front());
          next_verts.pop();
        }
        #pragma omp parallel
        for(int i=0; i<temp_list.size(); i++)
        {
            //no need to lock this cuz its already divided to different threads by
            // openmp
            int cur_node = temp_list[i];
            omp_set_lock(&n);
            node_level.insert(std::make_pair(cur_node, lev));
            omp_unset_lock(&n);
            list<int>nb;

            map<int, std::list<int> >::iterator it = graph.find(cur_node);


            if(it != graph.end())
            {
                // 'it' is local for each thread
                nb = it->second;

                std::list<int>::iterator it;
                for (it = nb.begin(); it != nb.end(); ++it)
                {
                    omp_set_lock(&l);
                    if(visited.find(*it) == visited.end())
                    {
                        omp_set_lock(&m);
                        if(visited.find(*it) == visited.end())
                          visited.insert(std::make_pair(*it, 1));
                        omp_unset_lock(&m);
                        if(visited.find(*it) != visited.end())
                        {
                        omp_set_lock(&k);
                        next_verts.push(*it);
                        omp_unset_lock(&k);
                        }
                    }
                    omp_unset_lock(&l);
                }
            }
        }
        #pragma omp barrier
        //next_verts = temp_queue;
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

/* Get system time */
uint64 getTimeMs64() {

    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64 ret = tv.tv_usec;
    // convert micro secs (10^-6) to millisecs (10^-3)
    ret /= 1000;

    // add seconds (10^0) after converting to millisecs (10^-3)
    ret += (tv.tv_sec*1000);
    return ret;
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
