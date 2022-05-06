#include "WinTarget.h"

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
 * 大砲を所持してるかどうかで
 * 判定するが。弓処理との兼ね合いで、雨が降ってるとどうこうの判定をパッチする。
 */
/*
0042B456   . E8 4AEEFDFF    CALL    TENSHOU.0040A2A5
0042B45B   . 83C4 04        ADD     ESP, 4
0042B45E   . 85C0           TEST    EAX, EAX
0042B460   . 75 04          JNZ     SHORT TENSHOU.0042B466
*/

// 対象武将は雨鉄砲が可能かどうか
extern bool IsCanAmeDeppou(int iBushouID );


//-----------------------------------その１---------------------------------------

int iCurBushouIDOnTaihouPatch = -1;
int isHasTaihouOnTaihouPatch = 0;

// 本当に今、大砲が撃てるのかどうか
BOOL IsCanTaihouPatch(int iBushouID ) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 対象武将が(城に居れば)位置を得る。
		HEX_POSITION p = GetCastleHexUnitPos( iBushouID );
		// 武将の位置の役割を得る
		int iHexRole = GetCastleHexRole( p );

		// 大砲を持っている
		if ( nb6bushouref[iBushouID].biggun ) {
			// 雨鉄砲を持っていれば、天気にかかわらず打てる
			if ( IsCanAmeDeppou(iBushouID) ) {
				return true;

			// 雨鉄砲は持っていないが雨以外なので打てる
			// 天気情報を得る 0:晴れ 1:曇り 2:雨 3:雪
			} else if ( getWeather() != 2 ) {
				return true;

			// 雨降って、雨鉄砲も持ってないが、本丸なので打てます。
			} else if ( iHexRole == CASTLE_HEX_ROLE::櫓 || iHexRole == CASTLE_HEX_ROLE::本丸 ) {
				return true;

			// 他は打てない
			} else  {
				return false;
			}

		// 大砲を持っていない
		} else {
			return false;
		}
	} return false;
}

void OnTenshouExeJudgeHasTaihouExecute() {
	int iBushouID = iCurBushouIDOnTaihouPatch-1; // 武将ＩＤへ

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 弓隊になっている場合は、変なことになっているので、大砲に関して辻褄を合わせる処理へと流す。
		if ( nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1 ) {
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				isHasTaihouOnTaihouPatch = IsCanTaihouPatch(iBushouID); // 大砲打てる打てない情報を上書き
			}
		}
	}
}



/*
0042B456   . E8 4AEEFDFF    CALL    TENSHOU.0040A2A5
				└ここを JMP TSMod.OnTSExeJudgeHasTaihou1 と書き換えてやる実際の処理
0042B45B   . 83C4 04        ADD     ESP, 4
0042B45E   . 85C0           TEST    EAX, EAX

EDIが武将番号
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1	 =0x42B456; // 関数はこのアドレスから、OnTenshouExeJudgeHasTaihou1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou1 =0x40A2A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou1	 =0x42B45B; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou1() {
	// スタックにためておく
	__asm {

		mov iCurBushouIDOnTaihouPatch, edi // 武将番号をコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou1

		mov isHasTaihouOnTaihouPatch, eax // 大砲所持判定

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeJudgeHasTaihouExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov eax, isHasTaihouOnTaihouPatch // EAXに大砲可能情報としてオーバーライド

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou1
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeHasTaihou1() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeHasTaihou1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou1  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou1), cmdOnTenshouExeJumpFromJudgeHasTaihou1, 5, NULL); //5バイトのみ書き込む
}



//-----------------------------------その２---------------------------------------





/*
0046960D  |. E8 930CFAFF    CALL    TENSHOU.0040A2A5
				└ここを JMP TSMod.OnTSExeJudgeHasTaihou2 と書き換えてやる実際の処理
00469612  |. 83C4 04        ADD     ESP, 4
00469615  |. 85C0           TEST    EAX, EAX

EDIが武将番号
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2	 =0x46960D; // 関数はこのアドレスから、OnTenshouExeJudgeHasTaihou2へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou2 =0x40A2A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou2	 =0x469612; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou2() {
	// スタックにためておく
	__asm {

		mov iCurBushouIDOnTaihouPatch, edi // 武将番号をコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou2

		mov isHasTaihouOnTaihouPatch, eax // 大砲所持判定

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeJudgeHasTaihouExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov eax, isHasTaihouOnTaihouPatch // EAXに大砲可能情報としてオーバーライド

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou2
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou2[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeHasTaihou2() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeHasTaihou2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou2  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou2), cmdOnTenshouExeJumpFromJudgeHasTaihou2, 5, NULL); //5バイトのみ書き込む
}



//-----------------------------------その３---------------------------------------





/*
0042B202  |. E8 9EF0FDFF    CALL    TENSHOU.0040A2A5
				└ここを JMP TSMod.OnTSExeJudgeHasTaihou3 と書き換えてやる実際の処理
0042B207  |. 83C4 04        ADD     ESP, 4
0042B20A  |. 85C0           TEST    EAX, EAX

この瞬間はESIが武将番号
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3	 =0x42B202; // 関数はこのアドレスから、OnTenshouExeJudgeHasTaihou3へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou3 =0x40A2A5; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou3	 =0x42B207; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeHasTaihou3() {
	// スタックにためておく
	__asm {

		mov iCurBushouIDOnTaihouPatch, esi // 武将番号をコピー

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHasTaihou3

		mov isHasTaihouOnTaihouPatch, eax // 大砲所持判定

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// ゲームデータをプレイヤーターンが変わった段階で読み込み更新しておく
	OnTenshouExeJudgeHasTaihouExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov eax, isHasTaihouOnTaihouPatch // EAXに大砲可能情報としてオーバーライド

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHasTaihou3
	}
}




char cmdOnTenshouExeJumpFromJudgeHasTaihou3[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeHasTaihou3() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeHasTaihou3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3 + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHasTaihou3  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeHasTaihou3+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHasTaihou3), cmdOnTenshouExeJumpFromJudgeHasTaihou3, 5, NULL); //5バイトのみ書き込む
}




//-----------------------------------その４---------------------------------------






