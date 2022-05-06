/*
í“¬’†‚ÉŽ€–S‚·‚éŽž‚É‚ÍA‚±‚±‚ð’Ê‰ß‚·‚éB

004316B8  /$ 55             PUSH    EBP
004316B9  |. 8BEC           MOV     EBP, ESP
004316BB  |. 56             PUSH    ESI
004316BC  |. 57             PUSH    EDI
004316BD  |. E8 26510600    CALL    TENSHOU.004967E8
004316C2  |. 85C0           TEST    EAX, EAX
004316C4  |. 75 3C          JNZ     SHORT TENSHOU.00431702
004316C6  |. 66:8B7D 0C     MOV     DI, WORD PTR SS:[EBP+C]
004316CA  |. 66:8B75 08     MOV     SI, WORD PTR SS:[EBP+8]
004316CE  |. 57             PUSH    EDI                              ; /Arg3
004316CF  |. 56             PUSH    ESI                              ; |Arg2
004316D0  |. 68 62100000    PUSH    1062                             ; |Arg1 = 00001062
004316D5  |. E8 A5AD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316DA  |. 83C4 0C        ADD     ESP, 0C
004316DD  |. 50             PUSH    EAX
004316DE  |. 56             PUSH    ESI
004316DF  |. E8 638F0600    CALL    TENSHOU.0049A647
004316E4  |. 83C4 08        ADD     ESP, 8
004316E7  |. 56             PUSH    ESI                              ; /Arg3
004316E8  |. 57             PUSH    EDI                              ; |Arg2
004316E9  |. 68 68100000    PUSH    1068                             ; |Arg1 = 00001068
004316EE  |. E8 8CAD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
004316F3  |. 83C4 0C        ADD     ESP, 0C
004316F6  |. 50             PUSH    EAX
004316F7  |. 57             PUSH    EDI
004316F8  |. E8 4A8F0600    CALL    TENSHOU.0049A647
004316FD  |. 83C4 08        ADD     ESP, 8
00431700  |. EB 08          JMP     SHORT TENSHOU.0043170A
00431702  |> 66:8B75 08     MOV     SI, WORD PTR SS:[EBP+8]
00431706  |. 66:8B7D 0C     MOV     DI, WORD PTR SS:[EBP+C]
0043170A  |> 68 FFFF0000    PUSH    0FFFF
0043170F  |. 6A 01          PUSH    1
00431711  |. 57             PUSH    EDI                              ; /Arg3
00431712  |. 56             PUSH    ESI                              ; |Arg2
00431713  |. 68 63100000    PUSH    1063                             ; |Arg1 = 00001063
00431718  |. E8 62AD0300    CALL    TENSHOU.0046C47F                 ; \TENSHOU.0046C47F
0043171D  |. 83C4 0C        ADD     ESP, 0C
00431720  |. 50             PUSH    EAX
00431721  |. E8 79320700    CALL    TENSHOU.004A499F
00431726  |. 83C4 0C        ADD     ESP, 0C
00431729  |. 6A 02          PUSH    2                                ; /Arg3 = 00000002
0043172B  |. 57             PUSH    EDI                              ; |Arg2
0043172C  |. 56             PUSH    ESI                              ; |Arg1
0043172D  |. E8 43F3FFFF    CALL    TENSHOU.00430A75                 ; \TENSHOU.00430A75
00431732  |. 83C4 0C        ADD     ESP, 0C
00431735  |. 6A 03          PUSH    3                                ; /Arg3 = 00000003
00431737  |. 56             PUSH    ESI                              ; |Arg2
00431738  |. 57             PUSH    EDI                              ; |Arg1
00431739  |. E8 73F1FFFF    CALL    TENSHOU.004308B1                 ; \TENSHOU.004308B1
0043173E  |. 83C4 0C        ADD     ESP, 0C
00431741  |. 68 00040000    PUSH    400
00431746  |. 56             PUSH    ESI
00431747  |. E8 068CFDFF    CALL    TENSHOU.0040A352
0043174C  |. 83C4 08        ADD     ESP, 8
0043174F  |. 5F             POP     EDI
00431750  |. 5E             POP     ESI
00431751  |. 5D             POP     EBP
00431752  \. C3             RETN
*/