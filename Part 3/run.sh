echo
echo ---------dijkstra is started----------
./sssp & tee -a terminal_output.out
echo ---------dijkstra is done-------------
echo
echo ---------dijkstra_omp is started----------
./sssp_omp |& tee -a terminal_output.out
echo ---------dijkstra_omp is done-------------