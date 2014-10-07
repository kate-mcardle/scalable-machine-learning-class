CC = /opt/local/bin/g++
OPENMP = -fopenmp
#EIGEN = /users/katemcardle/Dropbox/EclipseLib/Eigen
CFLAGS = -g -Wall
LIBS = 

TARGET = cd-svm

all: $(TARGET)

cd-svm: HW3.o
	$(CC) -o $@ $(LIBS) $(OPENMP) HW3.o

HW3.o: HW3.cpp
	$(CC) -c $(OPENMP) $(CFLAGS) HW3.cpp

clean:
	rm -f *.o $(TARGET)

test:
	./$(TARGET) 0.1 8 ../data/covtype.tr ../data/covtype.t
