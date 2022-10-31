echo
echo ---------scan is started----------
./scan.exe |& tee -a terminal_output.out
echo ---------scan is done-------------
echo
echo ---------scan_omp is started----------
./scan_omp.exe |& tee -a terminal_output.out
echo ---------scan_omp is done-------------
