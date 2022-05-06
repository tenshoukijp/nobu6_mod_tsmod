#include "TSModCommand.h"
#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"

/*
0040E2C7  /$ 55             PUSH    EBP　　　　　　　　← この辺でECXが夫側の武将ID
0040E2C8  |. 8BEC           MOV     EBP, ESP
0040E2CA  |. 53             PUSH    EBX
0040E2CB  |. 56             PUSH    ESI
0040E2CC  |. 33DB           XOR     EBX, EBX
0040E2CE  |. 57             PUSH    EDI
0040E2CF  |. 8B75 0C        MOV     ESI, DWORD PTR SS:[EBP+C]
0040E2D2  |. 66:BF F501     MOV     DI, 1F5						← 501番からスタート
0040E2D6  |> 57             /PUSH    EDI
0040E2D7  |. E8 A1BCFFFF    |CALL    TENSHOU.00409F7D
0040E2DC  |. 83C4 04        |ADD     ESP, 4
0040E2DF  |. 83F8 0C        |CMP     EAX, 0C
0040E2E2  |. 72 51          |JB      SHORT TENSHOU.0040E335
0040E2E4  |. F6C4 80        |TEST    AH, 80
0040E2E7  |. 75 4C          |JNZ     SHORT TENSHOU.0040E335
0040E2E9  |. 57             |PUSH    EDI
0040E2EA  |. E8 EEC4FFFF    |CALL    TENSHOU.0040A7DD
0040E2EF  |. 83C4 04        |ADD     ESP, 4
0040E2F2  |. 85C0           |TEST    EAX, EAX
0040E2F4  |. 75 3F          |JNZ     SHORT TENSHOU.0040E335
0040E2F6  |. 57             |PUSH    EDI
0040E2F7  |. E8 B3C4FFFF    |CALL    TENSHOU.0040A7AF
0040E2FC  |. 83C4 04        |ADD     ESP, 4
0040E2FF  |. 85C0           |TEST    EAX, EAX						このEAXがTRUEなら姫とみなす最低要件の１つ
0040E301  |. 74 32          |JE      SHORT TENSHOU.0040E335
0040E303  |. FF75 14        |PUSH    DWORD PTR SS:[EBP+14]           ; /Arg4
0040E306  |. FF75 10        |PUSH    DWORD PTR SS:[EBP+10]           ; |Arg3
0040E309  |. FF75 08        |PUSH    DWORD PTR SS:[EBP+8]            ; |Arg2
0040E30C  |. 57             |PUSH    EDI                             ; |Arg1
0040E30D  |. E8 3C000000    |CALL    TENSHOU.0040E34E                ; \TENSHOU.0040E34E
0040E312  |. 83C4 10        |ADD     ESP, 10
0040E315  |. 85C0           |TEST    EAX, EAX						このEAXがTRUEなら姫とみなす最低要件の１つ。
0040E317  |. 74 1C          |JE      SHORT TENSHOU.0040E335
0040E319  |. 837D 18 00     |CMP     DWORD PTR SS:[EBP+18], 0
0040E31D  |. 74 0B          |JE      SHORT TENSHOU.0040E32A
0040E31F  |. 57             |PUSH    EDI
0040E320  |. FF55 18        |CALL    DWORD PTR SS:[EBP+18]
0040E323  |. 83C4 04        |ADD     ESP, 4
0040E326  |. 85C0           |TEST    EAX, EAX
0040E328  |. 74 0B          |JE      SHORT TENSHOU.0040E335
0040E32A  |> 85F6           |TEST    ESI, ESI
0040E32C  |. 74 06          |JE      SHORT TENSHOU.0040E334
0040E32E  |. 66:893E        |MOV     WORD PTR DS:[ESI], DI
0040E331  |. 83C6 02        |ADD     ESI, 2
0040E334  |> 43             |INC     EBX
0040E335  |> 66:47          |INC     DI
0040E337  |. 66:81FF 1402   |CMP     DI, 214						532人まで。	
0040E33C  |.^76 98          \JBE     SHORT TENSHOU.0040E2D6
0040E33E  |. 85F6           TEST    ESI, ESI
0040E340  |. 74 05          JE      SHORT TENSHOU.0040E347
0040E342  |. 66:C706 FFFF   MOV     WORD PTR DS:[ESI], 0FFFF
0040E347  |> 8BC3           MOV     EAX, EBX
0040E349  |. 5F             POP     EDI
0040E34A  |. 5E             POP     ESI
0040E34B  |. 5B             POP     EBX
0040E34C  |. 5D             POP     EBP
0040E34D  \. C3             RETN

*/


// 0040E2D2  |. 66:BF F501     MOV     DI, 1F5						← 501番からスタート
#define HIME_MIN_RANGE_ADDRESS	0x40E2D4


// 姫と判定するのを501人目以降ではなく、1人目以降とする。
void SetHideRangePatch() {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	short sValue = 1;
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(HIME_MIN_RANGE_ADDRESS), &sValue, 2, NULL); //1バイトのみ書き込む
}





//---------------------------------------------------------------------------------------------------

bool isHimeTartareAndVersalia(int iBushouID, int iHimeBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			if ( isLancelotTartare( iBushouID ) ) {
				// ベルセリアオヴェリス
				if ( strcmp(nb6bushouname[iHimeBushouID].familyname, "")==0 && strcmp(nb6bushouname[iHimeBushouID].fastname, "")==0 ) {
					return true;
				}
			}
		}
	}
	return false;
}

bool isHimeBaalzephonAndOzma(int iBushouID, int iHimeBushouID) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		if ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
			// オズマ・モー・グラシャスとバールゼフォン・V・ラームズ なら「婚約者」とする。
			if ( strcmp( nb6bushouname[iHimeBushouID].familyname, "ёжз")==0 && strcmp( nb6bushouname[iHimeBushouID].fastname, "ийк")==0
				&& strcmp( nb6bushouname[iBushouID].familyname, "")==0 && strcmp( nb6bushouname[iBushouID].fastname, "")==0 ) {
				// 婚約者
				return true;
			}
		}
	}
	return false;
}


/*
 * 姫か否かの判定時、その②。実際に姫なのかどうかの前半
 */

int iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = -1; // 夫側の武将番号が入っているアドレス

int iResultOnJudgeHimeHanteiPrev = 0;  // EAX用につかう結果値。姫相当と判断するためには1にする必要がある。
int iHimeBushouIDOnJudgeHimeHanteiPrev = -1; // 姫側の武将番号の値。EDIの下半分のDI部分だけ使う

/*
bool isHimeBaalzephonAndOzmaTmpMarryFlag = false; // バールゼフォンとオズマ用
int isHimeOzmaOriginalState = -1; // オズマ用
*/
void OnTenshouExeJudgeHimeHanteiPrevExecute() {

	iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x10;
	int iBushouID = *(WORD *)(iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen) - 1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		int iHimeBushouID = (iHimeBushouIDOnJudgeHimeHanteiPrev & 0xFFFF) - 1; // 姫武将のID

		// ランスロットとベルサリア
		// バールゼフォンとオズマ
		if ( isHimeTartareAndVersalia( iBushouID, iHimeBushouID) ) {

			iResultOnJudgeHimeHanteiPrev = 1; // 結果を上書きして、姫リストに登場させる。
		}

		else if ( isHimeBaalzephonAndOzma(iBushouID, iHimeBushouID ) ) {

			// iResultOnJudgeHimeHanteiPrev = 1; // 結果を上書きして、姫リストに登場させる。

			/*
			isHimeBaalzephonAndOzmaTmpMarryFlag = false; // バールゼフォンとオズマ用

			// 2人とも結婚していなければ…
			if ( nb6bushouref[iBushouID].spouse == 0xFF && nb6bushouref[iHimeBushouID].spouse == 0xFF ) {
				// 一時的に2人を結婚させる。

				// 「setHimeMarriage」を利用すると名字等まで変化するので使わない。
				// あくまで婚約者

				// オズマを姫にする。
				isHimeOzmaOriginalState = nb6bushouname[iHimeBushouID].State; // 元のステートを保持
				nb6bushouname[iHimeBushouID].State = 5; // 一時姫にして…

				// 両者の妻値をオズマのものにして結婚を装う
				nb6bushouref[iBushouID].spouse = iHimeBushouID-500;
				nb6bushou[iBushouID].spouse = iHimeBushouID-500;

				nb6bushouref[iHimeBushouID].spouse = iHimeBushouID-500;
				nb6bushou[iHimeBushouID].spouse = iHimeBushouID-500;
				
				// 後で元へと戻すので、一時的な結婚フラグをONにする
				isHimeBaalzephonAndOzmaTmpMarryFlag = true;
			}
			*/
		}

	}
}


/*
0040E2F7  |. E8 B3C4FFFF    |CALL    TENSHOU.0040A7AF
				└ここを JMP TSMod.OnTSExeJudgeHimeHanteiPrev と書き換えてやる実際の処理
0040E2FC  |. 83C4 04        |ADD     ESP, 4
0040E2FF  |. 85C0           |TEST    EAX, EAX						このEAXがTRUEなら姫とみなす最低要件の１つ
0040E301  |. 74 32          |JE      SHORT TENSHOU.0040E335
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev	 =0x40E2F7; // 関数はこのアドレスから、OnTenshouExeJudgeHimeHanteiPrevへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPrev =0x40A7AF; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPrev	 =0x40E2FC; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeHimeHanteiPrev() {
	// スタックにためておく
	__asm {

		mov iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen, ebp; // 武将番号が入っているアドレスをコピー

		mov iHimeBushouIDOnJudgeHimeHanteiPrev, edi // 下半分だけコピー

		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPrev // 元々のTENSHOU.EXEの処理

		mov iResultOnJudgeHimeHanteiPrev, eax // 結果をコピーしておく

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeJudgeHimeHanteiPrevExecute();

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

		mov eax, iResultOnJudgeHimeHanteiPrev // 結果を上書きする。

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPrev
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている
				└ここを JMP TSMod.OnTSExeJudgeHimeHanteiPrev と書き換えてやる実際の処理
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPrev() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeHimeHanteiPrev;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHimeHanteiPrev  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPrev), cmdOnTenshouExeJumpFromJudgeHimeHanteiPrev, 5, NULL); //5バイトのみ書き込む
}




//----------------------------------------------------------------------------------






/*
 * 姫か否かの判定時、その②。実際に姫なのかどうかの前半
 */

int iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = -1;

int iResultOnJudgeHimeHanteiPost = 0;
int iHimeBushouIDOnJudgeHimeHanteiPost = -1;
int iResultOnJudgeHimeHentaiPostNeedButton = 0; // 「姫」ボタン必要か
void OnTenshouExeJudgeHimeHanteiPostExecute() {

	// モードIDの取得(0x7か0xBか、など)
	iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x8;
	int iModeID = *(WORD *)(iOttoModeIDOnJudgeHimeHanteiFromCallBusyouDetailGamen);

	// 夫の武将IDを改めて取得
	iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen = iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen + 0x10;
	int iBushouID = *(WORD *)(iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen) - 1;

	int iHimeBushouID = (iHimeBushouIDOnJudgeHimeHanteiPost & 0xFFFF) - 1; // 姫武将のID

	if ( isHimeTartareAndVersalia( iBushouID, iHimeBushouID) ) {

		if ( iModeID == 0x7 || iModeID==0xF ) { // 何やらよくわからないが、リスト表示時はモードは0x7と0xBの2回実行される。また、姫ボタンそのものが有効か否かの際にはは0xFのモードとなる。一般の武将リストでは0xFとして実行される。
			iResultOnJudgeHimeHanteiPost = 1; // 結果を上書きして、姫リストに登場させる。
		}

	}
	else if ( isHimeBaalzephonAndOzma( iBushouID, iHimeBushouID) ) {

		if ( iModeID == 0x7 || iModeID==0xF ) { // 何やらよくわからないが、リスト表示時はモードは0x7と0xBの2回実行される。また、姫ボタンそのものが有効か否かの際にはは0xFのモードとなる。一般の武将リストでは0xFとして実行される。
			// iResultOnJudgeHimeHanteiPost = 1; // 結果を上書きして、姫リストに登場させる。
		}

	}

	/*
	else if (  isHimeBaalzephonAndOzmaTmpMarryFlag ) {
		if ( iModeID==0xF || iModeID==0xF  ) { // 何やらよくわからないが、リスト表示時はモードは0x7と0xBの2回実行される。また、姫ボタンそのものが有効か否かの際にはは0xFのモードとなる。一般の武将リストでは0xFとして実行される。
			iResultOnJudgeHimeHanteiPost = 1; // 結果を上書きして、姫リストに登場させる。
		}
		// オズマを元に戻す。
		nb6bushouname[iHimeBushouID].State = isHimeOzmaOriginalState; // 元のステートを保持

		// 両者の妻値をオズマのものにして結婚を装う
		nb6bushouref[iBushouID].spouse = 0xFF;
		nb6bushou[iBushouID].spouse = 0xFF;

		nb6bushouref[iHimeBushouID].spouse = 0xFF;
		nb6bushou[iHimeBushouID].spouse = 0xFF;
		
		// 一時結婚フラグOFF
		isHimeBaalzephonAndOzmaTmpMarryFlag = false;
	}
	*/
}


/*
0040E30D  |. E8 3C000000    |CALL    TENSHOU.0040E34E                ; \TENSHOU.0040E34E
				└ここを JMP TSMod.OnTSExeJudgeHimeHanteiPost と書き換えてやる実際の処理
0040E312  |. 83C4 10        |ADD     ESP, 10
0040E315  |. 85C0           |TEST    EAX, EAX						このEAXがTRUEなら姫とみなす最低要件の１つ。
0040E317  |. 74 1C          |JE      SHORT TENSHOU.0040E335
*/
int pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost	 =0x40E30D; // 関数はこのアドレスから、OnTenshouExeJudgeHimeHanteiPostへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPost =0x40E34E; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPost	 =0x40E312; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeJudgeHimeHanteiPost() {
	// スタックにためておく
	__asm {

		mov iOttoBushouIDOnJudgeHimeHanteiFromCallBusyouDetailGamen, ebp; // モードが入っているアドレスをコピー

		mov iHimeBushouIDOnJudgeHimeHanteiPost, edi // 下半分だけコピー

		call pTenshouExeJumpCallFromToOnTenshouExeJudgeHimeHanteiPost // 元々のTENSHOU.EXEの処理

		mov iResultOnJudgeHimeHanteiPost, eax // 結果をコピーしておく

		mov iResultOnJudgeHimeHentaiPostNeedButton, ebx // ebxに最終的な姫カウント

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeJudgeHimeHanteiPostExecute();

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

		mov eax, iResultOnJudgeHimeHanteiPost // 結果を上書きする。

		jmp pTenshouExeReturnLblFromOnTenshouExeJudgeHimeHanteiPost
	}
}

/*
00419DED   . 50             PUSH    EAX                              ; |Arg1
00419DEE   . E8 B843FFFF    CALL    TENSHOU.0040E1AB                 ; \TENSHOU.0040E1AB  ここのCallで披露で選択武将リスト一覧になりえる武将リストが、4DBD40に作られている
				└ここを JMP TSMod.OnTSExeJudgeHimeHanteiPost と書き換えてやる実際の処理
00419DF3   . 83C4 18        ADD     ESP, 18
*/
char cmdOnTenshouExeJumpFromJudgeHimeHanteiPost[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeJudgeHimeHanteiPost() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeJudgeHimeHanteiPost;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeJudgeHimeHanteiPost  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJudgeHimeHanteiPost+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeJudgeHimeHanteiPost), cmdOnTenshouExeJumpFromJudgeHimeHanteiPost, 5, NULL); //5バイトのみ書き込む
}


