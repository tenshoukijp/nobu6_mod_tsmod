// FileName		:	CWAVE.cpp
// Contents		:	WAVE �t�@�C���N���X(������)
// UpDate		:	11/04/08(Fri)	
// LastUpDate	:	11/04/08(Fri)


#include "XAudioDbg.h"
#include "CWave.h"

//*******************************************
// �R���X�g���N�^
//*******************************************
CWAVE::CWAVE() : m_hMmio(0)
{
	ZeroMemory(&m_MmioInfo, sizeof(m_MmioInfo));
	ZeroMemory(&m_WaveFmtEx, sizeof(m_WaveFmtEx));
	m_WaveData.clear();
}

//*******************************************
// �f�X�g���N�^
//*******************************************
CWAVE::~CWAVE()
{
	this->Close();
}


//*******************************************
// WAVE �t�@�C���I�[�v��
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::Open(const std::string &FileName)
{
	m_FileName = FileName;

	// �t�@�C�����̃R�s�[(mmioOpen �ɓn��)
	std::vector<char> wFileName(FileName.begin(), FileName.end());
	wFileName.push_back('\0');

	char *str =  const_cast<char*>(FileName.c_str());

	ZeroMemory(&m_MmioInfo, sizeof(m_MmioInfo));

	if( !(m_hMmio = mmioOpen(&wFileName[0], &m_MmioInfo, MMIO_READ)) )
	{
		XAUDIO2_OUTDBG("File[ %s ]���J�������o���܂���ł����B\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}

//*******************************************
// WAVE �t�@�C�������[�h
// ����		�F	���\�[�X�h�c�A���W���[���n���h��
// �߂�l	�F	TRUE:����	FALSE:�ǂݍ���
//*******************************************
bool CWAVE::Open(int ResID, HMODULE hModule ) {
	LPBYTE lpTargetWavData;

	HRSRC hSrc = FindResourceA(hModule, MAKEINTRESOURCE(ResID), "WAVE");
	if ( !hSrc ) {
		XAUDIO2_OUTDBG("���\�[�X�ԍ�%d���������܂���B\n", ResID );
		return FALSE;
	}
	HGLOBAL hTargetWavData = LoadResource(hModule, hSrc);

	DWORD dwSize = SizeofResource(hModule, hSrc);

	// �ǂݍ��񂾃f�[�^�̃|�C���^�擾
	lpTargetWavData = (LPBYTE)LockResource(hTargetWavData);


	// �T�E���h�f�[�^���Đ�
	// sndPlaySoundA((LPCSTR)lpTargetWavData, SND_SYNC | SND_MEMORY);

	// ��������A���\�[�X�^�C�v��MmioInfo��mmioOpen�ւƌq���Ă䂭����
    char *m_pResourceBuffer = new char[dwSize];
	if( m_pResourceBuffer == NULL ) {
        return FALSE;
	}
    memcpy( m_pResourceBuffer, lpTargetWavData, dwSize );

    ZeroMemory( &m_MmioInfo, sizeof(m_MmioInfo) );
    m_MmioInfo.fccIOProc = FOURCC_MEM;
    m_MmioInfo.cchBuffer = dwSize;
    m_MmioInfo.pchBuffer = (char*) m_pResourceBuffer;

    m_hMmio = mmioOpen( NULL, &m_MmioInfo, MMIO_READ );

	return TRUE;
}




//*******************************************
// WAVE �t�@�C���N���[�Y
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::Close()
{
	if(mmioClose(m_hMmio, 0) == MMIOERR_CANNOTWRITE)
	{
		XAUDIO2_OUTDBG("File[ %s ]����鎖���o���܂���ł����B\n", m_FileName.c_str() );;
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// WAVE �w�b�_�����邩�`�F�b�N
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::CheckWAVEHeader()
{
	m_RIFFChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if( mmioDescend(m_hMmio, &m_RIFFChunk, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]�� WAVE �w�b�_�����݂��܂���B\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// FMT �`�����N�����邩�`�F�b�N
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::CheckFMTChunk()
{
	m_FMTChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	
	if( mmioDescend(m_hMmio, &m_FMTChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]�� FMT �`�����N�����݂��܂���B\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}

//*******************************************
// DAtA �`�����N�����邩�`�F�b�N
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::CheckDATAChunk()
{
	m_DATAChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if( mmioDescend(m_hMmio, &m_DATAChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]�� DATA �`�����N�����݂��܂���B\n", m_FileName.c_str() );
		return false;
	}

	return TRUE;
}

//*******************************************
// WAVEFORMATEX �Ƀf�[�^��ǂݍ���
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::ReadToWaveFmtEx()
{
	// mmioRead �͐�������Ɠǂ݂��񂾃T�C�Y��Ԃ�
	LONG wReadSize = mmioRead(m_hMmio, reinterpret_cast<HPSTR>(&m_WaveFmtEx), m_FMTChunk.cksize);

	// 0:����ȏ�ǂݍ��߂Ȃ��@-1:�t�@�C���̓ǂݎ��Ɏ��s
	switch(wReadSize)
	{
		//-----------------------------------------------------
		case 0:
				XAUDIO2_OUTDBG("File[ %s ]�͂���ȏ�ǂݍ��߂܂���B\n", m_FileName.c_str() );
			break;
		//-----------------------------------------------------
		case -1:
				XAUDIO2_OUTDBG("File[ %s ]�͓ǂ݂Ƃ�܂���ł����B\n", m_FileName.c_str() );
			break;
		//-----------------------------------------------------
		default:
			// �f�[�^�T�C�Y�������Ă邩�`�F�b�N����
			if(wReadSize != m_FMTChunk.cksize)
			{
				XAUDIO2_OUTDBG("File[ %s ]�̓ǂ݂��񂾃f�[�^�T�C�Y���قȂ�܂��B\n", m_FileName.c_str() );
				return FALSE;
			}
	}

	return TRUE;
}


//*******************************************
// m_WaveData �� WAVE �f�[�^��ǂݍ���
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CWAVE::ReadToWaveData()
{
	// WAVE �f�[�^�p�Ƀ��T�C�Y
	m_WaveData.resize(m_DATAChunk.cksize);

	if(mmioRead(m_hMmio, reinterpret_cast<HPSTR>(&m_WaveData[0]), m_DATAChunk.cksize) != m_DATAChunk.cksize)
	{
		XAUDIO2_OUTDBG("File[ %s ]�̓ǂ݂��񂾃f�[�^�T�C�Y���قȂ�܂��B\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// WAVE �t�@�C�������[�h
// ����		�F	�t�@�C����
// �߂�l	�F	TRUE:����	FALSE:�ǂݍ���
//*******************************************
bool CWAVE::Load(const std::string &FileName)
{
	// �t�@�C���I�[�v��
	if(!this->Open(FileName.c_str()))
		return FALSE;

	// WAVE �w�b�_�̃`�F�b�N
	if(!this->CheckWAVEHeader())
		return FALSE;

	// FMT �`�����N�̃`�F�b�N
	if(!this->CheckFMTChunk())
		return FALSE;

	// WAVEFORMATEX �\���̂Ƀf�[�^��ǂݍ���
	if(!this->ReadToWaveFmtEx())
		return FALSE;

	// �ǂݍ��݈ʒu��擪�ɖ߂�
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA �`�����N�̃`�F�b�N
	if(!this->CheckDATAChunk())
		return FALSE;

	// WAVE �f�[�^��ǂݍ���
	if(!this->ReadToWaveData())
		return FALSE;

	// �t�@�C�������
	this->Close();

	return TRUE;

}


//*******************************************
// WAVE �t�@�C�������[�h
// ����		�F	���\�[�X�h�c�A���W���[���n���h��
// �߂�l	�F	TRUE:����	FALSE:�ǂݍ���
//*******************************************
bool CWAVE::Load(int iResID, HMODULE hModule)
{
	// �t�@�C���I�[�v��
	if(!this->Open(iResID, hModule))
		return FALSE;

	// WAVE �w�b�_�̃`�F�b�N
	if(!this->CheckWAVEHeader())
		return FALSE;

	// FMT �`�����N�̃`�F�b�N
	if(!this->CheckFMTChunk())
		return FALSE;

	// WAVEFORMATEX �\���̂Ƀf�[�^��ǂݍ���
	if(!this->ReadToWaveFmtEx())
		return FALSE;

	// �ǂݍ��݈ʒu��擪�ɖ߂�
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA �`�����N�̃`�F�b�N
	if(!this->CheckDATAChunk())
		return FALSE;

	// WAVE �f�[�^��ǂݍ���
	if(!this->ReadToWaveData())
		return FALSE;

	// �t�@�C�������
	this->Close();

	return TRUE;

}





//*******************************************
// �t�@�C�������擾
// ����		�F	�Ȃ�
// �߂�l	�F	WAVE �t�H�[�}�b�g�\���̂ւ̎Q��
//*******************************************
const std::string& CWAVE::GetFileName()const
{
	return m_FileName;
}


//*******************************************
// WAVE �t�H�[�}�b�g���擾
// ����		�F	�Ȃ�
// �߂�l	�F	WAVE �t�H�[�}�b�g�\���̂ւ̎Q��
//*******************************************
const WAVEFORMATEX& CWAVE::GetWaveFmtEx()const
{
	return m_WaveFmtEx;
}

//*******************************************
// WAVE �f�[�^���擾
// ����		�F	�Ȃ�
// �߂�l	�F	WAVE �t�H�[�}�b�g�\���̂ւ̎Q��
//*******************************************
const BYTE* CWAVE::GetWaveData()const
{
	return &m_WaveData[0];
}

//*******************************************
// WAVE �f�[�^�̃T�C�Y���擾
// ����		�F	�Ȃ�
// �߂�l	�F	WAVE �t�H�[�}�b�g�\���̂ւ̎Q��
//*******************************************
const size_t CWAVE::GetWaveSize()const
{
	return m_WaveData.size();
}
