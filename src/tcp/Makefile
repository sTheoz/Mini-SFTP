CXX = gcc
CXXFLAGS = -Wall
SRC = 
OBJ = $(SRC:.cc=.o)

all: srv clt

srv: ft-tcp-srv.o
	$(CXX) -o $@ ft-tcp-srv.o

clt: ft-tcp-clt.o
	$(CXX) -o $@ ft-tcp-clt.o

clean:
	rm -rf $(OBJ) $(EXEC)