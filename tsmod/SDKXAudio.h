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
	D3DXVECTOR3 vEmitterVel; // エミッターの移動ベクトル
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
3D シグナル処理のために低レベルのオーディオ レンダリング関数に送られる X3DAudioCalculate の呼び出し結果を受け取ります。
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

メンバ
	pMatrixCoefficients
	各デスティネーション チャンネルにある各ソース チャンネルのボリューム レベルで初期化される、呼び出し元指定の配列です。この配列には、少なくとも (SrcChannelCount × DstChannelCount) 個の要素が必要です。
	配列には、ソース チャンネルが配列の列インデックスとして、デスティネーション チャンネルが配列の行インデックスとして配置されます。たとえば、2 チャンネル ステレオ入力を、加重が前方チャンネルにあって、中心および低周波数チャンネルにはない 5.1 出力にレンダリングすると、行列は次の表に示すようになります。
	出力 左入力 右入力 
	[左] 1.0 0.0 
	[右] 0.0 1.0 
	前方中央 0.0 0.0 
	LFE 0.0 0.0 
	後方左 0.8 0.0 
	後方右 0.0 0.8 

	左と右のチャンネルが、出力の左と右のチャンネルに完全にマップされています。左と右の入力の 80% が、後方の左と右のチャンネルにマップされています。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_MATRIX フラグを指定していない場合、pMatrixCoefficients メンバーを NULL にできます。
	pDelayTimes
	各デスティネーション チャンネルの遅延を受け取る、呼び出し元指定の遅延時間配列 (ミリ秒単位) です。この配列には、少なくとも DstChannelCount 個の要素が必要です。X3DAudio は、実際には遅延を実行しません。エフェクト チェーンに配置された遅延 DSP エフェクトの調整に使用できる係数を返すだけです。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_DELAY フラグを指定していない場合、pDelayTimes メンバーを NULL にできます。
	注    このメンバーは、X3DAudio がステレオ出力用に初期化される場合にのみ返されます。Xbox 360 の一般的な使用では、データをまったく返しません。
	SrcChannelCount
	ソース チャンネルの数です。X3DAudioCalculate を呼び出す前にエミッタ チャンネルの数に初期化する必要があります。
	DstChannelCount
	デスティネーション チャンネルの数です。X3DAudioCalculate を呼び出す前に最終ミックス チャンネルの数に初期化する必要があります。
	LPFDirectCoefficient
	LPF ダイレクト パス係数です。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_LPF_DIRECT フラグが指定される場合のみ、計算されます。
	LPFReverbCoefficient
	LPF リバーブ パス係数です。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_LPF_REVERB フラグが指定される場合のみ、計算されます。
	ReverbLevel
	リバーブ センド レベルです。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_REVERB フラグが指定される場合のみ、計算されます。
	DopplerFactor
	ドップラー偏移係数。ドップラー偏移効果を考慮して再サンプリング比をスケーリングします。
	コピー
	 effective_frequency = DopplerFactor × original_frequency
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_DOPPLER フラグが指定される場合のみ、計算されます。
	EmitterToListenerAngle
	エミッタ-リスナーの内角。エミッタの前方向に対するラジアン単位で表されます。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_EMITTER_ANGLE フラグが指定される場合のみ、計算されます。
	EmitterToListenerDistance
	リスナーからエミッタ ベース位置までのユーザー定義のワールド単位の距離です。
	EmitterVelocityComponent
	ユーザー定義のワールド単位/秒で、エミッタ-リスナー ベクトルに射影されたエミッタ ベロシティ ベクトルの要素です。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_DOPPLER フラグが指定される場合のみ、計算されます。
	ListenerVelocityComponent
	ユーザー定義のワールド単位/秒で、エミッタ-リスナー ベクトルに射影されたリスナー ベロシティ ベクトルの要素です。
	X3DAudioCalculate の呼び出し時に X3DAUDIO_CALCULATE_DOPPLER フラグが指定される場合のみ、計算されます。
	解説　
	この構造体を X3DAudioCalculate 関数に渡す前に、次のメンバーを初期化する必要があります。

	・pMatrixCoefficients
	・pDelayTimes
	・SrcChannelCount
	・DstChannelCount
	この構造体を X3DAudioCalculate 関数に渡すと、次のメンバーが返されます。

	・pMatrixCoefficients
	・pDelayTimes
	・LPFDirectCoefficient
	・LPFReverbCoefficient
	・ReverbLevel
	・DopplerFactor
	・EmitterToListenerAngle
	・EmitterToListenerDistance
	・EmitterVelocityComponent
	・ListenerVelocityComponent
	注    pMatrixCoefficients と pDelayTimes の場合、X3DAudioCalculate は追加のメモリーを割り当てません。X3DAudioCalculate は、これらのポインターに割り当てられたメモリー位置で、値を変更するだけです。

	要件
	ヘッダー: X3daudio.h 宣言
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

	メンバ
	OrientFront
	前方向の向きです。pCone が NULL の場合、OrientFront は行列と遅延の計算のみに使用されます。pCone が NULL ではない場合、OrientFront は行列、LPF (ダイレクト パスおよびリバーブ パスの両方)、およびリバーブの計算に使用されます。この値は OrientTop と正規直交でなければなりません。
	OrientTop
	上方向の向きです。行列と遅延の計算にのみ使用されます。この値は OrientFront と正規直交でなければなりません。
	Position
	ユーザー定義のワールド単位の位置です。この値は Velocity には影響しません。
	Velocity
	ユーザー定義のワールド単位/秒のベロシティ ベクトルです。ドップラー効果の計算にのみ使用されます。この値は Position には影響しません。
	pCone
	このリスナーの X3DAUDIO_CONE 構造体へのポインターです。リスナー コーンを指定すると、個々の音源のボリュームおよびフィルター DSP パラメーターを決定する際に追加の計算が行われます。pCone の NULL 値は、全方向性サウンドを指定し、コーン処理は適用されません。pCone は行列、LPF (ダイレクト パスおよびリバーブ パスの両方)、およびリバーブの計算にのみ使用されます。
	解説　
	パラメーター型 X3DAUDIO_VECTOR は D3DVECTOR に型変換され、浮動小数点値 x、y、z を提供します。

	リスナーの前方ベクトルと上方ベクトルは、正規直交でなければなりません。正規直交と見なされるには、ベクトルのペアの大きさを 1 +- 1x10-5、内積を 0 +- 1x10-5 にする必要があります。

	要件
	ヘッダー: X3daudio.h 宣言

*/


/*[[X3DAUDIO_EMITTER]]

任意の数のサウンド チャンネルで使用されるシングルポイントまたはマルチポイントの 3D オーディオ ソースを定義します。

コピー
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

	メンバ
	pCone
	サウンド コーンへのポインターです。単一チャンネルのエミッタで、行列、LPF (ダイレクト パスおよびリバーブ パスの両方)、およびリバーブの計算のみに使用されます。NULL は全方向性を指定します。
	OrientFront
	前方向の向きです。この値は OrientTop と正規直交でなければなりません。OrientFront は使用時に正規化されます。コーンを使用しない単一チャンネルのエミッタでは、OrientFront はエミッタの角度計算のみに使用されます。コーンを使用するマルチチャンネル エミッタまたは単一チャンネル エミッタでは、OrientFront は行列、LPF (ダイレクト パスおよびリバーブ パスの両方)、およびリバーブの計算に使用されます。
	OrientTop
	上方向の向きです。この値は OrientFront と正規直交でなければなりません。OrientTop はマルチチャンネル エミッタでの行列の計算にのみ使用されます。
	Position
	ユーザー定義のワールド単位の位置です。この値は Velocity には影響しません。
	Velocity
	ユーザー定義のワールド単位/秒のベロシティ ベクトルです。この値はドップラー効果の計算にのみ使用され、Position には影響しません。
	InnerRadius
	内部半径の計算に使用される値です。InnerRadius が 0 の場合、内部半径は使用されませんが、InnerRadiusAngle は使用される場合があります。0.0f ～ MAX_FLT の値を指定します。
	InnerRadiusAngle
	内部角度の計算に使用される値です。0.0f ～ X3DAUDIO_PI/4.0 の値を指定します。
	ChannelCount
	X3DAUDIO_EMITTER 構造体によって定義されるエミッタの数です。0 より大きい値を指定する必要があります。
	ChannelRadius
	ChannelCount が 1 より大きい場合にエミッタが配置される Position からの距離です。ChannelRadius は、マルチチャンネル エミッタでの行列の計算のみに使用されます。この値は 0.0f 以上であることが必要です。
	pChannelAzimuths
	ラジアン単位の方位角で表したチャンネル位置テーブルです。チャンネル半径と共に使用され、上方ベクトルと直交する平面の前方ベクトルとして示されます。低周波効果 (LFE) チャンネルを指定する場合は、X3DAUDIO_2PI にします。pChannelAzimuths は少なくとも ChannelCount 個の要素が必要です。テーブルの値は 0.0f ～ X3DAUDIO_2PI の範囲内でなければなりません。pChannelAzimuths は、マルチチャンネル エミッタでの行列の計算に使用されます。
	pVolumeCurve
	ボリューム レベル距離カーブです。行列の計算にのみ使用されます。NULL を指定すると、次のような逆二乗則に一致する特化した既定のカーブになります。距離が 0.0f ～ CurveDistanceScaler× 1.0f の範囲内にある場合、減衰は適用されません。
	距離が CurveDistanceScaler× 1.0f より大きい場合、増幅係数は (CurveDistanceScaler× 1.0f)/距離になります。CurveDistanceScaler× 2.0f の距離でサウンドは半分のボリューム (-6 dB) になり、CurveDistanceScaler× 4.0f の距離で 4 分の 1 のボリューム (-12 dB) になります。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	pLFECurve
	LFE ロールオフ距離カーブです。既定のカーブ ([0.0f,CurveDistanceScaler× 1.0f], [CurveDistanceScaler× 1.0f, 0.0f]) を使用する場合は NULL にします。pLFECurve に NULL 値を使用すると、減衰がないようにクランプされた距離 <= CurveDistanceScaler での逆二乗則に一致するデフォルト カーブが指定されます。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	pLPFDirectCurve
	ローパス フィルター (LPF) ダイレクト パス係数距離カーブです。既定のカーブ ([0.0f,0.0f], [1.0f,1.0f]) を使用する場合は NULL にします。pLPFDirectCurve は、LPF ダイレクト パスの計算にのみ使用されます。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	pLPFReverbCurve
	LPF リバーブ パス係数距離カーブです。デフォルトのカーブ ([0.0f,0.25f], [1.0f,0.25f]) を使用する場合は NULL にします。pLPFReverbCurve はローパスフィルタリバーブ パスの計算にのみ使用されます。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	pReverbCurve
	リバーブ センド レベル距離カーブです。デフォルトのカーブ ([0.0f, 1.0f], [1.0f, 0.0f]) を使用する場合は NULL にします。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	CurveDistanceScaler
	正規化された距離カーブをユーザー定義のワールド単位にスケーリングするために、またはその効果を強調するために使用するカーブ距離スケーラです。これは他の計算には影響しません。この値は FLT_MIN ～ FLT_MAX の範囲にする必要があります。CurveDistanceScaler は行列、LPF (ダイレクト パスおよびリバーブ パスの両方)、およびリバーブの計算にのみ使用されます。
	注    RPC カーブが Distance 変数を使用するように定義されている場合は、XACT3DCalculate はこのメンバーを無視します。このような RPC カーブが定義されている場合、このメンバーではなくカーブ内に対応する情報が提供されているものと見なされます。
	DopplerScaler
	ドップラー偏移効果を強調するために使用するドップラー偏移スケーラーです。DopplerScaler はドップラー効果の計算にのみ使用され、その他の計算には影響しません。この値は 0.0f ～ FLT_MAX の範囲内にする必要があります。
	解説　
	X3DAUDIO_EMITTER は、単一点エミッタのコーンのみをサポートします。多点エミッタは、関連する音源のグループを管理するのに便利で効果的な方法です。すべてのチャンネル ポイントで、ドップラー効果などの多くのプロパティが共有されます。同じドップラー偏移がエミッタのすべてのチャンネルに適用されます。このため、ドップラー効果の値の計算が必要になるのは 1 回だけです。複数の独立した単一点エミッタが必要になった場合でも、点ごとの計算は必要になりません。これは、X3DAUDIO_EMITTER に 1 方向のベクトルしかないためで、多点エミッタのコーンは、すべてのチャンネルが同じ方向に向いているように強制されるため、用途が限定されます。複数の独立したコーンが必要な場合は、複数の単一点エミッタを、特定の方向ごとに使用する必要があります。 

	パラメーター型 X3DAUDIO_VECTOR は D3DVECTOR に型変換され、浮動小数点値 x、y、z を提供します。

	X3DAudio では、x 軸の値が左から右に向かって、y 軸の値が下から上へ向かって、z 軸の値が近くから遠くに向かって大きくなる左手デカルト座標系を使用します。方位角は、指定した基準方向から時計回りに測定されます。

	ユーザー定義の距離カーブでは、最初のポイントの距離フィールドが 0.0f で、最後のポイントの距離フィールドが 1.0f でなければなりません。

	エミッタが CurveDistanceScaler× 1.0f を超える距離を移動する場合は、カーブの最後のポイントを使用してボリューム出力レベルを計算します。最後のポイントは次のように求められます。

	コピー
	 X3DAUDIO_DISTANCE_CURVE.pPoints[PointCount-1].DSPSetting)
	内部半径と内部半径角度
	InnerRadius には、サウンドがリスナーを直接通り抜けるとき、またはリスナーの上下を通り抜けるときにサウンドがスムーズに移動する、音源の周囲の領域を指定します。InnerRadiusAngle を指定すると、仰角も考慮されます。仰角を上げたり下げたりすれば、2 台のスピーカーから出ているとは思えないようなサウンドが得られます。

	内部半径と内部半径角度を使用しない場合、2 台の近接したスピーカー内のエミッタは、それらの現在の位置と方向に聞こえる状態になります (または、エミッタが 1 台のスピーカーの定義された角度を持つライン上にある場合は、そのスピーカーのみから聞こえます)。

	内部半径と内部半径角度は、指定するコーンの外側に置かれたエミッタに対して、影響しません。コーンの内側で、エミッタがリスナーと同じ位置にある (またはリスナーの真上か真下にある) 場合、内部半径と内部半径角度により、サウンドがすべてのスピーカーで等しく聞こえるようになるまで徐々に反対側のスピーカーからサウンドが発せられます。

	 
	★画像解説★
	http://msdn.microsoft.com/ja-jp/library/bb174736(v=vs.85).aspx

	要件
	ヘッダー: X3daudio.h 宣言

	関連項目
	X3DAudio の構造体

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

// ３Ｄ再生専用のスレッド
unsigned __stdcall X3DAudioThreadFunc(void *);
// トップレベルから呼ばれる３Ｄサウンド系の初期化
void ConstructX3DAudioThread();
// トップレベルから呼ばれる３Ｄサウンド系の解放化
void DestructX3DAudioThread();



// 3D音の再生を要求する。TRUEが返ってくれば、受け付けた。FALSEが返ってくれば受け付けなかった。
// 例)
//  X3DPlaySound( 601, CommonGlobalHandle::hResourceWavMod, true, D3DXVECTOR3(9.0f, 0, -5.0f), D3DXVECTOR3(-18.0f, 0, 0.0f) );
BOOL X3DPlaySound( int iResID, HMODULE hResource, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel );

// 3D音の再生を要求する。TRUEが返ってくれば、受け付けた。FALSEが返ってくれば受け付けなかった。
// 例)
//	X3DPlaySound( "itsuka.wav", true,  D3DXVECTOR3(0.0f, 0, 0.0f), D3DXVECTOR3(0.0f, 0, 0.0f) );
BOOL X3DPlaySound( string wavefilename, bool isLoop, D3DXVECTOR3 vEmitterPos, D3DXVECTOR3 vEmitterVel );


// 3D音の再生を停止する。リソース版
BOOL X3DPlaySound( int iResID, HMODULE hResource );

// 3D音の再生を停止する。ファイル名版
BOOL X3DPlaySound( string wavefilename );