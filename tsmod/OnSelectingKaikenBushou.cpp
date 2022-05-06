/*
	会見する武将を選択した時
*/

/*
	個々の会見のアルゴリズムデータは、
	セーブデータの0x10E40から保存されている。
	ほとんどは「24」になっているところを「01」などにすると、「開墾」となる。
	その次の「08」などになっているのがチラチラしているのは、おそらく該当思想の「深さ」か何か。

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
各ＡＩ文字列へのポインタ
004D14B8   38 00 4D 00 40 FF 4C 00 6C BC 4B 00 38 FF 4C 00  8.M.@�L.lｼK.8�L.
004D14C8   30 FF 4C 00 10 00 4D 00 7C B4 4B 00 0C BC 4B 00  0�L..M.|ｴK..ｼK.
004D14D8   3C FD 4C 00 30 FD 4C 00 24 FD 4C 00              <�L.0�L.$�L.


参照データ ： TENSHOU:.text -> 004D14B8..004D14B8
ｱﾄﾞﾚｽ      ﾃﾞｨｽｱｾﾝﾌﾞﾙ                                ｺﾒﾝﾄ
0043938C   MOV     EAX, DWORD PTR DS:[EAX*4+4D14B8]
0045E4B6   PUSH    DWORD PTR DS:[ECX*4+4D14B8]
0048823C   PUSH    DWORD PTR DS:[EAX*4+4D14B8]
*/

/*
00439383   > 56             PUSH    ESI
00439384   . E8 026F0300    CALL    TENSHOU.0047028B		← この関数の結果のEAXが、ＡＩの番号(５なら戦争)
00439389   . 83C4 04        ADD     ESP, 4
0043938C   . 8B0485 B8144D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D14B8]                ;  TENSHOU.004D0010
*/

