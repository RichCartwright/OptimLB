CXX = g++
CXXFLAGS = -std=c++2a -Wall -O3 -march=native -ffp-contract=fast -I/usr/local/include/optim
LIBFLAG = -L/usr/local/lib -loptim -fopenmp
OBJECTS = main.o

main: $(OBJECTS)
		$(CXX) $(CXXFLAGS) -o optimLB $^ $(LIBFLAG)
clean:
	rm $(OBJECTS) optimLB
$(OBJECTS): 
