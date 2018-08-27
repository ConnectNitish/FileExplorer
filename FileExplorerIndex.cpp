#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>	
#include <vector>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
using namespace std;

#include "MetaData.h"

#define ClearScreen() printf("\033[2J")
#define MoveUp() printf("\033[A")
#define MoveDown() printf("\033[B")
#define MoveLeft() printf("\033[C")
#define MoveRight() printf("\033[D")
#define SetCursor(x,y) printf("\033[%d;%dH",x,y)


string title = "File Explorer";
// By Default its Normal Mode 
static bool isNormalMode = 1; 
static const int STDIN = 0;
struct winsize size,currentRC;
char *currDir = NULL,*startingDirectory = NULL;
static vector<struct dirent *> fileList;
static vector<char *> navigationVector;

static int xdiraccess(const char *path);
void CursorFunctionality();
void GoToNextDirectory(int index);
void showCurrentDirectoryDetails();
void printAtLast(string message,int restore);
void GoToPreviousDirectory();

string FromCharacterPointerToString(char * Input)
{
	string temp;
	temp.append(Input);
	return temp;
}

string readable_fs(double spaceToConvert) {
    int i = 0;
    const char* bits[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (spaceToConvert > 1024) {
        spaceToConvert /= 1024;
        i++;
    }
	char buf[10];
    sprintf(buf, "%.*f%s", i, spaceToConvert, bits[i]);
	string sFileSize; sFileSize.append(buf);
    return sFileSize;
}

void ResetCursor()
{
	currentRC.ws_row=STARTTINGROW;
	currentRC.ws_col=1;
}

string PrintTime()
{
   	time_t now = time(0);   
   	// convert now to string form
   	char* dt = ctime(&now);
	string strDateTime; strDateTime.append(dt);
	return strDateTime;
}

ofstream myfile;

void writeToFile(string message)
{
  	myfile.open("test.txt", std::ios_base::app);
  	string strFinalLog;
  	strFinalLog = PrintTime();
  	strFinalLog.append("(");
	strFinalLog.append(to_string(currentRC.ws_row));
	strFinalLog.append(",");
	strFinalLog.append(to_string(currentRC.ws_col));
  	strFinalLog.append(")");
  	myfile << strFinalLog << " : " << message << endl;
  	myfile.close();
}

void SetHeaderPath(char * currDir)
{
	SetCursor(3,1);
	if(currDir!=NULL)
	{
		if(xdiraccess(currDir))
		{		
			printf("DIR : %s\n\n",currDir);
			ResetCursor();
		}
	}
	else
		printAtLast("Some Error Encountered",1);
}

void SetWelcomeScreen()
{
		SetCursor(1,1);	
		int mid = size.ws_col/2;
		//string name = "File Explorer";
		SetCursor(1,mid-(title.length()/2));
		cout << title;
		SetCursor(2,mid/2);
		int totalLength = (mid + (mid/2)) - (mid/2);
		for(int i=(mid/2);i<mid/2 + totalLength;i++)
		{
			printf("*");
		}
		printf("\n");	
		if(currDir==NULL)
		{
			currDir = getenv("PWD");
			startingDirectory = currDir;
			navigationVector.push_back(currDir);
		}
		SetHeaderPath(currDir);
}

void printAtLast(string message,int restore)
{
	
	if(restore)
	{
		int rowI = currentRC.ws_row;
		int colI = currentRC.ws_col;
		SetCursor(size.ws_row-2,1);	
		cout << message;
		SetCursor(rowI,colI);	
		return; 
	}
	else
	{
		SetCursor(size.ws_row-1,1);	
		cout << message;
	}	
}


static int xdiraccess(const char *path)
{
	static DIR *dirp;
	dirp = opendir(path);
	if (dirp == NULL) {
		printAtLast("Directory doea not have Access to Open",1);
		return 0;
	}
	closedir(dirp);
	return 1;
}

void GetCurrentDirectoryDetails(char * currDir)
{
	printAtLast("AAAA",1);
	//ResetCursor();
	if(currDir!=NULL)
	{
		fileList.clear();
		DIR *dp = opendir((const char*)currDir);
		struct dirent *deptr = NULL;
		struct stat filestat;
		if(dp == NULL){
			printAtLast("Error: Could not open the current working directory",1);
			ResetCursor();
		}
		else
		{
			struct entry *temp = (struct entry *)malloc(sizeof(struct entry *));			
			while((deptr = readdir(dp)) != NULL){
				char * name = deptr->d_name;
				string strTemp; strTemp.append(name);
				//if(strTemp.length()==1 && strTemp[0]=='.') continue;
					fileList.push_back(deptr);
			}
			showCurrentDirectoryDetails();
		}
	}
}

void showCurrentDirectoryDetails(){
	/*
	i.	File Name
	ii. File size (Human readable format similar to ls -lh)
	iii. Ownership (User & Group) & Permissions
	iv. Last modified
	*/
	string s ="";
	// Make Header
	currentRC.ws_col = 3; 
	SetCursor(currentRC.ws_row,currentRC.ws_col);
	s = "File Name"; s.append(28 - s.length(), ' '); cout << s ;
	s = "File Size"; s.append(20 - s.length(), ' '); cout << s ;
	s = "OwnerShip"; s.append(20 - s.length(), ' '); cout << s ;
	s = "Last Modified"; s.append(20 - s.length(), ' '); cout << s << endl;
	cout << endl;	
	currentRC.ws_row = currentRC.ws_row + 2;	
	struct dirent *deptr = NULL;
	string strHeader = "File Name\t\tFile Size\tOwnerShip\tLast Modified\n";
	struct stat filestat;
	for(int i=0;i<fileList.size();i++)
	{
		int isFile = 0;
		deptr = fileList[i];
		stat(deptr->d_name, &filestat);
		SetCursor(currentRC.ws_row,1);
		if(S_ISREG(filestat.st_mode))
			isFile = 1;
		
		if(!isFile)
			cout << ">";
			
		SetCursor(currentRC.ws_row,currentRC.ws_col);
		cout << deptr->d_name;
		
		string ownerdetails = "";
		ownerdetails += ((filestat.st_mode & S_IRUSR )? "r":"-");
    	ownerdetails += ((filestat.st_mode & S_IWUSR )? "w":"-");
		ownerdetails += ((filestat.st_mode & S_IXUSR )? "x":"-");
		
		ownerdetails += ((filestat.st_mode & S_IRGRP )? "r":"-");
		ownerdetails += ((filestat.st_mode & S_IWGRP )? "w":"-");
		ownerdetails += ((filestat.st_mode & S_IXGRP )? "x":"-");
		
		ownerdetails += ((filestat.st_mode & S_IROTH )? "r":"-");
		ownerdetails += ((filestat.st_mode & S_IWOTH )? "w":"-");
		ownerdetails += ((filestat.st_mode & S_IXOTH )? "x":"-");
		
		string fileSize;
		fileSize = readable_fs((double)filestat.st_size);

		SetCursor(currentRC.ws_row,30); cout << fileSize;
		SetCursor(currentRC.ws_row,50); cout <<  ownerdetails;
		SetCursor(currentRC.ws_row,70); cout << ctime(&filestat.st_mtime) << endl;

		currentRC.ws_row++;
	}	
	ResetCursor();
	currentRC.ws_row = currentRC.ws_row+2 ;
	SetCursor(currentRC.ws_row,currentRC.ws_col);
}

void redraw(int mainCalling)
{
	if(mainCalling)
		ClearScreen();
	//printAtLast(" REDRAW ---- ",1);
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);	

	
	if(!navigationVector.empty())
	{
		printAtLast("Vector is Not Empty",1);
		currDir = navigationVector.back();
	}
	else
	{
		printAtLast("Vector is Empty",1);
	}
	//if(!xdiraccess(currDir))
	//	return;
	
	SetWelcomeScreen();	
	GetCurrentDirectoryDetails(currDir);
	
	ResetCursor();
	currentRC.ws_row = currentRC.ws_row+2 ;
	SetCursor(currentRC.ws_row,currentRC.ws_col);
	
	if(isNormalMode)
		CursorFunctionality();
}

int main()
{	
	redraw(1);	
	return 0;
}

void CursorFunctionality()
{
	struct termios oldattr, newattr;
	int ch = 0;
    //set terminal
    tcgetattr( STDIN, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN, TCSANOW, &newattr );
    setbuf(stdin, NULL);
	int cursorright =COLUMNWIDTH,cursorleft =1;
	
	while ( ( ch = getchar ()) != 'q') 
	{
		int rowI = currentRC.ws_row;
		int colI = currentRC.ws_col;
		if ( ch == ARROWUP) 
		{
			printf ( "\033[A");//cursor up
			currentRC.ws_row--;
        }
        if ( ch == ARROWRIGHT) {
            printf ( "\033[C");//cursor right
			cursorleft++;
			cursorright--;			
			GoToNextDirectory(rowI-STARTTINGROWDATA);
			//currentRC.ws_col = cursorleft;
			SetCursor(rowI,colI);
        }
        if ( ch == ARROWLEFT) {
            printf ( "\033[D");//cursor left
			cursorleft--;
			cursorright++;
			GoToPreviousDirectory();
			SetCursor(rowI,colI);
			//currentRC.ws_col = cursorleft;
        }
        if ( ch == ARROWDOWN) {
				printf ( "\033[B");//cursor down
				currentRC.ws_row++; 
				
			//}
        }
		if ( ch == '\n') {
			writeToFile("&&&& " + to_string(currentRC.ws_row));
		}
		if ( ch == ':') {
			isNormalMode = 0;
			printf (":");
		}
		if ( ch == 'h' || ch == 'H') {
			currDir = startingDirectory;
			navigationVector.push_back(currDir);
			redraw(1);
		}
	}
	
	if ( ch == 'q') {
			printf ( "\033[2J");
    }
	
	tcsetattr( STDIN, TCSANOW, &oldattr );
}

char * GetPreviousDirectoryPath()
{
	if(navigationVector.size()<=1) 
	{
		printAtLast("&&&&!!!!!!!",1);
		return NULL;
	}
	else
	{
		char * currentDirectory = navigationVector.back();
		navigationVector.pop_back();
		char * previousDirectory = navigationVector.back();
		return previousDirectory;
	}
}

void GoToNextDirectory(int index)
{
	if(currDir!=NULL)
	{
		struct dirent *deptr = NULL;
		struct stat filestat;
		deptr = fileList[index];
		stat(deptr->d_name, &filestat);
        char * p;
        p = (char *)malloc(strlen(deptr->d_name) + strlen(currDir) + 1 + 1);
        strcpy(p,currDir);
        strcat(p,"/");
        strcat(p,deptr->d_name);
		navigationVector.push_back(p);
		redraw(1);
		free(p);
	}
}

void GoToPreviousDirectory()
{
	char * prevDir = GetPreviousDirectoryPath();
	if(prevDir!=NULL)
	{	
		redraw(1);
	}
	else 
		printAtLast("No Previous Directory Exist",1);
}