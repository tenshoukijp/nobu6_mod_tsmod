#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

#include "ScenarioMod.h"

// ＰＣ専用の戦後の登用処遇の可否

/*
004298AA   /$ B8 229A4200    MOV     EAX, TENSHOU.00429A22
004298AF   |. E8 C86B0800    CALL    TENSHOU.004B047C
004298B4   |. 81EC A8000000  SUB     ESP, 0A8
004298BA   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
004298C0   |. 53             PUSH    EBX
004298C1   |. 56             PUSH    ESI
004298C2   |. 57             PUSH    EDI
004298C3   |. 33F6           XOR     ESI, ESI
004298C5   |. E8 6D010000    CALL    TENSHOU.00429A37
004298CA   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004298CD   |. C745 F0 010000>MOV     DWORD PTR SS:[EBP-10], 1
004298D4   |. 8975 FC        MOV     DWORD PTR SS:[EBP-4], ESI
004298D7   |. E8 030EFEFF    CALL    TENSHOU.0040A6DF
004298DC   |. 83C4 04        ADD     ESP, 4
004298DF   |. 85C0           TEST    EAX, EAX
004298E1   |. 75 10          JNZ     SHORT TENSHOU.004298F3
004298E3   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]
004298E6   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004298E9   |. E8 EAFEFFFF    CALL    TENSHOU.004297D8			← このEAXの結果が1なら登用成功
004298EE   |. 83C4 08        ADD     ESP, 8
004298F1   |. 8BF0           MOV     ESI, EAX					← EAX→ESIへ
004298F3   |> 6A 01          PUSH    1
004298F5   |. 6A 0A          PUSH    0A
004298F7   |. E8 ED3A0600    CALL    TENSHOU.0048D3E9
004298FC   |. 83C4 08        ADD     ESP, 8
004298FF   |. A1 18EF4B00    MOV     EAX, DWORD PTR DS:[4BEF18]
00429904   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
0042990A   |. 33FF           XOR     EDI, EDI
0042990C   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; /Arg4
0042990F   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg3
00429912   |. 50             PUSH    EAX                              ; |Arg2 => 0000FFFF
00429913   |. FF35 14EF4B00  PUSH    DWORD PTR DS:[4BEF14]            ; |Arg1 = 0000FFFF
00429919   |. E8 9DB00700    CALL    TENSHOU.004A49BB                 ; \TENSHOU.004A49BB
0042991E   |. BB 01000000    MOV     EBX, 1
00429923   |> 8B04BD 20EF4B0>MOV     EAX, DWORD PTR DS:[EDI*4+4BEF20]
0042992A   |. 68 FFFF0000    PUSH    0FFFF
0042992F   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
00429932   |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
00429935   |. E8 A0B60700    CALL    TENSHOU.004A4FDA
0042993A   |. 83C4 04        ADD     ESP, 4
0042993D   |. 50             PUSH    EAX                              ; /Arg3
0042993E   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2
00429941   |. FF75 EC        PUSH    DWORD PTR SS:[EBP-14]            ; |Arg1
00429944   |. E8 362B0400    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
00429949   |. 83C4 0C        ADD     ESP, 0C
0042994C   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]       ; |
00429952   |. 50             PUSH    EAX                              ; |Arg1
00429953   |. E8 32B30700    CALL    TENSHOU.004A4C8A                 ; \TENSHOU.004A4C8A
00429958   |. E8 7A1F0500    CALL    TENSHOU.0047B8D7
0042995D   |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
00429960   |. 85C0           TEST    EAX, EAX						←この段階でESIが１なら登用成功(EAXではなくESI)
00429962   |. 74 0B          JE      SHORT TENSHOU.0042996F
00429964   |. 3BC3           CMP     EAX, EBX
00429966   |. 74 1A          JE      SHORT TENSHOU.00429982
00429968   |. 83F8 02        CMP     EAX, 2
0042996B   |. 74 28          JE      SHORT TENSHOU.00429995
0042996D   |. EB 4B          JMP     SHORT TENSHOU.004299BA
0042996F   |> C745 E8 DD0B00>MOV     DWORD PTR SS:[EBP-18], 0BDD
00429976   |. C745 E4 FFFF00>MOV     DWORD PTR SS:[EBP-1C], 0FFFF
0042997D   |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
00429980   |. EB 38          JMP     SHORT TENSHOU.004299BA
00429982   |> C745 E8 DF0B00>MOV     DWORD PTR SS:[EBP-18], 0BDF
00429989   |. C745 E4 070000>MOV     DWORD PTR SS:[EBP-1C], 7
00429990   |. 895D F0        MOV     DWORD PTR SS:[EBP-10], EBX
00429993   |. EB 25          JMP     SHORT TENSHOU.004299BA
00429995   |> 85F6           TEST    ESI, ESI						←この段階でESIが１なら成功(上でESIに設定しておけば、そのままの値が来る)
00429997   |. 74 0C          JE      SHORT TENSHOU.004299A5
00429999   |. C745 E8 E10B00>MOV     DWORD PTR SS:[EBP-18], 0BE1
004299A0   |. 895D E4        MOV     DWORD PTR SS:[EBP-1C], EBX
004299A3   |. EB 15          JMP     SHORT TENSHOU.004299BA
004299A5   |> C745 F0 000000>MOV     DWORD PTR SS:[EBP-10], 0
004299AC   |. 6A 02          PUSH    2
004299AE   |. 53             PUSH    EBX
004299AF   |. 57             PUSH    EDI
004299B0   |. E8 B7340800    CALL    TENSHOU.004ACE6C
004299B5   |. 83C4 0C        ADD     ESP, 0C
004299B8   |. 8BF8           MOV     EDI, EAX
004299BA   |> 837D F0 00     CMP     DWORD PTR SS:[EBP-10], 0
004299BE   |.^0F84 5FFFFFFF  JE      TENSHOU.00429923
004299C4   |. FF75 E4        PUSH    DWORD PTR SS:[EBP-1C]
004299C7   |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]
004299CA   |. E8 0BB60700    CALL    TENSHOU.004A4FDA
004299CF   |. 83C4 04        ADD     ESP, 4
004299D2   |. 50             PUSH    EAX                              ; /Arg3
004299D3   |. FF75 0C        PUSH    DWORD PTR SS:[EBP+C]             ; |Arg2
004299D6   |. FF75 E8        PUSH    DWORD PTR SS:[EBP-18]            ; |Arg1
004299D9   |. E8 A12A0400    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004299DE   |. 83C4 0C        ADD     ESP, 0C
004299E1   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]       ; |
004299E7   |. 50             PUSH    EAX                              ; |Arg1
004299E8   |. E8 9DB20700    CALL    TENSHOU.004A4C8A                 ; \TENSHOU.004A4C8A
004299ED   |. 6A 02          PUSH    2
004299EF   |. E8 3697FFFF    CALL    TENSHOU.0042312A
004299F4   |. 83C4 04        ADD     ESP, 4
004299F7   |. 8D8D 4CFFFFFF  LEA     ECX, DWORD PTR SS:[EBP-B4]
004299FD   |. E8 363A0500    CALL    TENSHOU.0047D438
00429A02   |. C745 FC FFFFFF>MOV     DWORD PTR SS:[EBP-4], -1
00429A09   |. E8 1E000000    CALL    TENSHOU.00429A2C
00429A0E   |. 8B45 EC        MOV     EAX, DWORD PTR SS:[EBP-14]
00429A11   |. 8B4D F4        MOV     ECX, DWORD PTR SS:[EBP-C]
00429A14   |. 64:890D 000000>MOV     DWORD PTR FS:[0], ECX
00429A1B   |. 5F             POP     EDI
00429A1C   |. 5E             POP     ESI
00429A1D   |. 5B             POP     EBX
00429A1E   |. 8BE5           MOV     ESP, EBP
00429A20   |. 5D             POP     EBP
00429A21   \. C3             RETN
*/


/* ＮＰＣ　ＰＣ　共通
004298A3   |. 5D             POP     EBP
004298A4   |. F7D8           NEG     EAX
004298A6   |. 5F             POP     EDI
004298A7   |. 5E             POP     ESI
004298A8   |. 5B             POP     EBX
004298A9   \. C3             RETN			← この段階でEAXが1なら登用成功、0なら登用失敗
*/

/*
004297D8   /$ 53             PUSH    EBX
004297D9   |. 56             PUSH    ESI
004297DA   |. 57             PUSH    EDI
004297DB   |. 55             PUSH    EBP
004297DC   |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004297E0   |. E8 F5B70700    CALL    TENSHOU.004A4FDA			← この値のＡＸが処断する側の大名の武将番号と思われる。
004297E5   |. 66:8B7C24 1C   MOV     DI, WORD PTR SS:[ESP+1C] 
004297EA   |. 83C4 04        ADD     ESP, 4
004297ED   |. 66:8BD8        MOV     BX, AX							（ここでＡＸをＢＸにコピーしているので注意）
*/

int iDaimyoBushouIDOfPostBattleTreatmentPeriod = -1; // 捕虜処遇する側の大名の武将番号(+1多い方)





//-------------------------------------------処断する側の大名の番号の保持--------------------------------


/*
004297DC   |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004297E0   |. E8 F5B70700    CALL    TENSHOU.004A4FDA			← この値のＡＸが処断する側の大名の武将番号と思われる。
				└ここを JMP TSMod.OnTSExeDaimyoBushouIDOfPostBattleTreatment と書き換えてやる実際の処理
004297E5   |. 66:8B7C24 1C   MOV     DI, WORD PTR SS:[ESP+1C] 
004297EA   |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment	 =0x4297E0; // 関数はこのアドレスから、OnTenshouExeDaimyoBushouIDOfPostBattleTreatmentへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment =0x4A4FDA; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeDaimyoBushouIDOfPostBattleTreatment  =0x4297E5; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeDaimyoBushouIDOfPostBattleTreatment() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment

		mov iDaimyoBushouIDOfPostBattleTreatmentPeriod, EAX	// この値のＡＸの部分だけが処断する側の大名の武将番号。これを保存しておく。

		jmp pTenshouExeReturnLblFromOnTenshouExeDaimyoBushouIDOfPostBattleTreatment
	}
}

char cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeDaimyoBushouIDOfPostBattleTreatment() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeDaimyoBushouIDOfPostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeDaimyoBushouIDOfPostBattleTreatment  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeDaimyoBushouIDOfPostBattleTreatment), cmdOnTenshouExeJumpFromDaimyoBushouIDOfPostBattleTreatment, 5, NULL); //5バイトのみ書き込む
}






//-------------------------------------------処断時の登用の成否--------------------------------
extern BOOL PostBattleTreatmentPeriod; // グローバルフラグ。捕虜処遇のタイミング

int iFlagOfSuccessOrFailOfPostBattleTreatment = -1;	// 登用の成否。 1が成功。0が失敗。
int iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment = -1; // 処断武将の武将番号(1多い方)
void OnTenshouExeSuccessOrFailOfPostBattleTreatmentExecute() {

	// 処断される側の武将ID
	int iTreatmenBushouID = (iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment & 0xFFFF) - 1;

	// 処断する側の大将の武将ID(大名じゃない)
	int iDaimyouBushouID = (iDaimyoBushouIDOfPostBattleTreatmentPeriod & 0xFFFF) - 1;

	// 戦争後の処遇中
	if ( PostBattleTreatmentPeriod ) {
		// 武将が有効範囲に入っている。
		if ( 0 <= iTreatmenBushouID && iTreatmenBushouID < GAMEDATASTRUCT_BUSHOU_NUM && 
			 0 <= iDaimyouBushouID && iDaimyouBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

			// ScenarioModのカスタム条件で捕縛処遇登用可否を設定する
			if ( p_snOnCustomCondition ) {															                        //┌処遇される側    ┌処遇側(通常大名)		┌ 現在予定されている成否結果(1が成功。0が失敗)
				int isTouyouSuccessOrFail = ((PFNSNONCUSTOMCONDITION) p_snOnCustomCondition)("AS条件::戦争::捕虜処遇登用可否", iTreatmenBushouID+1, iDaimyouBushouID+1, iFlagOfSuccessOrFailOfPostBattleTreatment, "", "", 0.0, 0.0);
				// 捕縛処遇登用可否結果が変更となっている。
				if ( isTouyouSuccessOrFail != CUSTOM_CONDITION_NOMODIFY ) {
					iFlagOfSuccessOrFailOfPostBattleTreatment = isTouyouSuccessOrFail;
				}
			}
		}
	}
}


/*
004298A3   |. 5D             POP     EBP
004298A4   |. F7D8           NEG     EAX
				└ここを JMP TSMod.OnTSExeSuccessOrFailOfPostBattleTreatment と書き換えてやる実際の処理
004298A6   |. 5F             POP     EDI
004298A7   |. 5E             POP     ESI
004298A8   |. 5B             POP     EBX
004298A9   \. C3             RETN			← この段階でEAXが1なら登用成功、0なら登用失敗
*/
int pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment	    =0x04298A4; // 関数はこのアドレスから、OnTenshouExeSuccessOrFailOfPostBattleTreatmentへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeSuccessOrFailOfPostBattleTreatment  =0x4298A9; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSuccessOrFailOfPostBattleTreatment() {
	// スタックにためておく
	// スタックにためておく
	__asm {
		// ここからが元々Tenshou.EXEにあった処理
		NEG     EAX
		POP     EDI
		POP     ESI
		POP     EBX

		mov iFlagOfSuccessOrFailOfPostBattleTreatment, EAX // 成否の現在の結果を入れておく。1が登用成功。0が登用失敗
		mov iTreatmentBushouOffSuccessOrFailOfPostBattleTreatment, EDI // 処断される側の武将番号。

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
	OnTenshouExeSuccessOrFailOfPostBattleTreatmentExecute();

	__asm {

		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iFlagOfSuccessOrFailOfPostBattleTreatment // 処断の成否を上書きする。

		jmp pTenshouExeReturnLblFromOnTenshouExeSuccessOrFailOfPostBattleTreatment
	}
}




char cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSuccessOrFailOfPostBattleTreatment() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSuccessOrFailOfPostBattleTreatment;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSuccessOrFailOfPostBattleTreatment  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSuccessOrFailOfPostBattleTreatment), cmdOnTenshouExeJumpFromSuccessOrFailOfPostBattleTreatment, 5, NULL); //5バイトのみ書き込む
}




