#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
熟練度の「自動鍛錬化」を無効化する。(tenshou.exeのデフォルトへと必ず戻す)。神パッチよりの移植。
 */


void WritePatchAutoUpExperienceDefault( int iAddress, char *pValue, int len ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), pValue, len, NULL); //1バイトのみ書き込む
}


#define CMD_AUTOUP_EXPERIENCE_DEFAULT1	0x441BE9
#define CMD_AUTOUP_EXPERIENCE_DEFAULT2	0x441DA6
#define CMD_AUTOUP_EXPERIENCE_DEFAULT3	0x441DB4
#define CMD_AUTOUP_EXPERIENCE_DEFAULT4	0x441DD3
#define CMD_AUTOUP_EXPERIENCE_DEFAULT5	0x441DE1
#define CMD_AUTOUP_EXPERIENCE_DEFAULT6	0x441E00
#define CMD_AUTOUP_EXPERIENCE_DEFAULT7	0x441E0E

void InitAutoUpExperienceDefault() {

	// 以下の場所は強制的に下記の値にしておく。
	/*
	00441BE9   74 14            JE      SHORT TENSHOU.00441BFF
	*/
	char szCmd1[] = "\x74\x14";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT1, szCmd1, sizeof(szCmd1)-1);

	/*
	00441DA6   76 28            JBE     SHORT TENSHOU.00441DD0
	*/
	char szCmd2[] = "\x76\x28";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT2, szCmd2, sizeof(szCmd2)-1);

	/*
	00441DB4   85C0             TEST    EAX, EAX
	00441DB6   74 18            JE      SHORT TENSHOU.00441DD0
	00441DB8   56               PUSH    ESI
	00441DB9   E8 2275FCFF      CALL    TENSHOU.004092E0
	00441DBE   83C4 04          ADD     ESP, 4
	00441DC1   3BC3             CMP     EAX, EBX
	00441DC3   72 0B            JB      SHORT TENSHOU.00441DD0
	00441DC5   6A 01            PUSH    1
	00441DC7   56               PUSH    ESI
	00441DC8   E8 9175FCFF      CALL    TENSHOU.0040935E
	*/
	char szCmd3[] = "\x85\xC0\x74\x18\x56\xE8\x22\x75\xFC\xFF\x83\xC4\x04\x3B\xC3\x72\x0B\x6A\x01\x56\xE8\x91\x75\xFC\xFF";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT3, szCmd3, sizeof(szCmd3)-1);


	/*
	00441DD3   76 28            JBE     SHORT TENSHOU.00441DFD
	*/
	char szCmd4[] = "\x76\x28";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT4, szCmd4, sizeof(szCmd4)-1);

	/*
	00441DE1   85C0             TEST    EAX, EAX
	00441DE3   74 18            JE      SHORT TENSHOU.00441DFD
	00441DE5   56               PUSH    ESI
	00441DE6   E8 A675FCFF      CALL    TENSHOU.00409391
	00441DEB   83C4 04          ADD     ESP, 4
	00441DEE   3BC3             CMP     EAX, EBX
	00441DF0   72 0B            JB      SHORT TENSHOU.00441DFD
	00441DF2   6A 01            PUSH    1
	00441DF4   56               PUSH    ESI
	00441DF5   E8 1576FCFF      CALL    TENSHOU.0040940F
	*/
	char szCmd5[] = "\x85\xC0\x74\x18\x56\xE8\xA6\x75\xFC\xFF\x83\xC4\x04\x3B\xC3\x72\x0B\x6A\x01\x56\xE8\x15\x76\xFC\xFF";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT5, szCmd5, sizeof(szCmd5)-1);

	/*
	00441E00   76 28            JBE     SHORT TENSHOU.00441E2A
	*/
	char szCmd6[] = "\x76\x28";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT6, szCmd6, sizeof(szCmd6)-1);

	/*
	00441E0E   85C0             TEST    EAX, EAX
	00441E10   74 18            JE      SHORT TENSHOU.00441E2A
	00441E12   56               PUSH    ESI
	00441E13   E8 1774FCFF      CALL    TENSHOU.0040922F
	00441E18   83C4 04          ADD     ESP, 4
	00441E1B   3BC3             CMP     EAX, EBX
	00441E1D   72 0B            JB      SHORT TENSHOU.00441E2A
	00441E1F   6A 01            PUSH    1
	00441E21   56               PUSH    ESI
	00441E22   E8 8674FCFF      CALL    TENSHOU.004092AD
	*/
	char szCmd7[] = "\x85\xC0\x74\x18\x56\xE8\x17\x74\xFC\xFF\x83\xC4\x04\x3B\xC3\x72\x0B\x6A\x01\x56\xE8\x86\x74\xFC\xFF";
	WritePatchAutoUpExperienceDefault(CMD_AUTOUP_EXPERIENCE_DEFAULT7, szCmd7, sizeof(szCmd7)-1);


}