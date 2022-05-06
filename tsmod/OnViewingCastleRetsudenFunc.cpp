#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#define GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP -0x1E4	// ���`�̃_�C�A���O��LEFT(X)���W���͂������ꏊ�iEBP�̒l�̃A�h���X����̃A�h���X����)
#define GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP -0x1E0	// ���`�̃_�C�A���O��TOP(Y)���W���͂������ꏊ�iEBP�̒l�̃A�h���X����̃A�h���X����)

extern BOOL isMainWindowCastleButtonMode;

/*
 * ��`��ʂ��J���O��̃t�b�N
 */ 


/*
0045376B  |. E8 94350200    CALL    TENSHOU.00476D04                 ; \TENSHOU.00476D04
00453770  |. E8 62810200    CALL    TENSHOU.0047B8D7
00453775  |. 8D8D F4FDFFFF  LEA     ECX, DWORD PTR SS:[EBP-20C]
*/

// ��̔ԍ��ւ̃|�C���^
static int pEBPOFViewingCastleRetsuden = NULL;
static int iCastleIDOFViewingCastleRetsudenFunc = -1;

static HMODULE hCASTLEMAPINRETSUDENDLL = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONBEGINPAINT)(int iCastleID, char *szCastleName, int x, int y, int iCastleRank);
static PFNCASTLEMAPINRETSUDENONBEGINPAINT pBeginPaint = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONUPDATEPAINT)(int x, int y);
static PFNCASTLEMAPINRETSUDENONUPDATEPAINT pUpdatePaint = NULL;

typedef int (WINAPI *PFNCASTLEMAPINRETSUDENONENDPAINT)();
static PFNCASTLEMAPINRETSUDENONENDPAINT pEndPaint = NULL;

// ���`�ŁA��}�b�v��`�悷�ׂ���
static bool isRetsudenEditorExtRepaint = false;

// ���`���A�\�����ꂽ�B
static bool isRetsudenEditorExtViewing = false;

static int iPrevWindowX = 0;
static int iPrevWindowY = 0;
static const int iDeltaInRepaintterval = 5; // ���x�`�悵�Ă���ƃE�B���h�E�̈ړ��ɒx�����ł�̂ŁA����}�b�v�̕��������ĂT�`�b�N�����点�邱�ƂŁA�E�B���h�E�̈ړ����~���ɂ���B
static int iDeltaTickCnt = 0;

void RetsudenEditorExtRepaintCheck(CONST RECT *lprcSrc1, CONST RECT *lprcSrc2) {
	if (!isRetsudenEditorExtRepaint) { return; }

	// CastleMapInRetsudenMod.dll��ǂ݂ɂ�����B��{�I�ɂ͖������낤�B
	if (!hCASTLEMAPINRETSUDENDLL) {
		hCASTLEMAPINRETSUDENDLL = LoadLibrary( "CastleMapInRetsudenMod.dll" );
		if (!hCASTLEMAPINRETSUDENDLL) {
			isRetsudenEditorExtRepaint = false;
			return;
		}
		pBeginPaint = (PFNCASTLEMAPINRETSUDENONBEGINPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnBeginPaint" );
		if (!pBeginPaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

		pUpdatePaint = (PFNCASTLEMAPINRETSUDENONUPDATEPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnUpdatePaint" );
		if (!pUpdatePaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

		pEndPaint = (PFNCASTLEMAPINRETSUDENONENDPAINT)GetProcAddress( hCASTLEMAPINRETSUDENDLL, "OnEndPaint" );
		if (!pEndPaint) {
			isRetsudenEditorExtRepaint = false;
			return;
		}

	}

	if ( pBeginPaint && pUpdatePaint && pEndPaint ) {
	
		int x = lprcSrc1->left;
		int y = lprcSrc1->top;
		int w = lprcSrc1->right-lprcSrc1->left;
		int h = lprcSrc1->bottom-lprcSrc1->top;

		if (isRetsudenEditorExtViewing) {
			// ���̃_�C�A���O�̍��W
			int *pX = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP);
			int *pY = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP);

			if ( iDeltaTickCnt >= iDeltaInRepaintterval ) {
				iDeltaTickCnt = 0;
				// �`��X�V
				pUpdatePaint(*pX, *pY);
			}

			iPrevWindowX = *pX;
			iPrevWindowY = *pY;

			// �}�E�X����ȂǂŁA�E�B���h�E�𓮂�������A10�t���[���͕`�悵�Ȃ��B
			if (*pX != iPrevWindowX || *pY != iPrevWindowY) {
				iDeltaTickCnt = 0;
			} else {
				iDeltaTickCnt++;
			}


		// �ŏ�������A�Ƃ肠�����f���^�J�E���g�𑝂₷
		} else if (isRetsudenEditorExtViewing == false && iDeltaTickCnt < 1 ) {
			iDeltaTickCnt = 1;

		// �f���^�łP�t���[���o�߂�����A�ŏ��̎��s
		} else if (isRetsudenEditorExtViewing == false && iDeltaTickCnt >= 1 ) {
			isRetsudenEditorExtViewing = true;
			iDeltaTickCnt = iDeltaInRepaintterval;

			// ���̃_�C�A���O�̍��W
			int *pX = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_X_ADDRESS_FROM_EBP);
			int *pY = (int *)(pEBPOFViewingCastleRetsuden + GAMEDATASTRUCT_CASTLE_RETSUDEN_LAYOUT_Y_ADDRESS_FROM_EBP);

			// �͈̓`�F�b�N
			if ( 0 <= iCastleIDOFViewingCastleRetsudenFunc && iCastleIDOFViewingCastleRetsudenFunc < GAMEDATASTRUCT_CASTLE_NUM) {

				// ��ID
				int iCastleID = iCastleIDOFViewingCastleRetsudenFunc;

				// ��̃t������
				string castlefullname = nb6castle[iCastleID].name;
				castlefullname = castlefullname + getCastleTypeName(nb6castle[iCastleID].title);

				// ��̃����N�́A�ʏ��0��1�����c
				int iCastleRank = nb6castle[iCastleID].bLarge;

				// ���y��Ƒ���Ɋւ��ẮA
				const int kCastleOosaka = 167;
				const int kCastleAzuchi = 136;

				// ����ŁA�������Ă���A������Ȃ̂Ń����N���Q��
				if ( iCastleID == kCastleOosaka && isOsakaCastleComplete() ) {
					iCastleRank = 2;
				}
				// ���y��ŁA�������Ă���B������Ȃ̂Ń����N���Q��
				else if ( iCastleID == kCastleAzuchi && isAzuchiCastleComplete() ) {
					iCastleRank = 2;
				}

				// �ŏ��̃y�C���g�B
				pBeginPaint(iCastleIDOFViewingCastleRetsudenFunc, (char *)castlefullname.c_str(), *pX, *pY, iCastleRank);
			}
		}
	}

}

void OnTenshouExeViewingCastleRetsudenFuncExecutePrev() {

	// ���C���E�B���h�E�́u��v�{�^����������Ă��Ă��邩�ǂ���
	if (!isMainWindowCastleButtonMode) {
		return;
	}

	// ��ID���X�^�b�N����擾
	WORD iCastleID = *((WORD *)(pEBPOFViewingCastleRetsuden + 0x8)) - 1;

	if ( 0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		iCastleIDOFViewingCastleRetsudenFunc = iCastleID;
		iDeltaTickCnt = 0;
		isRetsudenEditorExtRepaint=true;
	} else {
		isRetsudenEditorExtRepaint=false;
	}
}

void OnTenshouExeViewingCastleRetsudenFuncExecutePost() {

	// ���C���E�B���h�E�́u��v�{�^����������Ă��Ă��邩�ǂ���
	if (!isMainWindowCastleButtonMode) {
		return;
	}

	if (hCASTLEMAPINRETSUDENDLL && pEndPaint) {
		pEndPaint();
	}

	isRetsudenEditorExtRepaint=false;
	isRetsudenEditorExtViewing=false;
	iDeltaTickCnt = 0;
}


/*
0045376B  |. E8 94350200    CALL    TENSHOU.00476D04                 ; \TENSHOU.00476D04
00453770  |. E8 62810200    CALL    TENSHOU.0047B8D7
00453775  |. 8D8D F4FDFFFF  LEA     ECX, DWORD PTR SS:[EBP-20C]
*/
int pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc	   = 0x453770; // �֐��͂��̃A�h���X����AOnTenshouExeViewingCastleRetsudenFunc�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeViewingCastleRetsudenFunc = 0x47B8D7; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeViewingCastleRetsudenFunc  = 0x453775; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeViewingCastleRetsudenFunc() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		mov pEBPOFViewingCastleRetsuden, EBP // EBP+0x8�̃A�h���X�ɁA��ԍ�(+1�x�[�X)�������Ă���B
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeViewingCastleRetsudenFuncExecutePrev();

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

		// ���̏����������ɋL��
		call pTenshouExeJumpCallFromToOnTenshouExeViewingCastleRetsudenFunc
	}

	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}


	OnTenshouExeViewingCastleRetsudenFuncExecutePost();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingCastleRetsudenFunc
	}

}




char cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v5�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeViewingCastleRetsudenFunc() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeViewingCastleRetsudenFunc;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc + 5 )	;
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingCastleRetsudenFunc  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingCastleRetsudenFunc), cmdOnTenshouExeJumpFromViewingCastleRetsudenFunc, 5, NULL); //5�o�C�g�̂ݏ�������
}




