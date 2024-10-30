#PA2 cpp compiler
.PHONY: all run clean
all: PA2_110521086.o PA2_function.o
		@g++ -std=c++11 PA2_110521086.o PA2_function.o -o exe
run: 
		@./exe $(input) $(output)
clean:
		@rm *.o
		@rm exe
PA2_110521086.o:PA2_110521086.cpp
		@g++ -std=c++11 -c PA2_110521086.cpp
PA2_function.o:PA2_head.h PA2_function.cpp
		@g++ -std=c++11 -c PA2_head.h PA2_function.cpp