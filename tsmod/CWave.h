// FileName		:	CWAVE.hpp
// Contents		:	WAVE �t�@�C���N���X
// UpDate		:	11/04/08(Fri)	
// LastUpDate	:	11/04/08(Fri)

#pragma once

#pragma pack(1)

#include "xaudio2.h"
#include <vector>
#include <string>

class CWAVE
{
	private:
		HMMIO				m_hMmio;		// MMIO �n���h��
		MMIOINFO			m_MmioInfo;		// MMIO ���\����
		MMCKINFO			m_RIFFChunk;	// RIFF �`�����N
		MMCKINFO			m_FMTChunk;		// FMT �`�����N
		MMCKINFO			m_DATAChunk;	// DATA �`�����N
		WAVEFORMATEX		m_WaveFmtEx;	// WAVE �t�H�[�}�b�g�\����	
		std::vector<BYTE>	m_WaveData;		// ���y�f�[�^	
		std::string			m_FileName;		// �t�@�C����

	private:
		bool Open(const std::string &FileName);		// WAVE �t�@�C���I�[�v��
		bool Open(int iResID, HMODULE hModule);		// WAVE �t�@�C���I�[�v��(DLL�̃��\�[�X�ɓ����Ă�p)
		bool Close();								// WAVE �t�@�C���N���[�Y
		bool ReadToWaveFmtEx();						// WAVEFORMATEX �Ƀf�[�^��ǂݍ���
		bool ReadToWaveData();						// m_WaveData �� WAVE �f�[�^��ǂݍ���
		bool CheckWAVEHeader();						// WAVE �w�b�_�����邩�`�F�b�N
		bool CheckFMTChunk();						// FMT �`�����N�����邩�`�F�b�N
		bool CheckDATAChunk();						// DATA �`�����N�����邩�`�F�b�N
		
	public:
		CWAVE();
		~CWAVE();

		bool Load(const std::string &FileName);			// ���y�f�[�^�̃��[�h
		bool Load(int iResID, HMODULE hModule);			// ���y�f�[�^�̃��[�h (DLL���̃��\�[�X��WAV�p)

		const WAVEFORMATEX&		GetWaveFmtEx()const;	// WAVE �t�H�[�}�b�g���擾
		const std::string&		GetFileName()const;		// �t�@�C�����̎擾
		const BYTE*				GetWaveData()const;		// Wave �f�[�^���擾
		const size_t			GetWaveSize()const;		// Wave �f�[�^�̃T�C�Y���擾
};
