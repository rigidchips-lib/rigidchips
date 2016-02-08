#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
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


extern int dataCode;
extern GValList ValList[];
extern GKeyList KeyList[];
extern GWorld *World;
extern GRigid *Chip[];
extern int DataCheck;
extern int ChipCount;
extern int VarCount;
extern char *ScriptSource;
extern int ScriptType;
extern int ScriptPc;
extern GMYDATA MyPlayerData;



lua_State *ScriptL=NULL;

void MakeChip(int type,int rn)
{
	if(type==GT_CORE) {  //Core
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=0;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.2f;
		Chip[ChipCount]->Cd=GDEFCD;
		Chip[ChipCount]->X=GVector(0,5,0);
		Chip[ChipCount]->R=GMatrix();
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=2000000.0f;
		rn=0;
	}
	else if(type==GT_CHIP) { //Chip
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=1;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.2f;
		Chip[ChipCount]->Cd=GDEFCD;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=1000000.0f;
	}
	else if(type==GT_CHIPH) { //ChipH
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=21;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f*4;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.1f;
		Chip[ChipCount]->Us=2.2f;
		Chip[ChipCount]->Ud=1.0f;
		Chip[ChipCount]->Cd=GDEFCD;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=6000000.0f;
	}
	else if(type==GT_COWL) { //cowl
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=23;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=0.00f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.0;
		Chip[ChipCount]->Ud=0.0f;
		Chip[ChipCount]->Cd=0;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
		Chip[ChipCount]->Effect=0x0000fb;

	}
	else if(type==GT_CHIP2) { //Frame
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/12,CHIPSIZE);
		Chip[ChipCount]->Shape.PointN--;
		Chip[ChipCount]->MeshNo=7;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=1;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.1f;
		Chip[ChipCount]->Us=0.8f;
		Chip[ChipCount]->Ud=0.6f;
		Chip[ChipCount]->Cd=0.0f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=500000.0f;
	}
	else if(type==GT_RUDDER) { //Rudder
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=4;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.2f;
		Chip[ChipCount]->Cd=GDEFCD;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=1000000.0f;
	}
	else if(type==GT_RUDDER2) { //Rudder2
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/12,CHIPSIZE);
		Chip[ChipCount]->Shape.PointN--;
		Chip[ChipCount]->MeshNo=16;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=1;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.1f;
		Chip[ChipCount]->Us=0.8f;
		Chip[ChipCount]->Ud=0.6f;
		Chip[ChipCount]->Cd=0.0f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=500000.0f;
	}
	else if(type==GT_TRIM) { //Trim
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=5;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.2f;
		Chip[ChipCount]->Cd=GDEFCD;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=1000000.0f;
	}
	else if(type==GT_TRIM2) { //Trim2
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/12,CHIPSIZE);
		Chip[ChipCount]->Shape.PointN--;
		Chip[ChipCount]->MeshNo=17;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=1;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.1f;
		Chip[ChipCount]->Us=0.8f;
		Chip[ChipCount]->Ud=0.6f;
		Chip[ChipCount]->Cd=0.0f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=500000.0f;
	}
	else if(type==GT_DUMMY) { //Dummy
		Chip[ChipCount]=World->AddRigid(GTYPE_DISK,false,CHIPSIZE/2,CHIPSIZE,CHIPSIZE/2);
		Chip[ChipCount]->MeshNo=6;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*1.0f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.2f;
		Chip[ChipCount]->Cd=0.0f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
	}
	else if(type==GT_WHEEL) { //Wheel
		Chip[ChipCount]=World->AddRigid(GTYPE_DISK,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=2;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*1.3f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.01f;
		Chip[ChipCount]->Us=1.5f;
		Chip[ChipCount]->Ud=0.9f;
		Chip[ChipCount]->Cd=0.05f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
	}
	else if(type==GT_RLW) { //RLW
		Chip[ChipCount]=World->AddRigid(GTYPE_DISK,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=3;//無反動ホイール
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*1.3f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.01f;
		Chip[ChipCount]->Us=1.5f;
		Chip[ChipCount]->Ud=0.9f;
		Chip[ChipCount]->Cd=0.05f;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
	}
	else if(type==GT_JET) { //Jet
		Chip[ChipCount]=World->AddRigid(GTYPE_DISK,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=10;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*1.3f;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.3f;
		Chip[ChipCount]->Ud=0.1f;
		Chip[ChipCount]->Cd=0.000;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
	}
	else if(type==GT_ARM) { //Arm
		Chip[ChipCount]=World->AddRigid(GTYPE_FACE,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
		Chip[ChipCount]->MeshNo=30;
		Chip[ChipCount]->ChipType=type;
		Chip[ChipCount]->FrameFlag=0;
		Chip[ChipCount]->Density=Chip[ChipCount]->Density*0.7f*2;
		Chip[ChipCount]->Reset();
		Chip[ChipCount]->E=0.2f;
		Chip[ChipCount]->Us=0.9f;
		Chip[ChipCount]->Ud=0.6f;
		Chip[ChipCount]->Cd=GDEFCD*0.7f;
		MyPlayerData.haveArm++;
		World->haveArm++;
		Chip[ChipCount]->FuelMax=Chip[ChipCount]->Fuel=0.0f;
	}
	Chip[ChipCount]->SetTensor();
	Chip[ChipCount]->Dir=rn;
}

int getValNo(char *name)
{
	int vn=0;
	while(vn<VarCount && strcmp(ValList[vn].Name,name)!=0) vn++;
	if(vn>=VarCount) vn=-1;
	return vn;
	
}
int getValNo2(char *name,bool *minusFlag)
{
	int vn=0;
	if(name[0]=='-') {
		*minusFlag=true;
		name++;
	}
	else *minusFlag=false;
	while(vn<VarCount && strcmp(ValList[vn].Name,name)!=0) vn++;
	if(vn>=VarCount) vn=-1;
	return vn;
	
}

void setOption(GRigid *rigid,GFloat value)
{
	rigid->Option=value;
	if(rigid->ChipType==10) {	//ARM
		rigid->ArmEnergy=value;
	}
	else if(rigid->ChipType==4 || rigid->ChipType==5) { //Wheel or RLW
		if(value==1) {
			World->UpdateRigid(rigid,GTYPE_DISK,false,CHIPSIZE*1.5f,CHIPSIZE/(6*1.5f*1.5f),CHIPSIZE*1.5f);
			rigid->Ud=0.9f;
			rigid->Us=1.0f;
			rigid->E=0;
			rigid->CheckShape=rigid->Shape;
			rigid->SaveShape=rigid->Shape;
		}
		else if(value==2) {
			World->UpdateRigid(rigid,GTYPE_DISK,false,CHIPSIZE*2,CHIPSIZE/24.0f,CHIPSIZE*2);
			rigid->Ud=0.9f;
			rigid->Us=1.0f;
			rigid->CheckShape=rigid->Shape;
			rigid->E=0;
			rigid->SaveShape=rigid->Shape;
		}
		else {
			World->UpdateRigid(rigid,GTYPE_DISK,false,CHIPSIZE,CHIPSIZE/6.0f,CHIPSIZE);
			rigid->Ud=0.9f;
			rigid->Us=1.0f;
			rigid->CheckShape=rigid->Shape;
			rigid->E=0;
			rigid->SaveShape=rigid->Shape;
		}
	}
	else if(rigid->ChipType==33 || rigid->ChipType==34 || rigid->ChipType==35) { //Frame
		if(value==1) {
			rigid->Ghost=(int)value;
		}
	}
	else if(rigid->ChipType==7) {	//Jet
		rigid->Option=value;
	}
	else if(rigid->ChipType==GT_CHIPH) {	//Weight
		if(value<1.0 || value>8.0) return;
		rigid->Option=value;
		rigid->Density=1000.0*0.7f*4*value;
		rigid->Reset();
		rigid->FuelMax=rigid->Fuel=6000000.0f*value;
	}
	else if(rigid->ChipType==9) {	//Cowl
		if(value==1) {
			rigid->FrameFlag=1;
		}
		if(value==2) {
			World->UpdateRigid(rigid,GTYPE_DISK,false,CHIPSIZE,CHIPSIZE/6,CHIPSIZE);
			rigid->CheckShape=rigid->Shape;
			rigid->SaveShape=rigid->Shape;
		}
		if(value==3) {
		}
		if(value==4) {
		}
		if(value==5) {
		}
	}
}
int readChildData(FILE *fp,int parentNo,int parentType,bool checkFlag)
{
	GFloat angle,spring,damper,torque,friction,value;
	angle=0;spring=1.0;damper=0.5;torque=0;friction=0;
	GVector offA,offB,axis[3];
	int n,c,cno,cno2,type,cw,rn;
	int c2,c3;
	int dirCode=0;
	char str[128],str2[128],str3[128];
	bool minus;
	GLink *link1=NULL,*link2=NULL;
	axis[1]=GVector(0,1,0);
	while(1) {
		c=getToken(fp,str);
		switch(str[0]) {
		case 'N':
			offA=GVector(0,0,-CHIPSIZE/2);
			offB=GVector(-0,0,CHIPSIZE/2);
			axis[0]=GVector(-1,0,0);
			axis[2]=GVector(0,0,1);
			rn=2;
			c=getToken(fp,str);
			if(c!=':') return 100;//':' is necessary.
			c=getToken(fp,str);
			dirCode=0x01;
			break;
		case 'E':
			offA=GVector(-CHIPSIZE/2,0,0);
			offB=GVector(CHIPSIZE/2,0,0);
			axis[0]=GVector(0,0,1);
			axis[2]=GVector(1,0,0);
			rn=3;
			c=getToken(fp,str);
			if(c!=':') return 100;//':' is necessary.
			c=getToken(fp,str);
			dirCode=0x02;
			break;
		case 'W':
			offA=GVector(CHIPSIZE/2,0,0);
			offB=GVector(-CHIPSIZE/2,0,0);
			axis[0]=GVector(0,0,-1);
			axis[2]=GVector(-1,0,0);
			rn=1;
			c=getToken(fp,str);
			if(c!=':') return 100;//':' is necessary.
			c=getToken(fp,str);
			dirCode=0x04;
			break;
		case 'S':
			offA=GVector(0,0,CHIPSIZE/2);
			offB=GVector(0,0,-CHIPSIZE/2);
			axis[0]=GVector(1,0,0);
			axis[2]=GVector(0,0,-1);
			rn=0;
			c=getToken(fp,str);
			if(c!=':') return 100;//':' is necessary.
			c=getToken(fp,str);
			dirCode=0x08;
			break;
		case '}':
			return 0;
			break;
		default:
			offA=GVector(0,0,-CHIPSIZE/2);
			offB=GVector(-0,0,CHIPSIZE/2);
			axis[0]=GVector(-1,0,0);
			axis[2]=GVector(0,0,1);
			rn=2;
			dirCode=0x00;
			break;
		}
		if(strcmp(str,"CORE")==0) type='X';
		else if(strcmp(str,"CHIP")==0) type='C';
		else if(strcmp(str,"WEIGHT")==0) type='H';
		else if(strcmp(str,"COWL")==0) type='O';
		else if(strcmp(str,"ARM")==0) type='A';
		else if(strcmp(str,"FRAME")==0) type='c';
		else if(strcmp(str,"RUDDER")==0) type='R';
		else if(strcmp(str,"RUDDERF")==0) type='r';
		else if(strcmp(str,"TRIM")==0) type='T';
		else if(strcmp(str,"TRIMF")==0) type='t';
		else if(strcmp(str,"JET")==0) type='J';
		else if(strcmp(str,"WHEEL")==0) type='W';
		else if(strcmp(str,"RLW")==0) type='N';
		else return 102;//The chip-type is necessary.
		c=getToken(fp,str);
		if(c!='(') return 103;//'(' is necessary.
		link1=link2=NULL;
		int an=0;
		if(type=='X') {
			if(!checkFlag) {
				MakeChip(GT_CORE,0);
				cno=ChipCount;
				Chip[cno]->CheckShape=Chip[cno]->Shape;
				Chip[cno]->SaveShape=Chip[cno]->Shape;
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
			}
		}
		else if(type=='C' || type=='c' || type=='H'  || type=='O' || type=='R' || type=='r' || type=='T' || type=='t' || type=='J' || type=='A') {
			if(type=='C') {if(!checkFlag)MakeChip(GT_CHIP,rn);an=0;}
			else if(type=='c') {if(!checkFlag)MakeChip(GT_CHIP2,rn);an=0;}
			else if(type=='H') {if(!checkFlag)MakeChip(GT_CHIPH,rn);an=0;}
			else if(type=='O') {if(!checkFlag)MakeChip(GT_COWL,rn);an=0;}
			else if(type=='R') {if(!checkFlag)MakeChip(GT_RUDDER,rn);an=1;}
			else if(type=='r') {if(!checkFlag)MakeChip(GT_RUDDER2,rn);an=1;}
			else if(type=='T') {if(!checkFlag)MakeChip(GT_TRIM,rn);an=2;}
			else if(type=='t') {if(!checkFlag)MakeChip(GT_TRIM2,rn);an=2;}
			else if(type=='J') {if(!checkFlag)MakeChip(GT_JET,rn);an=0;}
			else if(type=='A') {if(!checkFlag)MakeChip(GT_ARM,rn);an=0;}
			
			if(!checkFlag){
				cno=ChipCount;
				Chip[parentNo]->DirCode|=dirCode;
				if(type=='C' || type=='c'|| type=='H'|| type=='O') {
					if(dirCode==0x01) {
						Chip[cno]->CheckShape.PointN=6;
						Chip[cno]->CheckShape.Point[0]=Chip[cno]->Shape.Point[1];
						Chip[cno]->CheckShape.Point[1]=Chip[cno]->Shape.Point[0];
						Chip[cno]->CheckShape.Point[2]=Chip[cno]->Shape.Point[2];
						Chip[cno]->CheckShape.Point[3]=Chip[cno]->Shape.Point[5];
						Chip[cno]->CheckShape.Point[4]=Chip[cno]->Shape.Point[6];
						Chip[cno]->CheckShape.Point[5]=Chip[cno]->Shape.Point[8];
					}
					else if(dirCode==0x02) {
						Chip[cno]->CheckShape.PointN=6;
						Chip[cno]->CheckShape.Point[0]=Chip[cno]->Shape.Point[2];
						Chip[cno]->CheckShape.Point[1]=Chip[cno]->Shape.Point[1];
						Chip[cno]->CheckShape.Point[2]=Chip[cno]->Shape.Point[3];
						Chip[cno]->CheckShape.Point[3]=Chip[cno]->Shape.Point[6];
						Chip[cno]->CheckShape.Point[4]=Chip[cno]->Shape.Point[7];
						Chip[cno]->CheckShape.Point[5]=Chip[cno]->Shape.Point[8];
					}
					else if(dirCode==0x04) {
						Chip[cno]->CheckShape.PointN=6;
						Chip[cno]->CheckShape.Point[0]=Chip[cno]->Shape.Point[0];
						Chip[cno]->CheckShape.Point[1]=Chip[cno]->Shape.Point[3];
						Chip[cno]->CheckShape.Point[2]=Chip[cno]->Shape.Point[1];
						Chip[cno]->CheckShape.Point[3]=Chip[cno]->Shape.Point[4];
						Chip[cno]->CheckShape.Point[4]=Chip[cno]->Shape.Point[5];
						Chip[cno]->CheckShape.Point[5]=Chip[cno]->Shape.Point[8];
					}
					else if(dirCode==0x08) {
						Chip[cno]->CheckShape.PointN=6;
						Chip[cno]->CheckShape.Point[0]=Chip[cno]->Shape.Point[3];
						Chip[cno]->CheckShape.Point[1]=Chip[cno]->Shape.Point[2];
						Chip[cno]->CheckShape.Point[2]=Chip[cno]->Shape.Point[0];
						Chip[cno]->CheckShape.Point[3]=Chip[cno]->Shape.Point[4];
						Chip[cno]->CheckShape.Point[4]=Chip[cno]->Shape.Point[7];
						Chip[cno]->CheckShape.Point[5]=Chip[cno]->Shape.Point[8];
					}
					Chip[cno]->SaveShape=Chip[cno]->CheckShape;
				}
				else {
					Chip[cno]->CheckShape=Chip[cno]->Shape;
					Chip[cno]->SaveShape=Chip[cno]->Shape;
				}
				if(type=='O') link2=World->AddCowl(Chip[parentNo],offA,Chip[ChipCount],offB,axis[an],angle);
				else link2=World->AddHinge(Chip[parentNo],offA,Chip[ChipCount],offB,axis[an],angle,spring,damper);
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
			}
			else {
				if(parentType=='O'&& type!='O') return 104;//The caul cannot be connected with the caul.
			}
		}
		else if(type=='W') {
			if(!checkFlag) {
				cno2=ChipCount;
				MakeChip(GT_DUMMY,rn);
				Chip[cno2]->CheckShape=Chip[cno2]->Shape;
				Chip[cno2]->SaveShape=Chip[cno2]->Shape;

				link2=World->AddHinge(Chip[parentNo],offA,Chip[cno2],offB,axis[0],angle,1.0,0.5);
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
				cno=ChipCount;
				MakeChip(GT_WHEEL,rn);
				Chip[cno]->CheckShape=Chip[cno]->Shape;
				Chip[cno]->SaveShape=Chip[cno]->Shape;
				Chip[parentNo]->DirCode|=dirCode;
				link1=World->AddShaft(Chip[cno2],GVector(0,0,0),Chip[cno],GVector(0,0,0),axis[1],0);
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
				an=2;
			}
		}
		else if(type=='N') {
			if(!checkFlag) {
				cno2=ChipCount;
				MakeChip(GT_DUMMY,rn);
				Chip[cno2]->CheckShape=Chip[cno2]->Shape;
				Chip[cno2]->SaveShape=Chip[cno2]->Shape;
				link2=World->AddHinge(Chip[parentNo],offA,Chip[cno2],offB,axis[0],angle,1.0,0.5);
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
				cno=ChipCount;
				MakeChip(GT_RLW,rn);
				Chip[cno]->CheckShape=Chip[cno]->Shape;
				Chip[cno]->SaveShape=Chip[cno]->Shape;
				Chip[parentNo]->DirCode|=dirCode;
				link1=World->AddShaft(Chip[cno2],GVector(0,0,0),Chip[cno],GVector(0,0,0),axis[1],0);
				ChipCount++;if(ChipCount>=GCHIPMAX) ChipCount=GCHIPMAX-1;
				an=2;
			}
		}
		do {
			c=getToken(fp,str);
			if(c==')') break;
			c2=getToken(fp,str2);if(c2!='=') return 105;//'=' is necessary.
			c3=getToken(fp,str3);
			
			minus=false;
			if(c3==2) {
				if(str3[0]=='-'&& str3[1]=='\0') {
					c3=getToken(fp,str3);
					if(c3!=1) return 1;
					if(!checkFlag) n=getValNo(str3);else n=0;
					minus=true;
					if(n<0) {
						c3=2;
						value=0;
						minus=false;
					}
				}
				else sscanf(str3,"%g",&value);
			}
			else if(c3==1) {
				if(!checkFlag) n=getValNo(str3);else n=0;
				minus=false;
				if(n<0) {
					c3=2;
					value=0;
				}
			}
			else return 106;//The numerical value or the variable is necessary. 
			if(!checkFlag){
				if(strcmp("ANGLE",str)==0 && link2) {
					if(c3==1) {
						link2->Angle=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(link2->Angle);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else link2->Angle=value;
				}
				else if(strcmp("SPRING",str)==0 && link2) {
					if(c3==1) {
						link2->SpringK=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(link2->SpringK);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else link2->SpringK=value;
				}
				else if((strcmp("DUMPER",str)==0 ||strcmp("DAMPER",str)==0)&& link2) {
					if(c3==1) {
						link2->DamperK=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(link2->DamperK);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else link2->DamperK=value;
				}
				else if(strcmp("POWER",str)==0) {
					if(c3==1) {
						Chip[cno]->Power=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(Chip[cno]->Power);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else {
						Chip[cno]->Power=value;
						Chip[cno]->Power2=value;
					}
				}
				else if((strcmp("BREAK",str)==0 ||strcmp("BRAKE",str)==0)&& link1) {
					if(c3==1) {
						link1->FrictionK=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(link1->FrictionK);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else link1->FrictionK=value;
				}
				else if(strcmp("NAME",str)==0) {
					lstrcpy(Chip[cno]->Name,str3);
					if(VarCount<GVALMAX) {
						lstrcpy(ValList[VarCount].Name,str3);
						ValList[VarCount].Def=(GFloat)cno;
						ValList[VarCount].Max=FLT_MAX;
						ValList[VarCount].Min=0;
						ValList[VarCount].Dec=0.0f;
						ValList[VarCount].visible=0;
						ValList[VarCount].Label=1;
						VarCount++;
					}
				}
				else if(strcmp("COLOR",str)==0) {
					if(c3==1) {
						Chip[cno]->Color=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(Chip[cno]->Color);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}

					if(c3==2) {
						Chip[cno]->Color=value;
						int col=(int)value;
					}
				}
				else if(strcmp("EFFECT",str)==0) {
					if(c3==1) {
						Chip[cno]->Effect=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(Chip[cno]->Effect);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else {
						Chip[cno]->Effect=value;
					}
				}
				else if(strcmp("OPTION",str)==0) {
					if(c3==2) {
						setOption(Chip[cno],value);
					}
				}
				else if(strcmp("USER1",str)==0) {
					if(c3==1) {
						Chip[cno]->UserEffect=ValList[n].Val;
						ValList[n].Ref[ValList[n].RefCount]=&(Chip[cno]->UserEffect);
						ValList[n].Flag[ValList[n].RefCount]=minus;
						ValList[n].RefCount++;
						if(ValList[n].RefCount>=GREFMAX) ValList[n].RefCount=GREFMAX-1;
					}
					else {
						Chip[cno]->UserEffect=value;
					}
				}
				else if(strcmp("USER2",str)==0) {
					if(c3==2) {
						Chip[cno]->UserOption=value;
					}
				}
				else return 107;// It is a key-word doesn't know
			}
			c=getToken(fp,str);
		} while (c==',');
		if(c!=')') return 108;//')' is necessary.
		c=getToken(fp,str);
		if(c=='{') {
			if(!checkFlag) cw=readChildData(fp,cno,type,checkFlag);
			else cw=readChildData(fp,0,type,checkFlag);
			if(cw!=0) return cw;
		}
	}
	return 0;
}


int  readData(char *name,bool checkFlag)
{
	FILE *fp;
	int haveArm=MyPlayerData.haveArm;
	MyPlayerData.haveArm=0;
	World->haveArm=0;
	DataCheck=0;
	if((fp=fopen(name,"r"))==NULL) return 1;//The file doesn't open it. 
	if(!checkFlag && ScriptL) {
		luaScriptEnd(ScriptL);
		ScriptL=NULL;
	}
	int a=readData2(fp,checkFlag);
	fclose(fp);
	if(checkFlag) {
		MyPlayerData.haveArm=haveArm;
		World->haveArm=haveArm;
	}

	return a;
}
int  readData2(FILE *fp,bool checkFlag)
{
	char str[1024];
	char str2[1024];
	char str3[1024];
	DataCheck=1;
	int i,cw,cw2,cw3,n,vn;
	GetTokenCh();
	if(!checkFlag) {
		World->DeleteRigids();
		for(i=0;i<GKEYMAX;i++) KeyList[i].Count=0;
		for(i=0;i<GVALMAX;i++) ValList[i].RefCount=0;
		if(ScriptL) luaScriptEnd(ScriptL);
		ScriptL=NULL;
		ChipCount=0;
		VarCount=0;
	}
	cw=getToken(fp,str);
	if(strcmp("VAL",str)==0||strcmp("VAR",str)==0) {
		cw=getToken(fp,str);
		if(cw!='{') {return 2;} // '{' is necessary
		cw=getToken(fp,str);
		while(cw!='}') {
			if(!checkFlag){
				if(VarCount>=GVALMAX) {return 3;}	//There are a lot of variables. 
				if(cw==1) {
					lstrcpy(ValList[VarCount].Name,str);
					ValList[VarCount].Def=0.0f;
					ValList[VarCount].Max=FLT_MAX;
					ValList[VarCount].Min=0;
					ValList[VarCount].Dec=0.0f;
					ValList[VarCount].visible=1;
					ValList[VarCount].Label=0;
					ValList[VarCount].RefCount=0;
					ValList[VarCount].Val=0.0f;
				}
				else {return 4;}	// '}' is necessary
			}
			cw=getToken(fp,str);
			if(cw!='(') {return 5;}	// '(' is necessary
			cw=getToken(fp,str);
			while(cw!=')') {
				if(cw==',') cw=getToken(fp,str);
				cw2=getToken(fp,str2);
				cw3=getToken(fp,str3);
				if(strcmp("DEFAULT",str)==0) {
					if(!checkFlag)sscanf(str3,"%g",&(ValList[VarCount].Def));
				}
				else if(strcmp("MIN",str)==0) {
					if(!checkFlag)sscanf(str3,"%g",&(ValList[VarCount].Min));
				}
				else if(strcmp("MAX",str)==0) {
					if(!checkFlag)sscanf(str3,"%g",&(ValList[VarCount].Max));
				}
				else if(strcmp("STEP",str)==0) {
					if(!checkFlag)sscanf(str3,"%g",&(ValList[VarCount].Dec));
				}
				else if(strcmp("DISP",str)==0) {
					if(!checkFlag)sscanf(str3,"%d",&(ValList[VarCount].visible));
				}
				else {return 6;} // It is a key-word doesn't know.
				cw=getToken(fp,str);
			}
			if(ValList[VarCount].Def>=ValList[VarCount].Max) ValList[VarCount].Def=ValList[VarCount].Max;
			if(ValList[VarCount].Def<=ValList[VarCount].Min) ValList[VarCount].Def=ValList[VarCount].Min;
//			ValList[VarCount].Val=ValList[VarCount].Def;
			if(!checkFlag) VarCount++;
			cw=getToken(fp,str);
		}
		cw=getToken(fp,str);
	}
	else {
		return 7; //There is no 'VAL' block.
	}
	if(strcmp("KEY",str)==0) {
		cw=getToken(fp,str);
		if(cw!='{') {return 2;}// '{' is necessary
		cw=getToken(fp,str);
		while(cw!='}') {
			if(cw!=',') {
				sscanf(str,"%d",&n);
				if(n<0 || n>=GKEYMAX) {return 8;}	//There are a lot of keys. 
				cw=getToken(fp,str);
				if(cw!=':') {return 8;}	// ':' is necessary
			}
			cw=getToken(fp,str);
			if(!checkFlag){
				vn=getValNo(str);
				if(KeyList[n].Count>=GVALMAX) {return 9;}//The variable doesn't exist. 
				KeyList[n].ValNo[KeyList[n].Count]=vn;
			}
			cw=getToken(fp,str);
			if(cw!='(') {return 5;}	// '(' is necessary
			cw=getToken(fp,str);
			while(cw!=')') {
				if(cw==',') cw=getToken(fp,str);
				cw2=getToken(fp,str2);
				cw3=getToken(fp,str3);
				GFloat v=0;
				bool   f=false;
				if(strcmp("STEP",str)==0) {
					if(!checkFlag){
						sscanf(str3,"%g",&v);
						KeyList[n].Step[KeyList[n].Count]=v;
					}
				}
				else if(strcmp("VALUE",str)==0){if(!checkFlag)sscanf(str3,"%g",&v);}
				else return 11;
				cw=getToken(fp,str);
			}
			if(!checkFlag)KeyList[n].Count++;
			cw=getToken(fp,str);
		}
		cw=getToken(fp,str);
	}
	else {
		return 10; //There is no 'KEY' block.
	}
	if(strcmp("BODY",str)==0) {
		cw=getToken(fp,str);
		int err=0;
		if(cw=='{') {
			err=readChildData(fp,0,0,checkFlag);
			if(err) {return err;}
			for(int i=0;i<ChipCount;i++) {
				if(Chip[i]->ChipType==GT_CORE) Chip[i]->Reset();
			}
		}
		cw=getToken(fp,str);
		
	}
	else {
		return 12;//There is no 'BODY' block.
	}
	if(!checkFlag) {
		if(ScriptSource) delete ScriptSource;
		ScriptSource= new char[200000];
		ScriptSource[0]='\0';
		int b=1;
		//resetToken();
		if(strcmp("SCRIPT",str)==0) {
			ScriptType=0;
			if(GetTokenCh()!='{') searchData(fp,'{'); 
			while(1) {
				cw=getToken2(fp,str);
				if(cw==EOF) break;
				if(strncmp(str,"MODELEND",8)==0) break;
				if(str[0]=='{') b++;
				else if(str[0]=='}') {
					b--;
					if(b==0) break;
				}
				strcat(ScriptSource,str);
			}
			if(cw!=EOF) cw=getToken(fp,str);
		}
		if(strcmp("LUA",str)==0) {
			char *e;
			if(GetTokenCh()!='{') searchData(fp,'{'); 
			ScriptSource[0]='\0';
			ScriptType=1;
			e=fgets(str,1024,fp);;
			strcat(ScriptSource,str);
			while (e!=NULL) {
				e=fgets(str,1024,fp);
				if(e==NULL) break;
				if(strncmp(str,"MODELEND",8)==0) break;
				strcat(ScriptSource,str);
			}
			//最後の}は外す
			for(int k=(int)strlen(ScriptSource)-1;k>=0;k--) {
				if(ScriptSource[k]=='}') {
					ScriptSource[k]='\0';
					break;
				}
			}
			ScriptL=luaScriptInit(ScriptSource);

			if(ScriptL==NULL) {
				ScriptSource[0]='\0';
			}
		}
	}
	return 0;
	
}
char *checkVal(GFloat *ref,char *tmp) {
	for(int i=0;i<VarCount;i++) {
		for(int j=0;j<ValList[i].RefCount;j++) {
			if(ref==ValList[i].Ref[j]) {
				if(ValList[i].Flag[j]) lstrcpy(tmp,TEXT("-"));
				else tmp[0]='\0';
				strcat(tmp,ValList[i].Name);
				return tmp;
			}
		}
	}
	return NULL;
}
int saveChips(FILE *fp,GRigid *rigid,int level)
{
	char tmp[128];
	if(rigid->ChipType!=GT_WHEEL && rigid->ChipType!=GT_RLW
		&& rigid->ChipType!=GT_WHEEL2 && rigid->ChipType!=GT_RLW2){
		for(int i=0;i<level;i++) fprintf(fp,"\t");
	}
	int c=' ';
	switch(rigid->Dir) {
		case 2:c='N';break;
		case 3:c='E';break;
		case 1:c='W';break;
		case 0:c='S';break;
	}
	switch(rigid->ChipType) {
		case GT_CORE:
			fprintf(fp,"Core");break;
		case GT_CHIP:
			fprintf(fp,"%c:Chip",c);break;
		case GT_RUDDER:
			fprintf(fp,"%c:Rudder",c);break;
		case GT_DUMMY:
			fprintf(fp,"%c:",c);
			break;
		case GT_WHEEL:
			fprintf(fp,"Wheel");break;
		case GT_RLW:
			fprintf(fp,"RLW");break;
		case GT_TRIM:
			fprintf(fp,"%c:Trim",c);break;
		case GT_JET:
			fprintf(fp,"%c:Jet",c);break;
		case GT_CHIPH:
			fprintf(fp,"%c:Weight",c);break;
		case GT_COWL:
			fprintf(fp,"%c:Cowl",c);break;
		case GT_ARM:
			fprintf(fp,"%c:Arm",c);break;
		case GT_CHIP2:
			fprintf(fp,"%c:Frame",c);break;
		case GT_RUDDER2:
			fprintf(fp,"%c:RudderF",c);break;
		case GT_TRIM2:
			fprintf(fp,"%c:TrimF",c);break;
		case GT_WHEEL2:
			fprintf(fp,"%c:WheelF",c);break;
		case GT_RLW2:
			fprintf(fp,"%c:RLWF",c);break;
	}
	int camma=0;
	char *vname;
	if(rigid->ChipType!=GT_DUMMY){
		if(rigid->ChipType==GT_CORE){
			fprintf(fp,"() {");
		}
		else if(rigid->ChipType==GT_WHEEL || rigid->ChipType==GT_RLW){
			fprintf(fp,"(");
			vname=checkVal(&rigid->Parent->LinkInfo->Angle,tmp);
			if(rigid->Parent->LinkInfo->Angle!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"angle=%g",rigid->Parent->LinkInfo->Angle);
				else fprintf(fp,"angle=%s",vname);
			}
			vname=checkVal(&rigid->Power,tmp);
			if(rigid->Power!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"power=%g",rigid->Power);
				else fprintf(fp,"power=%s",vname);
			}
			vname=checkVal(&rigid->LinkInfo->FrictionK,tmp);
			if(rigid->LinkInfo->FrictionK!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"brake=%g",rigid->LinkInfo->FrictionK);
				else fprintf(fp,"brake=%s",vname);
			}
			vname=checkVal(&rigid->Parent->LinkInfo->SpringK,tmp);
			if(rigid->Parent->LinkInfo->SpringK!=1 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"spring=%g",rigid->Parent->LinkInfo->SpringK);
				else fprintf(fp,"spring=%s",vname);
			}
			vname=checkVal(&rigid->Parent->LinkInfo->DamperK,tmp);
			if(rigid->Parent->LinkInfo->DamperK!=0.5 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"dumper=%g",rigid->Parent->LinkInfo->DamperK);
				else fprintf(fp,"dumper=%s",vname);
			}
			vname=checkVal(&rigid->Color,tmp);
			if(rigid->Color!=0xffffff || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"color=#%06X",(int)rigid->Color);
				else fprintf(fp,"color=%s",vname);
			}
			if(rigid->Option!=0) {
				if(camma==0) camma=1;else fprintf(fp,",");
				fprintf(fp,"option=%g",rigid->Option);
			}
			vname=checkVal(&rigid->Effect,tmp);
			if(rigid->Effect!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"effect=%g",rigid->Effect);
				else fprintf(fp,"effect=%s",vname);
			}
			if(rigid->Name[0]!='\0') {
				if(camma==0) camma=1;else fprintf(fp,",");
				fprintf(fp,"name=%s",rigid->Name);
			}
			fprintf(fp,") {");
		}
		else {
			fprintf(fp,"(");
			vname=checkVal(&rigid->LinkInfo->Angle,tmp);
			if(rigid->LinkInfo->Angle!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"angle=%g",rigid->LinkInfo->Angle);
				else fprintf(fp,"angle=%s",vname);
			}
			vname=checkVal(&rigid->Power,tmp);
			if(rigid->Power!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"power=%g",rigid->Power);
				else fprintf(fp,"power=%s",vname);
			}
			vname=checkVal(&rigid->LinkInfo->SpringK,tmp);
			if(rigid->LinkInfo->SpringK!=1 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"spring=%g",rigid->LinkInfo->SpringK);
				else fprintf(fp,"spring=%s",vname);
			}
			vname=checkVal(&rigid->LinkInfo->DamperK,tmp);
			if(rigid->LinkInfo->DamperK!=0.5 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"dumper=%g",rigid->LinkInfo->DamperK);
				else fprintf(fp,"dumper=%s",vname);
			}
			vname=checkVal(&rigid->Color,tmp);
			if(rigid->Color!=0xffffff || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"color=#%06X",(int)rigid->Color);
				else fprintf(fp,"color=%s",vname);
			}
			if(rigid->Option!=0) {
				if(camma==0) camma=1;else fprintf(fp,",");
				fprintf(fp,"option=%g",rigid->Option);
			}
			vname=checkVal(&rigid->Effect,tmp);
			if(rigid->Effect!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"effect=%g",rigid->Effect);
				else fprintf(fp,"effect=%s",vname);
			}
			if(rigid->Name[0]!='\0') {
				if(camma==0) camma=1;else fprintf(fp,",");
				fprintf(fp,"name=%s",rigid->Name);
			}
			vname=checkVal(&rigid->UserEffect,tmp);
			if(rigid->UserEffect!=0 || vname!=NULL) {
				if(camma==0) camma=1;else fprintf(fp,",");
				if(vname==NULL) fprintf(fp,"user1=%g",rigid->UserEffect);
				else fprintf(fp,"user1=%s",vname);
			}
			if(rigid->UserOption!=0) {
				if(camma==0) camma=1;else fprintf(fp,",");
				fprintf(fp,"user2=%g",rigid->UserOption);
			}

			fprintf(fp,") {");
		}
	}
	if(rigid->ChipType!=GT_DUMMY) fprintf(fp,"\n");
	if(rigid->ChildCount!=0) {
		for(int i=0;i<rigid->ChildCount;i++) {
			if(rigid->ChipType!=GT_WHEEL && rigid->ChipType!=GT_RLW
				&& rigid->ChipType!=GT_WHEEL2 && rigid->ChipType!=GT_RLW2)
				saveChips(fp,rigid->Child[i].RigidB,level+1);
			else saveChips(fp,rigid->Child[i].RigidB,level);
		}
	}
	if(rigid->ChipType!=GT_WHEEL && rigid->ChipType!=GT_RLW
		&& rigid->ChipType!=GT_WHEEL2 && rigid->ChipType!=GT_RLW2){
		for(int i=0;i<level;i++) fprintf(fp,"\t");
		fprintf(fp,"}\n");
	}
	return 0;
}
int saveData(char *fname)
{
	FILE *fp;
	if((fp=fopen(fname,"w"))!=NULL) {
		_SYSTEMTIME time;
		GetLocalTime(&time);
		fprintf(fp,"// RigidChips\n");
		fprintf(fp,"// %d/%d/%d %d:%d:%d\n",time.wMonth,time.wDay,time.wYear,time.wHour,time.wMinute,time.wSecond);
		fprintf(fp,"Val {\n");
		for(int i=0;i<VarCount;i++) {
			if(ValList[i].Label==0) {
				fprintf(fp,"\t%s(default=%g, step=%g"
					,ValList[i].Name,ValList[i].Def,ValList[i].Dec);
				if(ValList[i].Max<FLT_MAX) fprintf(fp,", max=%g",ValList[i].Max);
				if(ValList[i].Min!=0) fprintf(fp,", min=%g",ValList[i].Min);
				if(ValList[i].visible==false) fprintf(fp,", disp=0)\n");
				else fprintf(fp,")\n");
			}
		}
		fprintf(fp,"}\n");
		fprintf(fp,"Key {\n");
		for(int i=0;i<GKEYMAX;i++) {
			if(KeyList[i].Count>0) {
				fprintf(fp,"\t%d:",i);
				for(int j=0;j<KeyList[i].Count;j++) {
					if(j>0) fprintf(fp,", "); 
					fprintf(fp,"%s(step=%g)",ValList[KeyList[i].ValNo[j]].Name,KeyList[i].Step[j]);
				}
				fprintf(fp,"\n");
			}
		}
		fprintf(fp,"}\n");
		fprintf(fp,"Body {\n");
		for(int i=0;i<ChipCount;i++) {
			if(Chip[i]->Parent==NULL) saveChips(fp,Chip[i],1);
		}
		fprintf(fp,"}\n");
		fclose(fp);
	}
	return 0;
}

