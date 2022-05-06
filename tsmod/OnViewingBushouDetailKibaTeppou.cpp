#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"
#include <stdlib.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"



//--------------------------------以下、騎馬鉄砲を持つかどうか判定--------------------------------
/*
 * 武将詳細の騎馬鉄砲が表示される時
 */


extern int iViewingJobInBushouID; // 武将詳細情報表示で、表示しようとしている武将ＩＤが入っている

/*
 * 騎馬鉄砲隊欄を表示する必要があるかどうかの判定。
 */
void OnTenshouExeBushouDetailKibaTeppouJudgeExecute() {
}


/*
00493D98  |. E8 BA64F7FF    CALL    TENSHOU.0040A257
							└ここを JMP TSMod.OnTSExeBushouDetailKibaTeppouJudge と書き換えてやる実際の処理
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX					←このEAXが1なら騎馬鉄砲を持つということ。
00493DA2  |. 74 24          JE      SHORT TENSHOU.00493DC8

*/
int pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge	=0x493D98; // 関数はこのアドレスから、OnTenshouExeBushouDetailKibaTeppouJudgeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppouJudge=0x40A257; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppouJudge	=0x493DA2; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouDetailKibaTeppouJudge() {
	// スタックにためておく
	__asm {
		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppouJudge

		add esp, 4

		// 騎馬鉄砲隊欄は必ず表示する必要があるということにしてしまう。
		mov eax, 1

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
	OnTenshouExeBushouDetailKibaTeppouJudgeExecute();

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

		// 騎馬鉄砲隊欄を表示する必要があるかどうかの判定
		test eax, eax 

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppouJudge
	}
}





char cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppouJudge() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouDetailKibaTeppouJudge;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouDetailKibaTeppouJudge  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppouJudge), cmdOnTenshouExeJumpFromBushouDetailKibaTeppouJudge, 5, NULL); //5バイトのみ書き込む
}







//--------------------------------以下、実際に表示する--------------------------------

char szSpecialButaiNon[11] = "";
char szSpecialButaiAnkokukishidan[11] = "фхцч"; // ←ロスローリアン
char szSpecialButaiYumi[11] = "弓";
char szSpecialButaiYari[11] = "槍";
char szSpecialButaiOno[11] = "斧";
char szSpecialButaiMuchi[11] = "鞭";
char szSpecialButaiTuchi[11] = "槌";
char szSpecialButaiTue[11] = "杖";
char szSpecialButaiKenKitetsu[11] = "剣･騎鉄";
char szSpecialButaiYumiKitetsu[11] = "弓･騎鉄";
char szSpecialButaiYariKitetsu[11] = "槍･騎鉄";
char szSpecialButaiMuchiKitetsu[11] = "鞭･騎鉄";
char szSpecialButaiOnoKitetsu[11] = "斧･騎鉄";
char szSpecialButaiTueKitetsu[11] = "杖･騎鉄";
char szSpecialButaiYumiYari[11] = "弓･槍";
char szSpecialButaiYariOno[11] = "槍･斧";
char szSpecialButaiTuchiOno[11] = "槌･斧";
char szSpecialButaiTueYumi[11] = "杖･弓";
char szSpecialButaiMuchiYumi[11] = "鞭･弓";
char szSpecialButaiYumiYariKitetsu[11] = "弓･槍･騎鉄";
char szSpecialButaiYariOnoKitetsu[11] = "槍･斧･騎鉄";
char szSpecialButaiTuchiOnoKitetsu[11] = "槌･斧･騎鉄";
char szSpecialButaiTueYumiKitetsu[11] = "杖･弓･騎鉄";
char szSpecialButaiMuchiYumiKitetsu[11] = "鞭･弓･騎鉄";

int pAddressSpecialButai = NULL;

char szDisplayString[15] = "";

int iViewingSpecialButaiInXPosition = 0x98;

extern int isTenShougiModeDialog; // 天将棋のダイアログ表示周辺の時だけ、特殊兵科の代わりに「段位」となる。
extern char *getSpecialDaniStringOfBushou(int iBushouID); // 指定の

char* getSpecialHeitaiStringOfBushou(int iBushouID) {

	// 将棋の将棋相手を選ぶ瞬間だけは「段位」の表示へと切り替える。
	if ( isTenShougiModeDialog ) {
		return getSpecialDaniStringOfBushou(iBushouID);
	}


	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int is_horsegun = nb6bushouref[iBushouID].horsegun;			// 騎馬鉄砲属性
		int value_yumi = nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi;	// 弓属性
		int value_yari = nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari;	// 槍属性												
		int value_ono = nb6savedata_ex.bushou_ex[iBushouID].aptitudeono;	// 斧属性												
		int value_muchi = nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi;	// 鞭属性												
		int value_tuchi = nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi;	// 槌属性												
		int value_tue = nb6savedata_ex.bushou_ex[iBushouID].aptitudetue;	// 杖属性												
		int value_ken = nb6savedata_ex.bushou_ex[iBushouID].aptitudeken;	// 剣属性												

		char *szTempName = szSpecialButaiNon;

		// 弓･槍･騎馬鉄砲 全部使える
		if (  value_yumi >= 3  && value_yari >= 3&& is_horsegun ) {
			szTempName = szSpecialButaiYumiYariKitetsu;

		// 杖･弓･騎馬鉄砲が使える
		} else if ( value_tue >= 3 && value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTueYumiKitetsu;

		// 鞭･弓･騎馬鉄砲が使える
		} else if ( value_muchi >= 3 && value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiYumiKitetsu;

		// 槍･斧･騎馬鉄砲が使える
		} else if ( value_yari >= 3 && value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYariOnoKitetsu;

		// 槌･斧･騎馬鉄砲が使える
		} else if ( value_tuchi >= 3 && value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTuchiOnoKitetsu;

		// 剣･騎馬鉄砲が使える(事実上タルタロス専用)
		} else if ( value_ken >= 5 && is_horsegun ) {
			szTempName = szSpecialButaiKenKitetsu;

		// 槍･騎馬鉄砲が使える
		} else if ( value_yari >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYariKitetsu;

		// 鞭･騎馬鉄砲が使える
		} else if ( value_muchi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiKitetsu;

		// 弓･騎馬鉄砲が使える
		} else if ( value_yumi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiYumiKitetsu;

		// 鞭･騎馬鉄砲が使える
		} else if ( value_muchi >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiMuchiKitetsu;

		// 斧･騎馬鉄砲が使える
		} else if ( value_ono >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiOnoKitetsu;

		// 杖･騎馬鉄砲が使える
		} else if ( value_tue >= 3 && is_horsegun ) {
			szTempName = szSpecialButaiTueKitetsu;

		// 弓･槍が使える
		} else if ( value_yari >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiYumiYari;

		// 槌･斧が使える
		} else if ( value_tuchi >= 3 && value_ono >= 3 ) {
			szTempName = szSpecialButaiTuchiOno;

		// 槍･斧が使える
		} else if ( value_yari >= 3 && value_ono >= 3 ) {
			szTempName = szSpecialButaiYariOno;

		// 杖･弓が使える
		} else if ( value_tue >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiTueYumi;

		// 鞭･弓が使える
		} else if ( value_muchi >= 3 && value_yumi >= 3 ) {
			szTempName = szSpecialButaiMuchiYumi;

		// 弓のみ
		} else if ( value_yumi >= 3 ) {
			szTempName = szSpecialButaiYumi;

		// 槍のみ
		} else if ( value_yari >= 3 ) {
			szTempName = szSpecialButaiYari;

		// 斧のみ
		} else if ( value_ono >= 3 ) {
			szTempName = szSpecialButaiOno;

		// 鞭のみ
		} else if ( value_muchi >= 3 ) {
			szTempName = szSpecialButaiMuchi;

		// 杖のみ
		} else if ( value_tue >= 3 ) {
			szTempName = szSpecialButaiTue;

		// 騎馬鉄砲のみ使える
		} else if ( is_horsegun ) {
			szTempName = (char *)0x4CFF0C; // 騎馬鉄砲隊の文字列へのポインタ

		// 何も使えない
		} else {
			szTempName = szSpecialButaiNon;
		}

		// ScenarioModが有効であれば、
		// ScenarioModでカスタムな特殊兵態名を設定している場合、それを設定する。
		char szTempSpecialName[20] = "";
		if ( p_snOnViewingSpecialFormName ) {
			// 安全を見て大き目に
			bool isReWrite = ((PFNSNONVIEWINGSPECIALFORMNAME)p_snOnViewingSpecialFormName)( szTempSpecialName, iBushouID+1 );
			if ( isReWrite ) {
				szTempName = szTempSpecialName;
			}
		}
		
		return szTempName;
	}

	return NULL;
}


void OnTenshouExeBushouDetailKibaTeppouExecute() {

	iViewingSpecialButaiInXPosition = 0x98; // デフォルト値で必ず初期化

	// 騎馬鉄砲の情報
	int iBushouID = iViewingJobInBushouID-1;

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		char *szTempName = getSpecialHeitaiStringOfBushou(iBushouID);

		// 該当の特殊部隊名が２文字より短い場合、文字列を長くしてやる必要がある。
		if ( strlen(szTempName) < 4 ) {
			string s = string("  ")+szTempName+"  ";
			strcpy( szDisplayString, s.c_str() );
		} else {
			strcpy( szDisplayString, szTempName); // 全角で４文字がＭＡＸ
		}
		iViewingSpecialButaiInXPosition = 0xA8 - (strlen(szDisplayString)-4)*4;

		pAddressSpecialButai = (int)szDisplayString;

	// 武将がなぜか範囲に入っていない場合は(ありえないが)、とりあえず、空白状態を渡しておく
	} else {
		// 
		pAddressSpecialButai = (int)szSpecialButaiNon;
	}
}


/*
00493DAE     68 98000000    PUSH    98
				└ここを JMP TSMod.OnTSExeBushouDetailKibaTeppou と書き換えてやる実際の処理
00493DB3  |. 8BCF           MOV     ECX, EDI
00493DB5  |. E8 CED6F6FF    CALL    TENSHOU.00401488
00493DBA  |. 68 0CFF4C00    PUSH    TENSHOU.004CFF0C                 ;  ASCII "騎馬鉄砲"   ← ここは騎馬鉄砲のアドレス。tenshou.exe内で決め打ち埋め込みされている。
00493DBF  |. 57             PUSH    EDI
*/
int pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou		=0x493DAE; // 関数はこのアドレスから、OnTenshouExeBushouDetailKibaTeppouへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppou =0x401488; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppou	=0x493DBF; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeBushouDetailKibaTeppou() {
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
	OnTenshouExeBushouDetailKibaTeppouExecute();

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

		push iViewingSpecialButaiInXPosition

		// 元々TENSHOU.EXE内にあったものをここで
		mov ecx, edi

		call pTenshouExeJumpCallFromToOnTenshouExeBushouDetailKibaTeppou

		// 騎馬鉄砲隊の欄に表示すべき文字列へのポインタは
		push pAddressSpecialButai 

		jmp pTenshouExeReturnLblFromOnTenshouExeBushouDetailKibaTeppou
	}
}

/*
00446A7B  |. 57             PUSH    EDI
00446A7C  |. E8 3159FCFF    CALL    TENSHOU.0040C3B2
				└ここを JMP TSMod.OnTSExeBushouDetailKibaTeppou と書き換えてやる実際の処理
00446A81  |. 83C4 04        ADD     ESP, 4
00446A84  |. 85C0           TEST    EAX, EAX
00446A86  |. 74 45          JE      SHORT TENSHOU.00446ACD
*/
char cmdOnTenshouExeJumpFromBushouDetailKibaTeppou[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeBushouDetailKibaTeppou() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeBushouDetailKibaTeppou;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeBushouDetailKibaTeppou  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromBushouDetailKibaTeppou+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeBushouDetailKibaTeppou), cmdOnTenshouExeJumpFromBushouDetailKibaTeppou, 5, NULL); //5バイトのみ書き込む
}




