#pragma once

#pragma pack(1)

// ���C����ʂ�iBushouID�̕��������S�B�S��Ԃ̕����Ή��B
int FuncDoDeathInMain(int iBushouID);

// �푈��ʂ�iBushouID�����S�BActiveBushouID�����s�ҁB���s�҂����Ȃ��̂ł���΁A�w�肵�Ȃ��Ƃ��ǂ��B
int FuncDoDeathinBattle(int iKilledBushouID, int iCriminalBushouID=-1);
