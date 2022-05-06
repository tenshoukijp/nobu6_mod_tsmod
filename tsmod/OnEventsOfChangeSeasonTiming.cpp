#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"

/*
季節切り替わり時のイベント

00441B14   . E8 6F000000    CALL    TENSHOU.00441B88		← 寿命で武将が死んだ
00441B19   > E8 46030000    CALL    TENSHOU.00441E64		
00441B1E   . E8 1F220000    CALL    TENSHOU.00443D42		← 新たに武将が加わった
00441B23   . E8 0EB00000    CALL    TENSHOU.0044CB36
00441B28   . E8 072C0000    CALL    TENSHOU.00444734
00441B2D   . E8 1A180000    CALL    TENSHOU.0044334C
00441B32   . E8 C1070000    CALL    TENSHOU.004422F8

*/