include ../Makefile.in
PROGNAME=ogl1step
OUTNAME=$(PROGNAME).exe

SRC=\
tutorial01.cpp

OBJ= $(SRC:.cpp=.o)

$(OUTNAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f *.exe
	rm -f *.o

toturial01.o: toturial01.cpp
	@echo \< $*.cpp\>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@
