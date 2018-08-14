#ifndef PUBFUNC_H
#define PUBFUNC_H

#include <iostream>
#include <cstring>
//#include <windows.h>

using namespace std;
#define COMMAXLEN 1024

bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);



#endif // PUBFUNC_H
