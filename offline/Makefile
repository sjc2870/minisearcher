INC_DIR:= ./include/
SRC_DIR:= ./src/
EXE_DIR:=./bin/
SRCS:=$(wildcard $(SRC_DIR)*.cc)
#OBJS:= $(patsubst %.cc, %.o, $(SRCS))
OBJS:=$(SRCS:.cc=.o)
LIBS:= -lpthread -ljson

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I, $(INC_DIR)) $(LIBS) 

#EXE:=$(EXE_DIR)server.exe
EXE:=$(addprefix $(EXE_DIR),server.exe)

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)
	@echo $(SRCS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
