/*
00449906  |. 68 50FD4D00    PUSH    TENSHOU.004DFD50                 ; /pRect2 = 004DFD50 {368.,264.,568.,376.}
0044990B  |. 894D F4        MOV     DWORD PTR SS:[EBP-C], ECX        ; |
0044990E  |. 034D 14        ADD     ECX, DWORD PTR SS:[EBP+14]       ; |
00449911  |. BB 08FF4D00    MOV     EBX, TENSHOU.004DFF08            ; |
00449916  |. 8945 F8        MOV     DWORD PTR SS:[EBP-8], EAX        ; |
00449919  |. 894D FC        MOV     DWORD PTR SS:[EBP-4], ECX        ; |
0044991C  |. 8D45 F0        LEA     EAX, DWORD PTR SS:[EBP-10]       ; |
0044991F  |. 50             PUSH    EAX                              ; |pRect1
00449920  |. 53             PUSH    EBX                              ; |pRectDst => TENSHOU.004DFF08
00449921  |. FF15 F4754E00  CALL    DWORD PTR DS:[<&USER32.Intersect>; \IntersectRect

‰Æ•ó‚ª•`‰æ‚³‚ê‚éuŠÔ‚±‚±‚ð’Ê‚éB
*/
