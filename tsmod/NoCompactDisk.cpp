#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

extern void WriteAsmJumperOnTenshouExeFindTenshoukiDisk();

// ＣＤドライブ自体が存在してるかチェックを亡くす
void ZeroDiskCheck() {
	/*
	0041753C  |> E8 B5230500    CALL    TENSHOU.004698F6
	00417541     83F8 01        CMP     EAX, 1			← この値が1以上ならCDが存在する。0ならない
	00417544  |. 73 07          JNB     SHORT TENSHOU.0041754D
	00417546  |. BE 5CC44B00    MOV     ESI, TENSHOU.004BC45C            ;  ASCII "CD-ROM ドライブが無い環境では実行できません。"
	*/

	// 以下は書き換えが間に合わない(Tenshou.exeの処理が先に走ってしまう)
	/*
	int iEAX = 0;

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x417543), &iEAX, 1, NULL); //1バイトのみ書き込む
	*/
}

// １つ目のＣＤ存在チェックを亡くす
void FirstDiskCheck() {
	/*
	0048D336  |. 85C0           TEST EAX,EAX																
	0048D338  |. 74 4B          JE SHORT TENSHOU.0048D385																
	0048D33A  |. 68 78C24C00    PUSH TENSHOU.004CC278                    ;  ASCII "TENSHOUKI95"																
	0048D33F  |. E8 D62C0200    CALL <JMP.&N6PAudio.CDAudioOpen>																
	0048D344  |. 83C4 04        ADD ESP,4																
	0048D347  |. A3 802A4D00    MOV DWORD PTR DS:[4D2A80],EAX																
	0048D34C  |. 83F8 FF        CMP EAX,-1								 ← 上の行で、得た値と-1とを比較。
																　 　　　←この行をNOP NOP NOP(=90 90 90)とすると１つダイアログが削れる。 								
	0048D34F  |. 75 34          JNZ SHORT TENSHOU.0048D385								一方はリターンを、他方はダイアログへと。								
	0048D351  |> 6A 05          /PUSH 5                                  ; /Style = MB_RETRYCANCEL|MB_APPLMODAL																
	0048D353  |. A1 2CC24B00    |MOV EAX,DWORD PTR DS:[4BC22C]           ; |																
	0048D358  |. 68 842A4D00    |PUSH TENSHOU.004D2A84                   ; |Title = "「信長の野望 天翔記 with パワーアップキット」"																
	0048D35D  |. 68 B42A4D00    |PUSH TENSHOU.004D2AB4                   ; |Text = "「信長の野望 天翔記 with パワーアップキット」のＣＤ－ＲＯＭを入れてください"																
	0048D362  |. 50             |PUSH EAX                                ; |hOwner => 00120B72 ('天翔記 for Windows 95',class='Tenshouki95')																
	0048D363  |. FF15 A4754E00  |CALL DWORD PTR DS:[<&USER32.MessageBoxA>; \MessageBoxA																
	0048D369  |. 83F8 02        |CMP EAX,2																
	0048D36C  |. 74 17          |JE SHORT TENSHOU.0048D385																
	0048D36E  |. 68 78C24C00    |PUSH TENSHOU.004CC278                   ;  ASCII "TENSHOUKI95"																
	0048D373  |. E8 A22C0200    |CALL <JMP.&N6PAudio.CDAudioOpen>																
	0048D378  |. 83C4 04        |ADD ESP,4																
	0048D37B  |. A3 802A4D00    |MOV DWORD PTR DS:[4D2A80],EAX																
	0048D380  |. 83F8 FF        |CMP EAX,-1																
	0048D383  |.^74 CC          \JE SHORT TENSHOU.0048D351																
	0048D385  \> C3             RETN																
	*/
	char szNOP3[4] = "\x90\x90\x90";

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x48D34C), szNOP3, 3, NULL); //3バイトのみ書き込む
}


// ＣＤが無い場合の処理。
// ウィンドウを通常モードでひょうじして、
void NoCompactDisk() {
	SetWindowPos(CommonGlobalHandle::tenshouWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ZeroDiskCheck();
	FirstDiskCheck();
	WriteAsmJumperOnTenshouExeFindTenshoukiDisk();

	// ノーディスクモード
	// AddModuleItem( (HMODULE)NoCompactDisk, "no_compact_disk"); //(HMODULE)NoDisk などという数値に意味はない。ユニークにするためだけ。
}



void CheckValidLogicalVolumeInfomation(char DriveName) {

	char   szDriveName[16] = "C:\\";
	char   szVolume[256];
    char   szSystem[256];
    DWORD   dwSerial;
    DWORD   dwLength;
    DWORD   dwFlags;

	// ドライブ名を引数で上書き
	szDriveName[0] = DriveName;
    
    // ボリューム情報の取得
    GetVolumeInformation( szDriveName,
        szVolume, sizeof(szVolume),
        &dwSerial, &dwLength, &dwFlags,
        szSystem, sizeof(szSystem) );
	/*
    OutputDebugStream( "GetVolumeInformation()関数のテスト\n" );
    OutputDebugStream( "\n" );
    OutputDebugStream( "ドライブ名　　　　　　：%s\n", szDriveName );
    OutputDebugStream( "ドライブのボリューム名：%s\n", szVolume );
    OutputDebugStream( "ドライブのシリアル番号：%08X\n", dwSerial );
    OutputDebugStream( "\n" );
    OutputDebugStream( "ファイルシステム名　　：%s\n", szSystem );
    OutputDebugStream( "ファイルシステムフラグ：%08X\n", dwFlags );
    OutputDebugStream( "ファイル名の最大文字数：%lu\n", dwLength );
	*/
}

#define DRIVE_MAX_NUM 26
void CheckValidLogicalDrive() {

    DWORD dwDrive = GetLogicalDrives();
    for ( int nDrive = 0 ; nDrive < DRIVE_MAX_NUM ; nDrive++ ){ // 26=Zドライブ:アルファベットの数
        if ( dwDrive & (1 << nDrive) ){ // ドライブが有効ならば
             CheckValidLogicalVolumeInfomation(nDrive+'A');
        }
    }
}
