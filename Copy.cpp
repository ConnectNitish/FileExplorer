#include	<stdio.h>
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

void SearchDirectory(char *sourceFileName , char *target ,char *currentWorkingDir) ;
void put_dirs(char *sourceFileName , char *target ,char * to_append, char *currentWorkingDir,int f,int cd) ;

int executeCopyCommand(int ac,char *av[],char * currDir)
{
char cwd[256];char cwd1[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {} 
  /*   		 perror("getcwd() error");
        else
      		printf("current working directory in main is: %s\n", cwd); */
	strcpy(cwd1,cwd);
	char target[256];char sourceFileName[256];
    
	if ( ac == 3){
		strcpy(target,"/");
		//////
		////strcpy(target,av[2]);///OLD 
		//////

        
		#pragma region COPYCODE

        int lengthofString = strlen(av[2]);
        //char *temp = av[2];
        char temp[1000];
        strcpy(temp,av[2]);
       // printf(" temp ~~~~~ %s %d\n",temp,strlen(temp));
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
        char c1=paramValue[0];
        char c2=paramValue[1];
        int tpath_length = strlen(target);
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
            strcat(target,currDir);
            strcat(target,"/");
            strcat(target,temp);
           // printf("target is %s\n\n",target);
             tpath_length = strlen(target);
             //printf("~~~~~ %s %d\n",paramValue,strlen(paramValue));
         /*   for(int ii=1;ii<strlen(paramValue);ii++)
            {
                char c1=paramValue[ii];
                target[tpath_length] = c1;
                tpath_length++;
			}
            target[tpath_length]='\0';  */          
        }
		else{
    	   strcat(target,av[2]);
	     }
#pragma endregion COPYCODE


		strcat(sourceFileName,av[1]);
		//printf("sourceFileName is %s \n target is = %s\n",sourceFileName,target);
		SearchDirectory( sourceFileName, target,cwd);
	}
	else if(ac>3){	
		//printf("multi file copy\n\n");
		for(int i=1;i<ac-1;i++){
			char *dirname = (char *)cwd1;
			int chdirval = chdir(dirname);
			//printf("multi file copy\n\n");
			
            ////
            strcpy(target,av[ac-1]);
            ////






			strcpy(sourceFileName,av[i]);
			//printf("spath is %s \ntpath is = %s\n",sourceFileName,target);
			
			SearchDirectory( sourceFileName, target,cwd);
			strcpy(target,"\0");
			strcpy(sourceFileName,"\0");
		}
	}
	return 0;
}

/*
int main(int ac, char *av[]){
	 char * currDir = (char *)"/home/nitish/IIITH/Semester 1/FileExplorer"; 
	executeCopyCommand(ac,av,currDir);
	return 0;
}
*/

/*
int main(int ac, char *av[]){
	char cwd[256];char cwd1[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
     		 perror("getcwd() error");
        else
      		printf("current working directory in main is: %s\n", cwd);
	strcpy(cwd1,cwd);
	char target[256];char sourceFileName[256];
	if ( ac == 3){
		
		//////
		////strcpy(target,av[2]);///OLD 
		//////


		#pragma region COPYCODE

        int lengthofString = strlen(av[2]);
        char *temp = av[2];
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
        char c1=paramValue[0];
        char c2=paramValue[1];
        int tpath_length = strlen(target);
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
            strcat(target,currDir);
             tpath_length = strlen(target);
            for(int ii=1;ii<strlen(paramValue);ii++)
            {
                char c1=paramValue[ii];
                target[tpath_length] = c1;
                tpath_length++;
			}
            target[tpath_length]='\0';            
        }
		else{
    	   strcat(target,av[2]);
	     }
#pragma endregion COPYCODE


		strcpy(sourceFileName,av[1]);
		printf("sourceFileName is %s \n target is = %s\n",sourceFileName,target);
		SearchDirectory( sourceFileName, target,cwd);
	}
	else if(ac>3){	
		printf("multi file copy\n\n");
		for(int i=1;i<ac-1;i++){
			char *dirname = (char *)cwd1;
			int chdirval = chdir(dirname);
			printf("multi file copy\n\n");
			strcpy(target,av[ac-1]);
			strcpy(sourceFileName,av[i]);
			////printf("spath is %s \ntpath is = %s\n",spath,tPath);
			
			SearchDirectory( sourceFileName, target,cwd);
			strcpy(target,"\0");
			strcpy(sourceFileName,"\0");
		}
	}
	return 0;
}*/


void SearchDirectory(char *sourceFileName , char *target ,char *currentWorkingDir) {
	//	printf("received sourceFileName is %s \n target is = %s\n",sourceFileName,target);
		DIR *dir = opendir(sourceFileName);  	              //Assuming absolute pathname here.
		if(!dir){
			//copy_file( spath, tardir,NULL,currentWorkingDir);
			put_dirs(sourceFileName,target,NULL,currentWorkingDir,1,0);
			return;
		}
		if(dir) {
			put_dirs(sourceFileName,target,NULL,currentWorkingDir,0,0);
			char Path[256], *EndPtr = Path;
			struct dirent *e;
			strcpy(Path, sourceFileName); 
			strcat(Path,"/");                 //Copies the current path to the 'Path' variable.
			EndPtr += strlen(Path);              //Moves the EndPtr to the ending position.
			while((e = readdir(dir)) != NULL  ) { 
	//			printf("for chdir target path is %s\n",currentWorkingDir);	
				int chdirval = chdir(currentWorkingDir);
	//			printf("chdir returned val is %d\n",chdirval);	
	//			printf("tardir inside while name here is %s \n",target);		 //Iterates through the entire directory.
				struct stat info;                //Helps us know about stuff	
				if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
					continue;
            	strcpy(EndPtr, e->d_name);     
				if(!stat(Path, &info)) {         //stat returns zero on success.
                	if(S_ISDIR(info.st_mode) && e->d_name!="." && e->d_name!="..") {  //Are we dealing with a directory?
                    	//Make corresponding directory in the target folder here.
    //                		printf("rec call with path =%s tardir =%s\n",Path,target);
							SearchDirectory(Path,target,currentWorkingDir);   //Calls this function AGAIN, this time with the sub-name.
                	} else if(S_ISREG(info.st_mode)) { //Or did we find a regular file?
                    						//Run Copy routine
	//						printf("fname is = %s ",e->d_name);
							char cwd[256];
							if (getcwd(cwd, sizeof(cwd)) == NULL) {}
	//						    perror("getcwd() error");
	//						else
	//							printf("current working directory in put_dirs is: %s\n", cwd);
			
							put_dirs(e->d_name,target,Path,currentWorkingDir,1,0);
							//copy_file( e->d_name, tardir , Path,currentWorkingDir);
					}
				}
			}
		}
}
//void copy_file1( char *filename, char *dirname , char * to_append,char *currentWorkingDir)
void put_dirs(char *sourceFileName , char *target ,char * to_append, char *currentWorkingDir,int f,int cd) {
	if(f==0){
		char *dirname = (char *)target;
		int chdirval = chdir(dirname);
	//	printf("chdir returned val  in put_dirs  is %d\n",chdirval);
	//	printf("for chdir dirname name  in put_dirs  is %s \n",dirname);
		if (mkdir(sourceFileName,0777) == -1) {
				//perror(argv[0]);
	//		printf("failed to make dir\n");
			//exit(EXIT_FAILURE);
		}
		chdirval = chdir(currentWorkingDir);
	//	printf("chdir returned val  in put_dirs  is %d\n",chdirval);
		char cwd[256];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {}
	//			perror("getcwd() error");
    //    else
	//			printf("current working directory in put_dirs is: %s\n", cwd);
			return;
	}
	else if(f==1 && cd==1){
		int chdirval;
		chdirval= chdir(currentWorkingDir);
	//	printf("chdir returned val is at copy start is %d\n",chdirval);
	}
	//printf("received fname is = %s \n",sourceFileName);
	//printf("tardir is %s\n",target);
	//printf("to append is %s\n",to_append);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL){}
    // 		 perror("getcwd() error");
    //    else
    //  		printf("current working directory at copy start is: %s\n", cwd);
	int sz;int chdirval;
	char actualDir[100];int i;char *actualTarget;
	char curract[100];char *actualsource;
	if(to_append!=NULL){
		for( i=0;i<strlen(target);i++){
			actualDir[i]=target[i];
		}
		actualDir[i]='/';
		i++;
		for(int j=0;j<strlen(to_append);j++){
			actualDir[i]=to_append[j];i++;
		}
	//	printf("appended is %s\n",actualDir);
		int j=strlen(actualDir)-1;
		do{
			actualDir[j]='\0';
			j--;
		}while(actualDir[j]!='/');
		actualDir[j]='\0';
	//	printf("actual path after appended is %s\n",actualDir);
		actualTarget = (char *)actualDir;
	}
	if(to_append!=NULL){
		for( i=0;i<strlen(currentWorkingDir);i++){
			curract[i]=currentWorkingDir[i];
		}
		curract[i]='/';
		i++;
		for(int j=0;j<strlen(to_append);j++){
			curract[i]=to_append[j];i++;
		}
	//	printf("appended is %s\n",curract);
		int j=strlen(curract)-1;
		do{
			curract[j]='\0';
			j--;
		}while(curract[j]!='/');
		curract[j]='\0';
	//	printf("actual path after appended is %s\n",curract);
		actualsource = (char *)curract;
	}
	chdirval= chdir(actualsource);
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
    // 		 perror("getcwd() error");
    //    else
    //  		printf("current working directory is: %s\n", cwd);
	int fd = open(sourceFileName, O_RDONLY);
	//printf("file descriptor fd for file%s = %d\n",sourceFileName,fd);
	char buf[17];
	if(to_append!=NULL)
	 chdirval= chdir(actualTarget);
	else{
		chdirval= chdir(target);
	}
	//printf("chdir returned val is %d\n",chdirval);
	
	

	int fd_for_w = open(sourceFileName, O_RDWR | O_CREAT , 0777);
	//printf("file descriptor fd_for_w = %d\n",fd_for_w);
	if(fd_for_w == -1){
		//printf ("error number %d\n", errno);
	//	perror("program");
	}
	//printf("file opened \n");
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
	//	printf("erroe while closeing file 1\n");
	//	perror("c1");
		//exit(1);
	}
	//printf("file clsoed");
	close_id = close (fd_for_w);
	if(close_id < 0){
	//	printf("erroe while closeing\n");
	//	perror("c1");
		//exit(1);
	}
	
	//printf("file clsoed");
	//char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
    // 		 perror("getcwd() error");
    //    else
    //  		printf("current working directory is: %s\n", cwd);

}