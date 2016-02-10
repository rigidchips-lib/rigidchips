#include "GVector.hpp"
class GLandVertex {
public:
	GVector Pos;
	GLandVertex() {};
};
class GLandFace {
public:
	short Index[3];
	GVector Vertex[3];
	GVector Normal;
	GVector Center;
	double Radius;
	GVector OutVec[3];

	GLandFace() {};
	double Check(GVector *p) {
		double d2;
		double d = p->distanceFromFace(Normal, Vertex[0]);
		if (d > 0) return d;
		for (int i = 0;i < 3;i++) {
			d2 = p->distanceFromFace(OutVec[i], Vertex[i]);
			if (d2 > 0) return d2;
		}
		if (d2 > d) return d2;
		return d;
	}
};

class GLand {
public:
	int VertexCount;
	int FaceCount;
	GLandVertex *Vertex;
	GLandFace *Face;
	GLand(int v, int f) {
		Vertex = new GLandVertex[v];
		Face = new GLandFace[f];
		VertexCount = v;
		FaceCount = f;
	};
	void Set() {
		int f;
		double r;
		for (f = 0;f < FaceCount;f++) {
			for (int i = 0;i < 3;i++) Face[f].Vertex[i] = Vertex[Face[f].Index[i]].Pos;
			Face[f].Center = (Face[f].Vertex[0] + Face[f].Vertex[1] + Face[f].Vertex[2]) / 3.0;
			Face[f].Normal = ((Face[f].Vertex[2] - Face[f].Vertex[0])
				.cross(Face[f].Vertex[1] - Face[f].Vertex[0])).normalize2();
			Face[f].OutVec[0] = (Face[f].Normal).cross(Face[f].Vertex[1] - Face[f].Vertex[0]).normalize2();
			Face[f].OutVec[1] = (Face[f].Normal).cross(Face[f].Vertex[2] - Face[f].Vertex[1]).normalize2();
			Face[f].OutVec[2] = (Face[f].Normal).cross(Face[f].Vertex[0] - Face[f].Vertex[2]).normalize2();
			Face[f].Radius = (Face[f].Vertex[0] - Face[f].Center).abs();
			r = (Face[f].Vertex[1] - Face[f].Center).abs();
			if (r > Face[f].Radius) Face[f].Radius = r;
			r = (Face[f].Vertex[2] - Face[f].Center).abs();
			if (r > Face[f].Radius) Face[f].Radius = r;
		}
	}
	double Check(GVector *p) {
		int f;
		for (f = 0;f < FaceCount;f++) {
			double d = Face[f].Check(p);
			if (d < 0) return d;
		}
	}
};