#ifndef MAKELOGS_H
#define MAKELOGS_H

#define SPACEDELIMETER ' ' 
#include<string>
#include<bits/stdc++.h>
using namespace std;

ofstream myfileh;
void writeLogs(string message)
{
  	myfileh.open("logs.txt", std::ios_base::app);
  	string strFinalLog;
  	myfileh << " : " << message << endl;
  	myfileh.close();
}

#endif