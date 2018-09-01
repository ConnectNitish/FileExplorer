#include "Delete.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define ERRORDIRNOTEXIST -110
#define ERRORWHILEDELETING -111
#define ERRORPERMISSIONWHILEDELETING -112
#define ERRORPATHNOTEXIST -113
#define SUCCESS 100

int removesingleFile(const char path[]);

#include<string>
#include<bits/stdc++.h>
using namespace std;

/*
char * GetCharPointerFromString_TOREMOVEDEL(string param1)
{
    char * _CPointer = new char[param1.length() + 1];
    strcpy(_CPointer, param1.c_str());
    return _CPointer;
}

char[] GetCharArrayFromString(string param1)
{
    int length_string = param1.length(); 
    char paramCharArray[length_string+1]; 
    strcpy(paramCharArray, param1.c_str()); 
    return paramCharArray;
}

ofstream myfilehdel;
void writeLogsDelete_TOREMOVE(string message)
{
  	myfilehdel.open("logs.txt", std::ios_base::app);
  	string strFinalLog;
  	myfilehdel << " Delete File :" << message << endl;
  	myfilehdel.close();
}
writeLogsDelete_TOREMOVE(string(path));

int xdiraccessCheck_TOREMOVE(const char *path)
{
	static DIR *dirp;
	dirp = opendir(path);
	if (dirp == NULL) {
		return 0;
	}
	closedir(dirp);
	return 1;
}
*/

int removesingleFile(const char path[] ){
	//char *full_path; 
	if (unlink(path) == 0)
    {
            //printf("Removed a file: %s\n", path);
        return SUCCESS;
    }
    else
    {
           // printf("Can`t remove a file: %s\n", path);
        return ERRORWHILEDELETING;
    }
}

int permitToDelete  = 1;

int removedirectory(const char path[])
{

    //if(permitToDelete==0) return ERRORPERMISSIONWHILEDELETING; 

    size_t pathlength;
    
    DIR *directory;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    stat(path, &stat_path);

    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0) {
        //fprintf(stderr, "%s: %s\n", "Is not directory", path);
	    removesingleFile(path);
        //exit(-1);
        return SUCCESS;    
    }

    // if not possible to read the directory for this user
    if ((directory = opendir(path)) == NULL) {
        //fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
        //exit(-1);
        return ERRORPERMISSIONWHILEDELETING;
    }
    char *actual_path;
    pathlength = strlen(path);

    while ((entry = readdir(directory)) != NULL) {

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        actual_path = (char *)calloc(pathlength + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(actual_path, path);
        strcat(actual_path, "/");
        strcat(actual_path, entry->d_name);

        stat(actual_path, &stat_entry);

/*
        if(!xdiraccessCheck(actual_path))
        {
            permitToDelete = 0;
            return ERRORPERMISSIONWHILEDELETING;
        }
*/
        // recursively remove a nested directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            removedirectory(actual_path);
            continue;
        }

        // remove a file object
        if (unlink(actual_path) == 0)
        {
            //printf("Removed a file: %s\n", actual_path);
            return SUCCESS;
        }
        else
        {
            //printf("Can`t remove a file: %s\n", actual_path);
            return ERRORWHILEDELETING;
        }
    }

    if (rmdir(path) == 0)
    {
        //printf("Removed a directory: %s\n", path);
         return SUCCESS;
    }
    else
    {
        //printf("Can`t remove a directory: %s\n", path);
         return ERRORWHILEDELETING;
    }
    permitToDelete = 1;
    closedir(directory);
    return SUCCESS;
}

/*
int main(const int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Missing single operand: path\n");
        return -1;
    }

    removedirectory(argv[1]);

    return 0;
}

*/