// FileName		:	CXAUDIO2.cpp
// Contents		:	XAudio2�N���X(������)
// UpDate		:	11/04/06(Wed)	
// LastUpDate	:	11/04/06(Wed)


#include "WinTarget.h"
#include "CXAudio.h"
#include "XAudioDbg.h"



IXAudio2				*CXAUDIO2::m_pXAudio2	= NULL;
IXAudio2MasteringVoice	*CXAUDIO2::m_pMVoice	= NULL;


//*******************************************
// �R���X�g���N�^
//*******************************************
CXAUDIO2::CXAUDIO2() : m_pSVoice(NULL)
{
	ZeroMemory(&m_WaveFmt, sizeof(m_WaveFmt));
}


//*******************************************
// �f�X�g���N�^
//*******************************************
CXAUDIO2::~CXAUDIO2()
{
	this->Exit();
}



/*
	�ŁA�������ł����A
	�p�u���b�N�ȃ����o���\�b�h CXAUDIO2::Init ���Ă΂��� XAudio2 �̏��������n�܂�܂��B
	���͏����������̂悤�Ɏ������Ă܂��B

	�������̗���Ƃ��Ă�
	�ECOM (Component Object Model)�̏�����
	�EXAudio2 �C���^�[�t�F�[�X�̍쐬
	�E�}�X�^�[���H�C�X�̍쐬
	�EWAVE �̓ǂݍ���
	�E�\�[�X���H�C�X�̍쐬
	�E�\�[�X���H�C�X�L���[�Ƀf�[�^�𑗐M

	�Ƃ��������ł��B
*/
 
//*******************************************
// ������
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����������		FALSE:���������s
//*******************************************
bool CXAUDIO2::Init()
{
	// COM �̏�����
	if( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
	{
		if( FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) )
		{
			XAUDIO2_OUTDBG("[XAudio2]COM�̏������Ɏ��s���܂����B\n");
			return FALSE;
		}
	}

	// XAudio2 �C���^�[�t�F�[�X�̍쐬
	if(!this->CreateXAudio2())
		return FALSE;

	// �}�X�^�[�{�C�X�̍쐬
	if(!this->CreateMVoice())
		return FALSE;

	XAUDIO2_OUTDBG("XAudio2 �֘A�̏������ɐ������܂����B\n");
	return TRUE;
}

/*
	���́Athis->LoadWaveFile �����o���\�b�h���Ă�ł��܂���
	�EWAVE �ǂݍ���
	�E�\�[�X���H�C�X�̍쐬
	�E�f�[�^���M

	������Ă܂��B

	WAVE �̓ǂݍ��݂ɂ� WAVE �N���X�ł���
	CWAVE::Load(const std::string &FileName) �œǂݍ��ނ̂ł���
	����� WAVE �ǂݍ��݂͊��ɏo���Ă�����̂Ƃ��Ęb��i�߂܂��B
	�R���Ɋւ��Ă� Part2 ���炢�� WAVE �N���X���Љ�鎞�ɂł����b���܂��B


	�]�k�ł����ACXAUDIO2 �N���X�̃����o�� CWAVE �N���X���������Ă܂���
	�ꏏ�ɓ���Ȃ����������Ǝv���Ă܂��B
	���Ȃ� WAVE ���Ǘ�����N���X�Ȃ񂩂����Ă�������������܂���ˁB
	�܁A����͒��ɓ���Ƃ��܂����B
*/
//*******************************************
// �ȃf�[�^�̓ǂݍ���
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:�ǂݍ��ݐ���	FALSE:�ǂݍ��ݎ��s
//*******************************************
bool CXAUDIO2::LoadWaveFile(const std::string &FileName)
{
	// WAVE �N���X���g���ĉ��y���[�h
	if(!m_WaveSound.Load(FileName))
		return FALSE;

	// �\�[�X���H�C�X�̍쐬
	if(!this->CreateSVoice())
		return FALSE;

	// �ǂݍ��񂾉��y�f�[�^���L���[�ɑ��M
	this->Submit();

	return TRUE;
}

//*******************************************
// �ȃf�[�^�̓ǂݍ���
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:�ǂݍ��ݐ���	FALSE:�ǂݍ��ݎ��s
//*******************************************
bool CXAUDIO2::LoadWaveFile(int iResID, HMODULE hModule)
{
	// WAVE �N���X���g���ĉ��y���[�h
	if(!m_WaveSound.Load(iResID, hModule))
		return FALSE;

	// �\�[�X���H�C�X�̍쐬
	if(!this->CreateSVoice())
		return FALSE;

	// �ǂݍ��񂾉��y�f�[�^���L���[�ɑ��M
	this->Submit();

	return TRUE;
}


/*
	COM �̏������͂���ŏI���Ȃ̂�
	���� XAudio2 �C���^�[�t�F�[�X���쐬���郁���o���\�b�h�ł��B

	XAudio2Create ���\�b�h���ĂԂ����ł��B
	�������� IXAudio2* �I�u�W�F�N�g�̃A�h���X��n���Ă��܂��B
	���������|�C���g�ŁAMSDN �ɂ���
*/
//*******************************************
// XAudio2 �C���^�[�t�F�[�X�̍쐬
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����		FALSE:���s
//*******************************************
bool CXAUDIO2::CreateXAudio2()
{
	UINT wFlag = 0;

#ifndef NDEBUG
	wFlag |= XAUDIO2_DEBUG_ENGINE;
#endif

	if( FAILED( XAudio2Create(&m_pXAudio2, wFlag)) )
	{
		XAUDIO2_OUTDBG("XAudio2 �C���^�[�t�F�X�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("XAudio2 �C���^�[�t�F�X�̍쐬�ɐ������܂����B\n");
	return TRUE;
}

/*
	IXAudio2::CreateMasteringVoice ���\�b�h�Ń}�X�^�[���H�C�X���쐬���܂��B
	�������� IXAudio2MasteringVoice* �I�u�W�F�N�g�̃A�h���X��n���܂��B
	��Ɉ���������̂ł����A�f�t�H���g�̒l���g�p���܂����B

	IXAudio2::CreateMasteringVoice ���\�b�h�Ń}�X�^�[���H�C�X���쐬���܂��B
	�������� IXAudio2MasteringVoice* �I�u�W�F�N�g�̃A�h���X��n���܂��B
	��Ɉ���������̂ł����A�f�t�H���g�̒l���g�p���܂����B

	MSDN �ɂ���
	Several mastering voices can exist at once, but only one of them can be started.
	Only one device can be used by XAudio2 at a time.

	(��ȏ�̃}�X�^�[���H�C�X���������ɑ��݂��Ă����v�ŁA
	�X�^�[�g�ł���͈̂�x�ɂ�������ł��B
	XAudio2 ���g�p�ł���f�o�C�X�͈�x�Ɉ�����ł��B)

	�炵���̂ŁA����Ă����͂Ȃ������ł�������Ӗ��͂Ȃ��悤�ł��B

	�܂��A
	It is illegal to call CreateMasteringVoice from within a callback. 
	If CreateMasteringVoice is called within a callback XAUDIO2_E_INVALID_CALL will be returned.

	(CreateMasteringVoice ���\�b�h�́A�R�[���o�b�N�֐��̒�����Ăяo���Ɩ����ɂȂ�B
	�����Ăяo�����ꍇ�AXAUDIO2_E_INVALID_CALL ���Ԃ���܂��B)


	�Ƃ����̂�����̂� �R�[���o�b�N������̃��\�b�h�Ăяo��������Ă͑ʖڂȂ悤�ł��B
	���̕Ӓ��ӂ��Ă�����OK�݂����Ȃ̂ŕ֗��ł���ˁ`�B
*/
//*******************************************
// �}�X�^�[���H�C�X�̍쐬
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����		FALSE:���s
//*******************************************
bool CXAUDIO2::CreateMVoice()
{
	if( FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMVoice)) )
	{
		XAUDIO2_OUTDBG("�}�X�^�����O���H�C�X�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("�}�X�^�����O���H�C�X�̍쐬�ɐ������܂����B\n");

	return TRUE;
}


/*
�\�[�X���H�C�X�̍쐬�ɂ�
IXAudio2::CreateSourceVoice ���\�b�h���g���܂��B
�������� IXAudio2SourceVoice* �I�u�W�F�N�g�̃A�h���X��n���܂��B
�������� WAVEFORMATEX �\���̂�n���܂��B
���� WAVE �t�@�C����ǂݍ���ł���O��Ȃ̂ŃR�R�ɂ����n���܂��B


	http://msdn.microsoft.com/ja-jp/library/bb633468(v=vs.85).aspx
	[[IXAudio2::CreateSourceVoice]]
	�\�[�X �{�C�X���쐬���A�ݒ肵�܂��B

	�R�s�[
	 HRESULT CreateSourceVoice(
	  IXAudio2SourceVoice **ppSourceVoice,
	  const WAVEFORMATEX *pSourceFormat,
	  UINT32 Flags = 0,
	  float MaxFrequencyRatio = XAUDIO2_DEFAULT_FREQ_RATIO,
	  IXAudio2VoiceCallback *pCallback = NULL,
	  const XAUDIO2_VOICE_SENDS *pSendList = NULL,
	  const XAUDIO2_EFFECT_CHAIN *pEffectChain = NULL
	);
	�p�����[�^
	ppSourceVoice
	���������ꍇ�A�V���� IXAudio2SourceVoice �I�u�W�F�N�g�ւ̃|�C���^�[��Ԃ��܂��B
	pSourceFormat
	WAVEFORMATEX �\���̂ւ̃|�C���^�[�ł��B���̍\���̂ɂ́A�\�[�X �{�C�X�ɑ����邷�ׂẴI�[�f�B�I �o�b�t�@�[�Ɋ��҂����`�����܂܂�܂��B 
	��    XMA�AADPCM�AxWMA �ȂǁAWAVEFORMATEX �ɂ���Ē񋟂������������̏���K�v�Ƃ���f�[�^�`���ɂ́A���̌`���\���̂̍ŏ��̃����o�[�Ƃ��� WAVEFORMATEX �\���̂�����܂��B�����̌`���̂����ꂩ�Ń\�[�X �{�C�X���쐬����ꍇ�́A�`���̍\���̂� WAVEFORMATEX �\���̂Ƃ��ăL���X�g���A����� pSourceFormat �̒l�Ƃ��Ďg�p���܂��B
	Flags
	�\�[�X �{�C�X�̓�����w�肷��t���O�ł��B�t���O�ɂ� 0 �܂��͎��̂����ꂩ�̒l���w�肵�܂��B
	�l ���� 
	XAUDIO2_VOICE_NOPITCH �{�C�X�ɑ΂���s�b�` �R���g���[���͎g�p�ł��܂���B 
	XAUDIO2_VOICE_NOSRC �{�C�X�ɑ΂��ăT���v�� ���[�g�ϊ��͎��s�ł��܂���B�{�C�X�̏o�͂͂��ׂē������̓T���v�� ���[�g�ł���K�v������܂��B
	��    XAUDIO2_VOICE_NOSRC �t���O�ɂ���āAXAUDIO2_VOICE_NOPITCH �t���O���w�肳��Ă��邩�̂悤�Ƀ{�C�X�����삵�܂��B
	 
	XAUDIO2_VOICE_USEFILTER ���̃{�C�X�ł́A�t�B���^�[ �G�t�F�N�g���g�p�\�łȂ���΂Ȃ�܂���B 
	XAUDIO2_VOICE_MUSIC �{�C�X�́A�o�b�N�O���E���h �~���[�W�b�N�̍Đ��Ɏg�p����܂��B�{�C�X�́A���[�U�[���I�������~���[�W�b�N�Ŏ����I�ɒu���������܂��B 

	MaxFrequencyRatio
	���̃{�C�X�ɐݒ�ł���ő勖�e���g����ł��B���̈����̒l�́AXAUDIO2_MIN_FREQ_RATIO ���� XAUDIO2_MAX_FREQ_RATIO �܂ł̊ԂłȂ���΂Ȃ�܂���B�ȍ~�� IXAudio2SourceVoice::SetFrequencyRatio �ւ̌Ăяo���́AXAUDIO2_MIN_FREQ_RATIO �� MaxFrequencyRatio �̊ԂŃN�����v����܂��B
	���̈����̍ő�l�́AXAUDIO2_MAX_FREQ_RATIO �Ƃ��Ē�`����܂��B����ɂ��A�ő�� 10 �I�N�^�[�u�܂Ńs�b�`���グ�邱�Ƃ��ł��܂��B
	MaxFrequencyRatio �� 1.0 �����̏ꍇ�A�{�C�X�͍쐬�シ���ɁA����� 1.0 �ł͂Ȃ����̎��g������g�p���܂��B
	Xbox 360:  XMA �{�C�X�̏ꍇ�́AMaxFrequencyRatio ��������у{�C�X�̃T���v�� ���[�g�ɂ��āA����ɕʂ̐���������܂��B������ 2 �̐��l�̐ς��A1 �`�����l�� �{�C�X�̏ꍇ�� XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO ���A����ȊO�̃`�����l�����̃{�C�X�̏ꍇ�� XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL �𒴂��邱�Ƃ͂ł��܂���BMaxFrequencyRatio �Ɏw�肳��Ă���l���A�w��̌`���ɂ͍�������ꍇ�ACreateSourceVoice �ւ̌Ăяo���͎��s���A�f�o�b�O ���b�Z�[�W���\������܂��B

	��    MaxFrequencyRatio �ŉ\�ȍł��Ⴂ�l���g�p���邱�ƂŁAXAudio2 �̃������[�g�p�ʂ����炷���Ƃ��ł��܂��B
	pCallback
	�N���C�A���g���w�肵���R�[���o�b�N �C���^�[�t�F�C�X�ւ̃|�C���^�[�ł��� IXAudio2VoiceCallback �ł��B
	pSendList
	�\�[�X �{�C�X�ɑ΂����A�̃f�X�e�B�l�[�V���� �{�C�X��\�� XAUDIO2_VOICE_SENDS �\���̂̃��X�g�ւ̃|�C���^�[�ł��BpSendList �� NULL �̏ꍇ�A���M���X�g�́A����ŁA�ŏ��ɍ쐬���ꂽ�}�X�^�[ �{�C�X�ւ̒P��̏o�͂ƂȂ�܂��B
	pEffectChain
	�\�[�X �{�C�X�Ŏg�p����G�t�F�N�g �`�F�[����\�� XAUDIO2_EFFECT_CHAIN �\���̂̃��X�g�ւ̃|�C���^�[�ł��B
	�߂�l
	����Ɏ��s���ꂽ�ꍇ�� S_OK ��Ԃ��A���s�����ꍇ�̓G���[ �R�[�h��Ԃ��܂��BXAudio2 �ŗL�̃G���[ �R�[�h�̐����ɂ��ẮA�uXAudio2 �G���[ �R�[�h�v���Q�Ƃ��Ă��������B 

	����@
	�\�[�X �{�C�X�́A�N���C�A���g����̃I�[�f�B�I �f�[�^��ǂݎ��܂��B�\�[�X �{�C�X�́A�f�[�^���������āA�f�[�^�� XAudio2 �����O���t�ɑ��M���܂��B

	�\�[�X �{�C�X�ɂ́A�f�[�^���A�\�[�X �t�H�[�}�b�g �T���v�� ���[�g����{�C�X���M���X�g�ɕK�v�ȏo�̓��[�g�ɕϊ����邽�߂́A�σ��[�g �T���v�� ���[�g�ϊ����܂܂�܂��BNULL ���M���X�g���g�p����Ă���ꍇ�́A�^�[�Q�b�g�̃T���v�� ���[�g�́A�}�X�^�����O �{�C�X�̓��̓T���v�� ���[�g�ɂȂ�܂��B�P��̃{�C�X�� pSendList �Œ񋟂���Ă���ꍇ�́A���̃{�C�X�̓��̓T���v�� ���[�g���^�[�Q�b�g ���[�g�ɂȂ�܂��B�����̃{�C�X�� pSendList �Œ񋟂���Ă���ꍇ�́A�\�[�X �{�C�X�̏o�̓{�C�X�����ׂē����T���v�� ���[�g�Ŏ��s�����K�v������܂��B

	�}�X�^�[ �{�C�X�����݂��Ȃ���΁A�\�[�X �{�C�X�܂��̓T�u�~�b�N�X �{�C�X���쐬���邱�Ƃ͂ł��܂���B�܂��A�\�[�X �{�C�X�܂��̓T�u�~�b�N�X �{�C�X�����݂��Ă���Ԃ́A�}�X�^�[ �{�C�X��j�����邱�Ƃ͂ł��܂���B

	�\�[�X �{�C�X�́A��ɁA�T�u�~�b�N�X �{�C�X�܂��̓}�X�^�[ �{�C�X�̑O�ɏ�������܂��B���������āA���������𐧌䂷�� ProcessingStage �p�����[�^�[�͕K�v�͂���܂���B

	�ŏ��ɍ쐬�����Ƃ��A�\�[�X �{�C�X�͒�~������ԂɂȂ��Ă��܂��B

	XAudio2 �ł́A�����`���̃{�C�X�� 1 �̓����������[ �v�[�����g�p����̂ŁA�����̃{�C�X���쐬���ꂽ��A�j�������ꍇ�ɁA�{�C�X�̃������[���蓖�Ă��s����񐔂����Ȃ��Ȃ�܂��B�W���X�g�C���^�C���̊��蓖�Ă��ŏ����ɗ}���邽�߂ɁA�K�v�ȃ{�C�X�̗\�z�ő吔���^�C�g���Ŏ��O�ɍ쐬���Ă���A�K�v�ɉ����č폜���邱�Ƃ��ł��܂��B���̌�A�{�C�X�� XAudio2 �v�[������ė��p����܂��B�������[ �v�[���� XAudio2 �G���W���̃C���X�^���X�Ɋ֘A�t�����܂��BXAudio2 �G���W���̃C���X�^���X�ɂ���Ďg�p����邷�ׂẴ������[�́AXAudio2 �I�u�W�F�N�g��j�����A�K�v�ɉ����čč쐬���邱�ƂŁA�ė��p�ł��܂� (�K�v�ɉ����āA���O���蓖�Ăɂ�郁�����[ �v�[���̊g����ēK�p����K�v������܂�)�B

	�R�[���o�b�N������� CreateSourceVoice �̌Ăяo���͖����ł��B�R�[���o�b�N������ CreateSourceVoice ���Ăяo���ƁAXAUDIO2_E_INVALID_CALL ���Ԃ���܂��B
	*/

	/*
	[[IXAudio2VoiceCallback]]
	http://msdn.microsoft.com/ja-jp/library/bb694521(v=vs.85).aspx
	���̃g�s�b�N�͂܂��]������Ă��܂��� - ���̃g�s�b�N��]������ 
	The IXAudio2VoiceCallback �C���^�[�t�F�C�X�ɂ́A����� IXAudio2SourceVoice �ŁA����C�x���g�����������Ƃ��ɃN���C�A���g�ɒʒm���郁�\�b�h���i�[����܂��B���̃C���^�[�t�F�C�X�́AXAudio2 �̃N���C�A���g�ɂ���Ď��������K�v������܂��BXAudio2 �́AIXAudio2::CreateSourceVoice ���\�b�h���ɃN���C�A���g�ɂ���Ē񋟂����C���^�[�t�F�C�X �|�C���^�[����Ă����̃��\�b�h���Ăяo���܂��B���̃C���^�[�t�F�C�X�̃��\�b�h�̃N���C�A���g�����́AHRESULT �ł͂Ȃ��Avoid ��Ԃ��܂��B

	�R�[���o�b�N�̎����Ɋւ��鐧�������ɂ��ẮA�uXAudio2 �̃R�[���o�b�N�v�̃g�s�b�N���Q�Ƃ��Ă��������B

	���\�b�h ���� 
	OnBufferEnd �{�C�X���o�b�t�@�[�̏������I�������Ƃ��ɌĂяo����܂��B
	 
	OnBufferStart �{�C�X���V�����I�[�f�B�I �o�b�t�@�[�̏������J�n���悤�Ƃ���Ƃ��ɌĂяo����܂��B
	 
	OnLoopEnd �{�C�X�����[�v�̍Ō�ɓ��B�����Ƃ��ɌĂяo����܂��B
	 
	OnStreamEnd �{�C�X���A���I�[�f�B�I �X�g���[���̍Đ����I�������Ƃ��ɌĂяo����܂��B
	 
	OnVoiceError �{�C�X�̏������ɏd��ȃG���[�����������Ƃ��ɌĂяo����܂��B
	 
	OnVoiceProcessingPassEnd �{�C�X�I���̃v���Z�b�V���O �p�X�̒���ɌĂяo����܂��B
	 
	OnVoiceProcessingPassStart �{�C�X�J�n�̃v���Z�b�V���O �p�X�̒��O�ɌĂяo����܂��B
	 

	�v��
	�w�b�_�[: XAudio2.h �錾

	���C�u����: XAudio2.lib ���e 


����ŁA����炷�������o�����̂ŁASubmit�Ń\�[�X���H�C�X�L���[�ɑ��M���܂��B
*/
//*******************************************
// �\�[�X���H�C�X�̍쐬
// ������	�F	�\�[�X���H�C�X�I�u�W�F�N�g�ւ̃|�C���^�ւ̎Q��
// ������	�F	WAVEFORMATEX �\���̂ւ̎Q��
// �߂�l	�F	TRUE:����		FALSE:���s
//*******************************************
bool CXAUDIO2::CreateSVoice()
{
	// �\�[�X���H�C�X�̔j��
	if(m_pSVoice)
	{
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}

	//																				     ���{��
	if( FAILED(m_pXAudio2->CreateSourceVoice(&m_pSVoice, &m_WaveSound.GetWaveFmtEx(),0, 1.0f, /*pCallback*/0 )) )
	{
		XAUDIO2_OUTDBG("�\�[�X���H�C�X�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("�\�[�X���H�C�X�̍쐬�ɐ������܂����B\n");
	return TRUE;
}



/*
	�\�[�X���H�C�X�L���[�ɑ��M����ɂ�
	IXAudio2SourceVoice::SubmitSourceBuffer ���\�b�h���g���܂��B
	�������� XAUDIO2_BUFFER �\���̂̃A�h���X ��n���܂��B
	�������ł����A����͎g���܂���̂Ŏw�肵�܂���B(�f�t�H���g�� NULL �ł�)


	���āA
	wSubmit.AudioBytes = WAVE �f�[�^�̃T�C�Y; 
	wSubmit.pAudioData = WAVE �f�[�^;

	���̓�s�ł����A
	WAVE �f�[�^�̃T�C�Y �� �udata�`�����N�v��ǂݍ��񂾎���
	WAVE �̃T�C�Y���ꏏ�ɓǂݍ���ł���Ǝv���܂��B
	MMCKINFO::cksize �� WAVE �f�[�^�̃T�C�Y��������Ă܂��B


	WAVE �f�[�^ �͎��ۂ� WAVE �̏���ǂݍ��񂾃I�u�W�F�N�g�ł��B
	�Ⴆ�� mmioRead �œǂݍ��� WAVE �f�[�^�������Ɏw�肵�܂��B



	�����܂ŃG���[���Ȃ���Ό�͍Đ��E��~���邾���ł��B

	m_pSVoice->Start(0);
	m_pSVoice->Stop(0);

	�\�[�X���H�C�X�I�u�W�F�N�g�̃����o���\�b�h�ł���
	�Đ��E��~���\�b�h���Ăׂ�OK�ł��B


	�g���I�������㏈���ł��B
 */
//*******************************************
// �\�[�X���H�C�X�L���[�ɉ��y�f�[�^�𑗐M
// ����		�F	�Ȃ�
// �߂�l	�F	TRUE:����	FALSE:���s
//*******************************************
bool CXAUDIO2::Submit()
{
	XAUDIO2_BUFFER wSubmit = { 0 };
	wSubmit.AudioBytes     = m_WaveSound.GetWaveSize(); 
	wSubmit.pAudioData     = m_WaveSound.GetWaveData();
	wSubmit.Flags          = XAUDIO2_END_OF_STREAM;

	if( m_pSVoice->SubmitSourceBuffer(&wSubmit) != S_OK )
	{
		XAUDIO2_OUTDBG("���y�f�[�^�̑��M�Ɏ��s���܂����B\n");	
		return FALSE;
	}

	XAUDIO2_OUTDBG("���y�f�[�^�̑��M�ɐ������܂����B\n");	
	return TRUE;	
}



//*******************************************
// �I��
// ����		�F	�Ȃ�
// �߂�l	�F	�Ȃ�
//*******************************************
void CXAUDIO2::Exit()
{
	// �\�[�X���H�C�X�̔j��
	if(m_pSVoice)
	{
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}

	// �}�X�^�����O���H�C�X�̔j��
	if(m_pMVoice)
	{
		m_pMVoice->DestroyVoice();
		m_pMVoice = NULL;
	}

	if(m_pXAudio2) { 
		m_pXAudio2->Release();
		m_pXAudio2=NULL;
	}		// XAudio2 �C���^�[�t�F�[�X�̔j��

	CoUninitialize();
}



// �Đ�
void CXAUDIO2::Play()
{
	if(m_pSVoice)
		m_pSVoice->Start(0);
}



// ��~
void CXAUDIO2::Stop()
{
	if(m_pSVoice)
		m_pSVoice->Stop(0);
}


// �ꎞ��~
void CXAUDIO2::Pause()
{
	// �������ł�
}


/*
	XAudio2 �ɂāA���ʕύX�ɐ������܂����B
	�ƌ����Ă�����ȓ�����͂���܂���B

	IXAudio2SourceVoice(IXAudio2Voice) �̃����o���\�b�h SetVolume ���g�������ł��B
	�\�[�X�{�C�X�ɑ΂��Ẳ��ʂ�ύX�ł��܂��B


	HRESULT SetVolume(  float Volume,  UINT32 OperationSet = XAUDIO2_COMMIT_NOW);
	[ MSDN ] : http://bit.ly/wTT5I4
	���g�͂���Ȋ����ŁA�������Ƀ{�����[����ݒ肵�Ă�����΂��������ł��B
	�߂�������ȒP�ł��B

	�߂�l�͂���ȕ��ɏ����Ă܂��B

	Returns S_OK if successful, an error code otherwise.
	See XAudio2 Error Codes for descriptions of error codes

	�֐������������ S_OK �A���s����΃G���[�R�[�h���Ԃ��Ă��܂��B
	�G���[�R�[�h�� XAudio2 �̃G���[�R�[�h �����Ă˂炵���ł��B



	�������̂͊ȒP�Ȃ�ł����ǈӖ��s���ȒP�ꂪ�������Ă����ς�Ȃ̂������B
	�Q�C�����ĂȂ񂶂��ق��H���Ċ����ł��B
	�Ƃ肠�����d�v�����ȏ��H���Ǝv���܂��B

	XAUDIO2_MAX_VOLUME_LEVEL (-2^24 to 2^24), with a maximum gain of 144.5 dB

	XAUDIO2_MAX_VOLUME_LEVEL(����������͈�)�́A-2^24 �` 2^24 �̊Ԃ������ł��B
	�ő�Q�C���� 144.5db �炵���ł����ǂ�����Ȃ��G�G

	�܂��A����Ȏ���������Ă܂��B

	A volume level of 1.0 means there is no attenuation or gain and 0 means silence.

	�{�����[�����x�� 1.0 �́A����Ȃ����A�Q�C�����Ȃ��ƌ����Ӗ��ł��B
	�܂��A0 �͖����ł��B

	�ƌ������́A1.0f �� 100%�A0.0f �� 0% �ƌ����Ӗ��Ȃ̂ł͂Ȃ��ł��傤���B
	�������A�Q�C�����Ȃ��Ƃ������͂ǂ��������Ȃ̂��c�B


	�Ⴆ�΂���ȕ��Ɏg���Ă݂�B
*/
// �{�����[���̐ݒ�
void CXAUDIO2::SetVolume(float Volume)
{
	m_pMVoice->SetVolume( Volume );
}



// �{�����[�����擾
float CXAUDIO2::GetVolume()const
{
	float p_Volume=0;
	m_pMVoice->GetVolume(&p_Volume);
	return p_Volume;
}

// �X�e�[�g�̎擾
bool CXAUDIO2::GetState() {
	XAUDIO2_VOICE_STATE state = {};
	m_pSVoice->GetState(&state);
	
	return state.BuffersQueued > 0;
}

// �f�o�C�X�̏ڍׂ̎擾
void CXAUDIO2::GetDeviceDetails( XAUDIO2_DEVICE_DETAILS* deviceDatails) {
	m_pXAudio2->GetDeviceDetails( 0, deviceDatails );
}

// �\�[�X�{�C�X�̎擾
IXAudio2SourceVoice* CXAUDIO2::GetSVoice() {
	return m_pSVoice;
}

// �}�X�^�[�{�C�X�̎擾
IXAudio2MasteringVoice* CXAUDIO2::GetMVoice() { // �}�X�^�[�{�C�X�ւ̃|�C���^��Ԃ�
	return m_pMVoice;
}

// �\�[�X�{�C�X�̃t�H�[�}�b�g�̎擾
const WAVEFORMATEX& CXAUDIO2::GetSVoiceFormat() const {
	return m_WaveSound.GetWaveFmtEx();
}


//*******************************************
// �C���X�^���X�̐���
// ����		�F	�Ȃ�
// �߂�l	�F	CXAUDIO2 �N���X�ւ̃|�C���^
//*******************************************
CXAUDIO2* CXAUDIO2::GetInst()
{
	static CXAUDIO2 Inst;
	return &Inst;
}