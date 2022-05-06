#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Gundan {
		enum { 
			ŒR’c–¼=1,
			ŒR’c’·=1<<1,
			‹é=1<<2,
			Š—Ì=1<<3,
			•«=1<<4,
			•ºŽm=1<<5,
			‹R”n=1<<6,
			‹R•º=1<<7,
			“S–C=1<<8,
			–C•º=1<<9,
			‹à=1<<10,
			‹àŽûŽx=1<<11,
			‹àŽû“ü=1<<12,
			‹àŽxo=1<<13,
			•Ä=1<<14,
			•ÄŽûŽx=1<<15,
			•ÄŽû“ü=1<<16,
			•ÄŽxo=1<<17,
			s“®=1<<18,
			’©’ìŠ¯ˆÊ=1<<19,
			–‹•{–ðE=1<<20,
			ŽwŽ¦“à—e=1<<21,
			•ûj=1<<22,
			U—ª–Ú•W=1<<23,
			“à­‹’“_=1<<24,
			ŒRŽ–‹’“_=1<<25,

			ƒfƒtƒHƒ‹ƒg=0xCD7F
		};
	}
}

// ‘ÎÛ‚Æ‚È‚éŒR’cƒŠƒXƒgBÅŒã‚ÌƒIƒvƒVƒ‡ƒ“‚Í‚P‚Â‚à‘I‘ð‚Å‚«‚È‚¢ƒIƒvƒVƒ‡ƒ“BÅ‘å”‚Í‚PŒÅ’è
int FuncSelectGundanDialog(vector<int> list, int iFilterOption=Filter::Gundan::ƒfƒtƒHƒ‹ƒg );