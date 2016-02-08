#include "GVector.hpp"

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

// 距離
GFloat GVector::distance(const GVector& v) const {
    GFloat a,b,c;
    a=v.x-x;b=v.y-y;c=v.z-z;
	return((GFloat)sqrt(a*a+b*b+c*c));
}

// 直線上の点
GVector GVector::pointOnLine(const GVector& v1, const GVector& v2) const {
    GVector b=(v2-v1).normalize();
	GFloat t=((*this-v1).dot(b))/(b.x*b.x+b.y*b.y+b.z*b.z);
	return b*t+v1;
}
// 線分上の点
GVector GVector::pointOnLine2(const GVector& v1, const GVector& v2) const {
    GVector b=(v2-v1).normalize();
	GFloat t=((*this-v1).dot(b))/(b.x*b.x+b.y*b.y+b.z*b.z);
    if(t>1.0) t=1.0;else if (t<0.0) t=0.0;
	return b*t+v1;
}

// 平面上の点
GFloat GVector::distanceOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const {
	GFloat ab=n.dot(dir);
	if(ab==0.0f) ab=(GFloat)gEpsilon;
	GFloat t=-(n.dot(*this)-n.dot(p))/ab;
	return t;
}
GVector GVector::pointOnFaceAndLine(const GVector& n, const GVector& p,const GVector& dir) const {
	GFloat ab=n.dot(dir);
	if(ab==0.0f) ab=(GFloat)gEpsilon;
	GFloat t=-(n.dot(*this)-n.dot(p))/ab;
	return dir*t+(*this);
}
// 平面と線分との交点
int GVector::pointOnFaceAndLine2(const GVector& n, const GVector& p,const GVector& dir ,GVector &point) const {
	GFloat ab=n.dot(dir);
	if(ab==0.0f) ab=(GFloat)gEpsilon;
	GFloat t=-(n.dot(*this)-n.dot(p))/ab;
	point=dir*t+(*this);
    if(t>1.0 || t<0) return  1;
	return 0;
}
// 球と線分との交点
GFloat GVector::distanceOnBallAndLine(const GFloat r, const GVector& p,const GVector& dir) const {
     // 線分と球の判定
	GFloat	rs = r*r;
	GVector xc = p-GVector(x,y,z);// 中心からの相対位置
	GFloat xc2 = xc.dot(xc);
	GFloat vxc = dir.dot(xc);
	GFloat d = vxc*vxc-xc2+rs;// 判別式
	if(d<0) return -1;

     GFloat tn = -vxc-sqrt(d);
     GFloat tp = -vxc+sqrt(d);
     if(tn<0 && tp<0) return -1;
	 if(tn>=0 && tp>=0) {
		 if(tn<=tp) return tn;
		 else return tp;
	 }
	 if(tn>=0) return tn;
	 return tp;
}
int GVector::pointOnFaceAndLine2(const GVector& n, const GFloat d,const GVector& dir,GVector &point) const {
	GFloat ab=n.dot(dir);
	if(ab==0.0) ab=(GFloat)gEpsilon;
	GFloat t=-(n.dot(*this)-d)/ab;
	point=dir*t+(*this);
    if(t>1.0 || t<0) return  1;
	return 0;
}
GVector GVector::pointOnFaceAndLine(const GVector& n, const GFloat d,const GVector& dir) const {
	GFloat ab=n.dot(dir);
	if(ab==0.0) ab=(GFloat)gEpsilon;
	GFloat t=-(n.dot(*this)-d)/ab;
	return dir*t+(*this);
}
// 直線(2点)との距離
GFloat GVector::distanceFromLine(const GVector& p1, const GVector& p2) const {
    GVector b=(p2-p1).normalize2();
    if(b.abs()==0) return distance(p1);
	GFloat t=((*this-p1).dot(b))/(b.x*b.x+b.y*b.y+b.z*b.z);
	return (b*t+p1-(*this)).abs();
}

// 直線(点と方向ベクトル)との距離
GFloat GVector::distanceFromVectorLine(const GVector& p, const GVector& v) const {
    if(v.abs()==0) return distance(p);
    GVector b=v.normalize2();
	GFloat t=((*this-p).dot(b))/(b.x*b.x+b.y*b.y+b.z*b.z);
	return (b*t+p-(*this)).abs();
}

// 線分との距離
GFloat GVector::distanceFromLine2(const GVector& p1, const GVector& p2) const {
    GVector b=(p2-p1);
    if(b.abs()==0) return distance(p1);
	GFloat t=((*this-p1).dot(b))/(b.x*b.x+b.y*b.y+b.z*b.z);
    if(t>1.0) t=1.0;else if (t<0.0) t=0.0;
	return (b*t+p1-(*this)).abs();
}

// 平面との距離
GFloat GVector::distanceFromFace(const GVector& n, const GFloat d) const {
	return (this->dot(n)-d)/(GFloat)sqrt(n.dot(n));
}
GFloat GVector::distanceFromFace2(const GVector& n, const GFloat d) const {
	GFloat r=this->dot(n)-d;
	return r*r/(n.dot(n));
}

// 平面との距離
GFloat GVector::distanceFromFace(const GVector& n,const GVector& p) const {
	return (this->dot(n)-n.dot(p))/(GFloat)sqrt(n.dot(n));
}
GFloat GVector::distanceFromFace2(const GVector& n,const GVector& p) const {
	GFloat r=this->dot(n)-n.dot(p);
	return r*r/(n.dot(n));
}


// ベクトル間の角度（ラジアン）
GFloat GVector::angle(const GVector& v) const {
    GFloat f,a,b;
    a=this->abs();
    b=v.abs();
    if(a==0 || b==0) return 0.0;
    f=(this->normalize2()).dot(v.normalize2());
    if(fabs(f)<=1) return (GFloat)acos(f);
    if(f<-1) return (GFloat)M_PI;
	return 0.0;
}
// 単位ベクトル間の角度（ラジアン）
GFloat GVector::angleFast(const GVector& v) const {
    GFloat f=this->dot(v);
    if(fabs(f)<=1) return (GFloat)acos(f);
    if(f<-1) return (GFloat)M_PI;
	return 0.0;
}
// ベクトル間の符号付角度（ラジアン）
GFloat GVector::angle2(const GVector& v,GVector& n) const {
    GFloat f,a,b,s;
    a=this->abs();
    b=v.abs();
    if(a==0 || b==0) return 0.0;
	s=1.0f;
	if((this->cross(v)).dot(n)<=0) s=-1.0f;
    f=(this->normalize2()).dot(v.normalize2());
    if(fabs(f)<=1) return (GFloat)acos(f)*s;
    if(f<-1) return (GFloat)M_PI*s;
	return 0.0;
}
// [A*]
GMatrix GVector::star(void) const{
    GMatrix m(false);
	
    m.elem[0][0] = 0;
    m.elem[1][0] = z;
    m.elem[2][0] = -y;
	
    m.elem[0][1] = -z;
    m.elem[1][1] = 0;
    m.elem[2][1] = x;
	
    m.elem[0][2] = y;
    m.elem[1][2] = -x;
    m.elem[2][2] = 0;

    m.elem[0][3] = m.elem[1][3] = m.elem[2][3] = m.elem[3][0] = m.elem[3][1] = m.elem[3][2] = m.elem[3][3] = 0;
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
    return m;
}
//クォ－タニオンを行列にする
GMatrix GQuat::matrix(void)
{
    GMatrix m(false);
    GFloat d,s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;
    d = x*x + y*y + z*z + w*w;
    if (d == 0.0f) s = 1.0f; else s = 2.0f/d;
	
    xs = x * s;   ys = y * s;  zs = z * s;
    wx = w * xs;  wy = w * ys; wz = w * zs;
    xx = x * xs;  xy = x * ys; xz = x * zs;
    yy = y * ys;  yz = y * zs; zz = z * zs;
	
    m.elem[0][0] = 1.0f - (yy + zz);
    m.elem[0][1] = xy + wz;
    m.elem[0][2] = xz - wy;
	
    m.elem[1][0] = xy - wz;
    m.elem[1][1] = 1.0f - (zz + xx);
    m.elem[1][2] = yz + wx;
	
    m.elem[2][0] = xz + wy;
    m.elem[2][1] = yz - wx;
    m.elem[2][2] = 1.0f - (xx + yy);
	
    m.elem[0][3] = m.elem[1][3] = m.elem[2][3]
     =m.elem[3][0] = m.elem[3][1] = m.elem[3][2] = 0;
	m.elem[3][3] = 1;
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
    return m;
}
void GQuat::matrix(GMatrix &m)
{
    GFloat d,s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;
    d = x*x + y*y + z*z + w*w;
    if (d == 0.0f) s = 1.0f; else s = 2.0f/d;
	
    xs = x * s;   ys = y * s;  zs = z * s;
    wx = w * xs;  wy = w * ys; wz = w * zs;
    xx = x * xs;  xy = x * ys; xz = x * zs;
    yy = y * ys;  yz = y * zs; zz = z * zs;
	
    m.elem[0][0] = 1.0f - (yy + zz);
    m.elem[0][1] = xy + wz;
    m.elem[0][2] = xz - wy;
	
    m.elem[1][0] = xy - wz;
    m.elem[1][1] = 1.0f - (zz + xx);
    m.elem[1][2] = yz + wx;
	
    m.elem[2][0] = xz + wy;
    m.elem[2][1] = yz - wx;
    m.elem[2][2] = 1.0f - (xx + yy);
	
    m.elem[0][3] = m.elem[1][3] = m.elem[2][3]
     =m.elem[3][0] = m.elem[3][1] = m.elem[3][2] = 0;
	m.elem[3][3] = 1;
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
}
//クォ－タニオンを行列にする
GMatrix33 GQuat::matrix33(void)
{
    GMatrix33 m(false);
    GFloat d,s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;
    d = x*x + y*y + z*z + w*w;
    if (d == 0.0f) s = 1.0f; else s = 2.0f/d;
	
    xs = x * s;   ys = y * s;  zs = z * s;
    wx = w * xs;  wy = w * ys; wz = w * zs;
    xx = x * xs;  xy = x * ys; xz = x * zs;
    yy = y * ys;  yz = y * zs; zz = z * zs;
	
    m.elem[0][0] = 1.0f - (yy + zz);
    m.elem[0][1] = xy + wz;
    m.elem[0][2] = xz - wy;
	
    m.elem[1][0] = xy - wz;
    m.elem[1][1] = 1.0f - (zz + xx);
    m.elem[1][2] = yz + wx;
	
    m.elem[2][0] = xz + wy;
    m.elem[2][1] = yz - wx;
    m.elem[2][2] = 1.0f - (xx + yy);
	
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
    return m;
}

GMatrix GMatrix::select33(void) const{
	
    GMatrix m1(false);
    m1=*this;
    m1.elem[3][0]= m1.elem[3][1]= m1.elem[3][2]
		=m1.elem[0][3]= m1.elem[1][3]= m1.elem[2][3]=0;
    m1.elem[3][3]= 1;
    return(m1);
};

GMatrix GMatrix::scale(const GFloat& f) const {
    GMatrix m1(false);
	m1.elem[0][1] = m1.elem[0][2] = m1.elem[0][3] =
		m1.elem[1][0] = m1.elem[1][2] = m1.elem[1][3] =
		m1.elem[2][0] = m1.elem[2][1] = m1.elem[2][3] =
		m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[3][3] = 1;
    m1.elem[0][0]=m1.elem[1][1]=m1.elem[2][2]=f;
#ifdef GMATRIXTRANSPOSE
    return m1 * (*this);
#else
    return (*this) *m1;
#endif
}

GMatrix GMatrix::scale(const GVector& v) const {
    GMatrix m1(false);
	m1.elem[0][1] = m1.elem[0][2] = m1.elem[0][3] =
		m1.elem[1][0] = m1.elem[1][2] = m1.elem[1][3] =
		m1.elem[2][0] = m1.elem[2][1] = m1.elem[2][3] =
		m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[3][3] = 1;
    m1.elem[0][0]=v.x;
    m1.elem[1][1]=v.y;
    m1.elem[2][2]=v.z;
#ifdef GMATRIXTRANSPOSE
    return m1 * (*this);
#else
    return (*this) *m1;
#endif
}

GMatrix GMatrix::rotate(const GVector& v) const {
    GMatrix m1(false);
    GFloat sinx = (GFloat)sin(-v.x);
    GFloat siny = (GFloat)sin(-v.y);
    GFloat sinz = (GFloat)sin(-v.z);
	
    GFloat cosx = (GFloat)cos(v.x);
    GFloat cosy = (GFloat)cos(v.y);
    GFloat cosz = (GFloat)cos(v.z);
	
    m1.elem[0][0] = cosy * cosz;
    m1.elem[1][0] = cosy * sinz;
    m1.elem[2][0] = -siny;
	
    m1.elem[0][1] = cosz * sinx * siny - cosx * sinz;
    m1.elem[1][1] = cosx * cosz + sinx * siny * sinz;
    m1.elem[2][1] = cosy * sinx;
	
    m1.elem[0][2] = cosx * cosz * siny + sinx * sinz;
    m1.elem[1][2] = -(cosz * sinx) + cosx * siny * sinz;
    m1.elem[2][2] = cosx * cosy;
	
    m1.elem[0][3] = m1.elem[1][3] = m1.elem[2][3]
		= m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[3][3] = 1;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

void GMatrix::unity(void) {
    elem[0][1] = elem[0][2] = elem[0][3] =
		elem[1][0] = elem[1][2] = elem[1][3] =
		elem[2][0] = elem[2][1] = elem[2][3] =
		elem[3][0] = elem[3][1] = elem[3][2] = 0;
    elem[0][0] = elem[1][1] = elem[2][2] = elem[3][3] = 1;
}


GMatrix GMatrix::inverse33(void) const {
	GMatrix im(false);
	GFloat a=1.0f/(elem[0][0]*elem[1][1]*elem[2][2]
		+elem[0][1]*elem[1][2]*elem[2][0]
		+elem[0][2]*elem[1][0]*elem[2][1]
		-elem[0][0]*elem[1][2]*elem[2][1]
		-elem[0][1]*elem[1][0]*elem[2][2]
		-elem[0][2]*elem[1][1]*elem[2][0]);
    im.elem[0][0]=(elem[1][1]*elem[2][2]-elem[1][2]*elem[2][1])*a;
    im.elem[0][1]=(elem[0][2]*elem[2][1]-elem[0][1]*elem[2][2])*a;
    im.elem[0][2]=(elem[0][1]*elem[1][2]-elem[0][2]*elem[1][1])*a;
	
    im.elem[1][0]=(elem[1][2]*elem[2][0]-elem[1][0]*elem[2][2])*a;
    im.elem[1][1]=(elem[0][0]*elem[2][2]-elem[0][2]*elem[2][0])*a;
    im.elem[1][2]=(elem[0][2]*elem[1][0]-elem[0][0]*elem[1][2])*a;
	
    im.elem[2][0]=(elem[1][0]*elem[2][1]-elem[1][1]*elem[2][0])*a;
    im.elem[2][1]=(elem[0][1]*elem[2][0]-elem[0][0]*elem[2][1])*a;
    im.elem[2][2]=(elem[0][0]*elem[1][1]-elem[0][1]*elem[1][0])*a;
	im.elem[3][0] = im.elem[3][1] = im.elem[3][2] =
		im.elem[0][3] = im.elem[1][3] = im.elem[2][3] = 0;
	im.elem[3][3] = 1;
	//    im=transpose();
    return im;
}

GMatrix GMatrix::inverse(void) const {
    GMatrix im(false);
    GFloat a0 = elem[0][0];
    GFloat a1 = elem[0][1];
    GFloat a2 = elem[0][2];
    GFloat a3 = elem[0][3];
    GFloat a4 = elem[1][0];
    GFloat a5 = elem[1][1];
    GFloat a6 = elem[1][2];
    GFloat a7 = elem[1][3];
    GFloat a8 = elem[2][0];
    GFloat a9 = elem[2][1];
    GFloat a10 = elem[2][2];
    GFloat a11 = elem[2][3];
    GFloat a12 = elem[3][0];
    GFloat a13 = elem[3][1];
    GFloat a14 = elem[3][2];
    GFloat a15 = elem[3][3];
    GFloat a1a11a14a4 = a1*a11*a14*a4;
    GFloat a1a10a15a4 = a1*a10*a15*a4;
    GFloat a11a13a2a4 = a11*a13*a2*a4;
    GFloat a10a13a3a4 = a10*a13*a3*a4;
    GFloat a0a11a14a5 = a0*a11*a14*a5;
    GFloat a0a10a15a5 = a0*a10*a15*a5;
    GFloat a11a12a2a5 = a11*a12*a2*a5;
    GFloat a10a12a3a5 = a10*a12*a3*a5;
    GFloat a1a11a12a6 = a1*a11*a12*a6;
    GFloat a0a11a13a6 = a0*a11*a13*a6;
    GFloat a1a10a12a7 = a1*a10*a12*a7;
    GFloat a0a10a13a7 = a0*a10*a13*a7;
    GFloat a15a2a5a8 = a15*a2*a5*a8;
    GFloat a14a3a5a8 = a14*a3*a5*a8;
    GFloat a1a15a6a8 = a1*a15*a6*a8;
    GFloat a13a3a6a8 = a13*a3*a6*a8;
    GFloat a1a14a7a8 = a1*a14*a7*a8;
    GFloat a13a2a7a8 = a13*a2*a7*a8;
    GFloat a15a2a4a9 = a15*a2*a4*a9;
    GFloat a14a3a4a9 = a14*a3*a4*a9;
    GFloat a0a15a6a9 = a0*a15*a6*a9;
    GFloat a12a3a6a9 = a12*a3*a6*a9;
    GFloat a0a14a7a9 = a0*a14*a7*a9;
    GFloat a12a2a7a9 = a12*a2*a7*a9;
    GFloat a11a14a5 = a11*a14*a5;
    GFloat a10a15a5 = a10*a15*a5;
    GFloat a11a13a6 = a11*a13*a6;
    GFloat a10a13a7 = a10*a13*a7;
    GFloat a15a6a9 = a15*a6*a9;
    GFloat a14a7a9 = a14*a7*a9;
    GFloat a1a11a14 = a1*a11*a14;
    GFloat a1a10a15 = a1*a10*a15;
    GFloat a11a13a2 = a11*a13*a2;
    GFloat a10a13a3 = a10*a13*a3;
    GFloat a15a2a9 = a15*a2*a9;
    GFloat a14a3a9 = a14*a3*a9;
    GFloat a15a2a5 = a15*a2*a5;
    GFloat a14a3a5 = a14*a3*a5;
    GFloat a1a15a6 = a1*a15*a6;
    GFloat a13a3a6 = a13*a3*a6;
    GFloat a1a14a7 = a1*a14*a7;
    GFloat a13a2a7 = a13*a2*a7;
    GFloat a11a2a5 = a11*a2*a5;
    GFloat a10a3a5 = a10*a3*a5;
    GFloat a1a11a6 = a1*a11*a6;
    GFloat a1a10a7 = a1*a10*a7;
    GFloat a3a6a9 = a3*a6*a9;
    GFloat a2a7a9 = a2*a7*a9;
    GFloat a11a14a4 = a11*a14*a4;
    GFloat a10a15a4 = a10*a15*a4;
    GFloat a11a12a6 = a11*a12*a6;
    GFloat a10a12a7 = a10*a12*a7;
    GFloat a15a6a8 = a15*a6*a8;
    GFloat a14a7a8 = a14*a7*a8;
    GFloat a0a11a14 = a0*a11*a14;
    GFloat a0a10a15 = a0*a10*a15;
    GFloat a11a12a2 = a11*a12*a2;
    GFloat a10a12a3 = a10*a12*a3;
    GFloat a15a2a8 = a15*a2*a8;
    GFloat a14a3a8 = a14*a3*a8;
    GFloat a15a2a4 = a15*a2*a4;
    GFloat a14a3a4 = a14*a3*a4;
    GFloat a0a15a6 = a0*a15*a6;
    GFloat a12a3a6 = a12*a3*a6;
    GFloat a0a14a7 = a0*a14*a7;
    GFloat a12a2a7 = a12*a2*a7;
    GFloat a11a2a4 = a11*a2*a4;
    GFloat a10a3a4 = a10*a3*a4;
    GFloat a0a11a6 = a0*a11*a6;
    GFloat a0a10a7 = a0*a10*a7;
    GFloat a3a6a8 = a3*a6*a8;
    GFloat a2a7a8 = a2*a7*a8;
    GFloat a11a13a4 = a11*a13*a4;
    GFloat a11a12a5 = a11*a12*a5;
    GFloat a15a5a8 = a15*a5*a8;
    GFloat a13a7a8 = a13*a7*a8;
    GFloat a15a4a9 = a15*a4*a9;
    GFloat a12a7a9 = a12*a7*a9;
    GFloat a1a11a12 = a1*a11*a12;
    GFloat a0a11a13 = a0*a11*a13;
    GFloat a1a15a8 = a1*a15*a8;
    GFloat a13a3a8 = a13*a3*a8;
    GFloat a0a15a9 = a0*a15*a9;
    GFloat a12a3a9 = a12*a3*a9;
    GFloat a1a15a4 = a1*a15*a4;
    GFloat a13a3a4 = a13*a3*a4;
    GFloat a0a15a5 = a0*a15*a5;
    GFloat a12a3a5 = a12*a3*a5;
    GFloat a1a12a7 = a1*a12*a7;
    GFloat a0a13a7 = a0*a13*a7;
    GFloat a1a11a4 = a1*a11*a4;
    GFloat a0a11a5 = a0*a11*a5;
    GFloat a3a5a8 = a3*a5*a8;
    GFloat a1a7a8 = a1*a7*a8;
    GFloat a3a4a9 = a3*a4*a9;
    GFloat a0a7a9 = a0*a7*a9;
    GFloat a10a13a4 = a10*a13*a4;
    GFloat a10a12a5 = a10*a12*a5;
    GFloat a14a5a8 = a14*a5*a8;
    GFloat a13a6a8 = a13*a6*a8;
    GFloat a14a4a9 = a14*a4*a9;
    GFloat a12a6a9 = a12*a6*a9;
    GFloat a1a10a12 = a1*a10*a12;
    GFloat a0a10a13 = a0*a10*a13;
    GFloat a1a14a8 = a1*a14*a8;
    GFloat a13a2a8 = a13*a2*a8;
    GFloat a0a14a9 = a0*a14*a9;
    GFloat a12a2a9 = a12*a2*a9;
    GFloat a1a14a4 = a1*a14*a4;
    GFloat a13a2a4 = a13*a2*a4;
    GFloat a0a14a5 = a0*a14*a5;
    GFloat a12a2a5 = a12*a2*a5;
    GFloat a1a12a6 = a1*a12*a6;
    GFloat a0a13a6 = a0*a13*a6;
    GFloat a1a10a4 = a1*a10*a4;
    GFloat a0a10a5 = a0*a10*a5;
    GFloat a2a5a8 = a2*a5*a8;
    GFloat a1a6a8 = a1*a6*a8;
    GFloat a2a4a9 = a2*a4*a9;
    GFloat a0a6a9 = a0*a6*a9;
	
    GFloat det = a1a11a14a4 - a1a10a15a4 - a11a13a2a4 + a10a13a3a4 - 
		a0a11a14a5 + a0a10a15a5 + a11a12a2a5 - a10a12a3a5 - 
		a1a11a12a6 + a0a11a13a6 + a1a10a12a7 - a0a10a13a7 - 
		a15a2a5a8 + a14a3a5a8 + a1a15a6a8 - a13a3a6a8 - 
		a1a14a7a8 + a13a2a7a8 + a15a2a4a9 - a14a3a4a9 - 
	       a0a15a6a9 + a12a3a6a9 + a0a14a7a9 - a12a2a7a9;
	
    im.elem[0][0] = (-a11a14a5 + a10a15a5 + a11a13a6 -a10a13a7 - a15a6a9 + a14a7a9)/det;
    im.elem[0][1] = ( a1a11a14 - a1a10a15 - a11a13a2 + a10a13a3 + a15a2a9 - a14a3a9)/det;
    im.elem[0][2] = (-a15a2a5 + a14a3a5 + a1a15a6 - a13a3a6 - a1a14a7 + a13a2a7)/det;
    im.elem[0][3] = (a11a2a5 - a10a3a5 - a1a11a6 + a1a10a7 + a3a6a9 - a2a7a9)/det; 
    im.elem[1][0] = (a11a14a4 - a10a15a4 - a11a12a6 + a10a12a7 + a15a6a8 - a14a7a8)/det;
    im.elem[1][1] = (-a0a11a14 + a0a10a15 + a11a12a2 - a10a12a3 - a15a2a8 + a14a3a8)/det;
    im.elem[1][2] = (a15a2a4 - a14a3a4 - a0a15a6+ a12a3a6 + a0a14a7 - a12a2a7)/det;
    im.elem[1][3] = (-a11a2a4 + a10a3a4 + a0a11a6 - a0a10a7 - a3a6a8 + a2a7a8)/det; 
    im.elem[2][0] = (-a11a13a4 + a11a12a5 - a15a5a8 + a13a7a8 + a15a4a9 - a12a7a9)/det;
    im.elem[2][1] = (- a1a11a12 + a0a11a13 + a1a15a8 - a13a3a8 - a0a15a9 + a12a3a9)/det;
    im.elem[2][2] = (-a1a15a4 + a13a3a4 + a0a15a5 - a12a3a5 + a1a12a7 - a0a13a7)/det;
    im.elem[2][3] = (a1a11a4 - a0a11a5 + a3a5a8 - a1a7a8 - a3a4a9 + a0a7a9)/det;
    im.elem[3][0] = (a10a13a4 - a10a12a5 + a14a5a8 -a13a6a8 - a14a4a9 + a12a6a9)/det;
    im.elem[3][1] = (a1a10a12 - a0a10a13 - a1a14a8 + a13a2a8 + a0a14a9 - a12a2a9)/det;
    im.elem[3][2] = (a1a14a4 - a13a2a4 - a0a14a5 + a12a2a5 - a1a12a6 + a0a13a6)/det;
    im.elem[3][3] = (-a1a10a4 + a0a10a5 - a2a5a8 + a1a6a8 + a2a4a9 - a0a6a9)/det;
	
    return(im);
}
void GMatrix::inverse(GMatrix &im) const {
    GFloat a0 = elem[0][0];
    GFloat a1 = elem[0][1];
    GFloat a2 = elem[0][2];
    GFloat a3 = elem[0][3];
    GFloat a4 = elem[1][0];
    GFloat a5 = elem[1][1];
    GFloat a6 = elem[1][2];
    GFloat a7 = elem[1][3];
    GFloat a8 = elem[2][0];
    GFloat a9 = elem[2][1];
    GFloat a10 = elem[2][2];
    GFloat a11 = elem[2][3];
    GFloat a12 = elem[3][0];
    GFloat a13 = elem[3][1];
    GFloat a14 = elem[3][2];
    GFloat a15 = elem[3][3];
    GFloat a1a11a14a4 = a1*a11*a14*a4;
    GFloat a1a10a15a4 = a1*a10*a15*a4;
    GFloat a11a13a2a4 = a11*a13*a2*a4;
    GFloat a10a13a3a4 = a10*a13*a3*a4;
    GFloat a0a11a14a5 = a0*a11*a14*a5;
    GFloat a0a10a15a5 = a0*a10*a15*a5;
    GFloat a11a12a2a5 = a11*a12*a2*a5;
    GFloat a10a12a3a5 = a10*a12*a3*a5;
    GFloat a1a11a12a6 = a1*a11*a12*a6;
    GFloat a0a11a13a6 = a0*a11*a13*a6;
    GFloat a1a10a12a7 = a1*a10*a12*a7;
    GFloat a0a10a13a7 = a0*a10*a13*a7;
    GFloat a15a2a5a8 = a15*a2*a5*a8;
    GFloat a14a3a5a8 = a14*a3*a5*a8;
    GFloat a1a15a6a8 = a1*a15*a6*a8;
    GFloat a13a3a6a8 = a13*a3*a6*a8;
    GFloat a1a14a7a8 = a1*a14*a7*a8;
    GFloat a13a2a7a8 = a13*a2*a7*a8;
    GFloat a15a2a4a9 = a15*a2*a4*a9;
    GFloat a14a3a4a9 = a14*a3*a4*a9;
    GFloat a0a15a6a9 = a0*a15*a6*a9;
    GFloat a12a3a6a9 = a12*a3*a6*a9;
    GFloat a0a14a7a9 = a0*a14*a7*a9;
    GFloat a12a2a7a9 = a12*a2*a7*a9;
    GFloat a11a14a5 = a11*a14*a5;
    GFloat a10a15a5 = a10*a15*a5;
    GFloat a11a13a6 = a11*a13*a6;
    GFloat a10a13a7 = a10*a13*a7;
    GFloat a15a6a9 = a15*a6*a9;
    GFloat a14a7a9 = a14*a7*a9;
    GFloat a1a11a14 = a1*a11*a14;
    GFloat a1a10a15 = a1*a10*a15;
    GFloat a11a13a2 = a11*a13*a2;
    GFloat a10a13a3 = a10*a13*a3;
    GFloat a15a2a9 = a15*a2*a9;
    GFloat a14a3a9 = a14*a3*a9;
    GFloat a15a2a5 = a15*a2*a5;
    GFloat a14a3a5 = a14*a3*a5;
    GFloat a1a15a6 = a1*a15*a6;
    GFloat a13a3a6 = a13*a3*a6;
    GFloat a1a14a7 = a1*a14*a7;
    GFloat a13a2a7 = a13*a2*a7;
    GFloat a11a2a5 = a11*a2*a5;
    GFloat a10a3a5 = a10*a3*a5;
    GFloat a1a11a6 = a1*a11*a6;
    GFloat a1a10a7 = a1*a10*a7;
    GFloat a3a6a9 = a3*a6*a9;
    GFloat a2a7a9 = a2*a7*a9;
    GFloat a11a14a4 = a11*a14*a4;
    GFloat a10a15a4 = a10*a15*a4;
    GFloat a11a12a6 = a11*a12*a6;
    GFloat a10a12a7 = a10*a12*a7;
    GFloat a15a6a8 = a15*a6*a8;
    GFloat a14a7a8 = a14*a7*a8;
    GFloat a0a11a14 = a0*a11*a14;
    GFloat a0a10a15 = a0*a10*a15;
    GFloat a11a12a2 = a11*a12*a2;
    GFloat a10a12a3 = a10*a12*a3;
    GFloat a15a2a8 = a15*a2*a8;
    GFloat a14a3a8 = a14*a3*a8;
    GFloat a15a2a4 = a15*a2*a4;
    GFloat a14a3a4 = a14*a3*a4;
    GFloat a0a15a6 = a0*a15*a6;
    GFloat a12a3a6 = a12*a3*a6;
    GFloat a0a14a7 = a0*a14*a7;
    GFloat a12a2a7 = a12*a2*a7;
    GFloat a11a2a4 = a11*a2*a4;
    GFloat a10a3a4 = a10*a3*a4;
    GFloat a0a11a6 = a0*a11*a6;
    GFloat a0a10a7 = a0*a10*a7;
    GFloat a3a6a8 = a3*a6*a8;
    GFloat a2a7a8 = a2*a7*a8;
    GFloat a11a13a4 = a11*a13*a4;
    GFloat a11a12a5 = a11*a12*a5;
    GFloat a15a5a8 = a15*a5*a8;
    GFloat a13a7a8 = a13*a7*a8;
    GFloat a15a4a9 = a15*a4*a9;
    GFloat a12a7a9 = a12*a7*a9;
    GFloat a1a11a12 = a1*a11*a12;
    GFloat a0a11a13 = a0*a11*a13;
    GFloat a1a15a8 = a1*a15*a8;
    GFloat a13a3a8 = a13*a3*a8;
    GFloat a0a15a9 = a0*a15*a9;
    GFloat a12a3a9 = a12*a3*a9;
    GFloat a1a15a4 = a1*a15*a4;
    GFloat a13a3a4 = a13*a3*a4;
    GFloat a0a15a5 = a0*a15*a5;
    GFloat a12a3a5 = a12*a3*a5;
    GFloat a1a12a7 = a1*a12*a7;
    GFloat a0a13a7 = a0*a13*a7;
    GFloat a1a11a4 = a1*a11*a4;
    GFloat a0a11a5 = a0*a11*a5;
    GFloat a3a5a8 = a3*a5*a8;
    GFloat a1a7a8 = a1*a7*a8;
    GFloat a3a4a9 = a3*a4*a9;
    GFloat a0a7a9 = a0*a7*a9;
    GFloat a10a13a4 = a10*a13*a4;
    GFloat a10a12a5 = a10*a12*a5;
    GFloat a14a5a8 = a14*a5*a8;
    GFloat a13a6a8 = a13*a6*a8;
    GFloat a14a4a9 = a14*a4*a9;
    GFloat a12a6a9 = a12*a6*a9;
    GFloat a1a10a12 = a1*a10*a12;
    GFloat a0a10a13 = a0*a10*a13;
    GFloat a1a14a8 = a1*a14*a8;
    GFloat a13a2a8 = a13*a2*a8;
    GFloat a0a14a9 = a0*a14*a9;
    GFloat a12a2a9 = a12*a2*a9;
    GFloat a1a14a4 = a1*a14*a4;
    GFloat a13a2a4 = a13*a2*a4;
    GFloat a0a14a5 = a0*a14*a5;
    GFloat a12a2a5 = a12*a2*a5;
    GFloat a1a12a6 = a1*a12*a6;
    GFloat a0a13a6 = a0*a13*a6;
    GFloat a1a10a4 = a1*a10*a4;
    GFloat a0a10a5 = a0*a10*a5;
    GFloat a2a5a8 = a2*a5*a8;
    GFloat a1a6a8 = a1*a6*a8;
    GFloat a2a4a9 = a2*a4*a9;
    GFloat a0a6a9 = a0*a6*a9;
	
    GFloat det = a1a11a14a4 - a1a10a15a4 - a11a13a2a4 + a10a13a3a4 - 
		a0a11a14a5 + a0a10a15a5 + a11a12a2a5 - a10a12a3a5 - 
		a1a11a12a6 + a0a11a13a6 + a1a10a12a7 - a0a10a13a7 - 
		a15a2a5a8 + a14a3a5a8 + a1a15a6a8 - a13a3a6a8 - 
		a1a14a7a8 + a13a2a7a8 + a15a2a4a9 - a14a3a4a9 - 
	       a0a15a6a9 + a12a3a6a9 + a0a14a7a9 - a12a2a7a9;
	
    im.elem[0][0] = (-a11a14a5 + a10a15a5 + a11a13a6 -a10a13a7 - a15a6a9 + a14a7a9)/det;
    im.elem[0][1] = ( a1a11a14 - a1a10a15 - a11a13a2 + a10a13a3 + a15a2a9 - a14a3a9)/det;
    im.elem[0][2] = (-a15a2a5 + a14a3a5 + a1a15a6 - a13a3a6 - a1a14a7 + a13a2a7)/det;
    im.elem[0][3] = (a11a2a5 - a10a3a5 - a1a11a6 + a1a10a7 + a3a6a9 - a2a7a9)/det; 
    im.elem[1][0] = (a11a14a4 - a10a15a4 - a11a12a6 + a10a12a7 + a15a6a8 - a14a7a8)/det;
    im.elem[1][1] = (-a0a11a14 + a0a10a15 + a11a12a2 - a10a12a3 - a15a2a8 + a14a3a8)/det;
    im.elem[1][2] = (a15a2a4 - a14a3a4 - a0a15a6+ a12a3a6 + a0a14a7 - a12a2a7)/det;
    im.elem[1][3] = (-a11a2a4 + a10a3a4 + a0a11a6 - a0a10a7 - a3a6a8 + a2a7a8)/det; 
    im.elem[2][0] = (-a11a13a4 + a11a12a5 - a15a5a8 + a13a7a8 + a15a4a9 - a12a7a9)/det;
    im.elem[2][1] = (- a1a11a12 + a0a11a13 + a1a15a8 - a13a3a8 - a0a15a9 + a12a3a9)/det;
    im.elem[2][2] = (-a1a15a4 + a13a3a4 + a0a15a5 - a12a3a5 + a1a12a7 - a0a13a7)/det;
    im.elem[2][3] = (a1a11a4 - a0a11a5 + a3a5a8 - a1a7a8 - a3a4a9 + a0a7a9)/det;
    im.elem[3][0] = (a10a13a4 - a10a12a5 + a14a5a8 -a13a6a8 - a14a4a9 + a12a6a9)/det;
    im.elem[3][1] = (a1a10a12 - a0a10a13 - a1a14a8 + a13a2a8 + a0a14a9 - a12a2a9)/det;
    im.elem[3][2] = (a1a14a4 - a13a2a4 - a0a14a5 + a12a2a5 - a1a12a6 + a0a13a6)/det;
    im.elem[3][3] = (-a1a10a4 + a0a10a5 - a2a5a8 + a1a6a8 + a2a4a9 - a0a6a9)/det;
}

GMatrix GMatrix::transpose(void) const {
    GMatrix m(false);
	m.elem[0][0] = elem[0][0];
	m.elem[0][1] = elem[1][0];
	m.elem[0][2] = elem[2][0];
	m.elem[0][3] = elem[3][0];
	m.elem[1][0] = elem[0][1];
	m.elem[1][1] = elem[1][1];
	m.elem[1][2] = elem[2][1];
	m.elem[1][3] = elem[3][1];
	m.elem[2][0] = elem[0][2];
	m.elem[2][1] = elem[1][2];
	m.elem[2][2] = elem[2][2];
	m.elem[2][3] = elem[3][2];
	m.elem[3][0] = elem[0][3];
	m.elem[3][1] = elem[1][3];
	m.elem[3][2] = elem[2][3];
	m.elem[3][3] = elem[3][3];
    return(m);
}
void GMatrix::transpose(GMatrix &m) const {
	m.elem[0][0] = elem[0][0];
	m.elem[0][1] = elem[1][0];
	m.elem[0][2] = elem[2][0];
	m.elem[0][3] = elem[3][0];
	m.elem[1][0] = elem[0][1];
	m.elem[1][1] = elem[1][1];
	m.elem[1][2] = elem[2][1];
	m.elem[1][3] = elem[3][1];
	m.elem[2][0] = elem[0][2];
	m.elem[2][1] = elem[1][2];
	m.elem[2][2] = elem[2][2];
	m.elem[2][3] = elem[3][2];
	m.elem[3][0] = elem[0][3];
	m.elem[3][1] = elem[1][3];
	m.elem[3][2] = elem[2][3];
	m.elem[3][3] = elem[3][3];
}

bool GMatrix::operator==(const GMatrix& m) const {
	if(elem[0][0] != m.elem[0][0]) return false;
	if(elem[0][1] != m.elem[0][1]) return false;
	if(elem[0][2] != m.elem[0][2]) return false;
	if(elem[0][3] != m.elem[0][3]) return false;
	if(elem[1][0] != m.elem[1][0]) return false;
	if(elem[1][1] != m.elem[1][1]) return false;
	if(elem[1][2] != m.elem[1][2]) return false;
	if(elem[1][3] != m.elem[1][3]) return false;
	if(elem[2][0] != m.elem[2][0]) return false;
	if(elem[2][1] != m.elem[2][1]) return false;
	if(elem[2][2] != m.elem[2][2]) return false;
	if(elem[2][3] != m.elem[2][3]) return false;
	if(elem[3][0] != m.elem[3][0]) return false;
	if(elem[3][1] != m.elem[3][1]) return false;
	if(elem[3][2] != m.elem[3][2]) return false;
	if(elem[3][3] != m.elem[3][3]) return false;
	return true;
}

bool GMatrix::operator!=(const GMatrix& m) const {
	if(elem[0][0] != m.elem[0][0]) return true;
	if(elem[0][1] != m.elem[0][1]) return true;
	if(elem[0][2] != m.elem[0][2]) return true;
	if(elem[0][3] != m.elem[0][3]) return true;
	if(elem[1][0] != m.elem[1][0]) return true;
	if(elem[1][1] != m.elem[1][1]) return true;
	if(elem[1][2] != m.elem[1][2]) return true;
	if(elem[1][3] != m.elem[1][3]) return true;
	if(elem[2][0] != m.elem[2][0]) return true;
	if(elem[2][1] != m.elem[2][1]) return true;
	if(elem[2][2] != m.elem[2][2]) return true;
	if(elem[2][3] != m.elem[2][3]) return true;
	if(elem[3][0] != m.elem[3][0]) return true;
	if(elem[3][1] != m.elem[3][1]) return true;
	if(elem[3][2] != m.elem[3][2]) return true;
	if(elem[3][3] != m.elem[3][3]) return true;
	return false;
}

GMatrix GMatrix::operator+(const GMatrix& m) const {
    GMatrix m1(false);
	m1.elem[0][0] = elem[0][0] + m.elem[0][0];
	m1.elem[0][1] = elem[0][1] + m.elem[0][1];
	m1.elem[0][2] = elem[0][2] + m.elem[0][2];
	m1.elem[0][3] = elem[0][3] + m.elem[0][3];
	m1.elem[1][0] = elem[1][0] + m.elem[1][0];
	m1.elem[1][1] = elem[1][1] + m.elem[1][1];
	m1.elem[1][2] = elem[1][2] + m.elem[1][2];
	m1.elem[1][3] = elem[1][3] + m.elem[1][3];
	m1.elem[2][0] = elem[2][0] + m.elem[2][0];
	m1.elem[2][1] = elem[2][1] + m.elem[2][1];
	m1.elem[2][2] = elem[2][2] + m.elem[2][2];
	m1.elem[2][3] = elem[2][3] + m.elem[2][3];
	m1.elem[3][0] = elem[3][0] + m.elem[3][0];
	m1.elem[3][1] = elem[3][1] + m.elem[3][1];
	m1.elem[3][2] = elem[3][2] + m.elem[3][2];
	m1.elem[3][3] = elem[3][3] + m.elem[3][3];
	return(m1);
}

GMatrix GMatrix::operator-(const GMatrix& m) const {
    GMatrix m1(false);
	m1.elem[0][0] = elem[0][0] - m.elem[0][0];
	m1.elem[0][1] = elem[0][1] - m.elem[0][1];
	m1.elem[0][2] = elem[0][2] - m.elem[0][2];
	m1.elem[0][3] = elem[0][3] - m.elem[0][3];
	m1.elem[1][0] = elem[1][0] - m.elem[1][0];
	m1.elem[1][1] = elem[1][1] - m.elem[1][1];
	m1.elem[1][2] = elem[1][2] - m.elem[1][2];
	m1.elem[1][3] = elem[1][3] - m.elem[1][3];
	m1.elem[2][0] = elem[2][0] - m.elem[2][0];
	m1.elem[2][1] = elem[2][1] - m.elem[2][1];
	m1.elem[2][2] = elem[2][2] - m.elem[2][2];
	m1.elem[2][3] = elem[2][3] - m.elem[2][3];
	m1.elem[3][0] = elem[3][0] - m.elem[3][0];
	m1.elem[3][1] = elem[3][1] - m.elem[3][1];
	m1.elem[3][2] = elem[3][2] - m.elem[3][2];
	m1.elem[3][3] = elem[3][3] - m.elem[3][3];
	return(m1);
}


GMatrix GMatrix::translate(const GVector& v) const {
    GMatrix m1(false);
	m1.elem[0][1] = m1.elem[0][2] = m1.elem[0][3] =
		m1.elem[1][0] = m1.elem[1][2] = m1.elem[1][3] =
		m1.elem[2][0] = m1.elem[2][1] = m1.elem[2][3] = 0;
	m1.elem[0][0] = m1.elem[1][1] = m1.elem[2][2] = m1.elem[3][3] = 1;

    m1.elem[3][0] = v.x;
    m1.elem[3][1] = v.y;
    m1.elem[3][2] = v.z;
#ifdef GMATRIXTRANSPOSE
     return m1.transpose() * (*this);
#else
    return (*this) *m1;
#endif
}

GMatrix GMatrix::rotateX(const GFloat angle) const {
    GMatrix m1(false);
    GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
	m1.elem[0][1] = m1.elem[0][2] = m1.elem[0][3] =
		m1.elem[1][0] = m1.elem[1][3] =
		m1.elem[2][0] = m1.elem[2][3] =
		m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[0][0] = m1.elem[3][3] = 1;

    m1.elem[1][1] = c;
    m1.elem[2][2] = c;
    m1.elem[2][1] = s;
    m1.elem[1][2] = -s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return  *this *m1;
#endif
}

GMatrix GMatrix::rotateY(const GFloat angle) const {
    GMatrix m1(false);
    GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
	m1.elem[0][1] = m1.elem[0][3] =
		m1.elem[1][0] = m1.elem[1][2] = m1.elem[1][3] =
		m1.elem[2][1] = m1.elem[2][3] =
		m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[1][1] = m1.elem[3][3] = 1;

    m1.elem[0][0] = c;
    m1.elem[2][2] = c;
    m1.elem[2][0] = -s;
    m1.elem[0][2] = s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

GMatrix GMatrix::rotateZ(const GFloat angle) const {
    GMatrix m1(false);
    GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
	m1.elem[0][2] = m1.elem[0][3] =
		m1.elem[1][2] = m1.elem[1][3] =
		m1.elem[2][0] = m1.elem[2][1] = m1.elem[2][3] =
		m1.elem[3][0] = m1.elem[3][1] = m1.elem[3][2] = 0;
	m1.elem[2][2] = m1.elem[3][3] = 1;

	m1.elem[0][0] = c;
    m1.elem[1][1] = c;
    m1.elem[1][0] = s;
    m1.elem[0][1] = -s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}
//行列からオイラー角を求める
void GMatrix::getEular( GFloat & yaw, GFloat & pitch, GFloat & roll )
{
	GVector  y_axis, z_axis;
	z_axis.x = elem[2][0];//m[2];
	z_axis.y = elem[2][1];//m[5];
	z_axis.z = elem[2][2];//m[8];
	y_axis.x = elem[1][0];//m[1];
	y_axis.y = elem[1][1];//m[4];
	y_axis.z = elem[1][2];//m[7];

	yaw = atan2( z_axis.x, z_axis.z );

	float  cos_yaw = cos( yaw );
	pitch = atan2( cos_yaw * z_axis.y, fabs( z_axis.z ) );

	float  sin_yaw = sin( yaw );
	float  cos_pitch = cos( pitch );
	roll = atan2( cos_pitch * ( sin_yaw * y_axis.z - cos_yaw * y_axis.x ), y_axis.y );
}

GMatrix GMatrix::operator*(const GFloat& f) const {
    GMatrix m(false);
	m.elem[0][0] *= f;
	m.elem[0][1] *= f;
	m.elem[0][2] *= f;
	m.elem[0][3] *= f;
	m.elem[1][0] *= f;
	m.elem[1][1] *= f;
	m.elem[1][2] *= f;
	m.elem[1][3] *= f;
	m.elem[2][0] *= f;
	m.elem[2][1] *= f;
	m.elem[2][2] *= f;
	m.elem[2][3] *= f;
	m.elem[3][0] *= f;
	m.elem[3][1] *= f;
	m.elem[3][2] *= f;
	m.elem[3][3] *= f;
	return m;
}


GMatrix::GMatrix(const GMatrix& m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[0][3] = m.elem[0][3];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[1][3] = m.elem[1][3];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
	elem[2][3] = m.elem[2][3];
	elem[3][0] = m.elem[3][0];
	elem[3][1] = m.elem[3][1];
	elem[3][2] = m.elem[3][2];
	elem[3][3] = m.elem[3][3];
}

GMatrix& GMatrix::operator=(const GMatrix& m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[0][3] = m.elem[0][3];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[1][3] = m.elem[1][3];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
	elem[2][3] = m.elem[2][3];
	elem[3][0] = m.elem[3][0];
	elem[3][1] = m.elem[3][1];
	elem[3][2] = m.elem[3][2];
	elem[3][3] = m.elem[3][3];
	return(*this);
}
GMatrix& GMatrix::operator=(const GMatrix33& m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
	elem[0][3]=elem[1][3]=elem[2][3]=elem[3][0]=elem[3][1]=elem[3][2]=0;
	elem[3][3]=1;
	return(*this);
}

GMatrix& GMatrix::operator+=(const GMatrix& m) {
	elem[0][0] += m.elem[0][0];
	elem[0][1] += m.elem[0][1];
	elem[0][2] += m.elem[0][2];
	elem[0][3] += m.elem[0][3];
	elem[1][0] += m.elem[1][0];
	elem[1][1] += m.elem[1][1];
	elem[1][2] += m.elem[1][2];
	elem[1][3] += m.elem[1][3];
	elem[2][0] += m.elem[2][0];
	elem[2][1] += m.elem[2][1];
	elem[2][2] += m.elem[2][2];
	elem[2][3] += m.elem[2][3];
	elem[3][0] += m.elem[3][0];
	elem[3][1] += m.elem[3][1];
	elem[3][2] += m.elem[3][2];
	elem[3][3] += m.elem[3][3];
	return(*this);
}

GMatrix& GMatrix::operator-=(const GMatrix& m) {
	elem[0][0] -= m.elem[0][0];
	elem[0][1] -= m.elem[0][1];
	elem[0][2] -= m.elem[0][2];
	elem[0][3] -= m.elem[0][3];
	elem[1][0] -= m.elem[1][0];
	elem[1][1] -= m.elem[1][1];
	elem[1][2] -= m.elem[1][2];
	elem[1][3] -= m.elem[1][3];
	elem[2][0] -= m.elem[2][0];
	elem[2][1] -= m.elem[2][1];
	elem[2][2] -= m.elem[2][2];
	elem[2][3] -= m.elem[2][3];
	elem[3][0] -= m.elem[3][0];
	elem[3][1] -= m.elem[3][1];
	elem[3][2] -= m.elem[3][2];
	elem[3][3] -= m.elem[3][3];
	return(*this);
}

GMatrix& GMatrix::operator*=(const GMatrix& m) {
    GMatrix m1(false);
    for (register int i = 0; i < 4; i++) 
		for (register int j = 0; j < 4; j++) {
			m1.elem[i][j] = 0.0;
			for (register int k = 0; k < 4; k++)
				m1.elem[i][j] += (elem[i][k] * m.elem[k][j]);
		}
		
		*this = m1;
		return(*this);
}


GMatrix& GMatrix::operator*=(const GFloat& f) {
    for (register int i = 0; i < 4; i++)
		for (register int j = 0; j < 4; j++)
			elem[i][j] *= f; 
		return(*this);
}

GMatrix GMatrix::rotate(const GVector& axis, GFloat angle) const {
    GMatrix m(false);
    //クォ－タニオンをセット
    GQuat q=GQuat(axis,angle);
	
    //クォ－タニオンを行列にする
    m=q.matrix();
	
#ifdef GMATRIXTRANSPOSE
    return m.transpose() * (*this);
#else
    return *this *m;
#endif
}
//行列をクォ－タニオンにする
GQuat GMatrix::quat(void) {
	GQuat	q;
	GFloat	tr, w;
	
	tr = elem[0][0] + elem[1][1] + elem[2][2];
	if ( tr>=0 ) {
		w = (GFloat)sqrt(tr+1.0);
		q.w = w/2;
		w = -0.5f/w;
		q.x = (elem[2][1] - elem[1][2]) * w;
		q.y = (elem[0][2] - elem[2][0]) * w;
		q.z = (elem[1][0] - elem[0][1]) * w;
	} else {
		// xx, yy, zz の中で一番大きい要素を探す
		int		i=0;
		{
			GFloat	max=elem[0][0];
			if ( elem[1][1]>max )		{ i=1; max=elem[1][1]; }
			if ( elem[2][2]>max )		{ i=2; }
		}
		
		switch( i ) {
		case 0:
			w = (GFloat)sqrt( elem[0][0] - elem[1][1] - elem[2][2] + 1 );
			q.x = w/2;
			w = 0.5f/w;
			q.y =  (elem[0][1] + elem[1][0]) * w;
			q.z =  (elem[2][0] + elem[0][2]) * w;
			q.w = -(elem[2][1] - elem[1][2]) * w;
			break;
		case 1:
			w = (GFloat)sqrt( elem[1][1] - elem[2][2] - elem[0][0] + 1 );
			q.y = w/2;
			w = 0.5f/w;
			q.z =  (elem[1][2] + elem[2][1]) * w;
			q.x =  (elem[0][1] + elem[1][0]) * w;
			q.w = -(elem[0][2] - elem[2][0]) * w;
			break;
		case 2:
			w = (GFloat)sqrt( elem[2][2] - elem[0][0] - elem[1][1] + 1 );
			q.z = w/2;
			w = 0.5f/w;
			q.x =  (elem[2][0] + elem[0][2]) * w;
			q.y =  (elem[1][2] + elem[2][1]) * w;
			q.w = -(elem[1][0] - elem[0][1]) * w;
			break;
		}
	}
	
	return q;
}

GMatrix GMatrix::viewMatrixR(const GVector &at, const GVector &eye, const GVector &up) const {
	GMatrix m(false);
	GVector U,V,N;
	N = -(eye-at).normalize();
    if(N==GVector(0,0,0)) return m;
	U = -(up.cross(N)).normalize();
	V = (N.cross(U)).normalize();
	// 右手系に変換(Uを-Uにする)
    U=-U;
	m.elem[0][0] = U.x;
	m.elem[1][0] = U.y;
	m.elem[2][0] = U.z;
	m.elem[3][0] = - U.dot(eye);
	m.elem[0][1] = V.x;
	m.elem[1][1] = V.y;
	m.elem[2][1] = V.z;
	m.elem[3][1] = - V.dot(eye);
	m.elem[0][2] = N.x;
	m.elem[1][2] = N.y;
	m.elem[2][2] = N.z;
	m.elem[3][2] = - N.dot(eye);
    m.elem[0][3] = m.elem[1][3] = m.elem[2][3] = 0;
	m.elem[3][3] = 1;
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
	return (m);
}

GMatrix GMatrix::viewMatrixL(const GVector &at, const GVector &eye, const GVector &up) const {
	GMatrix m(false);
	GVector U,V,N;
	N = -(eye-at).normalize();
    if(N==GVector(0,0,0)) return m;
	U = -(up.cross(N)).normalize();
	V = (N.cross(U)).normalize();
	m.elem[0][0] = U.x;
	m.elem[1][0] = U.y;
	m.elem[2][0] = U.z;
	m.elem[3][0] = - U.dot(eye);
	m.elem[0][1] = V.x;
	m.elem[1][1] = V.y;
	m.elem[2][1] = V.z;
	m.elem[3][1] = - V.dot(eye);
	m.elem[0][2] = N.x;
	m.elem[1][2] = N.y;
	m.elem[2][2] = N.z;
	m.elem[3][2] = - N.dot(eye);
    m.elem[0][3] = m.elem[1][3] = m.elem[2][3] = 0;
	m.elem[3][3] = 1;
	
#ifdef GMATRIXTRANSPOSE
    m=m.transpose();
#endif
	return (m);
}



GMatrix GMatrix::projectionMatrix(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const{
	GMatrix m;
    GFloat f;
    f =1.0f/(GFloat)tan(fovy/2.0f);
	m.elem[0][0] = f/aspect;
	m.elem[1][1] = f;
	m.elem[2][2] = -(farZ+nearZ)/(farZ-nearZ);
	m.elem[2][3] = 1;
	m.elem[3][2] = -(2.0f*farZ*nearZ)/(farZ-nearZ);
	m.elem[3][3] = 0;
#ifdef GMATRIXTRANSPOSE
    return m.transpose();
#endif
    return (m);
}

GMatrix GMatrix::projectionMatrixX(GFloat fovy,GFloat aspect,GFloat nearZ,GFloat farZ) const{
	GMatrix m;
    GFloat q,w,h;
	
    w =1.0f/(GFloat)tan(fovy/2.0f);
    h =1.0f/(GFloat)tan(aspect/2.0f);
    q=farZ/(farZ-nearZ);
	m.elem[0][0] = w;
	m.elem[1][1] = h;
	m.elem[2][2] = q;
	m.elem[2][3] = 1;
	m.elem[3][2] = -q*nearZ;
	m.elem[3][3] = 0;
#ifdef GMATRIXTRANSPOSE
    return m.transpose();
#endif
    return (m);
}

GMatrix GMatrix::projectionMatrix(GFloat d) const{
	GMatrix m;
	m.elem[0][0] = 1;
	m.elem[1][1] = 1;
	m.elem[2][2] = 1;
	m.elem[2][3] = 1.0f/d;
	m.elem[3][2] = -d;
	m.elem[3][3] = 0;
#ifdef GMATRIXTRANSPOSE
    return m.transpose();
#endif
    return (m);
}

// 単位行列か調べる
bool GMatrix::isUnity(void) const {
    if (elem[0][0] !=1 || elem[0][1] !=0 || elem[0][2] !=0 || elem[0][3] !=0 ) return false;
    if (elem[1][0] !=0 || elem[1][1] !=1 || elem[1][2] !=0 || elem[1][3] !=0 ) return false;
    if (elem[2][0] !=0 || elem[2][1] !=0 || elem[2][2] !=1 || elem[2][3] !=0 ) return false;
    if (elem[3][0] !=0 || elem[3][1] !=0 || elem[3][2] !=0 || elem[3][3] !=1 ) return false;
    return true;
}

//

GMatrix33 GMatrix33::scale(const GFloat& f) const {
    GMatrix33 m1;
    m1.elem[0][0]=f;
    m1.elem[1][1]=f;
    m1.elem[2][2]=f;
    return(m1);
}

GMatrix33 GMatrix33::scale(const GVector& v) const {
    GMatrix33 m1;
    m1.elem[0][0]=v.x;
    m1.elem[1][1]=v.y;
    m1.elem[2][2]=v.z;
    return(m1);
}

GMatrix33 GMatrix33::rotate(const GVector& v) const {
    GMatrix33 m1(false);
    GFloat sinx = (GFloat)sin(-v.x);
    GFloat siny = (GFloat)sin(-v.y);
    GFloat sinz = (GFloat)sin(-v.z);
	
    GFloat cosx = (GFloat)cos(v.x);
    GFloat cosy = (GFloat)cos(v.y);
    GFloat cosz = (GFloat)cos(v.z);
	
    m1.elem[0][0] = cosy * cosz;
    m1.elem[1][0] = cosy * sinz;
    m1.elem[2][0] = -siny;
	
    m1.elem[0][1] = cosz * sinx * siny - cosx * sinz;
    m1.elem[1][1] = cosx * cosz + sinx * siny * sinz;
    m1.elem[2][1] = cosy * sinx;
	
    m1.elem[0][2] = cosx * cosz * siny + sinx * sinz;
    m1.elem[1][2] = -(cosz * sinx) + cosx * siny * sinz;
    m1.elem[2][2] = cosx * cosy;
	
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

void GMatrix33::unity(void) {
    elem[0][1] = elem[0][2] =
		elem[1][0] = elem[1][2] =
		elem[2][0] = elem[2][1] =
		elem[3][0] = elem[3][1] = 0;
    elem[0][0] = elem[1][1] = elem[2][2] = 1;
}


GMatrix33 GMatrix33::inverse(void) const {
	GMatrix33 im(false);
	GFloat a=1.0f/(elem[0][0]*elem[1][1]*elem[2][2]
		+elem[0][1]*elem[1][2]*elem[2][0]
		+elem[0][2]*elem[1][0]*elem[2][1]
		-elem[0][0]*elem[1][2]*elem[2][1]
		-elem[0][1]*elem[1][0]*elem[2][2]
		-elem[0][2]*elem[1][1]*elem[2][0]);
    im.elem[0][0]=(elem[1][1]*elem[2][2]-elem[1][2]*elem[2][1])*a;
    im.elem[0][1]=(elem[0][2]*elem[2][1]-elem[0][1]*elem[2][2])*a;
    im.elem[0][2]=(elem[0][1]*elem[1][2]-elem[0][2]*elem[1][1])*a;
	
    im.elem[1][0]=(elem[1][2]*elem[2][0]-elem[1][0]*elem[2][2])*a;
    im.elem[1][1]=(elem[0][0]*elem[2][2]-elem[0][2]*elem[2][0])*a;
    im.elem[1][2]=(elem[0][2]*elem[1][0]-elem[0][0]*elem[1][2])*a;
	
    im.elem[2][0]=(elem[1][0]*elem[2][1]-elem[1][1]*elem[2][0])*a;
    im.elem[2][1]=(elem[0][1]*elem[2][0]-elem[0][0]*elem[2][1])*a;
    im.elem[2][2]=(elem[0][0]*elem[1][1]-elem[0][1]*elem[1][0])*a;
	//    im=transpose();
    return im;
}

void GMatrix33::inverse(GMatrix33 &im) const {
	GFloat a=1.0f/(elem[0][0]*elem[1][1]*elem[2][2]
		+elem[0][1]*elem[1][2]*elem[2][0]
		+elem[0][2]*elem[1][0]*elem[2][1]
		-elem[0][0]*elem[1][2]*elem[2][1]
		-elem[0][1]*elem[1][0]*elem[2][2]
		-elem[0][2]*elem[1][1]*elem[2][0]);
    im.elem[0][0]=(elem[1][1]*elem[2][2]-elem[1][2]*elem[2][1])*a;
    im.elem[0][1]=(elem[0][2]*elem[2][1]-elem[0][1]*elem[2][2])*a;
    im.elem[0][2]=(elem[0][1]*elem[1][2]-elem[0][2]*elem[1][1])*a;
	
    im.elem[1][0]=(elem[1][2]*elem[2][0]-elem[1][0]*elem[2][2])*a;
    im.elem[1][1]=(elem[0][0]*elem[2][2]-elem[0][2]*elem[2][0])*a;
    im.elem[1][2]=(elem[0][2]*elem[1][0]-elem[0][0]*elem[1][2])*a;
	
    im.elem[2][0]=(elem[1][0]*elem[2][1]-elem[1][1]*elem[2][0])*a;
    im.elem[2][1]=(elem[0][1]*elem[2][0]-elem[0][0]*elem[2][1])*a;
    im.elem[2][2]=(elem[0][0]*elem[1][1]-elem[0][1]*elem[1][0])*a;
}


GMatrix33 GMatrix33::transpose(void) const {
    GMatrix33 m(false);
	m.elem[0][0] = elem[0][0];
	m.elem[0][1] = elem[1][0];
	m.elem[0][2] = elem[2][0];
	m.elem[1][0] = elem[0][1];
	m.elem[1][1] = elem[1][1];
	m.elem[1][2] = elem[2][1];
	m.elem[2][0] = elem[0][2];
	m.elem[2][1] = elem[1][2];
	m.elem[2][2] = elem[2][2];
    return(m);
}

bool GMatrix33::operator==(const GMatrix33& m) const {
    for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (elem[i][j] != m.elem[i][j])
				return (false);
		}
    }
    return (true);
}

bool GMatrix33::operator!=(const GMatrix33& m) const {
    for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (elem[i][j] != m.elem[i][j])
				return (true);
		}
    }
    return (false);
}

GMatrix33 GMatrix33::operator+(const GMatrix33& m) const {
    GMatrix33 m1(false);
    for (register int i = 0; i < 3; i++)
		for (register int j = 0; j < 3; j++)
			m1.elem[i][j] = elem[i][j] + m.elem[i][j];
		return(m1);
}

GMatrix33 GMatrix33::operator-(const GMatrix33& m) const {
    GMatrix33 m1(false);
    for (register int i = 0; i < 3; i++)
		for (register int j = 0; j < 3; j++)
			m1.elem[i][j] = elem[i][j] - m.elem[i][j];
		return(m1);
}

GMatrix33 GMatrix33::rotateX(const GFloat angle) const {
    GMatrix33 m1(false);
	m1.elem[0][1] = m1.elem[0][2] =
		m1.elem[1][0] =
		m1.elem[2][0] = 0;
	m1.elem[0][0] = 1;
	GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
    m1.elem[1][1] = m1.elem[2][2] = c;
    m1.elem[2][1] = s;
    m1.elem[1][2] = -s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

GMatrix33 GMatrix33::rotateY(const GFloat angle) const {
    GMatrix33 m1(false);
	m1.elem[0][1] = 
		m1.elem[1][0] = m1.elem[1][2] =
		m1.elem[2][1] = 0;
	m1.elem[1][1] = 1;
    GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
    m1.elem[0][0] = m1.elem[2][2] = c;
    m1.elem[2][0] = -s;
    m1.elem[0][2] = s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

GMatrix33 GMatrix33::rotateZ(const GFloat angle) const {
    GMatrix33 m1(false);
	m1.elem[0][2] =
		m1.elem[1][2] =
		m1.elem[2][0] = m1.elem[2][1] = 0;
	m1.elem[2][2] = 1;
    GFloat c = (GFloat)cos(angle);
    GFloat s = (GFloat)sin(-angle);
    m1.elem[0][0] = m1.elem[1][1] = c;
    m1.elem[1][0] = s;
    m1.elem[0][1] = -s;
#ifdef GMATRIXTRANSPOSE
    return m1.transpose() * (*this);
#else
    return *this *m1;
#endif
}

GMatrix::GMatrix(const GMatrix33 m) {
		elem[0][0] = m.elem[0][0]; elem[0][1] = m.elem[0][1]; elem[0][2] = m.elem[0][2];
		elem[1][0] = m.elem[1][0]; elem[1][1] = m.elem[1][1]; elem[1][2] = m.elem[1][2];
		elem[2][0] = m.elem[2][0]; elem[2][1] = m.elem[2][1]; elem[2][2] = m.elem[2][2];
		elem[3][0] = elem[3][1] = elem[3][2] =
			elem[0][3] = elem[1][3] = elem[2][3] = 0;
		elem[3][3] = 1;
	}
GMatrix33 GMatrix33::operator*(const GFloat& f) const {
    GMatrix m(false);
	m.elem[0][0] = elem[0][0]*f;
	m.elem[0][1] = elem[0][1]*f;
	m.elem[0][2] = elem[0][2]*f;
	m.elem[1][0] = elem[1][0]*f;
	m.elem[1][1] = elem[1][1]*f;
	m.elem[1][2] = elem[1][2]*f;
	m.elem[2][0] = elem[2][0]*f;
	m.elem[2][1] = elem[2][1]*f;
	m.elem[2][2] = elem[2][2]*f;
	return(m);
}


GMatrix33::GMatrix33(const GMatrix m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
}

GMatrix33& GMatrix33::operator=(const GMatrix33& m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
	return(*this);
}
GMatrix33& GMatrix33::operator=(const GMatrix& m) {
	elem[0][0] = m.elem[0][0];
	elem[0][1] = m.elem[0][1];
	elem[0][2] = m.elem[0][2];
	elem[1][0] = m.elem[1][0];
	elem[1][1] = m.elem[1][1];
	elem[1][2] = m.elem[1][2];
	elem[2][0] = m.elem[2][0];
	elem[2][1] = m.elem[2][1];
	elem[2][2] = m.elem[2][2];
	return(*this);
}

GMatrix33& GMatrix33::operator+=(const GMatrix33& m) {
    for (register int i = 0; i < 3; i++) 
		for (register int j = 0; j < 3; j++)
			elem[i][j] += m.elem[i][j];
		return(*this);
}

GMatrix33& GMatrix33::operator-=(const GMatrix33& m) {
    for (register int i = 0; i < 3; i++) 
		for (register int j = 0; j < 3; j++)
			elem[i][j] -= m.elem[i][j];
		return(*this);
}

GMatrix33& GMatrix33::operator*=(const GMatrix33& m) {
    GMatrix33 m1(false);
    for (register int i = 0; i < 3; i++) 
		for (register int j = 0; j < 3; j++) {
			m1.elem[i][j] = 0.0;
			for (register int k = 0; k < 4; k++)
				m1.elem[i][j] += (elem[i][k] * m.elem[k][j]);
		}
		
		*this = m1;
		return(*this);
}


GMatrix33& GMatrix33::operator*=(const GFloat& f) {
    for (register int i = 0; i < 3; i++)
		for (register int j = 0; j < 3; j++)
			elem[i][j] *= f; 
		return(*this);
}

GMatrix33 GMatrix33::rotate(const GVector& axis, GFloat angle) const {
    GMatrix33 m(false),m1(false);
    //クォ－タニオンをセット
    GQuat q=GQuat(axis,angle);
	
    //クォ－タニオンを行列にする
    m=q.matrix33();
	
#ifdef GMATRIXTRANSPOSE
    return m * (*this);
#else
    return  (*this) *m;
#endif
}

//行列からオイラー角を求める
void GMatrix33::getEular( GFloat & yaw, GFloat & pitch, GFloat & roll )
{
	GVector  y_axis, z_axis;
	z_axis.x = elem[2][0];//m[2];
	z_axis.y = elem[2][1];//m[5];
	z_axis.z = elem[2][2];//m[8];
	y_axis.x = elem[1][0];//m[1];
	y_axis.y = elem[1][1];//m[4];
	y_axis.z = elem[1][2];//m[7];

	yaw = atan2( z_axis.x, z_axis.z );

	float  cos_yaw = cos( yaw );
	pitch = atan2( cos_yaw * z_axis.y, fabs( z_axis.z ) );

	float  sin_yaw = sin( yaw );
	float  cos_pitch = cos( pitch );
	roll = atan2( cos_pitch * ( sin_yaw * y_axis.z - cos_yaw * y_axis.x ), y_axis.y );
}
//行列をクォ－タニオンにする
GQuat GMatrix33::quat(void) {
	GQuat	q;
	GFloat	tr, w;
	
	tr = elem[0][0] + elem[1][1] + elem[2][2];
	if ( tr>=0 ) {
		w = (GFloat)sqrt(tr+1.0);
		q.w = w/2;
		w = -0.5f/w;
		q.x = (elem[2][1] - elem[1][2]) * w;
		q.y = (elem[0][2] - elem[2][0]) * w;
		q.z = (elem[1][0] - elem[0][1]) * w;
	} else {
		int		i=0;
		// xx, yy, zz の中で一番大きい要素を探す
		{
			GFloat	max=elem[0][0];
			if ( elem[1][1]>max )		{ i=1; max=elem[1][1]; }
			if ( elem[2][2]>max )		{ i=2; }
		}
		
		switch( i ) {
		case 0:
			w = (GFloat)sqrt( elem[0][0] - elem[1][1] - elem[2][2] + 1 );
			q.x = w/2;
			w = 0.5f/w;
			q.y =  (elem[0][1] + elem[1][0]) * w;
			q.z =  (elem[2][0] + elem[0][2]) * w;
			q.w = -(elem[2][1] - elem[1][2]) * w;
			break;
		case 1:
			w = (GFloat)sqrt( elem[1][1] - elem[2][2] - elem[0][0] + 1 );
			q.y = w/2;
			w = 0.5f/w;
			q.z =  (elem[1][2] + elem[2][1]) * w;
			q.x =  (elem[0][1] + elem[1][0]) * w;
			q.w = -(elem[0][2] - elem[2][0]) * w;
			break;
		case 2:
			w = (GFloat)sqrt( elem[2][2] - elem[0][0] - elem[1][1] + 1 );
			q.z = w/2;
			w = 0.5f/w;
			q.x =  (elem[2][0] + elem[0][2]) * w;
			q.y =  (elem[1][2] + elem[2][1]) * w;
			q.w = -(elem[1][0] - elem[0][1]) * w;
			break;
		}
	}
	
	return q;
}

// 単位行列か調べる
bool GMatrix33::isUnity(void) const {
    if (elem[0][0] !=1 || elem[0][1] !=0 || elem[0][2] !=0 ) return false;
    if (elem[1][0] !=0 || elem[1][1] !=1 || elem[1][2] !=0 ) return false;
    if (elem[2][0] !=0 || elem[2][1] !=0 || elem[2][2] !=1 ) return false;
    return true;
}