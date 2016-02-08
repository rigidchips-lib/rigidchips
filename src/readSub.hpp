#ifndef READSUB_H
#define READSUB_H

#include <stdio.h>
int  getCharData(FILE *fp);
int  getStrData(FILE *fp,char *str);
int  getStrData2(FILE *fp,char *str);
void searchData(FILE *fp,int c);
int  getToken(FILE *fp,char *str);
void  resetToken();
int  getToken2(FILE *fp,char *str);
int GetTokenCh(void);
#endif
