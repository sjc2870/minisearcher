INC_DIR:= ./include/
SRC_DIR:= ./src/server/
EXE_DIR:=./bin
SRCS:=$(wildcard *.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -ljson

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I, $(INC_DIR)) $(LIBS) 

EXE:=./bin/server.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
