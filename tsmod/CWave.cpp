// FileName		:	CWAVE.cpp
// Contents		:	WAVE ファイルクラス(実装部)
// UpDate		:	11/04/08(Fri)	
// LastUpDate	:	11/04/08(Fri)


#include "XAudioDbg.h"
#include "CWave.h"

//*******************************************
// コンストラクタ
//*******************************************
CWAVE::CWAVE() : m_hMmio(0)
{
	ZeroMemory(&m_MmioInfo, sizeof(m_MmioInfo));
	ZeroMemory(&m_WaveFmtEx, sizeof(m_WaveFmtEx));
	m_WaveData.clear();
}

//*******************************************
// デストラクタ
//*******************************************
CWAVE::~CWAVE()
{
	this->Close();
}


//*******************************************
// WAVE ファイルオープン
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::Open(const std::string &FileName)
{
	m_FileName = FileName;

	// ファイル名のコピー(mmioOpen に渡す)
	std::vector<char> wFileName(FileName.begin(), FileName.end());
	wFileName.push_back('\0');

	char *str =  const_cast<char*>(FileName.c_str());

	ZeroMemory(&m_MmioInfo, sizeof(m_MmioInfo));

	if( !(m_hMmio = mmioOpen(&wFileName[0], &m_MmioInfo, MMIO_READ)) )
	{
		XAUDIO2_OUTDBG("File[ %s ]を開く事が出来ませんでした。\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}

//*******************************************
// WAVE ファイルをロード
// 引数		：	リソースＩＤ、モジュールハンドラ
// 戻り値	：	TRUE:成功	FALSE:読み込み
//*******************************************
bool CWAVE::Open(int ResID, HMODULE hModule ) {
	LPBYTE lpTargetWavData;

	HRSRC hSrc = FindResourceA(hModule, MAKEINTRESOURCE(ResID), "WAVE");
	if ( !hSrc ) {
		XAUDIO2_OUTDBG("リソース番号%dが見つけられません。\n", ResID );
		return FALSE;
	}
	HGLOBAL hTargetWavData = LoadResource(hModule, hSrc);

	DWORD dwSize = SizeofResource(hModule, hSrc);

	// 読み込んだデータのポインタ取得
	lpTargetWavData = (LPBYTE)LockResource(hTargetWavData);


	// サウンドデータを再生
	// sndPlaySoundA((LPCSTR)lpTargetWavData, SND_SYNC | SND_MEMORY);

	// ここから、リソースタイプをMmioInfoやmmioOpenへと繋げてゆく処理
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
// WAVE ファイルクローズ
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::Close()
{
	if(mmioClose(m_hMmio, 0) == MMIOERR_CANNOTWRITE)
	{
		XAUDIO2_OUTDBG("File[ %s ]を閉じる事が出来ませんでした。\n", m_FileName.c_str() );;
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// WAVE ヘッダがあるかチェック
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::CheckWAVEHeader()
{
	m_RIFFChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if( mmioDescend(m_hMmio, &m_RIFFChunk, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]は WAVE ヘッダが存在しません。\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// FMT チャンクがあるかチェック
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::CheckFMTChunk()
{
	m_FMTChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	
	if( mmioDescend(m_hMmio, &m_FMTChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]は FMT チャンクが存在しません。\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}

//*******************************************
// DAtA チャンクがあるかチェック
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::CheckDATAChunk()
{
	m_DATAChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if( mmioDescend(m_hMmio, &m_DATAChunk, &m_RIFFChunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
	{
		XAUDIO2_OUTDBG("File[ %s ]は DATA チャンクが存在しません。\n", m_FileName.c_str() );
		return false;
	}

	return TRUE;
}

//*******************************************
// WAVEFORMATEX にデータを読み込む
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::ReadToWaveFmtEx()
{
	// mmioRead は成功すると読みこんだサイズを返す
	LONG wReadSize = mmioRead(m_hMmio, reinterpret_cast<HPSTR>(&m_WaveFmtEx), m_FMTChunk.cksize);

	// 0:それ以上読み込めない　-1:ファイルの読み取りに失敗
	switch(wReadSize)
	{
		//-----------------------------------------------------
		case 0:
				XAUDIO2_OUTDBG("File[ %s ]はこれ以上読み込めません。\n", m_FileName.c_str() );
			break;
		//-----------------------------------------------------
		case -1:
				XAUDIO2_OUTDBG("File[ %s ]は読みとれませんでした。\n", m_FileName.c_str() );
			break;
		//-----------------------------------------------------
		default:
			// データサイズが合ってるかチェックする
			if(wReadSize != m_FMTChunk.cksize)
			{
				XAUDIO2_OUTDBG("File[ %s ]の読みこんだデータサイズが異なります。\n", m_FileName.c_str() );
				return FALSE;
			}
	}

	return TRUE;
}


//*******************************************
// m_WaveData に WAVE データを読み込む
// 引数		：	なし
// 戻り値	：	TRUE:成功	FALSE:失敗
//*******************************************
bool CWAVE::ReadToWaveData()
{
	// WAVE データ用にリサイズ
	m_WaveData.resize(m_DATAChunk.cksize);

	if(mmioRead(m_hMmio, reinterpret_cast<HPSTR>(&m_WaveData[0]), m_DATAChunk.cksize) != m_DATAChunk.cksize)
	{
		XAUDIO2_OUTDBG("File[ %s ]の読みこんだデータサイズが異なります。\n", m_FileName.c_str() );
		return FALSE;
	}

	return TRUE;
}


//*******************************************
// WAVE ファイルをロード
// 引数		：	ファイル名
// 戻り値	：	TRUE:成功	FALSE:読み込み
//*******************************************
bool CWAVE::Load(const std::string &FileName)
{
	// ファイルオープン
	if(!this->Open(FileName.c_str()))
		return FALSE;

	// WAVE ヘッダのチェック
	if(!this->CheckWAVEHeader())
		return FALSE;

	// FMT チャンクのチェック
	if(!this->CheckFMTChunk())
		return FALSE;

	// WAVEFORMATEX 構造体にデータを読み込む
	if(!this->ReadToWaveFmtEx())
		return FALSE;

	// 読み込み位置を先頭に戻す
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA チャンクのチェック
	if(!this->CheckDATAChunk())
		return FALSE;

	// WAVE データを読み込む
	if(!this->ReadToWaveData())
		return FALSE;

	// ファイルを閉じる
	this->Close();

	return TRUE;

}


//*******************************************
// WAVE ファイルをロード
// 引数		：	リソースＩＤ、モジュールハンドラ
// 戻り値	：	TRUE:成功	FALSE:読み込み
//*******************************************
bool CWAVE::Load(int iResID, HMODULE hModule)
{
	// ファイルオープン
	if(!this->Open(iResID, hModule))
		return FALSE;

	// WAVE ヘッダのチェック
	if(!this->CheckWAVEHeader())
		return FALSE;

	// FMT チャンクのチェック
	if(!this->CheckFMTChunk())
		return FALSE;

	// WAVEFORMATEX 構造体にデータを読み込む
	if(!this->ReadToWaveFmtEx())
		return FALSE;

	// 読み込み位置を先頭に戻す
	mmioAscend(m_hMmio, &m_FMTChunk, SEEK_SET);

	// DATA チャンクのチェック
	if(!this->CheckDATAChunk())
		return FALSE;

	// WAVE データを読み込む
	if(!this->ReadToWaveData())
		return FALSE;

	// ファイルを閉じる
	this->Close();

	return TRUE;

}





//*******************************************
// ファイル名を取得
// 引数		：	なし
// 戻り値	：	WAVE フォーマット構造体への参照
//*******************************************
const std::string& CWAVE::GetFileName()const
{
	return m_FileName;
}


//*******************************************
// WAVE フォーマットを取得
// 引数		：	なし
// 戻り値	：	WAVE フォーマット構造体への参照
//*******************************************
const WAVEFORMATEX& CWAVE::GetWaveFmtEx()const
{
	return m_WaveFmtEx;
}

//*******************************************
// WAVE データを取得
// 引数		：	なし
// 戻り値	：	WAVE フォーマット構造体への参照
//*******************************************
const BYTE* CWAVE::GetWaveData()const
{
	return &m_WaveData[0];
}

//*******************************************
// WAVE データのサイズを取得
// 引数		：	なし
// 戻り値	：	WAVE フォーマット構造体への参照
//*******************************************
const size_t CWAVE::GetWaveSize()const
{
	return m_WaveData.size();
}
