#include "WinTarget.h"
#include <fstream>
#include <mmsystem.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "EXBushou_All.h"
#include "MessageN6PPatch.h"
#include "ScenarioMessageN6PPatch.h"
#include "DialogKahou.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "TenShougiMod.h"
#include "FuncPlayBGM.h"

using namespace std;

// BGMMM.lib�̒��ɂ���
extern int BGMmm_GetShouldBeRePlayLastestBGM();


CurProcessMemory CPM;
DialogKahou DK;

DWORD preSnModTimeGetTime = 0;

extern void RetsudenEditorExtRepaintCheck(CONST RECT *lprcSrc1, CONST RECT *lprcSrc2);

BOOL Hook_IntersectRectMessage(
  LPRECT lprcDst,        // �����������i�[�����o�b�t�@
  CONST RECT *lprcSrc1,  // �ŏ��̒����`
  CONST RECT *lprcSrc2   // 2 �Ԗڂ̒����`
)
{

	// �܂��Q�[�����͂��܂��ĂȂ���΁c
	if ( CommonGlobalHandle::isNobunaganoYabouGameStart == false ) {
		return TRUE;
	}

	if ( CommonGlobalHandle::isNobunaganoYabouGameEnd ) {
		return TRUE;
	}

	// ��������CurProcessMemory��CPM�l���Z�b�g
	CPM.setVariableFromMemory();

	// ���݂�GameDataStruct��ǂݍ���ŃZ�b�g�B
	setCurProcessGameMemory();

	// ���ꕐ���̋@�\��S�����܂Ƃ߂�
	EXBushou_All_Execute();

	// ��ʕ����̋@�\��S�����܂Ƃ߂�
	EXSuperService_Regeneration();

	// �M���Ɋւ���t�B���^�[�B����ȑ������������Ɋւ��āA�M����������Ȃ�������A���ʂɉ�������ȂǁB
	EXSuperService_FilterMerits();

	// �������A���b�Z�[�W�A�t�����
	ReWritePatchMessageN6P( CPM.getCurVarPtr(LOC1), CPM.getCurVarPtr(LOC4), NULL );

	// �ƕ�֘A�̏���
	DK.CheckDialogKahou( lprcSrc1, lprcSrc2 );

	RetsudenEditorExtRepaintCheck(lprcSrc1, lprcSrc2);

	// BGM���Đ����I�����悤���B�ēx�Đ�����ׂ��ł���B
	if ( BGMmm_GetShouldBeRePlayLastestBGM() ) {
		int iBGM = getPlayingBGM();
		/*
		-- - - - - - - - - - - - �P�R�ȕ��z�\
		--
		--                       01 : �|
		--                       02 : �|
		--                       03 : [���k�n��][�֓��n��]
		--                       04 : [�ߋE�n��][�O���R�}���h]
		--                       05 : [�푈�E�t][�푈�E�~]
		--                       06 : [���C�n��][�b�M�n��][�k���n��]
		--                       07 : [�푈�E�H][�푈�E�M��][�푈�E����][�����ݒ�][�{�\���̕�][����]
		--                       08 : [�R�z�n��][�R�A�n��][���R�}���h]
		--                       09 : [���s�䏊][�C�O�f�Ս`][�u�`�R�}���h][��I�R�}���h]
		--                       10 : [�l���n��][��B�n��]
		--                       11 : [�S������][���y��][�푈�R�}���h][��I�E����][��I�E��O����][��I�E�ⓚ][�퓬�E����][�퓬�E�s�k]
		--                       12 : [�푈�E��][�푈�E�ٔ�][�V���{]
		--                       13 : �|
		--                       14 : [���S�E�ŖS]
		*/
		// 11 : �̂����A[�퓬�E����][�퓬�E�s�k] �����̓��[�v���Ȃ��B
		if ( iBGM == 11 && (Is_FieldWar() || Is_CastleWar()) ) {
			// ���̎������͂Ȃɂ����Ȃ��B
		} else {
			FuncStopBGM();
			FuncPlayBGM(iBGM);
		}
	}
	
	// ScenarioMod��OnTick���Ăяo���B1�b��5��ڈ��B
	if ( p_snOnTick ) {
		DWORD curSnModTimeGetTime = timeGetTime();
		int RefleshTime = 200; // �_�b

		// �K��̊Ԋu���Ԃ��߂���
		if ( int(curSnModTimeGetTime - preSnModTimeGetTime) > int(RefleshTime) ) {

			// ���߂Ă̎��s�̎�
			if ( preSnModTimeGetTime == 0 ) {
				p_snOnTick(curSnModTimeGetTime, 0);
			// ����ȊO
			} else {
				p_snOnTick(curSnModTimeGetTime, curSnModTimeGetTime-preSnModTimeGetTime); // �o�ߎ��Ԃ�n���ďo���B
			}

			preSnModTimeGetTime = curSnModTimeGetTime;
		}
	}

	return TRUE;
}





