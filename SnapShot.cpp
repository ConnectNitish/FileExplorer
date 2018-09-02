#include "SnapShot.h"

char pathname[MAXPATHLENGTH];

/* */
/*
////////////TO REMOVE 


char * GetCharPointerFromString_TOREMOVEDELSS(string param1)
{
    char * _CPointer = new char[param1.length() + 1];
    strcpy(_CPointer, param1.c_str());
    return _CPointer;
}

ofstream myfilehdelSS;
void writeLogsDelete_TOREMOVESS(string message)
{
  	myfilehdelSS.open("logs.txt", std::ios_base::app);
  	string strFinalLog;
  	myfilehdelSS << " SS File :" << message << endl;
  	myfilehdelSS.close();
}
//writeLogsDelete_TOREMOVE(string(path));

int xdiraccessCheck_TOREMOVESS(const char *path)
{
	static DIR *dirp;
	dirp = opendir(path);
	if (dirp == NULL) {
		return 0;
	}
	closedir(dirp);
	return 1;
}

////////////// END REMOVE 
*/
/**
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 * Master is BaseDir
 * Others are NesDir
 */

void SetAllFilesRecursively(char * Path,vector<pair<string,string>> &directoryInfo)
{
    char path[MAXPATHLENGTH]; 
    string temporaryPath;
    struct dirent *direntdp; 
    DIR *pathDIR = opendir(Path);
    if(pathDIR)
    {
        temporaryPath = string(Path);
//        writeLogsDelete_TOREMOVESS("PATH 1");
        
       // writeLogsDelete_TOREMOVESS(temporaryPath);
        directoryInfo.push_back(pair<string,string>(temporaryPath,"BaseDir"));

        while((direntdp = readdir(pathDIR))!=NULL)
        {
        string strTemp;
            if (strcmp(direntdp->d_name, ".") != 0 && strcmp(direntdp->d_name, "..") != 0)
            {
                strcpy(path, direntdp->d_name);
                strTemp = temporaryPath+"/"+string(path);
//                writeLogsDelete_TOREMOVESS(strTemp);
                directoryInfo.push_back(pair<string, string> (strTemp,"NesDir"));
            }    
        }
        closedir(pathDIR);
    }
}

int runSnapShot(const char * param1,const char * param2,char * currDir)
{
    char path[MAXPATHLENGTH];
    char destinationPath[MAXPATHLENGTH];

    vector<pair<string,string>> directoryInfo;

    strcpy(path,param1);
    strcpy(destinationPath,param2);

  //  writeLogsDelete_TOREMOVESS("Start SS");
    
//    writeLogsDelete_TOREMOVESS(path);
//    writeLogsDelete_TOREMOVESS(destinationPath);

    SetAllFilesRecursively(path,directoryInfo);

    int i=1;
    while(i<directoryInfo.size()){

    	strcpy(path, (directoryInfo[i].first).c_str());
    	if(directoryInfo[i].second=="NesDir")
    	SetAllFilesRecursively(path,directoryInfo);
    	i++;
    }

/*
                writeLogsDelete_TOREMOVESS("directoryInfo[i].first");
    for(int i=0;i<directoryInfo.size();i++)
    {

                writeLogsDelete_TOREMOVESS(directoryInfo[i].first);
                                writeLogsDelete_TOREMOVESS(directoryInfo[i].second);
    }
*/
    //            writeLogsDelete_TOREMOVESS("directoryInfo[i].firstEND ");

    //DisplayDir(directoryInfo);
    createManifestOfData(directoryInfo,destinationPath);

    //writeLogsDelete_TOREMOVESS("END SS");
    
    return 0;
}
/*
int main(int argc, char const *arguments[])
{
    char * currDir = NULL;
    runSnapShot(arguments[1],arguments[2],currDir);
    return 0;

}
*/
void DisplayDir(vector<pair<string, string>> directory){
	for(int i=0;i<directory.size();i++)
		if(directory[i].second=="BaseDir")
			printf("\n\nDirectory: %s%s\n\n",directory[i].first,directory[i].second);
		else
			printf("\n\nDirectory: %s%s\n\n",directory[i].first,directory[i].second);
}

void createManifestOfData(vector<pair<string,string>> directoryInfo,string destinationPath){
    //writeLogsDelete_TOREMOVESS("File Path For Writing ");
	
    //writeLogsDelete_TOREMOVESS(destinationPath);
	ofstream myTempfile;
	myTempfile.open (destinationPath);
	for(int i=0;i<directoryInfo.size();i++){
        string strInfo = "";
        strInfo = directoryInfo[i].first;
        size_t found = strInfo.find_last_of("/\\");

        if(found>0)
        {
            strInfo = strInfo.substr(found+1);
        }
        
 		if(directoryInfo[i].second=="BaseDir")
			//myTempfile<<"\n\nDirectory: "<<directoryInfo[i].first<<"\n\n";
    		myTempfile<<"\n\nDirectory: "<<strInfo<<"\n\n";
        else
			myTempfile<<strInfo<<"\t";
	}	
	myTempfile.close();  
}

