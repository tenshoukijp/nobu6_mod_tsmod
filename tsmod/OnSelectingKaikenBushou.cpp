/*
	‰ïŒ©‚·‚é•«‚ð‘I‘ð‚µ‚½Žž
*/

/*
	ŒÂX‚Ì‰ïŒ©‚ÌƒAƒ‹ƒSƒŠƒYƒ€ƒf[ƒ^‚ÍA
	ƒZ[ƒuƒf[ƒ^‚Ì0x10E40‚©‚ç•Û‘¶‚³‚ê‚Ä‚¢‚éB
	‚Ù‚Æ‚ñ‚Ç‚Íu24v‚É‚È‚Á‚Ä‚¢‚é‚Æ‚±‚ë‚ðu01v‚È‚Ç‚É‚·‚é‚ÆAuŠJ¤v‚Æ‚È‚éB
	‚»‚ÌŽŸ‚Ìu08v‚È‚Ç‚É‚È‚Á‚Ä‚¢‚é‚Ì‚ªƒ`ƒ‰ƒ`ƒ‰‚µ‚Ä‚¢‚é‚Ì‚ÍA‚¨‚»‚ç‚­ŠY“–Žv‘z‚Ìu[‚³v‚©‰½‚©B

/*
0045E3B7   53               PUSH    EBX
0045E3B8   56               PUSH    ESI
0045E3B9   57               PUSH    EDI
0045E3BA   8BF1             MOV     ESI, ECX
0045E3BC   55               PUSH    EBP
0045E3BD   E8 EDED0100      CALL    TENSHOU.0047D1AF
0045E3C2   8DBE 18010000    LEA     EDI, DWORD PTR DS:[ESI+118]
0045E3C8   66:8B07          MOV     AX, WORD PTR DS:[EDI]
0045E3CB   50               PUSH    EAX
0045E3CC   6A 40            PUSH    40
*/

/*
Še‚`‚h•¶Žš—ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
004D14B8   38 00 4D 00 40 FF 4C 00 6C BC 4B 00 38 FF 4C 00  8.M.@ÿL.l¼K.8ÿL.
004D14C8   30 FF 4C 00 10 00 4D 00 7C B4 4B 00 0C BC 4B 00  0ÿL..M.|´K..¼K.
004D14D8   3C FD 4C 00 30 FD 4C 00 24 FD 4C 00              <ýL.0ýL.$ýL.


ŽQÆƒf[ƒ^ F TENSHOU:.text -> 004D14B8..004D14B8
±ÄÞÚ½      ÃÞ¨½±¾ÝÌÞÙ                                ºÒÝÄ
0043938C   MOV     EAX, DWORD PTR DS:[EAX*4+4D14B8]
0045E4B6   PUSH    DWORD PTR DS:[ECX*4+4D14B8]
0048823C   PUSH    DWORD PTR DS:[EAX*4+4D14B8]
*/

/*
00439383   > 56             PUSH    ESI
00439384   . E8 026F0300    CALL    TENSHOU.0047028B		© ‚±‚ÌŠÖ”‚ÌŒ‹‰Ê‚ÌEAX‚ªA‚`‚h‚Ì”Ô†(‚T‚È‚çí‘ˆ)
00439389   . 83C4 04        ADD     ESP, 4
0043938C   . 8B0485 B8144D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D14B8]                ;  TENSHOU.004D0010
*/

