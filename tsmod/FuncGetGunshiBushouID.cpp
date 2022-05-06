#include "WinTarget.h"

#include "GameDataStruct.h"

/*
軍師関数の呼び出し例
00427768  |. 50             PUSH    EAX							// 軍団番号
00427769  |. E8 7BD40200    CALL    TENSHOU.00454BE9			← // 軍師Get関数
0042776E  |. 66:A3 907C4D00 MOV     WORD PTR DS:[4D7C90], AX	// どうも現在の軍団の軍師番号は0x4D7C90に格納されるらしい。「EAX」ではなく「AX」の部分だけ使う。
00427774  |. 83C4 04        ADD     ESP, 4

*/


/*
 軍団番号を引数として、軍師の武将番号を得る
*/
int pCallFromFuncGetGunshiBushouID = 0x454BE9; //

int iReturnOfBushouIDGetGunshiBushou = 0xFFFF;
int iSigOfGundanIDGetGunshiBushou = 0xFFFF;

void FuncGetGunshiBushouIDAsm() {
	__asm {
		// 軍団番号が引数
		MOV     ESI, iSigOfGundanIDGetGunshiBushou

		// 軍団番号を引数として、軍師番号を得るための関数を呼び出す
		PUSH    ESI
		CALL    pCallFromFuncGetGunshiBushouID
		ADD     ESP, 4

		MOV iReturnOfBushouIDGetGunshiBushou, EAX // 結果のうち、AXの番号を軍師武将番号として保存

	}
}

// 軍団の軍師武将を得る。軍師たる者が居なければ0xFFFFが返ってくる。
int getGunshiBushouID(int iGundanID) {

	if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {

		iSigOfGundanIDGetGunshiBushou = iGundanID+1; // 配列用→軍団に変更

		iReturnOfBushouIDGetGunshiBushou = 0xFFFF; // まずは、毎度この値でリセット。

		__asm {
			push eax
			push ebx
			push ecx
			push edx
			push esp
			push esi
			push edi
		}

		// TENSHOU.EXE内の関数呼び出し
		FuncGetGunshiBushouIDAsm();

		__asm {
			pop edi
			pop esi
			pop esp
			pop edx
			pop ecx
			pop ebx
			pop eax
		}

		iReturnOfBushouIDGetGunshiBushou = iReturnOfBushouIDGetGunshiBushou & 0xFFFF; // EAXのうち、AXの部分だけ利用する。

		// その軍団に軍師が居たならば
		if ( 0 <= iReturnOfBushouIDGetGunshiBushou-1 && iReturnOfBushouIDGetGunshiBushou-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
			return iReturnOfBushouIDGetGunshiBushou-1; // 武将番号→武将番号配列用にして返す
		}
	}

	return 0xFFFF;
}