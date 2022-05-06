// FileName		:	CXAUDIO2.hpp
// Contents		:	XAudio2 クラス(ヘッダー部)
// UpDate		:	11/04/06(Wed)	
// LastUpDate	:	11/04/06(Wed)

#pragma once

#pragma pack(1)

#include "CWAVE.h"
#include "WinTarget.h"
#include <X3DAudio.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <d3dx9.h>


class CXAUDIO2
{
	private:
		static IXAudio2					*m_pXAudio2;	// XAudio2インターフェース
		static IXAudio2MasteringVoice	*m_pMVoice;		// マスターヴォイス

		IXAudio2SourceVoice		*m_pSVoice;		// ソースヴォイス
		WAVEFORMATEX			m_WaveFmt;		// WAVE フォーマット構造体
		CWAVE					m_WaveSound;	// WAVE 音楽データ

	private:
		CXAUDIO2();
		CXAUDIO2(const CXAUDIO2&);
		CXAUDIO2& operator=(const CXAUDIO2&);

		void Exit();			// 終了
		bool Submit();			// 音楽データをキューに送信
		bool CreateXAudio2();	// XAudio2 インターフェースの作成
		bool CreateMVoice();	// マスターヴォイスの作成
		bool CreateSVoice();	// ソースヴォイスの作成

	public:
		~CXAUDIO2();

		static CXAUDIO2* GetInst();	// インスタンスの生成

		IXAudio2SourceVoice* GetSVoice(); // ソースボイスへのポインタを返す
		IXAudio2MasteringVoice* GetMVoice(); // マスターボイスへのポインタを返す
		const WAVEFORMATEX& GetSVoiceFormat() const; // WaveFormatを返す。
		void GetDeviceDetails( XAUDIO2_DEVICE_DETAILS* deviceDatails); // デバイスの詳細を得る

		bool	LoadWaveFile(const std::string &FineName);	// 音楽データのロード
		bool	LoadWaveFile(int iResID, HMODULE hModule);  // 音楽データのロード(DLLに入ってるリソース内WAV用)
		bool	Init();		// 初期化
		void	Play();		// 再生
		void	Stop();		// 停止
		void	Pause();	// 一時停止

		bool	GetState();	// 状態を得る

		void	SetVolume(float Volume);	// ボリュームの設定
		float	GetVolume()const;			// ボリュームを取得

};

/*
	CXAudio2 クラスのヘッダ部分です。
	CWAVE m_WaveSound は WAVE クラスです。
	ロード関数にファイル名を渡すと WAVE ファイルを読み込んだりしてくれますが、
	WAVE クラスとして別記事に分けようと思っていますので、今は気にしなくてもOKです。


	この XAudio2 クラスですが、
	二つ以上インスタンスを必要とするケースが解らなかったのでシングルトンで実装してます。
	なので、どこかで CXAUDIO2::GetInst を呼び、インスタンスを生成する必要があります。

	インスタンスの生成はこんな感じです。

	CXAUDIO2 *m_pCXAudio2 = CXAUDIO2::GetInst();

	これで XAudio2 クラスのインスタンスを生成できます。
*/