
SRC:=$(wildcard *.cc)
TARGETS:=$(patsubst %.cc, %, $(SRC))
all:$(TARGETS)
%:%.cc
	g++ $< -o $@ -lrt
	@echo 正在编译...
.PHONY:clean
clean:
	rm -rf $(TARGETS)

