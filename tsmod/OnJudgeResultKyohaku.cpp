/*
004A7530  |. E8 2345F9FF    |CALL    TENSHOU.0043BA58                ; \TENSHOU.0043BA58
(NPC������_)
0043BA58  /$ C705 40B64C00 >MOV     DWORD PTR DS:[4CB640], 1

0043BB00  |> E8 BF9DFEFF    CALL    TENSHOU.004258C4
(�Ō�\��)
0043BB00  |> E8 BF9DFEFF    CALL    TENSHOU.004258C4


004258C4  /$ 66:A1 F4E54B00 MOV     AX, WORD PTR DS:[4BE5F4]
(�����̌���ʃX�^�[�g�BNPC�����̉�ʂɂ͓���)

004258ED  |. E8 D9E6FFFF    CALL    TENSHOU.00423FCB
��ʐN��(NPC�̎��͉�ʂɂ͓���Ȃ�)

�f���EDI��FFFFFF�ɂȂ�B
004258F5  |. F7DF           NEG     EDI
*/

/*
004258ED  |. E8 D9E6FFFF    CALL    TENSHOU.00423FCB�@�@�@�����̊֐��̌��ʁA4BE604�ɒl����B
004258F2  |. 83C4 04        ADD     ESP, 4

�����O���̏�Ԃƌ��ʊi�[�̃������A�h���X������B
004BE5F4  E7 00 00 00 FF FF 00 00 19 00 00 00 FF FF 00 00  �E..��.....��..
004BE604  [00] 00 00 00 7B 1B 7C 1B 7D 1B 7E 1B 7F 1B 00 00  ....{|}~..
����[4BE604]�Ɂu�b�]�v���u���ہv���̕ԓ��̌��ʂ��i�[�B00�Ȃ�b�]�A01�Ȃ狑�ہBPC/NPC���Ȃ��B��Ăł�05�Ƃ������l�B
*/
