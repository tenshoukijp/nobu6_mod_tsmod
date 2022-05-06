//--------------------------------------------------------------------------------------
// File: audio.h
//
// XNA Developer Connection
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

#pragma pack(1)

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <d3dx9.h>

#include <string>

using namespace std;
//-----------------------------------------------------------------------------
// Global defines
//-----------------------------------------------------------------------------
extern int INPUTCHANNELS;
#define OUTPUTCHANNELS 8 // maximum number of destination channels supported in this sample

#define NUM_PRESETS 30

// Constants to define our world space
const INT           XMIN = -10;
const INT           XMAX =  10;
const INT           ZMIN = -10;
const INT           ZMAX =  10;

//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
	bool bInitialized;

	// XAudio2
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;
	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2SubmixVoice* pSubmixVoice;
	IUnknown* pReverbEffect;
	BYTE* pbSampleData;

	// 3D
	X3DAUDIO_HANDLE x3DInstance;
	int nFrameToApply3DAudio;

	DWORD dwChannelMask;
	UINT32 nChannels;

	X3DAUDIO_DSP_SETTINGS dspSettings;
	X3DAUDIO_LISTENER listener;
	X3DAUDIO_EMITTER emitter;
	X3DAUDIO_CONE emitterCone;

	D3DXVECTOR3 vListenerPos;
	D3DXVECTOR3 vEmitterPos;
	D3DXVECTOR3 vEmitterVel; // �G�~�b�^�[�̈ړ��x�N�g��
	float fListenerAngle;
	bool  fUseListenerCone;
	bool  fUseInnerRadius;
	bool  fUseRedirectToLFE;

	//FLOAT32 emitterAzimuths[INPUTCHANNELS];
	//FLOAT32 matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	FLOAT32 *emitterAzimuths;
	FLOAT32 *matrixCoefficients;
};

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

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
extern AUDIO_STATE  g_audioState;


//--------------------------------------------------------------------------------------
// External functions
//--------------------------------------------------------------------------------------
HRESULT InitAudio();
HRESULT PrepareAudio( const char *wavname, bool isLoop=false );
HRESULT PrepareAudio( int iResID, HMODULE hModule, bool isLoop=false );
HRESULT UpdateAudio( float fElapsedTime );
HRESULT SetReverb( int nReverb );
VOID PauseAudio( bool resume );
VOID CleanupAudio();

// �R�c�Đ���p�̃X���b�h
unsigned __stdcall X3DAudioThreadFunc(void *);
// �g�b�v���x������Ă΂��R�c�T�E���h�n�̏�����
void ConstructX3DAudioThread();
// �g�b�v���x������Ă΂��R�c�T�E���h�n�̉����
void DestructX3DAudioThread();



// 3D���̍Đ���v������BTRUE���Ԃ��Ă���΁A�󂯕t�����BFALSE���Ԃ��Ă���Ύ󂯕t���Ȃ������B
// ��)
//  X3DPlaySound( 601, CommonGlobalHandle::hResourceWavMod, true, D3DXVECTOR3(9.0f, 0, -5.0f), D3DXVECTOR3(-18.0f, 0, 0.0f) );
BOOL X3DPlaySound( int iResID, HMODULE hResource, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel );

// 3D���̍Đ���v������BTRUE���Ԃ��Ă���΁A�󂯕t�����BFALSE���Ԃ��Ă���Ύ󂯕t���Ȃ������B
// ��)
//	X3DPlaySound( "itsuka.wav", true,  D3DXVECTOR3(0.0f, 0, 0.0f), D3DXVECTOR3(0.0f, 0, 0.0f) );
BOOL X3DPlaySound( string wavefilename, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel );


// 3D���̍Đ����~����B���\�[�X��
BOOL X3DPlaySound( int iResID, HMODULE hResource );

// 3D���̍Đ����~����B�t�@�C������
BOOL X3DPlaySound( string wavefilename );