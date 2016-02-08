#ifndef G_VECTOR_H
#define G_VECTOR_H
#define GSINGLE_PRECISION
#include <math.h>
#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif
//#define GSINGLE_PRECISION
#ifdef GSINGLE_PRECISION
#define gEpsilon  1E-4
typedef float GFloat;
#else
#define gEpsilon  1E-10
typedef double GFloat;
#endif
#define MAXFLT	 1E+20
class GMatrix;

//#define GMATRIXTRANSPOSE
//<フラグの説明>>
// ベクトルを縦で使う時は行列を転置して使う
// 例）
// //ベクトルを横で使う
// GMatrix M1,M2;
// GVector V1,V2;
// V1=GVector(1,2,3);
// M1=M1.scale(GVector(2,2,2));
// M2=M2.translate(GVector(10,20,30));
// V2=V1*M1*M2;
//
// //ベクトルを縦で使う
// #define GMATRIXTRANSPOSE //必要
// GMatrix M1,M2;
// GVector V1,V2;
// V1=GVector(1,2,3);
// M1=M1.scale(GVector(2,2,2));
// M2=M2.translate(GVector(10,20,30));
// V2=M2*M1*V1;
//
class GVector {
public:
    GFloat x, y, z;
public:
    // 初期化1
    GVector(void) { x = y = z = 0; }
	
    // 初期化2(コピー）
    GVector(const GVector &v) : x(v.x), y(v.y), z(v.z) {}
	
    // 初期化3(float配列から初期化)
    GVector(float f[3]): x((GFloat)f[0]), y((GFloat)f[1]) ,z((GFloat)f[2]){}
	
    // 初期化4(double配列から初期化)
    GVector(double f[3]): x((GFloat)f[0]), y((GFloat)f[1]) ,z((GFloat)f[2]){}
	
    // 初期化5(3つの値から初期化)
    GVector(GFloat fx, GFloat fy, GFloat fz): x(fx), y(fy), z(fz) {}
	
    // 初期化6(1つの値から初期化)
    GVector(GFloat f): x(f), y(f), z(f) {}
	
    // ベクトルの比較(等しい)
    bool operator==(const GVector& v) const{
		if(v.x!=x) return false;
		if(v.y!=y) return false;
		if(v.z!=z) return false;
		return true;
    }
	
    // ベクトルの比較(等しくない)
    bool operator!=(const GVector& v) const{
		return v.x!=x || v.y!=y || v.z!=z;
    }
    // ベクトルの大きさ
    GFloat abs(void) const {
		return((GFloat)sqrt(x * x + y * y + z * z));
    }
	
    // ベクトルをクリア
    void clear(void)  {
		x=y=z=0;
    }
    // ベクトルの大きさ2(平方根なし)
    inline GFloat abs2(void) const {
		return(x * x + y * y + z * z);
    }
	
    // 距離
    GFloat distance(const GVector& v) const;
	
    // 直線上の点
    GVector pointOnLine(const GVector& v1, const GVector& v2) const ;
    // 線分上の点
    GVector pointOnLine2(const GVector& v1, const GVector& v2) const ;
	
    // 平面上の点
	GFloat distanceOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const ;
    GVector pointOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const ;
    GVector pointOnFaceAndLine(const GVector& n, const GFloat d,const GVector& dir) const ;
    int pointOnFaceAndLine2(const GVector& n, const GVector& p,const GVector& dir,GVector &point) const ;
    int pointOnFaceAndLine2(const GVector& n, const GFloat d,const GVector& dir,GVector &point) const ;

	// 球と線分との交点
	GFloat distanceOnBallAndLine(const GFloat r, const GVector& p,const GVector& dir) const;
    // 直線(2点)との距離
    GFloat distanceFromLine(const GVector& p1, const GVector& p2) const ;
	
    // 直線(点と方向ベクトル)との距離
    GFloat distanceFromVectorLine(const GVector& p, const GVector& v) const ;
	
    // 線分との距離
    GFloat distanceFromLine2(const GVector& p1, const GVector& p2) const ;
	
	// 平面との距離
	GFloat distanceFromFace(const GVector& n, const GFloat d) const ;
	GFloat distanceFromFace2(const GVector& n, const GFloat d) const ;
	
	// 平面との距離
	GFloat distanceFromFace(const GVector& n,const GVector& p) const ;
	GFloat distanceFromFace2(const GVector& n,const GVector& p) const ;
	
    // ベクトル間の角度（ラジアン）
    GFloat angle(const GVector& v) const ;
	GFloat angleFast(const GVector& v) const ;
    // ベクトル間の符号付角度（ラジアン）
    GFloat angle2(const GVector& v,GVector& n) const ;
    // 内積
    GFloat dot(const GVector& v) const {
		return(x * v.x + y * v.y + z * v.z);
    }
	
    // 外積
    GVector cross(const GVector& v) const {
		return(GVector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x));
    }
	
    // float配列からセット
    void get(float f[3]) {
		x = (GFloat)f[0]; y = (GFloat)f[1]; z = (GFloat)f[2];
    }
	
    // double配列からセット
    void get(double f[3]) {
		x = (GFloat)f[0]; y = (GFloat)f[1]; z = (GFloat)f[2];
    }
	
    // float配列への取り出し
    void put(float f[3]) const {
		f[0] = (float)x; f[1] = (float)y; f[2] = (float)z;
    }
	
    // double配列への取り出し
    void put(double f[3]) const {
		f[0] = x; f[1] = y; f[2] = z;
    }
	
    // 反転したベクトル
    GVector negate(void) const {
		return GVector(-x, -y, -z );
    }
	
    // 配列添字でアクセスする
    GFloat& operator[] (int inx) {
		if (inx == 0) return(x);
		else if (inx == 1) return(y);
		else return(z);
    }
	
    // 配列添字でアクセスする
    const GFloat& operator[] (int inx) const {
		if (inx == 0) return(x);
		else if (inx == 1) return(y);
		else return(z);
    }
	
	
    // ベクトルの加算(+=)
    GVector& operator+=(const GVector& A) {
		x += A.x;
		y += A.y;
		z += A.z;
		return(*this);
    }
	
    // ベクトルの減算(-=)
    GVector& operator-=(const GVector& A) {
		x -= A.x;
		y -= A.y;
		z -= A.z;
		return(*this);
    }
	
    // スカラーとの積算(*=)
    GVector& operator*=(GFloat A) {
		x *= A;
		y *= A;
		z *= A;
		return(*this);
    }
	
    // スカラーとの商算(/=)
    GVector& operator/=(GFloat A) {
		x /= A;
		y /= A;
		z /= A;
		return(*this);
    }
	
    // ベクトルの積算(*=)(要素同士)
    GVector& operator*=(const GVector& A) {
		x *= A.x;
		y *= A.y;
		z *= A.z;
		return(*this);
    }
	
    // ベクトルの商算(/=)(要素同士)
    GVector& operator/=(const GVector& A) {
		x /= A.x;
		y /= A.y;
		z /= A.z;
		return(*this);
    }
	
    // ベクトルの正規化(大きさが0とき(MAXFLT,MAXFLT,MAXFLT))
    GVector normalize(void) const{
		GFloat r = abs();
		GVector c;
		if (r != 0.0) {c.x=x/r; c.y=y/r; c.z=z/r; }
		else  c.x = c.y = c.z = (GFloat)MAXFLT;
		return(c);
    }
	
	// ベクトルの正規化(大きさが0とき(0,0,0))
    GVector normalize2(void) const {
		GFloat r = abs();
		GVector c(x,y,z);
		if (r > gEpsilon) {c.x=x / r;c.y= y / r;c.z= z / r;}
		return(c);
    }

	// 成分をカットする(Aは単位ベクトル)
    GVector Cut(const GVector& A) const{
		GVector c(x,y,z);
		GVector a=A.normalize2();
		if(a.abs2()==0.0) return(c);
		GFloat d=a.dot(c);
		if(d>=0.0) c=c-(d*a);
		return(c);
    }
	
    // 成分をカットする(Aは単位ベクトル)
    GVector Cut2(const GVector& A) const{
		GVector c(x,y,z);
		GVector a=A.normalize2();
		if(a.abs2()==0.0) return(c);
		GFloat d=a.dot(c);
		if(d!=0.0) c=c-(d*a);
		return(c);
    }
	
	
    // 要素ごとに小さい方の値を得る
    GVector Min(const GVector& A) const{
		GVector c(x,y,z);
		if (A.x < x) c.x = A.x;
		if (A.y < y) c.y = A.y;
		if (A.z < z) c.z = A.z;
		return(c);
    }
	
    // 要素ごとに大きい方の値を得る
    GVector Max(const GVector& A) const{
		GVector c(x,y,z);
		if (A.x > c.x) c.x = A.x;
		if (A.y > c.y) c.y = A.y;
		if (A.z > c.z) c.z = A.z;
		return(c);
    }
	
    // 要素の中で一番小さい値を得る
    GFloat Min(void) const{
		GFloat a=x;
		if (y < a) a = y;
		if (z < a) a = z;
		return a;
    }
	
    // 要素の中で一番大きい値を得る
    GFloat Max(void) const{
		GFloat a=x;
		if (y > a) a = y;
		if (z > a) a = z;
		return a;
    }
	
    // ベクトルの和
    friend GVector operator+(const GVector& A, const GVector& B) {
		return(GVector(A.x + B.x, A.y + B.y, A.z + B.z));
    }
	
    // ベクトルの反転
    friend GVector operator-(const GVector& A) {
		return(GVector(-A.x, -A.y, -A.z));
    }
	
    // ベクトルの差
    friend GVector operator-(const GVector& A, const GVector& B) {
		return(GVector(A.x - B.x, A.y - B.y, A.z - B.z));
    }
	
    // ベクトルの積
    friend GVector operator*(const GVector& A, const GVector& B) {
		return(GVector(A.x * B.x, A.y * B.y, A.z * B.z));
    }
	
    // ベクトルとスカラーの積
    friend GVector operator*(const GVector& A, GFloat B) {
		return(GVector(A.x * B, A.y * B, A.z * B));
    }
	
    // スカラーとベクトルの積
    friend GVector operator*(GFloat A, const GVector& B) {
		return(B * A);
    }
	
    // ベクトルの商
    friend GVector operator/(const GVector& A, const GVector& B) {
		return(GVector(A.x / B.x, A.y / B.y, A.z / B.z));
    }
	
    // ベクトルとスカラーの商
    friend GVector operator/(const GVector& A, GFloat B) {
		return(GVector(A.x / B, A.y / B, A.z / B));
    }
	
    // スカラーとベクトルの商
    friend GVector operator/(GFloat A, const GVector& B) {
		return(GVector(A / B.x, A / B.y, A / B.z));
    }
	
    /// ベクトルの要素の小さい方の値を得る
    friend GVector Min(const GVector& a, const GVector& b) {
		return(GVector((a.x<b.x)?a.x:b.x, (a.y<b.y)?a.y:b.y, (a.z<b.z)?a.z:b.z));
    }
	
    /// ベクトルの要素の大きい方の値を得る
	friend GVector Max(const GVector& a, const GVector& b) {
		return(GVector((a.x>b.x)?a.x:b.x, (a.y>b.y)?a.y:b.y, (a.z>b.z)?a.z:b.z));
    }
	
    // [A*]
    GMatrix star(void) const;
};


class GMatrix;
class GMatrix33;

class GQuat {
public:
	GFloat x,y,z,w;
public:
	// 初期化
	GQuat(void) {
		x=y=z=0;
        w=1;
	}
	GQuat(GFloat sx,GFloat sy,GFloat sz,GFloat sw) {
		x=sx;y=sy;z=sz;w=sw;
	}
	GQuat(const GVector& v,GFloat angle) {
        GFloat s=(GFloat)sin(angle/2.0);
		x=v.x*s;
		y=v.y*s;
		z=v.z*s;
		w=(GFloat)cos(angle/2.0);
	}
    //ノルム(|Q|)
	GFloat norm(void) const{
		return  x*x + y*y + z*z + w*w;
	}
    //大きさ(||Q||)
	GFloat abs(void) const{
		return (GFloat)sqrt(x*x + y*y + z*z + w*w);
	}
    //単位(Q/||Q||)
	inline GQuat unit(void) const{
		GFloat s = (GFloat)sqrt(x*x + y*y + z*z + w*w);
        if(s==0) return(GQuat(x,y,z,w));
        return(GQuat(x/s,y/s,z/s,w/s));
	}
    //共役(Q')
	GQuat conjugate(void) const{
        return(GQuat(-x,-y,-z,w));
	}
    // 内積(Q1・Q2)
	GFloat dot( const GQuat& q) const{
        return (x*q.x + y*q.y + z*q.z + w*q.w)/((GFloat)sqrt(x*x + y*y + z*z + w*w)*(GFloat)sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w));
	}
    //逆クォータニオン
	GQuat inverse(void) const{
        GFloat d = x*x + y*y + z*z + w*w;
        if (d == 0) return(GQuat(-x,-y,-z,w));
		d = (GFloat)1.0/d;  
        return(GQuat(-x * d,-y * d,-z * d,w * d));
	}
    // クォータニオンの和
    friend GQuat operator+(const GQuat& A, const GQuat& B) {
		return(GQuat(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w));
    }
    // クォータニオンの差
    friend GQuat operator-(const GQuat& A, const GQuat& B) {
		return(GQuat(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w));
    }
    // クォータニオンの積
    friend GQuat operator*(const GQuat& A, const GQuat& B) {
        return(GQuat((A.w*B.x + A.x*B.w) + (A.y*B.z - A.z*B.y),
			(A.w*B.y + A.y*B.w) + (A.z*B.x - A.x*B.z),
			(A.w*B.z + A.z*B.w) + (A.x*B.y - A.y*B.x),
			A.w*B.w - (A.x*B.x + A.y*B.y + A.z*B.z)));
    }
    // クォータニオンとスカラーの積
    friend GQuat operator*(const GFloat& s, const GQuat& A) {
        return(GQuat(A.x*s,A.y*s,A.z*s,A.w*s));
    }
    friend GQuat operator*(const GQuat& A,const GFloat& s) {
        return(GQuat(A.x*s,A.y*s,A.z*s,A.w*s));
    }
    // クォータニオンとスカラーの商
    friend GQuat operator/(const GQuat& A,const GFloat& s) {
        return(GQuat(A.x/s,A.y/s,A.z/s,A.w/s));
    }
    // クォータニオンの商
    friend GQuat operator/(const GQuat& A, const GQuat& B) {
        return(A*(B.inverse()));
    }
    // クォータニオンの補間
	GQuat slerp(const GQuat &q,const float t)
	{
		GQuat p;

		GFloat qq = x * q.x + y * q.y + z * q.z + w * q.w;

		GFloat ss = 1.0f - qq * qq;
		  
		if (ss == 0.0f) {
			p.x = x;
			p.y = y;
			p.z = z;
			p.w = w;
		}
		else {
			GFloat sp = (GFloat)sqrt((double)ss);
			GFloat ph = (GFloat)acos((double)qq);
			GFloat pt = ph * t;
			GFloat t1 = (GFloat)sin((double)pt) / sp;
			GFloat t0 = (GFloat)sin((double)(ph - pt)) / sp;
		    
			if(qq<0) 
				t1=-t1;
			p.x = x * t0 + q.x * t1;
			p.y = y * t0 + q.y * t1;
			p.z = z * t0 + q.z * t1;
			p.w = w * t0 + q.w * t1;
		}
		return p;
	}
	//クォ－タニオンを行列にする
    GMatrix matrix(void);
    void matrix(GMatrix &m);
	GMatrix33 matrix33(void);
	
};

class GMatrix33;
class GMatrix {
public:
	GFloat elem[4][4];
public:
	// 初期化
	GMatrix(bool flag=true) {
		if(flag) {
			elem[0][1] = elem[0][2] = elem[0][3] =
				elem[1][0] = elem[1][2] = elem[1][3] =
				elem[2][0] = elem[2][1] = elem[2][3] =
				elem[3][0] = elem[3][1] = elem[3][2] = 0;
			elem[0][0] = elem[1][1] = elem[2][2] = elem[3][3] = 1;
		}
	}
	GMatrix(GFloat f[4][4]) {
		elem[0][0] = f[0][0]; elem[0][1] = f[0][1]; elem[0][2] = f[0][2]; elem[0][3] = f[0][3];
		elem[1][0] = f[1][0]; elem[1][1] = f[1][1]; elem[1][2] = f[1][2]; elem[1][3] = f[1][3];
		elem[2][0] = f[2][0]; elem[2][1] = f[2][1]; elem[2][2] = f[2][2]; elem[2][3] = f[2][3];
		elem[3][0] = f[3][0]; elem[3][1] = f[3][1]; elem[3][2] = f[3][2]; elem[3][3] = f[3][3];
	}
	GMatrix(GFloat f[16]) {
		elem[0][0] =  f[0]; elem[0][1] =  f[1]; elem[0][2] =  f[2]; elem[0][3] =  f[3];
		elem[1][0] =  f[4]; elem[1][1] =  f[5]; elem[1][2] =  f[6]; elem[1][3] =  f[7];
		elem[2][0] =  f[8]; elem[2][1] =  f[9]; elem[2][2] = f[10]; elem[2][3] = f[11];
		elem[3][0] = f[12]; elem[3][1] = f[13]; elem[3][2] = f[14]; elem[3][3] = f[15];
	}
	// 初期化(コピー)
	GMatrix(const GMatrix&);
	GMatrix(const GMatrix33);
	
	// 初期化(3つのベクトルで初期化)
	GMatrix(const GVector& u, const GVector& v, const GVector& w) {
#ifdef GMATRIXTRANSPOSE
		elem[0][0] = u.x;
		elem[1][0] = u.y;
		elem[2][0] = u.z;
		
		elem[0][1] = v.x;
		elem[1][1] = v.y;
		elem[2][1] = v.z;
		
		elem[0][2] = w.x;
		elem[1][2] = w.y;
		elem[2][2] = w.z;
#else
		elem[0][0] = u.x;
		elem[0][1] = u.y;
		elem[0][2] = u.z;
		
		elem[1][0] = v.x;
		elem[1][1] = v.y;
		elem[1][2] = v.z;
		
		elem[2][0] = w.x;
		elem[2][1] = w.y;
		elem[2][2] = w.z;
#endif
		elem[3][0] = elem[3][1] = elem[3][2] =
			elem[0][3] = elem[1][3] = elem[2][3] = 0;
		elem[3][3] = 1;
	}
	
	// double配列からセット
	void get(double a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// float配列へからセット
	void get(float a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// double配列への取り出し
	void put(double a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = (double)elem[i][j];
	}
	
	// float配列への取り出し
	void put(float a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = (float)elem[i][j];
	}
	
	// 行列の比較(等しい)
	bool operator==(const GMatrix&) const;
	// 行列の比較(等しくない)
	bool operator!=(const GMatrix&) const;
	//　行列の和
	GMatrix operator+(const GMatrix&) const;
	// 行列の差
	GMatrix operator-(const GMatrix&) const;
	// 行列の積
	GMatrix operator*(const GMatrix& m) const {
		GMatrix m1(false);
		m1.elem[0][0] = elem[0][0]*m.elem[0][0] + elem[0][1]*m.elem[1][0] + elem[0][2]*m.elem[2][0] + elem[0][3]*m.elem[3][0];
		m1.elem[0][1] = elem[0][0]*m.elem[0][1] + elem[0][1]*m.elem[1][1] + elem[0][2]*m.elem[2][1] + elem[0][3]*m.elem[3][1];
		m1.elem[0][2] = elem[0][0]*m.elem[0][2] + elem[0][1]*m.elem[1][2] + elem[0][2]*m.elem[2][2] + elem[0][3]*m.elem[3][2];
		m1.elem[0][3] = elem[0][0]*m.elem[0][3] + elem[0][1]*m.elem[1][3] + elem[0][2]*m.elem[2][3] + elem[0][3]*m.elem[3][3];

		m1.elem[1][0] = elem[1][0]*m.elem[0][0] + elem[1][1]*m.elem[1][0] + elem[1][2]*m.elem[2][0] + elem[1][3]*m.elem[3][0];
		m1.elem[1][1] = elem[1][0]*m.elem[0][1] + elem[1][1]*m.elem[1][1] + elem[1][2]*m.elem[2][1] + elem[1][3]*m.elem[3][1];
		m1.elem[1][2] = elem[1][0]*m.elem[0][2] + elem[1][1]*m.elem[1][2] + elem[1][2]*m.elem[2][2] + elem[1][3]*m.elem[3][2];
		m1.elem[1][3] = elem[1][0]*m.elem[0][3] + elem[1][1]*m.elem[1][3] + elem[1][2]*m.elem[2][3] + elem[1][3]*m.elem[3][3];

		m1.elem[2][0] = elem[2][0]*m.elem[0][0] + elem[2][1]*m.elem[1][0] + elem[2][2]*m.elem[2][0] + elem[2][3]*m.elem[3][0];
		m1.elem[2][1] = elem[2][0]*m.elem[0][1] + elem[2][1]*m.elem[1][1] + elem[2][2]*m.elem[2][1] + elem[2][3]*m.elem[3][1];
		m1.elem[2][2] = elem[2][0]*m.elem[0][2] + elem[2][1]*m.elem[1][2] + elem[2][2]*m.elem[2][2] + elem[2][3]*m.elem[3][2];
		m1.elem[2][3] = elem[2][0]*m.elem[0][3] + elem[2][1]*m.elem[1][3] + elem[2][2]*m.elem[2][3] + elem[2][3]*m.elem[3][3];

		m1.elem[3][0] = elem[3][0]*m.elem[0][0] + elem[3][1]*m.elem[1][0] + elem[3][2]*m.elem[2][0] + elem[3][3]*m.elem[3][0];
		m1.elem[3][1] = elem[3][0]*m.elem[0][1] + elem[3][1]*m.elem[1][1] + elem[3][2]*m.elem[2][1] + elem[3][3]*m.elem[3][1];
		m1.elem[3][2] = elem[3][0]*m.elem[0][2] + elem[3][1]*m.elem[1][2] + elem[3][2]*m.elem[2][2] + elem[3][3]*m.elem[3][2];
		m1.elem[3][3] = elem[3][0]*m.elem[0][3] + elem[3][1]*m.elem[1][3] + elem[3][2]*m.elem[2][3] + elem[3][3]*m.elem[3][3];
		return(m1);
	}	
	// ベクトルとの積
	GVector operator*(const GVector& v) const {
		GVector r;
		r.x = v.x * elem[0][0] + v.y * elem[0][1] + v.z * elem[0][2] + elem[0][3];
		r.y = v.x * elem[1][0] + v.y * elem[1][1] + v.z * elem[1][2] + elem[1][3];
		r.z = v.x * elem[2][0] + v.y * elem[2][1] + v.z * elem[2][2] + elem[2][3];
		
	//	if(elem[3][0]!=0.0f || elem[3][1]!=0.0f || elem[3][2]!=0.0f || elem[3][3]!=1.0f) {
			GFloat w = v.x*elem[3][0]+ v.y*elem[3][1] + v.z*elem[3][2]+ elem[3][3];
			if(w!=0.0f) r.x /= w; r.y /= w; r.z /= w;
	//	}
		return(r);
	}
	// スカラーとの積
	GMatrix operator*(const GFloat&) const;
	// 逆行列33
	GMatrix inverse33(void) const;
	// 逆行列
	GMatrix inverse(void) const;
	void  inverse(GMatrix &im) const;
	// 転置行列
	GMatrix transpose(void) const;
	void transpose(GMatrix &m) const;
	
	// 行列の代入
	GMatrix& operator=(const GMatrix&);
	// 行列の代入
	GMatrix& operator=(const GMatrix33&);
	// 行列の加算(+=)
	GMatrix& operator+=(const GMatrix&);
	// 行列の減算(-=)
	GMatrix& operator-=(const GMatrix&);
	// 行列の積算(*=)
	GMatrix& operator*=(const GMatrix&);
	// スカラーとの積算(*=)
	GMatrix& operator*=(const GFloat&);
	
	// スケーリング行列の作成(スカラー)
	GMatrix scale(const GFloat&) const;
	
	// スケーリング行列の作成(ベクトル)
	GMatrix scale(const GVector&) const;
	
	// 移動行列の作成
	GMatrix translate(const GVector&) const;
	
	// X軸回転行列の作成
	GMatrix rotateX(const GFloat) const;
	
	// Y軸回転行列の作成
	GMatrix rotateY(const GFloat) const;
	
	// Z軸回転行列の作成
	GMatrix rotateZ(const GFloat) const;
	
	// XYZ軸回転行列の作成
	GMatrix rotate(const GVector&) const;
	
	// 任意軸軸回転行列の作成
	GMatrix rotate(const GVector& axis, GFloat angle) const;
	
	//行列からオイラー角を求める
	void  getEular( GFloat & yaw, GFloat & pitch, GFloat & roll ) ;
    //ビューイング変換行列の作成(右手系)
	GMatrix viewMatrixR(const GVector &at, const GVector &eye, const GVector &up) const;
	
    //ビューイング変換行列の作成(左手系)
	GMatrix viewMatrixL(const GVector &at, const GVector &eye, const GVector &up) const;
	
    //透視投影変換行列の作成
	GMatrix projectionMatrix(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const;
	
    //透視投影変換行列の作成(DirectX形式)
    GMatrix projectionMatrixX(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const;
	
    //透視投影変換行列の作成
	GMatrix projectionMatrix(GFloat d) const;
	
	// 単位行列で初期化する
	void unity(void);
	
	// 3x3部分を残して初期化する
    GMatrix select33(void) const;
	
	// 単位行列か調べる
	bool isUnity(void) const;
	
    // ベクトルと行列との積
    friend GVector operator*(GVector v,GMatrix m) {
		GVector r;
		r.x = v.x * m.elem[0][0] + v.y * m.elem[1][0] + v.z * m.elem[2][0] + m.elem[3][0];
		r.y = v.x * m.elem[0][1] + v.y * m.elem[1][1] + v.z * m.elem[2][1] + m.elem[3][1];
		r.z = v.x * m.elem[0][2] + v.y * m.elem[1][2] + v.z * m.elem[2][2] + m.elem[3][2];
//		if(m.elem[0][3]!=0.0f || m.elem[1][3]!=0.0f || m.elem[2][3]!=0.0f || m.elem[3][3]!=1.0f) {
			GFloat w = v.x*m.elem[0][3]+ v.y*m.elem[1][3] + v.z*m.elem[2][3]+ m.elem[3][3];
			if(w!=0) r.x /= w; r.y /= w; r.z /= w;
//		}
		return(r);
    }
    //行列をクォ－タニオンにする
    GQuat quat(void);
};
class GMatrix33 {
public:
	GFloat elem[3][3];
public:
	// 初期化
	GMatrix33(bool flag=true) {
		if(flag) {
			elem[0][1] = elem[0][2] =
				elem[1][0] = elem[1][2] =
				elem[2][0] = elem[2][1] = 0;
			elem[0][0] = elem[1][1] = elem[2][2] = 1;
		}
	}
	GMatrix33(GFloat f[3][3]) {
		elem[0][0] = f[0][0]; elem[0][1] = f[0][1]; elem[0][2] = f[0][2];
		elem[1][0] = f[1][0]; elem[1][1] = f[1][1]; elem[1][2] = f[1][2];
		elem[2][0] = f[2][0]; elem[2][1] = f[2][1]; elem[2][2] = f[2][2];
	}
	// 初期化(コピー)
	GMatrix33(const GMatrix);
	
	// 初期化(3つのベクトルで初期化)
	GMatrix33(const GVector& u, const GVector& v, const GVector& w) {
#ifdef GMATRIXTRANSPOSE
		elem[0][0] = u.x;
		elem[1][0] = u.y;
		elem[2][0] = u.z;
		
		elem[0][1] = v.x;
		elem[1][1] = v.y;
		elem[2][1] = v.z;
		
		elem[0][2] = w.x;
		elem[1][2] = w.y;
		elem[2][2] = w.z;
#else
		elem[0][0] = u.x;
		elem[0][1] = u.y;
		elem[0][2] = u.z;
		
		elem[1][0] = v.x;
		elem[1][1] = v.y;
		elem[1][2] = v.z;
		
		elem[2][0] = w.x;
		elem[2][1] = w.y;
		elem[2][2] = w.z;
#endif
	}
	
	// double配列からセット
	void get(double a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// float配列へからセット
	void get(float a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// double配列への取り出し
	void put(double a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a[i][j] = (double)elem[i][j];
	}
	
	// float配列への取り出し
	void put(float a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a[i][j] = (float)elem[i][j];
	}
	
	// 行列の比較(等しい)
	bool operator==(const GMatrix33&) const;
	// 行列の比較(等しくない)
	bool operator!=(const GMatrix33&) const;
	//　行列の和
	GMatrix33 operator+(const GMatrix33&) const;
	// 行列の差
	GMatrix33 operator-(const GMatrix33&) const;
	// 行列の積
	GMatrix33 operator*(const GMatrix33&m) const{
		GMatrix33 m1(false);

		m1.elem[0][0] = elem[0][0]*m.elem[0][0] + elem[0][1]*m.elem[1][0] + elem[0][2]*m.elem[2][0];
		m1.elem[0][1] = elem[0][0]*m.elem[0][1] + elem[0][1]*m.elem[1][1] + elem[0][2]*m.elem[2][1];
		m1.elem[0][2] = elem[0][0]*m.elem[0][2] + elem[0][1]*m.elem[1][2] + elem[0][2]*m.elem[2][2];

		m1.elem[1][0] = elem[1][0]*m.elem[0][0] + elem[1][1]*m.elem[1][0] + elem[1][2]*m.elem[2][0];
		m1.elem[1][1] = elem[1][0]*m.elem[0][1] + elem[1][1]*m.elem[1][1] + elem[1][2]*m.elem[2][1];
		m1.elem[1][2] = elem[1][0]*m.elem[0][2] + elem[1][1]*m.elem[1][2] + elem[1][2]*m.elem[2][2];

		m1.elem[2][0] = elem[2][0]*m.elem[0][0] + elem[2][1]*m.elem[1][0] + elem[2][2]*m.elem[2][0];
		m1.elem[2][1] = elem[2][0]*m.elem[0][1] + elem[2][1]*m.elem[1][1] + elem[2][2]*m.elem[2][1];
		m1.elem[2][2] = elem[2][0]*m.elem[0][2] + elem[2][1]*m.elem[1][2] + elem[2][2]*m.elem[2][2];

		return(m1);
	}
	// ベクトルとの積
	GVector operator*(const GVector& v) const {
		GVector r;
		r.x = v.x * elem[0][0] + v.y * elem[0][1] + v.z * elem[0][2] ;
		r.y = v.x * elem[1][0] + v.y * elem[1][1] + v.z * elem[1][2] ;
		r.z = v.x * elem[2][0] + v.y * elem[2][1] + v.z * elem[2][2] ;
		return(r);
	}
	// スカラーとの積
	GMatrix33 operator*(const GFloat&) const;
	// 逆行列33
	GMatrix33 inverse(void) const;
	void inverse(GMatrix33 &im) const;
	// 転置行列
	GMatrix33 transpose(void) const;
	
	// 行列の代入
	GMatrix33& operator=(const GMatrix33&);
	// 行列の代入
	GMatrix33& operator=(const GMatrix&);
	// 行列の加算(+=)
	GMatrix33& operator+=(const GMatrix33&);
	// 行列の減算(-=)
	GMatrix33& operator-=(const GMatrix33&);
	// 行列の積算(*=)
	GMatrix33& operator*=(const GMatrix33&);
	// スカラーとの積算(*=)
	GMatrix33& operator*=(const GFloat&);
	
	// スケーリング行列の作成(スカラー)
	GMatrix33 scale(const GFloat&) const;
	
	// スケーリング行列の作成(ベクトル)
	GMatrix33 scale(const GVector&) const;
	
	// X軸回転行列の作成
	GMatrix33 rotateX(const GFloat) const;
	
	// Y軸回転行列の作成
	GMatrix33 rotateY(const GFloat) const;
	
	// Z軸回転行列の作成
	GMatrix33 rotateZ(const GFloat) const;
	
	// XYZ軸回転行列の作成
	GMatrix33 rotate(const GVector&) const;
	
	GMatrix33 rotate(const GVector& axis, GFloat angle) const ;
	//行列からオイラー角を求める
	void  getEular( GFloat & yaw, GFloat & pitch, GFloat & roll ) ;
	//行列をクォ－タニオンにする
	GQuat quat(void) ;
	// 単位行列で初期化する
	void unity(void);
	
	
	// 単位行列か調べる
	bool isUnity(void) const;
	
    // ベクトルと行列との積
    friend GVector operator*(GVector v,GMatrix33 m) {
		GVector r;
		r.x = v.x * m.elem[0][0] + v.y * m.elem[1][0] + v.z * m.elem[2][0];
		r.y = v.x * m.elem[0][1] + v.y * m.elem[1][1] + v.z * m.elem[2][1];
		r.z = v.x * m.elem[0][2] + v.y * m.elem[1][2] + v.z * m.elem[2][2];
		return(r);
    }
};


#endif
