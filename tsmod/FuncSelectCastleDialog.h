#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Castle {
		enum { 
			�閼=1,
			��=1<<1,
			�喼=1<<2,
			�R�c=1<<3,
			���=1<<4,
			����=1<<5,
			�s����=1<<6,
			�A��=1<<7,
			�΍�=1<<8,
			�Ő�=1<<9,
			����=1<<10,
			�ŏ�=1<<11,
			��s=1<<12,
			�ŏ�=1<<13,
			����=1<<14,
			�l��=1<<15,
			������=1<<16,
			���m=1<<17,
			�R��=1<<18,
			�C��=1<<19,
			������=1<<20,
			�Ď���=1<<21,
			���f=1<<22,
			�O�f=1<<23,
			�n�Y=1<<24,
			�C�Y=1<<25,
			���R=1<<26,

			�f�t�H���g=0x33D57B
		};
	}
}

// �ΏۂƂȂ�郊�X�g�B�Ō�̃I�v�V�����͂P���I���ł��Ȃ��I�v�V�����B�ő吔�͂P�Œ�
int FuncSelectCastleDialog(vector<int> list, int iFilterOption=Filter::Castle::�f�t�H���g );