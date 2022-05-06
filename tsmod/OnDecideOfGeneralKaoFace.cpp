#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

// 武将や姫用の合成顔グラ編集時に、決定をした時

/*
以下、合成顔を決定した時


00407CB4  |. E8 1E3C0700    CALL    TENSHOU.0047B8D7
00407CB9  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX   ← このEAXに顔番号が入っているが、
																	本来は以下のように先頭が0b11という形でセーブデータに入っているが、この段階では最初が0b10というように１つフラグが立たない。

													■セーブデータやメモリの合成顔番号

															  ┌顔。左上が0x0　右下が0xF 1は左上の１つ下。
　　　　　													  │
													0b1100000[1111][000][00] 
　　　　　													  │　　 │ └ 行の中で４つのどれなのか
　　　　　													  └16顔 └猛将・知将などの行


														  
													■この段階のEAXの顔番号(最初が0b10と１つビットが立ってない)

															  ┌顔。左上が0x0　右下が0xF 1は左上の１つ下。
　　　　　													  │
													0b1000000[1111][000][00] 
　　　　　													  │　　 │ └ 行の中で４つのどれなのか
　　　　　													  └16顔 └猛将・知将などの行

*/


// そもそもちゃんと顔が出ない番号。よってこれらの番号は使ってはならない
extern DDRAW_HIMEKAO_BUG_MAPPING nb6ddraw_himekao_bug[10];


int iFaceIdOffGeneralKaoFaceExecute = -1;
int iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID = -1;
void OnTenshouExeDecideOfGeneralKaoFaceExecute() {
	int iNormalizedFaceID = iFaceIdOffGeneralKaoFaceExecute | 0x4000; // 0b[010][00][00][0000][00000] とのor

	// 駄目顔かどうかをチェック
	for ( int i=0; i < sizeof(nb6ddraw_himekao_bug)/sizeof(nb6ddraw_himekao_bug[0]); i++ ) {
		// もしダメ顔と一致したなら、もっとも近い矯正顔に直す
		if ( iNormalizedFaceID == nb6ddraw_himekao_bug[i].wrong ) {
			iNormalizedFaceID = nb6ddraw_himekao_bug[i].right;
			iFaceIdOffGeneralKaoFaceExecute = iNormalizedFaceID & 0xbfff; // 0b[101][11][11][1111][11111] とのand 
			break;
		}
	}

	iExterniFaceIdOffGeneralKaoFaceExecuteNormalizedFaceID = iNormalizedFaceID;
	// OutputDebugStream("%d", iNormalizedFaceID);
}


/*
00407CB4  |. E8 1E3C0700    CALL    TENSHOU.0047B8D7
				└ここを JMP TSMod.OnTSExeDecideOfGeneralKaoFace と書き換えてやる実際の処理
00407CB9  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX   ← このEAXに顔番号が入っているが、
*/
int pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace		=0x407CB4; // 関数はこのアドレスから、OnTenshouExeDecideOfGeneralKaoFaceへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDecideOfGeneralKaoFace =0x47B8D7; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDecideOfGeneralKaoFace	=0x407CB9; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDecideOfGeneralKaoFace() {

	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeDecideOfGeneralKaoFace

		// 選択した顔番号を保存
		mov iFaceIdOffGeneralKaoFaceExecute, eax

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
	OnTenshouExeDecideOfGeneralKaoFaceExecute();

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

		// 駄目顔を矯正する場合があるので結果を上書き
		mov eax, iFaceIdOffGeneralKaoFaceExecute

		jmp pTenshouExeReturnLblFromOnTenshouExeDecideOfGeneralKaoFace
	}
}

/*
0049D7AA  |. 6A 03          |PUSH    3                               ; |Arg3 = 00000003
0049D7AC  |. 56             |PUSH    ESI                             ; |Arg2
0049D7AD  |. 56             |PUSH    ESI                             ; |Arg1
0049D7AE  |. E8 D3540000    |CALL    TENSHOU.004A2C86                ; \TENSHOU.004A2C86
				└ここを JMP TSMod.OnTSExeDecideOfGeneralKaoFace と書き換えてやる実際の処理
0049D7B3  |. 83C4 1C        |ADD     ESP, 1C
0049D7B6  |. 85C0           |TEST    EAX, EAX
*/
char cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDecideOfGeneralKaoFace() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDecideOfGeneralKaoFace;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDecideOfGeneralKaoFace  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDecideOfGeneralKaoFace), cmdOnTenshouExeJumpFromDecideOfGeneralKaoFace, 5, NULL); //5バイトのみ書き込む
}




