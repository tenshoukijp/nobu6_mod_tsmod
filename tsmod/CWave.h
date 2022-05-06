// FileName		:	CWAVE.hpp
// Contents		:	WAVE ファイルクラス
// UpDate		:	11/04/08(Fri)	
// LastUpDate	:	11/04/08(Fri)

#pragma once

#pragma pack(1)

#include "xaudio2.h"
#include <vector>
#include <string>

class CWAVE
{
	private:
		HMMIO				m_hMmio;		// MMIO ハンドル
		MMIOINFO			m_MmioInfo;		// MMIO 情報構造体
		MMCKINFO			m_RIFFChunk;	// RIFF チャンク
		MMCKINFO			m_FMTChunk;		// FMT チャンク
		MMCKINFO			m_DATAChunk;	// DATA チャンク
		WAVEFORMATEX		m_WaveFmtEx;	// WAVE フォーマット構造体	
		std::vector<BYTE>	m_WaveData;		// 音楽データ	
		std::string			m_FileName;		// ファイル名

	private:
		bool Open(const std::string &FileName);		// WAVE ファイルオープン
		bool Open(int iResID, HMODULE hModule);		// WAVE ファイルオープン(DLLのリソースに入ってる用)
		bool Close();								// WAVE ファイルクローズ
		bool ReadToWaveFmtEx();						// WAVEFORMATEX にデータを読み込む
		bool ReadToWaveData();						// m_WaveData に WAVE データを読み込む
		bool CheckWAVEHeader();						// WAVE ヘッダがあるかチェック
		bool CheckFMTChunk();						// FMT チャンクがあるかチェック
		bool CheckDATAChunk();						// DATA チャンクがあるかチェック
		
	public:
		CWAVE();
		~CWAVE();

		bool Load(const std::string &FileName);			// 音楽データのロード
		bool Load(int iResID, HMODULE hModule);			// 音楽データのロード (DLL内のリソースのWAV用)

		const WAVEFORMATEX&		GetWaveFmtEx()const;	// WAVE フォーマットを取得
		const std::string&		GetFileName()const;		// ファイル名の取得
		const BYTE*				GetWaveData()const;		// Wave データを取得
		const size_t			GetWaveSize()const;		// Wave データのサイズを取得
};
