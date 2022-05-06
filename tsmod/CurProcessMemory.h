#pragma once

#include "OutputDebugString.h"
#include "OutputDebugStream.h"

enum DataLocation { LOC1=0, LOC2=1, LOC3=2, LOC4=3, LOC5=4, LOC6=5, LOCEND }; // これを使ってアクセスすること

class CurProcessMemory {
	public:
		CurProcessMemory();

		int m_LAdress[LOCEND]; // 具体的な(constな意味で)意味を持つメモリアドレスを格納する場所

		char szCurMemoryData[LOCEND][512]; // 各メモリアドレスのDataLocationに対応する文字列を格納する場所
		char szPreMemoryData[LOCEND][512]; // １つ前の各メモリアドレスのDataLocationに対応する文字列を格納する場所(有意義な変化があった場合のみCur⇒Preへの格納がされる)

		void setVariableFromMemory(void);  // これを呼び出すことでメモリ⇒クラスへと値を格納する

		char* getCurVarPtr( DataLocation eLOC ); // 現在のLOC[1～?]の値を得る
		char* getPreVarPtr( DataLocation eLOC ); // １つ前のLOC[1～?]の値を得る

		int getMemoryAddress( DataLocation eLOC );
};

extern CurProcessMemory CPM;
