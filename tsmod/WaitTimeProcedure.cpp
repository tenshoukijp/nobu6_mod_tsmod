#include "WinTarget.h"

#include "CommonGlobalHandle.h"

// �~���b���҂B���b�Z�[�W�����͌p�������B
void WaitTimeProcedure(int iWaitTime) {
	DWORD preTimeGetTime = timeGetTime();

	MSG msg;
	while(1) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����

		DWORD curTimeGetTime = timeGetTime();
		// �K��̊Ԋu���Ԃ��߂���
		if ( int(curTimeGetTime - preTimeGetTime) > int(iWaitTime) ) {
			break;
		} 

		// ���b�Z�[�W����������
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

/*
BOOL GetMessage(
  LPMSG lpMsg,
  HWND hWnd,
  UINT wMsgFilterMin,
  UINT wMsgFilterMax
);


lpMsg�́AMSG�\���̂̃A�h���X���w�肵�܂��B 
�֐����琧�䂪�Ԃ�ƁA���̍\���̂Ɏ擾�������b�Z�[�W�̏�񂪊i�[����܂��B 

hWnd�́A���b�Z�[�W���擾����E�C���h�E�̃n���h�����w�肵�܂��B
NULL���w�肷��ƁA�Ăяo�����X���b�h�Ɋ֘A�t�����Ă���S�Ă�
�E�C���h�E�ւ̃��b�Z�[�W���擾�ł��܂��B 

wMsgFilterMin��wMsgFilterMax�́A�擾�Ώۂ̃��b�Z�[�W�͈̔͂��w�肵�܂��B
�����̈�����0���w�肵���ꍇ�A�S�Ẵ��b�Z�[�W���擾����邱�ƂɂȂ�܂��B

�߂�l�́AWM_QUIT�ȊO�̃��b�Z�[�W���擾�����ꍇ��0�ȊO�̒l���Ԃ�A
WM_QUIT���擾�����ꍇ��0���Ԃ�܂��B �����̃A�h���X�������ł���ꍇ�Ȃǂ́A-1���Ԃ邱�Ƃ�����܂��B 
*/


/*
���b�Z�[�W���擾������A���͂�����E�C���h�E�v���V�[�W���ɑ���Ȃ���΂Ȃ�܂���B
���̂��Ƃ����b�Z�[�W�̃f�B�X�p�b�`�ƌĂсADispatchMessage�ōs�����ƂɂȂ�܂��B
DispatchMessage�́A������MSG�\���̂�hwnd�����o��p���� �E�C���h�E�v���V�[�W���̃A�h���X���擾����֐����Ăяo���A
���̌�AMSG�\���̂̃����o�������Ƃ��ăE�C���h�E�v���V�[�W�����Ăяo���܂��B 

LRESULT DispatchMessage(
  CONST MSG *lpmsg
);


lpmsg�́AMSG�\���̂̃A�h���X���w�肵�܂��B
��ʂɂ́AGetMessage�ŏ���������MSG�\���̂����̂܂܎w�肷�邱�ƂɂȂ�ł��傤�B
�߂�l�́A�E�C���h�E�v���V�[�W�����Ԃ����l�ƂȂ�܂��B 

���b�Z�[�W�̎擾�ƃf�B�X�p�b�`�̕��@�����Ă����Ƃ���ŁA
����2�̓���ƃ��b�Z�[�W�L���[�̎d�g�݂�����x�m�F���Ă݂܂��傤�B 

���A�E�C���h�E�Ń}�E�X�̍��{�^���������ꂽ�Ƃ��āA
�V�X�e���͂��̃E�C���h�E��������A�v���P�[�V������
���b�Z�[�W�L���[��WM_LBUTTONDOWN���i�[���܂��B

�A�v���P�[�V�����́A���̊i�[���ꂽ���b�Z�[�W��GetMessage�Ŏ擾���A
�����DispatchMessage�ŃE�C���h�E�v���V�[�W���ɑ��邱�ƂɂȂ�܂��B
���̂Ƃ��A���Ƃ��E�C���h�E�ɐV���ȃC�x���g���������ă��b�Z�[�W���������ꂽ�Ƃ��Ă��A
�V�X�e���͂�������b�Z�[�W�L���[�Ɋi�[���邾���ł�����A���̃��b�Z�[�W�����������̂́A
�ĂуA�v���P�[�V������GetMessage��DispatchMessage���Ăяo���Ƃ��ƂȂ�܂��B 

�����̂��Ƃ��番����悤�ɁA�A�v���P�[�V���������b�Z�[�W���������邽�߂ɂ́A
�₦��GetMessage��DispatchMessage���Ăяo���Ȃ���΂Ȃ�܂��񂩂�A
����2�̏����̓��[�v���Ƃ��Ď��s���邱�ƂɂȂ�܂��B 
*/

/*
TranslateMessage�ɂ��� 

���b�Z�[�W���[�v�ŌĂ΂�Ă���TranslateMessage�́A
�L�[�{�[�h���b�Z�[�W���當�����b�Z�[�W�𐶐����邽�߂ɑ��݂��܂��B 

�ʏ�A�A�v���P�[�V�������L�[�������ƁAWM_KEYDOWN�Ƃ����L�[�{�[�h���b�Z�[�W�������A
���Ƃ���A�L�[�Ȃ�wParam�ɂ�A�L�[�̉��z�L�[�R�[�h�ł���0x41���i�[����܂��B

�������A���z�L�[�R�[�h�����́A������A���啶����A�Ȃ̂���������a�Ȃ̂�������܂��񂩂�A
���̂悤�ȏꍇ�͕����R�[�h��wParam�Ƃ��Ď��������b�Z�[�W���K�v�ɂȂ�܂��B

TranslateMessage��WM_KEYDOWN�����o����ƁA�������b�Z�[�W�ł���WM_CHAR�����b�Z�[�W�L���[�Ɋi�[����A
����GetMessage���Ăяo�����ۂɎ擾����邱�ƂɂȂ�܂��B 
*/ 
