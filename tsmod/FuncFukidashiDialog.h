#pragma once

#pragma pack(1)

// �����o���I�[�v��
void FuncLeftNormalFukidashiMessage(int iBushouID, char *szMessage);
void FuncRightNormalFukidashiMessage(int iBushouID, char *szMessage);
// �����o���̍X�V
void FuncRightNormalFukidashiUpdate(int iBushouID, char *szMessage);
void FuncLeftNormalFukidashiUpdate(int iBushouID, char *szMessage);
// �����o������
void FuncLeftNormalFukidashiDelete();
void FuncRightNormalFukidashiDelete();

/*
void FuncCenterNormalFukidashiMessage(int iBushouID, char *szMessage);
*/