#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Kani {
		enum { 
			���ʖ�=1,
			��=1<<1,
			���L��=1<<2,
			����=1<<3,

			�f�t�H���g=0xF
		};
	}
}

// �ΏۂƂȂ�ƕ󃊃X�g�B
// iFilterOption�͂ǂ��\������̂��H �Ƃ������
int FuncSelectKaniDialog(vector<int> list, int iFilterOption=Filter::Kani::�f�t�H���g );