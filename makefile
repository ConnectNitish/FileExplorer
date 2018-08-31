C=g++

FLAGS = -c -Wall

#.PHONY: all run

all:
	@$(C) $(FLAGS) FileExplorerIndex.cpp 
	@$(C) -o FileExplorerIndex FileExplorerIndex.o
	@chmod a+x FileExplorerIndex
	@chmod a+r FileExplorerIndex
	@chmod a+w FileExplorerIndex

run: 
	@./FileExplorerIndex
