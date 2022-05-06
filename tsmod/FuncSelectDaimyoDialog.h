#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Daimyo {
		enum { 
			�喼=1,
			�{��=1<<1,
			�F�D=1<<2,
			����=1<<3,
			����=1<<4,
			���G=1<<5,
			�G�Α喼=1<<6,
			�R�c=1<<7,
			����=1<<8,
			����=1<<9,
			���m=1<<10,
			�R�n=1<<11,
			�R��=1<<12,
			�S�C=1<<13,
			�C��=1<<14,
			��=1<<15,
			�����x=1<<16,
			������=1<<17,
			���x�o=1<<18,
			��=1<<19,
			�Ď��x=1<<20,
			�Ď���=1<<21,
			�Ďx�o=1<<22,
			�z��=1<<23,
			���l=1<<24,

			�f�t�H���g=0x311079F
		};
	}
}


// �ΏۂƂȂ�喼���X�g�B
// iMainDaimyoID�́A�ǂ̑喼���_�̏����o���̂��H �Ƃ������Ƃł���B�ȗ������ꍇ�́A�^�[���喼�̂��̂ƂȂ�B
int FuncSelectDaimyoDialog(vector<int> list, int iFilterOption=Filter::Daimyo::�f�t�H���g, int iMainDaimyoID=-1 );