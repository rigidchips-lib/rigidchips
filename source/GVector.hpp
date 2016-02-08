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
//<�t���O�̐���>>
// �x�N�g�����c�Ŏg�����͍s���]�u���Ďg��
// ��j
// //�x�N�g�������Ŏg��
// GMatrix M1,M2;
// GVector V1,V2;
// V1=GVector(1,2,3);
// M1=M1.scale(GVector(2,2,2));
// M2=M2.translate(GVector(10,20,30));
// V2=V1*M1*M2;
//
// //�x�N�g�����c�Ŏg��
// #define GMATRIXTRANSPOSE //�K�v
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
    // ������1
    GVector(void) { x = y = z = 0; }
	
    // ������2(�R�s�[�j
    GVector(const GVector &v) : x(v.x), y(v.y), z(v.z) {}
	
    // ������3(float�z�񂩂珉����)
    GVector(float f[3]): x((GFloat)f[0]), y((GFloat)f[1]) ,z((GFloat)f[2]){}
	
    // ������4(double�z�񂩂珉����)
    GVector(double f[3]): x((GFloat)f[0]), y((GFloat)f[1]) ,z((GFloat)f[2]){}
	
    // ������5(3�̒l���珉����)
    GVector(GFloat fx, GFloat fy, GFloat fz): x(fx), y(fy), z(fz) {}
	
    // ������6(1�̒l���珉����)
    GVector(GFloat f): x(f), y(f), z(f) {}
	
    // �x�N�g���̔�r(������)
    bool operator==(const GVector& v) const{
		if(v.x!=x) return false;
		if(v.y!=y) return false;
		if(v.z!=z) return false;
		return true;
    }
	
    // �x�N�g���̔�r(�������Ȃ�)
    bool operator!=(const GVector& v) const{
		return v.x!=x || v.y!=y || v.z!=z;
    }
    // �x�N�g���̑傫��
    GFloat abs(void) const {
		return((GFloat)sqrt(x * x + y * y + z * z));
    }
	
    // �x�N�g�����N���A
    void clear(void)  {
		x=y=z=0;
    }
    // �x�N�g���̑傫��2(�������Ȃ�)
    inline GFloat abs2(void) const {
		return(x * x + y * y + z * z);
    }
	
    // ����
    GFloat distance(const GVector& v) const;
	
    // ������̓_
    GVector pointOnLine(const GVector& v1, const GVector& v2) const ;
    // ������̓_
    GVector pointOnLine2(const GVector& v1, const GVector& v2) const ;
	
    // ���ʏ�̓_
	GFloat distanceOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const ;
    GVector pointOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const ;
    GVector pointOnFaceAndLine(const GVector& n, const GFloat d,const GVector& dir) const ;
    int pointOnFaceAndLine2(const GVector& n, const GVector& p,const GVector& dir,GVector &point) const ;
    int pointOnFaceAndLine2(const GVector& n, const GFloat d,const GVector& dir,GVector &point) const ;

	// ���Ɛ����Ƃ̌�_
	GFloat distanceOnBallAndLine(const GFloat r, const GVector& p,const GVector& dir) const;
    // ����(2�_)�Ƃ̋���
    GFloat distanceFromLine(const GVector& p1, const GVector& p2) const ;
	
    // ����(�_�ƕ����x�N�g��)�Ƃ̋���
    GFloat distanceFromVectorLine(const GVector& p, const GVector& v) const ;
	
    // �����Ƃ̋���
    GFloat distanceFromLine2(const GVector& p1, const GVector& p2) const ;
	
	// ���ʂƂ̋���
	GFloat distanceFromFace(const GVector& n, const GFloat d) const ;
	GFloat distanceFromFace2(const GVector& n, const GFloat d) const ;
	
	// ���ʂƂ̋���
	GFloat distanceFromFace(const GVector& n,const GVector& p) const ;
	GFloat distanceFromFace2(const GVector& n,const GVector& p) const ;
	
    // �x�N�g���Ԃ̊p�x�i���W�A���j
    GFloat angle(const GVector& v) const ;
	GFloat angleFast(const GVector& v) const ;
    // �x�N�g���Ԃ̕����t�p�x�i���W�A���j
    GFloat angle2(const GVector& v,GVector& n) const ;
    // ����
    GFloat dot(const GVector& v) const {
		return(x * v.x + y * v.y + z * v.z);
    }
	
    // �O��
    GVector cross(const GVector& v) const {
		return(GVector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x));
    }
	
    // float�z�񂩂�Z�b�g
    void get(float f[3]) {
		x = (GFloat)f[0]; y = (GFloat)f[1]; z = (GFloat)f[2];
    }
	
    // double�z�񂩂�Z�b�g
    void get(double f[3]) {
		x = (GFloat)f[0]; y = (GFloat)f[1]; z = (GFloat)f[2];
    }
	
    // float�z��ւ̎��o��
    void put(float f[3]) const {
		f[0] = (float)x; f[1] = (float)y; f[2] = (float)z;
    }
	
    // double�z��ւ̎��o��
    void put(double f[3]) const {
		f[0] = x; f[1] = y; f[2] = z;
    }
	
    // ���]�����x�N�g��
    GVector negate(void) const {
		return GVector(-x, -y, -z );
    }
	
    // �z��Y���ŃA�N�Z�X����
    GFloat& operator[] (int inx) {
		if (inx == 0) return(x);
		else if (inx == 1) return(y);
		else return(z);
    }
	
    // �z��Y���ŃA�N�Z�X����
    const GFloat& operator[] (int inx) const {
		if (inx == 0) return(x);
		else if (inx == 1) return(y);
		else return(z);
    }
	
	
    // �x�N�g���̉��Z(+=)
    GVector& operator+=(const GVector& A) {
		x += A.x;
		y += A.y;
		z += A.z;
		return(*this);
    }
	
    // �x�N�g���̌��Z(-=)
    GVector& operator-=(const GVector& A) {
		x -= A.x;
		y -= A.y;
		z -= A.z;
		return(*this);
    }
	
    // �X�J���[�Ƃ̐ώZ(*=)
    GVector& operator*=(GFloat A) {
		x *= A;
		y *= A;
		z *= A;
		return(*this);
    }
	
    // �X�J���[�Ƃ̏��Z(/=)
    GVector& operator/=(GFloat A) {
		x /= A;
		y /= A;
		z /= A;
		return(*this);
    }
	
    // �x�N�g���̐ώZ(*=)(�v�f���m)
    GVector& operator*=(const GVector& A) {
		x *= A.x;
		y *= A.y;
		z *= A.z;
		return(*this);
    }
	
    // �x�N�g���̏��Z(/=)(�v�f���m)
    GVector& operator/=(const GVector& A) {
		x /= A.x;
		y /= A.y;
		z /= A.z;
		return(*this);
    }
	
    // �x�N�g���̐��K��(�傫����0�Ƃ�(MAXFLT,MAXFLT,MAXFLT))
    GVector normalize(void) const{
		GFloat r = abs();
		GVector c;
		if (r != 0.0) {c.x=x/r; c.y=y/r; c.z=z/r; }
		else  c.x = c.y = c.z = (GFloat)MAXFLT;
		return(c);
    }
	
	// �x�N�g���̐��K��(�傫����0�Ƃ�(0,0,0))
    GVector normalize2(void) const {
		GFloat r = abs();
		GVector c(x,y,z);
		if (r > gEpsilon) {c.x=x / r;c.y= y / r;c.z= z / r;}
		return(c);
    }

	// �������J�b�g����(A�͒P�ʃx�N�g��)
    GVector Cut(const GVector& A) const{
		GVector c(x,y,z);
		GVector a=A.normalize2();
		if(a.abs2()==0.0) return(c);
		GFloat d=a.dot(c);
		if(d>=0.0) c=c-(d*a);
		return(c);
    }
	
    // �������J�b�g����(A�͒P�ʃx�N�g��)
    GVector Cut2(const GVector& A) const{
		GVector c(x,y,z);
		GVector a=A.normalize2();
		if(a.abs2()==0.0) return(c);
		GFloat d=a.dot(c);
		if(d!=0.0) c=c-(d*a);
		return(c);
    }
	
	
    // �v�f���Ƃɏ��������̒l�𓾂�
    GVector Min(const GVector& A) const{
		GVector c(x,y,z);
		if (A.x < x) c.x = A.x;
		if (A.y < y) c.y = A.y;
		if (A.z < z) c.z = A.z;
		return(c);
    }
	
    // �v�f���Ƃɑ傫�����̒l�𓾂�
    GVector Max(const GVector& A) const{
		GVector c(x,y,z);
		if (A.x > c.x) c.x = A.x;
		if (A.y > c.y) c.y = A.y;
		if (A.z > c.z) c.z = A.z;
		return(c);
    }
	
    // �v�f�̒��ň�ԏ������l�𓾂�
    GFloat Min(void) const{
		GFloat a=x;
		if (y < a) a = y;
		if (z < a) a = z;
		return a;
    }
	
    // �v�f�̒��ň�ԑ傫���l�𓾂�
    GFloat Max(void) const{
		GFloat a=x;
		if (y > a) a = y;
		if (z > a) a = z;
		return a;
    }
	
    // �x�N�g���̘a
    friend GVector operator+(const GVector& A, const GVector& B) {
		return(GVector(A.x + B.x, A.y + B.y, A.z + B.z));
    }
	
    // �x�N�g���̔��]
    friend GVector operator-(const GVector& A) {
		return(GVector(-A.x, -A.y, -A.z));
    }
	
    // �x�N�g���̍�
    friend GVector operator-(const GVector& A, const GVector& B) {
		return(GVector(A.x - B.x, A.y - B.y, A.z - B.z));
    }
	
    // �x�N�g���̐�
    friend GVector operator*(const GVector& A, const GVector& B) {
		return(GVector(A.x * B.x, A.y * B.y, A.z * B.z));
    }
	
    // �x�N�g���ƃX�J���[�̐�
    friend GVector operator*(const GVector& A, GFloat B) {
		return(GVector(A.x * B, A.y * B, A.z * B));
    }
	
    // �X�J���[�ƃx�N�g���̐�
    friend GVector operator*(GFloat A, const GVector& B) {
		return(B * A);
    }
	
    // �x�N�g���̏�
    friend GVector operator/(const GVector& A, const GVector& B) {
		return(GVector(A.x / B.x, A.y / B.y, A.z / B.z));
    }
	
    // �x�N�g���ƃX�J���[�̏�
    friend GVector operator/(const GVector& A, GFloat B) {
		return(GVector(A.x / B, A.y / B, A.z / B));
    }
	
    // �X�J���[�ƃx�N�g���̏�
    friend GVector operator/(GFloat A, const GVector& B) {
		return(GVector(A / B.x, A / B.y, A / B.z));
    }
	
    /// �x�N�g���̗v�f�̏��������̒l�𓾂�
    friend GVector Min(const GVector& a, const GVector& b) {
		return(GVector((a.x<b.x)?a.x:b.x, (a.y<b.y)?a.y:b.y, (a.z<b.z)?a.z:b.z));
    }
	
    /// �x�N�g���̗v�f�̑傫�����̒l�𓾂�
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
	// ������
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
    //�m����(|Q|)
	GFloat norm(void) const{
		return  x*x + y*y + z*z + w*w;
	}
    //�傫��(||Q||)
	GFloat abs(void) const{
		return (GFloat)sqrt(x*x + y*y + z*z + w*w);
	}
    //�P��(Q/||Q||)
	inline GQuat unit(void) const{
		GFloat s = (GFloat)sqrt(x*x + y*y + z*z + w*w);
        if(s==0) return(GQuat(x,y,z,w));
        return(GQuat(x/s,y/s,z/s,w/s));
	}
    //����(Q')
	GQuat conjugate(void) const{
        return(GQuat(-x,-y,-z,w));
	}
    // ����(Q1�EQ2)
	GFloat dot( const GQuat& q) const{
        return (x*q.x + y*q.y + z*q.z + w*q.w)/((GFloat)sqrt(x*x + y*y + z*z + w*w)*(GFloat)sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w));
	}
    //�t�N�H�[�^�j�I��
	GQuat inverse(void) const{
        GFloat d = x*x + y*y + z*z + w*w;
        if (d == 0) return(GQuat(-x,-y,-z,w));
		d = (GFloat)1.0/d;  
        return(GQuat(-x * d,-y * d,-z * d,w * d));
	}
    // �N�H�[�^�j�I���̘a
    friend GQuat operator+(const GQuat& A, const GQuat& B) {
		return(GQuat(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w));
    }
    // �N�H�[�^�j�I���̍�
    friend GQuat operator-(const GQuat& A, const GQuat& B) {
		return(GQuat(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w));
    }
    // �N�H�[�^�j�I���̐�
    friend GQuat operator*(const GQuat& A, const GQuat& B) {
        return(GQuat((A.w*B.x + A.x*B.w) + (A.y*B.z - A.z*B.y),
			(A.w*B.y + A.y*B.w) + (A.z*B.x - A.x*B.z),
			(A.w*B.z + A.z*B.w) + (A.x*B.y - A.y*B.x),
			A.w*B.w - (A.x*B.x + A.y*B.y + A.z*B.z)));
    }
    // �N�H�[�^�j�I���ƃX�J���[�̐�
    friend GQuat operator*(const GFloat& s, const GQuat& A) {
        return(GQuat(A.x*s,A.y*s,A.z*s,A.w*s));
    }
    friend GQuat operator*(const GQuat& A,const GFloat& s) {
        return(GQuat(A.x*s,A.y*s,A.z*s,A.w*s));
    }
    // �N�H�[�^�j�I���ƃX�J���[�̏�
    friend GQuat operator/(const GQuat& A,const GFloat& s) {
        return(GQuat(A.x/s,A.y/s,A.z/s,A.w/s));
    }
    // �N�H�[�^�j�I���̏�
    friend GQuat operator/(const GQuat& A, const GQuat& B) {
        return(A*(B.inverse()));
    }
    // �N�H�[�^�j�I���̕��
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
	//�N�H�|�^�j�I�����s��ɂ���
    GMatrix matrix(void);
    void matrix(GMatrix &m);
	GMatrix33 matrix33(void);
	
};

class GMatrix33;
class GMatrix {
public:
	GFloat elem[4][4];
public:
	// ������
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
	// ������(�R�s�[)
	GMatrix(const GMatrix&);
	GMatrix(const GMatrix33);
	
	// ������(3�̃x�N�g���ŏ�����)
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
	
	// double�z�񂩂�Z�b�g
	void get(double a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// float�z��ւ���Z�b�g
	void get(float a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// double�z��ւ̎��o��
	void put(double a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = (double)elem[i][j];
	}
	
	// float�z��ւ̎��o��
	void put(float a[4][4]) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				a[i][j] = (float)elem[i][j];
	}
	
	// �s��̔�r(������)
	bool operator==(const GMatrix&) const;
	// �s��̔�r(�������Ȃ�)
	bool operator!=(const GMatrix&) const;
	//�@�s��̘a
	GMatrix operator+(const GMatrix&) const;
	// �s��̍�
	GMatrix operator-(const GMatrix&) const;
	// �s��̐�
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
	// �x�N�g���Ƃ̐�
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
	// �X�J���[�Ƃ̐�
	GMatrix operator*(const GFloat&) const;
	// �t�s��33
	GMatrix inverse33(void) const;
	// �t�s��
	GMatrix inverse(void) const;
	void  inverse(GMatrix &im) const;
	// �]�u�s��
	GMatrix transpose(void) const;
	void transpose(GMatrix &m) const;
	
	// �s��̑��
	GMatrix& operator=(const GMatrix&);
	// �s��̑��
	GMatrix& operator=(const GMatrix33&);
	// �s��̉��Z(+=)
	GMatrix& operator+=(const GMatrix&);
	// �s��̌��Z(-=)
	GMatrix& operator-=(const GMatrix&);
	// �s��̐ώZ(*=)
	GMatrix& operator*=(const GMatrix&);
	// �X�J���[�Ƃ̐ώZ(*=)
	GMatrix& operator*=(const GFloat&);
	
	// �X�P�[�����O�s��̍쐬(�X�J���[)
	GMatrix scale(const GFloat&) const;
	
	// �X�P�[�����O�s��̍쐬(�x�N�g��)
	GMatrix scale(const GVector&) const;
	
	// �ړ��s��̍쐬
	GMatrix translate(const GVector&) const;
	
	// X����]�s��̍쐬
	GMatrix rotateX(const GFloat) const;
	
	// Y����]�s��̍쐬
	GMatrix rotateY(const GFloat) const;
	
	// Z����]�s��̍쐬
	GMatrix rotateZ(const GFloat) const;
	
	// XYZ����]�s��̍쐬
	GMatrix rotate(const GVector&) const;
	
	// �C�ӎ�����]�s��̍쐬
	GMatrix rotate(const GVector& axis, GFloat angle) const;
	
	//�s�񂩂�I�C���[�p�����߂�
	void  getEular( GFloat & yaw, GFloat & pitch, GFloat & roll ) ;
    //�r���[�C���O�ϊ��s��̍쐬(�E��n)
	GMatrix viewMatrixR(const GVector &at, const GVector &eye, const GVector &up) const;
	
    //�r���[�C���O�ϊ��s��̍쐬(����n)
	GMatrix viewMatrixL(const GVector &at, const GVector &eye, const GVector &up) const;
	
    //�������e�ϊ��s��̍쐬
	GMatrix projectionMatrix(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const;
	
    //�������e�ϊ��s��̍쐬(DirectX�`��)
    GMatrix projectionMatrixX(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const;
	
    //�������e�ϊ��s��̍쐬
	GMatrix projectionMatrix(GFloat d) const;
	
	// �P�ʍs��ŏ���������
	void unity(void);
	
	// 3x3�������c���ď���������
    GMatrix select33(void) const;
	
	// �P�ʍs�񂩒��ׂ�
	bool isUnity(void) const;
	
    // �x�N�g���ƍs��Ƃ̐�
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
    //�s����N�H�|�^�j�I���ɂ���
    GQuat quat(void);
};
class GMatrix33 {
public:
	GFloat elem[3][3];
public:
	// ������
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
	// ������(�R�s�[)
	GMatrix33(const GMatrix);
	
	// ������(3�̃x�N�g���ŏ�����)
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
	
	// double�z�񂩂�Z�b�g
	void get(double a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// float�z��ւ���Z�b�g
	void get(float a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				elem[i][j] = (GFloat)a[i][j];
	}
	
	// double�z��ւ̎��o��
	void put(double a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a[i][j] = (double)elem[i][j];
	}
	
	// float�z��ւ̎��o��
	void put(float a[3][3]) {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				a[i][j] = (float)elem[i][j];
	}
	
	// �s��̔�r(������)
	bool operator==(const GMatrix33&) const;
	// �s��̔�r(�������Ȃ�)
	bool operator!=(const GMatrix33&) const;
	//�@�s��̘a
	GMatrix33 operator+(const GMatrix33&) const;
	// �s��̍�
	GMatrix33 operator-(const GMatrix33&) const;
	// �s��̐�
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
	// �x�N�g���Ƃ̐�
	GVector operator*(const GVector& v) const {
		GVector r;
		r.x = v.x * elem[0][0] + v.y * elem[0][1] + v.z * elem[0][2] ;
		r.y = v.x * elem[1][0] + v.y * elem[1][1] + v.z * elem[1][2] ;
		r.z = v.x * elem[2][0] + v.y * elem[2][1] + v.z * elem[2][2] ;
		return(r);
	}
	// �X�J���[�Ƃ̐�
	GMatrix33 operator*(const GFloat&) const;
	// �t�s��33
	GMatrix33 inverse(void) const;
	void inverse(GMatrix33 &im) const;
	// �]�u�s��
	GMatrix33 transpose(void) const;
	
	// �s��̑��
	GMatrix33& operator=(const GMatrix33&);
	// �s��̑��
	GMatrix33& operator=(const GMatrix&);
	// �s��̉��Z(+=)
	GMatrix33& operator+=(const GMatrix33&);
	// �s��̌��Z(-=)
	GMatrix33& operator-=(const GMatrix33&);
	// �s��̐ώZ(*=)
	GMatrix33& operator*=(const GMatrix33&);
	// �X�J���[�Ƃ̐ώZ(*=)
	GMatrix33& operator*=(const GFloat&);
	
	// �X�P�[�����O�s��̍쐬(�X�J���[)
	GMatrix33 scale(const GFloat&) const;
	
	// �X�P�[�����O�s��̍쐬(�x�N�g��)
	GMatrix33 scale(const GVector&) const;
	
	// X����]�s��̍쐬
	GMatrix33 rotateX(const GFloat) const;
	
	// Y����]�s��̍쐬
	GMatrix33 rotateY(const GFloat) const;
	
	// Z����]�s��̍쐬
	GMatrix33 rotateZ(const GFloat) const;
	
	// XYZ����]�s��̍쐬
	GMatrix33 rotate(const GVector&) const;
	
	GMatrix33 rotate(const GVector& axis, GFloat angle) const ;
	//�s�񂩂�I�C���[�p�����߂�
	void  getEular( GFloat & yaw, GFloat & pitch, GFloat & roll ) ;
	//�s����N�H�|�^�j�I���ɂ���
	GQuat quat(void) ;
	// �P�ʍs��ŏ���������
	void unity(void);
	
	
	// �P�ʍs�񂩒��ׂ�
	bool isUnity(void) const;
	
    // �x�N�g���ƍs��Ƃ̐�
    friend GVector operator*(GVector v,GMatrix33 m) {
		GVector r;
		r.x = v.x * m.elem[0][0] + v.y * m.elem[1][0] + v.z * m.elem[2][0];
		r.y = v.x * m.elem[0][1] + v.y * m.elem[1][1] + v.z * m.elem[2][1];
		r.z = v.x * m.elem[0][2] + v.y * m.elem[1][2] + v.z * m.elem[2][2];
		return(r);
    }
};


#endif
