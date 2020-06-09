CXX = g++
CXXFLAGS = -std=c++2a -Wall -O3 -march=native -ffp-contract=fast -I/usr/local/include/optim
OBJECTS = main.o

main: $(OBJECTS)
		$(CXX) $(CXXFLAGS) -o optimLB $^
clean:
	rm $(OBJECTS) optimLB
$(OBJECTS): 
