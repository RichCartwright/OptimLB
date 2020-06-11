CXX = g++
CXXFLAGS = -std=c++2a -Wall -O3 -march=native -ffp-contract=fast -I/usr/local/include/optim -I/usr/include/python3.8
LIBFLAG = -L/usr/local/lib -loptim -fopenmp -lpython3.8
OBJECTS = main.o

main: $(OBJECTS)
		$(CXX) $(CXXFLAGS) -o optimLB $^ $(LIBFLAG)
clean:
	rm $(OBJECTS) optimLB
$(OBJECTS): 
