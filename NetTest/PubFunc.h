#ifndef PUBFUNC_H
#define PUBFUNC_H

#define COMMAXLEN 1024

bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);



#endif // PUBFUNC_H
