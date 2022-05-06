#pragma once

#pragma pack(1)

#include <cmath>

// 普通の２次元座標型。
struct D2D_POSITION {
	D2D_POSITION(double _x, double _y) : x(_x),y(_y) {}
	D2D_POSITION() : x(0),y(0) {}
	double x;
	double y;


	D2D_POSITION& operator=(const D2D_POSITION& v);
    //比較演算子
	bool operator==(const D2D_POSITION& v) const;
	bool operator!=(const D2D_POSITION& v) const;
};

// 普通の２次元ベクトル型。
struct D2D_VECTOR {
	//メンバ変数
	double x;
	double y;
	//コンストラクタ
	D2D_VECTOR();
	D2D_VECTOR(double x,double y);
	D2D_VECTOR(D2D_POSITION pos);

	//代入演算子
	D2D_VECTOR& D2D_VECTOR::operator=(const D2D_VECTOR& v);
	//単項演算子
	D2D_VECTOR& operator+=(const D2D_VECTOR& v);
	D2D_VECTOR& operator-=(const D2D_VECTOR& v);
	D2D_VECTOR& operator*=(double k);
	D2D_VECTOR& operator/=(double k);
	D2D_VECTOR operator+()const;
	D2D_VECTOR operator-()const;
	//添え字演算子
	double& operator[](int i);
    //比較演算子
	bool operator==(const D2D_VECTOR& v) const;
	bool operator!=(const D2D_VECTOR& v) const;
	//べクトルの長さ
	double length()const;
	//正規化
	void normalize();
};
//ベクトル演算
//D2D_VECTOR+D2D_VECTOR
D2D_VECTOR operator+(const D2D_VECTOR& u,const D2D_VECTOR& v);
//D2D_VECTOR-D2D_VECTOR
D2D_VECTOR operator-(const D2D_VECTOR& u,const D2D_VECTOR& v);
//double*D2D_VECTOR
D2D_VECTOR operator*(double k,const  D2D_VECTOR& v);
//D2D_VECTOR*double
D2D_VECTOR operator*(const D2D_VECTOR& v,double k);
//D2D_VECTOR/double
D2D_VECTOR operator/(const D2D_VECTOR& v,double k);
//内積 D2D_VECTOR*D2D_VECTOR
double operator*(const D2D_VECTOR& u,const D2D_VECTOR& v);
//外積 D2D_VECTOR%D2D_VECTOR
double operator%(const D2D_VECTOR& u,const D2D_VECTOR& v);
//2つのベクトルのなす角度
double angle(const D2D_VECTOR& u,const D2D_VECTOR& v);
// ２点間座標の差
D2D_VECTOR operator-(D2D_POSITION &p1, D2D_POSITION &p2);