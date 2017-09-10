


#specify target
all: predictors

predictors: predictors.o
	    g++ -std=c++11 predictors.o -o predictors

predictors.o: predictors.cpp
	      g++ -std=c++11 -c predictors.cpp

clean:
	rm -f *.o predictors
