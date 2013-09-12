include ../Makefile.in
PROGNAME=polygon
OUTNAME=${PROGNAME}.exe

SRC=\
polygon.cpp

OBJ=$(SRC:.cpp=.o)

$(OUTNAME): $(OBJ)
	$(CXX) -g $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS) 

*.o: *.cpp
	$(CXX) -g -c $(CXXFLAGS) $*.cpp -o $@ 

clean:
	rm -f *.exe
	rm -f *.o


