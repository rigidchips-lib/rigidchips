#ifndef G_READDATA_H
#define G_READDATA_H
#include "Globals.hpp"
#include "Rigidmain.h"
#include "GVector.hpp"



#define GT_OPTION1		64 //ネットワーク送信時のビットフラグ。0x40
#define GT_OPTION2		128//ネットワーク送信時のビットフラグ0x80



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
	char Name[VALNAMEMAX];
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
