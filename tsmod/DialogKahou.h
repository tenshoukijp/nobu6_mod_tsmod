#pragma once


#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "OutputDebugStream.h"

enum DialogKahouMode { KAHOU_MODE_GAMEEND = -1, KAHOU_MODE_NO = 0, KAHOU_MODE1 = 1,  KAHOU_MODE2 = 2, KAHOU_MODE3 = 3, KAHOU_MODE4 = 4};

class DialogKahou {

	private:
		HBITMAP hBitmap, hPrevBitmap;  // �r�b�g�}�b�v�̃n���h��
		HDC hDC, hCompatDC;            // �f�o�C�X�R���e�L�X�g�n���h��

		BITMAP bmp_info;

		char m_szKahouName[256];
		char m_szKahouRetsuden[256];

		RECT *m_lprcSrc1;
		RECT *m_lprcSrc2;

		RECT m_BmpPosition;

		PAINTSTRUCT	ps;

	public:

		BOOL is_movie_mode;

		BITMAP* getBmpInfo() {
			return &bmp_info; 
		}

		HDC* getHCompatDC() {
			return &hCompatDC;
		}
		int getCurSrc1RectDidth() {
			return m_lprcSrc1->right - m_lprcSrc1->left;
		}
		int getCurSrc1RectHeight() {
			return m_lprcSrc1->bottom - m_lprcSrc1->top;
		}
		int getCurSrc2RectDidth() {
			return m_lprcSrc2->right - m_lprcSrc2->left;
		}
		int getCurSrc2RectHeight() {
			return m_lprcSrc2->bottom - m_lprcSrc2->top;
		}
		void CreateDCs() {
			/*
			HDC *pAddress = (HDC *)HANDLESTRUCT_HDC_ADDRESS;
			hDC = *pAddress;
			*/
    		if (!hDC) {
				 // �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����擾����
				hDC = ::GetDC( CommonGlobalHandle::tenshouWND );

				 // �������f�o�C�X�R���e�L�X�g���쐬����
				hCompatDC = ::CreateCompatibleDC( hDC );
			}
		}

		void DeleteDCs() {
			if (hDC) {
				// �������f�o�C�X�R���e�L�X�g���폜����
				::DeleteDC( hCompatDC );   
				// �E�B���h�E�̃f�o�C�X�R���e�L�X�g�n���h�����������
				::ReleaseDC( CommonGlobalHandle::tenshouWND, hDC );
			}
		}

		void setCurSrcRect(
			  CONST RECT *lprcSrc1,  // �ŏ��̒����`
			  CONST RECT *lprcSrc2   // 2 �Ԗڂ̒����`
		) {
			m_lprcSrc1 = (RECT *)lprcSrc1;
			m_lprcSrc2 = (RECT *)lprcSrc2;
		}

		// �ƕ��`�悷��|�W�V�����̐ݒ�
		void setKahouBMPPosition(CONST RECT *BmpPosition) {
			m_BmpPosition.left   = BmpPosition->left;
			m_BmpPosition.right  = BmpPosition->right;
			m_BmpPosition.top    = BmpPosition->top;
			m_BmpPosition.bottom = BmpPosition->bottom;
		}

		// �ƕ��`�悷��X���W�𓾂�
		int getKahouBMPPositionX() {
			return m_BmpPosition.left;
		}
		// �ƕ��`�悷��Y���W�𓾂�
		int getKahouBMPPositionY() {
			return m_BmpPosition.top;
		}

	public:
		static DialogKahouMode eModeKahou;

		static char szMovieKahouTmpName[256];

		DialogKahou() {
			m_szKahouName[0] = NULL;
			m_szKahouRetsuden[0] = NULL;
		}

		void CheckDialogKahou (  CONST RECT *lprcSrc1, CONST RECT *lprcSrc2 );

};

