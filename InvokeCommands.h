#ifndef INVOKECOMMANDS_H
#define INVOKECOMMANDS_H

#include "RenameFile.h"
#include "MakeLogs.h"
#include "Delete.h"
#include "Copy.h"
#include "SnapShot.h"
#include "MoveFile.h"

//#include "CreateNewFile.h"

#define SPACEDELIMETER ' ' 
#define ERRORWHILEPROCESSING -9
#define RIGHTISSUES -10
#define PARAMISSUES -11
#define NOTPREDEFINEDCOMMAND -12
#define SUCCESSANDWORKINGINCURRENTDIR 110



#include<string>
#include<vector>

#include <sys/ioctl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <bits/stdc++.h>

using namespace std;

vector<string> _CMDContent;
int exeRenameCommand();
size_t split(const string &txt, vector<string> &strs, char ch);
int createFile(char* fileName,char* directory);
int exeCreateFileCommand();
int exeCreateDirectoryCommand();
int xdiraccess1(const char *path);
int exeDeleteDirectoryCommand();
int exeCopyCommand(char * currDir);
int exeSnapShotCommand(char * currDir);
int exeMoveFileCommand(char * currDir);



//-11 While Taking Params 
//-10 Rights Issues
//-9 Error While Processing  
//110 Inidicating Working In Currently Working Directory
char * GetCharPointerFromString(string param1)
{
    char * _CPointer = new char[param1.length() + 1];
    strcpy(_CPointer, param1.c_str());
    return _CPointer;
}

int execCommand(string commandType,char * currDir)
{
    int Status=0;
    if (commandType == "rename")
    {
        Status = exeRenameCommand();
    } 
    else if (commandType == "create_file")
    {
        Status = exeCreateFileCommand();
    } 
    else if (commandType == "create_dir")
    {
        Status = exeCreateDirectoryCommand();
    }
    else if (commandType == "delete_file" || commandType == "delete_dir")
    {
        Status = exeDeleteDirectoryCommand();
    } 
    else if (commandType == "copy")
    {
        Status = exeCopyCommand(currDir);
    }
    else if (commandType == "snapshot")
    {
        Status = exeSnapShotCommand(currDir);
    } 
    else if (commandType == "move")
    {
        Status = exeMoveFileCommand(currDir);
    } 
    else
    {
        Status = NOTPREDEFINEDCOMMAND;
    }
    return Status;
}

int executeCommands(string strCommand,char * currDir)
{    
    string commandType;    
    _CMDContent.clear();
	split(strCommand,_CMDContent,SPACEDELIMETER);
    commandType = _CMDContent[0];  
    return execCommand(commandType,currDir);
    //return 0;
}

int exeRenameCommand()
{
    string param1 = _CMDContent[1];
    string param2 = _CMDContent[2];
    if(param1 == "")
        return -11;
    if(param2 == "")
        return -11;

    char *old = GetCharPointerFromString(param1);
    char *newnm = GetCharPointerFromString(param2);

    writeLogs(param1);
    writeLogs(param2);
	renameFiles(old,newnm);
    return 0;
}

int exeCreateFileCommand()
{
    string strDirectory = _CMDContent[_CMDContent.size()-1];

    if(strDirectory=="")
        return PARAMISSUES;
    
    char * directory = GetCharPointerFromString(strDirectory);
    int maxLimit = ((int)_CMDContent.size());
    for(int i=1;i<maxLimit-1;i++)
    {
        createFile(GetCharPointerFromString(_CMDContent[i]),directory);
    }
    return 0;
}




int CreateDirectory(const char * dirname,const char * path){
	
    if(!xdiraccess1(path))
        return RIGHTISSUES;

	else if(!strcmp(path,".")){
		if (mkdir(dirname,0777) == -1) {
        	return ERRORWHILEPROCESSING;
   	}
       else
       {
           return SUCCESSANDWORKINGINCURRENTDIR;
       }
	}
	else{
		//int chdirval= 
        chdir(path);
		if (mkdir(dirname,0777) == -1) {
        	return ERRORWHILEPROCESSING;
	}
	}
    return 0;
}
   

int exeCreateDirectoryCommand()
{
    string strDirectory = _CMDContent[_CMDContent.size()-1];

    if(strDirectory=="")
        return PARAMISSUES;
    
    for(int i=0;i<(int)_CMDContent.size();i++)
        writeLogs(_CMDContent[i]);

    char * directory = GetCharPointerFromString(strDirectory);
    int success;
    for(int i=1;i<(int)_CMDContent.size()-1;i++)
    {
        success = CreateDirectory(GetCharPointerFromString(_CMDContent[i]),directory);
    }
    return success;
}

int xdiraccess1(const char *path)
{
	static DIR *dirp;
	dirp = opendir(path);
	if (dirp == NULL) {
		return 0;
	}
	closedir(dirp);
	return 1;
}

int createFile(char* filename,char* directory)
{
   // char* pathFile=NULL;
   // string strDir = string(directory);
   // string strFile = string(filename);
    
    if(!xdiraccess1(directory))
        return RIGHTISSUES;

    // If not Presnt Direcotry 
    if(strcmp(directory,"."))
    {
        // Change the Directory 
        chdir(directory);
    }
    int fd_for_w = open(filename, O_RDWR | O_CREAT , 0700);
    int close_id = close (fd_for_w);
		if(close_id < 0)
        {
			return ERRORWHILEPROCESSING;
		}
    return 0;
}



size_t split(const string &txt, vector<string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
		string strTemp = txt.substr( initialPos, pos - initialPos );
		if(strTemp!="")
        	strs.push_back(strTemp);
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

int exeDeleteDirectoryCommand()
{
    string param1 = _CMDContent[1];
    int length_string = param1.length(); 
    char paramCharArray[length_string+1]; 
    strcpy(paramCharArray, param1.c_str()); 
     
    writeLogs("Calling Delete Dir ");
    int returnStatus = removedirectory(paramCharArray);
    writeLogs(" Return Status From  "+to_string(returnStatus));
    
    return returnStatus;
}

int exeCopyCommand(char * currDir)
{
    char *arrayOfParameters[100];
    for(int i=0;i<(int)_CMDContent.size();i++)
    {
        char * params = GetCharPointerFromString(_CMDContent[i]);
        //printf("SSSSSSSSSSSSSS %s /n",params);
        arrayOfParameters[i] = params;
    }

    int sucess = executeCopyCommand((int)_CMDContent.size(),arrayOfParameters,currDir);

   return 0;
}

int exeSnapShotCommand(char * currDir)
{
    writeLogs("EXECUTING SNAPSHOT");

    string strParam1 = _CMDContent[1];
    string strParam2 = _CMDContent[2];
    const char * chrParam1 = GetCharPointerFromString(strParam1);
    const char * chrParam2 = GetCharPointerFromString(strParam2); 
    int sucess = runSnapShot(chrParam1,chrParam2,currDir);

   return 0;
}

int exeMoveFileCommand(char * currDir)
{
    char *arrayOfParameters[100];
    for(int i=0;i<(int)_CMDContent.size();i++)
    {
        char * params = GetCharPointerFromString(_CMDContent[i]);
        arrayOfParameters[i] = params;
    }

    int returnValue = executeMoveCommand((int)_CMDContent.size(),arrayOfParameters,currDir);
    return 0;
}



#endif