#pragma once

#pragma pack(1)


#include <string>
#include <vector>

using namespace std;

namespace Filter {
	namespace Bushou {
		const __int64 ������=1;
		const __int64 �喼=1<<1;
		const __int64 �R�c=1<<2;
		const __int64 ��=1<<3;
		const __int64 �g��=1<<4;
		const __int64 �R�c��=1<<5;
		const __int64 ���=1<<6;
		const __int64 �N��=1<<7;
		const __int64 �d��=1<<8;
		const __int64 �M��=1<<9;
		const __int64 ����=1<<10;
		const __int64 ��]=1<<11;
		const __int64 ����=1<<12;
		const __int64 ����=1<<13;
		const __int64 ����=1<<14;
		const __int64 ����=1<<15;
		const __int64 �퓬=1<<16;
		const __int64 ���=1<<17;
		const __int64 �퓾=1<<18;
		const __int64 �q�d=1<<19;
		const __int64 �q��=1<<20;
		const __int64 �q��=1<<21;
		const __int64 ���m=1<<22;
		const __int64 �P��=1<<23;
		const __int64 �m�C=1<<24;
		const __int64 ����=1<<25;
		const __int64 ���y=1<<26;
		const __int64 �R�n=1<<27;
		const __int64 �S�C=1<<28;
		const __int64 ���R=1<<29;
		const __int64 ��C=1<<30;

		const __int64 �S�D=__int64(1)<<31;
		const __int64 ���ꕺ��=__int64(1)<<32;
		const __int64 �ꊅ=__int64(1)<<33;
		const __int64 �ÎE=__int64(1)<<34;
		const __int64 ���o=__int64(1)<<35;
		const __int64 ����=__int64(1)<<36;
		const __int64 ����=__int64(1)<<37;
		const __int64 ����=__int64(1)<<38;
		const __int64 �ē�=__int64(1)<<39;
		const __int64 ����=__int64(1)<<40;
		const __int64 �s��=__int64(1)<<41;
		const __int64 �=__int64(1)<<42;
		const __int64 �ӌ�=__int64(1)<<43;

		const int �f�t�H���g=0x93C10;
	}
}

// �ΏۂƂȂ镐�����X�g�B
int FuncSelectBushouDialog(vector<int> list, __int64 iFilterOption=Filter::Bushou::�f�t�H���g );