#ifndef INVOKECOMMANDS_H
#define INVOKECOMMANDS_H

#include "RenameFile.h"
#include "MakeLogs.h"
#define SPACEDELIMETER ' ' 
#include<string>
#include<vector>

using namespace std;

void exeRenameCommand(char * old,char * newnm);
size_t split(const string &txt, vector<string> &strs, char ch);

int executeCommands(string strCommand){
    
    vector<string> _CMDContent;
	split(strCommand,_CMDContent,SPACEDELIMETER);

    string commandType = _CMDContent[0];

    writeLogs(commandType);

    exeRenameCommand(NULL,NULL);
    return 0;
}

void exeRenameCommand(char * old,char * newnm)
{
    old = "/home/nitish/IIITH/Softwares/PK/31 August 2018 Workspace/workspace/jam233";
	newnm = "/home/nitish/IIITH/Softwares/PK/31 August 2018 Workspace/workspace/jam3";
	renameFiles(old,newnm);
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

#endif