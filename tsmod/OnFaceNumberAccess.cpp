#include "GameDataStruct.h"

/*
 * 武将や姫の顔グラの番号にアクセスした(グラフィックを年齢別に分ける際に最後にアクセスしたやつを調べておく)
 */

/*
004091C4  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
004091C8  |. E8 F0FEFFFF    CALL    TENSHOU.004090BD
004091CD  |. 0FB740 0E      MOVZX   EAX, WORD PTR DS:[EAX+E]
004091D1  |. 83C4 04        ADD     ESP, 4
004091D4  \. C3             RETN
*/
