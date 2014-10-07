CC = /opt/local/bin/g++
OPENMP = -fopenmp
EIGEN = /users/katemcardle/Dropbox/EclipseLib/Eigen
CFLAGS = -g -Wall
LIBS = 

TARGET = omp-als

all: $(TARGET)

omp-als: HW2_Prob2.o
	$(CC) -o $@ $(LIBS) $(OPENMP) HW2_Prob2.o

HW2_Prob2.o: HW2_Prob2.cpp
	$(CC) -c $(OPENMP) -I $(EIGEN) $(CFLAGS) HW2_Prob2.cpp

clean:
	rm -f *.o $(TARGET)

test:
	./$(TARGET) 10 1 8 ../data/small
