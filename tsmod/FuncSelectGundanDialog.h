#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Gundan {
		enum { 
			�R�c��=1,
			�R�c��=1<<1,
			����=1<<2,
			����=1<<3,
			����=1<<4,
			���m=1<<5,
			�R�n=1<<6,
			�R��=1<<7,
			�S�C=1<<8,
			�C��=1<<9,
			��=1<<10,
			�����x=1<<11,
			������=1<<12,
			���x�o=1<<13,
			��=1<<14,
			�Ď��x=1<<15,
			�Ď���=1<<16,
			�Ďx�o=1<<17,
			�s��=1<<18,
			���슯��=1<<19,
			���{��E=1<<20,
			�w�����e=1<<21,
			���j=1<<22,
			�U���ڕW=1<<23,
			�������_=1<<24,
			�R�����_=1<<25,

			�f�t�H���g=0xCD7F
		};
	}
}

// �ΏۂƂȂ�R�c���X�g�B�Ō�̃I�v�V�����͂P���I���ł��Ȃ��I�v�V�����B�ő吔�͂P�Œ�
int FuncSelectGundanDialog(vector<int> list, int iFilterOption=Filter::Gundan::�f�t�H���g );