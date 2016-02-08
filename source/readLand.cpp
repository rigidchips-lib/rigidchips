#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "GVector.hpp"
#include "GRigid.hpp"
#include "readLand.hpp"
#include "readSub.hpp"



extern int ChipCount;
extern int ValCount;
#define GDEFCD 1.0


void readLand(char *name)
{
	FILE *fp;
	char str[256];
	int i,cw,n,vn;
	if((fp=fopen(name,"r"))==NULL) return;
}
