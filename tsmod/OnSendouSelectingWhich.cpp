#include "WinTarget.h"
#include <time.h>

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"


/*
 * 煽動のどれを選択するかのタイミング
 */

/*
A	農民			
9	白黒	ターン増える		
8	失敗			
7	技能習得	技能を忘れることも有り		
6	励ます	までに全員		
5	失敗			
4	放火			
3	徴兵。失敗することもおおい			まれに全員
2	敵挑発			
1	誰か傍の人一喝(敵見方含む）			
0	誰か大混乱			まれに全員
*/

int iSendouSelectingWhich = -1;

// 煽動を行った実行武将ＩＤ
int iSendouSelectingWhichBushouID = -1;

// 兵数の最大数(カスタム考慮)
extern int getCustomSoldierMax(int, int);

// 前回の煽動時に混乱したカウント
extern int iSendouKonranTargetConranCnt;


void OnTenshouExeSendouSelectingWhichExecute() {

	// 前回の煽動で混乱した人数をここで念のためリセット
	iSendouKonranTargetConranCnt = 0;

	// 煽動参以上であれば、煽動の選択が調整される。

	// 武将IDが一定の範囲(それ以外の値が入ってくることもあるためチェック)
	if (0 <= iSendouSelectingWhichBushouID-1 && iSendouSelectingWhichBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {

		if (getBushouInt(iSendouSelectingWhichBushouID-1) >= 70 &&   // 智謀が70(=140)以上
			nb6bushouref[iSendouSelectingWhichBushouID-1].maxint >= 85 &&	 // 智才が85(=170)以上
			nb6bushouref[iSendouSelectingWhichBushouID-1].ambition >= 70) {	// 野望が70以上

			// 敵を挑発は失敗なので、大混乱へ
			if ( iSendouSelectingWhich == 2 ) {
				iSendouSelectingWhich = 0;


			// ３の徴兵、もしくは、５の失敗は、大混乱へ
			} else if ( iSendouSelectingWhich == 3 || iSendouSelectingWhich == 5) {

				//　現在の身分を得る
				int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

				//　身分から最大兵数を得る
				int soldiermax = nb6soldiermax[position].soldiermax;

				// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
				soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

				// まだ最大兵数でないならば…
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
					// 徴兵に	
					iSendouSelectingWhich = 3;
				} else {
					// 大混乱に
					iSendouSelectingWhich = 0;
				}

			// ６の鼓舞、もしくは、８は失敗なので、鼓舞出来るなら鼓舞に
			} else if ( iSendouSelectingWhich == 6 || iSendouSelectingWhich == 8 ) {
				// 士気がまだMAXでないならば･･･
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].solloyal < 100 ) {
					// 鼓舞をする
					iSendouSelectingWhich = 6;
				} else {
					//　現在の身分を得る
					int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

					//　身分から最大兵数を得る
					int soldiermax = nb6soldiermax[position].soldiermax;

					// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
					soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

					// まだ最大兵数でないならば…
					if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
						// 徴兵に	
						iSendouSelectingWhich = 3;
					} else {
						// 大混乱に
						iSendouSelectingWhich = 0;
					}
				}

			// 農民は失敗なので、混乱へ
			} else if ( iSendouSelectingWhich == 0x0A ) {
				// 士気がまだMAXでないならば･･･
				if ( nb6bushouref[iSendouSelectingWhichBushouID-1].solloyal < 100 ) {
					// 鼓舞をする
					iSendouSelectingWhich = 6;
				} else {
					//　現在の身分を得る
					int position = nb6bushouname[iSendouSelectingWhichBushouID-1].position;

					//　身分から最大兵数を得る
					int soldiermax = nb6soldiermax[position].soldiermax;

					// 最大兵士数は、忍者だったり官位・役職等によってフィルターがかかる
					soldiermax = getCustomSoldierMax( iSendouSelectingWhichBushouID-1, soldiermax );

					// まだ最大兵数でないならば…
					if ( nb6bushouref[iSendouSelectingWhichBushouID-1].soldier < soldiermax ) {
						// 徴兵に	
						iSendouSelectingWhich = 3;
					} else {
						// 大混乱に
						iSendouSelectingWhich = 0;
					}
				}

			}
		}
	}

	// 煽動極以上であれば、
	// 挑発と一喝すら混乱へとなだれ込む
	if (0 <= iSendouSelectingWhichBushouID-1 && iSendouSelectingWhichBushouID-1 < GAMEDATASTRUCT_BUSHOU_NUM) {
		if (getBushouInt(iSendouSelectingWhichBushouID-1) >= 110 &&   // 智謀が110(=220)以上
			nb6bushouref[iSendouSelectingWhichBushouID-1].maxint >= 110 &&	 // 智才が110(=220)以上
			nb6bushouref[iSendouSelectingWhichBushouID-1].ambition >= 110) {	// 野望が110以上

			// 挑発と一喝すら混乱となる
			if ( iSendouSelectingWhich == 1 || iSendouSelectingWhich == 2 ) {
				// 大混乱に
				iSendouSelectingWhich = 0;
			}
		}
		
	}

	// ないはずだが、0x0B以上になっている場合は、大混乱にする。
	if ( iSendouSelectingWhich >= 11 ) {
		iSendouSelectingWhich = 0;
	}
}


/*
0043483A  |. 83FA 0B             |CMP     EDX, 0B
0043483D  |.^72 ED               \JB      SHORT TENSHOU.0043482C
0043483F  |> 57                  PUSH    EDI
				└ここを JMP TSMod.OnTSExeSendouSelectingWhich と書き換えてやる実際の処理
00434840  |. FF1495 A8FB4B0>CALL    DWORD PTR DS:[EDX*4+4BFBA8]									← ここのEDX値で煽動の種類が決定される。									
00434847  |. 83C4 04        ADD     ESP, 4									　　10(=A)なら農民、1なら一喝、など。									
*/
int pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich	 =0x43483F; // 関数はこのアドレスから、OnTenshouExeSendouSelectingWhichへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeSendouSelectingWhich =0x43482C; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeSendouSelectingWhich  =0x434847; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeSendouSelectingWhich() {
	// スタックにためておく
	__asm {
		// 対象の武将ID
		mov iSendouSelectingWhichBushouID, edi

		// 煽動の種類番号
		mov iSendouSelectingWhich, edx

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
	OnTenshouExeSendouSelectingWhichExecute();

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

		// edxを上書き
		mov edx, iSendouSelectingWhich

		// 元々TENSHOU.EXEであったもの
		push edi

		// 元々TENSHOU.EXEであったもの(煽動コマンドの実行) EDXが実際の選択肢番号
		call dword ptr ds:[edx*4+0x4BFBA8]

		// 以下２回目の煽動を実行するならば、以下の４行のコメントアウトをはずせば良いようだが、良くチェックしないと飛ぶかもしれない。
		/*
		// 汚れたレジストリを復活
		mov edx, iSendouSelectingWhich;

		add esp, 4
		push edi
		// ２回目の実行
		call dword ptr ds:[edx*4+0x4BFBA8]
		*/

		// 元へとジャンプ
		jmp pTenshouExeReturnLblFromOnTenshouExeSendouSelectingWhich
	}
}

/*
0043483A  |. 83FA 0B             |CMP     EDX, 0B
0043483D  |.^72 ED               \JB      SHORT TENSHOU.0043482C
0043483F  |> 57                  PUSH    EDI
00434840  |. FF1495 A8FB4B0>CALL    DWORD PTR DS:[EDX*4+4BFBA8]									← ここのEDX値で煽動の種類が決定される。									
				└ここを JMP TSMod.OnTSExeSendouSelectingWhich と書き換えてやる実際の処理
00434847  |. 83C4 04        ADD     ESP, 4									　　10(=A)なら農民、1なら一喝、など。									
*/
char cmdOnTenshouExeJumpFromSendouSelectingWhich[9]="\xE9\x90\x90\x90\x90\x90\x90\x90";
// 元の命令が6バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeSendouSelectingWhich() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeSendouSelectingWhich;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich + 5 )	;
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeSendouSelectingWhich  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromSendouSelectingWhich+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeSendouSelectingWhich), cmdOnTenshouExeJumpFromSendouSelectingWhich, 8, NULL); //5バイトのみ書き込む
}

