#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"

/*
 ���C�����E�퓬��������炸�A�n���x�������������鎞
 */



// �Z�[�u�f�[�^�Ɏ������Ă���A�G�߂̕ς��ڂɃt�B���^�[�̂��߂̗ݐς̏n���x��0�ɂȂ�B
void EXSuperService_ExperienceLimitReset() {

	// �ݐς̏n���x�����Z�b�g�B
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {
		//  �ݐς̏n���x�����Z�b�g�B
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov = 0;
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat = 0;
		nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int = 0;
	}
}



/*
 �����n���x����������
 */
/*
ESI�͂��̃L�����̌��_�|�C���^���̂��̂Ȃ̂ŁA0�l�ڂ̌��_�ł���0x4BFD18
�������āA532�Ŋ���ƁAiBushouID���o��͂��E�E�E

�����n���x
004094B6  |. E8 B1390A00    CALL    TENSHOU.004ACE6C
004094BB  |. 66:8946 09     MOV     WORD PTR DS:[ESI+9], AX

���̎��A�Ԃ�l(AX)���V�����n���l
*/
int iPointerOfUpdateExperienceGov = NULL; // �Y�������̃|�C���^�̌��_
WORD iNewValueOfUpdatedExperienceGov = 0; // �X�V���ꂽ��̏n���l
void OnTenshouExeUpdateExperienceGovExecute() {
	int iBushouID = iPointerOfUpdateExperienceGov - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // �푈���̂�

			double sub = iNewValueOfUpdatedExperienceGov - nb6bushouref[iBushouID].expgov; // �V�l-���ݒl = ������ 
			if ( sub > 1 ) { // �}�C�i�X��P�̎��͏������Ȃ�
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov += (int)sub; // ���������Z�[�u�f�[�^�ɕێ�
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov > 2000 ) { // 2000�ȏ��2000�ŊW
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_gov = 2000;
				}
				// �����ɂ��ẮA�t�B���^�[�����͍s��Ȃ��B�ۑ������̂݁B
			}
		}
	}
}

/*
004094B6  |. E8 B1390A00    CALL    TENSHOU.004ACE6C
				�������� JMP TSMod.OnTSExeUpdateExperienceGov �Ə��������Ă����ۂ̏���
004094BB  |. 66:8946 09     MOV     WORD PTR DS:[ESI+9], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov	 =0x4094B6; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateExperienceGov�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceGov =0x4ACE6C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceGov  =0x4094BB; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceGov() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceGov

		mov iPointerOfUpdateExperienceGov, esi // esi ���Y�������̌��_

		mov iNewValueOfUpdatedExperienceGov, ax // ax���X�V���ꂽ�l�B

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceGovExecute();

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

		mov ax, iNewValueOfUpdatedExperienceGov // ���ʂ��㏑���������p

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceGov
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceGov[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateExperienceGov() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateExperienceGov;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceGov  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceGov+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceGov), cmdOnTenshouExeJumpFromUpdateExperienceGov, 5, NULL); //5�o�C�g�̂ݏ�������
}




//---------------------------------------------------------------------------------------------



/*
 �퓬���x����������
 */
/*
ESI�͂��̃L�����̌��_�|�C���^���̂��̂Ȃ̂ŁA0�l�ڂ̌��_�ł���0x4BFD18
�������āA532�Ŋ���ƁAiBushouID���o��͂��E�E�E

�����n���x
0040957B  |. E8 EC380A00    CALL    TENSHOU.004ACE6C
00409580  |. 66:8946 0B     MOV     WORD PTR DS:[ESI+B], AX

���̎��A�Ԃ�l(AX)���V�����n���l

*/
int iPointerOfUpdateExperienceBat = NULL;
WORD iNewValueOfUpdatedExperienceBat = 0; // �X�V���ꂽ��̏n���l
void OnTenshouExeUpdateExperienceBatExecute() {
	int iBushouID = iPointerOfUpdateExperienceBat - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // �푈���̂�

			double sub = iNewValueOfUpdatedExperienceBat - nb6bushouref[iBushouID].expbat; // �V�l-���ݒl = ������ 
			
			if ( sub > 1 ) { // �}�C�i�X��P�̎��͏������Ȃ�
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat += (int)sub; // ���������Z�[�u�f�[�^�ɕێ�
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat > 2000 ) { // 2000�ȏ��2000�ŊW
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat = 2000;
				}
				// �}�ɂ͐������Ȃ����n�m
				if ( TSModCommand::AllBushou::cmdNotGrowUpSuddenly ) {
					double coef = 1 + ((double)nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_bat / ((double)180 / (double)TSModCommand::AllBushou::cmdNotGrowUpSuddenly) ); // 1.0�`11.0���炢�܂ŁB���݂̗ݐϒl���傫���قǁAcoef���傫���B
					sub = sub / coef; // �ݐόo���l�������Ȃ����1�ɋ߂��l�Ŋ���B��������11�ɋ߂��l�Ŋ���B
					sub = ceil(sub); // �����_�ȉ��͐؂�グ��B
					iNewValueOfUpdatedExperienceBat = (int)sub + nb6bushouref[iBushouID].expbat; // ���̐V���ȍ����ƌ��ݒl�̍��v���A���䂪���������V�l�B
				}
			}
		}
	}
}

/*
0040957B  |. E8 EC380A00    CALL    TENSHOU.004ACE6C
				�������� JMP TSMod.OnTSExeUpdateExperienceBat �Ə��������Ă����ۂ̏���
00409580  |. 66:8946 0B     MOV     WORD PTR DS:[ESI+B], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat	 =0x40957B; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateExperienceBat�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceBat =0x4ACE6C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceBat  =0x409580; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceBat() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceBat

		mov iPointerOfUpdateExperienceBat, esi // esi ���Y�������̌��_

		mov iNewValueOfUpdatedExperienceBat, ax // ax���X�V���ꂽ�l�B

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceBatExecute();

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

		mov ax, iNewValueOfUpdatedExperienceBat // ���ʂ��㏑���������p

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceBat
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceBat[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateExperienceBat() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateExperienceBat;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceBat  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceBat+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceBat), cmdOnTenshouExeJumpFromUpdateExperienceBat, 5, NULL); //5�o�C�g�̂ݏ�������
}



//---------------------------------------------------------------------------------------------



/*
 �q�d�n���x����������
 */
/*
ESI�͂��̃L�����̌��_�|�C���^���̂��̂Ȃ̂ŁA0�l�ڂ̌��_�ł���0x4BFD18
�������āA532�Ŋ���ƁAiBushouID���o��͂��E�E�E

�m�͂̏n���l�̏�������
00409640  |. E8 27380A00    CALL    TENSHOU.004ACE6C
00409645  |. 66:8946 0D     MOV     WORD PTR DS:[ESI+D], AX

���̎��A�Ԃ�l(AX)���V�����n���l
*/

int iPointerOfUpdateExperienceInt = NULL;
WORD iNewValueOfUpdatedExperienceInt = 0; // �X�V���ꂽ��̏n���l
void OnTenshouExeUpdateExperienceIntExecute() {
	int iBushouID = iPointerOfUpdateExperienceInt - GAMEDATASTRUCT_BUSHOU_ADDRESSS; //
	iBushouID = iBushouID / sizeof(NB6BUSHOU);

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( CommonGlobalHandle::isBattleMode ) { // �푈���̂�

			double sub = iNewValueOfUpdatedExperienceInt - nb6bushouref[iBushouID].expint; // �V�l-���ݒl = ������ 
			if ( sub > 1 ) { // �}�C�i�X��P�̎��͏������Ȃ�
				nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int += (int)sub; // ���������Z�[�u�f�[�^�ɕێ�
				if ( nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int > 2000 ) { // 2000�ȏ��2000�ŊW
					nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int = 2000;
				}
				// �}�ɂ͐������Ȃ����n�m
				if ( TSModCommand::AllBushou::cmdNotGrowUpSuddenly ) {
					double coef = 1 + ((double)nb6savedata_ex.bushou_ex[iBushouID].ruiseki_exp_int / ((double)180 / (double)TSModCommand::AllBushou::cmdNotGrowUpSuddenly) ); // 1.0�`11.0���炢�܂ŁB���݂̗ݐϒl���傫���قǁAcoef���傫���B
					sub = sub / coef; // �ݐόo���l�������Ȃ����1�ɋ߂��l�Ŋ���B��������11�ɋ߂��l�Ŋ���B
					sub = ceil(sub); // �����_�ȉ��͐؂�グ��B
					iNewValueOfUpdatedExperienceInt = (int)sub + nb6bushouref[iBushouID].expint; // ���̐V���ȍ����ƌ��ݒl�̍��v���A���䂪���������V�l�B
				}
			}
		}
	}
}

/*
00409640  |. E8 27380A00    CALL    TENSHOU.004ACE6C
				�������� JMP TSMod.OnTSExeUpdateExperienceInt �Ə��������Ă����ۂ̏���
00409645  |. 66:8946 0D     MOV     WORD PTR DS:[ESI+D], AX
*/
int pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt	 =0x409640; // �֐��͂��̃A�h���X����AOnTenshouExeUpdateExperienceInt�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceInt =0x4ACE6C; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceInt  =0x409645; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeUpdateExperienceInt() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeUpdateExperienceInt

		mov iPointerOfUpdateExperienceInt, esi // esi ���Y�������̌��_

		mov iNewValueOfUpdatedExperienceInt, ax // ax���X�V���ꂽ�l�B


		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeUpdateExperienceIntExecute();

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

		mov ax, iNewValueOfUpdatedExperienceInt // ���ʂ��㏑���������p

		jmp pTenshouExeReturnLblFromOnTenshouExeUpdateExperienceInt
	}
}

char cmdOnTenshouExeJumpFromUpdateExperienceInt[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�T�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeUpdateExperienceInt() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeUpdateExperienceInt;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeUpdateExperienceInt  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromUpdateExperienceInt+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeUpdateExperienceInt), cmdOnTenshouExeJumpFromUpdateExperienceInt, 5, NULL); //5�o�C�g�̂ݏ�������
}



