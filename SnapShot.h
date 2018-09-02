#ifndef SNAPSHOT_H

#define SNAPSHOTSUCCESS 100
#define MAXPATHLENGTH 512

//void DisplayDir(vector<pair<string, string>> directory);
//void createManifestOfData(vector<pair<string,string>> directoryInfo,string destinationPath);

#include<iostream>
#include<fstream>
#include<string.h>
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
#include <dirent.h>
using namespace std;


void DisplayDir(vector<pair<string, string>> directory);
void createManifestOfData(vector<pair<string,string>> directoryInfo,string destinationPath);
int runSnapShot(const char * param1,const char * param2,char * currDir);


#endif