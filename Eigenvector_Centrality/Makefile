CC = /opt/local/bin/g++
OPENMP = -fopenmp
CFLAGS = -g -Wall
LIBS = 

TARGET = spmat

all: $(TARGET)

spmat: HW4.o SparseMatrix.o
	$(CC) -o $@ $(LIBS) $(OPENMP) HW4.o SparseMatrix.o

HW4.o: HW4.cpp SparseMatrix.h
	$(CC) -c $(OPENMP) $(CFLAGS) HW4.cpp

SparseMatrix.o: SparseMatrix.h SparseMatrix.cpp
	$(CC) -c $(OPENMP) $(CFLAGS) SparseMatrix.cpp

clean:
	rm -f *.o $(TARGET)

test:
	./$(TARGET) ../A.txt
