

#include "TimerMessageBox.h"


// TimerMessageBox.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

/***********************************************************************
��ʂɁA���Ԃ𐧌����ă��b�Z�[�W �{�b�N�X��\��������ꍇ�A
�w�肵�����Ԃ��o�߂�����Ŏ����I�ɕ���ʏ�̃_�C�A���O
�{�b�N�X����������K�v������܂��B

���̕��@�̖��_�́AWindows ���񋟂���W�����b�Z�[�W
�{�b�N�X�̋@�\�������Ă��܂����Ƃł��B

�ȉ��̗�ł́AMessageBox �֐����g�p���āA�w�肵�����Ԃ̌o�ߌ��
�����I�ɕ��郁�b�Z�[�W �{�b�N�X���쐬������@�������܂��B

���̗�Ɋւ��āA���̓_�ɒ��ӂ��Ă��������B
�E���̗�ł́A�w�肵�����Ԃ��o�߂�����ɃC�x���g�𔭐�������
�@Windows �^�C�}�[���g�p���܂��B
�E�^�C�}�[ �C�x���g����������ƁAPostQuitMessage API ���g�p���āA
�@MessageBox ���g�p���Ă��郂�[�_�� ���b�Z�[�W ���[�v�𔲂��܂��B
�E�� : WM_QUIT �́A���C�� ���b�Z�[�W �L���[�Ŏ擾����Ȃ��悤�ɁA
�@���b�Z�[�W �L���[����폜����K�v������܂��B

�T���v�� �R�[�h
�� : ���̃T���v���ł́A�T�[�r�X���� MessageBox ���Ăяo���ꍇ��
MB_SERVICE_NOTIFICATION �t���O�̎g�p�̓T�|�[�g���Ă��܂���B
MB_SERVICE_NOTIFICATION ���w�肳�ꂽ�ꍇ�A���b�Z�[�W �{�b�N�X�E�B���h�E�́A
MessageBox ���Ăяo���v���Z�X�ɂ���ď��L����܂���B

***********************************************************************/



#include "WinTarget.h"




/***********************************************************************
*
*  Global variables
*
***********************************************************************/
HWND g_hwndTimedOwner;
BOOL g_bTimedOut;


/***********************************************************************
*
*  MessageBoxTimer
*
*      The timer callback function that posts the fake quit message.
*      This function causes the message box to exit because the message box
*      has determined that the application is exiting.
*
***********************************************************************/
void CALLBACK MessageBoxTimer(HWND hwnd,
							  UINT uiMsg,
							  UINT idEvent,
							  DWORD dwTime)
{
	g_bTimedOut = TRUE;
	if (g_hwndTimedOwner)
		EnableWindow(g_hwndTimedOwner, TRUE);
	PostQuitMessage(0);
}


/***********************************************************************
*
*  TimedMessageBox
*
*      The same as the standard MessageBox, except that TimedMessageBox
*      also accepts a timeout. If the user does not respond within the
*      specified timeout, the value 0 is returned instead of one of the
*      ID* values.
*
***********************************************************************/
int TimedMessageBox(HWND hwndOwner,
					LPCTSTR pszMessage,
					LPCTSTR pszTitle,
					UINT flags,
					DWORD dwTimeout)
{
	UINT idTimer;
	int iResult;

	g_hwndTimedOwner = NULL;
	g_bTimedOut = FALSE;

	if (hwndOwner && IsWindowEnabled(hwndOwner))
		g_hwndTimedOwner = hwndOwner;

	//
	// Set a timer to dismiss the message box.
	idTimer = SetTimer(NULL, 0, dwTimeout, (TIMERPROC)MessageBoxTimer);

	iResult = MessageBox(hwndOwner, pszMessage, pszTitle, flags);

	//
	// Finished with the timer.
	KillTimer(NULL, idTimer);

	//
	// See if there is a WM_QUIT message in the queue if we timed out.
	// Eat the message so we do not quit the whole application.
	if (g_bTimedOut)
	{
		MSG msg;
		PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
		iResult = -1;
	}

	return iResult;
}



/***********************************************************************
*
*  WinMain
*
*      Program entry point. Demonstrate TimedMessageBox().
*
***********************************************************************/


void TimerMessageBoxTest(void) {

	// �^�C�}�[���b�Z�[�W�{�b�N�X
	//--------------------------------------------------------------------------
	UINT uiResult = 0;

	//
	// Ask the user a question. Give the user five seconds to
	// answer the question.
	uiResult = TimedMessageBox(NULL,
		"���b�Z�[�W��",
		"�^�C�g��",
		MB_OK,
		// NULL first parameter is important.
		3000); // �����ŏ�����~���b�B(���̏ꍇ3�b)
}

