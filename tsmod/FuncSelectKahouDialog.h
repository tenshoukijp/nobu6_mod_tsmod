#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Kahou {
		enum { 
			����=1,
			���=1<<1,
			����=1<<2,
			���L��=1<<3,
			����=1<<4,

			�f�t�H���g=0x1F
		};
	}
}

// �ΏۂƂȂ�ƕ󃊃X�g�B
// iFilterOption�͂ǂ��\������̂��H �Ƃ������
int FuncSelectKahouDialog(vector<int> list, int iFilterOption=Filter::Kahou::�f�t�H���g );