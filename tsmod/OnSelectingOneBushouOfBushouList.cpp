/*
004DBD40  68 00 66 00 67 00 69 00 FF FF FF FF 7F 00 80 00  h.f.g.i.����.�.
004DBD50  FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00  ����............
004DBD60  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00     ...............
*/



/*
�������X�g�I����ʂŁA�N���P�l�I�ԂƎ��s�����B

�Ȃ��AEAX�ɂ́A���Ԗڂ̐l��I�񂾂��������Ă���B
���̉��Ԗڂ̐l���Ă̂́A���0x4DB40�̕����̐l�B
*/

/*
004A8853   53               PUSH    EBX
004A8854   56               PUSH    ESI
004A8855   57               PUSH    EDI
004A8856   BE 06000000      MOV     ESI, 6
004A885B   8D99 E4020000    LEA     EBX, DWORD PTR DS:[ECX+2E4]
004A8861   8BF9             MOV     EDI, ECX
004A8863   E8 5CFEFFFF      CALL    TENSHOU.004A86C4
004A8868   8B03             /MOV     EAX, DWORD PTR DS:[EBX]
004A886A   8BCB             |MOV     ECX, EBX
004A886C   83C3 34          |ADD     EBX, 34
004A886F   FF50 10          |CALL    DWORD PTR DS:[EAX+10]
004A8872   4E               |DEC     ESI
004A8873  ^75 F3            \JNZ     SHORT TENSHOU.004A8868
004A8875   81C7 1C040000    ADD     EDI, 41C
004A887B   BE 02000000      MOV     ESI, 2
004A8880   8B07             /MOV     EAX, DWORD PTR DS:[EDI]
004A8882   8BCF             |MOV     ECX, EDI
*/
