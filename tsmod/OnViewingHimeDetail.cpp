#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "VariableNameRegistAndJudge.h"


/*
 姫の詳細ページを表示した際の処理
 */


/*
該当がターゲットの姫の番号

00494480  |. 66:8B5E 70     MOV     BX, WORD PTR DS:[ESI+70] // ここのBXが姫側の武将番号
00494484  |. 53             PUSH    EBX
00494485  |. E8 F35AF7FF    CALL    TENSHOU.00409F7D
0049448A  |. 83C4 04        ADD     ESP, 4



ここが娘の時、EBPのBPが
00494530  |. 68 102F4D00    PUSH    TENSHOU.004D2F10                 ;  ASCII "%sの娘"
00494535  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "ｰVK"
0049453A  |. E8 71A00100    CALL    TENSHOU.004AE5B0
0049453F  |. 83C4 0C        ADD     ESP, 0C

結婚していないという判定も入れる。
*/

int iHimeBushouIDOnViewingHimeDtail = -1;
void OnTenshouExeViewingHimeDetailExecute() {
	iHimeBushouIDOnViewingHimeDtail = iHimeBushouIDOnViewingHimeDtail & 0xFFFF; // BXレジスタ相当にする。
}


/*
00494485  |. E8 F35AF7FF    CALL    TENSHOU.00409F7D
				└ここを JMP TSMod.OnTSExeViewingHimeDetail と書き換えてやる実際の処理
0049448A  |. 83C4 04        ADD     ESP, 4
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail		=0x494485; // 関数はこのアドレスから、OnTenshouExeViewingHimeDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingHimeDetail  =0x409F7D; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetail	=0x49448A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetail() {
	// スタックにためておく
	__asm {
		mov iHimeBushouIDOnViewingHimeDtail, ebx; // 対象となる姫の武将番号の確保

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeViewingHimeDetail

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
	OnTenshouExeViewingHimeDetailExecute();

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

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetail
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingHimeDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingHimeDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetail), cmdOnTenshouExeJumpFromViewingHimeDetail, 5, NULL); //5バイトのみ書き込む
}


// -----------------------------------------------------------------------------------------------


// 娘の時に、娘の文字列を置き換える

extern bool isHimeTartareAndVersalia(int iBushouID, int iHimeBushouID);


int iOyaBushouIDOnViewingHimeDetailMusume = -1;
int iPushAddressOnViewingHimeDetailMusume = 0x4D2F10; // ;  ASCII "%sの娘"のアドレス
char szPushAdressOnViewingHimeDetailYoujo[] = "%sの養女";
char szPushAdressOnViewingHimeDetailMekake[] = "%sの愛妾";

void OnTenshouExeViewingHimeDetailMusumeExecute() {
	iOyaBushouIDOnViewingHimeDetailMusume = iOyaBushouIDOnViewingHimeDetailMusume & 0xFFFF; // BXレジスタ相当にする。

	int iBushouID = iOyaBushouIDOnViewingHimeDetailMusume-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// まずデフォルトを入れておく
	iPushAddressOnViewingHimeDetailMusume = 0x4D2F10;

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {

		// ランスロットとベルサリア
		if ( isHimeTartareAndVersalia(iBushouID, iHimeBushouID) ) {
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailMekake);
		}

		// 以下養女の要件を満たすもの
		// 茶々 と 長政以外
		else if ( nb6bushouname[iHimeBushouID].number == 366 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		// 初 と 長政以外
		}
		else if ( nb6bushouname[iHimeBushouID].number == 367 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// 茶々 と 長政以外
		else if ( nb6bushouname[iHimeBushouID].number == 368 && nb6bushouname[iBushouID].number != 31 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// 前田豪 と 利家以外
		else if ( nb6bushouname[iHimeBushouID].number == 378 && nb6bushouname[iBushouID].number != 2 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
		// 血筋が異なる
		else if ( nb6bushouref[iHimeBushouID].blood != nb6bushouref[iBushouID].blood ) {
			iPushAddressOnViewingHimeDetailMusume = int(szPushAdressOnViewingHimeDetailYoujo);
		}
	}
	// よくわからん奴は「娘」のまま
}


/*
EBPのBPが親の番号
00494530  |. 68 102F4D00    PUSH    TENSHOU.004D2F10                 ;  ASCII "%sの娘"
00494535  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "ｰVK"
				└ここを JMP TSMod.OnTSExeViewingHimeDetailMusume と書き換えてやる実際の処理
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume	=0x494530; // 関数はこのアドレスから、OnTenshouExeViewingHimeDetailMusumeへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailMusume	=0x494535; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailMusume() {
	// スタックにためておく
	__asm {
		mov iOyaBushouIDOnViewingHimeDetailMusume, EBP // 親の武将番号の確保

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
	OnTenshouExeViewingHimeDetailMusumeExecute();

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

		push iPushAddressOnViewingHimeDetailMusume; // TENSHOU.EXEに元々存在した処理

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailMusume
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailMusume[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingHimeDetailMusume() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingHimeDetailMusume;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailMusume  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailMusume+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailMusume), cmdOnTenshouExeJumpFromViewingHimeDetailMusume, 5, NULL); //5バイトのみ書き込む
}


//---------------------------------------------------------------------------



int iOttoBushouIDOnViewingHimeDetailTsuma = -1;
int iPushAddressOnViewingHimeDetailTsuma = 0x4D2EF8; // ;  ASCII "%sの妻"のアドレス
char szPushAdressOnViewingHimeDetailSeisitsu[] = "%sの正室";
char szPushAdressOnViewingHimeDetailSokusitsu[] = "%sの側室";
char szPushAdressOnViewingHimeDetailKonyakusya[] = "%sの婚約者";
void OnTenshouExeViewingHimeDetailTsumaExecute() {
	iOttoBushouIDOnViewingHimeDetailTsuma = iOttoBushouIDOnViewingHimeDetailTsuma & 0xFFFF; // DIレジスタ相当にする。

	int iBushouID = iOttoBushouIDOnViewingHimeDetailTsuma-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// まずデフォルトを入れておく。
	iPushAddressOnViewingHimeDetailTsuma = 0x4D2EF8;

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		// オズマ・モー・グラシャスとバールゼフォン・V・ラームズ なら「婚約者」とする。
		if ( strcmp( nb6bushouname[iHimeBushouID].familyname, "ёжз") == 0 && strcmp( nb6bushouname[iHimeBushouID].fastname, "ийк") == 0 
			&& strcmp( nb6bushouname[iBushouID].familyname, "") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "") == 0 ) { 
			// 婚約者
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailKonyakusya);

		// 側室の条件を満たすものが思い浮かんだら、ここにその条件を出す
		// 諏訪 湖衣 と 武田信玄
		} else if ( nb6bushouname[iHimeBushouID].number == 372 && nb6bushouname[iBushouID].number == 19 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// 茶々 と 秀吉
		} else if ( nb6bushouname[iHimeBushouID].number == 366 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// 初 と 秀吉
		} else if ( nb6bushouname[iHimeBushouID].number == 367 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// 達子(江) と 秀吉
		} else if ( nb6bushouname[iHimeBushouID].number == 368 && nb6bushouname[iBushouID].number == 5 &&
			IsOriginalRegistedBushou(iHimeBushouID) && IsOriginalRegistedBushou(iBushouID) ) { 
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSokusitsu);

		// その他は正室とする
		} else {
			iPushAddressOnViewingHimeDetailTsuma = int(szPushAdressOnViewingHimeDetailSeisitsu);
		}
	}
	// よくわからん奴は「妻」のまま
}


/*
EDIのDIが親の番号
0049466E  |. 68 F82E4D00    PUSH    TENSHOU.004D2EF8                 ;  ASCII "%sの妻"
00494673  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                 ;  ASCII "ｰVK"
				└ここを JMP TSMod.OnTSExeViewingHimeDetailTsuma と書き換えてやる実際の処理
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma		=0x49466E; // 関数はこのアドレスから、OnTenshouExeViewingHimeDetailTsumaへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailTsuma	=0x494673; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailTsuma() {
	// スタックにためておく
	__asm {
		mov iOttoBushouIDOnViewingHimeDetailTsuma, EDI // 夫の武将番号の確保

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
	OnTenshouExeViewingHimeDetailTsumaExecute();

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

		push iPushAddressOnViewingHimeDetailTsuma; // TENSHOU.EXEに元々存在した処理

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailTsuma
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailTsuma[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingHimeDetailTsuma() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingHimeDetailTsuma;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailTsuma  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailTsuma+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailTsuma), cmdOnTenshouExeJumpFromViewingHimeDetailTsuma, 5, NULL); //5バイトのみ書き込む
}





//--------------------------------------------------------------------------------



//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
/*
004945C4  |. 3BD8           CMP     EBX, EAX
004945C6  |. B8 0C2F4D00    MOV     EAX, TENSHOU.004D2F0C                                     ;  ASCII "姉"
004945CB  |. 77 05          JA      SHORT TENSHOU.004945D2
004945CD  |. B8 082F4D00    MOV     EAX, TENSHOU.004D2F08                                     ;  ASCII "妹"
004945D2  |> 50             PUSH    EAX
*/


int iShiteiBushouIDOnViewingHimeDetailShimai = -1;
int iDefaultAddressOnViewingHimeDetailAne = 0x4D2F0C; // ;  ASCII "姉"のアドレス
int iDefaultAddressOnViewingHimeDetailImouto = 0x4D2F08; // ;  ASCII "妹"のアドレス
int iMovAddressOnViewingHimeDetailShimai = 0x4D2F0C; // ;  姉妹をオーバーライドしたアドレス
char szPushAdressOnViewingHimeDetailGiShi[] = "%sの義姉";
char szPushAdressOnViewingHimeDetailGimai[] = "%sの義妹";
void OnTenshouExeViewingHimeDetailShimaiExecute() {
	iShiteiBushouIDOnViewingHimeDetailShimai = iShiteiBushouIDOnViewingHimeDetailShimai & 0xFFFF; // BXレジスタ相当にする。

	int iBushouID = iShiteiBushouIDOnViewingHimeDetailShimai-1;
	int iHimeBushouID = iHimeBushouIDOnViewingHimeDtail-1;

	// 姉のアドレスをデフォルトに入れておく
	iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailAne);

	if ( ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) && ( 0 <= iHimeBushouID && iHimeBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		// 姫が姉であれば…
		if ( getYearsOld(iHimeBushouID) > getYearsOld(iBushouID) ) { // 側室の条件を満たすものが思い浮かんだら、ここにその条件を出す

			// 姉の場合のデフォルト
			iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailAne);

			if ( 0 ) {
				iMovAddressOnViewingHimeDetailShimai = int(szPushAdressOnViewingHimeDetailGiShi);
			}

		// 妹であれば…
		} else {

			// 妹の場合のデフォルト
			iMovAddressOnViewingHimeDetailShimai = int(iDefaultAddressOnViewingHimeDetailImouto);

			if ( 0 ) {
				iMovAddressOnViewingHimeDetailShimai = int(szPushAdressOnViewingHimeDetailGimai);
			}
		}
	}

}


/*
EDIのDIが親の番号
004945C4  |. 3BD8           CMP     EBX, EAX
				└ここを JMP TSMod.OnTSExeViewingHimeDetailShimai と書き換えてやる実際の処理
004945C6  |. B8 0C2F4D00    MOV     EAX, TENSHOU.004D2F0C                                     ;  ASCII "姉"
004945CB  |. 77 05          JA      SHORT TENSHOU.004945D2
004945CD  |. B8 082F4D00    MOV     EAX, TENSHOU.004D2F08                                     ;  ASCII "妹"
004945D2  |> 50             PUSH    EAX
*/
int pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai	=0x4945C4; // 関数はこのアドレスから、OnTenshouExeViewingHimeDetailShimaiへとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailShimai	=0x4945D2; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingHimeDetailShimai() {
	// スタックにためておく
	__asm {
		mov iShiteiBushouIDOnViewingHimeDetailShimai, EDI // 夫の武将番号の確保

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
	OnTenshouExeViewingHimeDetailShimaiExecute();

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

		mov eax, iMovAddressOnViewingHimeDetailShimai; // TENSHOU.EXEに元々存在した処理

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingHimeDetailShimai
	}
}

char cmdOnTenshouExeJumpFromViewingHimeDetailShimai[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingHimeDetailShimai() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingHimeDetailShimai;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingHimeDetailShimai  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromViewingHimeDetailShimai+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingHimeDetailShimai), cmdOnTenshouExeJumpFromViewingHimeDetailShimai, 7, NULL); //7バイトのみ書き込む
}

