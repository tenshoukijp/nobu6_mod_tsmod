#define  _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageN6PPatch.h"
#include "ScenarioMod.h"


// マウスで選択した軍団長の軍団番号
int iOnGungiSelectingGudanNoNo = -1;

/*
004161E4  |. 50             PUSH    EAX                     ← この時、ここ軍団番号    
004161E5  |. E8 34E80300    CALL    TENSHOU.00454A1E
				└ここを JMP TSMod.OnTSExeGungiGudanNo と書き換えてやる実際の処理
004161EA  |. 83C4 04        ADD     ESP, 4
004161ED  |. 48             DEC     EAX
004161EE  |. 8BD8           MOV     EBX, EAX
004161F0  |. 8B4D FC        MOV     ECX, DWORD PTR SS:[EBP-4]
*/
int pTenshouExeJumpFromToOnTenshouExeGungiGudanNo	 =0x4161E5; // 関数はこのアドレスから、OnTenshouExeGungiGudanNoへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGungiGudanNo =0x454A1E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGungiGudanNo  =0x4161EA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGungiGudanNo() {
	// スタックにためておく
	__asm {

		// 選択した軍団長の軍団番号がコピーされる。
		mov iOnGungiSelectingGudanNoNo, eax

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	
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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGungiGudanNo

		jmp pTenshouExeReturnLblFromOnTenshouExeGungiGudanNo
	}
}



char cmdOnTenshouExeJumpFromGungiGudanNo[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGungiGudanNo() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGungiGudanNo;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGungiGudanNo + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGungiGudanNo  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGungiGudanNo+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGungiGudanNo), cmdOnTenshouExeJumpFromGungiGudanNo, 5, NULL); //5バイトのみ書き込む
}




//--------------------------------------------------------------------------------------------------------




/*
 * 軍議の時は、メモリアドレスLOC4にほとんどセリフが書かれているが、
 * 大名のセリフは、メモリアドレスLOC1に書かれてしまうようだ。このため、大名の姓名がうまく渡らないという問題がある。
 *
 */
// 現在の吹き出しが、｢評定｣の吹き出しであることのフラグ
int	isOnCurGungiFukidashi = FALSE;

void OnTenshouExeGungiFukidashiExecute() {
	// MessageN6PPatch::isOnTenshouExeMessageCopyUsed = false;

	// 範囲チェック
	int iBushouID = -1;
	// 軍団長の武将ID(配列用)を得る。
	if ( 0 < iOnGungiSelectingGudanNoNo && iOnGungiSelectingGudanNoNo <= GAMEDATASTRUCT_GUNDAN_NUM ) {
		iBushouID = nb6gundan[iOnGungiSelectingGudanNoNo-1].leader-1;
	}

	// 武将正当性の範囲チェック
	if ( ! (0 <= iBushouID && iBushouID <= GAMEDATASTRUCT_BUSHOU_NUM) ) {
		return;
	}

	// ターゲットとなるメッセージのポインタは、大名(No1)か、軍団長(No2以降)かで異なる。
	char *pGungiTargetMessage = NULL;
	// 選択した該当の武将が大名の場合 ( State:0 == 大名 )
	if ( nb6bushouname[iBushouID].State == 0 ) {
		// 対象のメモリアドレスは、アドレス1
		pGungiTargetMessage = (char *)CPM.getMemoryAddress(LOC1);
	} else {
		// 対象のメモリアドレスは、アドレス4
		pGungiTargetMessage = (char *)CPM.getMemoryAddress(LOC4);
	}

	/*
		TSMod内で、軍議関連で文字列を置き換えたい場合は、この場所で
		pGungiTargetMessage の文字を調べて、pGungiTargetMessage自体を書き換えればよい。
	*/


	// 武将名, 現在の書き換え対象となっているメッセージ, 関連人物情報
	// 外部メッセージ置き換えをScenarioModにて行う
	if ( p_snOnRewriteFukidashiMessage ) {

		// ここで名前を作り出す。
		char name[256] = "";

		strcat( name, nb6bushouname[iBushouID].familyname);
		strcat( name, nb6bushouname[iBushouID].fastname);

		// ScenarioModで置き換える場合に対応する。
		NB6FUKIDASHIRELATEPERSONS gungifukidashirelatepaeresons;
		gungifukidashirelatepaeresons.i1stPersonID = iBushouID + 1;

		int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
		int iDaimyoBushouID = nb6daimyo[iDaimyoID].attach - 1;
		gungifukidashirelatepaeresons.i2ndPersonID = iDaimyoBushouID + 1;

		char szModMsgBuf[512] = "";

		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, pGungiTargetMessage, &gungifukidashirelatepaeresons);
		if ( strlen(szModMsgBuf) > 1 ) {
			WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pGungiTargetMessage), szModMsgBuf, strlen(szModMsgBuf)+1, NULL);
		}
	}

	isOnCurGungiFukidashi = TRUE;
	// 評定の吹き出しフラグを立てる。MessageN6PPatch.cpp の ReWritePatchMessageN6P で処理してももう間に合わないため、
	// そちらでの処理を抑制するためにこのフラグを立てる。
}


/*
00416223  |. FF75 EC        PUSH    DWORD PTR SS:[EBP-14]            ; |Arg2
00416226  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]            ; |Arg1
00416229  |. E8 28BF0800    CALL    TENSHOU.004A2156                 ; \TENSHOU.004A2156
				└ここを JMP TSMod.OnTSExeGungiFukidashi と書き換えてやる実際の処理
0041622E  |. 6A 08          PUSH    8
*/
int pTenshouExeJumpFromToOnTenshouExeGungiFukidashi	 =0x416229; // 関数はこのアドレスから、OnTenshouExeGungiFukidashiへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeGungiFukidashi =0x4A2156; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeGungiFukidashi  =0x41622E; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeGungiFukidashi() {
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
	OnTenshouExeGungiFukidashiExecute();

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

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeGungiFukidashi

		jmp pTenshouExeReturnLblFromOnTenshouExeGungiFukidashi
	}
}



char cmdOnTenshouExeJumpFromGungiFukidashi[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeGungiFukidashi() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeGungiFukidashi;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeGungiFukidashi + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeGungiFukidashi  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromGungiFukidashi+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeGungiFukidashi), cmdOnTenshouExeJumpFromGungiFukidashi, 5, NULL); //5バイトのみ書き込む
}




