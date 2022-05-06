#define _CRT_SECURE_NO_WARNINGS
#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "ScenarioMod.h"
#include "MessageN6PPatch.h"
#include "FuncFukidashiDialog.h"
/*
//--------------------------左上 EAXは多分武将番号
0048B089   . 50             PUSH    EAX								武将番号？
0048B08A   . 57             PUSH    EDI                              ; /Arg8
0048B08B   . 57             PUSH    EDI                              ; |Arg7
0048B08C   . BD 907C4D00    MOV     EBP, TENSHOU.004D7C90            ; |
0048B091   . 57             PUSH    EDI                              ; |Arg6
0048B092   . 57             PUSH    EDI                              ; |Arg5
0048B093   . 57             PUSH    EDI                              ; |Arg4
0048B094   . 57             PUSH    EDI                              ; |Arg3
0048B095   . 55             PUSH    EBP                              ; |Arg2 => 004D7C90
0048B096   . 68 E8360000    PUSH    36E8                             ; |Arg1 = 000036E8
0048B09B   . E8 C00FFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B0A0   . 83C4 20        ADD     ESP, 20
0048B0A3   . 50             PUSH    EAX								書き換えアドレス？
0048B0A4   . E8 FE6E0100    CALL    TENSHOU.004A1FA7
0048B0A9   . 83C4 0C        ADD     ESP, 0C

//--------------------------右下 EAXは多分武将番号
0048B13D   . 50             PUSH    EAX
0048B13E   . 57             PUSH    EDI                              ; /Arg8
0048B13F   . 57             PUSH    EDI                              ; |Arg7
0048B140   . 57             PUSH    EDI                              ; |Arg6
0048B141   . 57             PUSH    EDI                              ; |Arg5
0048B142   . 57             PUSH    EDI                              ; |Arg4
0048B143   . 57             PUSH    EDI                              ; |Arg3
0048B144   . 55             PUSH    EBP                              ; |Arg2
0048B145   . 68 EB360000    PUSH    36EB                             ; |Arg1 = 000036EB		← シナリオID
0048B14A   . E8 110FFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B14F   . 83C4 20        ADD     ESP, 20



// また左上
0048B1E9   . 57             PUSH    EDI
0048B1EA   . 50             PUSH    EAX
0048B1EB   . 57             PUSH    EDI                              ; /Arg8
0048B1EC   . 57             PUSH    EDI                              ; |Arg7
0048B1ED   . 57             PUSH    EDI                              ; |Arg6
0048B1EE   . 57             PUSH    EDI                              ; |Arg5
0048B1EF   . 57             PUSH    EDI                              ; |Arg4
0048B1F0   . 57             PUSH    EDI                              ; |Arg3
0048B1F1   . 57             PUSH    EDI                              ; |Arg2
0048B1F2   . 68 F0360000    PUSH    36F0                             ; |Arg1 = 000036F0
0048B1F7   . E8 640EFEFF    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
0048B1FC   . 83C4 20        ADD     ESP, 20
0048B1FF   . 50             PUSH    EAX
0048B200   . E8 A26D0100    CALL    TENSHOU.004A1FA7
0048B205   . 83C4 0C        ADD     ESP, 0C

//----左上のメッセージ続きで変更
0048B208   . 57             PUSH    EDI
0048B209   . 55             PUSH    EBP
0048B20A   . 68 F1360000    PUSH    36F1
0048B20F   . E8 380AFAFF    CALL    TENSHOU.0042BC4C
0048B214   . 83C4 0C        ADD     ESP, 0C

//----左上のメッセージ続きで変更
0048B2AD   . 57             PUSH    EDI
0048B2AE   . 55             PUSH    EBP
0048B2AF   . 68 F5360000    PUSH    36F5
0048B2B4   . E8 9309FAFF    CALL    TENSHOU.0042BC4C
0048B2B9   . 83C4 0C        ADD     ESP, 0C

//----左上のメッセージ続きで変更
0048B2BC   . 57             PUSH    EDI
0048B2BD   . 55             PUSH    EBP
0048B2BE   . 68 F6360000    PUSH    36F6
0048B2C3   . E8 8409FAFF    CALL    TENSHOU.0042BC4C
0048B2C8   . 83C4 0C        ADD     ESP, 0C


// -- 右下のウィンドウを消す？
0048B412   . B9 F0994D00    MOV     ECX, TENSHOU.004D99F0            ;  ASCII "K"
0048B417   . E8 1C20FFFF    CALL    TENSHOU.0047D438
0048B41C   . 8BCB           MOV     ECX, EBX
0048B41E   . E8 1520FFFF    CALL    TENSHOU.0047D438

// -- 左上ののウィンドウを消す？
MOV     ECX, TENSHOU.004D99F0
0048B0CF   . 8BCB           MOV     ECX, EBX
0048B0D1   . E8 6223FFFF    CALL    TENSHOU.0047D438
0048B0D6   . E8 1382F9FF    CALL    TENSHOU.004232EE
*/

/*
0048B174   . 83C4 20        ADD     ESP, 20

0048B177   . 50             PUSH    EAX							← EAXは文字列へのポインタ
0048B178   . E8 2A6E0100    CALL    TENSHOU.004A1FA7
0048B17D   . 83C4 0C        ADD     ESP, 0C

*/


//---------------------左上に引き出しを出す-------------------------
int iBushouIDOfNormalFukidashiMessage = -1;
char pszMessageOfNormalFukidashiMessage[512] = "";
int iTargetMemoryAddressOfOfNormalFukidashiMessage = NULL;

NB6FUKIDASHIRELATEPERSONS nb6fukidashirelatepersons_for_push; // 手動でプッシュする用のPerson関係

static void ReWriteNormalFukidashiMessage() {
	// MessageN6PPatchを通過するためのお膳立て

	// 外部メッセージ置き換えをScenarioModにて行う
	if ( p_snOnRewriteFukidashiMessage ) {
		char szModMsgBuf[512] = "";

		nb6fukidashirelatepersons_for_push.i1stPersonID = iBushouIDOfNormalFukidashiMessage+1;
		nb6fukidashirelatepersons_for_push.i2ndPersonID = 0xFFFF;
		nb6fukidashirelatepersons_for_push.i3rdPersonID = 0xFFFF;

		// ここで名前を作り出す。
		char name[256] = "";

		if ( nb6fukidashirelatepersons_for_push.i1stPersonID <= 532 ) {
			strcat( name, nb6bushouname[nb6fukidashirelatepersons_for_push.i1stPersonID-1].familyname);
			strcat( name, nb6bushouname[nb6fukidashirelatepersons_for_push.i1stPersonID-1].fastname);
		}

		// 武将名, 現在の書き換え対象となっているメッセージ, 関連人物情報
		((PFNSNONREWRITEFUKIDASHIMESSAGE) p_snOnRewriteFukidashiMessage)(szModMsgBuf, name, pszMessageOfNormalFukidashiMessage, &nb6fukidashirelatepersons_for_push);

		if ( strlen(szModMsgBuf) > 1 ) {
			strcpy( pszMessageOfNormalFukidashiMessage, szModMsgBuf );
		}

	}
	// 引数で渡って来たものをコピー
	strcpy( (char *)iTargetMemoryAddressOfOfNormalFukidashiMessage, pszMessageOfNormalFukidashiMessage);

}

int pCallFromFuncFukidashiMessage1 = 0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pCallFromFuncFukidashiMessage2 = 0x4A1FA7; // 元々TENSHOU.EXE内にあったCALL先
int iBushouIDOfLeftNormalFukidashiMessage = -1;
void FuncLeftNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfLeftNormalFukidashiMessage
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push eax
		push edi
		push edi
		push edi
		push edi
		push edi
		push edi
		push ebp
		push 0x36e8
		call pCallFromFuncFukidashiMessage1                 ; \TENSHOU.0046C060
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eaxが書き換え先のアドレス
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		push eax   // eax はメッセージの格納先のメモリアドレス。例のCMP.getLocationほにゃららと同じ場所で、上の4d7c90のこと。

		call pCallFromFuncFukidashiMessage2

		add   esp, 0x0C
	}
}


// 
void FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfLeftNormalFukidashiMessage = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfLeftNormalFukidashiMessage;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

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
	FuncLeftNormalFukidashiMessageAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

}



//---------------------右下に引き出しを出す-------------------------
int pCallFromFuncFukidashiMessage3 = 0x4A1FFD; // 元々TENSHOU.EXE内にあったCALL先
int iBushouIDOfRightNormalFukidashiMessage = -1;
void FuncRightNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfRightNormalFukidashiMessage
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push eax
		push edi
		push edi
		push edi
		push edi
		push edi
		push edi
		push ebp
		push 0x36eb
		call pCallFromFuncFukidashiMessage1                 ; \TENSHOU.0046C060
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eaxが書き換え先のアドレス
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		push eax   // eax はメッセージの格納先のメモリアドレス。例のCMP.getLocationほにゃららと同じ場所で、上の4d7c90のこと。

		call pCallFromFuncFukidashiMessage3

		add   esp, 0x0C
	}

}


// 
void FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfRightNormalFukidashiMessage = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfRightNormalFukidashiMessage;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);
	
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
	FuncRightNormalFukidashiMessageAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}


/*
//---------------------真ん中に吹き出しを出す-------------------------
int pCallFromFuncFukidashiMessage4 = 0x4A2052; // 元々TENSHOU.EXE内にあったCALL先
int iBushouIDOfCenterNormalFukidashiMessage = -1;
void FuncCenterNormalFukidashiMessageAsm() {

	__asm {
		mov eax, iBushouIDOfCenterNormalFukidashiMessage
		mov esi, 0
		mov edi, 0xFFFF
		mov ebp, 0x4d7c90
		push    esi         
		push    esi         
		push	esi // push    ebx         
		push    esi         
		push    esi         
		push    esi         
		push    ebp    
		push    0x13C8        
		call    pCallFromFuncFukidashiMessage1
		add     esp, 0x20
	}

	__asm {
		mov iTargetMemoryAddressOfOfNormalFukidashiMessage, eax // eaxが書き換え先のアドレス
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		mov esi, 0
		mov edi, 0xFFFF
		push edi
		push edi
		push edi
		push 2
		push esi
		push esi
		push eax   // eax はメッセージの格納先のメモリアドレス。例のCMP.getLocationほにゃららと同じ場所で、上の4d7c90のこと。

		push iBushouIDOfCenterNormalFukidashiMessage
		call pCallFromFuncFukidashiMessage4

		add  esp, 0x20

	}

}


// 
void FuncCenterNormalFukidashiMessage(int iBushouID, char *szMessage) {
	iBushouIDOfCenterNormalFukidashiMessage = iBushouID+1;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);
	
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
	FuncCenterNormalFukidashiMessageAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}
*/



/*
0048B208   . 57             PUSH    EDI							← 0 固定
0048B209   . 55             PUSH    EBP							← 4d7c90 固定
0048B20A   . 68 F1360000    PUSH    36F1
0048B20F   . E8 380AFAFF    CALL    TENSHOU.0042BC4C			← 呼び出し関数
0048B214   . 83C4 0C        ADD     ESP, 0C
*/

int pCallFromFuncLeftFukidashiUpdate1 = 0x42BC4C; // 元々TENSHOU.EXE内にあったCALL先
int iBushouIDOfLeftNormalFukidashiUpdate = -1;
int isOnLeftNormalFukidashiUpdating = FALSE; // メッセージ枠の「Update」関数において、今回外部から文字列を変更することのシグナル。
void FuncLeftNormalFukidashiUpdateAsm() {

	__asm {
		mov eax, iBushouIDOfLeftNormalFukidashiUpdate
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push ebp
		push 0x36F1
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		call pCallFromFuncLeftFukidashiUpdate1                 ; \TENSHOU.0042BC4C
		add     esp, 0x0C
	}
}


// 
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	iBushouIDOfLeftNormalFukidashiUpdate = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfLeftNormalFukidashiUpdate;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

	isOnLeftNormalFukidashiUpdating = TRUE;

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
	FuncLeftNormalFukidashiUpdateAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	isOnLeftNormalFukidashiUpdating = FALSE;
}




/*
00411B3E  |. 53             PUSH    EBX
00411B3F  |. 57             PUSH    EDI
00411B40  |. 68 31140000    PUSH    1431
00411B45  |. E8 34A10100    CALL    TENSHOU.0042BC7E
00411B4A  |. 83C4 0C        ADD     ESP, 0C

*/

int pCallFromFuncRightFukidashiUpdate1 = 0x42BC7E; // 元々TENSHOU.EXE内にあったCALL先
int iBushouIDOfRightNormalFukidashiUpdate = -1;
int isOnRightNormalFukidashiUpdating = FALSE;
void FuncRightNormalFukidashiUpdateAsm() {

	__asm {
		mov eax, iBushouIDOfRightNormalFukidashiUpdate
		mov edi, 0
		mov ebp, 0x4d7c90
		push edi
		push ebp
		push 0x1431
	}

	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esp
		push esi
		push edi
	}

	ReWriteNormalFukidashiMessage();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	__asm {
		call pCallFromFuncRightFukidashiUpdate1                 ; \TENSHOU.0042BC7E
		add     esp, 0x0C
	}
}


// 
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	iBushouIDOfRightNormalFukidashiUpdate = iBushouID+1;
	iBushouIDOfNormalFukidashiMessage = iBushouIDOfRightNormalFukidashiUpdate;
	strcpy( pszMessageOfNormalFukidashiMessage, szMessage);

	isOnRightNormalFukidashiUpdating = TRUE;

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
	FuncRightNormalFukidashiUpdateAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}

	isOnRightNormalFukidashiUpdating = FALSE;
}





//---------------------吹き出し更新-------------------------

// 吹き出しの更新
/*
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiDelete();
	FuncRightNormalFukidashiMessage(iBushouID, szMessage);
}
*/
/*
// 吹き出しの更新
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiDelete();
	FuncLeftNormalFukidashiMessage(iBushouID, szMessage);
}
*/





//---------------------左上に引き出しを消す-------------------------
int pCallFromFuncFukidashiDelete = 0x47D438; // 元々TENSHOU.EXE内にあったCALL先


void FuncLeftNormalFukidashiDeleteAsm() {

	__asm {
		mov ecx, 0x4D9AA8
		call pCallFromFuncFukidashiDelete
	}

}
void FuncRightNormalFukidashiDeleteAsm() {

	__asm {
		mov ecx, 0x4D99F0
		call pCallFromFuncFukidashiDelete
	}

}


// 
void FuncLeftNormalFukidashiDelete() {
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
	FuncLeftNormalFukidashiDeleteAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}

void FuncRightNormalFukidashiDelete() {
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
	FuncRightNormalFukidashiDeleteAsm();

	__asm {
		pop edi
		pop esi
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}




// 外部参照用
void WINAPI Extern_FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiMessage(iBushouID, szMessage);
}
void WINAPI Extern_FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiMessage(iBushouID, szMessage);
}
void WINAPI Extern_FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncLeftNormalFukidashiUpdate(iBushouID, szMessage);
}
void WINAPI Extern_FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage) {
	FuncRightNormalFukidashiUpdate(iBushouID, szMessage);
}

void WINAPI Extern_FuncLeftNormalFukidashiDelete() {
	FuncLeftNormalFukidashiDelete();
}
void WINAPI Extern_FuncRightNormalFukidashiDelete() {
	FuncRightNormalFukidashiDelete();
}





/*
 * 噴出しの左上のメッセージが更新された際のアドレス
 */
int pointerOfFukidashiLeftUpdateMsgPointerExecute = NULL;
void OnTenshouExeFukidashiLeftUpdateMsgPointerExecute() {
	
	if (isOnLeftNormalFukidashiUpdating) {
		// 引数で渡って来たものをコピー
		strcpy( (char *)pointerOfFukidashiLeftUpdateMsgPointerExecute, pszMessageOfNormalFukidashiMessage);

	}
}


/*
0042BC5C  |. FF7424 24      PUSH    DWORD PTR SS:[ESP+24]            ; |Arg1
0042BC60  |. E8 FB030400    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeFukidashiLeftUpdateMsgPointer と書き換えてやる実際の処理
0042BC65  |. 83C4 20        ADD     ESP, 20

*/
int pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer	 =0x42BC60; // 関数はこのアドレスから、OnTenshouExeFukidashiLeftUpdateMsgPointerへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiLeftUpdateMsgPointer =0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFukidashiLeftUpdateMsgPointer  =0x42BC65; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiLeftUpdateMsgPointer() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiLeftUpdateMsgPointer

		// この時、EAXの値が、左上噴出しメッセージで更新表示する際のメッセージ文字列へのポインタの場所
		mov pointerOfFukidashiLeftUpdateMsgPointerExecute, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFukidashiLeftUpdateMsgPointerExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiLeftUpdateMsgPointer
	}
}

/*
*/
char cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiLeftUpdateMsgPointer() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiLeftUpdateMsgPointer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiLeftUpdateMsgPointer  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiLeftUpdateMsgPointer), cmdOnTenshouExeJumpFromFukidashiLeftUpdateMsgPointer, 5, NULL); //5バイトのみ書き込む
}








//-----------------------------------------------------------------------------------------------------
/*
 * 噴出しの右下のメッセージが更新された際のアドレス
 */
int pointerOfFukidashiRightUpdateMsgPointerExecute = NULL;
void OnTenshouExeFukidashiRightUpdateMsgPointerExecute() {
	if (isOnRightNormalFukidashiUpdating) {
		// 引数で渡って来たものをコピー
		strcpy( (char *)pointerOfFukidashiRightUpdateMsgPointerExecute, pszMessageOfNormalFukidashiMessage);
	}
}


/*
0042BC8E  |. FF7424 24      PUSH    DWORD PTR SS:[ESP+24]            ; |Arg1
0042BC92  |. E8 C9030400    CALL    TENSHOU.0046C060                 ; \TENSHOU.0046C060
				└ここを JMP TSMod.OnTSExeFukidashiRightUpdateMsgPointer と書き換えてやる実際の処理
0042BC97  |. 83C4 20        ADD     ESP, 20

*/
int pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer	   = 0x42BC92; // 関数はこのアドレスから、OnTenshouExeFukidashiRightUpdateMsgPointerへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeFukidashiRightUpdateMsgPointer =0x46C060; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeFukidashiRightUpdateMsgPointer  =0x42BC97; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeFukidashiRightUpdateMsgPointer() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeFukidashiRightUpdateMsgPointer

		// この時、EAXの値が、右下噴出しメッセージで更新表示する際のメッセージ文字列へのポインタの場所
		mov pointerOfFukidashiRightUpdateMsgPointerExecute, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	OnTenshouExeFukidashiRightUpdateMsgPointerExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeFukidashiRightUpdateMsgPointer
	}
}

/*
*/
char cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeFukidashiRightUpdateMsgPointer() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeFukidashiRightUpdateMsgPointer;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeFukidashiRightUpdateMsgPointer  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeFukidashiRightUpdateMsgPointer), cmdOnTenshouExeJumpFromFukidashiRightUpdateMsgPointer, 5, NULL); //5バイトのみ書き込む
}

