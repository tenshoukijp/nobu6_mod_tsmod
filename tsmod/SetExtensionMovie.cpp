#include "WinTarget.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "GameDataStruct.h"
#include "CommonGlobalHandle.h"
#include "SetExtensionMovie.h"

/*
0043CD64  /$ 55             PUSH    EBP
0043CD65  |. 33C0           XOR     EAX, EAX
0043CD67  |. 8BEC           MOV     EBP, ESP
0043CD69  |. 81EC 18010000  SUB     ESP, 118
0043CD6F  |. 53             PUSH    EBX
0043CD70  |. 8945 EC        MOV     DWORD PTR SS:[EBP-14], EAX
0043CD73  |. 56             PUSH    ESI
0043CD74  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
0043CD77  |. C745 F4 400100>MOV     DWORD PTR SS:[EBP-C], 140
0043CD7E  |. C745 F8 F00000>MOV     DWORD PTR SS:[EBP-8], 0F0
0043CD85  |. 57             PUSH    EDI
0043CD86  |. 8B7D 08        MOV     EDI, DWORD PTR SS:[EBP+8]
0043CD89  |. 3BF8           CMP     EDI, EAX
0043CD8B  |. 0F8E A1010000  JLE     TENSHOU.0043CF32
0043CD91  |. 83FF 24        CMP     EDI, 24
0043CD94  |. 0F8D 98010000  JGE     TENSHOU.0043CF32
0043CD9A  |. 83FF 01        CMP     EDI, 1
0043CD9D  |. 7C 05          JL      SHORT TENSHOU.0043CDA4
0043CD9F  |. 83FF 03        CMP     EDI, 3
0043CDA2  |. 7E 21          JLE     SHORT TENSHOU.0043CDC5
0043CDA4  |> 833D 38C24B00 >CMP     DWORD PTR DS:[4BC238], 0
0043CDAB  |. 0F84 81010000  JE      TENSHOU.0043CF32
0043CDB1  |. E8 41A20200    CALL    TENSHOU.00466FF7
0043CDB6  |. 85C0           TEST    EAX, EAX
0043CDB8  |. 0F85 74010000  JNZ     TENSHOU.0043CF32
0043CDBE  |. BE 01000000    MOV     ESI, 1
0043CDC3  |. EB 02          JMP     SHORT TENSHOU.0043CDC7
0043CDC5  |> 33F6           XOR     ESI, ESI
0043CDC7  |> BB 80954C00    MOV     EBX, TENSHOU.004C9580            ;  ASCII "T:\"
0043CDCC  |> 6A 01          /PUSH    1                               ; /ErrorMode = SEM_FAILCRITICALERRORS
0043CDCE  |. FF15 18754E00  |CALL    DWORD PTR DS:[<&KERNEL32.SetErr>; \SetErrorMode
0043CDD4  |. 53             |PUSH    EBX                             ; /Arg1
0043CDD5  |. 8945 FC        |MOV     DWORD PTR SS:[EBP-4], EAX       ; |
0043CDD8  |. E8 1CCD0200    |CALL    TENSHOU.00469AF9                ; \TENSHOU.00469AF9
0043CDDD  |. 83C4 04        |ADD     ESP, 4
0043CDE0  |. 85C0           |TEST    EAX, EAX
0043CDE2  |. 74 30          |JE      SHORT TENSHOU.0043CE14
0043CDE4  |. FF34BD 88954C0>|PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
0043CDEB  |. 53             |PUSH    EBX                             ; |<%s>
0043CDEC  |. 68 17F4C001    |PUSH    1C0F417                         ; |Format = 01C0F417 ???


004C9448                          68 6F 6E 6E 6F 6A 69 00          noji.
004C9458  63 68 69 6B 75 6A 6F 00 6B 61 69 6B 65 6E 00 00  chikujo.kaiken..
004C9468  6B 69 63 68 6F 00 00 00 73 68 75 6E 69 6E 00 00  kicho...shunin..
004C9478  63 64 65 6E 72 61 69 00 74 64 65 6E 72 61 69 00  cdenrai.tdenrai.
004C9488  6B 6F 6B 65 69 00 00 00 73 68 69 62 6F 00 00 00  kokei...shibo...
004C9498  79 6F 6D 65 69 72 69 00 69 6E 6B 79 6F 00 00 00  yomeiri.inkyo...
004C94A8  6B 69 6B 69 6E 00 00 00 6F 79 75 6B 69 00 00 00  kikin...oyuki...
004C94B8  6B 79 6F 73 61 6B 75 00 68 6F 73 61 6B 75 00 00  kyosaku.hosaku..
004C94C8  6E 61 67 61 61 6D 65 00 68 69 64 65 72 69 00 00  nagaame.hideri..
004C94D8  65 6B 69 62 79 6F 00 00 69 6B 6B 6F 69 6B 6B 69  ekibyo..ikkoikki
004C94E8  00 00 00 00 6E 6F 69 6B 6B 69 00 00 73 68 75 70  ....noikki..shup
004C94F8  70 6F 6E 00 6D 75 68 6F 6E 00 00 00 73 75 69 72  pon.muhon...suir
004C9508  65 6E 00 00 74 65 70 70 6F 00 00 00 62 61 6A 75  en..teppo...baju
004C9518  74 73 75 00 73 75 6D 6F 00 00 00 00 74 61 6B 61  tsu.sumo....taka
004C9528  67 61 72 69 00 00 00 00 73 61 6E 6A 75 74 73 75  gari....sanjutsu
004C9538  00 00 00 00 68 65 69 67 61 6B 75 00 6B 65 6E 6A  ....heigaku.kenj
004C9548  75 74 73 75 00 00 00 00 6D 6F 6E 64 6F 00 00 00  utsu....mondo...
004C9558  73 61 64 6F 00 00 00 00 65 6E 64 69 6E 67 00 00  sado....ending..
004C9568  6F 70 65 6E 69 6E 67 00 6B 6F 65 69 6C 6F 67 6F  opening.koeilogo
004C9578  00 00 00 00 00 00 00 00 54 3A 5C 00 00 00 00 00  ........T:\.....
					　　　┌これがkoeilogoの文字列ポインタ
004C9588  00 00 00 00 [70 95 4C] 00 [68 95 4C] 00 60 95 4C 00  
004C9598  58 95 4C 00 50 95 4C 00 44 95 4C 00 3C 95 4C 00  
004C95A8  30 95 4C 00 24 95 4C 00 1C 95 4C 00 14 95 4C 00  
004C95B8  0C 95 4C 00 04 95 4C 00 FC 94 4C 00 F4 94 4C 00  
004C95C8  EC 94 4C 00 E0 94 4C 00 D8 94 4C 00 D0 94 4C 00  
004C95D8  C8 94 4C 00 C0 94 4C 00 B8 94 4C 00 B0 94 4C 00  
004C95E8  A8 94 4C 00 A0 94 4C 00 98 94 4C 00 90 94 4C 00  
004C95F8  88 94 4C 00 80 94 4C 00 78 94 4C 00 70 94 4C 00  
												┌これが最後のnojiの文字列ポインタ
004C9608  68 94 4C 00 60 94 4C 00 58 94 4C 00 [50 94 4C]   



00453208  |> 6A 01          PUSH    1                                ; /Repaint = TRUE
0045320A  |. FF75 FC        PUSH    DWORD PTR SS:[EBP-4]             ; |Height
0045320D  |. FF75 F8        PUSH    DWORD PTR SS:[EBP-8]             ; |Width
00453210  |. FF75 F4        PUSH    DWORD PTR SS:[EBP-C]             ; |Y
00453213  |. FF75 F0        PUSH    DWORD PTR SS:[EBP-10]            ; |X
00453216  |. FF35 90FD4D00  PUSH    DWORD PTR DS:[4DFD90]            ; |hWnd = 00191884
0045321C  |. FF15 A8754E00  CALL    DWORD PTR DS:[<&USER32.MoveWindo>; \MoveWindow
00453222  |. 6A 05          PUSH    5                                ; /ShowState = SW_SHOW
00453224  |. FF35 90FD4D00  PUSH    DWORD PTR DS:[4DFD90]            ; |hWnd = 00191884
0045322A  |. FF15 1C764E00  CALL    DWORD PTR DS:[<&USER32.ShowWindo>; \ShowWindow


// 以下の140(320)と0F0(240)がEBPベースのアドレスに送られることで、ムービーの秒だサイズが決まるようだ。
0043CD74  |. 8945 F0        MOV     DWORD PTR SS:[EBP-10], EAX
0043CD77  |. C745 F4 400100>MOV     DWORD PTR SS:[EBP-C], 140
0043CD7E  |. C745 F8 F00000>MOV     DWORD PTR SS:[EBP-8], 0F0
0043CD85  |. 57             PUSH    EDI

0043CDE0  |. 85C0           |TEST    EAX, EAX
0043CDE2  |. 74 30          |JE      SHORT TENSHOU.0043CE14
0043CDE4  |. FF34BD 88954C0>|PUSH    DWORD PTR DS:[EDI*4+4C9588]     ; /<%s>
														   └ここがポインタの場所 = GAMEDATASTRUCT_MOVIELISTPOINTER_ADDRESS
0043CDEB  |. 53             |PUSH    EBX                             ; |<%s> = "T:\"
0043CDEC  |.-E9 90F7CC01    |JMP     TSMod.JmpOnTSExeMovieWsprintfPu>; |Format
*/

// むーびは全部で36個であり、その中でやりくりする必要がある。

NB6MOVIEPOINTER		*const nb6_moviepointer_ex = (NB6MOVIEPOINTER *)GAMEDATASTRUCT_MOVIELISTPOINTER_ADDRESS;

// 拡張ムービーのポインタは最大で800個あるが、そのうち、最初にtenshou.exeにある分をコピーしてセットする。
void setExtensionMoviePointer() {
}



