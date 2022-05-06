#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

extern void WriteAsmJumperOnTenshouExeFindTenshoukiDisk();

// �b�c�h���C�u���̂����݂��Ă邩�`�F�b�N��S����
void ZeroDiskCheck() {
	/*
	0041753C  |> E8 B5230500    CALL    TENSHOU.004698F6
	00417541     83F8 01        CMP     EAX, 1			�� ���̒l��1�ȏ�Ȃ�CD�����݂���B0�Ȃ�Ȃ�
	00417544  |. 73 07          JNB     SHORT TENSHOU.0041754D
	00417546  |. BE 5CC44B00    MOV     ESI, TENSHOU.004BC45C            ;  ASCII "CD-ROM �h���C�u���������ł͎��s�ł��܂���B"
	*/

	// �ȉ��͏����������Ԃɍ���Ȃ�(Tenshou.exe�̏�������ɑ����Ă��܂�)
	/*
	int iEAX = 0;

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x417543), &iEAX, 1, NULL); //1�o�C�g�̂ݏ�������
	*/
}

// �P�ڂ̂b�c���݃`�F�b�N��S����
void FirstDiskCheck() {
	/*
	0048D336  |. 85C0           TEST EAX,EAX																
	0048D338  |. 74 4B          JE SHORT TENSHOU.0048D385																
	0048D33A  |. 68 78C24C00    PUSH TENSHOU.004CC278                    ;  ASCII "TENSHOUKI95"																
	0048D33F  |. E8 D62C0200    CALL <JMP.&N6PAudio.CDAudioOpen>																
	0048D344  |. 83C4 04        ADD ESP,4																
	0048D347  |. A3 802A4D00    MOV DWORD PTR DS:[4D2A80],EAX																
	0048D34C  |. 83F8 FF        CMP EAX,-1								 �� ��̍s�ŁA�����l��-1�Ƃ��r�B
																�@ �@�@�@�����̍s��NOP NOP NOP(=90 90 90)�Ƃ���ƂP�_�C�A���O������B 								
	0048D34F  |. 75 34          JNZ SHORT TENSHOU.0048D385								����̓��^�[�����A�����̓_�C�A���O�ւƁB								
	0048D351  |> 6A 05          /PUSH 5                                  ; /Style = MB_RETRYCANCEL|MB_APPLMODAL																
	0048D353  |. A1 2CC24B00    |MOV EAX,DWORD PTR DS:[4BC22C]           ; |																
	0048D358  |. 68 842A4D00    |PUSH TENSHOU.004D2A84                   ; |Title = "�u�M���̖�] �V�ċL with �p���[�A�b�v�L�b�g�v"																
	0048D35D  |. 68 B42A4D00    |PUSH TENSHOU.004D2AB4                   ; |Text = "�u�M���̖�] �V�ċL with �p���[�A�b�v�L�b�g�v�̂b�c�|�q�n�l�����Ă�������"																
	0048D362  |. 50             |PUSH EAX                                ; |hOwner => 00120B72 ('�V�ċL for Windows 95',class='Tenshouki95')																
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

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x48D34C), szNOP3, 3, NULL); //3�o�C�g�̂ݏ�������
}


// �b�c�������ꍇ�̏����B
// �E�B���h�E��ʏ탂�[�h�łЂ傤�����āA
void NoCompactDisk() {
	SetWindowPos(CommonGlobalHandle::tenshouWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ZeroDiskCheck();
	FirstDiskCheck();
	WriteAsmJumperOnTenshouExeFindTenshoukiDisk();

	// �m�[�f�B�X�N���[�h
	// AddModuleItem( (HMODULE)NoCompactDisk, "no_compact_disk"); //(HMODULE)NoDisk �ȂǂƂ������l�ɈӖ��͂Ȃ��B���j�[�N�ɂ��邽�߂����B
}



void CheckValidLogicalVolumeInfomation(char DriveName) {

	char   szDriveName[16] = "C:\\";
	char   szVolume[256];
    char   szSystem[256];
    DWORD   dwSerial;
    DWORD   dwLength;
    DWORD   dwFlags;

	// �h���C�u���������ŏ㏑��
	szDriveName[0] = DriveName;
    
    // �{�����[�����̎擾
    GetVolumeInformation( szDriveName,
        szVolume, sizeof(szVolume),
        &dwSerial, &dwLength, &dwFlags,
        szSystem, sizeof(szSystem) );
	/*
    OutputDebugStream( "GetVolumeInformation()�֐��̃e�X�g\n" );
    OutputDebugStream( "\n" );
    OutputDebugStream( "�h���C�u���@�@�@�@�@�@�F%s\n", szDriveName );
    OutputDebugStream( "�h���C�u�̃{�����[�����F%s\n", szVolume );
    OutputDebugStream( "�h���C�u�̃V���A���ԍ��F%08X\n", dwSerial );
    OutputDebugStream( "\n" );
    OutputDebugStream( "�t�@�C���V�X�e�����@�@�F%s\n", szSystem );
    OutputDebugStream( "�t�@�C���V�X�e���t���O�F%08X\n", dwFlags );
    OutputDebugStream( "�t�@�C�����̍ő啶�����F%lu\n", dwLength );
	*/
}

#define DRIVE_MAX_NUM 26
void CheckValidLogicalDrive() {

    DWORD dwDrive = GetLogicalDrives();
    for ( int nDrive = 0 ; nDrive < DRIVE_MAX_NUM ; nDrive++ ){ // 26=Z�h���C�u:�A���t�@�x�b�g�̐�
        if ( dwDrive & (1 << nDrive) ){ // �h���C�u���L���Ȃ��
             CheckValidLogicalVolumeInfomation(nDrive+'A');
        }
    }
}
