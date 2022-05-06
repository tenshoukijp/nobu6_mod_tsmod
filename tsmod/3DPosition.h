#pragma once

#pragma pack(1)

#include <cmath>

// 普通の３次元座標型。
struct D3D_POSITION {
	D3D_POSITION(double _x, double _y, double _z) : x(_x),y(_y),z(_z) {}
	D3D_POSITION() : x(0),y(0),z(0) {}
	double x;
	double y;
	double z;

	D3D_POSITION& operator=(const D3D_POSITION& v);
    //比較演算子
	bool operator==(const D3D_POSITION& v) const;
	bool operator!=(const D3D_POSITION& v) const;
};

// 普通の３次元ベクトル型。
struct D3D_VECTOR {
	//メンバ変数
	double x;
	double y;
	double z;

	//コンストラクタ
	D3D_VECTOR();
	D3D_VECTOR(double x,double y,double z);
	D3D_VECTOR(D3D_POSITION pos);

	//代入演算子
	D3D_VECTOR& D3D_VECTOR::operator=(const D3D_VECTOR& v);
	//単項演算子
	D3D_VECTOR& operator+=(const D3D_VECTOR& v);
	D3D_VECTOR& operator-=(const D3D_VECTOR& v);
	D3D_VECTOR& operator*=(double k);
	D3D_VECTOR& operator/=(double k);
	D3D_VECTOR operator+()const;
	D3D_VECTOR operator-()const;
	//添え字演算子
	double& operator[](int i);
    //比較演算子
	bool operator==(const D3D_VECTOR& v) const;
	bool operator!=(const D3D_VECTOR& v) const;
	//べクトルの長さ
	double length()const;
	//正規化
	void normalize();
};
//ベクトル演算
//D3D_VECTOR+D3D_VECTOR
D3D_VECTOR operator+(const D3D_VECTOR& u,const D3D_VECTOR& v);
//D3D_VECTOR-D3D_VECTOR
D3D_VECTOR operator-(const D3D_VECTOR& u,const D3D_VECTOR& v);
//double*D3D_VECTOR
D3D_VECTOR operator*(double k,const  D3D_VECTOR& v);
//D3D_VECTOR*double
D3D_VECTOR operator*(const D3D_VECTOR& v,double k);
//D3D_VECTOR/double
D3D_VECTOR operator/(const D3D_VECTOR& v,double k);
//内積 D3D_VECTOR*D3D_VECTOR
double operator*(const D3D_VECTOR& u,const D3D_VECTOR& v);
//外積 D3D_VECTOR%D3D_VECTOR
D3D_VECTOR operator%(const D3D_VECTOR& u,const D3D_VECTOR& v);
//2つのベクトルのなす角度
double angle(const D3D_VECTOR& u,const D3D_VECTOR& v);
// ２点間座標の差
D3D_VECTOR operator-(D3D_POSITION &p1, D3D_POSITION &p2);