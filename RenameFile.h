#ifndef RENAMEFILE_H
#define RENAMEFILE_H

using namespace std;

void renameFiles(char *old,char *newnm){
  rename(old, newnm);
}
#endif