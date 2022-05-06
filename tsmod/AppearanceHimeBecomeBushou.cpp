#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ‚b‚o‚t‚Ì“oê•P‚à•P•«‚Æ‚È‚éB_ƒpƒbƒ`‚æ‚è‚ÌˆÚAB
 */


void WritePatchAppearanceHimeBecomeBushou( int iAddress, char *pValue, int len ) {
	// \’z‚µ‚½ƒj[ƒ‚ƒjƒbƒN–½—ß‚ðTENSHOU.EXE‚Ìƒƒ‚ƒŠ‚É‘‚«Š·‚¦‚é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), pValue, len, NULL); //1ƒoƒCƒg‚Ì‚Ý‘‚«ž‚Þ
}


#define CMD_APPEARANCE_HIME_BECOME_BUSHOU1	0x44446D	
#define CMD_APPEARANCE_HIME_BECOME_BUSHOU2	0x44447D
#define CMD_APPEARANCE_HIME_BECOME_BUSHOU3	0x444689

void SetAppearanceHimeBecomeBushou() {

	if ( TSModCommand::AllBushou::cmdAppearanceHimeBecomeBushou ) {

		//--------
		// _ƒpƒbƒ`‚Ì¢‚b‚o‚t‚à•P“oêŽžA‹­§•«‰»v‚ð—LŒø‚Æ‚µ‚Ä‚¢‚éê‡
		/*
		0044446D   . E8 02010000    CALL    TENSHOU.00444574
		*/
		char szCmd1[] = "\xE8\x02\x01\x00\x00";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU1, szCmd1, sizeof(szCmd1)-1);

		/*
		0044447D   . 6A 0F          PUSH    0F
		0044447F   . E8 20240200    CALL    TENSHOU.004668A4
		00444484   . 83C4 04        ADD     ESP, 4
		00444487   . 48             DEC     EAX
		00444488   . 66:8B7C24 14   MOV     DI, WORD PTR SS:[ESP+14]
		0044448D   . BD A89A4D00    MOV     EBP, TENSHOU.004D9AA8            ;  ASCII "ðVK"
		00444492   . 6A 00          PUSH    0
		00444494   . 83F8 01        CMP     EAX, 1
		00444497   . 1BC0           SBB     EAX, EAX
		00444499   . 83C4 04        ADD     ESP, 4
		0044449C   . 56             PUSH    ESI
		0044449D     E8             DB      E8
		0044449E     C5             DB      C5
		0044449F   . 11060083       DD      83000611
		004444A3     C4             DB      C4
		004444A4     04             DB      04
		004444A5   . 85C0           TEST    EAX, EAX
		004444A7   . 0F84 92000000  JE      TENSHOU.0044453F
		004444AD   . 90             NOP
		004444AE   . 90             NOP
		004444AF   . 90             NOP
		004444B0   . 90             NOP
		004444B1   . 90             NOP
		``‚¸‚Á‚ÆNOP
		004444D6   . 90             NOP
		004444D7   . 90             NOP
		004444D8   . 90             NOP
		004444D9   . 57             PUSH    EDI                              ; /Arg2
		004444DA   . 68 BB130000    PUSH    13BB                             ; |Arg1 = 000013BB
		004444DF   . E8 747F0200    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
		004444E4   . 83C4 08        ADD     ESP, 8
		004444E7   . 50             PUSH    EAX
		004444E8   . E8 E3D30500    CALL    TENSHOU.004A18D0
		004444ED   . 83C4 04        ADD     ESP, 4
		004444F0   . 85C0           TEST    EAX, EAX
		004444F2   . B8 BD130000    MOV     EAX, 13BD
		004444F7   . 74 76          JE      SHORT TENSHOU.0044456F
		004444F9   . 90             NOP
		004444FA   . 90             NOP
		004444FB   . 90             NOP
		``‚¸‚Á‚ÆNOP
		0044453C   . 90             NOP
		0044453D   . 90             NOP
		0044453E   . 90             NOP
		0044453F   > 56             PUSH    ESI
		00444540   . E8 950A0600    CALL    TENSHOU.004A4FDA
		00444545   . 83C4 04        ADD     ESP, 4
		00444548   . 50             PUSH    EAX
		00444549   . 57E8ACC6       DD      C6ACE857
		0044454D     FF             DB      FF
		0044454E     FF             DB      FF
		0044454F   . 83C4 08        ADD     ESP, 8
		00444552   . 57             PUSH    EDI
		00444553   . 56             PUSH    ESI
		00444554   . E8 52110600    CALL    TENSHOU.004A56AB
		00444559   . 83C4 04        ADD     ESP, 4
		0044455C   . 50             PUSH    EAX
		0044455D   . E8 026CFCFF    CALL    TENSHOU.0040B164
		00444562   . 83C4 08        ADD     ESP, 8
		00444565   . B9 A89A4D00    MOV     ECX, TENSHOU.004D9AA8            ;  ASCII "ðVK"
		*/
		char szCmd2[] = "\x6A\x0F\xE8\x20\x24\x02\x00\x83\xC4\x04\x48\x66\x8B\x7C\x24\x14\xBD\xA8\x9A\x4D\x00\x6A\x00\x83\xF8\x01\x1B\xC0\x83\xC4\x04\x56\xE8\xC5\x11\x06\x00\x83\xC4\x04\x85\xC0\x0F\x84\x92\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x57\x68\xBB\x13\x00\x00\xE8\x74\x7F\x02\x00\x83\xC4\x08\x50\xE8\xE3\xD3\x05\x00\x83\xC4\x04\x85\xC0\xB8\xBD\x13\x00\x00\x74\x76\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x56\xE8\x95\x0A\x06\x00\x83\xC4\x04\x50\x57\xE8\xAC\xC6\xFF\xFF\x83\xC4\x08\x57\x56\xE8\x52\x11\x06\x00\x83\xC4\x04\x50\xE8\x02\x6C\xFC\xFF\x83\xC4\x08\xB9\xA8\x9A\x4D\x00";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU2, szCmd2, sizeof(szCmd2)-1);

		/*
		00444689  |. 0F84 7D000000  JE      TENSHOU.0044470C
		*/
		char szCmd3[] = "\x0F\x84\x7D\x00\x00\x00";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU3, szCmd3, sizeof(szCmd3)-1);

	} else {

		//--------
		// ƒfƒtƒHƒ‹ƒg‚Ì•P‚Ì•«ˆ—B(‚Ù‚Æ‚ñ‚Ç‚Ìê‡A’Êí‚Ì•P‚Æ‚È‚é)
		/*
		0044446D  |. E8 F5110600    CALL    TENSHOU.004A5667
		*/
		char szCmd1[] = "\xE8\xF5\x11\x06\x00";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU1, szCmd1, sizeof(szCmd1)-1);

		/*
		0044447D  |. 56             PUSH    ESI
		0044447E  |. E8 F1000000    CALL    TENSHOU.00444574
		00444483  |. 83C4 04        ADD     ESP, 4
		00444486  |. 85C0           TEST    EAX, EAX
		00444488  |. 0F84 E1000000  JE      TENSHOU.0044456F
		0044448E  |. 6A 0F          PUSH    0F
		00444490  |. E8 0F240200    CALL    TENSHOU.004668A4
		00444495  |. 83C4 04        ADD     ESP, 4
		00444498  |. 48             DEC     EAX
		00444499  |. 66:8B7C24 14   MOV     DI, WORD PTR SS:[ESP+14]
		0044449E  |. BD A89A4D00    MOV     EBP, TENSHOU.004D9AA8            ;  ASCII "ðVK"
		004444A3  |. 6A 00          PUSH    0
		004444A5  |. 83F8 01        CMP     EAX, 1
		004444A8  |. 1BC0           SBB     EAX, EAX
		004444AA  |. 55             PUSH    EBP
		004444AB  |. 05 B9130000    ADD     EAX, 13B9
		004444B0  |. 57             PUSH    EDI
		004444B1  |. 53             PUSH    EBX                              ; /Arg3
		004444B2  |. 57             PUSH    EDI                              ; |Arg2
		004444B3  |. 50             PUSH    EAX                              ; |Arg1
		004444B4  |. E8 C67F0200    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
		004444B9  |. 83C4 0C        ADD     ESP, 0C
		004444BC  |. 50             PUSH    EAX                              ; |Arg3
		004444BD  |. B8 FFFF0000    MOV     EAX, 0FFFF                       ; |
		004444C2  |. 50             PUSH    EAX                              ; |Arg2 => 0000FFFF
		004444C3  |. 50             PUSH    EAX                              ; |Arg1 => 0000FFFF
		004444C4  |. E8 97DA0500    CALL    TENSHOU.004A1F60                 ; \TENSHOU.004A1F60
		004444C9  |. 83C4 18        ADD     ESP, 18
		004444CC  |. 6A 02          PUSH    2
		004444CE  |. E8 57ECFDFF    CALL    TENSHOU.0042312A
		004444D3  |. 83C4 04        ADD     ESP, 4
		004444D6  |. 6A 00          PUSH    0
		004444D8  |. 53             PUSH    EBX                              ; /Arg3
		004444D9  |. 57             PUSH    EDI                              ; |Arg2
		004444DA  |. 68 BA130000    PUSH    13BA                             ; |Arg1 = 000013BA
		004444DF  |. E8 9B7F0200    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
		004444E4  |. 83C4 0C        ADD     ESP, 0C
		004444E7  |. 8BCD           MOV     ECX, EBP                         ; |
		004444E9  |. 50             PUSH    EAX                              ; |Arg1
		004444EA  |. E8 0AD90500    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
		004444EF  |. 6A 02          PUSH    2
		004444F1  |. E8 34ECFDFF    CALL    TENSHOU.0042312A
		004444F6  |. 83C4 04        ADD     ESP, 4
		004444F9  |. 57             PUSH    EDI                              ; /Arg2
		004444FA  |. 68 BB130000    PUSH    13BB                             ; |Arg1 = 000013BB
		004444FF  |. E8 547F0200    CALL    TENSHOU.0046C458                 ; \TENSHOU.0046C458
		00444504  |. 83C4 08        ADD     ESP, 8
		00444507  |. 50             PUSH    EAX
		00444508  |. E8 C3D30500    CALL    TENSHOU.004A18D0
		0044450D  |. 83C4 04        ADD     ESP, 4
		00444510  |. 85C0           TEST    EAX, EAX
		00444512  |. B8 BD130000    MOV     EAX, 13BD
		00444517  |. 74 2B          JE      SHORT TENSHOU.00444544
		00444519  |. 56             PUSH    ESI
		0044451A  |. E8 BB0A0600    CALL    TENSHOU.004A4FDA
		0044451F  |. 83C4 04        ADD     ESP, 4
		00444522  |. 50             PUSH    EAX
		00444523  |. 57             PUSH    EDI
		00444524  |. E8 D2C6FFFF    CALL    TENSHOU.00440BFB
		00444529  |. 83C4 08        ADD     ESP, 8
		0044452C  |. 57             PUSH    EDI
		0044452D  |. 56             PUSH    ESI
		0044452E  |. E8 78110600    CALL    TENSHOU.004A56AB
		00444533  |. 83C4 04        ADD     ESP, 4
		00444536  |. 50             PUSH    EAX
		00444537  |. E8 286CFCFF    CALL    TENSHOU.0040B164
		0044453C  |. 83C4 08        ADD     ESP, 8
		0044453F  |. B8 BC130000    MOV     EAX, 13BC
		00444544  |> 6A 00          PUSH    0
		00444546  |. 53             PUSH    EBX                              ; /Arg3
		00444547  |. 57             PUSH    EDI                              ; |Arg2
		00444548  |. BE A89A4D00    MOV     ESI, TENSHOU.004D9AA8            ; |ASCII "ðVK"
		0044454D  |. 50             PUSH    EAX                              ; |Arg1
		0044454E  |. E8 2C7F0200    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
		00444553  |. 83C4 0C        ADD     ESP, 0C
		00444556  |. 8BCE           MOV     ECX, ESI                         ; |
		00444558  |. 50             PUSH    EAX                              ; |Arg1
		00444559  |. E8 9BD80500    CALL    TENSHOU.004A1DF9                 ; \TENSHOU.004A1DF9
		0044455E  |. 6A 02          PUSH    2
		00444560  |. E8 C5EBFDFF    CALL    TENSHOU.0042312A
		00444565  |. 83C4 04        ADD     ESP, 4
		00444568  |. 8BCE           MOV     ECX, ESI
		*/
		char szCmd2[] = "\x56\xE8\xF1\x00\x00\x00\x83\xC4\x04\x85\xC0\x0F\x84\xE1\x00\x00\x00\x6A\x0F\xE8\x0F\x24\x02\x00\x83\xC4\x04\x48\x66\x8B\x7C\x24\x14\xBD\xA8\x9A\x4D\x00\x6A\x00\x83\xF8\x01\x1B\xC0\x55\x05\xB9\x13\x00\x00\x57\x53\x57\x50\xE8\xC6\x7F\x02\x00\x83\xC4\x0C\x50\xB8\xFF\xFF\x00\x00\x50\x50\xE8\x97\xDA\x05\x00\x83\xC4\x18\x6A\x02\xE8\x57\xEC\xFD\xFF\x83\xC4\x04\x6A\x00\x53\x57\x68\xBA\x13\x00\x00\xE8\x9B\x7F\x02\x00\x83\xC4\x0C\x8B\xCD\x50\xE8\x0A\xD9\x05\x00\x6A\x02\xE8\x34\xEC\xFD\xFF\x83\xC4\x04\x57\x68\xBB\x13\x00\x00\xE8\x54\x7F\x02\x00\x83\xC4\x08\x50\xE8\xC3\xD3\x05\x00\x83\xC4\x04\x85\xC0\xB8\xBD\x13\x00\x00\x74\x2B\x56\xE8\xBB\x0A\x06\x00\x83\xC4\x04\x50\x57\xE8\xD2\xC6\xFF\xFF\x83\xC4\x08\x57\x56\xE8\x78\x11\x06\x00\x83\xC4\x04\x50\xE8\x28\x6C\xFC\xFF\x83\xC4\x08\xB8\xBC\x13\x00\x00\x6A\x00\x53\x57\xBE\xA8\x9A\x4D\x00\x50\xE8\x2C\x7F\x02\x00\x83\xC4\x0C\x8B\xCE\x50\xE8\x9B\xD8\x05\x00\x6A\x02\xE8\xC5\xEB\xFD\xFF\x83\xC4\x04\x8B\xCE";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU2, szCmd2, sizeof(szCmd2)-1);

		/*
		00444689  |. 0F84 9D000000  JE      TENSHOU.0044472C
		*/
		char szCmd3[] = "\x0F\x84\x9D\x00\x00\x00";
		WritePatchAppearanceHimeBecomeBushou(CMD_APPEARANCE_HIME_BECOME_BUSHOU3, szCmd3, sizeof(szCmd3)-1);


	}
}