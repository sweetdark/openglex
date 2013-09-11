include ../Makefile.in
PROGNAME=triangle
OUTNAME=$(PROGNAME).exe

SRC=\
triangle.cpp

OBJ= $(SRC:.cpp=.o)


$(OUTNAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)
*.o: *.cpp
	@echo \< $*.cpp\>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
	

clean:
	rm -f *.exe
	rm -f *.o

