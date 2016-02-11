#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include "Globals.hpp"
#include "GVector.hpp"
#include "GRigid.hpp"
#include "readData.hpp"
#include "readSub.hpp"
#include "luaScript.hpp"
#include "luaSystem.hpp"
#include "GDPlay.hpp"
#include "GPlayers.h"

//メモリリーク検出用
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 
//--メモリリーク検出用

#define GDEFCD 1.0

void MakeChip(int type, int rn)
{
	int cn = g_World->getChipCount();
	if (type == GT_CORE) {  //Core		
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 0;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.3f;
		g_Chip[cn]->Ud = 0.2f;
		g_Chip[cn]->Cd = GDEFCD;
		g_Chip[cn]->X = GVector(0, 5, 0);
		g_Chip[cn]->R = GMatrix();
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 2000000.0f;
		rn = 0;
	}
	else if (type == GT_CHIP) { //g_Chip
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 1;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.3f;
		g_Chip[cn]->Ud = 0.2f;
		g_Chip[cn]->Cd = GDEFCD;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 1000000.0f;
	}
	else if (type == GT_CHIPH) { //ChipH
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 21;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f * 4;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.1f;
		g_Chip[cn]->Us = 2.2f;
		g_Chip[cn]->Ud = 1.0f;
		g_Chip[cn]->Cd = GDEFCD;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 6000000.0f;
	}
	else if (type == GT_COWL) { //cowl
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 23;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = 0.00f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.0;
		g_Chip[cn]->Ud = 0.0f;
		g_Chip[cn]->Cd = 0;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 0.0f;
		g_Chip[cn]->Effect = 0x0000fb;

	}
	else if (type == GT_CHIP2) { //Frame
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 12, CHIPSIZE);
		g_Chip[cn]->Shape.PointN--;
		g_Chip[cn]->MeshNo = 7;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 1;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.1f;
		g_Chip[cn]->Us = 0.8f;
		g_Chip[cn]->Ud = 0.6f;
		g_Chip[cn]->Cd = 0.0f;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 500000.0f;
	}
	else if (type == GT_RUDDER) { //Rudder
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 4;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.3f;
		g_Chip[cn]->Ud = 0.2f;
		g_Chip[cn]->Cd = GDEFCD;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 1000000.0f;
	}
	else if (type == GT_RUDDER2) { //Rudder2
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 12, CHIPSIZE);
		g_Chip[cn]->Shape.PointN--;
		g_Chip[cn]->MeshNo = 16;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 1;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.1f;
		g_Chip[cn]->Us = 0.8f;
		g_Chip[cn]->Ud = 0.6f;
		g_Chip[cn]->Cd = 0.0f;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 500000.0f;
	}
	else if (type == GT_TRIM) { //Trim
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 5;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.3f;
		g_Chip[cn]->Ud = 0.2f;
		g_Chip[cn]->Cd = GDEFCD;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 1000000.0f;
	}
	else if (type == GT_TRIM2) { //Trim2
		g_Chip[cn] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 12, CHIPSIZE);
		g_Chip[cn]->Shape.PointN--;
		g_Chip[cn]->MeshNo = 17;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 1;
		g_Chip[cn]->Density = g_Chip[cn]->Density*0.7f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.1f;
		g_Chip[cn]->Us = 0.8f;
		g_Chip[cn]->Ud = 0.6f;
		g_Chip[cn]->Cd = 0.0f;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 500000.0f;
	}
	else if (type == GT_DUMMY) { //Dummy
		g_Chip[cn] = g_World->AddRigid(GTYPE_DISK, false, CHIPSIZE / 2, CHIPSIZE, CHIPSIZE / 2);
		g_Chip[cn]->MeshNo = 6;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->Density = g_Chip[cn]->Density*1.0f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.2f;
		g_Chip[cn]->Us = 0.3f;
		g_Chip[cn]->Ud = 0.2f;
		g_Chip[cn]->Cd = 0.0f;
		g_Chip[cn]->FuelMax = g_Chip[cn]->Fuel = 0.0f;
	}
	else if (type == GT_WHEEL) { //Wheel
		g_Chip[cn] = g_World->AddRigid(GTYPE_DISK, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[cn]->MeshNo = 2;
		g_Chip[cn]->ChipType = type;
		g_Chip[cn]->FrameFlag = 0;
		g_Chip[cn]->Density = g_Chip[cn]->Density*1.3f;
		g_Chip[cn]->Reset();
		g_Chip[cn]->E = 0.01f;
		g_Chip[cn]->Us = 1.5f;
		g_Chip[cn]->Ud = 0.9f;
		g_Chip[cn]->Cd = 0.05f;
		g_Chip[cn]->FuelMax = g_Chip[g_World->getChipCount()]->Fuel = 0.0f;
	}
	else if (type == GT_RLW) { //RLW
		g_Chip[g_World->getChipCount()] = g_World->AddRigid(GTYPE_DISK, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[g_World->getChipCount()]->MeshNo = 3;//無反動ホイール
		g_Chip[g_World->getChipCount()]->ChipType = type;
		g_Chip[g_World->getChipCount()]->FrameFlag = 0;
		g_Chip[g_World->getChipCount()]->Density = g_Chip[g_World->getChipCount()]->Density*1.3f;
		g_Chip[g_World->getChipCount()]->Reset();
		g_Chip[g_World->getChipCount()]->E = 0.01f;
		g_Chip[g_World->getChipCount()]->Us = 1.5f;
		g_Chip[g_World->getChipCount()]->Ud = 0.9f;
		g_Chip[g_World->getChipCount()]->Cd = 0.05f;
		g_Chip[g_World->getChipCount()]->FuelMax = g_Chip[g_World->getChipCount()]->Fuel = 0.0f;
	}
	else if (type == GT_JET) { //Jet
		g_Chip[g_World->getChipCount()] = g_World->AddRigid(GTYPE_DISK, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[g_World->getChipCount()]->MeshNo = 10;
		g_Chip[g_World->getChipCount()]->ChipType = type;
		g_Chip[g_World->getChipCount()]->FrameFlag = 0;
		g_Chip[g_World->getChipCount()]->Density = g_Chip[g_World->getChipCount()]->Density*1.3f;
		g_Chip[g_World->getChipCount()]->Reset();
		g_Chip[g_World->getChipCount()]->E = 0.2f;
		g_Chip[g_World->getChipCount()]->Us = 0.3f;
		g_Chip[g_World->getChipCount()]->Ud = 0.1f;
		g_Chip[g_World->getChipCount()]->Cd = 0.000;
		g_Chip[g_World->getChipCount()]->FuelMax = g_Chip[g_World->getChipCount()]->Fuel = 0.0f;
	}
	else if (type == GT_ARM) { //Arm
		g_Chip[g_World->getChipCount()] = g_World->AddRigid(GTYPE_FACE, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
		g_Chip[g_World->getChipCount()]->MeshNo = 30;
		g_Chip[g_World->getChipCount()]->ChipType = type;
		g_Chip[g_World->getChipCount()]->FrameFlag = 0;
		g_Chip[g_World->getChipCount()]->Density = g_Chip[g_World->getChipCount()]->Density*0.7f * 2;
		g_Chip[g_World->getChipCount()]->Reset();
		g_Chip[g_World->getChipCount()]->E = 0.2f;
		g_Chip[g_World->getChipCount()]->Us = 0.9f;
		g_Chip[g_World->getChipCount()]->Ud = 0.6f;
		g_Chip[g_World->getChipCount()]->Cd = GDEFCD*0.7f;
		g_MyPlayerData.haveArm++;
		g_World->haveArm++;
		g_Chip[g_World->getChipCount()]->FuelMax = g_Chip[g_World->getChipCount()]->Fuel = 0.0f;
	}
	g_Chip[g_World->getChipCount()]->SetTensor();
	g_Chip[g_World->getChipCount()]->Dir = rn;
}

int getValNo(char *name)
{
	int vn = 0;
	while (vn < g_VarCount && strcmp(g_ValList[vn].Name, name) != 0) vn++;
	if (vn >= g_VarCount) vn = -1;
	return vn;

}
int getValNo2(char *name, bool *minusFlag)
{
	int vn = 0;
	if (name[0] == '-') {
		*minusFlag = true;
		name++;
	}
	else *minusFlag = false;
	while (vn < g_VarCount && strcmp(g_ValList[vn].Name, name) != 0) vn++;
	if (vn >= g_VarCount) vn = -1;
	return vn;

}

void setOption(GRigid *rigid, GFloat value)
{
	rigid->Option = value;
	if (rigid->ChipType == 10) {	//ARM
		rigid->ArmEnergy = value;
	}
	else if (rigid->ChipType == 4 || rigid->ChipType == 5) { //Wheel or RLW
		if (value == 1) {
			g_World->UpdateRigid(rigid, GTYPE_DISK, false, CHIPSIZE*1.5f, CHIPSIZE / (6 * 1.5f*1.5f), CHIPSIZE*1.5f);
			rigid->Ud = 0.9f;
			rigid->Us = 1.0f;
			rigid->E = 0;
			rigid->CheckShape = rigid->Shape;
			rigid->SaveShape = rigid->Shape;
		}
		else if (value == 2) {
			g_World->UpdateRigid(rigid, GTYPE_DISK, false, CHIPSIZE * 2, CHIPSIZE / 24.0f, CHIPSIZE * 2);
			rigid->Ud = 0.9f;
			rigid->Us = 1.0f;
			rigid->CheckShape = rigid->Shape;
			rigid->E = 0;
			rigid->SaveShape = rigid->Shape;
		}
		else {
			g_World->UpdateRigid(rigid, GTYPE_DISK, false, CHIPSIZE, CHIPSIZE / 6.0f, CHIPSIZE);
			rigid->Ud = 0.9f;
			rigid->Us = 1.0f;
			rigid->CheckShape = rigid->Shape;
			rigid->E = 0;
			rigid->SaveShape = rigid->Shape;
		}
	}
	else if (rigid->ChipType == 33 || rigid->ChipType == 34 || rigid->ChipType == 35) { //Frame
		if (value == 1) {
			rigid->Ghost = (int)value;
		}
	}
	else if (rigid->ChipType == 7) {	//Jet
		rigid->Option = value;
	}
	else if (rigid->ChipType == GT_CHIPH) {	//Weight
		if (value<1.0 || value>8.0) return;
		rigid->Option = value;
		rigid->Density = 1000.0*0.7f * 4 * value;
		rigid->Reset();
		rigid->FuelMax = rigid->Fuel = 6000000.0f*value;
	}
	else if (rigid->ChipType == 9) {	//Cowl
		if (value == 1) {
			rigid->FrameFlag = 1;
		}
		if (value == 2) {
			g_World->UpdateRigid(rigid, GTYPE_DISK, false, CHIPSIZE, CHIPSIZE / 6, CHIPSIZE);
			rigid->CheckShape = rigid->Shape;
			rigid->SaveShape = rigid->Shape;
		}
		if (value == 3) {
		}
		if (value == 4) {
		}
		if (value == 5) {
		}
	}
}
int readChildData(FILE *fp, int parentNo, int parentType, bool checkFlag)
{
	GFloat angle, spring, damper, torque, friction, value;
	angle = 0;spring = 1.0;damper = 0.5;torque = 0;friction = 0;
	GVector offA, offB, axis[3];
	int n, c, cno, cno2, type, cw, rn;
	int c2, c3;
	int dirCode = 0;
	char str[128], str2[128], str3[128];
	bool minus;
	GLink *link1 = NULL, *link2 = NULL;
	axis[1] = GVector(0, 1, 0);
	while (1) {
		c = getToken(fp, str);
		switch (str[0]) {
		case 'N':
			offA = GVector(0, 0, -CHIPSIZE / 2);
			offB = GVector(-0, 0, CHIPSIZE / 2);
			axis[0] = GVector(-1, 0, 0);
			axis[2] = GVector(0, 0, 1);
			rn = 2;
			c = getToken(fp, str);
			if (c != ':') return 100;//':' is necessary.
			c = getToken(fp, str);
			dirCode = 0x01;
			break;
		case 'E':
			offA = GVector(-CHIPSIZE / 2, 0, 0);
			offB = GVector(CHIPSIZE / 2, 0, 0);
			axis[0] = GVector(0, 0, 1);
			axis[2] = GVector(1, 0, 0);
			rn = 3;
			c = getToken(fp, str);
			if (c != ':') return 100;//':' is necessary.
			c = getToken(fp, str);
			dirCode = 0x02;
			break;
		case 'W':
			offA = GVector(CHIPSIZE / 2, 0, 0);
			offB = GVector(-CHIPSIZE / 2, 0, 0);
			axis[0] = GVector(0, 0, -1);
			axis[2] = GVector(-1, 0, 0);
			rn = 1;
			c = getToken(fp, str);
			if (c != ':') return 100;//':' is necessary.
			c = getToken(fp, str);
			dirCode = 0x04;
			break;
		case 'S':
			offA = GVector(0, 0, CHIPSIZE / 2);
			offB = GVector(0, 0, -CHIPSIZE / 2);
			axis[0] = GVector(1, 0, 0);
			axis[2] = GVector(0, 0, -1);
			rn = 0;
			c = getToken(fp, str);
			if (c != ':') return 100;//':' is necessary.
			c = getToken(fp, str);
			dirCode = 0x08;
			break;
		case '}':
			return 0;
			break;
		default:
			offA = GVector(0, 0, -CHIPSIZE / 2);
			offB = GVector(-0, 0, CHIPSIZE / 2);
			axis[0] = GVector(-1, 0, 0);
			axis[2] = GVector(0, 0, 1);
			rn = 2;
			dirCode = 0x00;
			break;
		}
		if (strcmp(str, "CORE") == 0) type = 'X';
		else if (strcmp(str, "CHIP") == 0) type = 'C';
		else if (strcmp(str, "WEIGHT") == 0) type = 'H';
		else if (strcmp(str, "COWL") == 0) type = 'O';
		else if (strcmp(str, "ARM") == 0) type = 'A';
		else if (strcmp(str, "FRAME") == 0) type = 'c';
		else if (strcmp(str, "RUDDER") == 0) type = 'R';
		else if (strcmp(str, "RUDDERF") == 0) type = 'r';
		else if (strcmp(str, "TRIM") == 0) type = 'T';
		else if (strcmp(str, "TRIMF") == 0) type = 't';
		else if (strcmp(str, "JET") == 0) type = 'J';
		else if (strcmp(str, "WHEEL") == 0) type = 'W';
		else if (strcmp(str, "RLW") == 0) type = 'N';
		else return 102;//The chip-type is necessary.
		c = getToken(fp, str);
		if (c != '(') return 103;//'(' is necessary.
		link1 = link2 = NULL;
		int an = 0;
		if (type == 'X') {
			if (!checkFlag) {
				MakeChip(GT_CORE, 0);
				cno = g_World->getChipCount();
				g_Chip[cno]->CheckShape = g_Chip[cno]->Shape;
				g_Chip[cno]->SaveShape = g_Chip[cno]->Shape;
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
			}
		}
		else if (type == 'C' || type == 'c' || type == 'H' || type == 'O' || type == 'R' || type == 'r' || type == 'T' || type == 't' || type == 'J' || type == 'A') {
			if (type == 'C') { if (!checkFlag)MakeChip(GT_CHIP, rn);an = 0; }
			else if (type == 'c') { if (!checkFlag)MakeChip(GT_CHIP2, rn);an = 0; }
			else if (type == 'H') { if (!checkFlag)MakeChip(GT_CHIPH, rn);an = 0; }
			else if (type == 'O') { if (!checkFlag)MakeChip(GT_COWL, rn);an = 0; }
			else if (type == 'R') { if (!checkFlag)MakeChip(GT_RUDDER, rn);an = 1; }
			else if (type == 'r') { if (!checkFlag)MakeChip(GT_RUDDER2, rn);an = 1; }
			else if (type == 'T') { if (!checkFlag)MakeChip(GT_TRIM, rn);an = 2; }
			else if (type == 't') { if (!checkFlag)MakeChip(GT_TRIM2, rn);an = 2; }
			else if (type == 'J') { if (!checkFlag)MakeChip(GT_JET, rn);an = 0; }
			else if (type == 'A') { if (!checkFlag)MakeChip(GT_ARM, rn);an = 0; }

			if (!checkFlag) {
				cno = g_World->getChipCount();
				g_Chip[parentNo]->DirCode |= dirCode;
				if (type == 'C' || type == 'c' || type == 'H' || type == 'O') {
					if (dirCode == 0x01) {
						g_Chip[cno]->CheckShape.PointN = 6;
						g_Chip[cno]->CheckShape.Point[0] = g_Chip[cno]->Shape.Point[1];
						g_Chip[cno]->CheckShape.Point[1] = g_Chip[cno]->Shape.Point[0];
						g_Chip[cno]->CheckShape.Point[2] = g_Chip[cno]->Shape.Point[2];
						g_Chip[cno]->CheckShape.Point[3] = g_Chip[cno]->Shape.Point[5];
						g_Chip[cno]->CheckShape.Point[4] = g_Chip[cno]->Shape.Point[6];
						g_Chip[cno]->CheckShape.Point[5] = g_Chip[cno]->Shape.Point[8];
					}
					else if (dirCode == 0x02) {
						g_Chip[cno]->CheckShape.PointN = 6;
						g_Chip[cno]->CheckShape.Point[0] = g_Chip[cno]->Shape.Point[2];
						g_Chip[cno]->CheckShape.Point[1] = g_Chip[cno]->Shape.Point[1];
						g_Chip[cno]->CheckShape.Point[2] = g_Chip[cno]->Shape.Point[3];
						g_Chip[cno]->CheckShape.Point[3] = g_Chip[cno]->Shape.Point[6];
						g_Chip[cno]->CheckShape.Point[4] = g_Chip[cno]->Shape.Point[7];
						g_Chip[cno]->CheckShape.Point[5] = g_Chip[cno]->Shape.Point[8];
					}
					else if (dirCode == 0x04) {
						g_Chip[cno]->CheckShape.PointN = 6;
						g_Chip[cno]->CheckShape.Point[0] = g_Chip[cno]->Shape.Point[0];
						g_Chip[cno]->CheckShape.Point[1] = g_Chip[cno]->Shape.Point[3];
						g_Chip[cno]->CheckShape.Point[2] = g_Chip[cno]->Shape.Point[1];
						g_Chip[cno]->CheckShape.Point[3] = g_Chip[cno]->Shape.Point[4];
						g_Chip[cno]->CheckShape.Point[4] = g_Chip[cno]->Shape.Point[5];
						g_Chip[cno]->CheckShape.Point[5] = g_Chip[cno]->Shape.Point[8];
					}
					else if (dirCode == 0x08) {
						g_Chip[cno]->CheckShape.PointN = 6;
						g_Chip[cno]->CheckShape.Point[0] = g_Chip[cno]->Shape.Point[3];
						g_Chip[cno]->CheckShape.Point[1] = g_Chip[cno]->Shape.Point[2];
						g_Chip[cno]->CheckShape.Point[2] = g_Chip[cno]->Shape.Point[0];
						g_Chip[cno]->CheckShape.Point[3] = g_Chip[cno]->Shape.Point[4];
						g_Chip[cno]->CheckShape.Point[4] = g_Chip[cno]->Shape.Point[7];
						g_Chip[cno]->CheckShape.Point[5] = g_Chip[cno]->Shape.Point[8];
					}
					g_Chip[cno]->SaveShape = g_Chip[cno]->CheckShape;
				}
				else {
					g_Chip[cno]->CheckShape = g_Chip[cno]->Shape;
					g_Chip[cno]->SaveShape = g_Chip[cno]->Shape;
				}
				if (type == 'O') link2 = g_World->AddCowl(g_Chip[parentNo], offA, g_Chip[g_World->getChipCount()], offB, axis[an], angle);
				else link2 = g_World->AddHinge(g_Chip[parentNo], offA, g_Chip[g_World->getChipCount()], offB, axis[an], angle, spring, damper);
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
			}
			else {
				if (parentType == 'O'&& type != 'O') return 104;//The caul cannot be connected with the caul.
			}
		}
		else if (type == 'W') {
			if (!checkFlag) {
				cno2 = g_World->getChipCount();
				MakeChip(GT_DUMMY, rn);
				g_Chip[cno2]->CheckShape = g_Chip[cno2]->Shape;
				g_Chip[cno2]->SaveShape = g_Chip[cno2]->Shape;

				link2 = g_World->AddHinge(g_Chip[parentNo], offA, g_Chip[cno2], offB, axis[0], angle, 1.0, 0.5);
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
				cno = g_World->getChipCount();
				MakeChip(GT_WHEEL, rn);
				g_Chip[cno]->CheckShape = g_Chip[cno]->Shape;
				g_Chip[cno]->SaveShape = g_Chip[cno]->Shape;
				g_Chip[parentNo]->DirCode |= dirCode;
				link1 = g_World->AddShaft(g_Chip[cno2], GVector(0, 0, 0), g_Chip[cno], GVector(0, 0, 0), axis[1], 0);
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
				an = 2;
			}
		}
		else if (type == 'N') {
			if (!checkFlag) {
				cno2 = g_World->getChipCount();
				MakeChip(GT_DUMMY, rn);
				g_Chip[cno2]->CheckShape = g_Chip[cno2]->Shape;
				g_Chip[cno2]->SaveShape = g_Chip[cno2]->Shape;
				link2 = g_World->AddHinge(g_Chip[parentNo], offA, g_Chip[cno2], offB, axis[0], angle, 1.0, 0.5);
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
				cno = g_World->getChipCount();
				MakeChip(GT_RLW, rn);
				g_Chip[cno]->CheckShape = g_Chip[cno]->Shape;
				g_Chip[cno]->SaveShape = g_Chip[cno]->Shape;
				g_Chip[parentNo]->DirCode |= dirCode;
				link1 = g_World->AddShaft(g_Chip[cno2], GVector(0, 0, 0), g_Chip[cno], GVector(0, 0, 0), axis[1], 0);
				g_World->IncreaseChipCount();//if (g_World->getChipCount() >= GCHIPMAX) g_World->getChipCount() = GCHIPMAX - 1;
				an = 2;
			}
		}
		do {
			c = getToken(fp, str);
			if (c == ')') break;
			c2 = getToken(fp, str2);if (c2 != '=') return 105;//'=' is necessary.
			c3 = getToken(fp, str3);

			minus = false;
			if (c3 == 2) {
				if (str3[0] == '-'&& str3[1] == '\0') {
					c3 = getToken(fp, str3);
					if (c3 != 1) return 1;
					if (!checkFlag) n = getValNo(str3);else n = 0;
					minus = true;
					if (n < 0) {
						c3 = 2;
						value = 0;
						minus = false;
					}
				}
				else sscanf(str3, "%g", &value);
			}
			else if (c3 == 1) {
				if (!checkFlag) n = getValNo(str3);else n = 0;
				minus = false;
				if (n < 0) {
					c3 = 2;
					value = 0;
				}
			}
			else return 106;//The numerical value or the variable is necessary. 
			if (!checkFlag) {
				if (strcmp("ANGLE", str) == 0 && link2) {
					if (c3 == 1) {
						link2->Angle = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(link2->Angle);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else link2->Angle = value;
				}
				else if (strcmp("SPRING", str) == 0 && link2) {
					if (c3 == 1) {
						link2->SpringK = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(link2->SpringK);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else link2->SpringK = value;
				}
				else if ((strcmp("DUMPER", str) == 0 || strcmp("DAMPER", str) == 0) && link2) {
					if (c3 == 1) {
						link2->DamperK = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(link2->DamperK);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else link2->DamperK = value;
				}
				else if (strcmp("POWER", str) == 0) {
					if (c3 == 1) {
						g_Chip[cno]->Power = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(g_Chip[cno]->Power);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else {
						g_Chip[cno]->Power = value;
						g_Chip[cno]->Power2 = value;
					}
				}
				else if ((strcmp("BREAK", str) == 0 || strcmp("BRAKE", str) == 0) && link1) {
					if (c3 == 1) {
						link1->FrictionK = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(link1->FrictionK);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else link1->FrictionK = value;
				}
				else if (strcmp("NAME", str) == 0) {
					lstrcpy(g_Chip[cno]->Name, str3);
					if (g_VarCount < GVALMAX) {
						lstrcpy(g_ValList[g_VarCount].Name, str3);
						g_ValList[g_VarCount].Def = (GFloat)cno;
						g_ValList[g_VarCount].Max = FLT_MAX;
						g_ValList[g_VarCount].Min = 0;
						g_ValList[g_VarCount].Dec = 0.0f;
						g_ValList[g_VarCount].visible = 0;
						g_ValList[g_VarCount].Label = 1;
						g_VarCount++;
					}
				}
				else if (strcmp("COLOR", str) == 0) {
					if (c3 == 1) {
						g_Chip[cno]->Color = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(g_Chip[cno]->Color);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}

					if (c3 == 2) {
						g_Chip[cno]->Color = value;
						int col = (int)value;
					}
				}
				else if (strcmp("EFFECT", str) == 0) {
					if (c3 == 1) {
						g_Chip[cno]->Effect = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(g_Chip[cno]->Effect);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else {
						g_Chip[cno]->Effect = value;
					}
				}
				else if (strcmp("OPTION", str) == 0) {
					if (c3 == 2) {
						setOption(g_Chip[cno], value);
					}
				}
				else if (strcmp("USER1", str) == 0) {
					if (c3 == 1) {
						g_Chip[cno]->UserEffect = g_ValList[n].Val;
						g_ValList[n].Ref[g_ValList[n].RefCount] = &(g_Chip[cno]->UserEffect);
						g_ValList[n].Flag[g_ValList[n].RefCount] = minus;
						g_ValList[n].RefCount++;
						if (g_ValList[n].RefCount >= GREFMAX) g_ValList[n].RefCount = GREFMAX - 1;
					}
					else {
						g_Chip[cno]->UserEffect = value;
					}
				}
				else if (strcmp("USER2", str) == 0) {
					if (c3 == 2) {
						g_Chip[cno]->UserOption = value;
					}
				}
				else return 107;// It is a key-word doesn't know
			}
			c = getToken(fp, str);
		} while (c == ',');
		if (c != ')') return 108;//')' is necessary.
		c = getToken(fp, str);
		if (c == '{') {
			if (!checkFlag) cw = readChildData(fp, cno, type, checkFlag);
			else cw = readChildData(fp, 0, type, checkFlag);
			if (cw != 0) return cw;
		}
	}
	return 0;
}


int  readData(char *name, bool checkFlag)
{
	FILE *fp;
	int haveArm = g_MyPlayerData.haveArm;
	g_MyPlayerData.haveArm = 0;
	g_World->haveArm = 0;
	DataCheck = 0;
	if ((fp = fopen(name, "r")) == NULL) return 1;//The file doesn't open it. 
	if (!checkFlag && g_ScriptLua) {
		luaScriptEnd(g_ScriptLua);
		g_ScriptLua = NULL;
	}
	int a = readData2(fp, checkFlag);
	fclose(fp);
	if (checkFlag) {
		g_MyPlayerData.haveArm = haveArm;
		g_World->haveArm = haveArm;
	}

	return a;
}
int  readData2(FILE *fp, bool checkFlag)
{
	char str[1024];
	char str2[1024];
	char str3[1024];
	DataCheck = 1;
	int i, cw, cw2, cw3, n, vn;
	GetTokenCh();
	if (!checkFlag) {
		g_World->DeleteRigids();
		for (i = 0;i < GKEYMAX;i++) g_KeyList[i].Count = 0;
		for (i = 0;i < GVALMAX;i++) g_ValList[i].RefCount = 0;
		if (g_ScriptLua) luaScriptEnd(g_ScriptLua);
		g_ScriptLua = NULL;
		//g_World->getChipCount() = 0;
		g_VarCount = 0;
	}
	cw = getToken(fp, str);
	if (strcmp("VAL", str) == 0 || strcmp("VAR", str) == 0) {
		cw = getToken(fp, str);
		if (cw != '{') { return 2; } // '{' is necessary
		cw = getToken(fp, str);
		while (cw != '}') {
			if (!checkFlag) {
				if (g_VarCount >= GVALMAX) { return 3; }	//There are a lot of variables. 
				if (cw == 1) {
					lstrcpy(g_ValList[g_VarCount].Name, str);
					g_ValList[g_VarCount].Def = 0.0f;
					g_ValList[g_VarCount].Max = FLT_MAX;
					g_ValList[g_VarCount].Min = 0;
					g_ValList[g_VarCount].Dec = 0.0f;
					g_ValList[g_VarCount].visible = 1;
					g_ValList[g_VarCount].Label = 0;
					g_ValList[g_VarCount].RefCount = 0;
					g_ValList[g_VarCount].Val = 0.0f;
				}
				else { return 4; }	// '}' is necessary
			}
			cw = getToken(fp, str);
			if (cw != '(') { return 5; }	// '(' is necessary
			cw = getToken(fp, str);
			while (cw != ')') {
				if (cw == ',') cw = getToken(fp, str);
				cw2 = getToken(fp, str2);
				cw3 = getToken(fp, str3);
				if (strcmp("DEFAULT", str) == 0) {
					if (!checkFlag)sscanf(str3, "%g", &(g_ValList[g_VarCount].Def));
				}
				else if (strcmp("MIN", str) == 0) {
					if (!checkFlag)sscanf(str3, "%g", &(g_ValList[g_VarCount].Min));
				}
				else if (strcmp("MAX", str) == 0) {
					if (!checkFlag)sscanf(str3, "%g", &(g_ValList[g_VarCount].Max));
				}
				else if (strcmp("STEP", str) == 0) {
					if (!checkFlag)sscanf(str3, "%g", &(g_ValList[g_VarCount].Dec));
				}
				else if (strcmp("DISP", str) == 0) {
					if (!checkFlag)sscanf(str3, "%d", &(g_ValList[g_VarCount].visible));
				}
				else { return 6; } // It is a key-word doesn't know.
				cw = getToken(fp, str);
			}
			if (g_ValList[g_VarCount].Def >= g_ValList[g_VarCount].Max) g_ValList[g_VarCount].Def = g_ValList[g_VarCount].Max;
			if (g_ValList[g_VarCount].Def <= g_ValList[g_VarCount].Min) g_ValList[g_VarCount].Def = g_ValList[g_VarCount].Min;
			//			g_ValList[g_VarCount].Val=g_ValList[g_VarCount].Def;
			if (!checkFlag) g_VarCount++;
			cw = getToken(fp, str);
		}
		cw = getToken(fp, str);
	}
	else {
		return 7; //There is no 'VAL' block.
	}
	if (strcmp("KEY", str) == 0) {
		cw = getToken(fp, str);
		if (cw != '{') { return 2; }// '{' is necessary
		cw = getToken(fp, str);
		while (cw != '}') {
			if (cw != ',') {
				sscanf(str, "%d", &n);
				if (n < 0 || n >= GKEYMAX) { return 8; }	//There are a lot of keys. 
				cw = getToken(fp, str);
				if (cw != ':') { return 8; }	// ':' is necessary
			}
			cw = getToken(fp, str);
			if (!checkFlag) {
				vn = getValNo(str);
				if (g_KeyList[n].Count >= GVALMAX) { return 9; }//The variable doesn't exist. 
				g_KeyList[n].ValNo[g_KeyList[n].Count] = vn;
			}
			cw = getToken(fp, str);
			if (cw != '(') { return 5; }	// '(' is necessary
			cw = getToken(fp, str);
			while (cw != ')') {
				if (cw == ',') cw = getToken(fp, str);
				cw2 = getToken(fp, str2);
				cw3 = getToken(fp, str3);
				GFloat v = 0;
				bool   f = false;
				if (strcmp("STEP", str) == 0) {
					if (!checkFlag) {
						sscanf(str3, "%g", &v);
						g_KeyList[n].Step[g_KeyList[n].Count] = v;
					}
				}
				else if (strcmp("VALUE", str) == 0) { if (!checkFlag)sscanf(str3, "%g", &v); }
				else return 11;
				cw = getToken(fp, str);
			}
			if (!checkFlag)g_KeyList[n].Count++;
			cw = getToken(fp, str);
		}
		cw = getToken(fp, str);
	}
	else {
		return 10; //There is no 'KEY' block.
	}
	if (strcmp("BODY", str) == 0) {
		cw = getToken(fp, str);
		int err = 0;
		if (cw == '{') {
			err = readChildData(fp, 0, 0, checkFlag);
			if (err) { return err; }
			for (int i = 0;i < g_World->getChipCount();i++) {
				if (g_Chip[i]->ChipType == GT_CORE) g_Chip[i]->Reset();
			}
		}
		cw = getToken(fp, str);

	}
	else {
		return 12;//There is no 'BODY' block.
	}
	if (!checkFlag) {
		if (ScriptSource) delete ScriptSource;
		ScriptSource = new char[200000];
		ScriptSource[0] = '\0';
		int b = 1;
		//resetToken();
		if (strcmp("SCRIPT", str) == 0) {
			ScriptType = 0;
			if (GetTokenCh() != '{') searchData(fp, '{');
			while (1) {
				cw = getToken2(fp, str);
				if (cw == EOF) break;
				if (strncmp(str, "MODELEND", 8) == 0) break;
				if (str[0] == '{') b++;
				else if (str[0] == '}') {
					b--;
					if (b == 0) break;
				}
				strcat(ScriptSource, str);
			}
			if (cw != EOF) cw = getToken(fp, str);
		}
		if (strcmp("LUA", str) == 0) {
			char *e;
			if (GetTokenCh() != '{') searchData(fp, '{');
			ScriptSource[0] = '\0';
			ScriptType = 1;
			e = fgets(str, 1024, fp);;
			strcat(ScriptSource, str);
			while (e != NULL) {
				e = fgets(str, 1024, fp);
				if (e == NULL) break;
				if (strncmp(str, "MODELEND", 8) == 0) break;
				strcat(ScriptSource, str);
			}
			//最後の}は外す
			for (int k = (int)strlen(ScriptSource) - 1;k >= 0;k--) {
				if (ScriptSource[k] == '}') {
					ScriptSource[k] = '\0';
					break;
				}
			}
			g_ScriptLua = luaScriptInit(ScriptSource);

			if (g_ScriptLua == NULL) {
				ScriptSource[0] = '\0';
			}
		}
	}
	return 0;

}
char *checkVal(GFloat *ref, char *tmp) {
	for (int i = 0;i < g_VarCount;i++) {
		for (int j = 0;j < g_ValList[i].RefCount;j++) {
			if (ref == g_ValList[i].Ref[j]) {
				if (g_ValList[i].Flag[j]) lstrcpy(tmp, TEXT("-"));
				else tmp[0] = '\0';
				strcat(tmp, g_ValList[i].Name);
				return tmp;
			}
		}
	}
	return NULL;
}
int saveChips(FILE *fp, GRigid *rigid, int level)
{
	char tmp[128];
	if (rigid->ChipType != GT_WHEEL && rigid->ChipType != GT_RLW
		&& rigid->ChipType != GT_WHEEL2 && rigid->ChipType != GT_RLW2) {
		for (int i = 0;i < level;i++) fprintf(fp, "\t");
	}
	int c = ' ';
	switch (rigid->Dir) {
	case 2:c = 'N';break;
	case 3:c = 'E';break;
	case 1:c = 'W';break;
	case 0:c = 'S';break;
	}
	switch (rigid->ChipType) {
	case GT_CORE:
		fprintf(fp, "Core");break;
	case GT_CHIP:
		fprintf(fp, "%c:g_Chip", c);break;
	case GT_RUDDER:
		fprintf(fp, "%c:Rudder", c);break;
	case GT_DUMMY:
		fprintf(fp, "%c:", c);
		break;
	case GT_WHEEL:
		fprintf(fp, "Wheel");break;
	case GT_RLW:
		fprintf(fp, "RLW");break;
	case GT_TRIM:
		fprintf(fp, "%c:Trim", c);break;
	case GT_JET:
		fprintf(fp, "%c:Jet", c);break;
	case GT_CHIPH:
		fprintf(fp, "%c:Weight", c);break;
	case GT_COWL:
		fprintf(fp, "%c:Cowl", c);break;
	case GT_ARM:
		fprintf(fp, "%c:Arm", c);break;
	case GT_CHIP2:
		fprintf(fp, "%c:Frame", c);break;
	case GT_RUDDER2:
		fprintf(fp, "%c:RudderF", c);break;
	case GT_TRIM2:
		fprintf(fp, "%c:TrimF", c);break;
	case GT_WHEEL2:
		fprintf(fp, "%c:WheelF", c);break;
	case GT_RLW2:
		fprintf(fp, "%c:RLWF", c);break;
	}
	int camma = 0;
	char *vname;
	if (rigid->ChipType != GT_DUMMY) {
		if (rigid->ChipType == GT_CORE) {
			fprintf(fp, "() {");
		}
		else if (rigid->ChipType == GT_WHEEL || rigid->ChipType == GT_RLW) {
			fprintf(fp, "(");
			vname = checkVal(&rigid->Parent->LinkInfo->Angle, tmp);
			if (rigid->Parent->LinkInfo->Angle != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "angle=%g", rigid->Parent->LinkInfo->Angle);
				else fprintf(fp, "angle=%s", vname);
			}
			vname = checkVal(&rigid->Power, tmp);
			if (rigid->Power != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "power=%g", rigid->Power);
				else fprintf(fp, "power=%s", vname);
			}
			vname = checkVal(&rigid->LinkInfo->FrictionK, tmp);
			if (rigid->LinkInfo->FrictionK != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "brake=%g", rigid->LinkInfo->FrictionK);
				else fprintf(fp, "brake=%s", vname);
			}
			vname = checkVal(&rigid->Parent->LinkInfo->SpringK, tmp);
			if (rigid->Parent->LinkInfo->SpringK != 1 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "spring=%g", rigid->Parent->LinkInfo->SpringK);
				else fprintf(fp, "spring=%s", vname);
			}
			vname = checkVal(&rigid->Parent->LinkInfo->DamperK, tmp);
			if (rigid->Parent->LinkInfo->DamperK != 0.5 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "dumper=%g", rigid->Parent->LinkInfo->DamperK);
				else fprintf(fp, "dumper=%s", vname);
			}
			vname = checkVal(&rigid->Color, tmp);
			if (rigid->Color != 0xffffff || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "color=#%06X", (int)rigid->Color);
				else fprintf(fp, "color=%s", vname);
			}
			if (rigid->Option != 0) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				fprintf(fp, "option=%g", rigid->Option);
			}
			vname = checkVal(&rigid->Effect, tmp);
			if (rigid->Effect != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "effect=%g", rigid->Effect);
				else fprintf(fp, "effect=%s", vname);
			}
			if (rigid->Name[0] != '\0') {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				fprintf(fp, "name=%s", rigid->Name);
			}
			fprintf(fp, ") {");
		}
		else {
			fprintf(fp, "(");
			vname = checkVal(&rigid->LinkInfo->Angle, tmp);
			if (rigid->LinkInfo->Angle != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "angle=%g", rigid->LinkInfo->Angle);
				else fprintf(fp, "angle=%s", vname);
			}
			vname = checkVal(&rigid->Power, tmp);
			if (rigid->Power != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "power=%g", rigid->Power);
				else fprintf(fp, "power=%s", vname);
			}
			vname = checkVal(&rigid->LinkInfo->SpringK, tmp);
			if (rigid->LinkInfo->SpringK != 1 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "spring=%g", rigid->LinkInfo->SpringK);
				else fprintf(fp, "spring=%s", vname);
			}
			vname = checkVal(&rigid->LinkInfo->DamperK, tmp);
			if (rigid->LinkInfo->DamperK != 0.5 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "dumper=%g", rigid->LinkInfo->DamperK);
				else fprintf(fp, "dumper=%s", vname);
			}
			vname = checkVal(&rigid->Color, tmp);
			if (rigid->Color != 0xffffff || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "color=#%06X", (int)rigid->Color);
				else fprintf(fp, "color=%s", vname);
			}
			if (rigid->Option != 0) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				fprintf(fp, "option=%g", rigid->Option);
			}
			vname = checkVal(&rigid->Effect, tmp);
			if (rigid->Effect != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "effect=%g", rigid->Effect);
				else fprintf(fp, "effect=%s", vname);
			}
			if (rigid->Name[0] != '\0') {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				fprintf(fp, "name=%s", rigid->Name);
			}
			vname = checkVal(&rigid->UserEffect, tmp);
			if (rigid->UserEffect != 0 || vname != NULL) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				if (vname == NULL) fprintf(fp, "user1=%g", rigid->UserEffect);
				else fprintf(fp, "user1=%s", vname);
			}
			if (rigid->UserOption != 0) {
				if (camma == 0) camma = 1;else fprintf(fp, ",");
				fprintf(fp, "user2=%g", rigid->UserOption);
			}

			fprintf(fp, ") {");
		}
	}
	if (rigid->ChipType != GT_DUMMY) fprintf(fp, "\n");
	if (rigid->ChildCount != 0) {
		for (int i = 0;i < rigid->ChildCount;i++) {
			if (rigid->ChipType != GT_WHEEL && rigid->ChipType != GT_RLW
				&& rigid->ChipType != GT_WHEEL2 && rigid->ChipType != GT_RLW2)
				saveChips(fp, rigid->Child[i].RigidB, level + 1);
			else saveChips(fp, rigid->Child[i].RigidB, level);
		}
	}
	if (rigid->ChipType != GT_WHEEL && rigid->ChipType != GT_RLW
		&& rigid->ChipType != GT_WHEEL2 && rigid->ChipType != GT_RLW2) {
		for (int i = 0;i < level;i++) fprintf(fp, "\t");
		fprintf(fp, "}\n");
	}
	return 0;
}
int saveData(char *fname)
{
	FILE *fp;
	if ((fp = fopen(fname, "w")) != NULL) {
		_SYSTEMTIME time;
		GetLocalTime(&time);
		fprintf(fp, "// RigidChips\n");
		fprintf(fp, "// %d/%d/%d %d:%d:%d\n", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond);
		fprintf(fp, "Val {\n");
		for (int i = 0;i < g_VarCount;i++) {
			if (g_ValList[i].Label == 0) {
				fprintf(fp, "\t%s(default=%g, step=%g"
					, g_ValList[i].Name, g_ValList[i].Def, g_ValList[i].Dec);
				if (g_ValList[i].Max < FLT_MAX) fprintf(fp, ", max=%g", g_ValList[i].Max);
				if (g_ValList[i].Min != 0) fprintf(fp, ", min=%g", g_ValList[i].Min);
				if (g_ValList[i].visible == false) fprintf(fp, ", disp=0)\n");
				else fprintf(fp, ")\n");
			}
		}
		fprintf(fp, "}\n");
		fprintf(fp, "Key {\n");
		for (int i = 0;i < GKEYMAX;i++) {
			if (g_KeyList[i].Count > 0) {
				fprintf(fp, "\t%d:", i);
				for (int j = 0;j < g_KeyList[i].Count;j++) {
					if (j > 0) fprintf(fp, ", ");
					fprintf(fp, "%s(step=%g)", g_ValList[g_KeyList[i].ValNo[j]].Name, g_KeyList[i].Step[j]);
				}
				fprintf(fp, "\n");
			}
		}
		fprintf(fp, "}\n");
		fprintf(fp, "Body {\n");
		for (int i = 0;i < g_World->getChipCount();i++) {
			if (g_Chip[i]->Parent == NULL) saveChips(fp, g_Chip[i], 1);
		}
		fprintf(fp, "}\n");
		fclose(fp);
	}
	return 0;
}

