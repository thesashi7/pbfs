Daniel Acevedo (daacevedo) and Sashi Thapaliya (sbthapaliya)

Parallel Breadth-First Search

Compile Instructions for Verification File:
Compile: g++ verf.cpp -o verf
Execute: ./verf graph_file source_node order_file

Compile Instructions for Parallel Breadth-First Search:
Compile: srun -N1 —-qos=59903 g++ -fopenmp pbfs.cpp -o pbfs
Execute: srun -N1 —qos=59903 ./pbfs graph_file source_node number_of_threads

The results of the parallel BFS is stored in a text file, called pbfs_order.txt. We used this file as the order_file for the verification file.

We used many different graphs for testing purposes. We have included g.txt, the largest graph we used, in our submission.

An example execution:

srun -N1 —-qos=59903 g++ -fopenmp pbfs.cpp -o pbfs

srun -N1 —qos=59903 ./pbfs g.txt 0 2

g++ verf.cpp -o verf

./verf g.txt 0 pbfs_order.txt