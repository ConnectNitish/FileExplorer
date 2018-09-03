h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include	<grp.h>
#include        <string.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include        <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include<string>
#include<bits/stdc++.h>

using namespace std;

#include "Copy.h"

int SearchDirectory( char *name , char tardir[] ,char *currentWorkingDir);
int MultipleSearchDirectory(char *spath , char *tardir ,char *currentWorkingDir);
int functionforCopy( char *filename, char *dirname , char * to_append,char *currentWorkingDir);
int functionforCopyforMultipleFile( char *filename, char *dirname , char * to_append,char *currentWorkingDir);


char * GetCharPointerFromString_TOREMOVEDEL(string param1)
{
    char * _CPointer = new char[param1.length() + 1];
    strcpy(_CPointer, param1.c_str());
    return _CPointer;
}

ofstream myfilehdel;
void writeLogsDelete_TOREMOVE(string message)
{
  	myfilehdel.open("logs.txt", std::ios_base::app);
  	string strFinalLog;
  	myfilehdel << " Delete File :" << message << endl;
  	myfilehdel.close();
}
//writeLogsDelete_TOREMOVE(string(path));

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
/**/
/*
int main(int ac, char *av[])
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
     		 perror("getcwd() error");
        else
      		printf("current working directory is: %s\n", cwd);
	char tPath[256];char spath[256];
	if ( ac == 3){
		strcpy(tPath,"/");
		strcat(tPath,av[2]);
		strcat(spath,av[1]);
    //		printf("spath is %s \ntpath is = %s\n",spath,tPath);
		SearchDirectory( spath, tPath,cwd);
	}
	else if(ac>3){	
		for(int i=1;i<ac-1;i++){
			strcpy(tPath,"/");
			strcat(tPath,av[ac-1]);
			strcat(spath,av[i]);
			//printf("spath is %s \ntpath is = %s\n",spath,tPath);
			SearchDirectory1( spath, tPath,cwd);
			strcpy(tPath,"\0");
			strcpy(spath,"\0");
		}
	}
	return 0;
}
*/

int preparePathAndExecuteCopy(int size,vector<char *> params, char * currDir)
{

    /*writeLogsDelete_TOREMOVE("Curent ");
    //writeLogsDelete_TOREMOVE(currDir);

    //writeLogsDelete_TOREMOVE("Start");
    for(int i=0;i<params.size();i++)
        writeLogsDelete_TOREMOVE(params[i]);

    writeLogsDelete_TOREMOVE("END");
    */
    int chdirVal = chdir(currDir);
    //writeLogsDelete_TOREMOVE("before copying going to cur directory "+to_string(chdirVal));

    char cwd[256],cwd2[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
     		 perror("getcwd() error");
        //else
      	//	printf("current working directory is: %s\n", cwd);
    strcpy(cwd2,cwd);

	char tPath[256];char spath[256];

	if ( size == 3){
        strcpy(tPath,"/");
        //./T2
        
        #pragma region COPYCODE

      //  writeLogsDelete_TOREMOVE(" Start TPAATH %%%% ");
     
        

        int lengthofString = strlen(params[2]);
     //    writeLogsDelete_TOREMOVE(" 1 "+to_string(lengthofString));
        char *temp = params[2];
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
     //    writeLogsDelete_TOREMOVE(" 2 ");
     //    writeLogsDelete_TOREMOVE(paramValue);
        //if(!strcmp(params[2],"./")){
        char c1=paramValue[0];
     //   writeLogsDelete_TOREMOVE(" 2.1 ");
     //    writeLogsDelete_TOREMOVE(string(1,c1));
        char c2=paramValue[1];
     //   writeLogsDelete_TOREMOVE(" 2.2 ");
     //   writeLogsDelete_TOREMOVE(string(1,c2));
      //  if(c1=='.')
     //   writeLogsDelete_TOREMOVE(" 2.11 hurray");
     //   else{
      //          writeLogsDelete_TOREMOVE(" 2.11 fuck u");
     //   }
        // writeLogsDelete_TOREMOVE(to_string(a));
        int tpath_length = strlen(tPath);
     //   writeLogsDelete_TOREMOVE("  tpath len b4 appending ./t2 "+to_string(tpath_length));
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
     //       writeLogsDelete_TOREMOVE(" 2.101 fuck u "+to_string(tpath_length));
            strcat(tPath,currDir);
     //       writeLogsDelete_TOREMOVE(" 2.121 fuck u curdur");
     //       writeLogsDelete_TOREMOVE(currDir);
     //        writeLogsDelete_TOREMOVE(tPath);
             tpath_length = strlen(tPath);
     //         writeLogsDelete_TOREMOVE("  tpath len after appending curdir "+to_string(tpath_length));
            for(int ii=1;ii<strlen(paramValue);ii++)
            {
     //           writeLogsDelete_TOREMOVE(" 2.141 fuck u "+to_string(ii));
                char c1=paramValue[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
     //            writeLogsDelete_TOREMOVE(" inside for ffor tpath");
     //           writeLogsDelete_TOREMOVE(tPath);
            }
            tPath[tpath_length]='\0';
            
     //        writeLogsDelete_TOREMOVE(" 3 ");

        }
		else{
            //strcpy(tPath,"/");
            strcat(tPath,params[2]);
     //        writeLogsDelete_TOREMOVE(" 4 ");
        }

     //   writeLogsDelete_TOREMOVE(" TPAATH %%%% ");
     //   writeLogsDelete_TOREMOVE(tPath);

#pragma endregion COPYCODE

        //strcat(tPath,"/home/nitish/IIITH/Semester 1/FileExplorer");
		strcpy(spath,params[1]);
        //strcpy(spath,"T16");
        strcat(spath,"\0");
        
    //		printf("spath is %s \ntpath is = %s\n",spath,tPath);
		SearchDirectory( spath, tPath,cwd);
	}
	else if(size>3){	
		for(int i=1;i<size-1;i++){
			//strcpy(tPath,"/");
			//strcat(tPath,params[size-1]);
            strcpy(tPath,"/");
		    /////////////////////////////////////////////////
            //strcat(tPath,params[size-1]);
            ////////////////////////////////////////////////

 #pragma region COPYCODENEW

     //   writeLogsDelete_TOREMOVE(" Start TPAATH %%%% ");
     
        

        int lengthofString = strlen(params[size-1]);
     //    writeLogsDelete_TOREMOVE(" 1 "+to_string(lengthofString));
        char *temp = params[size-1];
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
    //    writeLogsDelete_TOREMOVE(" 2 ");
    //     writeLogsDelete_TOREMOVE(paramValue);
        //if(!strcmp(params[2],"./")){
        char c1=paramValue[0];
     //   writeLogsDelete_TOREMOVE(" 2.1 ");
     //    writeLogsDelete_TOREMOVE(string(1,c1));
        char c2=paramValue[1];
     //   writeLogsDelete_TOREMOVE(" 2.2 ");
     //   writeLogsDelete_TOREMOVE(string(1,c2));
     //   if(c1=='.')
     //   writeLogsDelete_TOREMOVE(" 2.11 hurray");
     //   else{
     //           writeLogsDelete_TOREMOVE(" 2.11 fuck u");
    //    }
        // writeLogsDelete_TOREMOVE(to_string(a));
        int tpath_length = strlen(tPath);
     //   writeLogsDelete_TOREMOVE("  tpath len b4 appending ./t2 "+to_string(tpath_length));
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
     //       writeLogsDelete_TOREMOVE(" 2.101 fuck u "+to_string(tpath_length));
            strcat(tPath,currDir);
     //       writeLogsDelete_TOREMOVE(" 2.121 fuck u curdur");
     //       writeLogsDelete_TOREMOVE(currDir);
     //        writeLogsDelete_TOREMOVE(tPath);
             tpath_length = strlen(tPath);
     //         writeLogsDelete_TOREMOVE("  tpath len after appending curdir "+to_string(tpath_length));
            for(int ii=1;ii<strlen(paramValue);ii++){
      //          writeLogsDelete_TOREMOVE(" 2.141 fuck u "+to_string(ii));
                char c1=paramValue[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
     //            writeLogsDelete_TOREMOVE(" inside for ffor tpath");
      //          writeLogsDelete_TOREMOVE(tPath);
            }
            tPath[tpath_length]='\0';
            
      //       writeLogsDelete_TOREMOVE(" 3 ");

        }
		else{
            //strcpy(tPath,"/");
            strcat(tPath,params[2]);
      //       writeLogsDelete_TOREMOVE(" 4 ");
        }

      //  writeLogsDelete_TOREMOVE(" TPAATH %%%% ");
     //   writeLogsDelete_TOREMOVE(tPath);

#pragma endregion COPYCODENEW


            strcpy(spath,params[i]);
             strcat(spath,"\0");
			//strcat(spath,params[i]);
			//printf("spath is %s \ntpath is = %s\n",spath,tPath);
			MultipleSearchDirectory( spath, tPath,cwd);
			strcpy(tPath,"\0");
			strcpy(spath,"\0");
		}
	}
   // chdirVal = chdir(cwd2);
  //  writeLogsDelete_TOREMOVE("after copying going to cur directory ");

}


int SearchDirectory(char *spath , char *tardir ,char *currentWorkingDir) 
{
    /*writeLogsDelete_TOREMOVE("at start seasrch dir with spath as");
    writeLogsDelete_TOREMOVE(spath);
     writeLogsDelete_TOREMOVE("at start seasrch dir with tpath as");
    writeLogsDelete_TOREMOVE(tardir);
     writeLogsDelete_TOREMOVE("at start seasrch dir with cwd as");
    writeLogsDelete_TOREMOVE(currentWorkingDir);
    */DIR *dir = opendir(spath);  	              //Assuming absolute pathname here.
    if(dir) {
  /*      writeLogsDelete_TOREMOVE("considering aas a directory");
	printf("dir name here is %s \n",spath);
	printf("tardir name here is %s \n",tardir);*/
	char *dirname = (char *)tardir;
	int chdirval = chdir(dirname);
//	printf("chdir returned val is %d\n",chdirval);
//	printf("for chdir dirname name here is %s \n",dirname);
	if (mkdir(spath,0777) == -1) {
        	//perror(argv[0]);
        	//exit(EXIT_FAILURE);
		printf("failed to make dir\n");
		return -99;
   	}
	chdirval = chdir(currentWorkingDir);
//	printf("chdir returned val is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
     		 perror("getcwd() error");
			  return -99;
	}
//        else
//7
//    		printf("current working directory is: %s\n", cwd);
		
	/*strcat(tardir,"/"); 
	strcat(tardir,spath);
	printf("tardir name here is %s \n",tardir);*/
	char *tardir1;
	tardir1 = tardir;
//	printf("tardir11111 name here is %s \n",tardir1);
        char Path[256], *EndPtr = Path;
        struct dirent *e;
        strcpy(Path, spath); 
    	strcat(Path,"/");                 //Copies the current path to the 'Path' variable.
        EndPtr += strlen(Path);              //Moves the EndPtr to the ending position.
        while((e = readdir(dir)) != NULL  ) { 
		chdirval = chdir(currentWorkingDir);
//		printf("chdir returned val is %d\n",chdirval);	
//		printf("tardir inside while name here is %s \n",tardir);		 //Iterates through the entire directory.
           	struct stat info;                //Helps us know about stuff	
	  	if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
           		continue;
            	strcpy(EndPtr, e->d_name);       //Copies the current filename to the end of the path, overwriting it with each loop.
//		printf("e->d_name %s \n" ,e->d_name);
//		printf("path is %s\n",Path);
//		printf("stat val =%d\n",stat(Path, &info));
            	if(!stat(Path, &info)) 
				{         //stat returns zero on success.
                	if(S_ISDIR(info.st_mode) && e->d_name!="." && e->d_name!="..") 
					{  //Are we dealing with a directory?
                    	//Make corresponding directory in the target folder here.
//                    		printf("rec call with path =%s tardir =%s\n",Path,tardir);

						SearchDirectory(Path,tardir,currentWorkingDir);   //Calls this function AGAIN, this time with the sub-name.
                	} 
					else if(S_ISREG(info.st_mode)) 
					{ //Or did we find a regular file?
                    //Run Copy routine
//					printf("fname is = %s ",e->d_name);
					functionforCopy( e->d_name, tardir , Path,currentWorkingDir);
               		}
            	}
        }
    }
	else {
//        writeLogsDelete_TOREMOVE("considering aas a file");
//		printf("not a dir fname is = %s\n ", spath);
		functionforCopy( spath, tardir,NULL,currentWorkingDir);
	}
}




int functionforCopy( char *filename, char *dirname , char * to_append,char *currentWorkingDir){

	int chdirval;
	//printf("chdir returned val is at copy start is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
     		 perror("getcwd() error");
			  return -99;
	}
//        else
//      		printf("current working directory at copy start is: %s\n", cwd);
	int sz;
	//printf("%s\n",argv[1]);
//	printf("received fname is = %s \n",filename);
//	printf("tardir is %s\n",dirname);
//	printf("to append is %s\n",to_append);
	
	char actualDir[100];int i;char *actualTarget;
	if(to_append!=NULL){
		for( i=0;i<strlen(dirname);i++){
			actualDir[i]=dirname[i];
		}
		actualDir[i]='/';
		i++;
		for(int j=0;j<strlen(to_append);j++){
			actualDir[i]=to_append[j];i++;
		}
//		printf("appended is %s\n",actualDir);
		int j=strlen(actualDir)-1;
		do{
			actualDir[j]='\0';
			j--;
		}while(actualDir[j]!='/');
		actualDir[j]='\0';
//		printf("actual path after appended is %s\n",actualDir);
		actualTarget = (char *)actualDir;
	}
	/*read from file */
	//char *buf = (char *) calloc (100, sizeof(char));
	
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		//perror("getcwd() error");
		return -99;
	}
//        else
//      		printf("current working directory is: %s\n", cwd);
	int fd = open(filename, O_RDONLY);
//	printf("file descriptor fd = %d\n",fd);
	char buf[17];
	if(to_append!=NULL)
	 chdirval= chdir(actualTarget);
	else{
		chdirval= chdir(dirname);
	}
//	printf("chdir returned val is %d\n",chdirval);
	
	

	int fd_for_w = open(filename, O_RDWR | O_CREAT , 0777);
//	printf("file descriptor fd_for_w = %d\n",fd_for_w);
	if(fd_for_w == -1){
		//printf ("error number %d\n", errno);
		//perror("program");
		return -99;
	}
//	printf("file opened \n");
	int sz1;
	sz = read(fd,buf,16);
	while(sz>0){
	//printf("called read(%d ,buf,10 ) returned %d bytes were read\n",fd,sz);
	buf[16]='\0';
	//printf("%s ",buf);
	sz = read(fd,buf,16);
	sz1 = write(fd_for_w, buf,strlen(buf));
	}
	//file close
	/*file close*/
	int close_id = close (fd);
	if(close_id < 0){
//		printf("erroe while closeing file 1\n");
//		perror("c1");
//		exit(1);
		return -99;
	}
//	printf("file clsoed");
	close_id = close (fd_for_w);
	if(close_id < 0){
//		printf("erroe while closeing\n");
//		perror("c1");
//		exit(1);
		return -99;
	}
	
//	printf("file clsoed");
	//char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
 	{
		 //    		 perror("getcwd() error");
		return -99;
		}
//        else
//      		printf("current working directory is: %s\n", cwd);
}



int MultipleSearchDirectory(char *spath , char *tardir ,char *currentWorkingDir) {
    DIR *dir = opendir(spath);  	              //Assuming absolute pathname here.
    if(dir) {
//	printf("dir name here is %s \n",spath);
//	printf("tardir name here is %s \n",tardir);
	char *dirname = (char *)tardir;
	int chdirval = chdir(dirname);
//	printf("chdir returned val is %d\n",chdirval);
//	printf("for chdir dirname name here is %s \n",dirname);
	if (mkdir(spath,0777) == -1) {
        	//perror(argv[0]);
        	//exit(EXIT_FAILURE);
		//printf("failed to make dir\n");
		return -99;
   	}
	chdirval = chdir(currentWorkingDir);
//	printf("chdir returned val is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)		 
	{
		//perror("getcwd() error");
		return -999;
	}
//        else
//      		printf("current working directory is: %s\n", cwd);
		
	/*strcat(tardir,"/"); 
	strcat(tardir,spath);
	printf("tardir name here is %s \n",tardir);*/
	char *tardir1;
	tardir1 = tardir;
//	printf("tardir11111 name here is %s \n",tardir1);
        char Path[256], *EndPtr = Path;
        struct dirent *e;
        strcpy(Path, spath); 
		strcat(Path,"/");                 //Copies the current path to the 'Path' variable.
        EndPtr += strlen(Path);              //Moves the EndPtr to the ending position.
        while((e = readdir(dir)) != NULL  ) { 
		chdirval = chdir(currentWorkingDir);
//		printf("chdir returned val is %d\n",chdirval);	
//		printf("tardir inside while name here is %s \n",tardir);		 //Iterates through the entire directory.
           	struct stat info;                //Helps us know about stuff	
	  	if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
           		continue;
            	strcpy(EndPtr, e->d_name);       //Copies the current filename to the end of the path, overwriting it with each loop.
//		printf("e->d_name %s \n" ,e->d_name);
//		printf("path is %s\n",Path);
//		printf("stat val =%d\n",stat(Path, &info));
            	if(!stat(Path, &info)) {         //stat returns zero on success.
                	if(S_ISDIR(info.st_mode) && e->d_name!="." && e->d_name!="..") {  //Are we dealing with a directory?
                    	//Make corresponding directory in the target folder here.
                    		printf("rec call with path =%s tardir =%s\n",Path,tardir);

				MultipleSearchDirectory(Path,tardir,currentWorkingDir);   //Calls this function AGAIN, this time with the sub-name.
                	} else if(S_ISREG(info.st_mode)) { //Or did we find a regular file?
                    						//Run Copy routine
//					printf("fname is = %s ",e->d_name);
					functionforCopyforMultipleFile( e->d_name, tardir , Path,currentWorkingDir);
               		}
            	}
        }
    }
	else {
		printf("not a dir fname is = %s\n ", spath);
		functionforCopyforMultipleFile( spath, tardir,NULL,currentWorkingDir);
	}
}
int functionforCopyforMultipleFile( char *filename, char *dirname , char * to_append,char *currentWorkingDir){

	int chdirval;
	chdirval= chdir(currentWorkingDir);
//	printf("chdir returned val is at copy start is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
		//perror("getcwd() error");
		return -999;
	} 		 
//        else
//      		printf("current working directory at copy start is: %s\n", cwd);
	int sz;
	//printf("%s\n",argv[1]);
/*	printf("received fname is = %s \n",filename);
	printf("tardir is %s\n",dirname);
	printf("to append is %s\n",to_append);
*/	
	char actualDir[100];int i;char *actualTarget;
	if(to_append!=NULL){
		for( i=0;i<strlen(dirname);i++){
			actualDir[i]=dirname[i];
		}
		actualDir[i]='/';
		i++;
		for(int j=0;j<strlen(to_append);j++){
			actualDir[i]=to_append[j];i++;
		}
//		printf("appended is %s\n",actualDir);
		int j=strlen(actualDir)-1;
		do{
			actualDir[j]='\0';
			j--;
		}while(actualDir[j]!='/');
		actualDir[j]='\0';
//		printf("actual path after appended is %s\n",actualDir);
		actualTarget = (char *)actualDir;
	}
	/*read from file */
	//char *buf = (char *) calloc (100, sizeof(char));
	
	if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
			 //perror("getcwd() error");
			 return -999;
	} 	
//        else
//      		printf("current working directory is: %s\n", cwd);
	int fd = open(filename, O_RDONLY);
	printf("file descriptor fd = %d\n",fd);
	char buf[17];
	
	if(to_append!=NULL)
	 chdirval= chdir(actualTarget);
	else{
		chdirval= chdir(dirname);
	}
	//printf("chdir returned val is %d\n",chdirval);
	
	

	int fd_for_w = open(filename, O_RDWR | O_CREAT , 0777);
	printf("file descriptor fd_for_w = %d\n",fd_for_w);
	if(fd_for_w == -1){
		//printf ("error number %d\n", errno);
	//	perror("program");
	return -99;
	}
	printf("file opened \n");
	int sz1;
	sz = read(fd,buf,16);
	while(sz>0){
	//printf("called read(%d ,buf,10 ) returned %d bytes were read\n",fd,sz);
	buf[16]='\0';
	//printf("%s ",buf);
	sz = read(fd,buf,16);
	sz1 = write(fd_for_w, buf,strlen(buf));
	}
	//file close
	/*file close*/
	int close_id = close (fd);
	if(close_id < 0){
		/*printf("erroe while closeing file 1\n");
		perror("c1");
		exit(1);*/
		return -999;
	}
	//printf("file clsoed");
	close_id = close (fd_for_w);
	if(close_id < 0){
	/*	printf("erroe while closeing\n");
		perror("c1");
		exit(1);*/
		return -999;
	}
	
//	printf("file clsoed");
	//char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		//perror("getcwd() error");
		return -999;
	}
//        else
//      		printf("current working directory is: %s\n", cwd);
return 0; 
}



return 0; 
}



re