
// 戦争画面時の「勢力」ボタンの画面

/*
0046471F  |. FF75 10        PUSH    DWORD PTR SS:[EBP+10]            ; /Arg3			← 総数？
00464722  |. FF75 08        PUSH    DWORD PTR SS:[EBP+8]             ; |Arg2			← 例の順序の番号
00464725  |. 8B7D 0C        MOV     EDI, DWORD PTR SS:[EBP+C]        ; |
00464728  |. 57             PUSH    EDI                              ; |Arg1			← ただの座標？
00464729  |. E8 A2FDFFFF    CALL    TENSHOU.004644D0                 ; \TENSHOU.004644D0
*/

/*
004644D0  /$ 83EC 08        SUB     ESP, 8
004644D3  |. 53             PUSH    EBX
004644D4  |. 56             PUSH    ESI
004644D5  |. 8B7424 1C      MOV     ESI, DWORD PTR SS:[ESP+1C]
004644D9  |. 57             PUSH    EDI
004644DA  |. 66:8B7C24 1C   MOV     DI, WORD PTR SS:[ESP+1C]
004644DF  |. 55             PUSH    EBP
004644E0  |. 6A 00          PUSH    0
004644E2  |. 8BC6           MOV     EAX, ESI
004644E4  |. 80CC 01        OR      AH, 1
004644E7  |. 50             PUSH    EAX
004644E8  |. 57             PUSH    EDI
004644E9  |. E8 B976FEFF    CALL    TENSHOU.0044BBA7
004644EE  |. 894424 1C      MOV     DWORD PTR SS:[ESP+1C], EAX
004644F2  |. 83C4 0C        ADD     ESP, 0C
004644F5  |. 8BC6           MOV     EAX, ESI
004644F7  |. 6A 00          PUSH    0
004644F9  |. 80CC 03        OR      AH, 3
004644FC  |. 50             PUSH    EAX
004644FD  |. 57             PUSH    EDI
004644FE  |. BD F8674E00    MOV     EBP, TENSHOU.004E67F8                                ;  ASCII "ｰVK"
00464503  |. E8 9F76FEFF    CALL    TENSHOU.0044BBA7
00464508  |. 8B5C24 28      MOV     EBX, DWORD PTR SS:[ESP+28]
0046450C  |. 894424 20      MOV     DWORD PTR SS:[ESP+20], EAX
00464510  |. 83C4 0C        ADD     ESP, 0C
00464513  |. 8BCD           MOV     ECX, EBP
00464515  |. 53             PUSH    EBX
00464516  |. 6A 10          PUSH    10
00464518  |. E8 6BCFF9FF    CALL    TENSHOU.00401488
0046451D  |. 6A 04          PUSH    4
0046451F  |. 8BCD           MOV     ECX, EBP
00464521  |. 6A 0F          PUSH    0F
00464523  |. 6A 0F          PUSH    0F
00464525  |. E8 F29D0400    CALL    TENSHOU.004AE31C
0046452A  |. 83FE 04        CMP     ESI, 4
0046452D  |. 75 0D          JNZ     SHORT TENSHOU.0046453C
0046452F  |. 68 3CB44B00    PUSH    TENSHOU.004BB43C                                     ;  ASCII "中立"
00464534  |. 68 B0C04C00    PUSH    TENSHOU.004CC0B0                                     ;  ASCII "%s勢力"
00464539  |. 55             PUSH    EBP
0046453A  |. EB 2F          JMP     SHORT TENSHOU.0046456B
0046453C  |> 57             PUSH    EDI
0046453D  |. E8 66200000    CALL    TENSHOU.004665A8
00464542  |. 83C4 04        ADD     ESP, 4
00464545  |. 85C0           TEST    EAX, EAX
00464547  |. 75 08          JNZ     SHORT TENSHOU.00464551
00464549  |. 66:A1 CC4C4D00 MOV     AX, WORD PTR DS:[4D4CCC]
0046454F  |. EB 06          JMP     SHORT TENSHOU.00464557
00464551  |> 66:A1 4C5D4D00 MOV     AX, WORD PTR DS:[4D5D4C]
00464557  |> 50             PUSH    EAX													← このEAXは対象の「大名番号」
00464558  |. E8 EC0A0400    CALL    TENSHOU.004A5049
0046455D  |. 83C4 04        ADD     ESP, 4
00464560  |. 50             PUSH    EAX
00464561  |. 68 B0C04C00    PUSH    TENSHOU.004CC0B0                                     ;  ASCII "%s勢力"
00464566  |. 68 F8674E00    PUSH    TENSHOU.004E67F8                                     ;  ASCII "ｰVK"
0046456B  |> 33FF           XOR     EDI, EDI
0046456D  |. E8 3EA00400    CALL    TENSHOU.004AE5B0
00464572  |. 83C4 0C        ADD     ESP, 0C
00464575  |. B9 E8C54D00    MOV     ECX, TENSHOU.004DC5E8
0046457A  |. 6A 04          PUSH    4
0046457C  |. 57             PUSH    EDI
0046457D  |. 68 80EB0000    PUSH    0EB80
00464582  |. BD F8674E00    MOV     EBP, TENSHOU.004E67F8                                ;  ASCII "ｰVK"
00464587  |. E8 796C0400    CALL    TENSHOU.004AB205
0046458C  |. 8BF0           MOV     ESI, EAX
0046458E  |. 57             PUSH    EDI
0046458F  |. 57             PUSH    EDI
00464590  |. 8BCD           MOV     ECX, EBP
00464592  |. 57             PUSH    EDI
00464593  |. E8 9218FAFF    CALL    TENSHOU.00405E2A
00464598  |. 8D86 80070000  LEA     EAX, DWORD PTR DS:[ESI+780]
0046459E  |. 50             PUSH    EAX                                                  ; /Arg7
0046459F  |. 57             PUSH    EDI                                                  ; |Arg6
004645A0  |. 57             PUSH    EDI                                                  ; |Arg5
004645A1  |. 8BCD           MOV     ECX, EBP                                             ; |
004645A3  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
004645A5  |. 6A 30          PUSH    30                                                   ; |Arg3 = 00000030
004645A7  |. 53             PUSH    EBX                                                  ; |Arg2
004645A8  |. 68 A8000000    PUSH    0A8                                                  ; |Arg1 = 000000A8
004645AD  |. E8 79980400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
004645B2  |. 53             PUSH    EBX
004645B3  |. 8BCD           MOV     ECX, EBP
004645B5  |. 68 E0000000    PUSH    0E0
004645BA  |. E8 C9CEF9FF    CALL    TENSHOU.00401488
004645BF  |. FF7424 10      PUSH    DWORD PTR SS:[ESP+10]
004645C3  |. 68 E4C14C00    PUSH    TENSHOU.004CC1E4                                     ;  ASCII "%2u"
004645C8  |. 55             PUSH    EBP
004645C9  |. E8 E29F0400    CALL    TENSHOU.004AE5B0
004645CE  |. 83C4 0C        ADD     ESP, 0C
004645D1  |. 8D86 00060000  LEA     EAX, DWORD PTR DS:[ESI+600]
004645D7  |. 50             PUSH    EAX                                                  ; /Arg7
004645D8  |. 57             PUSH    EDI                                                  ; |Arg6
004645D9  |. 57             PUSH    EDI                                                  ; |Arg5
004645DA  |. 8BCD           MOV     ECX, EBP                                             ; |
004645DC  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
004645DE  |. 6A 30          PUSH    30                                                   ; |Arg3 = 00000030
004645E0  |. 53             PUSH    EBX                                                  ; |Arg2
004645E1  |. 68 08010000    PUSH    108                                                  ; |Arg1 = 00000108
004645E6  |. E8 40980400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
004645EB  |. 53             PUSH    EBX
004645EC  |. 8BCD           MOV     ECX, EBP
004645EE  |. 68 40010000    PUSH    140
004645F3  |. E8 90CEF9FF    CALL    TENSHOU.00401488
004645F8  |. FF7424 14      PUSH    DWORD PTR SS:[ESP+14]
004645FC  |. 68 58B94B00    PUSH    TENSHOU.004BB958                                     ;  ASCII "%4u"
00464601  |. 55             PUSH    EBP
00464602  |. 83C3 14        ADD     EBX, 14
00464605  |. E8 A69F0400    CALL    TENSHOU.004AE5B0
0046460A  |. 83C4 0C        ADD     ESP, 0C
0046460D  |. 56             PUSH    ESI                                                  ; /Arg7
0046460E  |. 57             PUSH    EDI                                                  ; |Arg6
0046460F  |. 57             PUSH    EDI                                                  ; |Arg5
00464610  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
00464612  |. 6A 20          PUSH    20                                                   ; |Arg3 = 00000020
00464614  |. 53             PUSH    EBX                                                  ; |Arg2
00464615  |. 6A 20          PUSH    20                                                   ; |Arg1 = 00000020
00464617  |. 8BCD           MOV     ECX, EBP                                             ; |
00464619  |. E8 0D980400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
0046461E  |. 8D86 00010000  LEA     EAX, DWORD PTR DS:[ESI+100]
00464624  |. 50             PUSH    EAX                                                  ; /Arg7
00464625  |. 57             PUSH    EDI                                                  ; |Arg6
00464626  |. 57             PUSH    EDI                                                  ; |Arg5
00464627  |. 8BCD           MOV     ECX, EBP                                             ; |
00464629  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
0046462B  |. 6A 40          PUSH    40                                                   ; |Arg3 = 00000040
0046462D  |. 53             PUSH    EBX                                                  ; |Arg2
0046462E  |. 6A 78          PUSH    78                                                   ; |Arg1 = 00000078
00464630  |. E8 F6970400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
00464635  |. 8D8E 00030000  LEA     ECX, DWORD PTR DS:[ESI+300]
0046463B  |. 51             PUSH    ECX                                                  ; /Arg7
0046463C  |. 57             PUSH    EDI                                                  ; |Arg6
0046463D  |. 57             PUSH    EDI                                                  ; |Arg5
0046463E  |. 8BCD           MOV     ECX, EBP                                             ; |
00464640  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
00464642  |. 6A 20          PUSH    20                                                   ; |Arg3 = 00000020
00464644  |. 53             PUSH    EBX                                                  ; |Arg2
00464645  |. 68 D8000000    PUSH    0D8                                                  ; |Arg1 = 000000D8
0046464A  |. E8 DC970400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
0046464F  |. 8D8E 00040000  LEA     ECX, DWORD PTR DS:[ESI+400]
00464655  |. 51             PUSH    ECX                                                  ; /Arg7
00464656  |. 57             PUSH    EDI                                                  ; |Arg6
00464657  |. 57             PUSH    EDI                                                  ; |Arg5
00464658  |. 8BCD           MOV     ECX, EBP                                             ; |
0046465A  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
0046465C  |. 81C6 00050000  ADD     ESI, 500                                             ; |
00464662  |. 6A 20          PUSH    20                                                   ; |Arg3 = 00000020
00464664  |. 53             PUSH    EBX                                                  ; |Arg2
00464665  |. 68 08010000    PUSH    108                                                  ; |Arg1 = 00000108
0046466A  |. E8 BC970400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
0046466F  |. 56             PUSH    ESI                                                  ; /Arg7
00464670  |. 57             PUSH    EDI                                                  ; |Arg6
00464671  |. 57             PUSH    EDI                                                  ; |Arg5
00464672  |. 8BCD           MOV     ECX, EBP                                             ; |
00464674  |. 6A 10          PUSH    10                                                   ; |Arg4 = 00000010
00464676  |. 6A 20          PUSH    20                                                   ; |Arg3 = 00000020
00464678  |. 53             PUSH    EBX                                                  ; |Arg2
00464679  |. 68 38010000    PUSH    138                                                  ; |Arg1 = 00000138
0046467E  |. E8 A8970400    CALL    TENSHOU.004ADE2B                                     ; \TENSHOU.004ADE2B
00464683  |. 5D             POP     EBP
00464684  |. 5F             POP     EDI
00464685  |. 5E             POP     ESI
00464686  |. 5B             POP     EBX
00464687  |. 83C4 08        ADD     ESP, 8
0046468A  \. C2 0C00        RETN    0C
*/
