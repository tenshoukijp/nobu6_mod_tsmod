
#include "GameDataStruct.h"
#include "OutputDebugStream.h"
// �Ƃ�������
int getFactoryGeneralKaoFace(int iGeneralFaceBaseType, int iFaceID, int iEye, int iMouse) {
	int iNewBase = 49152; /*0x1100000000000000*/  // �܂��x�[�X�B�Ȃ����̒l�Ȃ̂��͕s���B
	int iNewType = iGeneralFaceBaseType;
	int iNewFace = iFaceID << 5;
	int iNewEye = iEye << 9;
	int iNewMouse = iMouse << 11;

	return iNewBase | iNewFace | iGeneralFaceBaseType | iNewEye | iNewMouse;
}




// DDraw��.bmp�t�@�C������A�I���W�i����getFactoryGeneralKaoFace�����̔ԍ��𓾂�
// iFaceID�̊T�O���A�V�ċL��DDraw�ł́AXY���t�Ȃ̂ŁA���̕ϊ��z��
int iFaceIDReverseArray[] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };

int getDDrawFaceIDToFeneralKaoFaceID(int iDDrawFaceID) {

	// �P�n�̊炾
	if (3001 <= iDDrawFaceID && iDDrawFaceID <= 4000) {
		// ����������̃I���W�i���ԍ��ɒ���
		int HimeNo = iDDrawFaceID - 3001; // �O�`�̔ԍ��Ƃ��āc

		// �P�̒��ł�index�����߂�
		int iGeneralFaceBaseTypeInHime = HimeNo / 256; // �P�P(=0) �P�Q(1) �P�R(=2) ����(=3)

		int iFaceID = HimeNo % 16;
		// DDraw�͍����E�֔ԍ���t���Ă�B�������A�V�ċL�͏と���֔ԍ������Ă�̂ŁA�ϊ�
		iFaceID = iFaceIDReverseArray[iFaceID];
		int iEye = int(HimeNo / 16) % 4; // �ڂ̔ԍ�
		int iMouse = int(HimeNo / 64) % 4; // �ڂ̔ԍ�

		int iHimeOriginalFaceID = getFactoryGeneralKaoFace( GeneralFaceBaseType::�P��+iGeneralFaceBaseTypeInHime, iFaceID, iEye, iMouse );

		return iHimeOriginalFaceID;
	}
	return 0xFFFF;
}