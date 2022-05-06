#include "GameDataStruct.h"



//比較演算子
bool D3D_POSITION::operator==(const D3D_POSITION& v ) const{
    return (x == v.x) && (y == v.y) && (z == v.z);
}
bool D3D_POSITION::operator!=(const D3D_POSITION& v ) const{
    return !(*this == v);
}
//代入演算子
D3D_POSITION& D3D_POSITION::operator=(const D3D_POSITION& v){
	this->x=v.x;
	this->y=v.y;
	this->z=v.z;
	return *this;
}



//コンストラクタ
D3D_VECTOR::D3D_VECTOR() { x = y = z = 0; }

D3D_VECTOR::D3D_VECTOR(double x,double y,double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

D3D_VECTOR::D3D_VECTOR(D3D_POSITION pos) {
	this->x=pos.x;
	this->y=pos.y;
	this->z=pos.z;
}


//代入演算子
D3D_VECTOR& D3D_VECTOR::operator=(const D3D_VECTOR& v){
	this->x=v.x;
	this->y=v.y;
	this->z=v.z;
	return *this;
}
//単項演算子
D3D_VECTOR& D3D_VECTOR::operator+=(const D3D_VECTOR& v){
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}
D3D_VECTOR& D3D_VECTOR::operator-=(const D3D_VECTOR& v){
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}
D3D_VECTOR& D3D_VECTOR::operator*=(double k){
	this->x *= k;
	this->y *= k;
	this->z *= k;
	return *this;
}
D3D_VECTOR& D3D_VECTOR::operator/=(double k){
	this->x /= k;
	this->y /= k;
	this->z /= k;
	return *this;
}
D3D_VECTOR D3D_VECTOR::operator+()const{		//+D3D_VECTOR
	return *this;
}
D3D_VECTOR D3D_VECTOR::operator-()const{		//-D3D_VECTOR
	return D3D_VECTOR(-x,-y,-z);
}
//添え字演算子
double& D3D_VECTOR::operator[](int i){
	if(i == 0){
		return x;
	}
	else if(i == 1){
		return y;
	}
	else if(i == 2){
		return z;
	}
	else{
		return x;
	}
}
//比較演算子
bool D3D_VECTOR::operator==(const D3D_VECTOR& v ) const{
    return (x == v.x) && (y == v.y) && (z == v.z);
}
bool D3D_VECTOR::operator!=(const D3D_VECTOR& v ) const{
    return !(*this == v);
}
//べクトルの長さ
double D3D_VECTOR::length()const{
	return pow(x*x+y*y+z*z,(double)0.5);
}
//正規化
void D3D_VECTOR::normalize(){

	if ( length() == 0 ) {
		MessageBox(NULL, "長さが０のベクトル(=方向が無いベクトル)を単位ベクトル化しようとしました。無限０除算でエラーとなります。","長さが０のベクトル(=方向が無いベクトル)を単位ベクトル化しようとしました。無限０除算でエラーとなります。", NULL);
	}

	*this /= length();
}
//*----------------------グローバル関数の実装--------------------------*//
//二項演算子の定義
//D3D_VECTOR+D3D_VECTOR
D3D_VECTOR operator+(const D3D_VECTOR& u,const D3D_VECTOR& v){
	D3D_VECTOR w;
	w.x=u.x+v.x;
	w.y=u.y+v.y;
	w.z=u.z+v.z;
	return w;
}
//D3D_VECTOR-D3D_VECTOR
D3D_VECTOR operator-(const D3D_VECTOR& u,const D3D_VECTOR& v){
	D3D_VECTOR w;
	w.x=u.x-v.x;
	w.y=u.y-v.y;
	w.z=u.z-v.z;
	return w;
}
//double*D3D_VECTOR
D3D_VECTOR operator*(double k,const  D3D_VECTOR& v){
	return D3D_VECTOR(k*v.x,k*v.y,k*v.z);
}
//D3D_VECTOR*double
D3D_VECTOR operator*(const D3D_VECTOR& v,double k){
	return D3D_VECTOR(v.x*k,v.y*k,v.z*k);
}
//D3D_VECTOR/double
D3D_VECTOR operator/(const D3D_VECTOR& v,double k){
	return D3D_VECTOR(v.x/k,v.y/k,v.z/k);
}
//内積 D3D_VECTOR*D3D_VECTOR
double operator*(const D3D_VECTOR& u,const D3D_VECTOR& v){
	return u.x*v.x+u.y*v.y+u.z*v.z;
}
//外積 D3D_VECTOR%D3D_VECTOR
D3D_VECTOR operator%(const D3D_VECTOR& u,const D3D_VECTOR& v){
	D3D_VECTOR w;
	w.x=u.y*v.z-u.z*v.y;
	w.y=u.z*v.x-u.x*v.z;
	w.z=u.x*v.y-u.y*v.x;
	return w;
}

//２つのベクトルのなす角
double angle(const D3D_VECTOR& u,const D3D_VECTOR& v){
	double cos =u*v/(u.length()*v.length());
	return double(acos(cos)/M_PI*180);
}

//２点間座標の差
D3D_VECTOR operator-(D3D_POSITION &p1, D3D_POSITION &p2) {
	D3D_VECTOR v;
	v.x = p1.x - p2.x;
	v.y = p1.y - p2.y;
	v.z = p1.z - p2.z;
	return v;
}