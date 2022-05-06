//-----------------------------------------------------------------------------
// File: WaveFile.h
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#pragma pack(1)

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <d3dx9.h>
#include <dxerr.h>

//-----------------------------------------------------------------------------
// Typing macros 
//-----------------------------------------------------------------------------
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return TRUE; } }
#endif

// #define DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (DWORD)__LINE__, hr, str, FALSE )

//-----------------------------------------------------------------------------
// Name: class CWaveFile
// Desc: Encapsulates reading or writing sound data to or from a wave file
//-----------------------------------------------------------------------------
class CWaveFile
{
public:
	WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
	HMMIO m_hmmio;       // MM I/O handle for the WAVE
	MMCKINFO m_ck;          // Multimedia RIFF chunk
	MMCKINFO m_ckRiff;      // Use in opening a WAVE file
	DWORD m_dwSize;      // The size of the wave file
	MMIOINFO m_mmioinfoOut;
	DWORD m_dwFlags;
	BOOL m_bIsReadingFromMemory;
	BYTE* m_pbData;
	BYTE* m_pbDataCur;
	ULONG m_ulDataSize;
	CHAR* m_pResourceBuffer;

protected:
	HRESULT ReadMMIO();
	HRESULT WriteMMIO( WAVEFORMATEX* pwfxDest );

public:
	CWaveFile();
	~CWaveFile();

	HRESULT Open( char *strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT Open( int iResID, WAVEFORMATEX* pwfx, DWORD dwFlags, HMODULE hModule );
	HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT Close();

	HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
	HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );

	DWORD   GetSize();
	HRESULT ResetFile();
	WAVEFORMATEX* GetFormat()
	{
		return m_pwfx;
	};
};

