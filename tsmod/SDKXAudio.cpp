//--------------------------------------------------------------------------------------
// File: audio.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "WinTarget.h"
#include <X3DAudio.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <d3dx9.h>

#include <process.h>

#include "SDKXAudio.h"
#include "SDKwavefile.h"

#include "CommonGlobalHandle.h"

#include <string>
using namespace std;

#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return TRUE; } }
#endif

#pragma comment(lib, "X3daudio.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
AUDIO_STATE g_audioState;

// Specify sound cone to add directionality to listener for artistic effect:
// Emitters behind the listener are defined here to be more attenuated,
// have a lower LPF cutoff frequency,
// yet have a slightly higher reverb send level.

static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI*5.0f/6.0f, X3DAUDIO_PI*11.0f/6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

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



// Must match order of g_PRESET_NAMES
XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[ NUM_PRESETS ] =
{
	XAUDIO2FX_I3DL2_PRESET_FOREST,
	XAUDIO2FX_I3DL2_PRESET_DEFAULT,
	XAUDIO2FX_I3DL2_PRESET_GENERIC,
	XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
	XAUDIO2FX_I3DL2_PRESET_ROOM,
	XAUDIO2FX_I3DL2_PRESET_BATHROOM,
	XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
	XAUDIO2FX_I3DL2_PRESET_STONEROOM,
	XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
	XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
	XAUDIO2FX_I3DL2_PRESET_CAVE,
	XAUDIO2FX_I3DL2_PRESET_ARENA,
	XAUDIO2FX_I3DL2_PRESET_HANGAR,
	XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
	XAUDIO2FX_I3DL2_PRESET_HALLWAY,
	XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
	XAUDIO2FX_I3DL2_PRESET_ALLEY,
	XAUDIO2FX_I3DL2_PRESET_CITY,
	XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
	XAUDIO2FX_I3DL2_PRESET_QUARRY,
	XAUDIO2FX_I3DL2_PRESET_PLAIN,
	XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
	XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
	XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
	XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
	XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
	XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
	XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
	XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
	XAUDIO2FX_I3DL2_PRESET_PLATE,
};


//-----------------------------------------------------------------------------------------
// �\�[�X�{�C�X�̃`�����l�����B��Ƀ\�[�X�t�@�C����ǂݍ��񂾍ۂɁA���I�ɒ��ׂāAX3DAudio�G���W���ւ̃p�����[�^�Ƃ��Đ��ӏ��Ŏg�p����B
//-----------------------------------------------------------------------------------------
int INPUTCHANNELS = 0;



//-----------------------------------------------------------------------------------------
// Initialize the audio by creating the XAudio2 device, mastering voice, etc.
//-----------------------------------------------------------------------------------------
HRESULT InitAudio()
{
	// Clear struct
	ZeroMemory( &g_audioState, sizeof( AUDIO_STATE ) );

	//
	// Initialize XAudio2   XAudio2 �̏������B
	//
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	HRESULT hr;

	// XAudio2 �C���^�[�t�F�[�X�̍쐬
	if( FAILED( hr = XAudio2Create( &g_audioState.pXAudio2, flags ) ) )
		return hr;

	//
	// Create a mastering voice
	//
	// �}�X�^�[�{�C�X�̍쐬
	if( FAILED( hr = g_audioState.pXAudio2->CreateMasteringVoice( &g_audioState.pMasteringVoice ) ) )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		return hr;
	}

	// Check device details to make sure it's within our sample supported parameters
	// �}�X�^�[ �{�C�X �`�����l�����Ɉ�v����X�s�[�J�[ �`�����l���\�������� X3DAudio ���C�u���������������܂��B
	// 3.��ԃI�[�f�B�I�̌v�Z�����s���A�t���[���X�V���ɍX�V���܂��B 
	// X3DAudioInitialize �ɓ˂����ނ��߂̃f�[�^�ݒ�
	XAUDIO2_DEVICE_DETAILS details;
	if( FAILED( hr = g_audioState.pXAudio2->GetDeviceDetails( 0, &details ) ) )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		return hr;
	}

	if( details.OutputFormat.Format.nChannels > OUTPUTCHANNELS )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		return E_FAIL;
	}

	g_audioState.dwChannelMask = details.OutputFormat.dwChannelMask;
	g_audioState.nChannels = details.OutputFormat.Format.nChannels;

	//
	// Create reverb effect
	//
	flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2FX_DEBUG;
#endif

	if( FAILED( hr = XAudio2CreateReverb( &g_audioState.pReverbEffect, flags ) ) )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		return hr;
	}

	//
	// Create a submix voice
	//

	// Performance tip: you need not run global FX with the sample number
	// of channels as the final mix.  For example, this sample runs
	// the reverb in mono mode, thus reducing CPU overhead.
	/*
	[[XAUDIO2_EFFECT_DESCRIPTOR]]
	�G�t�F�N�g �`�F�[���Ŏg�p���� XAPO �ɂ��Ă̏����i�[���܂��B

	typedef struct XAUDIO2_EFFECT_DESCRIPTOR {
	IUnknown *pEffect;
	BOOL InitialState;
	UINT32 OutputChannels;
	} XAUDIO2_EFFECT_DESCRIPTOR;


	�����o
	pEffectXAPO �I�u�W�F�N�g�� IUnknown �C���^�[�t�F�C�X�ւ̃|�C���^�[�ł��BInitialState�G�t�F�N�g���L���X�e�[�g�ŊJ�n�����ꍇ�� TRUE �ł��B����ȊO�̏ꍇ�� FALSE �ł��BOutputChannels�G�t�F�N�g�ɂ���Đ��������o�̓`�����l���̐��ł��B
	����@

	XAPO �C���X�^���X�� IUnknown �C���^�[�t�F�C�X�Ƃ��� XAudio2 �ɓn����܂��BXAudio2 �� IXAPO::QueryInterface ���g�p���� IXAPO �C���^�[�t�F�C�X���擾���AXAPO �� IXAPOParameters �C���^�[�t�F�C�X���������Ă��邩�ǂ��������o���܂��B

	XAPO �� XAudio2 �Ŏg�p������@�̏ڍׂɂ��ẮA�u���@ : �G�t�F�N�g �`�F�[���̍쐬�v����сu���@ : XAudio2 �ł� XAPO �̎g�p�v���Q�Ƃ��Ă��������B

	�v��

	�w�b�_�[: Xaudio2.h �錾
	*/
	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { g_audioState.pReverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	if( FAILED( hr = g_audioState.pXAudio2->CreateSubmixVoice( &g_audioState.pSubmixVoice, 1,
		details.OutputFormat.Format.nSamplesPerSec, 0, 0,
		NULL, &effectChain ) ) )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		SAFE_RELEASE( g_audioState.pReverbEffect );
		return hr;
	}

	// Set default FX params
	/*
	[[XAUDIO2FX_REVERB_PARAMETERS]]
	���o�[�u APO �Ŏg�p����p�����[�^�[���L�q���܂��B
	typedef struct XAUDIO2FX_REVERB_PARAMETERS {
	float WetDryMix;
	UINT32 ReflectionsDelay;
	BYTE ReverbDelay;
	BYTE RearDelay;
	BYTE PositionLeft;
	BYTE PositionRight;
	BYTE PositionMatrixLeft;
	BYTE PositionMatrixRight;
	BYTE EarlyDiffusion;
	BYTE LateDiffusion;
	BYTE LowEQGain;
	BYTE LowEQCutoff;
	BYTE HighEQGain;
	BYTE HighEQCutoff;
	float RoomFilterFreq;
	float RoomFilterMain;
	float RoomFilterHF;
	float ReflectionsGain;
	float ReverbGain;
	float DecayTime;
	float Density;
	float RoomSize;
	} XAUDIO2FX_REVERB_PARAMETERS;

	�����o
	WetDryMix���̕ύX����Ă��Ȃ��T�E���h�ƂȂ�o�͂̊����ł��B���e�l�� 0 �` 100 �ł��BReflectionsDelay�_�C���N�g �p�X�ɑ΂��鏉�����˂̒x�����Ԃł��B�w��\�͈͂� 0 �` 300 �~���b�ł��BReverbDelay�������˂ɑ΂��郊�o�[�u�̒x���ł��B�w��\�͈͂� 0 �` 85 �~���b�ł��BRearDelay������o�͂���щE����o�͂̒x���ł��B�w��\�͈͂� 0 �` 5 �~���b�ł��BPositionLeft�V�~�����[�V������Ԃɂ����鎋���҂ɑ΂��鍶���͂̈ʒu�ł��BPositionLeft ���ŏ��l�ɐݒ肵���ꍇ�A�����͎͂����҂̋߂��ɔz�u����܂��B���̈ʒu�ł́A�T�E���h �t�B�[���h�ɂ����ď������˂��D���ɂȂ�A�c�������͎�܂��āA�U�����������Ȃ�܂��BPositionLeft ���ő�l�ɐݒ肵���ꍇ�A�����͂̓V�~�����[�V���������Ŏ����҂���ő�������ʒu�ɔz�u����܂��BPositionLeft �͎c���������� (�����̎c������) �ɉe�������A�����҂ɑ΂��鉹���̌������̈ʒu�݂̂ɉe�����܂��B�w��\�͈͂� 0 �` 30 (�P�ʂȂ�) �ł��BPositionRightPositionLeft �Ɠ����ł����A�E���͂ɂ̂݉e�����܂��B�w��\�͈͂� 0 �` 30 (�P�ʂȂ�) �ł��BPositionMatrixLeft�������王���҂܂ł̋����ɂ��C���v���b�V�����𑝌������܂��B�w��ł���͈͂� 0 �` 30 (�P�ʂȂ�) �ł��BPositionMatrixRight�������王���҂܂ł̋����ɂ��C���v���b�V�����𑝌������܂��B�w��ł���͈͂� 0 �` 30 (�P�ʂȂ�) �ł��BEarlyDiffusion�X�̕ǂ̔��˓����𐧌䂵�܂��B��������ȕ\�ʂ��V�~�����[�g����ɂ͏����Ȓl��ݒ肵�A�U�����̕\�ʂ��V�~�����[�g����ɂ͑傫�Ȓl��ݒ肵�܂��B�w��\�͈͂� 0 �` 15 (�P�ʂȂ�) �ł��BLateDiffusion�X�̕ǂ̃��o�[�u�����𐧌䂵�܂��B��������ȕ\�ʂ��V�~�����[�g����ɂ͏����Ȓl��ݒ肵�A�U�����̕\�ʂ��V�~�����[�g����ɂ͑傫�Ȓl��ݒ肵�܂��B�w��\�͈͂� 0 �` 15 (�P�ʂȂ�) �ł��BLowEQGain1 kHz �ɂ����錸�����Ԃ���ɂ��Ē���g���̌������Ԃ𒲐����܂��B���ɁA�l�ƃQ�C�� (dB) �̊֌W�������܂��B

	�l
	0 1 2 3 4 5 6 7 8 9 10 11 12 

	�Q�C�� (dB)
	-8 -7 -6 -5 -4 -3 -2 -1 0 +1 +2 +3 +4 
	LowEQGain �̒l�� 8 �̏ꍇ�A����g���̌������Ԃ� 1 kHz �ɂ����錸�����Ԃ��������Ȃ邱�Ƃɒ��ӂ��Ă��������B�w��\�͈͂� 0 �` 12 (�P�ʂȂ�) �ł��BLowEQCutoffLowEQGain �p�����[�^�[�ɂ�萧�䂳��郍�[�p�X �t�B���^�[�̐ܓ_���g����ݒ肵�܂��B���ɁA�l�Ǝ��g�� (Hz) �̊֌W�������܂��B

	�l
	0 1 2 3 4 5 6 7 8 9 

	���g�� (Hz)
	50 100 150 200 250 300 350 400 450 500 
	�w��\�͈͂� 0 �` 9 (�P�ʂȂ�) �ł��B HighEQGain1 kHz �ɂ����錸�����Ԃ���ɂ��č����g���̌������Ԃ𒲐����܂��B0 �ɐݒ肷��ƁA�����g���̉��� 1 kHz �̏ꍇ�Ɠ��������Ō������܂��B�ő�l�ɐݒ肷��ƁA�����g���̉��� 1 kHz �̏ꍇ�����͂邩�ɍ��������Ō������܂��B

	�l
	0 1 2 3 4 5 6 7 8 

	�Q�C�� (dB)
	-8 -7 -6 -5 -4 -3 -2 -1 0 
	�w��\�͈͂� 0 �` 8 (�P�ʂȂ�) �ł��B HighEQCutoffHighEQGain �p�����[�^�[�ɂ�萧�䂳���n�C�p�X �t�B���^�[�̐ܓ_���g����ݒ肵�܂��B���ɁA�l�Ǝ��g�� (kHz) �̊֌W�������܂��B

	�l
	0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 

	���g�� (kHz)
	1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 
	�w��\�͈͂� 0 �` 14 (�P�ʂȂ�) �ł��BRoomFilterFreq�����G�t�F�N�g�̃��[�p�X �t�B���^�[�̐ܓ_���g����ݒ肵�܂��B �w��\�͈͂� 20 �` 20,000 Hz �ł��BRoomFilterMain�������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X �t�B���^�[�̃p�X �o���h���x���x����ݒ肵�܂��B�w��\�͈͂� -100 �` 0 dB �ł��BRoomFilterHF�ܓ_���g�� (RoomFilterFreq) �ł̏������˂ƌ���t�B�[���h�c���̗����ɓK�p����郍�[�p�X �t�B���^�[�̃p�X �o���h���x���x����ݒ肵�܂��B�w��\�͈͂� -100 �` 0 dB �ł��BReflectionsGain�������˂̋��x/���x���𒲐����܂��B�w��\�͈͂� -100 �` 20 dB �ł��BReverbGain���o�[�u�̋��x/���x���𒲐����܂��B�w��\�͈͂� -100 �` 20 dB �ł��BDecayTime1 kHz �ɂ�����c���������Ԃł��B����́A�t�� �X�P�[���̓��͐M���� 60 dB ��������܂ł̎��Ԃł��B�w��\�͈͂� 0.1 �`���� (�b) �ł��BDensity����t�B�[���h�c���̃��[�h���x�𐧌䂵�܂��B���F (colorless) �̋�Ԃł́ADensity ���ő�l (100) �ɐݒ肷��K�v������܂��BDensity ������������ƁA�T�E���h�͂����������� (�����`�t�B���^�[���K�p���ꂽ��) �ɂȂ�܂��B�T�C�����V�~�����[�V��������Ƃ��ɗL���ȃG�t�F�N�g�ł��B�w��\�͈͂� 0 �` 100 �ŁA�P�ʂ̓p�[�Z���g�ł��BRoomSize������Ԃ̌�������̃T�C�Y�ł��B�w��\�͈͂� 1 (30.48 cm) �` 100 �t�B�[�g (30.48 m) �ł��B
	�v��

	�w�b�_�[: Xaudio2fx.h �錾

	*/
	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[0], &native );
	g_audioState.pSubmixVoice->SetEffectParameters( 0, &native, sizeof( native ) );

	//
	// Initialize X3DAudio
	//  Speaker geometry configuration on the final mix, specifies assignment of channels
	//  to speaker positions, defined as per WAVEFORMATEXTENSIBLE.dwChannelMask
	//
	//  SpeedOfSound - speed of sound in user-defined world units/second, used
	//  only for doppler calculations, it must be >= FLT_MIN
	//
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, SPEEDOFSOUND, g_audioState.x3DInstance );

	//
	// Done
	//
	g_audioState.vListenerPos = D3DXVECTOR3( 0, 0, 0 ); // ���X�i�[�̈ʒu�̐ݒ�
	g_audioState.vEmitterPos = D3DXVECTOR3( 9, 0, 5 );  // �G�~�b�^�[�̈ʒu�̐ݒ�
	g_audioState.vEmitterVel = D3DXVECTOR3( -0.1f, 0, 0.0 );  // �G�~�b�^�[�̈ʒu�̐ݒ�

	g_audioState.fListenerAngle = 0;
	g_audioState.fUseListenerCone = TRUE;
	g_audioState.fUseInnerRadius = TRUE;
	g_audioState.fUseRedirectToLFE = ((details.OutputFormat.dwChannelMask & SPEAKER_LOW_FREQUENCY) != 0);

	g_audioState.bInitialized = true;

	return S_OK;
}


void Init3DParams() {

	//
	// Setup 3D audio structs
	//
	g_audioState.listener.Position = g_audioState.vListenerPos;
	g_audioState.listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
	g_audioState.listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
	g_audioState.listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

	g_audioState.emitter.pCone = &g_audioState.emitterCone;
	g_audioState.emitter.pCone->InnerAngle = 0.0f;
	// Setting the inner cone angles to X3DAUDIO_2PI and
	// outer cone other than 0 causes
	// the emitter to act like a point emitter using the
	// INNER cone settings only.
	g_audioState.emitter.pCone->OuterAngle = 0.0f;
	// Setting the outer cone angles to zero causes
	// the emitter to act like a point emitter using the
	// OUTER cone settings only.
	g_audioState.emitter.pCone->InnerVolume = 0.0f;
	g_audioState.emitter.pCone->OuterVolume = 1.0f;
	g_audioState.emitter.pCone->InnerLPF = 0.0f;
	g_audioState.emitter.pCone->OuterLPF = 1.0f;
	g_audioState.emitter.pCone->InnerReverb = 0.0f;
	g_audioState.emitter.pCone->OuterReverb = 1.0f;

	g_audioState.emitter.Position = g_audioState.vEmitterPos;
	g_audioState.emitter.Velocity = g_audioState.vEmitterVel;
	g_audioState.emitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
	g_audioState.emitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
	g_audioState.emitter.ChannelCount = INPUTCHANNELS;
	g_audioState.emitter.ChannelRadius = 1.0f;
	g_audioState.emitterAzimuths = new FLOAT32[INPUTCHANNELS];
	g_audioState.emitter.pChannelAzimuths = g_audioState.emitterAzimuths;

	// Use of Inner radius allows for smoother transitions as
	// a sound travels directly through, above, or below the listener.
	// It also may be used to give elevation cues.
	g_audioState.emitter.InnerRadius = 2.0f;
	g_audioState.emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;;

	g_audioState.emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	g_audioState.emitter.pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	g_audioState.emitter.pLPFDirectCurve = NULL; // use default curve
	g_audioState.emitter.pLPFReverbCurve = NULL; // use default curve
	g_audioState.emitter.pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	g_audioState.emitter.CurveDistanceScaler = 14.0f;
	g_audioState.emitter.DopplerScaler = 1.0f;

	g_audioState.dspSettings.SrcChannelCount = INPUTCHANNELS;
	g_audioState.dspSettings.DstChannelCount = g_audioState.nChannels;
	g_audioState.matrixCoefficients = new FLOAT32[ INPUTCHANNELS * OUTPUTCHANNELS];
	g_audioState.dspSettings.pMatrixCoefficients = g_audioState.matrixCoefficients;
}



//-----------------------------------------------------------------------------
// Prepare a looping wave
//-----------------------------------------------------------------------------

HRESULT PrepareAudio( int iResID, HMODULE hModule, bool isLoop )
{
	if( !g_audioState.bInitialized )
		return E_FAIL;

	if( g_audioState.pSourceVoice )
	{
		g_audioState.pSourceVoice->Stop( 0 );
		g_audioState.pSourceVoice->DestroyVoice();
		g_audioState.pSourceVoice = 0;
	}

	//
	// Search for media
	//

	// char strFilePath[ MAX_PATH ];
	// char wavFilePath[ MAX_PATH ];

	// strcpy_s( wavFilePath, MAX_PATH, wavname );
	// strcpy_s( strFilePath, MAX_PATH, wavname );

	HRESULT hr;

	// V_RETURN( DXUTFindDXSDKMediaFileCch( strFilePath, MAX_PATH, wavFilePath ) );

	//
	// Read in the wave file
	//
	CWaveFile wav;
	//    V_RETURN( wav.Open( strFilePath, NULL, WAVEFILE_READ ) );
	V_RETURN( wav.Open( iResID, NULL, WAVEFILE_READ, hModule ) );

	// Get format of wave file
	WAVEFORMATEX* pwfx = wav.GetFormat();
	INPUTCHANNELS = pwfx->nChannels;


	// ��3D�̃p�����[�^�̐ݒ�̑����������ŁB���\�[�XWAV�̃`�����l���������������̂ő����������o����B
	Init3DParams();


	// Calculate how many bytes and samples are in the wave
	DWORD cbWaveSize = wav.GetSize();


	// Read the sample data into memory
	SAFE_DELETE_ARRAY( g_audioState.pbSampleData );

	g_audioState.pbSampleData = new BYTE[ cbWaveSize ];

	V_RETURN( wav.Read( g_audioState.pbSampleData, cbWaveSize, &cbWaveSize ) );

	//
	// Play the wave using a source voice that sends to both the submix and mastering voices
	//
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = g_audioState.pMasteringVoice;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[1].pOutputVoice = g_audioState.pSubmixVoice;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };


	// create the source voice
	V_RETURN( g_audioState.pXAudio2->CreateSourceVoice( &g_audioState.pSourceVoice, pwfx, 0,
		2.0f, NULL, &sendList ) );

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = g_audioState.pbSampleData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = cbWaveSize;

	// ���[�v���邩���Ȃ���
	if ( isLoop ) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	} else {
		buffer.LoopCount = 0;
		buffer.LoopLength = 0;
		buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
	}

	V_RETURN( g_audioState.pSourceVoice->SubmitSourceBuffer( &buffer ) );

	V_RETURN( g_audioState.pSourceVoice->Start( 0 ) );

	g_audioState.nFrameToApply3DAudio = 0;

	return S_OK;
}

HRESULT PrepareAudio( const char *wavname, bool isLoop )
{
	if( !g_audioState.bInitialized )
		return E_FAIL;

	if( g_audioState.pSourceVoice )
	{
		g_audioState.pSourceVoice->Stop( 0 );
		g_audioState.pSourceVoice->DestroyVoice();
		g_audioState.pSourceVoice = 0;
	}

	//
	// Search for media
	//

	char strFilePath[ MAX_PATH ];
	strcpy_s( strFilePath, MAX_PATH, wavname );

	HRESULT hr;

	//
	// Read in the wave file
	//
	CWaveFile wav;
	V_RETURN( wav.Open( strFilePath, NULL, WAVEFILE_READ ) );

	// Get format of wave file
	WAVEFORMATEX* pwfx = wav.GetFormat();
	INPUTCHANNELS = pwfx->nChannels;


	// ��3D�̃p�����[�^�̐ݒ�̑����������ŁB���\�[�XWAV�̃`�����l���������������̂ő����������o����B
	Init3DParams();


	// Calculate how many bytes and samples are in the wave
	DWORD cbWaveSize = wav.GetSize();


	// Read the sample data into memory
	SAFE_DELETE_ARRAY( g_audioState.pbSampleData );

	g_audioState.pbSampleData = new BYTE[ cbWaveSize ];

	V_RETURN( wav.Read( g_audioState.pbSampleData, cbWaveSize, &cbWaveSize ) );

	//
	// Play the wave using a source voice that sends to both the submix and mastering voices
	//
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = g_audioState.pMasteringVoice;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[1].pOutputVoice = g_audioState.pSubmixVoice;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };


	// create the source voice
	V_RETURN( g_audioState.pXAudio2->CreateSourceVoice( &g_audioState.pSourceVoice, pwfx, 0,
		2.0f, NULL, &sendList ) );

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = g_audioState.pbSampleData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = cbWaveSize;
	if ( isLoop ) {
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	V_RETURN( g_audioState.pSourceVoice->SubmitSourceBuffer( &buffer ) );

	V_RETURN( g_audioState.pSourceVoice->Start( 0 ) );

	g_audioState.nFrameToApply3DAudio = 0;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Perform per-frame update of audio
//-----------------------------------------------------------------------------
HRESULT UpdateAudio( float fElapsedTime )
{
	if( !g_audioState.bInitialized )
		return S_FALSE;

	//    if( g_audioState.nFrameToApply3DAudio == 0 )
	{
		// Calculate listener orientation in x-z plane
		if( g_audioState.vListenerPos.x != g_audioState.listener.Position.x
			|| g_audioState.vListenerPos.z != g_audioState.listener.Position.z )
		{
			D3DXVECTOR3 vDelta = g_audioState.vListenerPos - g_audioState.listener.Position;

			g_audioState.fListenerAngle = float( atan2( vDelta.x, vDelta.z ) );

			vDelta.y = 0.0f;
			D3DXVec3Normalize( &vDelta, &vDelta );

			g_audioState.listener.OrientFront.x = vDelta.x;
			g_audioState.listener.OrientFront.y = 0.f;
			g_audioState.listener.OrientFront.z = vDelta.z;
		}


		if (g_audioState.fUseListenerCone)
		{
			g_audioState.listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;
		}
		else
		{
			g_audioState.listener.pCone = NULL;
		}



		if (g_audioState.fUseInnerRadius)
		{
			g_audioState.emitter.InnerRadius = 2.0f;
			g_audioState.emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;
		}
		else
		{
			g_audioState.emitter.InnerRadius = 0.0f;
			g_audioState.emitter.InnerRadiusAngle = 0.0f;
		}

		if( fElapsedTime > 0 )
		{
			D3DXVECTOR3 lVelocity = ( g_audioState.vListenerPos - g_audioState.listener.Position ) / fElapsedTime;
			g_audioState.listener.Position = g_audioState.vListenerPos;
			g_audioState.listener.Velocity = lVelocity;

			D3DXVECTOR3 eVelocity = ( g_audioState.vEmitterPos - g_audioState.emitter.Position ) / fElapsedTime;
			g_audioState.emitter.Position = g_audioState.vEmitterPos;
			g_audioState.emitter.Velocity = eVelocity;
		}

		DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT 
			| X3DAUDIO_CALCULATE_LPF_DIRECT;
		if (g_audioState.fUseRedirectToLFE)
		{
			// On devices with an LFE channel, allow the mono source data
			// to be routed to the LFE destination channel.
			dwCalcFlags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
		}

		X3DAudioCalculate( g_audioState.x3DInstance, &g_audioState.listener, &g_audioState.emitter, dwCalcFlags,
			&g_audioState.dspSettings );

		IXAudio2SourceVoice* voice = g_audioState.pSourceVoice;
		if( voice )
		{
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

			X3DAudioCalculate�֐��́A���o�[�u�̌��ʂȂǂ��v�Z�\�ł��B
			*/
			/*
			5.��ԃI�[�f�B�I���������߂ɁA�ȗ������ꂽ X/Z �O���b�h��ɂ��� X3DAudio �̃G�~�b�^�[ �I�u�W�F�N�g�ƃ��X�i�[ �I�u�W�F�N�g��\�����āA���䂵�܂��B 
			1.�e�����_�����O �t���[���ŁA�G�~�b�^�[�ƃ��X�i�[�̌��݂̈ʒu��`�悵�܂��B
			2.�p�����[�^�[����̕ύX��K�p���āA�e�t���[���� UI �v�f���X�V���܂��B
			3.��ԃI�[�f�B�I�̌v�Z�����s���A�t���[���X�V���ɍX�V���܂��B 
			���t���[���̌o�ߎ��ԂɊ�Â��A�V�����ʒu�Ƒ��x�̏����g�p���āA�G�~�b�^�[ �I�u�W�F�N�g�ƃ��X�i�[ �I�u�W�F�N�g���X�V���܂��B
			��X3DAudioCalculate ���\�b�h���Ăяo���āA���ʂ� DSP �R���g���[���̐ݒ���v�Z���܂��B
			��DSP �̐ݒ���AIXAudio2Voice::SetOutputMatrix �֐��� IXAudio2SourceVoice::SetFrequencyRatio �֐����o�R���ă\�[�X �{�C�X�ɓn���܂��B
			*/

			// Apply X3DAudio generated DSP settings to XAudio2
			voice->SetFrequencyRatio( g_audioState.dspSettings.DopplerFactor );

			voice->SetOutputMatrix( g_audioState.pMasteringVoice, INPUTCHANNELS, g_audioState.nChannels, g_audioState.matrixCoefficients );

			voice->SetOutputMatrix(g_audioState.pSubmixVoice, INPUTCHANNELS, 1, &g_audioState.dspSettings.ReverbLevel);

			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * g_audioState.dspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			voice->SetOutputFilterParameters(g_audioState.pMasteringVoice, &FilterParametersDirect);
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * g_audioState.dspSettings.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
			voice->SetOutputFilterParameters(g_audioState.pSubmixVoice, &FilterParametersReverb);

		}
	}

	//    g_audioState.nFrameToApply3DAudio++;
	//    g_audioState.nFrameToApply3DAudio &= 1;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Set reverb effect
//-----------------------------------------------------------------------------
HRESULT SetReverb( int nReverb )
{
	if( !g_audioState.bInitialized )
		return S_FALSE;

	if( nReverb < 0 || nReverb >= NUM_PRESETS )
		return E_FAIL;

	if( g_audioState.pSubmixVoice )
	{
		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[ nReverb ], &native );
		g_audioState.pSubmixVoice->SetEffectParameters( 0, &native, sizeof( native ) );
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
/* �{�����[���̐ݒ�
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
*/
//-----------------------------------------------------------------------------
HRESULT SetVolume( float Volume ) {
	if( !g_audioState.bInitialized )
		return S_FALSE;

	g_audioState.pSourceVoice->SetVolume( Volume );
	return S_OK;
}

// �X�e�[�g�̎擾�B(�Đ����؂��ǂ���)
bool GetAudioPlaying() {
	XAUDIO2_VOICE_STATE state = {0};
	g_audioState.pSourceVoice->GetState(&state);
	
	return state.BuffersQueued > 0;
}

//-----------------------------------------------------------------------------
// Pause audio playback
//-----------------------------------------------------------------------------
VOID PauseAudio( bool resume )
{
	if( !g_audioState.bInitialized )
		return;

	if( resume )
		g_audioState.pXAudio2->StartEngine();
	else
		g_audioState.pXAudio2->StopEngine();
}



//-----------------------------------------------------------------------------
// Releases XAudio2
//-----------------------------------------------------------------------------
VOID CleanupAudio()
{
	if( !g_audioState.bInitialized )
		return;

	if( g_audioState.pSourceVoice )
	{
		g_audioState.pSourceVoice->DestroyVoice();
		g_audioState.pSourceVoice = NULL;
	}

	if( g_audioState.pSubmixVoice )
	{
		g_audioState.pSubmixVoice->DestroyVoice();
		g_audioState.pSubmixVoice = NULL;
	}

	if( g_audioState.pMasteringVoice )
	{
		g_audioState.pMasteringVoice->DestroyVoice();
		g_audioState.pMasteringVoice = NULL;
	}

	g_audioState.pXAudio2->StopEngine();
	SAFE_RELEASE( g_audioState.pXAudio2 );
	SAFE_RELEASE( g_audioState.pReverbEffect );

	SAFE_DELETE_ARRAY( g_audioState.pbSampleData );
	SAFE_DELETE_ARRAY( g_audioState.emitterAzimuths );
	SAFE_DELETE_ARRAY( g_audioState.matrixCoefficients );

	CoUninitialize();

	g_audioState.bInitialized = false;
}





namespace X3DAudioThreadData {
	bool		isPushed = false;					// ��񂪃v�b�V�����ꂽ���ǂ����B���ꂪON�̊Ԃ́AX3DAudioThreadFunc�͎󂯎���B�󂯎������OFF�Ƃ���B���A���ꂪON�̊Ԃ͊O���瑗�M���Ă͂Ȃ�Ȃ��B

	string		szPushedX3DAudioFileName = "";		// �t�@�C�������ڔ�
	int			iPushedX3DAudioResID = 0;			// ���\�[�X�h�c��
	HMODULE		hPushedX3DAudioModuleHandle = NULL;	// ���\�[�X�������Ă郂�W���[���n���h��

	bool		isLoop = false;

	D3DXVECTOR3 vEmitterPos = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 vEmitterVel = D3DXVECTOR3(0,0,0);

	bool	isMustThreadClose = false;
}

unsigned __stdcall X3DAudioThreadFunc(void *) {


	using namespace X3DAudioThreadData;

	while ( TRUE ) {
		// �����X���b�h���I�����ׂ��Ȃ�Υ��
		if ( isMustThreadClose ) {
			// �����ɃN���[���A�b�v���ău���C�N�˂��̃X���b�h�I���M�����o���āA��͏I��邾�낤�c
			CleanupAudio();
			break;
		}

		// ���݊O����v�b�V�����ꂽ�B
		if ( isPushed ) {

			// ���\�[�X�h�c�A���W���[�����AWAV�t�@�C������3��ނ��ɃX���b�h�����ɃR�s�[���Ă��܂��B
			int iCurX3DAudioResID = iPushedX3DAudioResID;
			string szCurX3DAudioFileName = szPushedX3DAudioFileName;
			HMODULE hCurX3DAudioModuleHandle = hPushedX3DAudioModuleHandle;

			// ���\�[�X�h�c�̕��̎w��̂悤���B
			if ( iCurX3DAudioResID && hCurX3DAudioModuleHandle ) {

				// �V���ȉ����Ȃ̂ŃG�~�b�^�|�̍Đݒ�
				g_audioState.vEmitterPos = vEmitterPos;  // �G�~�b�^�[�̈ʒu�̐ݒ�
				g_audioState.vEmitterVel = vEmitterVel;  // �G�~�b�^�[�̈ʒu�̐ݒ�

				// WAV���\�[�X�Ƃ��ēǂݍ���ōĐ�
				PrepareAudio(iCurX3DAudioResID, hCurX3DAudioModuleHandle, isLoop);

			}
			else if ( szCurX3DAudioFileName.size() ) {

				// �V���ȉ����Ȃ̂ŃG�~�b�^�|�̍Đݒ�
				g_audioState.vEmitterPos = vEmitterPos;  // �G�~�b�^�[�̈ʒu�̐ݒ�
				g_audioState.vEmitterVel = vEmitterVel;  // �G�~�b�^�[�̈ʒu�̐ݒ�

				// WAV�t�@�C���Ƃ��ēǂݍ���ōĐ�
				PrepareAudio((char *)szCurX3DAudioFileName.c_str(), isLoop);
			}

			// �n�����疳�����B
			szPushedX3DAudioFileName = "";
			iPushedX3DAudioResID = 0;
			hPushedX3DAudioModuleHandle = NULL;
			isLoop = false;

			// �R�s�[�����̂ŊO����̏㏑�����ēx�F�߂�B
			// ���K�����̈�ԉ��̈ʒu�łȂ���΂Ȃ�Ȃ��B�Ȃ��Ȃ�AX3DAudioThreadData�̒l���R�s�[���Ă���Œ��ɊO�����珑���������Ȃ��悤�ɂ��邽�߁B
			isPushed = false;

			UpdateAudio(0);
		}

		// �܂��́A����1�ōĐ��󋵂��A�b�v�f�[�g
		Sleep(50);
		UpdateAudio(50);
		g_audioState.vEmitterPos = g_audioState.vEmitterPos + g_audioState.vEmitterVel / 1000 * 50;  // �G�~�b�^�[�̈ʒu������ɔ����čX�V
	}

	_endthreadex(0);
	return TRUE;
}

HANDLE hX3DAudioThread = NULL;
void ConstructX3DAudioThread() {
	// �����ɗ��鎞�ɂ́A���ł�n6pwav�n���W���[���͓ǂݍ��ݍ�

	// 3D�T�E���h�̏�����
	InitAudio();

	unsigned thID = 0;
	hX3DAudioThread = (HANDLE)_beginthreadex(NULL, 0, X3DAudioThreadFunc, NULL, 0, &thID);
}

void DestructX3DAudioThread() {
	X3DAudioThreadData::isMustThreadClose = true;
	WaitForSingleObject(hX3DAudioThread, INFINITE); /* �X���b�h���I������܂ő҂B */ 
	CloseHandle(hX3DAudioThread);
	CleanupAudio();
}


// 3D���̍Đ���v������BTRUE���Ԃ��Ă���΁A�󂯕t�����BFALSE���Ԃ��Ă���Ύ󂯕t���Ȃ������B
BOOL X3DPlaySound( int iResID, HMODULE hResource, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel ) {
	// X3D�G���W�����������Ǝ�M����������łȂ��Ǝ󂯕t���Ȃ��B
	if ( X3DAudioThreadData::isPushed == false ) {

		// ���\�[�X�h�c�̑��
		X3DAudioThreadData::szPushedX3DAudioFileName = "";		// �t�@�C�������ڔ�
		X3DAudioThreadData::iPushedX3DAudioResID = iResID;
		X3DAudioThreadData::hPushedX3DAudioModuleHandle = hResource;
		X3DAudioThreadData::isLoop = isLoop;
		X3DAudioThreadData::vEmitterPos = vEmitterPos;
		X3DAudioThreadData::vEmitterVel = vEmitterVel;


		// �v�b�V�������̂Ń��V�[�u����Ƃ������𑗂�
		X3DAudioThreadData::isPushed = true;

		return true;
	}
	return false;
}

// 3D���̍Đ���v������BTRUE���Ԃ��Ă���΁A�󂯕t�����BFALSE���Ԃ��Ă���Ύ󂯕t���Ȃ������B
BOOL X3DPlaySound( string wavefilename, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel ) {
	// X3D�G���W�����������Ǝ�M����������łȂ��Ǝ󂯕t���Ȃ��B
	if ( X3DAudioThreadData::isPushed == false ) {

		// ���\�[�X�h�c�̑��
		X3DAudioThreadData::szPushedX3DAudioFileName = wavefilename;		// �t�@�C�������ڔ�
		X3DAudioThreadData::iPushedX3DAudioResID = 0;
		X3DAudioThreadData::hPushedX3DAudioModuleHandle = NULL;
		X3DAudioThreadData::isLoop = isLoop;
		X3DAudioThreadData::vEmitterPos = vEmitterPos;
		X3DAudioThreadData::vEmitterVel = vEmitterVel;


		// �v�b�V�������̂Ń��V�[�u����Ƃ������𑗂�
		X3DAudioThreadData::isPushed = true;

		return true;
	}
	return false;
}


BOOL X3DPlaySound( int iResID, HMODULE hResource ) {
	PauseAudio( 0 );
	return TRUE;
}

BOOL X3DPlaySound( string wavefilename ) {
	PauseAudio( 0 );
	return TRUE;
}