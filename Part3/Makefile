INC="./inc"
FLAGS=-I$(INC)
OMPFLAG=-fopenmp
CC=gcc-12
P1=sssp
P2=sssp_omp

all: $(P1) $(P2)
$(P1): $(P1).c
	$(CC) $(CFLAGS) $(OMPFLAG) $(P1).c -o $(P1)
$(P2): $(P2).c
	$(CC) $(CFLAGS) $(OMPFLAG) $(P2).c -o $(P2)