#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string>
using namespace std;

void SearchDirectoryMove( char *name , char tardir[] ,char *currentWorkingDir);
void MultipleSearchDirectoryMove(char *spath , char *tardir ,char *currentWorkingDir);
void functionforCopyMove( char *filename, char *dirname , char * to_append,char *currentWorkingDir);
void functionforCopyforMultipleFileMove( char *filename, char *dirname , char * to_append,char *currentWorkingDir);
void removeDirectory(const char path[]);
void removeFileFromSystem(const char path[] );

int executeMoveCommand(int ac,char *av[],char * currDir)
{
	char cwd[256];int chdirval;
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
	char tPath[256];char spath[256];
	if ( ac == 3){
		strcpy(tPath,"/");

		#pragma region BuildStringBasedOnParam

        int lengthofString = strlen(av[2]);
        char *temp = av[2];
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
        char c1=paramValue[0];
        char c2=paramValue[1];
        int tpath_length = strlen(tPath);
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
            strcat(tPath,currDir);
             tpath_length = strlen(tPath);
            for(int ii=1;ii<strlen(paramValue);ii++)
            {
                char c1=paramValue[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
            }
            tPath[tpath_length]='\0';
        }
		else{
    		 tpath_length = strlen(tPath);
			
			
			lengthofString = strlen(av[2]);
			char *temp2 = av[2];
        char  paramValue2[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue2[ii] = temp2[ii];
        }
            for(int ii=1;ii<strlen(paramValue2);ii++)
            {
                char c1=paramValue2[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
            }
            tPath[tpath_length]='\0';
        }

#pragma endregion BuildStringBasedOnParam
		
		stpcpy(spath,av[1]);
		SearchDirectoryMove( spath, tPath,cwd);
		chdirval = chdir(cwd);
		removeDirectory(av[1]);
	}
	else if(ac>3){	
		for(int i=1;i<ac-1;i++){
			strcpy(tPath,"/");
			
			#pragma region BuildStringBasedOnParamMultiple

        int lengthofString = strlen(av[ac-1]);
        char *temp = av[ac-1];
        char  paramValue[1000]; 
        for(int ii=0;ii<lengthofString;ii++){
            paramValue[ii] = temp[ii];
        }
        char c1=paramValue[0];
        char c2=paramValue[1];
		int tpath_length = strlen(tPath);
        if(lengthofString >1 && c1=='.'  && c2!='.')
        {
            strcat(tPath,currDir);
            tpath_length = strlen(tPath);
            for(int ii=1;ii<strlen(paramValue);ii++)
            {
                char c1=paramValue[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
            }
            tPath[tpath_length]='\0';
        }
		else
		{
            
			tpath_length = strlen(tPath);
			lengthofString = strlen(av[ac-1]);
			char *temp2 = av[ac-1];
        	char  paramValue2[1000]; 
        	for(int ii=0;ii<lengthofString;ii++){
            paramValue2[ii] = temp2[ii];
        }
		    for(int ii=1;ii<strlen(paramValue2);ii++)
            {
                char c1=paramValue2[ii];
                tPath[tpath_length] = c1;
                tpath_length++;
            }
            tPath[tpath_length]='\0';
        }
		
#pragma endregion BuildStringBasedOnParamMultiple
			
			strcat(spath,av[i]);
			MultipleSearchDirectoryMove( spath, tPath,cwd);
			chdirval = chdir(cwd);
			removeDirectory(av[i]);
			strcpy(tPath,"\0");
			strcpy(spath,"\0");
		}
	}
	return 0;
}
/*
int main(int ac, char *av[]){
	 char * currDir = (char *)"/home/chitta/FileExplorer"; 
	invokeMoveCommand(ac,av,currDir);
	return 0;
}*/

void SearchDirectoryMove(char *spath , char *tardir ,char *currentWorkingDir) {
    DIR *dir = opendir(spath);  	              //Assuming absolute pathname here.
    if(dir) {
	char *dirname = (char *)tardir;
	int chdirval = chdir(dirname);
	if (mkdir(spath,0777) == -1) {
        	//perror(argv[0]);
        	//exit(EXIT_FAILURE);
		//printf("failed to make dir\n");
		return;
   	}
	chdirval = chdir(currentWorkingDir);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
	char *tardir1;
	tardir1 = tardir;
        char Path[256], *EndPtr = Path;
        struct dirent *e;
        strcpy(Path, spath); 
	    strcat(Path,"/");                 //Copies the current path to the 'Path' variable.
        EndPtr += strlen(Path);              //Moves the EndPtr to the ending position.
        while((e = readdir(dir)) != NULL  ) { 
		chdirval = chdir(currentWorkingDir);
		struct stat info;                //Helps us know about stuff	
	  	if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
           		continue;
        strcpy(EndPtr, e->d_name);       //Copies the current filename to the end of the path, overwriting it with each loop.
		if(!stat(Path, &info))
        {         //stat returns zero on success.
                if(S_ISDIR(info.st_mode) && e->d_name!="." && e->d_name!="..") 
                {  //Are we dealing with a directory?
                //Make corresponding directory in the target folder here.
                SearchDirectoryMove(Path,tardir,currentWorkingDir);   //Calls this function AGAIN, this time with the sub-name.
                	} 
                    else if(S_ISREG(info.st_mode)) 
                    { 
                    //Or did we find a regular file?
                    //Run Copy routine
					functionforCopyMove( e->d_name, tardir , Path,currentWorkingDir);
               		}
            	}
        }
    }
	else {
		functionforCopyMove( spath, tardir,NULL,currentWorkingDir);
	}
}




void functionforCopyMove( char *filename, char *dirname , char * to_append,char *currentWorkingDir){

	int chdirval;
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
	int sz;
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
		int j=strlen(actualDir)-1;
		do{
			actualDir[j]='\0';
			j--;
		}while(actualDir[j]!='/');
		actualDir[j]='\0';
		actualTarget = (char *)actualDir;
	}
	
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
//        else
//      		printf("current working directory is: %s\n", cwd);
	int fd = open(filename, O_RDONLY);
//	printf("file descriptor fd = %d\n",fd);
	char buf[17];
	/*sz = read(fd,buf,16);
	while(sz>0){
	//printf("called read(%d ,buf,10 ) returned %d bytes were read\n",fd,sz);
	buf[16]='\0';
	printf("%s ",buf);
	sz = read(fd,buf,16);
	}*/
//	printf("Before CHDIR dirnamae %s\n",dirname);
	
//	printf("Before CHDIR actualTarget %s\n",actualTarget);
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
//		perror("program");
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
	}
//	printf("file clsoed");
	close_id = close (fd_for_w);
	if(close_id < 0){
//		printf("erroe while closeing\n");
//		perror("c1");
//		exit(1);
	}
	
//	printf("file clsoed");
	//char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
//        else
//      		printf("current working directory is: %s\n", cwd);

}



void removeFileFromSystem(const char path[] ){
	//char *full_path; 
	if (unlink(path) == 0) {}
//            printf("Removed a file: %s\n", path);
//        else
//            printf("Can`t remove a file: %s\n", path);
}
void removeDirectory(const char path[])
{
    size_t path_len;
    char *actual_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    // stat for the path
    stat(path, &stat_path);

    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0) {
 //        fprintf(stderr, "%s: %s\n", "Is not directory", path);
	     removeFileFromSystem(path);
         return;
    }

    // if not possible to read the directory for this user
    if ((dir = opendir(path)) == NULL) {
 //       fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
 //       exit(-1);
 		return;
    }

    // the length of the path
    path_len = strlen(path);

    // iteration through entries in the directory
    while ((entry = readdir(dir)) != NULL) {

        // skip entries "." and ".."
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // determinate a full path of an entry
        actual_path = (char *)calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(actual_path, path);
        strcat(actual_path, "/");
        strcat(actual_path, entry->d_name);

        // stat for the entry
        stat(actual_path, &stat_entry);
	//printf("ful path si %s \n",actual_path);
        // recursively remove a nested directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            removeDirectory(actual_path);
            continue;
        }

        // remove a file object
        if (unlink(actual_path) == 0) {}
//            printf("Removed a file: %s\n", actual_path);
//        else
//            printf("Can`t remove a file: %s\n", actual_path);
    }

    // remove the devastated directory and close the object of it
    if (rmdir(path) == 0) {}
//        printf("Removed a directory: %s\n", path);
//    else
//        printf("Can`t remove a directory: %s\n", path);

    closedir(dir);
}

void MultipleSearchDirectoryMove(char *spath , char *tardir ,char *currentWorkingDir) {
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
//        	exit(EXIT_FAILURE);
//		printf("failed to make dir\n");
		return;
   	}
	chdirval = chdir(currentWorkingDir);
//	printf("chdir returned val is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
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
//                    		printf("rec call with path =%s tardir =%s\n",Path,tardir);

				MultipleSearchDirectoryMove(Path,tardir,currentWorkingDir);   //Calls this function AGAIN, this time with the sub-name.
                	} else if(S_ISREG(info.st_mode)) { //Or did we find a regular file?
                    						//Run Copy routine
//					printf("fname is = %s ",e->d_name);
					/*char *actualTarget;
					strcpy(actualTarget,tardir);
					strcat(actualTarget,"/");
					strcat(actualTarget,Path);
					int l= strlen(Path);
					int i=l-1;*/
					/*do{
					actualTarget[i] = '\0';
					i--;
					}while(actualTarget[i]!='/');*/
					//printf("sent actualTarget = %s ",actualTarget);
					functionforCopyforMultipleFileMove( e->d_name, tardir , Path,currentWorkingDir);
               		}
            	}
        }
    }
	else {
//		printf("not a dir fname is = %s\n ", spath);
		functionforCopyforMultipleFileMove( spath, tardir,NULL,currentWorkingDir);
	}
}
void functionforCopyforMultipleFileMove( char *filename, char *dirname , char * to_append,char *currentWorkingDir){

	int chdirval;
	chdirval= chdir(currentWorkingDir);
//	printf("chdir returned val is at copy start is %d\n",chdirval);
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
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
	
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
//        else
//      		printf("current working directory is: %s\n", cwd);
	int fd = open(filename, O_RDONLY);
//	printf("file descriptor fd = %d\n",fd);
	char buf[17];
	/*sz = read(fd,buf,16);
	while(sz>0){
	//printf("called read(%d ,buf,10 ) returned %d bytes were read\n",fd,sz);
	buf[16]='\0';
	printf("%s ",buf);
	sz = read(fd,buf,16);
	}*/
	
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
//		perror("program");
		return;
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
		return ;
	}
//	printf("file clsoed");
	close_id = close (fd_for_w);
	if(close_id < 0){
//		printf("erroe while closeing\n");
//		perror("c1");
//		exit(1);
		return ;
	}
	
//	printf("file clsoed");
	//char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {}
//     		 perror("getcwd() error");
//        else
//      		printf("current working directory is: %s\n", cwd);
}


