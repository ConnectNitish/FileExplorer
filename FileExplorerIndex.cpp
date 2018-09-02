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
#include <algorithm>
#include <regex>
#include <signal.h>

#include	<pwd.h>
#include	<grp.h>
#include   <string.h>
#include	<fcntl.h>

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
char * UpFolderPath = NULL;
static vector<struct dirent *> fileList;
static vector<char *> navigationVector;
int isScrollingEnabled = 0;
static int xdiraccess(const char *path);
void CursorFunctionality();
void GoToNextDirectory(int index);
void showCurrentDirectoryDetails(int,int,int);
void printAtLast(string message,int restore);
void GoToPreviousDirectory();
void GoToUpFolder();
int EnableUpScrolling();
int EnableDownScrolling();
void GetCommandLine(string sInput);
int invokeCommands(string strCommand);
size_t split(const string &txt, vector<string> &strs, char ch);
void GetCommandLine(string str);

static int    terminal_descriptor = -1;
static struct termios terminal_original;
static struct termios terminal_settings;

void setCommandMode();

/* Restore terminal to original settings
*/
static void terminal_done(void)
{
    if (terminal_descriptor != -1)
        tcsetattr(terminal_descriptor, TCSANOW, &terminal_original);
    terminal_descriptor  = -1;
}
 

static int terminal_init(void)
{
   // struct sigaction act;
 
    /* Already initialized? */
    if (terminal_descriptor != -1)
        return errno = 0;
 
    /* Which standard stream is connected to our TTY? */
    if (isatty(STDERR_FILENO))
        terminal_descriptor = STDERR_FILENO;
    else
    if (isatty(STDIN_FILENO))
        terminal_descriptor = STDIN_FILENO;
    else
    if (isatty(STDOUT_FILENO))
        terminal_descriptor = STDOUT_FILENO;
    else
        return errno = ENOTTY;
 
    /* Obtain terminal settings. */
    if (tcgetattr(terminal_descriptor, &terminal_original) ||
        tcgetattr(terminal_descriptor, &terminal_settings))
        return errno = ENOTSUP;
 
    /* Disable buffering for terminal streams. */
    if (isatty(STDIN_FILENO))
        setvbuf(stdin, NULL, _IONBF, 0);
    if (isatty(STDOUT_FILENO))
        setvbuf(stdout, NULL, _IONBF, 0);
    if (isatty(STDERR_FILENO))
        setvbuf(stderr, NULL, _IONBF, 0);
 
    
    /* Let BREAK cause a SIGINT in input. */
    terminal_settings.c_iflag &= ~IGNBRK;
    terminal_settings.c_iflag |=  BRKINT;
 
    /* Ignore framing and parity errors in input. */
    terminal_settings.c_iflag |=  IGNPAR;
    terminal_settings.c_iflag &= ~PARMRK;
 
    /* Do not strip eighth bit on input. */
    terminal_settings.c_iflag &= ~ISTRIP;
 
    /* Do not do newline translation on input. */
    //terminal_settings.c_iflag &= ~(INLCR | IGNCR | ICRNL);
	// Is For Ignore Enter Key 
	terminal_settings.c_iflag &= ~(INLCR | IGNCR );

 
    /* Use 8-bit characters. This too may affect standard streams,
     * but any sane C library can deal with 8-bit characters. */
    terminal_settings.c_cflag &= ~CSIZE;
    terminal_settings.c_cflag |=  CS8;
 
    /* Enable receiver. */
    terminal_settings.c_cflag |=  CREAD;
 
    /* Let INTR/QUIT/SUSP/DSUSP generate the corresponding signals. */
    terminal_settings.c_lflag |=  ISIG;
 
    /* Enable noncanonical mode.
     * This is the most important bit, as it disables line buffering etc. */
    terminal_settings.c_lflag &= ~ICANON;
 
    /* Disable echoing input characters. */
    terminal_settings.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
 
    /* Disable implementation-defined input processing. */
    terminal_settings.c_lflag &= ~IEXTEN;
 
    /* To maintain best compatibility with normal behaviour of terminals,
     * we set TIME=0 and MAX=1 in noncanonical mode. This means that
     * read() will block until at least one byte is available. */
    terminal_settings.c_cc[VTIME] = 0;
    terminal_settings.c_cc[VMIN] = 1;
 
    /* Set the new terminal settings.
     * Note that we don't actually check which ones were successfully
     * set and which not, because there isn't much we can do about it. */
    tcsetattr(terminal_descriptor, TCSANOW, &terminal_settings);
 
    /* Done. */
    return errno = 0;
}



// Flag 0 for Getting Index
// Flag 1 for Setting Index
int SetStartingIndexofFileItem =0 ;
int GetSetCurrentIndexOfFileList(int flag,int _I)
{
	if(!fileList.empty())
	{
		// Set the Starting INdex
		if(flag==1)
		{
			SetStartingIndexofFileItem = _I;
		}
	}
	return SetStartingIndexofFileItem;
}

int SetLastIndexofFileItem =0 ;
int GetSetLastIndexOfFileList(int flag,int _I)
{
	if(!fileList.empty())
	{
		// Set the Starting INdex
		if(flag==1)
		{
			SetLastIndexofFileItem = fileList.size();
		}
	}
	return SetLastIndexofFileItem;
}

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

void ResetCursorToDataRow()
{
	currentRC.ws_row=STARTTINGROWDATA;
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
		ClearScreen();
		SetCursor(0,0);
		int mid = size.ws_col/2;
		//string name = "File Explorer";
		SetCursor(1,(int)(mid-(title.length()/2)));
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

void SetParentPath(char * currDir)
{
	if(currDir!=NULL)
	{
		string s1;
		s1.append(currDir);
		int location = s1.find_last_of("\\/");
		if(location>0)
		{
		s1 = s1.substr(0, location);
		s1.append("/");
		char *cstr = new char[s1.length() + 1];
		strcpy(cstr, s1.c_str());
		UpFolderPath = cstr;
		}
	}
}

void printAtLast(string message,int restore)
{

	if(restore)
	{
		int rowI = currentRC.ws_row;
		int colI = currentRC.ws_col;
		SetCursor(size.ws_row-2,1);
		cout << "																				";
		SetCursor(size.ws_row-2,1);
		cout << message;
		SetCursor(rowI,colI);
		return;
	}
	else
	{
		SetCursor(size.ws_row-2,1);
		cout << "																				";
		SetCursor(size.ws_row-2,1);
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

int isFile(struct dirent *deptr)
{
	struct stat filestat;
	int isFile = 0;
	stat(deptr->d_name, &filestat);
	if(S_ISREG(filestat.st_mode))
			isFile = 1;
	return isFile;

}

void GetCurrentDirectoryDetails(char * currDir,int startIndex,int endingIndex,int isFromSameSource)
{
	if(currDir!=NULL)
	{
		SetParentPath(currDir);
		DIR *dp = opendir((const char*)currDir);
		struct dirent *deptr = NULL;
//		struct stat filestat;
		if(dp == NULL){
			printAtLast("Error: Could not open the current working directory",1);
			ResetCursor();
		}
		else
		{
			if(isFromSameSource == 0)
			{
			fileList.clear();
			//struct entry *temp = (struct entry *)malloc(sizeof(struct entry *));
			while((deptr = readdir(dp)) != NULL){
				//char * name = deptr->d_name;
				//string strTemp; strTemp.append(name);
				//if(strTemp.length()==1 && strTemp[0]=='.') continue;
				//if(strTemp.length()==2 && strTemp == "..")
				fileList.push_back(deptr);
			}
		}
		showCurrentDirectoryDetails(startIndex,endingIndex,isFromSameSource);
		}
	}
}

void showCurrentDirectoryDetails(int startingIndex,int endingIndex,int isFromSameSource){

	/*
	i.	File Name
	ii. File size (Human readable format similar to ls -lh)
	iii. Ownership (User & Group) & Permissions
	iv. Last modified
	*/
	string s ="";
	// Make Header
	currentRC.ws_col = 3;
//	int positionRow = currentRC.ws_row, positionColumn = currentRC.ws_col;
	SetCursor(currentRC.ws_row,currentRC.ws_col);
	s = "File Name"; s.append(38 - s.length(), ' '); cout << s ;
	s = "File Size"; s.append(20 - s.length(), ' '); cout << s ;
	s = "OwnerShip"; s.append(20 - s.length(), ' '); cout << s ;
	s = "Last Modified"; s.append(20 - s.length(), ' '); cout << s << endl;
	cout << endl;
	currentRC.ws_row = currentRC.ws_row + 2;
	struct dirent *deptr = NULL;
	string strHeader = "File Name\t\tFile Size\tOwnerShip\tLast Modified\n";
	struct stat filestat;

	// limit the End Index to File Size
	if(endingIndex>=((int)fileList.size()))
	{
		endingIndex = fileList.size();
		endingIndex = endingIndex-1;
	}

	GetSetCurrentIndexOfFileList(1,startingIndex);
	GetSetLastIndexOfFileList(1,endingIndex);
	for(int i=startingIndex;i <= endingIndex;i++)
	{
			
		//int isFile = 0,
		int isDirectory=0;
		deptr = fileList[i];
		stat(deptr->d_name, &filestat);
		SetCursor(currentRC.ws_row,1);
		//if(S_ISREG(filestat.st_mode))
		//	isFile = 1;

		if(deptr->d_type == DT_DIR)
		{
			isDirectory = 1;
		}

		if(isDirectory)
			cout << ">";

		SetCursor(currentRC.ws_row,currentRC.ws_col);
		cout << deptr->d_name;

		string ownerdetails = "";
		ownerdetails += ( isDirectory  ? "d":"-");
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

		SetCursor(currentRC.ws_row,40); cout << fileSize;
		SetCursor(currentRC.ws_row,60); cout <<  ownerdetails;
		SetCursor(currentRC.ws_row,80); cout << ctime(&filestat.st_mtime) << endl;
		currentRC.ws_row++;
	}
	
	if(isFromSameSource==0)
	{
			//writeToFile(" Cursor is Reset because found Scrolling is Not Source");
			ResetCursorToDataRow();
			//currentRC.ws_row = currentRC.ws_row+2 ;
	}
	else
	{
		currentRC.ws_row--;
		SetCursor(currentRC.ws_row,currentRC.ws_col);
		//writeToFile(" Cursor is Not Reset because found Scrolling is Source");
	}
	
}

void redraw(int mainCalling,int startIndex,int endingIndex,int isFromSameSource)
{
	if(mainCalling)
		ClearScreen();
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);


	if(!navigationVector.empty())
	{
		currDir = navigationVector.back();
	}

	SetWelcomeScreen();
	
	GetCurrentDirectoryDetails(currDir,startIndex,endingIndex,isFromSameSource);
	
	if(isFromSameSource==0)
	{
		ResetCursor();
		currentRC.ws_row = currentRC.ws_row+2 ;
		SetCursor(currentRC.ws_row,currentRC.ws_col);
	}
	if(isNormalMode)
		CursorFunctionality();
}

int main()
{
	// 1,0,32-7+1,0
	redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
	return 0;
}

void CursorFunctionality()
{
	
	//struct termios oldattr, newattr;
	int ch = 0;
    //set terminal
/*    tcgetattr( STDIN, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN, TCSANOW, &newattr );
    setbuf(stdin, NULL);
*/
	terminal_init();
	int cursorright =COLUMNWIDTH,cursorleft =1;
	//NormalMode :
	//ch = getchar();
	string str(1,ch);
	//NormalModeStart : 
	while ( (((ch = getchar()) != 'q') && (ch!='Q')) && isNormalMode == 1)
	{
		int rowI = currentRC.ws_row;
		int colI = currentRC.ws_col;
		if ( ch == BACKSPACE)
		{
			GoToUpFolder();
		}
		if ( ch == ARROWUP)
		{
			int currentPositionOfFileIndex = GetSetCurrentIndexOfFileList(0,0);
			if((currentRC.ws_row-1)<STARTTINGROWDATA)
			{
				//printAtLast("Scrooling Need to Be Impleented for Up ",1);

				if(currentPositionOfFileIndex>0)
				{
					//writeToFile("Scrooling Need to Be Impleented for Down"+
					//to_string(currentPositionOfFileIndex));
					int startIndex = currentPositionOfFileIndex-1;
					int endingIndex = startIndex + (MAXIMUMROWNUMBER-STARTTINGROWDATA+1);
					//currentRC.ws_row--;
					//writeToFile("File Up Scrolling " + to_string(startIndex)+" : "+to_string(endingIndex));
					redraw(1,startIndex,endingIndex,1);
					//writeToFile("POSt Redraw File Up Scrolling " + to_string(startIndex)+" : "+to_string(endingIndex));

				}
				//else
				//{
				//	writeToFile("Scrooling Need Not to be Done Because Current File Index is 0");
				//}
			}
			else
			{
				printf ( "\033[A");//cursor up
				currentRC.ws_row--;
			}
        }
        if ( ch == ARROWRIGHT) {
            printf ( "\033[C");//cursor right
			cursorleft++;
			cursorright--;
			//writeToFile(" Index of Array RIGHT ARROW " + 
			//to_string(rowI-STARTTINGROWDATA+GetSetCurrentIndexOfFileList(0,0)-1));
			GoToNextDirectory(rowI-STARTTINGROWDATA+GetSetCurrentIndexOfFileList(0,0));
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
			if(currentRC.ws_row>MAXIMUMROWNUMBER)
			{
				//currentRC.ws_row++;
				//writeToFile("Scrooling Need to Be Impleented for Down");
				int currentPositionOfFileIndex = GetSetCurrentIndexOfFileList(0,0);
				writeToFile("Scrooling Need to Be Impleented for Down row : "+to_string(currentRC.ws_row)+" : current " +
				to_string(currentPositionOfFileIndex));
				int startIndex = currentRC.ws_row-MAXIMUMROWNUMBER+currentPositionOfFileIndex;
				int endingIndex = startIndex + (MAXIMUMROWNUMBER-STARTTINGROWDATA+1);
				int lastIndexOfFile = GetSetLastIndexOfFileList(0,0);
				writeToFile("File " + to_string(startIndex)+" : "+to_string(endingIndex)
					+" Last Index Of File  "+to_string(lastIndexOfFile)
					+ " Index of Array " + to_string(rowI-STARTTINGROWDATA+startIndex-1));
				//currentRC.ws_row--;
				/*if(endingIndex>=lastIndexOfFile)
				{
					writeToFile("No Scrolling is Needed because max reached");
					writeToFile("File " + to_string(startIndex)+" : "+to_string(endingIndex)
					+" Last Index Of File  "+to_string(lastIndexOfFile));
				}
				else*/
				if(!(endingIndex>=lastIndexOfFile))
				{
					//currentRC.ws_row++;
					//writeToFile("File " + to_string(startIndex)+" : "+to_string(endingIndex)
					//+" Last Index Of File  "+to_string(lastIndexOfFile));
					redraw(1,startIndex,endingIndex,1);
				}
			}
			else
			{
				printf ( "\033[B");//cursor down
				currentRC.ws_row++;
			}
        }
		if ( ch == '\n') {
			//printAtLast(to_string(currentRC.ws_row) + " NM : " + to_string(currentRC.ws_col),1);
			//GoToNextDirectory(rowI-STARTTINGROWDATA);
			//currentRC.ws_col = cursorleft;
			SetCursor(rowI,colI);
			printAtLast(to_string(currentRC.ws_row) + " NM : " + to_string(currentRC.ws_col)
			+ " Index of Array " + to_string(rowI-STARTTINGROWDATA) + " file List Size() "+
			to_string(fileList.size()),1);
		}
		if ( ch == ':') {
			isNormalMode = 0;
			terminal_done();
			GetCommandLine("");
		}
		if ( ch == 'h' || ch == 'H') {
			currDir = startingDirectory;
			navigationVector.push_back(currDir);
			redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
		}
	}

	if((ch == 'q' || ch == 'Q') && isNormalMode == 1)
	{
		//	writeToFile("Going In Normal Mode Quit");
			printf ( "\033[2J");
			//tcsetattr( STDIN, TCSANOW, &oldattr );
			terminal_done();
			//return;
			exit(0);
    }
	// Command Mode Region
	//ch = getchar();

/*	string strI;
	//cin >> sInput;
	//scanf("%s",sInput);
	//cout.flush();
	//cout << ":" ;
	//cout.flush();
	//fflush(stdin);
	getline (cin, strI);
	//cout.flush();
	
	//cin.ignore(); 
	//getline(cin,sInput);
	//printAtLast(strI,1);
	//ch = strI[0];
    //cout << strI ;

    char s[350];

    if (!cin.fail()) {
      cout << "Enter some text: ";
      cin.getline(s, 11);
      cout << "You entered '"<< s << "'" << endl;  
    }
	strI = string(s);
	writeToFile(strI);
	writeToFile("testing &&&&&&&&&&& String");
	do
	{
		if(ch == ESC)
		{
			writeToFile("Going to ESC via terminal Mode ");
			struct termios GotToNormalMode;
			 GotToNormalMode = oldattr;
				GotToNormalMode.c_lflag &= ~( ICANON | ECHO );
		    tcsetattr( STDIN, TCSANOW, &GotToNormalMode );
			isNormalMode=1;
			ResetCursorToDataRow();
			goto NormalMode;
		}
		if ( ch == '\n') {
			//printAtLast(to_string(currentRC.ws_row) + " CM : " + to_string(currentRC.ws_col),1);
			GetCommandLine(strI);
		}
		//writeToFile(strI.append("  -- testing &&&&&&&&&&& String"));
	}while (((ch = getchar()) != 'q' && (ch != 'Q'))  && isNormalMode == 0);
	*/

	if((ch == 'q' || ch == 'Q') && isNormalMode == 0)
	{
		//writeToFile("Going In Command Mode Quit");
		printf ( "\033[2J");
		//tcsetattr( STDIN, TCSANOW, &oldattr );
		terminal_done();
		//exit(0);
		return;
	}


	//tcsetattr( STDIN, TCSANOW, &oldattr );
	terminal_done();
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
//		char * currentDirectory = navigationVector.back();
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
		if(isFile(deptr))
		{
			printAtLast("It is File So need to Open in Application",1);

			

			char filePath[512];
			strcpy(filePath,"xdg-open ");
			strcat(filePath,deptr->d_name);
			system(filePath);
			//system("xdg-open http://www.google.com"); 
			writeToFile(string(filePath));
/*
			char arr[100];
			strcpy(arr,"xdg-open ");
			strcat(arr,av[1]);
			system(arr);
*/
		}
		else
		{
			string sname(deptr->d_name);
			string snamedummy = string(deptr->d_name);
			if(snamedummy!=".")
			{
				if(snamedummy=="..")
				{
					snamedummy = string(currDir);
					size_t found = snamedummy.find_last_of("//");
					snamedummy = snamedummy.substr(0,found);
					char *cstr = new char[snamedummy.length() + 1];
					strcpy(cstr, snamedummy.c_str());
					snamedummy = string(cstr);	
					navigationVector.push_back(cstr);
					redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);		
				}
				else
				{
					stat(deptr->d_name, &filestat);
					char * p;
					p = (char *)malloc(strlen(deptr->d_name) + strlen(currDir) + 1 + 1);
					strcpy(p,currDir);
					strcat(p,"/");
					strcat(p,deptr->d_name);
					navigationVector.push_back(p);
					redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
					free(p);
				}
			}
			else
			{
				printAtLast("You are in Current Directory",1);
			}
		}
	}
}



void GoToPreviousDirectory()
{
	char * prevDir = GetPreviousDirectoryPath();
	if(prevDir!=NULL)
	{
		redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
	}
	else
		printAtLast("No Previous Directory Exist",1);
}

void GoToUpFolder()
{
	char * upFolderPath = UpFolderPath;
	if(upFolderPath!=NULL)
	{
		navigationVector.push_back(upFolderPath);
		redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
	}
	else
		printAtLast("No Up Folder Path Defined",1);
}

int EnableUpScrolling()
{
	if(isNormalMode)
	{
		if(currentRC.ws_row>MAXIMUMROWNUMBER)
		{
			return 1;
		}
	}
	return 0;
}

int EnableDownScrolling()
{
	if(isNormalMode)
	{
		if(currentRC.ws_row<STARTTINGROWDATA)
		{
			return 1;
		}
	}
	return 0;
}

int invokeCommands(string strCommand)
{

	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
     		 perror("getcwd() error");
        else
      		printf("current working directory is: %s\n", cwd);

	int i=executeCommands(strCommand,currDir);
	// To Clear If Anyhting Written 
	printAtLast("",1);
	if(i==0)
		writeToFile("SucessRename");
	else if(i==-11)
		printAtLast("Problem in Parameters",1);
	else if(i==110)
		{
			writeToFile("Redarwing on Success ");
			isNormalMode = 1;
			redraw(1,0,MAXIMUMROWNUMBER-STARTTINGROWDATA+1,0);
			isNormalMode = 0;
		}
		else if(i==-12)
		{
			writeToFile("Invalid Command Name");
			//printAtLast("Invalid Command",1);
			//SetCursor(currentRC.ws_row-3,55);
		}

	return 0;
}

/*
int executeCommands(string strCommand){
    
    vector<string> seperatedSpaceCommands;
	split(strCommand,seperatedSpaceCommands,' ');
	for(int i=0;i<seperatedSpaceCommands.size();i++)
	{
		writeToFile(seperatedSpaceCommands[i]);
	}
    exeRenameCommand(NULL,NULL);
    return 0;
}
*/

void GetCommandLine(string str)
{
	
			writeToFile("to_string(ch)");
			SetCursor(size.ws_row-2,2);
			bool isEscPressed = false,isEnterPressed = false;			
			do {
				int columnNumber =2;
			SetCursor(size.ws_row-2,columnNumber);
			printf(":");	
			columnNumber++;
			//SetCursor(size.ws_row-3,2);
			string str = "";
  			char ch;
  			while ((ch = getchar())) {
				 //writeToFile(to_string(ch));
				if(ch == 10)
				{
					isEnterPressed = true;
					break;
				}
				else if(ch == 27)
			  	{
					isEscPressed = true;
				/*	columnNumber -= 2;
					SetCursor(size.ws_row-3,columnNumber);
					printf(" "); printf(" ");	
				*/	
					break;
				}
			/*	else if( ch == ARROWLEFT || ch == ARROWRIGHT || ch == ARROWDOWN || ch == ARROWUP )
				{
					writeToFile(" L R U D 7 ");
					columnNumber -= 3;
					SetCursor(size.ws_row-3,columnNumber);
					printf(" "); printf(" "); printf(" ");		
				}
				*/
				else if(isalpha(ch) || isblank(ch) || isdigit(ch) || ch == '_' || ch == '.' || ch == '/')
				{
					str += ch;
					columnNumber++;
			  	}
  			}
			  if(isEscPressed == true)
			  {
				isNormalMode = 1;
				//tcsetattr( STDIN, TCSANOW, &newattr);
				terminal_init();
			  	break;
			  }
			  if(isEnterPressed == true)
			  {
				SetCursor(size.ws_row-2,2);
				for(int i=0;i<size.ws_col;i++){ printf(" "); }
				SetCursor(size.ws_row-2,2);				
			  }
			  writeToFile(str);

			  invokeCommands(str);

			}while(true);
}


