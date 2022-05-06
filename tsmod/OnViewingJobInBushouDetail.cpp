#pragma pack(1)
#define _CRT_SECURE_NO_WARNINGS

#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "ScenarioMod.h"
#include "VariableNameRegistAndJudge.h"
#include "EXBushou_All.h"




/*
			┌待機			┌死亡     		┌忍者		┌茶人
004D13E8  [AC FE 4C] 00 《A4 FE 4C》 00 [9C FE 4C] 00 [AC B8 4C] 00  
			┌剣豪			┌僧侶		一喝
004D13F8  [94 FE 4C] 00 [8C FE 4C] 00 [84 B4 4B] 00 64 B4 4B 00  
004D1408  DC BB 4B 00 5C B4 4B 00 D4 BB 4B                 

*/

/*
00493D5D  |> 56             PUSH    ESI
00493D5E  |. E8 7F86F7FF    CALL    TENSHOU.0040C3E2					←★この関数の中で、Job番号(0x10=忍者～0x40=僧侶)を1,2,3,4に変換している。
00493D63  |. 83C4 04        ADD     ESP, 4
00493D66  |. 8BF8           MOV     EDI, EAX
00493D68  |. 85FF           TEST    EDI, EDI
00493D6A  |. 74 2B          JE      SHORT TENSHOU.00493D97
00493D6C  |. 68 A0000000    PUSH    0A0
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8            ;  ASCII "ｰVK"
00493D76  |. 68 A8000000    PUSH    0A8
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]　　　　　← ★この4D13ECが、「忍者」「茶人」「剣豪」「僧侶」 忍者=1 茶人=2 剣豪=3 僧侶=4
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
*/

#define GAMEDATASTRUCT_JOB_EX_COUNT 15

// 職業文字列のリスト
char szJobEXList[GAMEDATASTRUCT_JOB_EX_COUNT][9] = {
	"　",
	"忍者",
	"茶人",
	"剣豪",
	"僧侶",
	// ----これ以降はTSMOD必須----
	"海賊",
	"高家",
	"商人",
	"庶民",
	" 切支丹 ",
	// ----これ以降は.jpの中の人用----
	"  騎士  ",
	" 魔術士 ",
	"  魔族  ",
	"－",
	"－",
};

// 職業文字列へのポインタのリスト
char *pJobExList[GAMEDATASTRUCT_JOB_EX_COUNT] = {
	szJobEXList[0],
	(char *)0x4CFE9C, // 忍者 のTENSHOU.EXE内アドレス  szJobEXList[1],
	(char *)0x4CB8AC, // 茶人 のTENSHOU.EXE内アドレス  szJobEXList[2],
	(char *)0x4CFE94, // 剣豪 のTENSHOU.EXE内アドレス  szJobEXList[3],
	(char *)0x4CFE8C, // 僧侶 のTENSHOU.EXE内アドレス  szJobEXList[4],
	szJobEXList[5],
	szJobEXList[6],
	szJobEXList[7],
	szJobEXList[8],
	szJobEXList[9],
	szJobEXList[10],
	szJobEXList[11],
	szJobEXList[12],
	szJobEXList[13],
	szJobEXList[14],
};

char szJobNameAnkokuKishiDancho[11]  = "прсту"; // ← 暗黒騎士団長
char szJobNameAnkokuKishi[9]  = "暗黒騎士";
char szJobNameYamiSouryo[9] = " 黒僧侶 ";
char szJobNameKemonoTsukai[9] = " 獣使い ";
char szJobNameShirokishi[9] = " 白騎士 ";
char szJobNameSenseiJutsushi[9] = "占星術師";
char szJobNameYuuyokujin[9] = " 有翼人 ";
char szJobNameMajo[9] = "  魔女  ";
char szJobNameKensei[9] = "  剣聖  ";
char szJobNameMadoushi[9] = " 妖術士 ";
char szJobNameIkusaMico[9] = "  巫女  ";

char szJobNameCustomJobName[32] = ""; // カスタムＪＯＢ


// 対象ジョブの対象の武将ID(配列上ではなく、属性の値である(配列より+1多い数))
int iViewingJobInBushouID = 0;
int iViewingJobInJobID = 0;
int iViewingJobInXPosition = 0xA8;


void OnTenshouExeResetAllJobName() {
	pJobExList[0] =szJobEXList[0];

	pJobExList[1]=(char *)0x4CFE9C; // 忍者を元に戻す
	pJobExList[2]=(char *)0x4CB8AC; // 茶人を元に戻す
	pJobExList[3]=(char *)0x4CFE94; // 剣豪を元に戻す
	pJobExList[4]=(char *)0x4CFE8C; // 僧侶を元に戻す

	pJobExList[5] =szJobEXList[5];
	pJobExList[6] =szJobEXList[6];
	pJobExList[7] =szJobEXList[7];
	pJobExList[8] =szJobEXList[8];
	pJobExList[9] =szJobEXList[9];
	pJobExList[10]=szJobEXList[10];
	pJobExList[11]=szJobEXList[11];
	pJobExList[12]=szJobEXList[12];
	pJobExList[13]=szJobEXList[13];
	pJobExList[14]=szJobEXList[14];
}

// 高家・国人 などといった置き換えようの文字列1300人分のリスト
extern char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM];


void OnTenshouExeViewingJobInBushouDetailExecute() {

	// 下位４バイトが武将IDなので、マスク
	iViewingJobInBushouID = iViewingJobInBushouID & 0xFFFF;

	// 範囲外ならダメ
	if ( !(0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM )) {
		return;
	}

	// タルタロスであれば…剣豪ではなく暗黒騎士とする
	if (isLancelotTartare(iViewingJobInBushouID - 1)) {

		// 『剣豪』を指すポインタを、『暗黒騎士』を指すポインタに直す。『剣豪』でありながら『暗黒騎士』となるのだ、。
		pJobExList[3] = (char *)szJobNameAnkokuKishi;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// 暗黒騎士と表示するために、X座標を調整
		//iViewingJobInXPosition = 0x90; // ５文字前提の配置にするため、２文字に対してやや左に配置。普通は５文字なら0x90だが、外字でギッチギチなのでさらに左に-1
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// 朱伊であれば…巫女とする
	} else if (isShuiHidechiyo(iViewingJobInBushouID - 1)) {

			// 『剣豪』を指すポインタを、『暗黒騎士』を指すポインタに直す。『剣豪』でありながら『暗黒騎士』となるのだ、。
			pJobExList[3] = (char *)szJobNameIkusaMico;

			// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
			// 暗黒騎士と表示するために、X座標を調整
			//iViewingJobInXPosition = 0x90; // ５文字前提の配置にするため、２文字に対してやや左に配置。普通は５文字なら0x90だが、外字でギッチギチなのでさらに左に-1
			iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// 暗黒騎士であれば…騎士ではなく暗黒騎士とする
	} else if ( IsLoslorien(iViewingJobInBushouID-1) ) {

		// ハボリムが騎士なら、剣聖とする。
		if (  strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 && nb6bushouref[iViewingJobInBushouID-1].job == 0xA0) {
			pJobExList[10]=(char *)szJobNameKensei;

		// カチュアがキリシタンならば…切支丹ではなく闇僧侶とする
		} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 && nb6bushouref[iViewingJobInBushouID-1].job == 0x90) {
			// 『切支丹』を指すポインタを、『闇僧侶』を指すポインタに直す。『切支丹』でありながら『闇僧侶』となるのだ、。
			pJobExList[9]=(char *)szJobNameYamiSouryo;

			// ハボリム以外は暗黒騎士
		} else if ( nb6bushouref[iViewingJobInBushouID-1].job == 0xA0 ) {
			// 『騎士』を指すポインタを、『暗黒騎士』を指すポインタに直す。『騎士』でありながら『暗黒騎士』となるのだ、。
			pJobExList[10]=(char *)szJobNameAnkokuKishi;
		}

		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置


	// ウォーレンならば…魔術師ではなく占星術師とする
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 ) {

		// 『魔術師』を指すポインタを、『占星術師』を指すポインタに直す。『魔術師』でありながら『占星術師』となるのだ、。
		pJobExList[11]=(char *)szJobNameSenseiJutsushi;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// " 獣使い "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// カノープスならば…魔人ではなく有翼人とする
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 ) {

		// 『魔族』を指すポインタを、『有翼人』を指すポインタに直す。
		pJobExList[12]=(char *)szJobNameYuuyokujin;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// " 獣使い "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// ギルバルドであれば…騎士ではなく獣使いとする
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 ) {

		// 『騎士』を指すポインタを、『獣使い』を指すポインタに直す。『騎士』でありながら『獣使い』となるのだ、。
		pJobExList[10]=(char *)szJobNameKemonoTsukai;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// " 獣使い "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// ミルディンか、ギルダスならば…騎士ではなく白騎士とする
	} else if ( (strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0) || 
				(strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "шщъ")==0)
		) {

		// 『騎士』を指すポインタを、『白騎士』を指すポインタに直す。『騎士』でありながら『白騎士』となるのだ、。
		pJobExList[10]=(char *)szJobNameShirokishi;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// " 白騎士 "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置

	// デネブならば…魔術師ではなく魔女とする
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 ) {

		// 『魔術師』を指すポインタを、『魔女』を指すポインタに直す。
		pJobExList[11]=(char *)szJobNameMajo;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// "  魔女  "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置


	// ラドラムならば…魔術師ではなく魔導士とする
	} else if ( strcmp(nb6bushouname[iViewingJobInBushouID-1].familyname, "")==0 && strcmp(nb6bushouname[iViewingJobInBushouID-1].fastname, "")==0 ) {

		// 『魔術師』を指すポインタを、『魔導士』を指すポインタに直す。
		pJobExList[11]=(char *)szJobNameMadoushi;

		// 職業は実は2文字が前提となっている表示位置で固定されている。ここでは4文字配置するので、位置を変更する。
		// " 魔導士 "と表示するために、X座標を調整
		iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置


	// それ以外は、もともとの職業名に戻す
	} else {
		// 全ての職業の名前を元へと戻す。
		OnTenshouExeResetAllJobName();

		 // JobIDが0xA0(=iViewingJobInJobID=9=切支丹)を超えているならば、X座標を調整
		if ( iViewingJobInJobID >= 9 ) {
			 iViewingJobInXPosition = 0x98; // ４文字前提の配置にするため、２文字に対してやや左に配置
			// それ以外は、デフォルト
		} else {
			 iViewingJobInXPosition = 0xA8;	// デフォルトの2文字前提の配置に戻す
		}

		// 天翔記に元々いる武将であるならば…
		if ( IsOriginalRegistedBushou( iViewingJobInBushouID-1 ) ) {

			if ( 0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {

				int iFaceID = nb6bushouname[iViewingJobInBushouID-1].number;
				// 安全を見て大き目に
				char szTempName[20] = "";
				strcpy( szTempName, pOriginalBushouJobExList[iFaceID]);

				// 有効な置き換え文字がはいっているならば…
				if ( strlen( szTempName ) > 1 ) {

					// 該当武将のJOBのところを書き換える。
					int iJobID = (nb6bushouref[iViewingJobInBushouID-1].job / 0x10); // JOBの値を16(=0x10)で割ると配列のＩＤとなる。

					// 該当の職業名が２文字より短い場合、文字列を長くしてやる必要がある。
					if ( strlen(szTempName) < 4 ) {
						string s = string("  ")+szTempName+"  ";
						strcpy( szJobNameCustomJobName, s.c_str() );
					} else {
						strcpy( szJobNameCustomJobName, szTempName); // 全角で４文字がＭＡＸ
					}
					pJobExList[iJobID] = szJobNameCustomJobName;
					iViewingJobInXPosition = 0xA8 - (strlen(szJobNameCustomJobName)-4)*4;
				}
			}
		}

		// ScenarioModが有効であれば、
		// ScenarioModでカスタムＪＯＢを設定している場合、それを設定する。
		if ( p_snOnViewingJobName ) {
			if ( 0 <= iViewingJobInBushouID-1 && iViewingJobInBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM ) {
				// 安全を見て大き目に
				char szTempName[20] = "";
				bool isReWrite = ((PFNSNONVIEWINGJOBNAME)p_snOnViewingJobName)( szTempName, iViewingJobInBushouID );
				if ( isReWrite ) {
					// 該当武将のJOBのところを書き換える。
					int iJobID = (nb6bushouref[iViewingJobInBushouID-1].job / 0x10); // JOBの値を16(=0x10)で割ると配列のＩＤとなる。

					// 該当の職業名が２文字より短い場合、文字列を長くしてやる必要がある。
					if ( strlen(szTempName) < 4 ) {
						string s = string("  ")+szTempName+"  ";
						strcpy( szJobNameCustomJobName, s.c_str() );
					} else {
						strcpy( szJobNameCustomJobName, szTempName); // 全角で４文字がＭＡＸ
					}
					pJobExList[iJobID] = szJobNameCustomJobName;
					iViewingJobInXPosition = 0xA8 - (strlen(szJobNameCustomJobName)-4)*4;
				}
			}
		}
	}
}


/*
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8
00493D76  |. 68 A8000000    PUSH    0A8
				└ここを JMP TSMod.OnTSExeViewingJobInBushouDetail と書き換えてやる実際の処理
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
00493D94  |. 83C4 0C        ADD     ESP, 0C
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
*/
int pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail		=0x493D76; // 関数はこのアドレスから、OnTenshouExeViewingJobInBushouDetailへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeViewingJobInBushouDetail   =0x401488; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeViewingJobInBushouDetail	=0x493D89; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeViewingJobInBushouDetail() {
	// スタックにためておく
	__asm {
		mov iViewingJobInBushouID, esi	// 武将IDをコピーただし、esiの下位4バイトが武将IDなので、マスクする必要あり。
		mov iViewingJobInJobID, eax		// 職業IDをコピー

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// この関数内で安全にデータを加工する。
	OnTenshouExeViewingJobInBushouDetailExecute();

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
		
		// 元々TENSHOU.EXE内にあったが座標を調整したもの
		push iViewingJobInXPosition

		// 元々TENSHOU.EXE内にあったものをここで
		mov ecx, ebx

		call pTenshouExeJumpCallFromToOnTenshouExeViewingJobInBushouDetail

		push DWORD PTR DS:[edi*4+pJobExList]

		jmp pTenshouExeReturnLblFromOnTenshouExeViewingJobInBushouDetail
	}
}

/*
00493D71  |. BB F8674E00    MOV     EBX, TENSHOU.004E67F8
00493D76  |. 68 A8000000    PUSH    0A8
				└ここを JMP TSMod.OnTSExeViewingJobInBushouDetail と書き換えてやる実際の処理
00493D7B  |. 8BCB           MOV     ECX, EBX
00493D7D  |. E8 06D7F6FF    CALL    TENSHOU.00401488
00493D82  |. FF34BD EC134D0>PUSH    DWORD PTR DS:[EDI*4+4D13EC]
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
00493D8E  |. 53             PUSH    EBX
00493D8F  |. E8 1CA80100    CALL    TENSHOU.004AE5B0
00493D94  |. 83C4 0C        ADD     ESP, 0C
00493D89  |. 68 582E4D00    PUSH    TENSHOU.004D2E58                 ;  ASCII "%4s"
*/
char cmdOnTenshouExeJumpFromJobInBushouDetail[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計５バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeViewingJobInBushouDetail() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeViewingJobInBushouDetail;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeViewingJobInBushouDetail  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromJobInBushouDetail+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeViewingJobInBushouDetail), cmdOnTenshouExeJumpFromJobInBushouDetail, 5, NULL); // 6バイトのみ書き込む 元の命令が6バイト、新規の命令が５バイトなので、NOPが付け足されている。
}

/*
 * 以下、1300人分の武将の出自等のリスト。
 */
char *pOriginalBushouJobExList[GAMEDATASTRUCT_REGISTED_BUSHOU_NUM] = {
"守護代",
"",
"槍名人",
"",
"",
"庶民･茶人",
"庶民･茶人",
"茶人･商人",
"",
"槍名人",
"守護･茶人",
"重臣",
"",
"守護",
"守護",
"",
"",
"",
"",
"守護",
"守護",
"",
"",
"国人",
"国人",
"国人",
"僧･守護代",
"",
"",
"僧侶･一向",
"高家･弓名",
"",
"",
"剣豪･高家",
"切丹･守護",
"守護代",
"茶人･商人",
"",
"",
"",
"茶人･切丹",
"重臣",
"守護代",
"",
"",
"切丹･守護",
"旧仏教",
"守護",
"守護",
"剣豪･高家",
"",
"庶民･槍名",
"庶民･槍名",
"",
"",
"",
"",
"",
"",
"守護",
"",
"国人",
"国人",
"",
"",
"茶人･切丹",
"一向･国人",
"",
"",
"",
"",
"",
"",
"",
"茶人･歌人",
"",
"守護",
"",
"",
"",
"守護",
"",
"旧仏教",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"隠密",
"",
"",
"茶人･国人",
"",
"",
"国人･隠密",
"",
"商人･切丹",
"",
"",
"",
"重臣",
"一向宗",
"",
"守護",
"守護",
"",
"",
"守護",
"重臣",
"重臣",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"茶･守護代",
"茶人･切丹",
"",
"",
"",
"僧侶･茶人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"切丹･守護",
"守護",
"守護",
"",
"切丹･隠密",
"茶人･僧侶",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"守護代",
"旧仏教",
"",
"",
"守護代",
"守護代",
"守護代",
"守護代",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"守護",
"",
"",
"",
"",
"",
"",
"",
"",
"一向宗",
"",
"",
"重臣",
"守護",
"国人",
"国人",
"国人",
"国人",
"",
"",
"",
"重臣",
"国人･隠密",
"",
"",
"重臣",
"守護",
"剣豪･槍名",
"",
"",
"",
"",
"旧仏教",
"",
"",
"重臣",
"重臣",
"",
"",
"守護",
"",
"",
"国人",
"",
"",
"国人",
"国人",
"高家･弓名",
"",
"",
"",
"",
"",
"",
"",
"守護",
"旧仏教",
"国人",
"旧仏教",
"",
"守護代",
"国人",
"守護",
"国人",
"",
"守護代",
"僧侶･一向",
"僧侶･一向",
"僧侶･一向",
"",
"",
"重臣",
"",
"",
"",
"国人",
"守護代",
"",
"国人",
"守護",
"重臣",
"守護代",
"",
"守護代",
"守護代",
"",
"庶民･国人",
"",
"一向･国人",
"",
"国人",
"",
"",
"",
"",
"",
"",
"守護代",
"守護代",
"",
"",
"重臣",
"",
"",
"",
"",
"",
"国人･旧仏",
"守護代",
"",
"",
"国人",
"",
"",
"国人",
"高家･切丹",
"守護",
"",
"",
"",
"",
"",
"",
"",
"",
"重臣",
"旧仏教",
"",
"",
"守護･旧仏",
"守護",
"国人",
"旧仏教",
"重臣",
"重臣",
"",
"",
"国人",
"剣豪･国人",
"",
"",
"",
"守護･旧仏",
"国人",
"僧侶･一向",
"守護代",
"守護代",
"",
"",
"",
"守護",
"",
"守護",
"高家･守護",
"守護代",
"",
"守護",
"守護",
"",
"",
"守護代",
"",
"",
"守護",
"",
"",
"国人･剣豪",
"",
"槍名人",
"",
"国人",
"隠密･槍名",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"重臣",
"重臣",
"",
"重臣",
"",
"",
"",
"",
"",
"",
"",
"国人",
"守護",
"",
"",
"",
"",
"国人",
"",
"",
"守護",
"国人",
"",
"守護",
"",
"守護",
"",
"",
"重臣",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"守護",
"国人",
"国人",
"高家･国人",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"国人",
"",
"",
"国人",
"国人",
"国人",
"",
"",
"国人",
"旧仏教",
"旧仏教",
"旧仏教",
"",
"国人",
"重臣",
"",
"",
"重臣",
"",
"守護",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"守護",
"守護",
"守護",
"",
"",
"重臣",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"重臣",
"",
"",
"守護",
"守護",
"",
"",
"",
"",
"",
"",
"",
"剣豪･旧仏",
"国人",
"",
"国人",
"",
"国人",
"国人",
"旧仏教",
"旧仏教",
"",
"",
"旧仏教",
"国人",
"国人",
"国人",
"重臣",
"国人",
"国人",
"",
"",
"",
"",
"重臣",
"国人",
"",
"",
"国人･旧仏",
"国人",
"国人",
"",
"",
"",
"重臣",
"",
"",
"",
"",
"重臣",
"国人",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"海賊･国人",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"国人",
"",
"重臣",
"",
"",
"",
"",
"旧仏教",
"一向宗",
"",
"",
"",
"",
"",
"",
"",
"国人",
"守護･切丹",
"",
"守護代",
"守護代",
"守護代",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"守護代",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"国人",
"国人",
"国人",
"",
"",
"僧侶･一向",
"国人",
"国人",
"国人",
"国人",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"守護",
"",
"",
"",
"",
"国人",
"国人",
"旧仏教",
"",
"国人",
"",
"国人",
"国人･一向",
"",
"",
"守護",
"国人",
"国人",
"旧仏教",
"国人",
"国人",
"",
"",
"",
"旧仏教",
"",
"",
"守護代",
"",
"",
"",
"",
"",
"国人",
"",
"",
"国人",
"国人",
"",
"国人",
"国人",
"国人",
"国人",
"守護代",
"一向宗",
"",
"",
"守護",
"重臣",
"守護",
"旧仏教",
"重臣",
"僧侶･一向",
"一向･国人",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"守護代",
"高家･旧仏",
"高家･守護",
"",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"国人",
"国人",
"",
"",
"守護",
"国人",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"国人",
"",
"国人",
"茶人･旧仏",
"守護代･仏",
"",
"守護代",
"守護代･仏",
"",
"",
"僧侶･一向",
"国人",
"国人",
"国人",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"旧仏教",
"旧仏教",
"",
"僧侶･切丹",
"",
"",
"",
"一向･国人",
"守護･高家",
"守護･高家",
"守護･高家",
"国人",
"国人",
"国人",
"国人",
"",
"",
"",
"",
"国人",
"",
"",
"国人",
"国人",
"守護代",
"",
"",
"守護代",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"国人",
"国人",
"守護代",
"守護代",
"守護代",
"",
"重臣･国人",
"",
"",
"",
"",
"",
"国人",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"弓名人",
"旧仏教",
"旧仏教",
"",
"",
"",
"国人･旧仏",
"国人･旧仏",
"",
"国人",
"国人",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"国人",
"",
"",
"国人",
"",
"",
"",
"重臣･高家",
"重臣･高家",
"",
"国人･高家",
"",
"守護代",
"",
"",
"守護代",
"",
"",
"国人",
"国人",
"国人",
"国人",
"",
"",
"守護",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"弓名人",
"旧仏教",
"旧仏教",
"",
"",
"",
"",
"",
"",
"切丹･守護",
"切丹･守護",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"旧仏･国人",
"",
"",
"",
"国人",
"",
"切丹･守護",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"",
"国人",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"国人",
"国人",
"国人",
"",
"守護",
"",
"",
"",
"",
"守護",
"守護",
"重臣",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"鎌名人",
"",
"",
"",
"",
"",
"",
"",
"剣豪･国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"一向宗",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人･一向",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"重臣",
"",
"国人",
"弓名人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"旧仏教",
"旧仏教",
"",
"",
"",
"国人",
"",
"国人",
"国人",
"",
"国人",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"旧仏教",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"茶人･国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"歌人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"隠密",
"",
"",
"切丹･旧仏",
"",
"",
"国人",
"",
"",
"国人",
"",
"",
"",
"",
"",
"",
"国人",
"剣豪･槍名",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"旧仏教",
"重臣･旧仏",
"",
"",
"",
"国人",
"切丹･国人",
"",
"",
"",
"歌人",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"国人",
"",
"",
"",
"",
"",
"国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"切丹･国人",
"国人",
"",
"",
"",
"",
"",
"",
"",
"",
"",
""
};
