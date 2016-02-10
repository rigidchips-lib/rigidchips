#ifndef G_READDATA_H
#define G_READDATA_H

#include "Rigidmain.h"
#include "GVector.hpp"

#define GT_CORE		0
#define GT_CHIP		1
#define GT_RUDDER	2
#define GT_DUMMY	3
#define GT_WHEEL	4
#define GT_RLW		5
#define GT_TRIM		6
#define GT_JET		7
#define GT_CHIPH	8
#define GT_COWL		9
#define GT_ARM		10
#define GT_CHIP2	33
#define GT_RUDDER2	34
#define GT_TRIM2	35
#define GT_WHEEL2	36
#define GT_RLW2		37

#define GT_OPTION1		64
#define GT_OPTION2		128

#define GT_BALLOBJ		31

typedef struct {
	int ValNo[GVALMAX];
	GFloat Step[GVALMAX];
	bool ResetFlag[GVALMAX];
	bool ReleaseFlag[GVALMAX];
	GFloat Step2[GVALMAX];
	bool ResetFlag2[GVALMAX];
	bool Pressed;
	bool Down;
	bool Up;
	int Count;
	int Lock;
	bool SPressed;
	bool SDown;
	bool SUp;
} GKeyList;
typedef struct {
	bool  Updated;
	char Name[256];
	GFloat Min;
	GFloat Max;
	GFloat Def;
	GFloat Val;
	GFloat Dec;
	GFloat RecVal;
	bool  RecUpdated;
	GFloat *Ref[GREFMAX];
	bool Flag[GREFMAX];
	int visible;
	int Label;
	int RefCount;
} GValList;

void MakeChip(int type, int rn);
int readChildData(FILE *fp, int parentNo, bool checkFlag);
int readData(char *fname, bool checkFlag);
int readData2(FILE *fp, bool checkFlag);

int saveData(char *fname);

#endif
