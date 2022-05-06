// 6wav.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
// IXAudio2VoiceCallbackの実装
// http://msdn.microsoft.com/ja-jp/library/microsoft.directx_sdk.ixaudio2voicecallback.ixaudio2voicecallback(v=VS.85).aspx
// "通常は"これを用いてストリーミング再生をします
// pBufferContextとはXAUDIO2_BUFFERに渡せるユーザデータを指す
// pBufferContextが引数のメソッドはキューデータが存在してないと動かない(でないと困る)
// ----------------------
struct voice_callback : public ::IXAudio2VoiceCallback
{
	// ボイスがバッファキューデータを読み取る直前にプロセスで呼び出し
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart( ::UINT32 BytesRequired )
	{
		std::printf("Processing pass start.\n");
	}

	// ボイスがプロセス終了時に呼び出し
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd()
	{
		std::printf("Processing pass end.\n");
	}

	// オーディオストリームが終了するときに呼び出し
	void STDMETHODCALLTYPE OnStreamEnd()
	{
		std::printf("Stream end.\n");
	}

	// バッファキューデータの開始時呼び出し
	void STDMETHODCALLTYPE OnBufferStart( void* pBufferContext )
	{
		std::printf("Buffer start.\n");
	}

	// バッファキューデータの終了直後に呼び出し
	void STDMETHODCALLTYPE OnBufferEnd( void* pBufferContext )
	{
		std::printf("Buffer end.\n");
	}

	// ループ再生終了時に呼び出し
	void STDMETHODCALLTYPE OnLoopEnd( void* pBufferContext )
	{
		std::printf("Loop end.\n");
	}

	// ボイスにエラーがあると呼び出し
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
// エミッタの向きに対して、単一チャンネルの非 LFE エミッタの方向を DSP 動作のスケーリングによって指定します。
/*
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


	// 困ったことにIXAudio2VoiceCallbackは仮想デストラクタを持たない
	// よってボイスが削除されるまで持っている必要がある
	// 例えば、デリータに持たせる
	struct voice_deleter_has_callback
	{
		voice_deleter_has_callback()
			: Callback_( new voice_callback() )
		{}

		void operator()( ::IXAudio2SourceVoice* voice ) const
		{
			voice->DestroyVoice();
		}

		// shared_ptrに持たせるのは結構よろしくない(コスト見合わない)
		// しかしデリータを渡すときにコピーになるのでそれを回避する
		boost::shared_ptr<voice_callback> Callback_;
	} deleter;




	// シングルトン
	CXAUDIO2 *pXAudio2 = CXAUDIO2::GetInst();
	// 初期化
	pXAudio2->Init();

	// pXAudio2->LoadWaveFile("601.wav");
	pXAudio2->LoadWaveFile(601, hModule);

	IXAudio2SourceVoice* pSVoice = pXAudio2->GetSVoice();
	IXAudio2MasteringVoice* pMVoice = pXAudio2->GetMVoice();

	WAVEFORMATEX pSVoiceFormat = pXAudio2->GetSVoiceFormat();
	int nInputChannnel = pSVoiceFormat.nChannels;
	XAUDIO2_OUTDBG("%d", nInputChannnel);

	// マスター ボイス チャンネル数に一致するスピーカー チャンネル構成を持つ X3DAudio ライブラリを初期化します。
	// 3.空間オーディオの計算を実行し、フレーム更新時に更新します。 
	// X3DAudioInitialize に突っ込むためのデータ設定
	XAUDIO2_DEVICE_DETAILS deviceDetails;
	pXAudio2->GetDeviceDetails( &deviceDetails );
	DWORD channelMask = deviceDetails.OutputFormat.dwChannelMask;
	int m_numChannels = deviceDetails.OutputFormat.Format.nChannels;
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAUDIO_HANDLE x3DInstance;
	::memset( x3DInstance, 0, sizeof( x3DInstance ) );

	X3DAudioInitialize( channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DInstance );


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

	X3DAudioCalculate関数は、リバーブの効果なども計算可能なので
	それもいつか実装しようと思います。
	*/

	// 宣言開始
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
	// 宣言ここまで

    vListenerPos = D3DXVECTOR3( 0, 0, 0 );	 // ユーザーの位置
    vEmitterPos = D3DXVECTOR3( 7, 0, -7 ); // x, y, z (z=float(ZMAX) エミッターの位置
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
	5.空間オーディオを示すために、簡略化された X/Z グリッド上にある X3DAudio のエミッター オブジェクトとリスナー オブジェクトを表示して、制御します。 
	1.各レンダリング フレームで、エミッターとリスナーの現在の位置を描画します。
	2.パラメーター制御の変更を適用して、各フレームで UI 要素を更新します。
	3.空間オーディオの計算を実行し、フレーム更新時に更新します。 
	■フレームの経過時間に基づき、新しい位置と速度の情報を使用して、エミッター オブジェクトとリスナー オブジェクトを更新します。
	■X3DAudioCalculate メソッドを呼び出して、結果の DSP コントロールの設定を計算します。
	■DSP の設定を、IXAudio2Voice::SetOutputMatrix 関数と IXAudio2SourceVoice::SetFrequencyRatio 関数を経由してソース ボイスに渡します。
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


/*
[[X3DAUDIO_DISTANCE_CURVE_POINT]]
正規化された一定の距離で DSP 設定を定義します。

コピー
 typedef struct X3DAUDIO_DISTANCE_CURVE_POINT {
    FLOAT32 Distance;
    FLOAT32 DSPSetting;
} X3DAUDIO_DISTANCE_CURVE_POINT, *LPX3DAUDIO_DISTANCE_CURVE_POINT;

メンバ
	Distance
	正規化された距離です。この値は 0.0f ～ 1.0f の範囲内でなければなりません。
	DSPSetting
	DSP コントロールの設定です。
	要件
	ヘッダー: X3daudio.h 宣言
*/


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