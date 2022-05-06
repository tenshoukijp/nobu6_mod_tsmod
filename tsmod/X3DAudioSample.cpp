// 6wav.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "WinTarget.h"
#include <d3dx9math.h>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

#include "XAudioDbg.h"
#include "CXAudio.h"

#include "OutputDebugStream.h"



#pragma comment(lib, "X3daudio.lib")


// ----------------------
// IXAudio2VoiceCallback�̎���
// http://msdn.microsoft.com/ja-jp/library/microsoft.directx_sdk.ixaudio2voicecallback.ixaudio2voicecallback(v=VS.85).aspx
// "�ʏ��"�����p���ăX�g���[�~���O�Đ������܂�
// pBufferContext�Ƃ�XAUDIO2_BUFFER�ɓn���郆�[�U�f�[�^���w��
// pBufferContext�������̃��\�b�h�̓L���[�f�[�^�����݂��ĂȂ��Ɠ����Ȃ�(�łȂ��ƍ���)
// ----------------------
struct voice_callback : public ::IXAudio2VoiceCallback
{
	// �{�C�X���o�b�t�@�L���[�f�[�^��ǂݎ�钼�O�Ƀv���Z�X�ŌĂяo��
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart( ::UINT32 BytesRequired )
	{
		std::printf("Processing pass start.\n");
	}

	// �{�C�X���v���Z�X�I�����ɌĂяo��
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd()
	{
		std::printf("Processing pass end.\n");
	}

	// �I�[�f�B�I�X�g���[�����I������Ƃ��ɌĂяo��
	void STDMETHODCALLTYPE OnStreamEnd()
	{
		std::printf("Stream end.\n");
	}

	// �o�b�t�@�L���[�f�[�^�̊J�n���Ăяo��
	void STDMETHODCALLTYPE OnBufferStart( void* pBufferContext )
	{
		std::printf("Buffer start.\n");
	}

	// �o�b�t�@�L���[�f�[�^�̏I������ɌĂяo��
	void STDMETHODCALLTYPE OnBufferEnd( void* pBufferContext )
	{
		std::printf("Buffer end.\n");
	}

	// ���[�v�Đ��I�����ɌĂяo��
	void STDMETHODCALLTYPE OnLoopEnd( void* pBufferContext )
	{
		std::printf("Loop end.\n");
	}

	// �{�C�X�ɃG���[������ƌĂяo��
	void STDMETHODCALLTYPE OnVoiceError( void* pBufferContext, HRESULT Error )
	{
		std::printf("Voice Error!!\n");
	}
};





// Constants to define our world space
const INT XMIN = -10;
const INT XMAX = 10;
const INT ZMIN = -10;
const INT ZMAX = 10;
#define OUTPUTCHANNELS 8 // maximum number of destination channels supported in this sample

// Specify sound cone to add directionality to listener for artistic effect:
// Emitters behind the listener are defined here to be more attenuated,
// have a lower LPF cutoff frequency,
// yet have a slightly higher reverb send level.
// �G�~�b�^�̌����ɑ΂��āA�P��`�����l���̔� LFE �G�~�b�^�̕����� DSP ����̃X�P�[�����O�ɂ���Ďw�肵�܂��B
/*
�����o
	InnerAngle
	�����R�[���̊p�x (���W�A���P��) �ł��B���̒l�� 0.0f �` X3DAUDIO_2PI �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterAngle
	�O���R�[���̊p�x (���W�A���P��) �ł��B���̒l�� InnerAngle �` X3DAUDIO_2PI �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerVolume
	�����R�[����/���̃{�����[�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterVolume
	�O���R�[����/�ȍ~�̃{�����[�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerLPF
	�����R�[����/���̃��[�p�X�t�B���^�_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[���ł��B���̒l�̓��[�p�X�t�B���^�v�Z�݂̂Ɏg�p����A0.0f �` 1.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterLPF
	�O���R�[����/�ȍ~�̃��[�p�X�t�B���^�_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[���ł��B���̒l�̓��[�p�X�t�B���^�v�Z�݂̂Ɏg�p����A0.0f �` 1.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerReverb
	�����R�[����/���̃��o�[�u �Z���h ���x�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterReverb
	�O���R�[����/�ȍ~�̃��o�[�u �Z���h ���x�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
*/
static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI*5.0f/6.0f, X3DAUDIO_PI*11.0f/6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

// Specify LFE level distance curve such that it rolls off much sooner than
// all non-LFE channels, making use of the subwoofer more dramatic.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve          = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

// Specify reverb send level distance curve such that reverb send increases
// slightly with distance before rolling off to silence.
// With the direct channels being increasingly attenuated with distance,
// this has the effect of increasing the reverb-to-direct sound ratio,
// reinforcing the perception of distance.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve          = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

int XAudio3DSample()
{
	HMODULE hModule = LoadLibrary("n6pwav1.dll");


	// ���������Ƃ�IXAudio2VoiceCallback�͉��z�f�X�g���N�^�������Ȃ�
	// ����ă{�C�X���폜�����܂Ŏ����Ă���K�v������
	// �Ⴆ�΁A�f���[�^�Ɏ�������
	struct voice_deleter_has_callback
	{
		voice_deleter_has_callback()
			: Callback_( new voice_callback() )
		{}

		void operator()( ::IXAudio2SourceVoice* voice ) const
		{
			voice->DestroyVoice();
		}

		// shared_ptr�Ɏ�������̂͌��\��낵���Ȃ�(�R�X�g������Ȃ�)
		// �������f���[�^��n���Ƃ��ɃR�s�[�ɂȂ�̂ł�����������
		boost::shared_ptr<voice_callback> Callback_;
	} deleter;




	// �V���O���g��
	CXAUDIO2 *pXAudio2 = CXAUDIO2::GetInst();
	// ������
	pXAudio2->Init();

	// pXAudio2->LoadWaveFile("601.wav");
	pXAudio2->LoadWaveFile(601, hModule);

	IXAudio2SourceVoice* pSVoice = pXAudio2->GetSVoice();
	IXAudio2MasteringVoice* pMVoice = pXAudio2->GetMVoice();

	WAVEFORMATEX pSVoiceFormat = pXAudio2->GetSVoiceFormat();
	int nInputChannnel = pSVoiceFormat.nChannels;
	XAUDIO2_OUTDBG("%d", nInputChannnel);

	// �}�X�^�[ �{�C�X �`�����l�����Ɉ�v����X�s�[�J�[ �`�����l���\�������� X3DAudio ���C�u���������������܂��B
	// 3.��ԃI�[�f�B�I�̌v�Z�����s���A�t���[���X�V���ɍX�V���܂��B 
	// X3DAudioInitialize �ɓ˂����ނ��߂̃f�[�^�ݒ�
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	pXAudio2->GetDeviceDetails( &deviceDetails );
	DWORD channelMask = deviceDetails.OutputFormat.dwChannelMask;
	int m_numChannels = deviceDetails.OutputFormat.Format.nChannels;
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAUDIO_HANDLE x3DInstance;
	::memset( x3DInstance, 0, sizeof( x3DInstance ) );

	X3DAudioInitialize( channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DInstance );


	/*
	XAudio2�ł́ASetOutputMatrix�֐��Ŋe�`�����l���̃{�����[���̒������s��
	SetFrequencyRatio�֐��Ńs�b�`�̒������s�����Ƃ��ł��܂��B
	�ŁA����2�̊֐��ɓn�������p�̃p�����[�^���K�v�Ȃ̂ł���
	�����X3DAudio�Ƃ���API���g���Ώ���Ɍv�Z���Ă���܂��B

	����X3DAudio�͔��ɃV���v����API�ŁA�֐���2��������܂���B
	X3DAudioInitialize��X3DAudioCalculate�ł��B
	���O���番����悤�ɁAX3DAudioInitialize�ŏ������A
	X3DAudioCalculate�Ōv�Z����킯�ł���
	�v�Z���邽�߂ɂ̓G�~�b�^�ƃ��X�i�[�̏�񂪕K�v�ł��B
	���ꂼ��\���̂���`����Ă���A�ʒu�A�����A���x�Ȃǂ̐ݒ�����܂��B
	�v�Z���ʂ�X3DAUDIO_DSP_SETTINGS�Ƃ����\���̂œ��邱�Ƃ��ł��܂��B
	���̒��Ƀh�b�v���[�W����{�����[�����x���̔z��Ȃǂ�����̂�
	�����XAudio2�̊֐��ɓn���Ď��ۂɃ{�����[���ƃs�b�`�𒲐�����킯�ł��B

	X3DAudioCalculate�֐��́A���o�[�u�̌��ʂȂǂ��v�Z�\�Ȃ̂�
	����������������悤�Ǝv���܂��B
	*/

	// �錾�J�n
    X3DAUDIO_DSP_SETTINGS dspSettings;
    X3DAUDIO_LISTENER listener;
    X3DAUDIO_EMITTER emitter;
    X3DAUDIO_CONE emitterCone;

    D3DXVECTOR3 vListenerPos;
    D3DXVECTOR3 vEmitterPos;
    float fListenerAngle;
    bool  fUseListenerCone;
    bool  fUseInnerRadius;
    bool  fUseRedirectToLFE;


    FLOAT32 *emitterAzimuths = new FLOAT32[nInputChannnel];
    FLOAT32 *matrixCoefficients= new FLOAT32[nInputChannnel * OUTPUTCHANNELS];
	//FLOAT32 emitterAzimuths[INPUTCHANNELS];
    //FLOAT32 matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	// �錾�����܂�

    vListenerPos = D3DXVECTOR3( 0, 0, 0 );	 // ���[�U�[�̈ʒu
    vEmitterPos = D3DXVECTOR3( 7, 0, -7 ); // x, y, z (z=float(ZMAX) �G�~�b�^�[�̈ʒu
    fListenerAngle = 0;
    fUseListenerCone = TRUE;
    fUseInnerRadius = TRUE;
    fUseRedirectToLFE = ((deviceDetails.OutputFormat.dwChannelMask & SPEAKER_LOW_FREQUENCY) != 0);

    //
    // Setup 3D audio structs
    //
    listener.Position = vListenerPos;
    listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;
    // listener.pCone = (X3DAUDIO_CONE*)NULL;

    emitter.pCone = &emitterCone;
    emitter.pCone->InnerAngle = 0.0f;
    // Setting the inner cone angles to X3DAUDIO_2PI and
    // outer cone other than 0 causes
    // the emitter to act like a point emitter using the
    // INNER cone settings only.
    emitter.pCone->OuterAngle = 0.0f;
    // Setting the outer cone angles to zero causes
    // the emitter to act like a point emitter using the
    // OUTER cone settings only.
    emitter.pCone->InnerVolume = 0.0f;
    emitter.pCone->OuterVolume = 1.0f;
    emitter.pCone->InnerLPF = 0.0f;
    emitter.pCone->OuterLPF = 1.0f;
    emitter.pCone->InnerReverb = 0.0f;
    emitter.pCone->OuterReverb = 1.0f;

    emitter.Position = vEmitterPos;

	emitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    emitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    emitter.ChannelCount = nInputChannnel;
    emitter.ChannelRadius = 1.0f;
    emitter.pChannelAzimuths = emitterAzimuths;

    // Use of Inner radius allows for smoother transitions as
    // a sound travels directly through, above, or below the listener.
    // It also may be used to give elevation cues.
    emitter.InnerRadius = 2.0f;
    emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;;

    emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
    emitter.pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
    emitter.pLPFDirectCurve = NULL; // use default curve
    emitter.pLPFReverbCurve = NULL; // use default curve
    emitter.pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
    emitter.CurveDistanceScaler = 14.0f;
    emitter.DopplerScaler = 1.0f;

    dspSettings.SrcChannelCount = nInputChannnel;
    dspSettings.DstChannelCount = m_numChannels;
    dspSettings.pMatrixCoefficients = matrixCoefficients;

	UINT32 dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT;


    X3DAudioCalculate( x3DInstance, &listener, &emitter, dwCalcFlags, &dspSettings );

	/*
	5.��ԃI�[�f�B�I���������߂ɁA�ȗ������ꂽ X/Z �O���b�h��ɂ��� X3DAudio �̃G�~�b�^�[ �I�u�W�F�N�g�ƃ��X�i�[ �I�u�W�F�N�g��\�����āA���䂵�܂��B 
	1.�e�����_�����O �t���[���ŁA�G�~�b�^�[�ƃ��X�i�[�̌��݂̈ʒu��`�悵�܂��B
	2.�p�����[�^�[����̕ύX��K�p���āA�e�t���[���� UI �v�f���X�V���܂��B
	3.��ԃI�[�f�B�I�̌v�Z�����s���A�t���[���X�V���ɍX�V���܂��B 
	���t���[���̌o�ߎ��ԂɊ�Â��A�V�����ʒu�Ƒ��x�̏����g�p���āA�G�~�b�^�[ �I�u�W�F�N�g�ƃ��X�i�[ �I�u�W�F�N�g���X�V���܂��B
	��X3DAudioCalculate ���\�b�h���Ăяo���āA���ʂ� DSP �R���g���[���̐ݒ���v�Z���܂��B
	��DSP �̐ݒ���AIXAudio2Voice::SetOutputMatrix �֐��� IXAudio2SourceVoice::SetFrequencyRatio �֐����o�R���ă\�[�X �{�C�X�ɓn���܂��B
	*/

	pSVoice->SetOutputMatrix(pMVoice, emitter.ChannelCount, m_numChannels, dspSettings.pMatrixCoefficients );
	pSVoice->SetFrequencyRatio( dspSettings.DopplerFactor );

	pXAudio2->Play();

	delete [] emitterAzimuths;
	delete [] matrixCoefficients;

	return 0;
}
/*
[[X3DAUDIO_CONE]]
�G�~�b�^�̌����ɑ΂��āA�P��`�����l���̔� LFE �G�~�b�^�̕����� DSP ����̃X�P�[�����O�ɂ���Ďw�肵�܂��B

�R�s�[
 typedef struct X3DAUDIO_CONE {
    FLOAT32 InnerAngle;
    FLOAT32 OuterAngle;
    FLOAT32 InnerVolume;
    FLOAT32 OuterVolume;
    FLOAT32 InnerLPF;
    FLOAT32 OuterLPF;
    FLOAT32 InnerReverb;
    FLOAT32 OuterReverb;
} X3DAUDIO_CONE, *LPX3DAUDIO_CONE;

	�����o
	InnerAngle
	�����R�[���̊p�x (���W�A���P��) �ł��B���̒l�� 0.0f �` X3DAUDIO_2PI �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterAngle
	�O���R�[���̊p�x (���W�A���P��) �ł��B���̒l�� InnerAngle �` X3DAUDIO_2PI �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerVolume
	�����R�[����/���̃{�����[�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterVolume
	�O���R�[����/�ȍ~�̃{�����[�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerLPF
	�����R�[����/���̃��[�p�X�t�B���^�_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[���ł��B���̒l�̓��[�p�X�t�B���^�v�Z�݂̂Ɏg�p����A0.0f �` 1.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterLPF
	�O���R�[����/�ȍ~�̃��[�p�X�t�B���^�_�C���N�g �p�X�܂��̓��o�[�u �p�X�̌W���X�P�[���ł��B���̒l�̓��[�p�X�t�B���^�v�Z�݂̂Ɏg�p����A0.0f �` 1.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	InnerReverb
	�����R�[����/���̃��o�[�u �Z���h ���x�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	OuterReverb
	�O���R�[����/�ȍ~�̃��o�[�u �Z���h ���x�� �X�P�[���ł��B���̒l�� 0.0f �` 2.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	����@
	�T�E���h �R�[���̏ڍׂɂ��ẮA�u�T�E���h �R�[���v���Q�Ƃ��Ă��������B

	�v��
	�w�b�_�[: X3daudio.h �錾
*/


/*
[[X3DAUDIO_DISTANCE_CURVE]]
���`�Z�O�����g�ō\�����ꂽ�����I�ȋ敪�Ȑ����`���A���K�����ꂽ�����ɑ΂��� DSP ����𒼐ڒ�`���܂��B

�R�s�[
 typedef struct X3DAUDIO_DISTANCE_CURVE {
    X3DAUDIO_DISTANCE_CURVE_POINT *pPoints;
    UINT32 PointCount;
} X3DAUDIO_DISTANCE_CURVE;

�����o
	pPoints
	X3DAUDIO_DISTANCE_CURVE_POINT �z��ł��B���̔z��ɂ͕����������Ă͂Ȃ炸�A��������ɂ��ď����Ń\�[�g����Ă��Ȃ���΂Ȃ�܂���B
	PointCount
	�����J�[�u��̃|�C���g�̐��ł��B���ׂẴJ�[�u�ɂ́A0.0f �� 1.0f �̐��K�����ꂽ�����̒l�����ꂼ���`���鏭�Ȃ��Ƃ� 2 �̃G���h�|�C���g������̂ŁA2 �ȏ�̃|�C���g������͂��ł��B
	�v��
	�w�b�_�[: X3daudio.h �錾
*/


/*
[[X3DAUDIO_DISTANCE_CURVE_POINT]]
���K�����ꂽ���̋����� DSP �ݒ���`���܂��B

�R�s�[
 typedef struct X3DAUDIO_DISTANCE_CURVE_POINT {
    FLOAT32 Distance;
    FLOAT32 DSPSetting;
} X3DAUDIO_DISTANCE_CURVE_POINT, *LPX3DAUDIO_DISTANCE_CURVE_POINT;

�����o
	Distance
	���K�����ꂽ�����ł��B���̒l�� 0.0f �` 1.0f �͈͓̔��łȂ���΂Ȃ�܂���B
	DSPSetting
	DSP �R���g���[���̐ݒ�ł��B
	�v��
	�w�b�_�[: X3daudio.h �錾
*/


/*
[[X3DAUDIO_DSP_SETTINGS]]
3D �V�O�i�������̂��߂ɒ჌�x���̃I�[�f�B�I �����_�����O�֐��ɑ����� X3DAudioCalculate �̌Ăяo�����ʂ��󂯎��܂��B
typedef struct X3DAUDIO_DSP_SETTINGS {
    FLOAT32 *pMatrixCoefficients;
    FLOAT32 *pDelayTimes;
    UINT32 SrcChannelCount;
    UINT32 DstChannelCount;
    FLOAT32 LPFDirectCoefficient;
    FLOAT32 LPFReverbCoefficient;
    FLOAT32 ReverbLevel;
    FLOAT32 DopplerFactor;
    FLOAT32 EmitterToListenerAngle;
    FLOAT32 EmitterToListenerDistance;
    FLOAT32 EmitterVelocityComponent;
    FLOAT32 ListenerVelocityComponent;
} X3DAUDIO_DSP_SETTINGS, *LPX3DAUDIO_DSP_SETTINGS;

�����o
	pMatrixCoefficients
	�e�f�X�e�B�l�[�V���� �`�����l���ɂ���e�\�[�X �`�����l���̃{�����[�� ���x���ŏ����������A�Ăяo�����w��̔z��ł��B���̔z��ɂ́A���Ȃ��Ƃ� (SrcChannelCount �~ DstChannelCount) �̗v�f���K�v�ł��B
	�z��ɂ́A�\�[�X �`�����l�����z��̗�C���f�b�N�X�Ƃ��āA�f�X�e�B�l�[�V���� �`�����l�����z��̍s�C���f�b�N�X�Ƃ��Ĕz�u����܂��B���Ƃ��΁A2 �`�����l�� �X�e���I���͂��A���d���O���`�����l���ɂ����āA���S����ђ���g���`�����l���ɂ͂Ȃ� 5.1 �o�͂Ƀ����_�����O����ƁA�s��͎��̕\�Ɏ����悤�ɂȂ�܂��B
	�o�� ������ �E���� 
	[��] 1.0 0.0 
	[�E] 0.0 1.0 
	�O������ 0.0 0.0 
	LFE 0.0 0.0 
	����� 0.8 0.0 
	����E 0.0 0.8 

	���ƉE�̃`�����l�����A�o�͂̍��ƉE�̃`�����l���Ɋ��S�Ƀ}�b�v����Ă��܂��B���ƉE�̓��͂� 80% ���A����̍��ƉE�̃`�����l���Ƀ}�b�v����Ă��܂��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_MATRIX �t���O���w�肵�Ă��Ȃ��ꍇ�ApMatrixCoefficients �����o�[�� NULL �ɂł��܂��B
	pDelayTimes
	�e�f�X�e�B�l�[�V���� �`�����l���̒x�����󂯎��A�Ăяo�����w��̒x�����Ԕz�� (�~���b�P��) �ł��B���̔z��ɂ́A���Ȃ��Ƃ� DstChannelCount �̗v�f���K�v�ł��BX3DAudio �́A���ۂɂ͒x�������s���܂���B�G�t�F�N�g �`�F�[���ɔz�u���ꂽ�x�� DSP �G�t�F�N�g�̒����Ɏg�p�ł���W����Ԃ������ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_DELAY �t���O���w�肵�Ă��Ȃ��ꍇ�ApDelayTimes �����o�[�� NULL �ɂł��܂��B
	��    ���̃����o�[�́AX3DAudio ���X�e���I�o�͗p�ɏ����������ꍇ�ɂ̂ݕԂ���܂��BXbox 360 �̈�ʓI�Ȏg�p�ł́A�f�[�^���܂������Ԃ��܂���B
	SrcChannelCount
	�\�[�X �`�����l���̐��ł��BX3DAudioCalculate ���Ăяo���O�ɃG�~�b�^ �`�����l���̐��ɏ���������K�v������܂��B
	DstChannelCount
	�f�X�e�B�l�[�V���� �`�����l���̐��ł��BX3DAudioCalculate ���Ăяo���O�ɍŏI�~�b�N�X �`�����l���̐��ɏ���������K�v������܂��B
	LPFDirectCoefficient
	LPF �_�C���N�g �p�X�W���ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_LPF_DIRECT �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	LPFReverbCoefficient
	LPF ���o�[�u �p�X�W���ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_LPF_REVERB �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	ReverbLevel
	���o�[�u �Z���h ���x���ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_REVERB �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	DopplerFactor
	�h�b�v���[�ΈڌW���B�h�b�v���[�Έڌ��ʂ��l�����čăT���v�����O����X�P�[�����O���܂��B
	�R�s�[
	 effective_frequency = DopplerFactor �~ original_frequency
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_DOPPLER �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	EmitterToListenerAngle
	�G�~�b�^-���X�i�[�̓��p�B�G�~�b�^�̑O�����ɑ΂��郉�W�A���P�ʂŕ\����܂��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_EMITTER_ANGLE �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	EmitterToListenerDistance
	���X�i�[����G�~�b�^ �x�[�X�ʒu�܂ł̃��[�U�[��`�̃��[���h�P�ʂ̋����ł��B
	EmitterVelocityComponent
	���[�U�[��`�̃��[���h�P��/�b�ŁA�G�~�b�^-���X�i�[ �x�N�g���Ɏˉe���ꂽ�G�~�b�^ �x���V�e�B �x�N�g���̗v�f�ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_DOPPLER �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	ListenerVelocityComponent
	���[�U�[��`�̃��[���h�P��/�b�ŁA�G�~�b�^-���X�i�[ �x�N�g���Ɏˉe���ꂽ���X�i�[ �x���V�e�B �x�N�g���̗v�f�ł��B
	X3DAudioCalculate �̌Ăяo������ X3DAUDIO_CALCULATE_DOPPLER �t���O���w�肳���ꍇ�̂݁A�v�Z����܂��B
	����@
	���̍\���̂� X3DAudioCalculate �֐��ɓn���O�ɁA���̃����o�[������������K�v������܂��B

	�EpMatrixCoefficients
	�EpDelayTimes
	�ESrcChannelCount
	�EDstChannelCount
	���̍\���̂� X3DAudioCalculate �֐��ɓn���ƁA���̃����o�[���Ԃ���܂��B

	�EpMatrixCoefficients
	�EpDelayTimes
	�ELPFDirectCoefficient
	�ELPFReverbCoefficient
	�EReverbLevel
	�EDopplerFactor
	�EEmitterToListenerAngle
	�EEmitterToListenerDistance
	�EEmitterVelocityComponent
	�EListenerVelocityComponent
	��    pMatrixCoefficients �� pDelayTimes �̏ꍇ�AX3DAudioCalculate �͒ǉ��̃������[�����蓖�Ă܂���BX3DAudioCalculate �́A�����̃|�C���^�[�Ɋ��蓖�Ă�ꂽ�������[�ʒu�ŁA�l��ύX���邾���ł��B

	�v��
	�w�b�_�[: X3daudio.h �錾
*/

/*
[[X3DAUDIO_LISTENER]]

typedef struct X3DAUDIO_LISTENER {
    X3DAUDIO_VECTOR OrientFront;
    X3DAUDIO_VECTOR OrientTop;
    X3DAUDIO_VECTOR Position;
    X3DAUDIO_VECTOR Velocity;
    X3DAUDIO_CONE *pCone;
} X3DAUDIO_LISTENER, *LPX3DAUDIO_LISTENER;

	�����o
	OrientFront
	�O�����̌����ł��BpCone �� NULL �̏ꍇ�AOrientFront �͍s��ƒx���̌v�Z�݂̂Ɏg�p����܂��BpCone �� NULL �ł͂Ȃ��ꍇ�AOrientFront �͍s��ALPF (�_�C���N�g �p�X����у��o�[�u �p�X�̗���)�A����у��o�[�u�̌v�Z�Ɏg�p����܂��B���̒l�� OrientTop �Ɛ��K�����łȂ���΂Ȃ�܂���B
	OrientTop
	������̌����ł��B�s��ƒx���̌v�Z�ɂ̂ݎg�p����܂��B���̒l�� OrientFront �Ɛ��K�����łȂ���΂Ȃ�܂���B
	Position
	���[�U�[��`�̃��[���h�P�ʂ̈ʒu�ł��B���̒l�� Velocity �ɂ͉e�����܂���B
	Velocity
	���[�U�[��`�̃��[���h�P��/�b�̃x���V�e�B �x�N�g���ł��B�h�b�v���[���ʂ̌v�Z�ɂ̂ݎg�p����܂��B���̒l�� Position �ɂ͉e�����܂���B
	pCone
	���̃��X�i�[�� X3DAUDIO_CONE �\���̂ւ̃|�C���^�[�ł��B���X�i�[ �R�[�����w�肷��ƁA�X�̉����̃{�����[������уt�B���^�[ DSP �p�����[�^�[�����肷��ۂɒǉ��̌v�Z���s���܂��BpCone �� NULL �l�́A�S�������T�E���h���w�肵�A�R�[�������͓K�p����܂���BpCone �͍s��ALPF (�_�C���N�g �p�X����у��o�[�u �p�X�̗���)�A����у��o�[�u�̌v�Z�ɂ̂ݎg�p����܂��B
	����@
	�p�����[�^�[�^ X3DAUDIO_VECTOR �� D3DVECTOR �Ɍ^�ϊ�����A���������_�l x�Ay�Az ��񋟂��܂��B

	���X�i�[�̑O���x�N�g���Ə���x�N�g���́A���K�����łȂ���΂Ȃ�܂���B���K�����ƌ��Ȃ����ɂ́A�x�N�g���̃y�A�̑傫���� 1 +- 1x10-5�A���ς� 0 +- 1x10-5 �ɂ���K�v������܂��B

	�v��
	�w�b�_�[: X3daudio.h �錾

*/

/*[[X3DAUDIO_EMITTER]]

�C�ӂ̐��̃T�E���h �`�����l���Ŏg�p�����V���O���|�C���g�܂��̓}���`�|�C���g�� 3D �I�[�f�B�I �\�[�X���`���܂��B

�R�s�[
 typedef struct X3DAUDIO_EMITTER {
    X3DAUDIO_CONE *pCone;
    X3DAUDIO_VECTOR OrientFront;
    X3DAUDIO_VECTOR OrientTop;
    X3DAUDIO_VECTOR Position;
    X3DAUDIO_VECTOR Velocity;
    FLOAT32 InnerRadius;
    FLOAT32 InnerRadiusAngle;
    UINT32 ChannelCount;
    FLOAT32 ChannelRadius;
    FLOAT32 *pChannelAzimuths;
    X3DAUDIO_DISTANCE_CURVE *pVolumeCurve;
    X3DAUDIO_DISTANCE_CURVE *pLFECurve;
    X3DAUDIO_DISTANCE_CURVE *pLPFDirectCurve;
    X3DAUDIO_DISTANCE_CURVE *pLPFReverbCurve;
    X3DAUDIO_DISTANCE_CURVE *pReverbCurve;
    FLOAT32 CurveDistanceScaler;
    FLOAT32 DopplerScaler;
} X3DAUDIO_EMITTER, *LPX3DAUDIO_EMITTER;

	�����o
	pCone
	�T�E���h �R�[���ւ̃|�C���^�[�ł��B�P��`�����l���̃G�~�b�^�ŁA�s��ALPF (�_�C���N�g �p�X����у��o�[�u �p�X�̗���)�A����у��o�[�u�̌v�Z�݂̂Ɏg�p����܂��BNULL �͑S���������w�肵�܂��B
	OrientFront
	�O�����̌����ł��B���̒l�� OrientTop �Ɛ��K�����łȂ���΂Ȃ�܂���BOrientFront �͎g�p���ɐ��K������܂��B�R�[�����g�p���Ȃ��P��`�����l���̃G�~�b�^�ł́AOrientFront �̓G�~�b�^�̊p�x�v�Z�݂̂Ɏg�p����܂��B�R�[�����g�p����}���`�`�����l�� �G�~�b�^�܂��͒P��`�����l�� �G�~�b�^�ł́AOrientFront �͍s��ALPF (�_�C���N�g �p�X����у��o�[�u �p�X�̗���)�A����у��o�[�u�̌v�Z�Ɏg�p����܂��B
	OrientTop
	������̌����ł��B���̒l�� OrientFront �Ɛ��K�����łȂ���΂Ȃ�܂���BOrientTop �̓}���`�`�����l�� �G�~�b�^�ł̍s��̌v�Z�ɂ̂ݎg�p����܂��B
	Position
	���[�U�[��`�̃��[���h�P�ʂ̈ʒu�ł��B���̒l�� Velocity �ɂ͉e�����܂���B
	Velocity
	���[�U�[��`�̃��[���h�P��/�b�̃x���V�e�B �x�N�g���ł��B���̒l�̓h�b�v���[���ʂ̌v�Z�ɂ̂ݎg�p����APosition �ɂ͉e�����܂���B
	InnerRadius
	�������a�̌v�Z�Ɏg�p�����l�ł��BInnerRadius �� 0 �̏ꍇ�A�������a�͎g�p����܂��񂪁AInnerRadiusAngle �͎g�p�����ꍇ������܂��B0.0f �` MAX_FLT �̒l���w�肵�܂��B
	InnerRadiusAngle
	�����p�x�̌v�Z�Ɏg�p�����l�ł��B0.0f �` X3DAUDIO_PI/4.0 �̒l���w�肵�܂��B
	ChannelCount
	X3DAUDIO_EMITTER �\���̂ɂ���Ē�`�����G�~�b�^�̐��ł��B0 ���傫���l���w�肷��K�v������܂��B
	ChannelRadius
	ChannelCount �� 1 ���傫���ꍇ�ɃG�~�b�^���z�u����� Position ����̋����ł��BChannelRadius �́A�}���`�`�����l�� �G�~�b�^�ł̍s��̌v�Z�݂̂Ɏg�p����܂��B���̒l�� 0.0f �ȏ�ł��邱�Ƃ��K�v�ł��B
	pChannelAzimuths
	���W�A���P�ʂ̕��ʊp�ŕ\�����`�����l���ʒu�e�[�u���ł��B�`�����l�����a�Ƌ��Ɏg�p����A����x�N�g���ƒ������镽�ʂ̑O���x�N�g���Ƃ��Ď�����܂��B����g���� (LFE) �`�����l�����w�肷��ꍇ�́AX3DAUDIO_2PI �ɂ��܂��BpChannelAzimuths �͏��Ȃ��Ƃ� ChannelCount �̗v�f���K�v�ł��B�e�[�u���̒l�� 0.0f �` X3DAUDIO_2PI �͈͓̔��łȂ���΂Ȃ�܂���BpChannelAzimuths �́A�}���`�`�����l�� �G�~�b�^�ł̍s��̌v�Z�Ɏg�p����܂��B
	pVolumeCurve
	�{�����[�� ���x�������J�[�u�ł��B�s��̌v�Z�ɂ̂ݎg�p����܂��BNULL ���w�肷��ƁA���̂悤�ȋt��摥�Ɉ�v���������������̃J�[�u�ɂȂ�܂��B������ 0.0f �` CurveDistanceScaler�~ 1.0f �͈͓̔��ɂ���ꍇ�A�����͓K�p����܂���B
	������ CurveDistanceScaler�~ 1.0f ���傫���ꍇ�A�����W���� (CurveDistanceScaler�~ 1.0f)/�����ɂȂ�܂��BCurveDistanceScaler�~ 2.0f �̋����ŃT�E���h�͔����̃{�����[�� (-6 dB) �ɂȂ�ACurveDistanceScaler�~ 4.0f �̋����� 4 ���� 1 �̃{�����[�� (-12 dB) �ɂȂ�܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	pLFECurve
	LFE ���[���I�t�����J�[�u�ł��B����̃J�[�u ([0.0f,CurveDistanceScaler�~ 1.0f], [CurveDistanceScaler�~ 1.0f, 0.0f]) ���g�p����ꍇ�� NULL �ɂ��܂��BpLFECurve �� NULL �l���g�p����ƁA�������Ȃ��悤�ɃN�����v���ꂽ���� <= CurveDistanceScaler �ł̋t��摥�Ɉ�v����f�t�H���g �J�[�u���w�肳��܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	pLPFDirectCurve
	���[�p�X �t�B���^�[ (LPF) �_�C���N�g �p�X�W�������J�[�u�ł��B����̃J�[�u ([0.0f,0.0f], [1.0f,1.0f]) ���g�p����ꍇ�� NULL �ɂ��܂��BpLPFDirectCurve �́ALPF �_�C���N�g �p�X�̌v�Z�ɂ̂ݎg�p����܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	pLPFReverbCurve
	LPF ���o�[�u �p�X�W�������J�[�u�ł��B�f�t�H���g�̃J�[�u ([0.0f,0.25f], [1.0f,0.25f]) ���g�p����ꍇ�� NULL �ɂ��܂��BpLPFReverbCurve �̓��[�p�X�t�B���^���o�[�u �p�X�̌v�Z�ɂ̂ݎg�p����܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	pReverbCurve
	���o�[�u �Z���h ���x�������J�[�u�ł��B�f�t�H���g�̃J�[�u ([0.0f, 1.0f], [1.0f, 0.0f]) ���g�p����ꍇ�� NULL �ɂ��܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	CurveDistanceScaler
	���K�����ꂽ�����J�[�u�����[�U�[��`�̃��[���h�P�ʂɃX�P�[�����O���邽�߂ɁA�܂��͂��̌��ʂ��������邽�߂Ɏg�p����J�[�u�����X�P�[���ł��B����͑��̌v�Z�ɂ͉e�����܂���B���̒l�� FLT_MIN �` FLT_MAX �͈̔͂ɂ���K�v������܂��BCurveDistanceScaler �͍s��ALPF (�_�C���N�g �p�X����у��o�[�u �p�X�̗���)�A����у��o�[�u�̌v�Z�ɂ̂ݎg�p����܂��B
	��    RPC �J�[�u�� Distance �ϐ����g�p����悤�ɒ�`����Ă���ꍇ�́AXACT3DCalculate �͂��̃����o�[�𖳎����܂��B���̂悤�� RPC �J�[�u����`����Ă���ꍇ�A���̃����o�[�ł͂Ȃ��J�[�u���ɑΉ������񂪒񋟂���Ă�����̂ƌ��Ȃ���܂��B
	DopplerScaler
	�h�b�v���[�Έڌ��ʂ��������邽�߂Ɏg�p����h�b�v���[�ΈڃX�P�[���[�ł��BDopplerScaler �̓h�b�v���[���ʂ̌v�Z�ɂ̂ݎg�p����A���̑��̌v�Z�ɂ͉e�����܂���B���̒l�� 0.0f �` FLT_MAX �͈͓̔��ɂ���K�v������܂��B
	����@
	X3DAUDIO_EMITTER �́A�P��_�G�~�b�^�̃R�[���݂̂��T�|�[�g���܂��B���_�G�~�b�^�́A�֘A���鉹���̃O���[�v���Ǘ�����̂ɕ֗��Ō��ʓI�ȕ��@�ł��B���ׂẴ`�����l�� �|�C���g�ŁA�h�b�v���[���ʂȂǂ̑����̃v���p�e�B�����L����܂��B�����h�b�v���[�Έڂ��G�~�b�^�̂��ׂẴ`�����l���ɓK�p����܂��B���̂��߁A�h�b�v���[���ʂ̒l�̌v�Z���K�v�ɂȂ�̂� 1 �񂾂��ł��B�����̓Ɨ������P��_�G�~�b�^���K�v�ɂȂ����ꍇ�ł��A�_���Ƃ̌v�Z�͕K�v�ɂȂ�܂���B����́AX3DAUDIO_EMITTER �� 1 �����̃x�N�g�������Ȃ����߂ŁA���_�G�~�b�^�̃R�[���́A���ׂẴ`�����l�������������Ɍ����Ă���悤�ɋ�������邽�߁A�p�r�����肳��܂��B�����̓Ɨ������R�[�����K�v�ȏꍇ�́A�����̒P��_�G�~�b�^���A����̕������ƂɎg�p����K�v������܂��B 

	�p�����[�^�[�^ X3DAUDIO_VECTOR �� D3DVECTOR �Ɍ^�ϊ�����A���������_�l x�Ay�Az ��񋟂��܂��B

	X3DAudio �ł́Ax ���̒l��������E�Ɍ������āAy ���̒l���������֌������āAz ���̒l���߂����牓���Ɍ������đ傫���Ȃ鍶��f�J���g���W�n���g�p���܂��B���ʊp�́A�w�肵����������玞�v���ɑ��肳��܂��B

	���[�U�[��`�̋����J�[�u�ł́A�ŏ��̃|�C���g�̋����t�B�[���h�� 0.0f �ŁA�Ō�̃|�C���g�̋����t�B�[���h�� 1.0f �łȂ���΂Ȃ�܂���B

	�G�~�b�^�� CurveDistanceScaler�~ 1.0f �𒴂��鋗�����ړ�����ꍇ�́A�J�[�u�̍Ō�̃|�C���g���g�p���ă{�����[���o�̓��x�����v�Z���܂��B�Ō�̃|�C���g�͎��̂悤�ɋ��߂��܂��B

	�R�s�[
	 X3DAUDIO_DISTANCE_CURVE.pPoints[PointCount-1].DSPSetting)
	�������a�Ɠ������a�p�x
	InnerRadius �ɂ́A�T�E���h�����X�i�[�𒼐ڒʂ蔲����Ƃ��A�܂��̓��X�i�[�̏㉺��ʂ蔲����Ƃ��ɃT�E���h���X���[�Y�Ɉړ�����A�����̎��̗͂̈���w�肵�܂��BInnerRadiusAngle ���w�肷��ƁA�p���l������܂��B�p���グ���艺�����肷��΁A2 ��̃X�s�[�J�[����o�Ă���Ƃ͎v���Ȃ��悤�ȃT�E���h�������܂��B

	�������a�Ɠ������a�p�x���g�p���Ȃ��ꍇ�A2 ��̋ߐڂ����X�s�[�J�[���̃G�~�b�^�́A�����̌��݂̈ʒu�ƕ����ɕ��������ԂɂȂ�܂� (�܂��́A�G�~�b�^�� 1 ��̃X�s�[�J�[�̒�`���ꂽ�p�x�������C����ɂ���ꍇ�́A���̃X�s�[�J�[�݂̂��畷�����܂�)�B

	�������a�Ɠ������a�p�x�́A�w�肷��R�[���̊O���ɒu���ꂽ�G�~�b�^�ɑ΂��āA�e�����܂���B�R�[���̓����ŁA�G�~�b�^�����X�i�[�Ɠ����ʒu�ɂ��� (�܂��̓��X�i�[�̐^�ォ�^���ɂ���) �ꍇ�A�������a�Ɠ������a�p�x�ɂ��A�T�E���h�����ׂẴX�s�[�J�[�œ�������������悤�ɂȂ�܂ŏ��X�ɔ��Α��̃X�s�[�J�[����T�E���h���������܂��B

	 
	���摜�����
	http://msdn.microsoft.com/ja-jp/library/bb174736(v=vs.85).aspx

	�v��
	�w�b�_�[: X3daudio.h �錾

	�֘A����
	X3DAudio �̍\����

*/