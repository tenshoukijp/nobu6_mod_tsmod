#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "TSModCommand.h"

/*
モバイル用の解像度に変更する。縦幅を１段階小さくする
0049918E     BF 40060000    MOV     EDI, 400
00499193     81FB D0020000  CMP     EBX, 2D0 → 250へ置き換え
00499199  |. 7E 32          |JLE     SHORT TENSHOU.004991CD
0049919B     BB D0020000    MOV     EBX, 2D0 → 250へ置き換え
004991A0  |. EB 2B          |JMP     SHORT TENSHOU.004991CD
004991A2  |> 83FF 00        |CMP     EDI, 0
004991A5  |. 75 14          |JNZ     SHORT TENSHOU.004991BB
004991A7     BF 20030000    MOV     EDI, 320
004991AC  |. 81FB 50020000  |CMP     EBX, 250 → 1E0へ置き換え
004991B2  |. 7E 19          |JLE     SHORT TENSHOU.004991CD
004991B4  |. BB 50020000    |MOV     EBX, 250 → 1E0へ置き換え
004991B9  |. EB 12          |JMP     SHORT TENSHOU.004991CD
004991BB     BF 80020000    MOV     EDI, 280
004991C0  |. 81FB E0010000  |CMP     EBX, 1E0
004991C6  |. 7E 05          |JLE     SHORT TENSHOU.004991CD
004991C8  |. BB E0010000    |MOV     EBX, 1E0

004D32C4  82 B7 82 A9 00 00 00 00 20 20 31 30 32 34 81 7E  すか....  1024×
004D32D4  36 30 30 20 20 20 00 00 20 20 20 38 30 30 81 7E  600   ..   800×
004D32E4  34 38 30 20 20 20 00 00 00 00 00 00 01 00 00 00  480   .........
*/
void setMobileResolution() {
	if ( TSModCommand::Environment::cmdMobileResolution ) {
		int HeightOf1024=0x250;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x499195), &HeightOf1024, 4, NULL);
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x49919C), &HeightOf1024, 4, NULL);
		int HeightOf800=0x1E0;
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4991AE), &HeightOf800, 4, NULL);
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4991B5), &HeightOf800, 4, NULL);

		char szHeightOf1024[4]="600";
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4D32D4), szHeightOf1024, 3, NULL);
		char szHeightOf800[4]="480";
		WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x4D32E4), szHeightOf800, 3, NULL);
	}
}