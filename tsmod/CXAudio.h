// FileName		:	CXAUDIO2.hpp
// Contents		:	XAudio2 �N���X(�w�b�_�[��)
// UpDate		:	11/04/06(Wed)	
// LastUpDate	:	11/04/06(Wed)

#pragma once

#pragma pack(1)

#include "CWAVE.h"
#include "WinTarget.h"
#include <X3DAudio.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <d3dx9.h>


class CXAUDIO2
{
	private:
		static IXAudio2					*m_pXAudio2;	// XAudio2�C���^�[�t�F�[�X
		static IXAudio2MasteringVoice	*m_pMVoice;		// �}�X�^�[���H�C�X

		IXAudio2SourceVoice		*m_pSVoice;		// �\�[�X���H�C�X
		WAVEFORMATEX			m_WaveFmt;		// WAVE �t�H�[�}�b�g�\����
		CWAVE					m_WaveSound;	// WAVE ���y�f�[�^

	private:
		CXAUDIO2();
		CXAUDIO2(const CXAUDIO2&);
		CXAUDIO2& operator=(const CXAUDIO2&);

		void Exit();			// �I��
		bool Submit();			// ���y�f�[�^���L���[�ɑ��M
		bool CreateXAudio2();	// XAudio2 �C���^�[�t�F�[�X�̍쐬
		bool CreateMVoice();	// �}�X�^�[���H�C�X�̍쐬
		bool CreateSVoice();	// �\�[�X���H�C�X�̍쐬

	public:
		~CXAUDIO2();

		static CXAUDIO2* GetInst();	// �C���X�^���X�̐���

		IXAudio2SourceVoice* GetSVoice(); // �\�[�X�{�C�X�ւ̃|�C���^��Ԃ�
		IXAudio2MasteringVoice* GetMVoice(); // �}�X�^�[�{�C�X�ւ̃|�C���^��Ԃ�
		const WAVEFORMATEX& GetSVoiceFormat() const; // WaveFormat��Ԃ��B
		void GetDeviceDetails( XAUDIO2_DEVICE_DETAILS* deviceDatails); // �f�o�C�X�̏ڍׂ𓾂�

		bool	LoadWaveFile(const std::string &FineName);	// ���y�f�[�^�̃��[�h
		bool	LoadWaveFile(int iResID, HMODULE hModule);  // ���y�f�[�^�̃��[�h(DLL�ɓ����Ă郊�\�[�X��WAV�p)
		bool	Init();		// ������
		void	Play();		// �Đ�
		void	Stop();		// ��~
		void	Pause();	// �ꎞ��~

		bool	GetState();	// ��Ԃ𓾂�

		void	SetVolume(float Volume);	// �{�����[���̐ݒ�
		float	GetVolume()const;			// �{�����[�����擾

};

/*
	CXAudio2 �N���X�̃w�b�_�����ł��B
	CWAVE m_WaveSound �� WAVE �N���X�ł��B
	���[�h�֐��Ƀt�@�C������n���� WAVE �t�@�C����ǂݍ��񂾂肵�Ă���܂����A
	WAVE �N���X�Ƃ��ĕʋL���ɕ����悤�Ǝv���Ă��܂��̂ŁA���͋C�ɂ��Ȃ��Ă�OK�ł��B


	���� XAudio2 �N���X�ł����A
	��ȏ�C���X�^���X��K�v�Ƃ���P�[�X������Ȃ������̂ŃV���O���g���Ŏ������Ă܂��B
	�Ȃ̂ŁA�ǂ����� CXAUDIO2::GetInst ���ĂсA�C���X�^���X�𐶐�����K�v������܂��B

	�C���X�^���X�̐����͂���Ȋ����ł��B

	CXAUDIO2 *m_pCXAudio2 = CXAUDIO2::GetInst();

	����� XAudio2 �N���X�̃C���X�^���X�𐶐��ł��܂��B
*/