#include <windows.h>

//////////////////////////////////////////////////////////////////////////////
//getFileName
//�t���p�X������t�@�C�������擾����
//////////////////////////////////////////////////////////////////////////////
/*
�����F  char *lpszPath    �t�@�C�������܂ރp�X���ւ̃|�C���^

�߂�l�Fchar * �t�@�C�����ւ̃|�C���^
�@�@�@�@�@�@�@�t�@�C�������܂܂Ȃ��Ƃ���""�ւ̃|�C���^
    [\],[/],[:]��������Ȃ������ꍇ�A�������t�@�C�����Ƃ݂Ȃ��Ă��̂܂ܕԂ�
*/
char *GetFileNameWithOutDirName(char *lpszPath)
{
    char* lpszPtr=lpszPath;

    while(*lpszPtr != '\0')
    {
        //�Q�o�C�g�����̐擪�̓X�L�b�v
        if(IsDBCSLeadByte(*lpszPtr) == 0)
        {
            //[\],[/],[:]���������猻�ݒn+1�̃|�C���^��ۑ�
            if((*lpszPtr == '\\') || (*lpszPtr == '/') || (*lpszPtr == ':'))
            {
                lpszPath=lpszPtr+1;
            }
        }
        //���̕�����
        lpszPtr=CharNext(lpszPtr);
    }
    return lpszPath;
}