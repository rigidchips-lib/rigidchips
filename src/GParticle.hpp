#include "GVector.hpp"
#include "GRigid.hpp"
//メモリリーク検出用
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 
#include "stdlib.h"

#define GPARTMAX 10000
extern float ARMSPEED;

extern GFloat WaterLine;

int myrand();
class GParticleVertex {
public:
	int Net;
	int Type;
	GVector Pos;
	GVector Vec;
	GVector Acc;
	GFloat Size;
	GFloat SizeD;
	GFloat Life;
	GFloat Power;
	GFloat LifeSpan;
	DWORD  dpnid;
	GVector Color;
};
class GParticle {
public:
	GParticleVertex *Vertex;
	int NextVertex;
	int MaxVertex;
	GParticle(int n) {
		Vertex = new GParticleVertex[n];
		MaxVertex = n;
		NextVertex = 0;
		Clear();
	}
	~GParticle() {
		delete[]Vertex;
	}
	GParticleVertex *Add(GVector pos, GVector vec, GVector acc, GFloat sizeD, GFloat life, GFloat lifeSpan, GVector col) {
		GParticleVertex *ret = &Vertex[NextVertex];
		Vertex[NextVertex].Net = 0;
		Vertex[NextVertex].Type = 0;
		Vertex[NextVertex].Pos = pos;
		Vertex[NextVertex].Vec = vec;
		Vertex[NextVertex].Acc = acc;
		Vertex[NextVertex].Size = 0.4f + (myrand() % 100 * sizeD * 4) / 100.0f - sizeD * 2;
		Vertex[NextVertex].SizeD = sizeD;
		Vertex[NextVertex].Life = life;
		Vertex[NextVertex].Power = life;
		Vertex[NextVertex].LifeSpan = lifeSpan;
		Vertex[NextVertex].Color = col;
		Vertex[NextVertex].dpnid = 0;
		NextVertex++;
		if (NextVertex >= MaxVertex) NextVertex = 0;
		return ret;
	}
	GParticleVertex *Add(int type, GVector pos, GVector vec, GVector acc, GFloat sizeD, GFloat life, GFloat lifeSpan, GVector col, DWORD  dpnid) {
		GParticleVertex *ret = &Vertex[NextVertex];
		Vertex[NextVertex].Type = type;
		if (type != 0) Vertex[NextVertex].Net = 1;
		Vertex[NextVertex].Pos = pos;
		Vertex[NextVertex].Vec = vec;
		Vertex[NextVertex].Acc = acc;
		Vertex[NextVertex].Size = 0.4f + (myrand() % 100 * sizeD * 4) / 100.0f - sizeD * 2;
		Vertex[NextVertex].SizeD = sizeD;
		Vertex[NextVertex].Life = life;
		if (type == 1) Vertex[NextVertex].Life = life*2.0f;
		if (type == 2) Vertex[NextVertex].Life = life*1.6f;
		Vertex[NextVertex].Power = life;
		Vertex[NextVertex].LifeSpan = lifeSpan;
		Vertex[NextVertex].Color = col;
		Vertex[NextVertex].dpnid = dpnid;
		NextVertex++;
		if (NextVertex >= MaxVertex) NextVertex = 0;
		return ret;
	}
	void Clear() {
		for (int i = 0;i < MaxVertex;i++) {
			Vertex[i].Life = -1;
			Vertex[i].Net = 0;
		}
		NextVertex = 0;
	}
	void Move() {
		for (int i = 0;i < MaxVertex;i++) {
			if (Vertex[i].Life > 0) {
				float y = Vertex[i].Pos.y;
				if (y > WaterLine && Vertex[i].Pos.y + Vertex[i].Vec.y < WaterLine) {
					Vertex[i].Vec.x = (GFloat)(Vertex[i].Vec.x + ((myrand() % 100) / 100.0 - 0.5)*Vertex[i].Vec.y);
					Vertex[i].Vec.z = (GFloat)(Vertex[i].Vec.z + ((myrand() % 100) / 100.0 - 0.5)*Vertex[i].Vec.y);
					Vertex[i].Vec.y = (GFloat)(-Vertex[i].Vec.y / 10.0);
				}
				Vertex[i].Pos += Vertex[i].Vec;
				Vertex[i].Vec = Vertex[i].Vec*0.995f + Vertex[i].Acc;
				Vertex[i].Size = Vertex[i].Size + Vertex[i].SizeD;
				Vertex[i].Life -= Vertex[i].LifeSpan;
				if (Vertex[i].Net > 0) Vertex[i].Net++;
			}
		}
	}
};

class GBulletVertex {
public:
	int Net;
	GVector Pos;
	GVector Pos2;
	GVector Vec;
	GFloat Power;
	GFloat Life;
	GFloat Size;
	GVector Tar;
	GFloat Dist;
	GFloat Dist2;
	DWORD  dpnid;
	GRigid *Rigid;
};
class GBullet {
public:
	GBulletVertex *Vertex;
	int NextVertex;
	int MaxVertex;
	GBullet(int n) {
		Vertex = new GBulletVertex[n];
		MaxVertex = n;
		NextVertex = 0;
		Clear();
	}
	~GBullet() {
		delete[]Vertex;
	}
	GBulletVertex *Add(GRigid *rigid, GVector pos, GVector vec, GFloat power, GFloat size, GFloat dist, GVector t, DWORD dpnid) {

		GBulletVertex *ret = &Vertex[NextVertex];
		Vertex[NextVertex].Pos = pos;
		Vertex[NextVertex].Pos2 = pos;
		Vertex[NextVertex].Vec = vec;
		Vertex[NextVertex].Power = power;
		Vertex[NextVertex].Size = size;
		Vertex[NextVertex].Life = 1200.0f;
		Vertex[NextVertex].Tar = t;
		Vertex[NextVertex].Dist = dist;
		Vertex[NextVertex].Dist2 = dist;
		Vertex[NextVertex].Rigid = rigid;
		Vertex[NextVertex].dpnid = dpnid;
		Vertex[NextVertex].Net = 1;
		NextVertex++;
		if (NextVertex >= MaxVertex) NextVertex = 0;
		return ret;
	}
	void Clear() {
		for (int i = 0;i < MaxVertex;i++) {
			Vertex[i].Life = -1;
			Vertex[i].Net = 0;
		}

		NextVertex = 0;
	}
	void Move() {
		for (int i = 0;i < MaxVertex;i++) {
			if (Vertex[i].Dist < 0) {
				Vertex[i].Life = -1;
				Vertex[i].Net = 0;
			}
			if (Vertex[i].Life > 0) {
				float y = Vertex[i].Pos.y;
				Vertex[i].Pos += Vertex[i].Vec;
				Vertex[i].Life -= Vertex[i].Vec.abs();
				Vertex[i].Dist -= Vertex[i].Vec.abs();
				if (Vertex[i].Net > 0) Vertex[i].Net++;
			}
		}
	}
};