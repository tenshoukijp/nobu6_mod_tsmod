
#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "FuncPlayResourceWav.h"

/*
0049BA16  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]
0049BA19  |. 6A 0A          PUSH    0A           // この10番の音というのが、鉄砲の発射音
0049BA1B  |. 33F6           XOR     ESI, ESI
0049BA1D  |. E8 8B78FEFF    CALL    TENSHOU.004832AD	//　ここで実際に再生することとなる。
0049BA22  |. 83C4 08        ADD     ESP, 8
*/


/*
 * 鉄砲を撃った時の音
 */

extern int iFireTeppouAttackBushouID; // 鉄砲を撃った武将
extern int iFireTeppouTargetBushouID; // 鉄砲を撃たれた武将

int isMustPlaySubstituteSE = 1; // 代替えの音を再生する必要があるかどうか。

int isShouldBePlayTeppouSE = 1;
void OnTenshouExeFireTeppouSEExecute() {
	isShouldBePlayTeppouSE = 1; // 普通は鉄砲をならすべき
	isMustPlaySubstituteSE = 1; // 本来代替えの音の再生の必要がある

	int iBushouID = iFireTeppouAttackBushouID-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// 足軽隊もしくは、騎馬隊なのに、鉄砲を撃ったということは… それは弓隊もしくは弓騎馬隊である
		if ( (nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {

			isShouldBePlayTeppouSE = 0;

			struct stat  st;
			// フル効果音キットを導入していれば…
			if (stat("n6pwav1.dll", &st) == 0) {
				isMustPlaySubstituteSE = 0; // 代替えは必要なし。
				FuncPlaySoundWav( 716 );
			} else {
			}
		}
	}
}



/*
0049BA16  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]
				└ここを JMP TSMod.OnTSExeFireTeppouSE と書き換えてやる実際の処理
0049BA19  |. 6A 0A          PUSH    0A           // この10番の音というのが、鉄砲の発射音
0049BA1B  |. 33F6           XOR     ESI, ESI
0049BA1D  |. E8 8B78FEFF    CALL    TENSHOU.004832AD	//　ここで実際に再生することとなる。
0049BA22  |. 83C4 08        ADD     ESP, 8
*/
int pTenshouExeJumpFromToOnTenshouExeFireTeppouSE		=0x49BA16; // 関数はこのアドレスから、OnTenshouExeFireTeppouSEへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE   =0x4832AD; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE	=0x49BA22; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFireTeppouSE() {
	// スタックにためておく
	__asm {

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
	OnTenshouExeFireTeppouSEExecute();

	// 普通に鉄砲隊もしくは鉄砲騎馬隊であれば… 音をそのままならす
	if ( isShouldBePlayTeppouSE ) {
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xA           // この10番の音というのが、鉄砲の発射音
			XOR     ESI, ESI

			call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // ←ここで音が再生される

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}

	// 弓だが、代替えの音が必要ならば…14番を鳴らす
	} else if ( isMustPlaySubstituteSE ) {
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xE           // この14番の音というのが、壁登り(ｼｭｼｭｼｭ)の発射音
			XOR     ESI, ESI

			call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // ←ここで音が再生される

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}

	// フル効果音きっとも入っており、弓ならば、別枠で鳴らしているので、関数を再生しない。
	} else {
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

			PUSH    DWORD PTR SS:[EBP-0x10]

			PUSH    0xA           // この10番の音というのが、鉄砲の発射音
			XOR     ESI, ESI

			// call pTenshouExeJumpCallFromToOnTenshouExeFireTeppouSE // ←ここで音が再生されるのをコメントアウトすることで、元の効果音はならない

			jmp pTenshouExeReturnLblFromOnTenshouExeFireTeppouSE
		}
	}
}




char cmdOnTenshouExeJumpFromFireTeppouSE[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFireTeppouSE() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFireTeppouSE;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireTeppouSE + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFireTeppouSE  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFireTeppouSE+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireTeppouSE), cmdOnTenshouExeJumpFromFireTeppouSE, 5, NULL); //5バイトのみ書き込む
}








///--------------------------------------------------------------------------------------------




/*
 * 鉄砲を撃った時の音
 */

int iCurrentKonranAttackSENo = -1; // 今回の攻撃によるＳＥナンバーの保存場所。0xAだと鉄砲音
int iKonranAttackSENoEAX = -1;
int isShouldBePlayOriginalSE = 0;

void OnTenshouExeFireKonranTeppouSEExecute() {
	isShouldBePlayOriginalSE = 1; // 普通は鉄砲をならすべき
	isMustPlaySubstituteSE = 1; // 本来代替えの音の再生の必要がある

	// もし鉄砲なのであれば…
	if ( iCurrentKonranAttackSENo == 0xA ) {

		int iBushouID = iFireTeppouAttackBushouID-1;

		if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// 足軽隊もしくは、騎馬隊なのに、鉄砲を撃ったということは… それは弓隊もしくは弓騎馬隊である
			if ( (nb6bushouref[iBushouID].form == 0 || nb6bushouref[iBushouID].form == 1) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				isShouldBePlayOriginalSE = 0;

				struct stat  st;
				// フル効果音キットを導入していれば…
				if (stat("n6pwav1.dll", &st) == 0) {
					isMustPlaySubstituteSE = 0; // 代替えは必要なし。
					FuncPlaySoundWav( 716 );
				} else {
				}
			}
		}
	} else {
	}
}



/*
004832B6  |. 83C4 04        ADD     ESP, 4
004832B9  |. 50             PUSH    EAX                              ; /Arg4
004832BA  |. 6A 00          PUSH    0                                ; |Arg3 = 00000000
004832BC  |. 6A 03          PUSH    3                                ; |Arg2 = 00000003
004832BE  |. FF7424 10      PUSH    DWORD PTR SS:[ESP+10]            ; |Arg1 = ここに10番が入った時、鉄砲の音のようだが、10番が入るとは限りあないかもしれないので判定の必要がある。
004832C2  |. E8 AE9F0000    CALL    TENSHOU.0048D275                 ; \TENSHOU.0048D275
				└ここを JMP TSMod.OnTSExeFireKonranTeppouSE と書き換えてやる実際の処理
004832C7  |. 83C4 10        ADD     ESP, 10
*/
int pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE		=0x4832C2; // 関数はこのアドレスから、OnTenshouExeFireKonranTeppouSEへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE =0x48D275; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE	=0x4832C7; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFireKonranTeppouSE() {
	// スタックにためておく
	__asm {
		mov iKonranAttackSENoEAX, eax		// eaxを使うので一端保持
		pop eax								// eaxに最後にpushした値を復元
		mov iCurrentKonranAttackSENo, eax	// それを保存
		mov eax, iKonranAttackSENoEAX		// eaxを元の値に戻す

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
	OnTenshouExeFireKonranTeppouSEExecute();

	// 鉄砲なら元のまま
	if ( isShouldBePlayOriginalSE ) {

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

			push iCurrentKonranAttackSENo
			call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // ←ここで音が再生される

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}
	}

	// 弓だが、代替えの音が必要ならば…14番を鳴らす
	else if ( isMustPlaySubstituteSE ) {

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

			push 0xE
			call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // ←ここで音が再生される

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}

	// フル効果音きっとも入っており、弓ならば、別枠で鳴らしているので、関数を再生しない。
	} else {
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

			push iCurrentKonranAttackSENo
			// call pTenshouExeJumpCallFromToOnTenshouExeFireKonranTeppouSE // ←ここで音が再生されるのをコメントアウトすることで、元の効果音はならない

			jmp pTenshouExeReturnLblFromOnTenshouExeFireKonranTeppouSE
		}
	}
}




char cmdOnTenshouExeJumpFromFireKonranTeppouSE[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFireKonranTeppouSE() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFireKonranTeppouSE;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFireKonranTeppouSE  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFireKonranTeppouSE+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFireKonranTeppouSE), cmdOnTenshouExeJumpFromFireKonranTeppouSE, 5, NULL); //5バイトのみ書き込む
}




