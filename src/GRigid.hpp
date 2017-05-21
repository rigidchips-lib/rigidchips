#ifndef G_RIGID_H
#define G_RIGID_H
#include <stdlib.h>
#include "Consts.hpp"

#include "GVector.hpp"
#include "readData.hpp"
#include "GLand.hpp"

class GWorld;
class GRigid;

typedef struct {
	signed char x, y, z, w;
} GQuat2;

typedef struct {
	short x, y, z;
	//	char xp,yp,zp;
} GVector2;

typedef struct {
	short id;
	short color;
	union {
		struct {
			unsigned char type;
			unsigned char option;
			GVector2 pos;
			GQuat2	quat;
		};
		struct {
			float f[3];
		};
	} data;
} GCHIPDATA;


typedef struct {
	int CheckShapeNo;
	GVector Pos;
	GVector Normal;
	GFloat Distance;
	GFloat Ud;
	GFloat Us;
	GFloat Ux;
	GRigid  *Target;
	GVector FricV;
	GVector J;
} GHit;

typedef struct {
	int PointN;
	GVector Point[64];
} GShape;


//***************************************************************
//	�X�^�b�t�N���X
//***************************************************************
class GCamera {
public:
	GRigid *Link;
	GVector Pos;
	GVector Target;
	GCamera() {
		Pos = GVector(0, 10, 10);
		Target = GVector(0, 0, 0);
		Link = NULL;
	}
	void SetPosition();
};

class GLight {
public:
	GRigid *Link;
	GVector Pos;
	GVector Target;
	GLight() {
		Pos = GVector(0, 10, 10);
		Target = GVector(0, 0, 0);
		Link = NULL;
	}
	void SetPosition();
};

//***************************************************************
//	���̃N���X
//***************************************************************
class GRigid;
class GLink {
public:
	int Type;
	int ID;
	GRigid *RigidB;
	GRigid *RigidB2;
	GVector OffsetA;
	GVector OffsetB;
	GVector Axis;
	GFloat Angle;
	GFloat Length;
	GFloat SpringK;
	GFloat DamperK;
	GFloat FrictionK;
	GFloat DestroyK;
	GFloat PreDestroyRatio;
	GLink() {
		RigidB = NULL;
		RigidB2 = NULL;
		OffsetA.clear();
		OffsetB.clear();
		Axis = GVector(1, 0, 0);
		Angle = 0.0f;
		Length = 1.0f;
		SpringK = 1.0f;
		DamperK = 0.5f;
		FrictionK = 0;
		DestroyK = 0;
		PreDestroyRatio = 0;
	}
};

class GRigid {
public:
	int Type;
	int Crush;
	int ID;
	int ChipType;
	GVector Param;
	void *Model;
	GShape Shape;
	GShape CheckShape;
	GShape SaveShape;
	GMatrix Rb;		//��{����(�s��)
	int HitN;
	GHit Hit[100];
	bool Fixed;
	int Ghost;
	GWorld *World;

	//��������
	GVector X;		//*�d�S�ʒu
	GVector preX;		//*1�t���[���O�̈ʒu
	GMatrix R;		//*����(�s��)
	GMatrix Rt;		//*�����̋t�s��
	GQuat   Q;		//*����(�N�H�[�^�j�I��)
	GVector P;		//*���i�^����
	GVector L;		//*�p�^����

	GVector P2;		//*���i�^����
	GVector L2;		//*�p�^����

	GVector V;		//*���x
	GVector preV;	//*���x
	GVector W;		//*�p���x

	GFloat	E;		//�e���W��
	GFloat	Us;		//�Ö��C�W��
	GFloat	Ud;		//�����C�W��
	GFloat  Ux;		//���낪��W��
	GFloat  Cd;		//�R�͌W��

	GFloat  Density;//���x
	GFloat  Volume;	//�̐�
	GFloat  M;		//����
	GFloat  M_;		//���ʂ̋t��
	GMatrix33 I;		//*�����e���\��
	GMatrix33 Ib;		//��{�p���ł̊����e���\��
	GMatrix33 I_;		//*�����e���\���̋t��
	GMatrix33 Ib_;	//��{�p���ł̊����e���\���̋t��

	GMatrix TM;		//*�p���s��i�ʒu�E�p�x�j
//		GFloat  Dt;		//���ݎ���

	GFloat Radius;
	GVector Bias;
	GVector TopBias;

	GFloat TotalRadius;
	GFloat TotalMass;
	GVector TotalCenter;
	GVector TotalCenterOfGravity;
	int TotalCount;
	int TotalCowlCount;
	int TotalHitCount;
	GFloat MaxImpulse;

	GVector Ext;

	int ByeFlag;
	GFloat Tolerant;
	GFloat Energy;
	GFloat ArmEnergy;
	GFloat preF;
	GRigid *Top;
	GRigid *Parent;
	GRigid *CowlTop;
	GLink *LinkInfo;
	int ChildCount;
	GLink Child[GCHILDMAX];
	int DirCode;

	char Name[128];
	GFloat Power;
	GFloat Power2;
	GFloat PowerSave;

	GFloat PowerByFuel;

	int HitLand;
	int HitObj;
	int HitChip;
	int HitBullet;

	int MeshNo;
	int FrameFlag;
	int Dir;
	GFloat Option;
	GFloat Color;
	GFloat Effect;
	GFloat UserOption;
	GFloat UserEffect;

	double FuelMax;
	double Fuel;
	double TotalFuelMax;
	double TotalFuel;
	double TotalFuel2;

	int tmp;
public:
	//�R���X�g���N�^
	GRigid(int type, bool fix, GFloat x = 1.0, GFloat y = 1.0, GFloat z = 1.0);
	//�f�X�g���N�^
	~GRigid();
	void Reset();
	void RSet();
	void RbSet(GFloat x, GFloat y, GFloat z);
	void SetTensor();
	void ApplyExtForce();
	void ApplyForce(GVector force, GVector pos);
	void ApplyTorque(GVector torque);
	void ApplyImpulse(GVector &momentum, GVector &pos);
	void ApplyImpulse2(GVector &momentum, GVector &pos);
	void ApplyAngularImpulse(GVector momentum);
	void ApplyLocalImpulse(GVector momentum, GVector pos);
	void ApplyLocalAngularImpulse(GVector momentum);
	void ApplyLocalForce(GVector force, GVector pos);
	void ApplyLocalTorque(GVector torque);

	void CollisionCheck(GRigid *rigid);
	void Calc();
	void CalcLight();
	void Impulse();
	void CreateViewModel();
	void AddViewModel();
	void Disp();
	void DispObject();
	void DispJet(LPDIRECT3DDEVICE9 g_D3DDevice, D3DXMATRIX worldMatrix, CD3DMesh* jetMesh, CD3DMesh* fireMesh, bool JetFlag);
	void DispShadow();
	GMatrix33	CalcMassMat(GVector &p);
	void CalcTotalFuel(void);
	double CheckFuel(double f);
	void UseFuel(double f);
	void CalcTotalCenter(void);
	GVector CalcTotalPos(int *c, int *cc);
	void CalcTotalMass();
	GVector CalcTotalXG(int *c);
	GFloat CalcTotalRadius(GFloat r);
	int CountTotal();
	void TranslateWithChild(GVector &v);
	void TranslateWithChild(GVector v, GRigid* noUse);
	void TranslateWithParentAll(GVector v);
	void TranslateWithParentAll2(GVector v);
	void TranslateWithParent(GVector v);
	void ResetXfWithChild();
	void EnervateWithChild();
	void RotateWithChildAbs(GMatrix33 m, GVector x);
	void RotateWithChild(GMatrix33 m, GVector x);
	void RotateWithChild(GMatrix33 m, GVector x, GRigid* noUse);
	void RotateWithParentAll2(GMatrix33 m, GVector x);
};

//***************************************************************
//	���E�N���X
//***************************************************************

class GWorld
{
public:
	GFloat StepTime;
	int SubStep;
	int Method;		//0:�I�C���[�@1:�����Q�E�N�b�^
	GFloat  Dt;		//���ݎ���
	GVector G;		//�d��
	GCamera Camera;
	GLight Light;
	bool Alt;
	bool Stop;
	bool NetStop;
	bool B20Bullet;
	bool B26Bullet;
	int MainStepCount;
	GFloat DestroyK;
	bool DestroyFlag;
	int ChipCount;
	GRigid	*Rigid[GCHIPMAX];
	GRigid	*RecRigid[GCHIPMAX];
	int haveArm;
	int RndTable[GCHIPMAX];
	int RndTable2[GCHILDMAX];
	int ObjectCount;
	GRigid	*Object[GOBJMAX];
	GRigid	*RecObject[GOBJMAX];
	GLand *Land;
	GRigid	*LandRigid;

public:
	GWorld(GFloat steptime, int substep);
	~GWorld(void);
	void InitRndTable();
	void SetStepTime(float stepTime);
	void SetSubStep(int substep);
	void DeleteRigids();
	void DeleteObjects();
	void DeleteLand();
	GRigid *AddRigid(int type, bool fix, GFloat x, GFloat y, GFloat z, GFloat ax = 0.0, GFloat ay = 0.0, GFloat az = 0.0);
	GRigid *UpdateRigid(GRigid *r, int type, bool fix, GFloat x, GFloat y, GFloat z, GFloat ax = 0.0, GFloat ay = 0.0, GFloat az = 0.0);
	GRigid *AddObject(int type, bool fix, GFloat x, GFloat y, GFloat z, GFloat density = 1.0f, GFloat ax = 0.0, GFloat ay = 0.0, GFloat az = 0.0);
	GLand *AddLand(int vn, int fn);
	void DeleteLand(int vn, int fn);
	GLink *AddJoint(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GFloat friction = 0);
	GLink *AddShaft(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat friction = 0);
	GLink *AddHinge(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat angle, GFloat k, GFloat damper);
	GLink *AddCowl(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat angle);
	void DeleteLinkSub(GRigid* rigid, GRigid* top);
	void DeleteLink(GRigid* rigid);
	void RestoreLink(GRigid* rigid, GRigid* top);
	void MoveReset();
	void Move(bool initFlag);
	void Check(GRigid *rigid);
	void CheckObject(GRigid *rigid);
	void Disp(BOOL net);
	void Disp2();
	void DispNetChip(int n);
	void DispNetChipInfo(int n, float z);
	void DispNetShadow();
	void ObjectDisp();
	void DispNetJetAll();
	void DispNetJet(int type, GMatrix tm, float f, int dir);
	void DispJet(LPDIRECT3DDEVICE9 g_D3DDevice, D3DXMATRIX worldMatrix, CD3DMesh* jetMesh, CD3DMesh* fireMesh, bool JetFlag);
	void CheckJoint(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB);
	void CheckShaft(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis);
	void CheckLink(GRigid* rigidA);
	void CheckLink2(GRigid* rigidA);
	void CalcLink(GRigid* rigidA);
	void CalcLinkCowl(GRigid* rigidA);
	GFloat CalcJoint(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GFloat friction);
	GFloat CalcJoint2(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat friction);
	GFloat CalcShaft(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat friction);
	GFloat CalcHinge(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat angle, GFloat k, GFloat damper);
	GFloat CalcCowl(GRigid* rigidA, GVector &offsetA, GRigid* rigidB, GVector &offsetB, GVector &axis, GFloat angle);

	int getChipCount();
	void IncreaseChipCount();
};
#endif