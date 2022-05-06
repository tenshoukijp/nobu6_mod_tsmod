// FileName		:	CXAUDIO2.cpp
// Contents		:	XAudio2クラス(実装部)
// UpDate		:	11/04/06(Wed)	
// LastUpDate	:	11/04/06(Wed)


#include "WinTarget.h"
#include "CXAudio.h"
#include "XAudioDbg.h"



IXAudio2				*CXAUDIO2::m_pXAudio2	= NULL;
IXAudio2MasteringVoice	*CXAUDIO2::m_pMVoice	= NULL;


//*******************************************
// コンストラクタ
//*******************************************
CXAUDIO2::CXAUDIO2() : m_pSVoice(NULL)
{
	ZeroMemory(&m_WaveFmt, sizeof(m_WaveFmt));
}


//*******************************************
// デストラクタ
//*******************************************
CXAUDIO2::~CXAUDIO2()
{
	this->Exit();
}



/*
	で、初期化ですが、
	パブリックなメンバメソッド CXAUDIO2::Init が呼ばれると XAudio2 の初期化が始まります。
	私は初期化をこのように実装してます。

	初期化の流れとしては
	・COM (Component Object Model)の初期化
	・XAudio2 インターフェースの作成
	・マスターヴォイスの作成
	・WAVE の読み込み
	・ソースヴォイスの作成
	・ソースヴォイスキューにデータを送信

	という感じです。
*/
 
//*******************************************
// 初期化
// 引数		：	なし
// 戻り値	：	TRUE:初期化成功		FALSE:初期化失敗
//*******************************************
bool CXAUDIO2::Init()
{
	// COM の初期化
	if( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
	{
		if( FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) )
		{
			XAUDIO2_OUTDBG("[XAudio2]COMの初期化に失敗しました。\n");
			return FALSE;
		}
	}

	// XAudio2 インターフェースの作成
	if(!this->CreateXAudio2())
		return FALSE;

	// マスターボイスの作成
	if(!this->CreateMVoice())
		return FALSE;

	XAUDIO2_OUTDBG("XAudio2 関連の初期化に成功しました。\n");
	return TRUE;
}

/*
	次は、this->LoadWaveFile メンバメソッドを呼んでいますが
	・WAVE 読み込み
	・ソースヴォイスの作成
	・データ送信

	をやってます。

	WAVE の読み込みには WAVE クラスである
	CWAVE::Load(const std::string &FileName) で読み込むのですが
	今回は WAVE 読み込みは既に出来ているものとして話を進めます。
	コレに関しては Part2 くらいで WAVE クラスを紹介する時にでもお話します。


	余談ですが、CXAUDIO2 クラスのメンバに CWAVE クラスを持たせてますが
	一緒に入れない方がいいと思ってます。
	やるなら WAVE を管理するクラスなんかあってもいいかもしれませんね。
	ま、今回は中に入れときました。
*/
//*******************************************
// 曲データの読み込み
// 引数		：	なし
// 戻り値	：	TRUE:読み込み成功	FALSE:読み込み失敗
//*******************************************
bool CXAUDIO2::LoadWaveFile(const std::string &FileName)
{
	// WAVE クラスを使って音楽ロード
	if(!m_WaveSound.Load(FileName))
		return FALSE;

	// ソースヴォイスの作成
	if(!this->CreateSVoice())
		return FALSE;

	// 読み込んだ音楽データをキューに送信
	this->Submit();

	return TRUE;
}

//*******************************************
// 曲データの読み込み
// 引数		：	なし
// 戻り値	：	TRUE:読み込み成功	FALSE:読み込み失敗
//*******************************************
bool CXAUDIO2::LoadWaveFile(int iResID, HMODULE hModule)
{
	// WAVE クラスを使って音楽ロード
	if(!m_WaveSound.Load(iResID, hModule))
		return FALSE;

	// ソースヴォイスの作成
	if(!this->CreateSVoice())
		return FALSE;

	// 読み込んだ音楽データをキューに送信
	this->Submit();

	return TRUE;
}


/*
	COM の初期化はあれで終わりなので
	次は XAudio2 インターフェースを作成するメンバメソッドです。

	XAudio2Create メソッドを呼ぶだけです。
	第一引数に IXAudio2* オブジェクトのアドレスを渡しています。
	第二引数がポイントで、MSDN によると
*/
//*******************************************
// XAudio2 インターフェースの作成
// 引数		：	なし
// 戻り値	：	TRUE:成功		FALSE:失敗
//*******************************************
bool CXAUDIO2::CreateXAudio2()
{
	UINT wFlag = 0;

#ifndef NDEBUG
	wFlag |= XAUDIO2_DEBUG_ENGINE;
#endif

	if( FAILED( XAudio2Create(&m_pXAudio2, wFlag)) )
	{
		XAUDIO2_OUTDBG("XAudio2 インターフェスの作成に失敗しました。\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("XAudio2 インターフェスの作成に成功しました。\n");
	return TRUE;
}

/*
	IXAudio2::CreateMasteringVoice メソッドでマスターヴォイスを作成します。
	第一引数に IXAudio2MasteringVoice* オブジェクトのアドレスを渡します。
	後に引数があるのですが、デフォルトの値を使用しました。

	IXAudio2::CreateMasteringVoice メソッドでマスターヴォイスを作成します。
	第一引数に IXAudio2MasteringVoice* オブジェクトのアドレスを渡します。
	後に引数があるのですが、デフォルトの値を使用しました。

	MSDN によると
	Several mastering voices can exist at once, but only one of them can be started.
	Only one device can be used by XAudio2 at a time.

	(二つ以上のマスターヴォイスがが同時に存在しても大丈夫で、
	スタートできるのは一度につき一個だけです。
	XAudio2 が使用できるデバイスは一度に一個だけです。)

	らしいので、作っても問題はないそうですが二つ作る意味はないようです。

	また、
	It is illegal to call CreateMasteringVoice from within a callback. 
	If CreateMasteringVoice is called within a callback XAUDIO2_E_INVALID_CALL will be returned.

	(CreateMasteringVoice メソッドは、コールバック関数の中から呼び出すと無効になる。
	もし呼び出した場合、XAUDIO2_E_INVALID_CALL が返されます。)


	というのもあるので コールバック内からのメソッド呼び出しもやっては駄目なようです。
	その辺注意しておけばOKみたいなので便利ですよね～。
*/
//*******************************************
// マスターヴォイスの作成
// 引数		：	なし
// 戻り値	：	TRUE:成功		FALSE:失敗
//*******************************************
bool CXAUDIO2::CreateMVoice()
{
	if( FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMVoice)) )
	{
		XAUDIO2_OUTDBG("マスタリングヴォイスの作成に失敗しました。\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("マスタリングヴォイスの作成に成功しました。\n");

	return TRUE;
}


/*
ソースヴォイスの作成には
IXAudio2::CreateSourceVoice メソッドを使います。
第一引数に IXAudio2SourceVoice* オブジェクトのアドレスを渡します。
第二引数に WAVEFORMATEX 構造体を渡します。
既に WAVE ファイルを読み込んでいる前提なのでココにそれを渡します。


	http://msdn.microsoft.com/ja-jp/library/bb633468(v=vs.85).aspx
	[[IXAudio2::CreateSourceVoice]]
	ソース ボイスを作成し、設定します。

	コピー
	 HRESULT CreateSourceVoice(
	  IXAudio2SourceVoice **ppSourceVoice,
	  const WAVEFORMATEX *pSourceFormat,
	  UINT32 Flags = 0,
	  float MaxFrequencyRatio = XAUDIO2_DEFAULT_FREQ_RATIO,
	  IXAudio2VoiceCallback *pCallback = NULL,
	  const XAUDIO2_VOICE_SENDS *pSendList = NULL,
	  const XAUDIO2_EFFECT_CHAIN *pEffectChain = NULL
	);
	パラメータ
	ppSourceVoice
	成功した場合、新しい IXAudio2SourceVoice オブジェクトへのポインターを返します。
	pSourceFormat
	WAVEFORMATEX 構造体へのポインターです。この構造体には、ソース ボイスに送られるすべてのオーディオ バッファーに期待される形式が含まれます。 
	注    XMA、ADPCM、xWMA など、WAVEFORMATEX によって提供されるよりも多くの情報を必要とするデータ形式には、その形式構造体の最初のメンバーとして WAVEFORMATEX 構造体があります。これらの形式のいずれかでソース ボイスを作成する場合は、形式の構造体を WAVEFORMATEX 構造体としてキャストし、それを pSourceFormat の値として使用します。
	Flags
	ソース ボイスの動作を指定するフラグです。フラグには 0 または次のいずれかの値を指定します。
	値 説明 
	XAUDIO2_VOICE_NOPITCH ボイスに対するピッチ コントロールは使用できません。 
	XAUDIO2_VOICE_NOSRC ボイスに対してサンプル レート変換は実行できません。ボイスの出力はすべて同じ入力サンプル レートである必要があります。
	注    XAUDIO2_VOICE_NOSRC フラグによって、XAUDIO2_VOICE_NOPITCH フラグも指定されているかのようにボイスが動作します。
	 
	XAUDIO2_VOICE_USEFILTER このボイスでは、フィルター エフェクトが使用可能でなければなりません。 
	XAUDIO2_VOICE_MUSIC ボイスは、バックグラウンド ミュージックの再生に使用されます。ボイスは、ユーザーが選択したミュージックで自動的に置き換えられます。 

	MaxFrequencyRatio
	このボイスに設定できる最大許容周波数比です。この引数の値は、XAUDIO2_MIN_FREQ_RATIO から XAUDIO2_MAX_FREQ_RATIO までの間でなければなりません。以降の IXAudio2SourceVoice::SetFrequencyRatio への呼び出しは、XAUDIO2_MIN_FREQ_RATIO と MaxFrequencyRatio の間でクランプされます。
	この引数の最大値は、XAUDIO2_MAX_FREQ_RATIO として定義されます。これにより、最大で 10 オクターブまでピッチを上げることができます。
	MaxFrequencyRatio が 1.0 未満の場合、ボイスは作成後すぐに、既定の 1.0 ではなくその周波数比を使用します。
	Xbox 360:  XMA ボイスの場合は、MaxFrequencyRatio 引数およびボイスのサンプル レートについて、さらに別の制限があります。これらの 2 つの数値の積が、1 チャンネル ボイスの場合は XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO を、それ以外のチャンネル数のボイスの場合は XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL を超えることはできません。MaxFrequencyRatio に指定されている値が、指定の形式には高すぎる場合、CreateSourceVoice への呼び出しは失敗し、デバッグ メッセージが表示されます。

	注    MaxFrequencyRatio で可能な最も低い値を使用することで、XAudio2 のメモリー使用量を減らすことができます。
	pCallback
	クライアントが指定したコールバック インターフェイスへのポインターである IXAudio2VoiceCallback です。
	pSendList
	ソース ボイスに対する一連のデスティネーション ボイスを表す XAUDIO2_VOICE_SENDS 構造体のリストへのポインターです。pSendList が NULL の場合、送信リストは、既定で、最初に作成されたマスター ボイスへの単一の出力となります。
	pEffectChain
	ソース ボイスで使用するエフェクト チェーンを表す XAUDIO2_EFFECT_CHAIN 構造体のリストへのポインターです。
	戻り値
	正常に実行された場合は S_OK を返し、失敗した場合はエラー コードを返します。XAudio2 固有のエラー コードの説明については、「XAudio2 エラー コード」を参照してください。 

	解説　
	ソース ボイスは、クライアントからのオーディオ データを読み取ります。ソース ボイスは、データを処理して、データを XAudio2 処理グラフに送信します。

	ソース ボイスには、データを、ソース フォーマット サンプル レートからボイス送信リストに必要な出力レートに変換するための、可変レート サンプル レート変換も含まれます。NULL 送信リストが使用されている場合は、ターゲットのサンプル レートは、マスタリング ボイスの入力サンプル レートになります。単一のボイスが pSendList で提供されている場合は、そのボイスの入力サンプル レートがターゲット レートになります。複数のボイスが pSendList で提供されている場合は、ソース ボイスの出力ボイスがすべて同じサンプル レートで実行される必要があります。

	マスター ボイスが存在しなければ、ソース ボイスまたはサブミックス ボイスを作成することはできません。また、ソース ボイスまたはサブミックス ボイスが存在している間は、マスター ボイスを破棄することはできません。

	ソース ボイスは、常に、サブミックス ボイスまたはマスター ボイスの前に処理されます。したがって、処理順序を制御する ProcessingStage パラメーターは必要はありません。

	最初に作成したとき、ソース ボイスは停止した状態になっています。

	XAudio2 では、同じ形式のボイスに 1 つの内部メモリー プールを使用するので、多くのボイスが作成された後、破棄される場合に、ボイスのメモリー割り当てが行われる回数が少なくなります。ジャストインタイムの割り当てを最小限に抑えるために、必要なボイスの予想最大数をタイトルで事前に作成してから、必要に応じて削除することができます。その後、ボイスは XAudio2 プールから再利用されます。メモリー プールは XAudio2 エンジンのインスタンスに関連付けられます。XAudio2 エンジンのインスタンスによって使用されるすべてのメモリーは、XAudio2 オブジェクトを破棄し、必要に応じて再作成することで、再利用できます (必要に応じて、事前割り当てによるメモリー プールの拡大を再適用する必要があります)。

	コールバック内からの CreateSourceVoice の呼び出しは無効です。コールバック内から CreateSourceVoice を呼び出すと、XAUDIO2_E_INVALID_CALL が返されます。
	*/

	/*
	[[IXAudio2VoiceCallback]]
	http://msdn.microsoft.com/ja-jp/library/bb694521(v=vs.85).aspx
	このトピックはまだ評価されていません - このトピックを評価する 
	The IXAudio2VoiceCallback インターフェイスには、特定の IXAudio2SourceVoice で、あるイベントが発生したときにクライアントに通知するメソッドが格納されます。このインターフェイスは、XAudio2 のクライアントによって実装される必要があります。XAudio2 は、IXAudio2::CreateSourceVoice メソッド内にクライアントによって提供されるインターフェイス ポインターを介してこれらのメソッドを呼び出します。このインターフェイスのメソッドのクライアント実装は、HRESULT ではなく、void を返します。

	コールバックの実装に関する制限事項については、「XAudio2 のコールバック」のトピックを参照してください。

	メソッド 説明 
	OnBufferEnd ボイスがバッファーの処理を終了したときに呼び出されます。
	 
	OnBufferStart ボイスが新しいオーディオ バッファーの処理を開始しようとするときに呼び出されます。
	 
	OnLoopEnd ボイスがループの最後に到達したときに呼び出されます。
	 
	OnStreamEnd ボイスが連続オーディオ ストリームの再生を終了したときに呼び出されます。
	 
	OnVoiceError ボイスの処理中に重大なエラーが発生したときに呼び出されます。
	 
	OnVoiceProcessingPassEnd ボイス終了のプロセッシング パスの直後に呼び出されます。
	 
	OnVoiceProcessingPassStart ボイス開始のプロセッシング パスの直前に呼び出されます。
	 

	要件
	ヘッダー: XAudio2.h 宣言

	ライブラリ: XAudio2.lib 内容 


これで、音を鳴らす準備が出来たので、Submitでソースヴォイスキューに送信します。
*/
//*******************************************
// ソースヴォイスの作成
// 第一引数	：	ソースヴォイスオブジェクトへのポインタへの参照
// 第二引数	：	WAVEFORMATEX 構造体への参照
// 戻り値	：	TRUE:成功		FALSE:失敗
//*******************************************
bool CXAUDIO2::CreateSVoice()
{
	// ソースヴォイスの破棄
	if(m_pSVoice)
	{
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}

	//																				     ┌倍率
	if( FAILED(m_pXAudio2->CreateSourceVoice(&m_pSVoice, &m_WaveSound.GetWaveFmtEx(),0, 1.0f, /*pCallback*/0 )) )
	{
		XAUDIO2_OUTDBG("ソースヴォイスの作成に失敗しました。\n");
		return FALSE;
	}

	XAUDIO2_OUTDBG("ソースヴォイスの作成に成功しました。\n");
	return TRUE;
}



/*
	ソースヴォイスキューに送信するには
	IXAudio2SourceVoice::SubmitSourceBuffer メソッドを使います。
	第一引数に XAUDIO2_BUFFER 構造体のアドレス を渡します。
	第二引数ですが、今回は使いませんので指定しません。(デフォルトで NULL です)


	さて、
	wSubmit.AudioBytes = WAVE データのサイズ; 
	wSubmit.pAudioData = WAVE データ;

	この二行ですが、
	WAVE データのサイズ は 「dataチャンク」を読み込んだ時に
	WAVE のサイズも一緒に読み込んでいると思います。
	MMCKINFO::cksize に WAVE データのサイズが入れられてます。


	WAVE データ は実際に WAVE の情報を読み込んだオブジェクトです。
	例えば mmioRead で読み込んだ WAVE データをここに指定します。



	ここまでエラーがなければ後は再生・停止するだけです。

	m_pSVoice->Start(0);
	m_pSVoice->Stop(0);

	ソースヴォイスオブジェクトのメンバメソッドである
	再生・停止メソッドを呼べばOKです。


	使い終わったら後処理です。
 */
//*******************************************
// ソースヴォイスキューに音楽データを送信
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CXAUDIO2::Submit()
{
	XAUDIO2_BUFFER wSubmit = { 0 };
	wSubmit.AudioBytes     = m_WaveSound.GetWaveSize(); 
	wSubmit.pAudioData     = m_WaveSound.GetWaveData();
	wSubmit.Flags          = XAUDIO2_END_OF_STREAM;

	if( m_pSVoice->SubmitSourceBuffer(&wSubmit) != S_OK )
	{
		XAUDIO2_OUTDBG("音楽データの送信に失敗しました。\n");	
		return FALSE;
	}

	XAUDIO2_OUTDBG("音楽データの送信に成功しました。\n");	
	return TRUE;	
}



//*******************************************
// 終了
// 引数		：	なし
// 戻り値	：	なし
//*******************************************
void CXAUDIO2::Exit()
{
	// ソースヴォイスの破棄
	if(m_pSVoice)
	{
		m_pSVoice->Stop(0);
		m_pSVoice->DestroyVoice();
		m_pSVoice = NULL;
	}

	// マスタリングヴォイスの破棄
	if(m_pMVoice)
	{
		m_pMVoice->DestroyVoice();
		m_pMVoice = NULL;
	}

	if(m_pXAudio2) { 
		m_pXAudio2->Release();
		m_pXAudio2=NULL;
	}		// XAudio2 インターフェースの破棄

	CoUninitialize();
}



// 再生
void CXAUDIO2::Play()
{
	if(m_pSVoice)
		m_pSVoice->Start(0);
}



// 停止
void CXAUDIO2::Stop()
{
	if(m_pSVoice)
		m_pSVoice->Stop(0);
}


// 一時停止
void CXAUDIO2::Pause()
{
	// 未実装です
}


/*
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


	例えばこんな風に使ってみる。
*/
// ボリュームの設定
void CXAUDIO2::SetVolume(float Volume)
{
	m_pMVoice->SetVolume( Volume );
}



// ボリュームを取得
float CXAUDIO2::GetVolume()const
{
	float p_Volume=0;
	m_pMVoice->GetVolume(&p_Volume);
	return p_Volume;
}

// ステートの取得
bool CXAUDIO2::GetState() {
	XAUDIO2_VOICE_STATE state = {};
	m_pSVoice->GetState(&state);
	
	return state.BuffersQueued > 0;
}

// デバイスの詳細の取得
void CXAUDIO2::GetDeviceDetails( XAUDIO2_DEVICE_DETAILS* deviceDatails) {
	m_pXAudio2->GetDeviceDetails( 0, deviceDatails );
}

// ソースボイスの取得
IXAudio2SourceVoice* CXAUDIO2::GetSVoice() {
	return m_pSVoice;
}

// マスターボイスの取得
IXAudio2MasteringVoice* CXAUDIO2::GetMVoice() { // マスターボイスへのポインタを返す
	return m_pMVoice;
}

// ソースボイスのフォーマットの取得
const WAVEFORMATEX& CXAUDIO2::GetSVoiceFormat() const {
	return m_WaveSound.GetWaveFmtEx();
}


//*******************************************
// インスタンスの生成
// 引数		：	なし
// 戻り値	：	CXAUDIO2 クラスへのポインタ
//*******************************************
CXAUDIO2* CXAUDIO2::GetInst()
{
	static CXAUDIO2 Inst;
	return &Inst;
}