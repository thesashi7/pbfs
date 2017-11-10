Daniel Acevedo (daacevedo) and Sashi Thapaliya (sbthapaliya)

Parallel Breadth-First Search

Compile Instructions for Verification File:
Compile: srun -N1 --qos=59903 g++ verf.cpp -o verf
Execute: srun -N1 -—qos=59903 ./verf graph_file source_node order_file

Compile Instructions for Parallel Breadth-First Search:
Compile: srun -N1 —-qos=59903 g++ -fopenmp pbfs.cpp -o pbfs
Execute: srun -N1 —qos=59903 ./pbfs graph_file source_node number_of_threads