C=g++

FLAGS = -c -w

#.PHONY: all run

all:
	@$(C) $(FLAGS) FileExplorerIndex.cpp Deletefile.cpp Copy.cpp SnapShot.cpp
	@$(C) -o FileExplorerIndex FileExplorerIndex.o Deletefile.o Copy.o SnapShot.o
	@chmod a+x FileExplorerIndex
	@chmod a+r FileExplorerIndex
	@chmod a+w FileExplorerIndex

run: 
	@./FileExplorerIndex
