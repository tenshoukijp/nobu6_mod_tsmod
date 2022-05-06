#include <windows.h>

//////////////////////////////////////////////////////////////////////////////
//getFileName
//フルパス名からファイル名を取得する
//////////////////////////////////////////////////////////////////////////////
/*
引数：  char *lpszPath    ファイル名を含むパス名へのポインタ

戻り値：char * ファイル名へのポインタ
　　　　　　　ファイル名を含まないときは""へのポインタ
    [\],[/],[:]が見つからなかった場合、引数をファイル名とみなしてそのまま返す
*/
char *GetFileNameWithOutDirName(char *lpszPath)
{
    char* lpszPtr=lpszPath;

    while(*lpszPtr != '\0')
    {
        //２バイト文字の先頭はスキップ
        if(IsDBCSLeadByte(*lpszPtr) == 0)
        {
            //[\],[/],[:]を見つけたら現在地+1のポインタを保存
            if((*lpszPtr == '\\') || (*lpszPtr == '/') || (*lpszPtr == ':'))
            {
                lpszPath=lpszPtr+1;
            }
        }
        //次の文字へ
        lpszPtr=CharNext(lpszPtr);
    }
    return lpszPath;
}