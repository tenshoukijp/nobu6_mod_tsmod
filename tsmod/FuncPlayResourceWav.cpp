#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
// #include "SDKXAudio.h"


/*
004A20A6  |. 50             PUSH    EAX                              ; /Arg4 => 00000000
004A20A7  |. 50             PUSH    EAX                              ; |Arg3 => 00000000
004A20A8  |. 50             PUSH    EAX                              ; |Arg2 => 00000000
004A20A9  |. FF75 24        PUSH    DWORD PTR SS:[EBP+24]            ; |Arg1
004A20AC  |. E8 C4B1FEFF    CALL    TENSHOU.0048D275                 ; \TENSHOU.0048D275
004A20B1  |. 83C4 10        ADD     ESP, 10
*/

/*
0048D275  /$ 55             PUSH    EBP
0048D276  |. 8BEC           MOV     EBP, ESP
0048D278  |. 83EC 04        SUB     ESP, 4
0048D27B  |. 833D F0294D00 >CMP     DWORD PTR DS:[4D29F0], 0
0048D282  |. 56             PUSH    ESI
0048D283  |. 57             PUSH    EDI
0048D284  |. 74 57          JE      SHORT TENSHOU.0048D2DD
0048D286  |. 6A 01          PUSH    1
0048D288  |. E8 149DFDFF    CALL    TENSHOU.00466FA1
0048D28D  |. 83C4 04        ADD     ESP, 4
0048D290  |. 85C0           TEST    EAX, EAX
0048D292  |. 74 49          JE      SHORT TENSHOU.0048D2DD
0048D294  |. 6A 06          PUSH    6
0048D296  |. BF 00000000    MOV     EDI, 0
0048D29B  |. E8 43000000    CALL    TENSHOU.0048D2E3
0048D2A0  |. 83C4 04        ADD     ESP, 4
0048D2A3  |. 8B75 0C        MOV     ESI, DWORD PTR SS:[EBP+C]
0048D2A6  |. 85F6           TEST    ESI, ESI
0048D2A8  |. 74 03          JE      SHORT TENSHOU.0048D2AD
0048D2AA  |. 8B7D 10        MOV     EDI, DWORD PTR SS:[EBP+10]
0048D2AD  |> 8B45 08        MOV     EAX, DWORD PTR SS:[EBP+8]
0048D2B0  |. 83F8 21        CMP     EAX, 21
0048D2B3  |. 73 28          JNB     SHORT TENSHOU.0048D2DD
0048D2B5  |. 8D4D FC        LEA     ECX, DWORD PTR SS:[EBP-4]
0048D2B8  |. 51             PUSH    ECX
0048D2B9  |. FF3485 F8294D0>PUSH    DWORD PTR DS:[EAX*4+4D29F8]
0048D2C0  |. E8 912D0200    CALL    <JMP.&N6PWAV.LoadWaveResource>
0048D2C5  |. 83C4 08        ADD     ESP, 8
0048D2C8  |. 85C0           TEST    EAX, EAX
0048D2CA  |. 74 11          JE      SHORT TENSHOU.0048D2DD
0048D2CC  |. FF75 14        PUSH    DWORD PTR SS:[EBP+14]
0048D2CF  |. 57             PUSH    EDI
0048D2D0  |. FF75 FC        PUSH    DWORD PTR SS:[EBP-4]
0048D2D3  |. 50             PUSH    EAX
0048D2D4  |. 56             PUSH    ESI
0048D2D5  |. E8 282D0200    CALL    <JMP.&N6PAudio.dsndPlayStart>
0048D2DA  |. 83C4 14        ADD     ESP, 14
0048D2DD  |> 5F             POP     EDI
0048D2DE  |. 5E             POP     ESI
0048D2DF  |. 8BE5           MOV     ESP, EBP
0048D2E1  |. 5D             POP     EBP
0048D2E2  \. C3             RETN
*/


int iResIDInFuncPlayResourceWav = 1;
short iResIDInFuncPlayResourceDummy = 0;

//---------------------音の再生-------------------------
int pCallFromFuncPlayResourceWavAsm = 0x48D275; // 元々TENSHOU.EXE内にあったCALL先
void FuncPlayResourceWavAsm() {
	__asm {
		// 引数1～3 2バイトずつ
		push iResIDInFuncPlayResourceDummy;
		push iResIDInFuncPlayResourceDummy;
		push iResIDInFuncPlayResourceDummy;
		// 引数4 音の番号(配列上のINDEXと言ったほうが良い)(INT=4バイト)
		push iResIDInFuncPlayResourceWav;
		call pCallFromFuncPlayResourceWavAsm

		// スタックがずれ込んだ分
		add  esp, 0x10
	}

}

// n6pwav.dll内にあるリソースを再生する。
// iResIDはリソース内のＩＤではなく、何番目の配列に入っているか？　である。
// 現在、実行ステップとしては正しく動いているように思えるが、0番(=601)しか音が鳴らない。何か勘違いしているかな？
void FuncPlayResourceWav(int iResID) {
	iResIDInFuncPlayResourceWav = iResID;

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
	FuncPlayResourceWavAsm();

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
プログラムID(=iResID)	リソース名	使われ所
0	601	ターン開始 
1	602	成功 
2	603	失敗 
3	604	豊作 
4	605	凶作 
5	606	台風 
6	607	地震 
7	608	斬首 
8	609	通常戦闘、メイン画面での戦争時、剣技披露の太刀
9	610	突撃時、何度も鳴る。708と同時になる。
11	611	大砲発射
12	612	大砲ピューン
13	613	落城・着弾 
10	614	鉄砲 
15	615	門叩き、桶狭間のイベント時にも利用 
16	616	開門 
14	617	壁越え（ｼｭｼｭｼｭｼｭ） 
17	618	一喝 
18	619	挑発・混乱 
19	620	滅亡・悲しいイベント 
20	701	戦争突入 
21	702	謁見（外交） 
22	703	朝廷 
23	704	夏
24	705	秋
25	706	冬
26	707	春
27	708	突撃時の継続音 610と同時に鳴る。 
28	709	勝ち鬨（本丸占拠） 
29	710	大砲着弾・鉄砲打たれ時 
30	711	雨 
31	712	雷（台風時） 
32	713	雪 
*/
// 普通にAPIで音の再生
BOOL FuncPlaySoundWav( int iResID , BOOL isLoop ) {
	// 環境音の設定が有効であれば…  そしてフォアグランドウィンドウならば…
	if ( !isSEValid() ) {
		return FALSE;
	}
	
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return FALSE;
	}
	if ( isLoop ) {
		isLoop = SND_LOOP;
	}
	// 音を鳴らす。																		┌音は非同期  ┌リソースより
	return PlaySound( MAKEINTRESOURCE(iResID), CommonGlobalHandle::hResourceWavMod, SND_ASYNC | SND_RESOURCE | isLoop );
}

BOOL FuncPlaySoundSyncWav( int iResID ) {
	// 環境音の設定が有効であれば…  そしてフォアグランドウィンドウならば…
	if ( !isSEValid() ) {
		return FALSE;
	}
	
	// フォアグランドではない上、かつ、メッセージスピードが一番左(超早い）ならば、サウンドは鳴らさない
	if ( GetForegroundWindow()!=CommonGlobalHandle::tenshouWND && nb6settings.messagespd == 0) {
		return FALSE;
	}

	// 音を鳴らす。																		┌音は非同期  ┌リソースより
	return PlaySound( MAKEINTRESOURCE(iResID), CommonGlobalHandle::hResourceWavMod, SND_SYNC | SND_RESOURCE );
}


BOOL FuncStopSoundWav( int iResID ) {
	// 音を止める。													  ┌リソースより
	return PlaySound( NULL, CommonGlobalHandle::hResourceWavMod, SND_ASYNC | SND_RESOURCE );
}



