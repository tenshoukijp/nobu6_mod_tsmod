/*
004A7530  |. E8 2345F9FF    |CALL    TENSHOU.0043BA58                ; \TENSHOU.0043BA58
(NPC脅迫基点)
0043BA58  /$ C705 40B64C00 >MOV     DWORD PTR DS:[4CB640], 1

0043BB00  |> E8 BF9DFEFF    CALL    TENSHOU.004258C4
(最後表示)
0043BB00  |> E8 BF9DFEFF    CALL    TENSHOU.004258C4


004258C4  /$ 66:A1 F4E54B00 MOV     AX, WORD PTR DS:[4BE5F4]
(脅迫の交渉画面スタート。NPCもこの画面には入る)

004258ED  |. E8 D9E6FFFF    CALL    TENSHOU.00423FCB
画面侵入(NPCの時は画面には入らない)

断るとEDIがFFFFFFになる。
004258F5  |. F7DF           NEG     EDI
*/

/*
004258ED  |. E8 D9E6FFFF    CALL    TENSHOU.00423FCB　　　←この関数の結果、4BE604に値入る。
004258F2  |. 83C4 04        ADD     ESP, 4

脅迫外交の状態と結果格納のメモリアドレスあたり。
004BE5F4  E7 00 00 00 FF FF 00 00 19 00 00 00 FF FF 00 00  ・..��.....��..
004BE604  [00] 00 00 00 7B 1B 7C 1B 7D 1B 7E 1B 7F 1B 00 00  ....{|}~..
この[4BE604]に「臣従」か「拒否」かの返答の結果を格納。00なら臣従、01なら拒否。PC/NPC問わない。代案では05といった値。
*/
