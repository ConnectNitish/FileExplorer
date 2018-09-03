C=g++

FLAGS = -c -w

RM=/bin/rm -f

#.PHONY: all run

all:main

main.o:	InvokeCommands.h FileExplorerIndex.cpp
	@$(C) $(FLAGS) FileExplorerIndex.cpp 

others.o: Copy.h SnapShot.h Copy.cpp SnapShot.cpp 
	@$(C) $(FLAGS) Copy.cpp SnapShot.cpp

delete.o: Delete.h Deletefile.cpp
	@$(C) $(FLAGS) Deletefile.cpp 

movefile.o: MoveFile.h MoveFile.cpp
	@$(C) $(FLAGS) MoveFile.cpp 

main: main.o delete.o others.o movefile.o
	@$(C) -o FileExplorerIndex FileExplorerIndex.o Deletefile.o Copy.o SnapShot.o MoveFile.o

run: 
	@./FileExplorerIndex
