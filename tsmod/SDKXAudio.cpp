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
エミッタの向きに対して、単一チャンネルの非 LFE エミッタの方向を DSP 動作のスケーリングによって指定します。

コピー
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

メンバ
InnerAngle
内部コーンの角度 (ラジアン単位) です。この値は 0.0f ～ X3DAUDIO_2PI の範囲内でなければなりません。
OuterAngle
外部コーンの角度 (ラジアン単位) です。この値は InnerAngle ～ X3DAUDIO_2PI の範囲内でなければなりません。
InnerVolume
内部コーン上/内のボリューム スケーラです。この値は 0.0f ～ 2.0f の範囲内でなければなりません。
OuterVolume
外部コーン上/以降のボリューム スケーラです。この値は 0.0f ～ 2.0f の範囲内でなければなりません。
InnerLPF
内部コーン上/内のローパスフィルタダイレクト パスまたはリバーブ パスの係数スケーラです。この値はローパスフィルタ計算のみに使用され、0.0f ～ 1.0f の範囲内でなければなりません。
OuterLPF
外部コーン上/以降のローパスフィルタダイレクト パスまたはリバーブ パスの係数スケーラです。この値はローパスフィルタ計算のみに使用され、0.0f ～ 1.0f の範囲内でなければなりません。
InnerReverb
内部コーン上/内のリバーブ センド レベル スケーラです。この値は 0.0f ～ 2.0f の範囲内でなければなりません。
OuterReverb
外部コーン上/以降のリバーブ センド レベル スケーラです。この値は 0.0f ～ 2.0f の範囲内でなければなりません。
解説　
サウンド コーンの詳細については、「サウンド コーン」を参照してください。

要件
ヘッダー: X3daudio.h 宣言
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
線形セグメントで構成された明示的な区分曲線を定義し、正規化された距離に対して DSP 動作を直接定義します。

コピー
typedef struct X3DAUDIO_DISTANCE_CURVE {
X3DAUDIO_DISTANCE_CURVE_POINT *pPoints;
UINT32 PointCount;
} X3DAUDIO_DISTANCE_CURVE;

メンバ
pPoints
X3DAUDIO_DISTANCE_CURVE_POINT 配列です。この配列には複製があってはならず、距離を基準にして昇順でソートされていなければなりません。
PointCount
距離カーブ上のポイントの数です。すべてのカーブには、0.0f と 1.0f の正規化された距離の値をそれぞれ定義する少なくとも 2 つのエンドポイントがあるので、2 つ以上のポイントがあるはずです。
要件
ヘッダー: X3daudio.h 宣言
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
// ソースボイスのチャンネル数。先にソースファイルを読み込んだ際に、動的に調べて、X3DAudioエンジンへのパラメータとして数箇所で使用する。
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
	// Initialize XAudio2   XAudio2 の初期化。
	//
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	HRESULT hr;

	// XAudio2 インターフェースの作成
	if( FAILED( hr = XAudio2Create( &g_audioState.pXAudio2, flags ) ) )
		return hr;

	//
	// Create a mastering voice
	//
	// マスターボイスの作成
	if( FAILED( hr = g_audioState.pXAudio2->CreateMasteringVoice( &g_audioState.pMasteringVoice ) ) )
	{
		SAFE_RELEASE( g_audioState.pXAudio2 );
		return hr;
	}

	// Check device details to make sure it's within our sample supported parameters
	// マスター ボイス チャンネル数に一致するスピーカー チャンネル構成を持つ X3DAudio ライブラリを初期化します。
	// 3.空間オーディオの計算を実行し、フレーム更新時に更新します。 
	// X3DAudioInitialize に突っ込むためのデータ設定
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
	エフェクト チェーンで使用する XAPO についての情報を格納します。

	typedef struct XAUDIO2_EFFECT_DESCRIPTOR {
	IUnknown *pEffect;
	BOOL InitialState;
	UINT32 OutputChannels;
	} XAUDIO2_EFFECT_DESCRIPTOR;


	メンバ
	pEffectXAPO オブジェクトの IUnknown インターフェイスへのポインターです。InitialStateエフェクトが有効ステートで開始される場合は TRUE です。それ以外の場合は FALSE です。OutputChannelsエフェクトによって生成される出力チャンネルの数です。
	解説　

	XAPO インスタンスは IUnknown インターフェイスとして XAudio2 に渡されます。XAudio2 は IXAPO::QueryInterface を使用して IXAPO インターフェイスを取得し、XAPO が IXAPOParameters インターフェイスを実装しているかどうかを検出します。

	XAPO を XAudio2 で使用する方法の詳細については、「方法 : エフェクト チェーンの作成」および「方法 : XAudio2 での XAPO の使用」を参照してください。

	要件

	ヘッダー: Xaudio2.h 宣言
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
	リバーブ APO で使用するパラメーターを記述します。
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

	メンバ
	WetDryMix元の変更されていないサウンドとなる出力の割合です。許容値は 0 ～ 100 です。ReflectionsDelayダイレクト パスに対する初期反射の遅延時間です。指定可能範囲は 0 ～ 300 ミリ秒です。ReverbDelay初期反射に対するリバーブの遅延です。指定可能範囲は 0 ～ 85 ミリ秒です。RearDelay左後方出力および右後方出力の遅延です。指定可能範囲は 0 ～ 5 ミリ秒です。PositionLeftシミュレーション空間における視聴者に対する左入力の位置です。PositionLeft を最小値に設定した場合、左入力は視聴者の近くに配置されます。この位置では、サウンド フィールドにおいて初期反射が優勢になり、残響減衰は弱まって、振幅が小さくなります。PositionLeft を最大値に設定した場合、左入力はシミュレーション室内で視聴者から最大限遠い位置に配置されます。PositionLeft は残響減衰時間 (部屋の残響効果) に影響せず、視聴者に対する音源の見かけの位置のみに影響します。指定可能範囲は 0 ～ 30 (単位なし) です。PositionRightPositionLeft と同じですが、右入力にのみ影響します。指定可能範囲は 0 ～ 30 (単位なし) です。PositionMatrixLeft音源から視聴者までの距離によるインプレッションを増減させます。指定できる範囲は 0 ～ 30 (単位なし) です。PositionMatrixRight音源から視聴者までの距離によるインプレッションを増減させます。指定できる範囲は 0 ～ 30 (単位なし) です。EarlyDiffusion個々の壁の反射特性を制御します。堅く平らな表面をシミュレートするには小さな値を設定し、散乱性の表面をシミュレートするには大きな値を設定します。指定可能範囲は 0 ～ 15 (単位なし) です。LateDiffusion個々の壁のリバーブ特性を制御します。堅く平らな表面をシミュレートするには小さな値を設定し、散乱性の表面をシミュレートするには大きな値を設定します。指定可能範囲は 0 ～ 15 (単位なし) です。LowEQGain1 kHz における減衰時間を基準にして低周波数の減衰時間を調整します。次に、値とゲイン (dB) の関係を示します。

	値
	0 1 2 3 4 5 6 7 8 9 10 11 12 

	ゲイン (dB)
	-8 -7 -6 -5 -4 -3 -2 -1 0 +1 +2 +3 +4 
	LowEQGain の値が 8 の場合、低周波数の減衰時間と 1 kHz における減衰時間が等しくなることに注意してください。指定可能範囲は 0 ～ 12 (単位なし) です。LowEQCutoffLowEQGain パラメーターにより制御されるローパス フィルターの折点周波数を設定します。次に、値と周波数 (Hz) の関係を示します。

	値
	0 1 2 3 4 5 6 7 8 9 

	周波数 (Hz)
	50 100 150 200 250 300 350 400 450 500 
	指定可能範囲は 0 ～ 9 (単位なし) です。 HighEQGain1 kHz における減衰時間を基準にして高周波数の減衰時間を調整します。0 に設定すると、高周波数の音が 1 kHz の場合と同じ割合で減衰します。最大値に設定すると、高周波数の音が 1 kHz の場合よりもはるかに高い割合で減衰します。

	値
	0 1 2 3 4 5 6 7 8 

	ゲイン (dB)
	-8 -7 -6 -5 -4 -3 -2 -1 0 
	指定可能範囲は 0 ～ 8 (単位なし) です。 HighEQCutoffHighEQGain パラメーターにより制御されるハイパス フィルターの折点周波数を設定します。次に、値と周波数 (kHz) の関係を示します。

	値
	0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 

	周波数 (kHz)
	1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 
	指定可能範囲は 0 ～ 14 (単位なし) です。RoomFilterFreq室内エフェクトのローパス フィルターの折点周波数を設定します。 指定可能範囲は 20 ～ 20,000 Hz です。RoomFilterMain初期反射と後期フィールド残響の両方に適用されるローパス フィルターのパス バンド強度レベルを設定します。指定可能範囲は -100 ～ 0 dB です。RoomFilterHF折点周波数 (RoomFilterFreq) での初期反射と後期フィールド残響の両方に適用されるローパス フィルターのパス バンド強度レベルを設定します。指定可能範囲は -100 ～ 0 dB です。ReflectionsGain初期反射の強度/レベルを調整します。指定可能範囲は -100 ～ 20 dB です。ReverbGainリバーブの強度/レベルを調整します。指定可能範囲は -100 ～ 20 dB です。DecayTime1 kHz における残響減衰時間です。これは、フル スケールの入力信号が 60 dB 減衰するまでの時間です。指定可能範囲は 0.1 ～無限 (秒) です。Density後期フィールド残響のモード密度を制御します。無色 (colorless) の空間では、Density を最大値 (100) に設定する必要があります。Density を小さくすると、サウンドはくぐもった音 (くし形フィルターが適用された音) になります。サイロをシミュレーションするときに有効なエフェクトです。指定可能範囲は 0 ～ 100 で、単位はパーセントです。RoomSize音響空間の見かけ上のサイズです。指定可能範囲は 1 (30.48 cm) ～ 100 フィート (30.48 m) です。
	要件

	ヘッダー: Xaudio2fx.h 宣言

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
	g_audioState.vListenerPos = D3DXVECTOR3( 0, 0, 0 ); // リスナーの位置の設定
	g_audioState.vEmitterPos = D3DXVECTOR3( 9, 0, 5 );  // エミッターの位置の設定
	g_audioState.vEmitterVel = D3DXVECTOR3( -0.1f, 0, 0.0 );  // エミッターの位置の設定

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


	// ★3Dのパラメータの設定の続きをここで。リソースWAVのチャンネル数が判明したので続きを処理出来る。
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

	// ループするかしないか
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


	// ★3Dのパラメータの設定の続きをここで。リソースWAVのチャンネル数が判明したので続きを処理出来る。
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
			XAudio2では、SetOutputMatrix関数で各チャンネルのボリュームの調整を行い
			SetFrequencyRatio関数でピッチの調整を行うことができます。
			で、この2つの関数に渡す調整用のパラメータが必要なのですが
			それはX3DAudioというAPIを使えば勝手に計算してくれます。

			このX3DAudioは非常にシンプルなAPIで、関数は2つしかありません。
			X3DAudioInitializeとX3DAudioCalculateです。
			名前から分かるように、X3DAudioInitializeで初期化、
			X3DAudioCalculateで計算するわけですが
			計算するためにはエミッタとリスナーの情報が必要です。
			それぞれ構造体が定義されており、位置、向き、速度などの設定をします。
			計算結果はX3DAUDIO_DSP_SETTINGSという構造体で得ることができます。
			この中にドップラー係数やボリュームレベルの配列などがあるので
			それをXAudio2の関数に渡して実際にボリュームとピッチを調整するわけです。

			X3DAudioCalculate関数は、リバーブの効果なども計算可能です。
			*/
			/*
			5.空間オーディオを示すために、簡略化された X/Z グリッド上にある X3DAudio のエミッター オブジェクトとリスナー オブジェクトを表示して、制御します。 
			1.各レンダリング フレームで、エミッターとリスナーの現在の位置を描画します。
			2.パラメーター制御の変更を適用して、各フレームで UI 要素を更新します。
			3.空間オーディオの計算を実行し、フレーム更新時に更新します。 
			■フレームの経過時間に基づき、新しい位置と速度の情報を使用して、エミッター オブジェクトとリスナー オブジェクトを更新します。
			■X3DAudioCalculate メソッドを呼び出して、結果の DSP コントロールの設定を計算します。
			■DSP の設定を、IXAudio2Voice::SetOutputMatrix 関数と IXAudio2SourceVoice::SetFrequencyRatio 関数を経由してソース ボイスに渡します。
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
/* ボリュームの設定
XAudio2 にて、音量変更に成功しました。
と言ってもそんな難しい事はありません。

IXAudio2SourceVoice(IXAudio2Voice) のメンバメソッド SetVolume を使うだけです。
ソースボイスに対しての音量を変更できます。


HRESULT SetVolume(  float Volume,  UINT32 OperationSet = XAUDIO2_COMMIT_NOW);
[ MSDN ] : http://bit.ly/wTT5I4
中身はこんな感じで、第一引数にボリュームを設定してあげればいいだけです。
めっっちゃ簡単です。

戻り値はこんな風に書いてます。

Returns S_OK if successful, an error code otherwise.
See XAudio2 Error Codes for descriptions of error codes

関数が成功すれば S_OK 、失敗すればエラーコードが返ってきます。
エラーコードは XAudio2 のエラーコード を見てねらしいです。



扱い自体は簡単なんですけど意味不明な単語が多すぎてさっぱりなのも事実。
ゲインってなんじゃらほい？って感じです。
とりあえず重要そうな所？だと思います。

XAUDIO2_MAX_VOLUME_LEVEL (-2^24 to 2^24), with a maximum gain of 144.5 dB

XAUDIO2_MAX_VOLUME_LEVEL(多分扱える範囲)は、-2^24 ～ 2^24 の間だそうです。
最大ゲインは 144.5db らしいですが良く解らない；；

また、こんな事も書かれてます。

A volume level of 1.0 means there is no attenuation or gain and 0 means silence.

ボリュームレベル 1.0 は、衰弱ないし、ゲインがないと言う意味です。
また、0 は無音です。

と言う事は、1.0f が 100%、0.0f が 0% と言う意味なのではないでしょうか。
しかし、ゲインがないという事はどういう事なのか…。
*/
//-----------------------------------------------------------------------------
HRESULT SetVolume( float Volume ) {
	if( !g_audioState.bInitialized )
		return S_FALSE;

	g_audioState.pSourceVoice->SetVolume( Volume );
	return S_OK;
}

// ステートの取得。(再生中華かどうか)
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
	bool		isPushed = false;					// 情報がプッシュされたかどうか。これがONの間は、X3DAudioThreadFuncは受け取れる。受け取ったらOFFとする。又、これがONの間は外から送信してはならない。

	string		szPushedX3DAudioFileName = "";		// ファイル名直接版
	int			iPushedX3DAudioResID = 0;			// リソースＩＤ版
	HMODULE		hPushedX3DAudioModuleHandle = NULL;	// リソースが入ってるモジュールハンドル

	bool		isLoop = false;

	D3DXVECTOR3 vEmitterPos = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 vEmitterVel = D3DXVECTOR3(0,0,0);

	bool	isMustThreadClose = false;
}

unsigned __stdcall X3DAudioThreadFunc(void *) {


	using namespace X3DAudioThreadData;

	while ( TRUE ) {
		// もしスレッドを終了すべきならば･･･
		if ( isMustThreadClose ) {
			// 即座にクリーンアップしてブレイク⇒このスレッド終了信号を出して、後は終わるだろう…
			CleanupAudio();
			break;
		}

		// 現在外からプッシュされた。
		if ( isPushed ) {

			// リソースＩＤ、モジュール名、WAVファイル名の3種類を先にスレッド内部にコピーしてしまう。
			int iCurX3DAudioResID = iPushedX3DAudioResID;
			string szCurX3DAudioFileName = szPushedX3DAudioFileName;
			HMODULE hCurX3DAudioModuleHandle = hPushedX3DAudioModuleHandle;

			// リソースＩＤの方の指定のようだ。
			if ( iCurX3DAudioResID && hCurX3DAudioModuleHandle ) {

				// 新たな音源なのでエミッタ－の再設定
				g_audioState.vEmitterPos = vEmitterPos;  // エミッターの位置の設定
				g_audioState.vEmitterVel = vEmitterVel;  // エミッターの位置の設定

				// WAVリソースとして読み込んで再生
				PrepareAudio(iCurX3DAudioResID, hCurX3DAudioModuleHandle, isLoop);

			}
			else if ( szCurX3DAudioFileName.size() ) {

				// 新たな音源なのでエミッタ－の再設定
				g_audioState.vEmitterPos = vEmitterPos;  // エミッターの位置の設定
				g_audioState.vEmitterVel = vEmitterVel;  // エミッターの位置の設定

				// WAVファイルとして読み込んで再生
				PrepareAudio((char *)szCurX3DAudioFileName.c_str(), isLoop);
			}

			// 渡したら無くす。
			szPushedX3DAudioFileName = "";
			iPushedX3DAudioResID = 0;
			hPushedX3DAudioModuleHandle = NULL;
			isLoop = false;

			// コピーしたので外からの上書きを再度認める。
			// ★必ずこの一番下の位置でなければならない。なぜなら、X3DAudioThreadDataの値をコピーしている最中に外部から書き換えられないようにするため。
			isPushed = false;

			UpdateAudio(0);
		}

		// まずは、時差1で再生状況をアップデート
		Sleep(50);
		UpdateAudio(50);
		g_audioState.vEmitterPos = g_audioState.vEmitterPos + g_audioState.vEmitterVel / 1000 * 50;  // エミッターの位置を次回に備えて更新
	}

	_endthreadex(0);
	return TRUE;
}

HANDLE hX3DAudioThread = NULL;
void ConstructX3DAudioThread() {
	// ここに来る時には、すでにn6pwav系モジュールは読み込み済

	// 3Dサウンドの初期化
	InitAudio();

	unsigned thID = 0;
	hX3DAudioThread = (HANDLE)_beginthreadex(NULL, 0, X3DAudioThreadFunc, NULL, 0, &thID);
}

void DestructX3DAudioThread() {
	X3DAudioThreadData::isMustThreadClose = true;
	WaitForSingleObject(hX3DAudioThread, INFINITE); /* スレッドが終了するまで待つ。 */ 
	CloseHandle(hX3DAudioThread);
	CleanupAudio();
}


// 3D音の再生を要求する。TRUEが返ってくれば、受け付けた。FALSEが返ってくれば受け付けなかった。
BOOL X3DPlaySound( int iResID, HMODULE hResource, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel ) {
	// X3Dエンジン側がちゃんと受信しきった後でないと受け付けない。
	if ( X3DAudioThreadData::isPushed == false ) {

		// リソースＩＤの代入
		X3DAudioThreadData::szPushedX3DAudioFileName = "";		// ファイル名直接版
		X3DAudioThreadData::iPushedX3DAudioResID = iResID;
		X3DAudioThreadData::hPushedX3DAudioModuleHandle = hResource;
		X3DAudioThreadData::isLoop = isLoop;
		X3DAudioThreadData::vEmitterPos = vEmitterPos;
		X3DAudioThreadData::vEmitterVel = vEmitterVel;


		// プッシュしたのでレシーブせよという情報を送る
		X3DAudioThreadData::isPushed = true;

		return true;
	}
	return false;
}

// 3D音の再生を要求する。TRUEが返ってくれば、受け付けた。FALSEが返ってくれば受け付けなかった。
BOOL X3DPlaySound( string wavefilename, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel ) {
	// X3Dエンジン側がちゃんと受信しきった後でないと受け付けない。
	if ( X3DAudioThreadData::isPushed == false ) {

		// リソースＩＤの代入
		X3DAudioThreadData::szPushedX3DAudioFileName = wavefilename;		// ファイル名直接版
		X3DAudioThreadData::iPushedX3DAudioResID = 0;
		X3DAudioThreadData::hPushedX3DAudioModuleHandle = NULL;
		X3DAudioThreadData::isLoop = isLoop;
		X3DAudioThreadData::vEmitterPos = vEmitterPos;
		X3DAudioThreadData::vEmitterVel = vEmitterVel;


		// プッシュしたのでレシーブせよという情報を送る
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