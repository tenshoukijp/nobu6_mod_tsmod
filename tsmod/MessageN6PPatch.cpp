#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "TSMod.h"
#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioDataStruct.h"
#include "ScenarioMessageN6PPatch.h"
#include "MessageN6PTemplate.h"
#include "MessageN6PPatch.h"
#include "MessageInflection.h"
#include "MessageInflectionSurfix.h"
#include "MessageInflectionEnding.h"
#include "OnFireAttackSuccessInBattle.h"
#include "SoundServer.h"
#include "WaitTimeProcedure.h"
#include "FuncPlayResourceWav.h"


using namespace std;




// ���o�����b�Z�[�W���X�V����ăR�s�[���ꂽ�X�^���o�C�^�C�~���O
bool MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;


int isMessageLOCMode = -1; // LOC���S���[�h



MessageN6PPatch::MessageN6PPatch( char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo ) {
	m_iReWritePercentForce = -1;
	m_isCurOrigMsgReWrited = false;
	strcpy_s(m_szCurOrigMsg, szCurOrigMsg);
	m_hOtherInfo = hOtherInfo;

	// szBName�ɂ̓f�t�H���g�Ţ�c������O   �������ƣ�݂����ȕ����������Ă���̂ŁA�c��+���O�����Ƃ���B
	strncpy( m_szBName, szBName, 12 );
	// �����̔��p�󔒂���������
	for ( int i=sizeof(m_szBName)-1; i>4; i--) {
		if (m_szBName[i] == ' ') {
			m_szBName[i] = '\0';
		}
	}
	m_szBName[12]=NULL;
}


char* MessageN6PPatch::getBName() {
	return m_szBName;
}


bool MessageN6PPatch::isBName(char* szTargetName) {
	return (
		strcmp(m_szBName, szTargetName)==0
	);
}


BOOL MessageN6PPatch::isMsg( char* szMsgReg, Matches* matches ) {
	// �Y���̕����p�^�[������v���邩�ǂ���
	return ( 
		// strstr( m_szCurOrigMsg, szMsgReg ) != NULL 
		OnigMatch( m_szCurOrigMsg, szMsgReg, matches )
	);
}


bool MessageN6PPatch::rwMsg( char* szMsgReg, char *szCurReWriteMsg, int iReWritePercent ) {
	// ���łɑ��Ń}�b�`���Ă��珈������K�v���Ȃ��c�͂��c
	if ( m_isCurOrigMsgReWrited && (iReWritePercent != m_iReWritePercentForce) ) { 
		return false;
	}

	// �^�[�Q�b�g�̃��b�Z�[�W�ƍ��v�����
	if ( isMsg( szMsgReg ) ) {

		if ( isMessageLOCMode == 4 ) {
			// LOC4��LOC5���������e�ł���΁A�R�s�[�t���O�𗧂Ă�
			bool bMsgMustCopyTo5Flag = false;
			if ( strcmp( (char *)CPM.getMemoryAddress(LOC4), (char *)CPM.getMemoryAddress(LOC5)) == 0 ) {
				bMsgMustCopyTo5Flag = true; // LOC4��LOC5�̓��b�Z�[�W�������Ȃ̂ŗ����Ƃ����������Ȃ���΂Ȃ�Ȃ��B
			}
			bool bMsgMustCopyTo2Flag = false;
			if ( strcmp( (char *)CPM.getMemoryAddress(LOC4), (char *)CPM.getMemoryAddress(LOC2)) == 0 ) {
				bMsgMustCopyTo2Flag = true; // LOC4��LOC2�̓��b�Z�[�W�������Ȃ̂ŗ����Ƃ����������Ȃ���΂Ȃ�Ȃ��B
			}
			// ��������											�@�@�@�@�����b�Z�[�W�̃������A�h���X
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			if ( bMsgMustCopyTo5Flag ) {
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC5)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			}
			if ( bMsgMustCopyTo2Flag ) {
				WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC2)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			}
		}
		else if ( isMessageLOCMode == 2 ) {

			// ��������											�@�@�@�@�����b�Z�[�W�̃������A�h���X
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC4)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(CPM.getMemoryAddress(LOC2)), szCurReWriteMsg, strlen(szCurReWriteMsg)+1, NULL);
		}
		strcpy(CPM.getPreVarPtr(LOC1) ,szCurReWriteMsg);
		m_isCurOrigMsgReWrited = true;

		return true;
	}
	return false;
}


// �t�ÎE���b�Z�[�W���[�h
extern int iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute;
extern int iOrgBushouIDOfAnsatsuSareruGawa;
extern int iOrgBushouIDOfAnsatsuSuruGawa;

// ���l�����ɏ��l�������l�̕���ID(-1���Ďg������)
extern int iCurBushouIDInCommerceMessageJudge;
// ���l�����ɏ��l�ɑΏ��l�v�̃��b�Z�[�W�����킹�邩�ǂ���
extern bool isUseSyoninMsgInCommerceSuccessJudge;

// �����ō����������l���ɂ���đS�������̃��b�Z�[�W��ύX����
extern int iSendouKonranTargetConranCnt;
// �����ō������������(���{�l)�ƂȂ镐��ID
extern int iSendouKonranMultiKonranBushouID;

bool isCurScenarioModReWrite = false;




extern char szMessageBuildLastestWordName[];
extern int isOnCurGungiFukidashi;		// ����̐����o������]�裐����o�����ǂ����B�B
extern int isOnCurGunshiFukidashi;		// �R�t�����̐����o�����ǂ���
extern int iTeppouHanekaesiInTeppouDeath; // �S�C��e���Ԃ������[�h���ǂ����B

// ���C���̏�������
bool ReWritePatchMessageN6P(char *szBName, char *szCurOrigMsg, HOtherInfo* hOtherInfo) {

	// �R�t�����̐����o���͂�����ʂ����ʂ̏ꏊ������B
	if ( isOnCurGunshiFukidashi ) {
		isOnCurGunshiFukidashi = FALSE;
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;
		return false;
	}

	// �]�萁���o���t���O�������Ă�΁A�����ł͉������Ȃ��B���������Ă��\���ɊԂɍ���Ȃ����炾�B
	if ( isOnCurGungiFukidashi ) {
		isOnCurGungiFukidashi = FALSE;
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;
		return false;
	}

	isMessageLOCMode = 4; // �f�t�H���g��4

	/*
	 * ���t���[����������Əd���̂ŁA�����Ő���
	 * ���Ȃ�̃t���[���^�C�~���O�ł����ɂȂ����̂ŁA�����ŐV���ȃ��b�Z�[�W���Z�b�g����Ă��邩�ǂ������`�F�b�N��
	 * �Z�b�g����Ă��Ȃ���΁A���ɏ�������K�v���Ȃ��Ɣ��f����B
	 */
	
	if ( nb6fukidashirelatepersons->i1stPersonID <= 0 || 532 < nb6fukidashirelatepersons->i1stPersonID ) { // 530�Ԃ܂ł����ʂ̕���
		return false;
	}

	// �R�s�[�^�C�~���O�̃��b�Z�[�W�𗘗p���Ă��Ȃ��Ȃ�΁c
	if ( ! MessageN6PPatch::isOnTenshouExeMessageCopyUsed ) {

		// �Y�����b�Z�[�W�𗘗p�����Ƃ����t���O�𗧂Ă�
		MessageN6PPatch::isOnTenshouExeMessageCopyUsed = true;

		// ScenarioMod�������������Ƃ������͂����Ń��Z�b�g�B
		isCurScenarioModReWrite = false;

	// ���łɃR�s�[�^�C�~���O�̃��b�Z�[�W�𗘗p���Ă����Ȃ�΁A�������������Ԃ�
	} else {
		return false;
	}


	// �����Ŗ��O�����o���B
	char name[256] = "";

	if ( nb6fukidashirelatepersons->i1stPersonID <= 532 ) {
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].familyname);
		strcat( name, nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname);
	}


	/*	
	ofsClassMessageN6PPatch << "-------------------------------------------------------" << endl;

	// �ȉ��e�X�g�p�B��P�l�ҁA��Q�l�ҁA��R�l��
	nb6fukidashirelatepersons->i1stPersonID;
	nb6fukidashirelatepersons->i2ndPersonID;
	nb6fukidashirelatepersons->i3rdPersonID;
	ofsClassMessageN6PPatch << nb6bushouname[nb6fukidashirelatepersons->i1stPersonID-1].fastname << endl;
	if (  nb6fukidashirelatepersons->i2ndPersonID <= 0 || 532 < nb6fukidashirelatepersons->i2ndPersonID ) {
	} else {
		ofsClassMessageN6PPatch << nb6bushouname[nb6fukidashirelatepersons->i2ndPersonID-1].fastname << ":" << nb6bushouname[nb6fukidashirelatepersons->i2ndPersonID-1].fastname  << endl;
	}
	ofsClassMessageN6PPatch << "Main4" << name << ":" << CPM.getCurVarPtr(LOC4) << endl;
	ofsClassMessageN6PPatch << "Main4" << name << ":" << CPM.getCurVarPtr(LOC5) << endl;

	/*
	 * �T�u�̕�(���l�Ȃ�)���`�F�b�N
	 */

	/*
	 * �ȉ��A���낢��Ə󋵂𔻒f���A���b�Z�[�W�u�������B
	 */

	/*
		������������������
		������������������
		������������������
	*/
	// �ǂ������b�Z�[�W�S�̓p�����^�^�C�v�ŁA���b�Z�[�W�Q�ɂ��̓W�J���ꂽ���̂�����悤���c
	if ( isTheParamMsgIsIncludeVerifyMsg( (char *)CPM.getCurVarPtr(LOC2), (char *)CPM.getCurVarPtr(LOC4) ) ) {
		strcpy( szCurOrigMsg, (char *)CPM.getCurVarPtr(LOC2) );
		strcpy( (char *)CPM.getCurVarPtr(LOC4), (char *)CPM.getCurVarPtr(LOC2) );
		// �W�J�ς݂̂��̂�W�J�O�̃p�����[�^��Ԃ̂��̂ɏ㏑�����Ă��܂��B
		isMessageLOCMode = 2;	// ���b�Z�[�W�i�[�ꏊ�̂Q���������[�h
	}

	MessageN6PPatch oCurMsg = MessageN6PPatch( name, szCurOrigMsg,   hOtherInfo );

	/*
	ofsClassMessageN6PPatch << "4" << CPM.getCurVarPtr(LOC4) << endl;
	ofsClassMessageN6PPatch << "2" << CPM.getCurVarPtr(LOC2) << endl;
	*/

	/*
	 * ���l�Ƃ̌��BOnCommerceBenzetsuJudge,OnCommerceSuccessJudge�֘A
	 */ 
	// �����l�Ƃ̌��������珑�������� 
	if (isUseSyoninMsgInCommerceSuccessJudge && oCurMsg.isMsg("�󂩂�ʒ��c���̒l��")) {
		isUseSyoninMsgInCommerceSuccessJudge = false;

		char szCommerseMsg1[70] = "%s�a�Ƃ�\x0A�����l�̒��c���̒l��\x0A���ł��܂��傤";
		char szCommerseMsg2[70] = "";
		wsprintf(szCommerseMsg2, szCommerseMsg1, nb6bushouname[iCurBushouIDInCommerceMessageJudge-1].fastname);
		oCurMsg.rwMsg( "�󂩂�ʒ��c���̒l��", szCommerseMsg2 );
	}

	/*
	 * �΍U�ߌn�B���b�Z�[�W�u�������ƂƂ��ɁA������������炷�BOnFireBattleAttackSuccessInBattle�n
	 */
	// ���ʁB�ΐӂ߂ł���΁c
	else if ( oCurMsg.isMsg(  �����o���E�B���h�E::�푈::�΍U��::REG01 ) ||
		 oCurMsg.isMsg(  �����o���E�B���h�E::�푈::�΍U��::REG02 ) ||
		 oCurMsg.isMsg(  �����o���E�B���h�E::�푈::�΍U��::REG03 ) ||
		 oCurMsg.isMsg(  �����o���E�B���h�E::�푈::�΍U��::REG04 ) ) {

		 // ���̒��ł̓��b�Z�[�W�̏����������s��Ȃ�����!! �^���^���X�������������Ȃ��Ȃ邽��!!
		 // �΍U�ߐ������̏���
		 OnTenshouExeFireBattleAttackSuccessExecute();
	}

	// ����������A�G��������퓬���S�Z�\�g����̃t���O���n�m�������珈��������B
	else if ( oCurMsg.isMsg (  �����o���E�B���h�E::�푈::����::REG01 ) || oCurMsg.isMsg(  �����o���E�B���h�E::�푈::����::REG02 ) ) {
		// ���������̕��o���E�B���h�E�ł���΁c
		EXSuperService_IfCanAllAlibityInBattleSeiryokuChange( nb6fukidashirelatepersons->i1stPersonID-1 , nb6fukidashirelatepersons->i2ndPersonID-1 );
	}


	/*
	 * �����n�B�����̓��e��ω��������̂ŁA����ɑΉ����郁�b�Z�[�W�BOnSendouKonran****�n�B
	 */
	// �����֘A
	// �����������̂��V���ȏ�
	if ( iSendouKonranTargetConranCnt >= 7 ) {

		// �ǂꂩ�̃��b�Z�[�W�͊m���Ȃ̂Œu��������
		char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
		int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
		int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;
		char szAnsatsuMsg[256] = "";
		/*
		�i����=1or3or�����j�e�X���A���f�߂����
		�i����=2�j�҂ǂ��A���f�����
		�i���j�F�̎ҁA���f�����
		*/
		// �����X���b�g��^���^���X
		if ( oCurMsg.isBName("�C�D�E�@�A�B" ) ) {
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01,		"�ق��c�s�ςȂ�\x0A�틵���m���Ɍ��ɂ�\x0A�e�Ɍ��j����" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02,		"�悵�I�@������\x0A��p�I�]�@���I\x0A�G�̍����̌���˂��I" );

		} else if ( iBushouTone==1 || iBushouTone==3 || iBushouSex== 1 ) {
			wsprintf( szAnsatsuMsg, "�F�����藧���Ă����\x0A�e�X���A���f�߂����" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "���͂͑卬������c\x0A�Ƃ͂���\x0A���Ƃ����낵����̂�" );

		} else if ( iBushouTone==2 ) {
			wsprintf( szAnsatsuMsg, "�F�����藧���Ă����\x0A�҂ǂ��A���f�����" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "���͂͑卬������c\x0A�Ƃ͂���\x0A���Ƃ����낵����̂�" );

		} else {
			wsprintf( szAnsatsuMsg, "�F�����藧���Ă����\x0A�F�̎ҁA���f�����" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, szAnsatsuMsg);
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "���͂͑卬������c\x0A�Ƃ͂���\x0A���Ƃ����낵����̂�" );
		}

	// �����������̂��Q���ȏ�
	} else if ( iSendouKonranTargetConranCnt >= 1 ) {

		// �����X���b�g��^���^���X
		if ( oCurMsg.isBName("�C�D�E�@�A�B" ) ) {
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01,		"�ӂ�\x0A������Ȃ����Ƃ��c\x0A�����|���ɂ�����I" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02,		"���͋S���Ȃ�\x0A���̋@�ɏ悶\x0A�G�̍����̌���˂��I" );
		
		} else {
			// �����͐����������A�����������l���Ƃ��Ă͏���
			// �ǂꂩ�̃��b�Z�[�W�͊m���Ȃ̂Œu��������
			char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
			int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
			int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;
			char szAnsatsuMsg[256] = "";

			if ( iBushouSex==1 ) { 
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�悵�I�@�����\x0A���ʂ����Ȃ�\x0A�Ȃ����悤�ł���" );
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�悵�I\x0A���ꂼ�]�@��I\x0A���̍����̌���˂��I" );
			} else if ( iBushouTone==2 || iBushouTone==4) {
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�悵�I�@�����\x0A���ʂ����Ȃ�\x0A�Ȃ����悤����" );
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�悵�I\x0A���ꂼ�]�@���I\x0A���̍����̌���˂��I" );
			} else {
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�悵�I�@�����\x0A���ʂ����Ȃ�\x0A�Ȃ����悤�����" );
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�悵�I\x0A���ꂼ�]�@����I\x0A���̍����̌���˂��I" );
			}
		}

	// �����������̂��O��
	} else {
		// �����X���b�g��^���^���X
		if ( oCurMsg.isBName("�C�D�E�@�A�B" ) ) {
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01,		"�ނށc\x0A�����ł���\x0A���肪����ʂ�" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02,		"�ނށc\x0A�����ł���\x0A���肪����ʂ�" );

		} else {
			// �ǂꂩ�̃��b�Z�[�W�͊m���Ȃ̂Œu��������
			char *pBushouName = nb6bushouname[iSendouKonranMultiKonranBushouID-1].fastname;
			int iBushouSex = nb6bushouname[iSendouKonranMultiKonranBushouID-1].sex;
			int iBushouTone = nb6bushouref[iSendouKonranMultiKonranBushouID-1].tone;

			if ( iBushouTone==1 || iBushouTone==3 || iBushouSex== 1 ) {
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�ނށc\x0A�����ł���\x0A���肪����܂���");
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�ނށc\x0A�����ł���\x0A���肪����܂���");

			} else if ( iBushouTone==2 ) {
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�ނށc\x0A�����ł���\x0A���肪�����");
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�ނށc\x0A�����ł���\x0A���肪�����");

			} else {
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, "�ނށc\x0A�����ł���\x0A���肪����ʂ�");
				oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, "�ނށc\x0A�����ł���\x0A���肪����ʂ�");
			}
		}

	}

	// �S�C�̃��b�Z�[�W���̃��b�Z�[�W�ɕύX����B
	if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
		int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;
		if ( nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1 ) {

			string msg = string("��̂�׎˂���\x0A�ւ��ẮA����") + nb6bushouname[iBushouID].fastname + string("\x0A���𑈂��r�O����");
			char szmsg[100] = "";
			strcpy(szmsg, msg.c_str() );
			oCurMsg.rwMsg( "�V���������������\x0A",	szmsg );
			oCurMsg.rwMsg( "�̂��̂��o�ė�����\x0A",	"�|����̈З�\x0A�v���m�点�Ă���I\x0A���Ă����I" );
			oCurMsg.rwMsg( "�܂�����c������\x0A",		"�܂�����c������\x0A�\���Ɉ������āc\x0A�悵�A���Ă����I" );
			oCurMsg.rwMsg( "�킴�킴�W�I�ɂȂ��\x0A",	"�킴�킴��̓I��\x0A�Ȃ�ɂ���������c\x0A���Ă����I" );
			oCurMsg.rwMsg( "�e�̗���\x0A",				"��̉J��\x0A�������Ă�����\x0A��Ăɕ��Ă����I" );
		}

	}

	// �{�ۗ���̃��b�Z�[�W�B�V���[�g�J�b�g�ŁB
	if ( TSModCommand::AllCastle::cmdShortCutFallCastle ) {
		if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( Is_CastleWar() ) {
				int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;
				extern int iCheckRojoBushouNum; // �ď鑤�̏���ɋ���l��

				if ( oCurMsg.isMsg( �����o���E�B���h�E::�푈::����::REG01) || 
					 oCurMsg.isMsg( �����o���E�B���h�E::�푈::����::REG02) ||
					 oCurMsg.isMsg( �����o���E�B���h�E::�푈::����::REG03) ) {
					HEX_POSITION my2DPos = GetCastleHexUnitPos(iBushouID); // �����̈ʒu
					int iRole = GetCastleHexRole(my2DPos); // ���̕����̖���
					if ( iRole != CASTLE_HEX_ROLE::�{�� ) { // �{�ۈȊO�ɋ����̂ɗ��邵���B
	
						Matches matches;
						if ( oCurMsg.isMsg("(�{�ۈ�ԏ�肶��)\\s(.+)", &matches ) ) {
							string s1 = "�G�ɐ키�͂Ȃ�����\x0A";
							string s2 = matches[2];
							string reg01 = s1+s2;
							oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG01, (char *)reg01.c_str() );
						} else if ( oCurMsg.isMsg("(�{�ۈ�ԏ��̖��_��)\\s(.+?\x0A)(.+)", &matches ) ) {
							extern int iNextTurnCastleID;   // �\���ď�풆�̏�ԍ�(+1�̕�)
							int iCastleID = iNextTurnCastleID-1;
							string s1 = "���͂⏟���͌�����\x0A";
							string s2 = matches[2];
							string s3 = "���̏�A���Ղ����I";
							if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
								s3 = string("") + nb6castle[iCastleID].name + getCastleTypeName(nb6castle[iCastleID].title) + "�A�������I";
							}
							string reg02 = s1+s2+s3;
							oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG02, (char *)reg02.c_str() );
						} else if ( oCurMsg.isMsg("�{�ۈ�ԏ�肼�[���I") ) {
							string reg03;
							if ( rand()%2 == 0 ) {
								reg03 = ���͂͂́�(iBushouID,iBushouID,0xFFFF) + "\x0A" + "�G�ɐ키�͂Ȃ�\x0A" + "��͉��̂��̂���";
							} else {
								reg03 = string("�G�͍~�Q�����悤����\x0A") + "��͂���" + nb6bushouname[iBushouID].fastname + "��\x0A" + "����" + �����܂���(iBushouID, iBushouID, 0xFFFF);
							}
							oCurMsg.rwMsg( �����o���E�B���h�E::�푈::����::REG03, (char *)reg03.c_str() );
							oCurMsg.rwMsg( "�����Ƃ���\x0A�ЂƂ܂��傫��", (char *)reg03.c_str() );
							oCurMsg.rwMsg( "����̏p\x0A�J�Ⴕ��", (char *)reg03.c_str() );
							oCurMsg.rwMsg( "���܂Ŋ��������ƂȂ�\x0A�V������", (char *)reg03.c_str() );
						}
					}
				}
			}
		}
	}


	/*
	 * �����X���b�g�^���^���X�̑��̏󋵂ł̃��b�Z�[�W�u������
	 */
	// �����X���b�g��^���^���X
	if ( oCurMsg.isBName("�C�D�E�@�A�B" ) ) {

		/*
		 * �^���^���X���S�C�𒵂˕Ԃ��ꎀ�S����
		 */
		if ( iTeppouHanekaesiInTeppouDeath == 1 ) {
			iTeppouHanekaesiInTeppouDeath = 0;
			oCurMsg.rwMsg( "�ށA���O�c\x0A",		"�Ȃ�ƁI�@�S�C��e��\x0A���ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );
			oCurMsg.rwMsg( "�����Ŏ��ʂ�\x0A",		"�Ȃ�ƁI�@�S�C��e��\x0A���ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );
			oCurMsg.rwMsg( "����قǂ̋��|��\x0A",		"�Ȃ�ƁI�@�S�C��e��\x0A���ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );
			oCurMsg.rwMsg( "�ɗ�����������\x0A",		"�Ȃ�ƁI�@�S�C��e��\x0A���ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );
			oCurMsg.rwMsg( "���͕��a�Ȏ����\x0A",		"�Ȃ�ƁI�@�S�C��e��\x0A���ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );

		/*
		 * �^���^���X���S�C�𒵂˕Ԃ������̃Z���t
		 */
		} else if ( iTeppouHanekaesiInTeppouDeath == 2 ) {
			iTeppouHanekaesiInTeppouDeath = 0;
			oCurMsg.rwMsg( "���̐���\x0A",		"���ʂȂ��Ƃ��c\x0A���̈Í��R�m�c��\x0A�S�C�ł͓|���ʁI" );
			oCurMsg.rwMsg( "�񂾂��ƂȂ�ʂĂ���\x0A",		"����𖯂̖��H���I\x0A�S�C���Ƃ��ɖ��S��\x0A����ȂĖłт�̂��c" );
			oCurMsg.rwMsg( "�������ɐ��������Ƃ�\x0A",		"�ӂӂӁc�_���͈����Ȃ�\x0A�����g�̏�ɍ����\x0A���݂͐g��S�ڂ��̂�" );
		}


		// �ە��n�̃��b�Z�[�W
		if ( oCurMsg.isMsg( "���щΎR�̊���\x0A�Ƃ���" ) ) {
			if ( rand()%2 == 0 ) {
				oCurMsg.rwMsg( "���щΎR�̊���\x0A�Ƃ���",		"���̃A���r�V�I��\x0A���̐g�ł�����\x0A���키���悢�I" );
			} else {
				oCurMsg.rwMsg( "���щΎR�̊���\x0A�Ƃ���",		"���[�f�B�X�ɋt�炤\x0A�����҂߁c�A�킪���`\x0A�󂯂Ă݂�b�I" );
			}
		}
		oCurMsg.rwMsg( "�킢�͂��ꂩ��",				"�G�R����������\x0A���X���[���A��������\x0A�Ɛ^�������Ǝ����I" );
		oCurMsg.rwMsg( "�F�̎ҁA��������",				"�F�̎ҁA�ǂ����I\x0A���̔C���͏d�傾\x0A��͑����͉������I" );
		oCurMsg.rwMsg( "���Ă΋Ɋy",					"���F�A�G�R��\x0A��X�ɉ����򓙖���\x0A���̈Ⴂ�������̂��I" );
		oCurMsg.rwMsg( "�҂ǂ��A�Ђ�ނ�",				"�F�̎ҁA�G�ɗ^���邾\x0A���ł悢�c���ɔs�k\x0A����Ƃ����������I" );
//		oCurMsg.rwMsg( "�҂ǂ��A�Ђ�ނ�",				"�G�͉��Ɏx�z�����\x0A��]��ł���I�@����\x0A�肢�����Ă�ꂢ�I" );
		oCurMsg.rwMsg( "���ǂ���A����x",				"����ȏ�A�z���\x0A������ɂ͂ł���\x0A���₩�ɐ�������I" );
		
		// �ە����ł�MAX
		oCurMsg.rwMsg( "�킪���͂��ł�",		"�䂪���m�B�͂��ł�\x0A�m�C�����ł���\x0A�������D�@���I" );
		
		// ����ȏ㒥�����s�\
		oCurMsg.rwMsg( "�킪���̕��m��",		"���X���[���A����\x0A�������s���|\x0A�]�v�ȕ��͗v��܂�" );

		// �ދp���̃��b�Z�[�W
		oCurMsg.rwMsg( "���͂₱��܂�",		"�ނށc�I\x0A���̎��Ƀv���b�V���[\x0A��^����Ƃ́c" );

		// ��C�j�󎞂ɏC���̃��b�Z�[�W���̂P
		// ��ꂽ�̂ŁA�V���ȑ�C�ɔԍ���t����B�ԍ��͓�ԁ`�\��
		int iTaihouIX = (rand() % 9);
		char szTaihouNo[7][3] = {"��","�O","�l","��","�Z","��","��"};
		char szTaihouMessage[70] = "";
		// �����_���Ŕԍ���I��ŁA���b�Z�[�W�ɍ���
		wsprintfA(szTaihouMessage, "�ނށA��C����ꂽ���I\x0A�ւ���%s�ԖC��\x0A�}������Ɏ��Ă��I", szTaihouNo[iTaihouIX]);
		oCurMsg.rwMsg( "���A��C����ꂽ�I",	szTaihouMessage );

		// ��C�j�󎞂ɏC���̃��b�Z�[�W���̂Q
		oCurMsg.rwMsg( "������C����ꂽ���I",	"������C����ꂽ���I\x0A���ʂ��c���}��\x0A�C�����������I" );

		oCurMsg.rwMsg( "���щΎR�̊���\x0A���c",  "�ȁc�Ȃ�ƁI\x0A���̎��ɒv���̈�ł�\x0A�^�����Ƃ����̂��c" );


		/* 
		 * �����X���b�g�^���^���X�̋t�ÎE���̃��b�Z�[�W�B
		 * �t�ÎE�͒ʏ�Ƃ͈قȂ鏈�������Ă���̂ŁAMessageN6PPatch::isOnTenshouExeMessageCopyUsed
		 * �����ł͏�肭�����Ȃ��̂ŁA����ŁBOnAnsatsuAttackInBattle�֘A�B
		 */
		// �t�ÎE�ł�鑤�i��ʕ����j  (==2�Ƃ������Ƃ͕�����ID��Org�n�̃��x���ő��v�ł�����)
		if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 2 ) {

			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
			string strBushouName = ���l�|���聓(iTmp1st_BushouID, iTmp2nd_BushouID );
			char *pBushouName = (char *)strBushouName.c_str();
			char szAnsatsuMsg[256] = "";

			if ( oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG04 ) ||
				 oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG05 ) ) {
				// �t�ÎE�p�̃��b�Z�[�W�ϓ����[�h�͏I��
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;
			}

			// �t�ÎE���̂P
			wsprintf( szAnsatsuMsg, "������%s��c�I\x0A�߂������S��\x0A�g��łڂ��݂̂��I", pBushouName );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG04,	szAnsatsuMsg );

			// �t�ÎE���̂Q
			wsprintf( szAnsatsuMsg, "���̈Í��R�m�c����\x0A�ÎE���d�|����Ƃ́c\x0A���グ����������I" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG05,	szAnsatsuMsg );

		}		
	
	/* 
	 * �ÎE���̂���鑤�̃��b�Z�[�W�BOnAnsatsuAttackInBattle�֘A�B
	 */
	// �t�ÎE���ł���鑤 (==1�Ƃ������Ƃ͕�����ID��Org�n�̃��x���ő��v�ł�����)
	} else if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 1 ) {

		// �t�ÎE�p�̃��b�Z�[�W�ϓ����[�h�����ɐi�߂�
		iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 2;

		// char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa-1].fastname;
		int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
		int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
		string strBushouName = ���l�|���聓(iTmp1st_BushouID, iTmp2nd_BushouID );
		char *pBushouName = (char *)strBushouName.c_str();
		char szAnsatsuMsg[256] = "";

		wsprintf( szAnsatsuMsg, "��I�M�l�A��������I\x0A�c������I�c���̂�\x0A%s�߁c�������c��", pBushouName);
		oCurMsg.rwMsg( �����o���E�B���h�E::�푈::��ÎE::REG01, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "���c�������瑄���I\x0A���A���܂����c\x0A%s�߁c���j��c��", pBushouName);
		oCurMsg.rwMsg( �����o���E�B���h�E::�푈::��ÎE::REG02, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "���I�����Ⴑ�̒ɂ݂�\x0A�c���A����͓Ŗ�I\x0A%s�߁c�������c��", pBushouName);
		oCurMsg.rwMsg( �����o���E�B���h�E::�푈::��ÎE::REG03, szAnsatsuMsg);

		wsprintf( szAnsatsuMsg, "�ށI�������ȍ������c\x0A%s�߁c��Ɏh�q��\x0A�c�A�����c����", pBushouName);
		oCurMsg.rwMsg( �����o���E�B���h�E::�푈::��ÎE::REG04, szAnsatsuMsg);


	/* 
	 * �ÎE���̂�鑤�̃��b�Z�[�W�BOnAnsatsuAttackInBattle�֘A�B
	 */
	// �t�ÎE�ł�鑤�i��ʕ����j  (==2�Ƃ������Ƃ͕�����ID��Org�n�̃��x���ő��v�ł�����)
	} else if ( iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute == 2 ) {

		// �^���^���X�ȊO�̏ꍇ�B���̏����͂Ȃ����K�v�B
		if ( isLancelotTartare(iOrgBushouIDOfAnsatsuSareruGawa-1) ) {
		
			// �ǂꂩ�̃��b�Z�[�W�ɂЂ��������Ă�����I��
			if ( oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG01 ) ||
				 oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG02 ) ||
				 oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG03 ) ||
				 oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG04 ) ||
				 oCurMsg.isMsg( �����o���E�B���h�E::�푈::���ÎE::REG05 ) ) {

				// �t�ÎE�p�̃��b�Z�[�W�ϓ����[�h�͏I��
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;
	
			}

			// �ǂꂩ�̃��b�Z�[�W�͊m���Ȃ̂Œu��������
			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa-1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa-1;
			string strBushouName = �����聓(iTmp1st_BushouID, iTmp2nd_BushouID );
			char *pBushouName = (char *)strBushouName.c_str();

			int iBushouSex = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa-1].sex;
			int iBushouTone = nb6bushouref[iOrgBushouIDOfAnsatsuSareruGawa-1].tone;
			char szAnsatsuMsg[256] = "";

			// �t�ÎE���̂P
			wsprintf( szAnsatsuMsg, "��ɓM�ꂵ�����҂�\x0A�łɖł��邪�悢" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG01,	szAnsatsuMsg );

			// �t�ÎE���̂Q
			wsprintf( szAnsatsuMsg, "����Ȃ�%s�c\x0A�܂����̎��������\x0A���܂����c", pBushouName );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG02,	szAnsatsuMsg );

			// �t�ÎE���̂R
			wsprintf( szAnsatsuMsg, "�ق��ق��ق�\x0A�{���̒���\x0A���ɂ܂��Ċi�ʁc" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG03,	szAnsatsuMsg );

			// �t�ÎE���̂S
			// ������
			if ( iBushouSex == 1 ) {
				wsprintf( szAnsatsuMsg, "�Ƒ��Ȃ�%s�I\x0A����A�����Ȃ�����\x0A�����͑�����ł��傤", pBushouName );
			// �������e�\
			} else if ( iBushouTone == 2 ) {
				wsprintf( szAnsatsuMsg, "���m�b�Ȃ�%s�I\x0A����A�����Ȃ�����\x0A�����͑����ꂾ�낤", pBushouName );
			// ���������M
			} else if ( iBushouTone == 3 ) {
				wsprintf( szAnsatsuMsg, "�˒m�͐g�̋w�c\x0A����A�����Ȃ�����\x0A�����͑����ꂶ��낤" );
			// ���̑��̌���
			} else {
				wsprintf( szAnsatsuMsg, "��ɓM�ꂽ��%s�I\x0A����A�����Ȃ�����\x0A�����͑�����ł��낤", pBushouName );
			}
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG04,	szAnsatsuMsg );

			// �t�ÎE���̂T
			wsprintf( szAnsatsuMsg, "�ˎq�˂��T��\x0A���ꂪ�Ŋ��Ƃ�\x0A�͂��Ȃ����̂�̂��c" );
			oCurMsg.rwMsg( �����o���E�B���h�E::�푈::���ÎE::REG05,	szAnsatsuMsg );
		}

		// ��ɂ̏ꍇ�B���̏����͂Ȃ����K�v�B
		else if (isShuiHidechiyo(iOrgBushouIDOfAnsatsuSareruGawa - 1)) {

			// �ǂꂩ�̃��b�Z�[�W�ɂЂ��������Ă�����I��
			if (oCurMsg.isMsg(�����o���E�B���h�E::�푈::���ÎE::REG01) ||
				oCurMsg.isMsg(�����o���E�B���h�E::�푈::���ÎE::REG02) ||
				oCurMsg.isMsg(�����o���E�B���h�E::�푈::���ÎE::REG03) ||
				oCurMsg.isMsg(�����o���E�B���h�E::�푈::���ÎE::REG04) ||
				oCurMsg.isMsg(�����o���E�B���h�E::�푈::���ÎE::REG05)) {

				// �t�ÎE�p�̃��b�Z�[�W�ϓ����[�h�͏I��
				iStatusAntiAnsatsuMessageModeOnAttackInBattleExecute = 0;

			}

			// �ǂꂩ�̃��b�Z�[�W�͊m���Ȃ̂Œu��������
			//char *pBushouName = nb6bushouname[iOrgBushouIDOfAnsatsuSuruGawa-1].fastname;
			int iTmp2nd_BushouID = iOrgBushouIDOfAnsatsuSuruGawa - 1;
			int iTmp1st_BushouID = iOrgBushouIDOfAnsatsuSareruGawa - 1;
			string strBushouName = �����聓(iTmp1st_BushouID, iTmp2nd_BushouID);
			char *pBushouName = (char *)strBushouName.c_str();

			int iBushouSex = nb6bushouname[iOrgBushouIDOfAnsatsuSareruGawa - 1].sex;
			int iBushouTone = nb6bushouref[iOrgBushouIDOfAnsatsuSareruGawa - 1].tone;
			char szAnsatsuMsg[256] = "";

			// �t�ÎE���̂P
			wsprintf(szAnsatsuMsg, "��ɓM�ꂵ�����҂�\x0A�łɖł��邪�悢");
			oCurMsg.rwMsg(�����o���E�B���h�E::�푈::���ÎE::REG01, szAnsatsuMsg);

			// �t�ÎE���̂Q
			wsprintf(szAnsatsuMsg, "����Ȃ�%s�c\x0A�܂����̎��������\x0A���܂����c", pBushouName);
			oCurMsg.rwMsg(�����o���E�B���h�E::�푈::���ÎE::REG02, szAnsatsuMsg);

			// �t�ÎE���̂R
			wsprintf(szAnsatsuMsg, "�ق��ق��ق�\x0A�{���̒���\x0A���ɂ܂��Ċi�ʁc");
			oCurMsg.rwMsg(�����o���E�B���h�E::�푈::���ÎE::REG03, szAnsatsuMsg);

			// �t�ÎE���̂S
			// ������
			if (iBushouSex == 1) {
				wsprintf(szAnsatsuMsg, "�Ƒ��Ȃ�%s�I\x0A����A�����Ȃ�����\x0A�����͑�����ł��傤", pBushouName);
				// �������e�\
			}
			else if (iBushouTone == 2) {
				wsprintf(szAnsatsuMsg, "���m�b�Ȃ�%s�I\x0A����A�����Ȃ�����\x0A�����͑����ꂾ�낤", pBushouName);
				// ���������M
			}
			else if (iBushouTone == 3) {
				wsprintf(szAnsatsuMsg, "�˒m�͐g�̋w�c\x0A����A�����Ȃ�����\x0A�����͑����ꂶ��낤");
				// ���̑��̌���
			}
			else {
				wsprintf(szAnsatsuMsg, "��ɓM�ꂽ��%s�I\x0A����A�����Ȃ�����\x0A�����͑�����ł��낤", pBushouName);
			}
			oCurMsg.rwMsg(�����o���E�B���h�E::�푈::���ÎE::REG04, szAnsatsuMsg);

			// �t�ÎE���̂T
			wsprintf(szAnsatsuMsg, "�ˎq�˂��T��\x0A���ꂪ�Ŋ��Ƃ�\x0A�͂��Ȃ����̂�̂��c");
			oCurMsg.rwMsg(�����o���E�B���h�E::�푈::���ÎE::REG05, szAnsatsuMsg);
		}

	}


	if ( iTeppouHanekaesiInTeppouDeath == 1 ) {
		/*
		 �ȉ��A�ʏ핐�����S�C��e���Ԃ��ꂽ���̃��b�Z�[�W
		*/
		extern int iTeppouAttackBushouIDInTeppouDeath; // �����ꂽ��(���֍ς݂Ȃ���)
		extern int iTeppouDefendBushouIDInTeppouDeath; // ��������(���֍ς݂Ȃ���)
		int i1st = iTeppouDefendBushouIDInTeppouDeath-1;
		int i2nd = iTeppouAttackBushouIDInTeppouDeath-1;

		if ( oCurMsg.isMsg( "�ށA���O�c\x0A" ) || 
			 oCurMsg.isMsg( "�����Ŏ��ʂ�\x0A" ) ||
			 oCurMsg.isMsg( "����قǂ̋��|��\x0A" ) ||
			 oCurMsg.isMsg( "�ɗ�����������\x0A" ) ||
			 oCurMsg.isMsg( "���͕��a�Ȏ����\x0A" ) ) {

			// �����X���b�g�ȊO�͒e���Ԃ��Ȃ��͂��ł��邪�A�ꉞ�̏���
			if ( isLancelotTartare(i2nd) ) { 
				iTeppouHanekaesiInTeppouDeath = 2;
			} else { 
				iTeppouHanekaesiInTeppouDeath = 0;
			}

			string msg = ���l�|���聓(i1st ,i2nd) + "�߁I\x0A�S�C�𓁂Œe���Ԃ��Ƃ�\x0A"+���͂͂́�(i1st,i2nd,0xFFFF)+"�c�c���Ӂc";
			if (isShuiHidechiyo(i2nd)) {
				msg = ���l�|���聓(i1st, i2nd) + "�߁I\x0A�S�C��_�Œe���Ԃ��Ƃ�\x0A" + ���͂͂́�(i1st, i2nd, 0xFFFF) + "�c�c���Ӂc";
			}
			char szmsg[100] = "";
			strcpy(szmsg, msg.c_str() );
			oCurMsg.rwMsg( "�ށA���O�c\x0A",		szmsg);
			oCurMsg.rwMsg( "�����Ŏ��ʂ�\x0A",		szmsg);
			oCurMsg.rwMsg( "����قǂ̋��|��\x0A",		szmsg);
			oCurMsg.rwMsg( "�ɗ�����������\x0A",		szmsg);
			oCurMsg.rwMsg( "���͕��a�Ȏ����\x0A",		szmsg);
		}
	}

	if ( oCurMsg.isMsg("�ς��Ă�����\x0A�҂ǂ��A�������I") || oCurMsg.isMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I")) {
		if ( nb6fukidashirelatepersons->i1stPersonID <= GAMEDATASTRUCT_BUSHOU_NUM ) {
			int iBushouID = nb6fukidashirelatepersons->i1stPersonID-1;

			if ( rand()%6 == 0 ) {
				// �o�[���[�t�H��
				if ( strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�L�M�N" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "�^�̋R�m�̌ւ��\x0A�M�l�Ɍ����Ă�낤�c\x0A�t���C�~���O�f�X�I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "�^�̋R�m�̌ւ��\x0A�M�l�Ɍ����Ă�낤�c\x0A�t���C�~���O�f�X�I");
				}
				// ���H���b�N
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�s�t�u" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�v�w�x" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "�Í��R�m�c�̗͂�\x0A�����Ă�����Ă͍���\x0A���C�A�b�g�o�[���I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "�Í��R�m�c�̗͂�\x0A�����Ă�����Ă͍���\x0A���C�A�b�g�o�[���I");
				}
				// �o���o�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�U�V�W" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�X�Y�Z" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "���̉��̉��`��������\x0A�̂��H�@�����x�����c\x0A�f�X�A�x���W���[�I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "���̉��̉��`��������\x0A�̂��H�@�����x�����c\x0A�f�X�A�x���W���[�I");
				}
				// �}���e�B��
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�c�d" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�`�a�b" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "���̉��ƃT�V�Ő킨��\x0A�Ȃ�10�N�����񂾁I\x0A�t���[���F�m���I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "���̉��ƃT�V�Ő킨��\x0A�Ȃ�10�N�����񂾁I\x0A�t���[���F�m���I");
				}
				// �n�{����
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�d�e�f" )==0 ) {
				}
				// �I�Y�}
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�v�w�x" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�y�z�{" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "���������͔F�߂��c\x0A�ł��A�����܂ł�c�A\x0A�f�[�������[�Y�I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "���������͔F�߂��c\x0A�ł��A�����܂ł�c�A\x0A�f�[�������[�Y�I");
				}
				// �x���T���A
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�^�_�`" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�a�b�c" )==0 ) {
				}
				// �J�m�[�v�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�O�u" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�[�\�]" )==0 ) {
				}
				// �M���o���h
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�g�h�i" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "���" )==0 ) {
				}
				// �~���f�B��
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�O�P�Q" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�R�S�T" )==0 ) {
				}
				// �M���_�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�G�H�F" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "������" )==0 ) {
				}
				// �f�l�u
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�j�k" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�l�m" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "�ւ���ꂵ���Â̗d��\x0A�Ăі߂���c�Ă���I\x0A�X�[�p�[�m���@�I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "�ւ���ꂵ���Â̗d��\x0A�Ăі߂���c�Ă���I\x0A�X�[�p�[�m���@�I");
				}
				// ���h����
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�y�z�{" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�|�}�~" )==0 ) {
					oCurMsg.rwMsg("�ς��Ă�����\x0A�҂ǂ��A�������I", "�V�삯������̌�͂�\x0A���X���n�ɗ��Ƃ���\x0A���e�I�X�g���C�N�I");
					oCurMsg.rwMsg("�������͂Ȃ���\x0A�U�߂����ꂦ���I", "�V�삯������̌�͂�\x0A���X���n�ɗ��Ƃ���\x0A���e�I�X�g���C�N�I");
				}
				// �E�H�[����
				else if (strcmp( nb6bushouname[iBushouID].familyname,"�q�r")==0 &&  strcmp( nb6bushouname[iBushouID].fastname, "�n�o�p")==0 ) {
				}

			}
		}
	}

	// �O�����b�Z�[�W�u��������ScenarioMod�ɂčs��
	if ( p_snOnRewriteFukidashiMessage ) {
		char szModMsgBuf[512] = "";

		// �ʏ�^�C�v
		if ( isMessageLOCMode == 4) {

			// ���݂̃������̍ŐV�̏�Ԃ���荞��
			char szCurMsgBuf[512] = "�@�@�@�@"; // �n�����������k���Ȃǂ̔���ɍ��i���Ȃ��悤�ɂ���Ȃ��
			// ����������ǂݍ���
			ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC4)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
			szCurMsgBuf[511] = NULL; // ���S�u���b�N

			// ������, ���݂̏��������ΏۂƂȂ��Ă��郁�b�Z�[�W, �֘A�l�����
			// ((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, CPM.getCurVarPtr(LOC4), nb6fukidashirelatepersons);
			((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
			if ( strlen(szModMsgBuf) > 1 ) {
				// ��������getCurVarPtr �Ƃ����悤�ɌÂ���Ԃł��n�j
				oCurMsg.rwMsg( (char *)CPM.getCurVarPtr(LOC4), szModMsgBuf, -1 );
				isCurScenarioModReWrite = true;
			}

		// �p�����^�^�C�v(�S���p�����^�łQ�ɓW�J���ꂽ���͂�����j
		} else if ( isMessageLOCMode == 2 ) {

			// ���݂̃������̍ŐV�̏�Ԃ���荞��
			char szCurMsgBuf[512] = "�@�@�@�@"; // �n�����������k���Ȃǂ̔���ɍ��i���Ȃ��悤�ɂ���Ȃ��
			ReadProcessMemory( CommonGlobalHandle::curProcess, (LPCVOID)(CPM.getMemoryAddress(LOC2)), szCurMsgBuf, sizeof(szCurMsgBuf)-1, NULL);
			szCurMsgBuf[511] = NULL; // ���S�u���b�N

			// ((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, CPM.getCurVarPtr(LOC2), nb6fukidashirelatepersons);
			((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, szCurMsgBuf, nb6fukidashirelatepersons);
			if ( strlen(szModMsgBuf) > 1 ) {
				// ��������getCurVarPtr �Ƃ����悤�ɌÂ���Ԃł��n�j
				oCurMsg.rwMsg( (char *)CPM.getCurVarPtr(LOC2), szModMsgBuf, -1 );
				isCurScenarioModReWrite = true;
			}

		}
	}

	return true;
}



void checkOnTenshouExeMessageLoad() {
	;
}


/*

0046C18C  |. E8 0C000000    CALL    TENSHOU.0046C19D
0046C191  |. 83C4 04        ADD     ESP, 4
0046C194  |. B8 C0974D00    MOV     EAX, TENSHOU.004D97C0            ;  ASCII "�f�[�^�̓ǂݍ��ݒ��ł�
				�������� JMP TSMod.JmpOnTSExeMessageLoad �Ƃ���̂��ړI�@�����āA���X���̍s�ɂ����� MOV     EAX, TENSHOU.004D97C0 �͊֐����ŏ���

0046C199  |. 5E             POP     ESI

0049D144  -E9 8782F2FF               ; �� OnTenshouExeMessageLoad��
�@�@�@�@�@�@�@�@�� ���������s���ɏ��������Ă��K�v������B
*/
int pTenshouExeJumpFromToOnTenshouExeMessageLoad    =0x46C194; // �֐��͂��̃A�h���X����AOnTenshouExeMessageLoad�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeMessageLoad =0x46C199; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMessageLoad() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov eax, 4D97C0h

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageLoad();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageLoad
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.JmpOnTSExeMessageLoad         ; �� OnTenshouExeMessageLoad��
�@�@�@�@�@�@�@�@�� ���������s���ɏ��������Ă��K�v������B
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageLoad[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


void WriteAsmJumperOnTenshouExeMessageLoad() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMessageLoad;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageLoad + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageLoad  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMessageLoad+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageLoad), cmdOnTenshouExeJumpFromMessageLoad, 5, NULL); // 5�o�C�g�̂ݏ�������
}









/*
004ADB28  /$ 8B4424 04      MOV     EAX, DWORD PTR SS:[ESP+4]
004ADB2C  |. 8D4C24 0C      LEA     ECX, DWORD PTR SS:[ESP+C]
004ADB30  |. 51             PUSH    ECX
004ADB31  |. A3 B8674E00    MOV     DWORD PTR DS:[4E67B8], EAX
004ADB36  |. FF7424 0C      PUSH    DWORD PTR SS:[ESP+C]
004ADB3A  |. 68 16DB4A00    PUSH    TENSHOU.004ADB16
004ADB3F  |. E8 E9FCFFFF    CALL    TENSHOU.004AD82D
004ADB44  |. 83C4 0C        ADD     ESP, 0C
004ADB47  |. 8B0D B8674E00  MOV     ECX, DWORD PTR DS:[4E67B8]       ;  TENSHOU.004D9B91
004ADB4D  |. C601 00        MOV     BYTE PTR DS:[ECX], 0
004ADB50  \. C3             RETN


004ADB47  |. 8B0D B8674E00  MOV     ECX, DWORD PTR DS:[4E67B8]       ;  TENSHOU.004D9B91
				�������� JMP TSMod.OnTSExeMessageCopy �Ƃ���̂��ړI�@�����āA���X���̍s�ɂ����� MOV     EAX, TENSHOU.004D97C0 �͊֐����ŏ���

*/
extern void OnTenshouExeEndingConditionYakushokuMsgRewrite();
extern void OnTenshouExeEndingConditionScenarioModMsgRewrite();
// ���b�Z�[�W���R�s�[���ꂽ���B�����o����e��E�B���h�E�ɕ\������镶�������炽�ɌĂ΂ꂽ�^�C�~���O
void checkOnTenshouExeMessageCopy() {
	// �n�����ꃂ�[�h�̏ꍇ�ȂǁA�J�X�^���G���f�B���O���菈��
	if ( TSModCommand::MyselfDaimyo::cmdGameEndConditionIsGettingPosition ) {
		OnTenshouExeEndingConditionYakushokuMsgRewrite();
	}

	OnTenshouExeEndingConditionScenarioModMsgRewrite();
}


int pTenshouExeJumpFromToOnTenshouExeMessageCopy    =0x4ADB47; // �֐��͂��̃A�h���X����AOnTenshouExeMessageCopy�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeMessageCopy =0x4ADB4D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMessageCopy() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		mov ecx, dword ptr ds:[4E67B8h]

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageCopy();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeMessageCopy
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.OnTSExeMessageCopy         ; �� OnTenshouExeMessageCopy��
�@�@�@�@�@�@�@�@�� ���������s���ɏ��������Ă��K�v������B
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageCopy[7]="\xE9\x90\x90\x90\x90\x90";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂ŁANOP���Ō�ɕt��������Ă���B


void WriteAsmJumperOnTenshouExeMessageCopy() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMessageCopy;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageCopy + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageCopy  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMessageCopy+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageCopy), cmdOnTenshouExeJumpFromMessageCopy, 6, NULL); // 6�o�C�g�̂ݏ������� ���̖��߂�6�o�C�g�A�V�K�̖��߂��T�o�C�g�Ȃ̂ŁANOP���t��������Ă���B
}










/*
004AD208  |> 5B             POP     EBX                              ;  TENSHOU.004B5930
				�������� JMP TSMod.OnTSExeMessageCopyMem �Ƃ���̂��ړI�@�����āA���X���̍s�ɂ����� MOV     EAX, TENSHOU.004D97C0 �͊֐����ŏ���
004AD209  |. C601 00        MOV     BYTE PTR DS:[ECX], 0
004AD20C  \. C3             RETN
*/

// ���b�Z�[�W���R�s�[���ꂽ���B�����o����e��E�B���h�E�ɕ\������镶�������炽�ɌĂ΂ꂽ�^�C�~���O

void checkOnTenshouExeMessageCopyMem() {
	;
}

int pTenshouExeJumpFromToOnTenshouExeMessageCopyMem    =0x4AD208; // �֐��͂��̃A�h���X����AOnTenshouExeMessageCopyMem�ւƃW�����v���Ă���B
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeMessageCopyMem() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		POP     EBX
		MOV     BYTE PTR DS:[ECX], 0
		// ���XTENSHOU.EXE���ɂ��������̂�������

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	checkOnTenshouExeMessageCopyMem();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		RETN
	}
}

/*
0049D143   90               NOP
0049D144  -E9 8782F2FF      JMP     TSMod.OnTSExeMessageCopyMem         ; �� OnTenshouExeMessageCopyMem��
�@�@�@�@�@�@�@�@�� ���������s���ɏ��������Ă��K�v������B
0049D149   90               NOP
*/
char cmdOnTenshouExeJumpFromMessageCopyMem[6]="\xE9";
// ���̖��߂�6�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂ŁANOP���Ō�ɕt��������Ă���B


void WriteAsmJumperOnTenshouExeMessageCopyMem() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeMessageCopyMem;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeMessageCopyMem + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeMessageCopyMem  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromMessageCopyMem+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeMessageCopyMem), cmdOnTenshouExeJumpFromMessageCopyMem, 5, NULL); // 5�o�C�g�̂ݏ�������
}
