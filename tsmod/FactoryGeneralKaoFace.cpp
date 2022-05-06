
#include "GameDataStruct.h"
#include "OutputDebugStream.h"
// という並び
int getFactoryGeneralKaoFace(int iGeneralFaceBaseType, int iFaceID, int iEye, int iMouse) {
	int iNewBase = 49152; /*0x1100000000000000*/  // まずベース。なぜこの値なのかは不明。
	int iNewType = iGeneralFaceBaseType;
	int iNewFace = iFaceID << 5;
	int iNewEye = iEye << 9;
	int iNewMouse = iMouse << 11;

	return iNewBase | iNewFace | iGeneralFaceBaseType | iNewEye | iNewMouse;
}




// DDrawの.bmpファイルから、オリジナルのgetFactoryGeneralKaoFace相当の番号を得る
// iFaceIDの概念が、天翔記とDDrawでは、XYが逆なので、その変換配列
int iFaceIDReverseArray[] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };

int getDDrawFaceIDToFeneralKaoFaceID(int iDDrawFaceID) {

	// 姫系の顔だ
	if (3001 <= iDDrawFaceID && iDDrawFaceID <= 4000) {
		// これを元来のオリジナル番号に直す
		int HimeNo = iDDrawFaceID - 3001; // ０～の番号として…

		// 姫の中でのindexを求める
		int iGeneralFaceBaseTypeInHime = HimeNo / 256; // 姫１(=0) 姫２(1) 姫３(=2) 町娘(=3)

		int iFaceID = HimeNo % 16;
		// DDrawは左→右へ番号を付けてる。しかし、天翔記は上→下へ番号をつけてるので、変換
		iFaceID = iFaceIDReverseArray[iFaceID];
		int iEye = int(HimeNo / 16) % 4; // 目の番号
		int iMouse = int(HimeNo / 64) % 4; // 目の番号

		int iHimeOriginalFaceID = getFactoryGeneralKaoFace( GeneralFaceBaseType::姫一+iGeneralFaceBaseTypeInHime, iFaceID, iEye, iMouse );

		return iHimeOriginalFaceID;
	}
	return 0xFFFF;
}