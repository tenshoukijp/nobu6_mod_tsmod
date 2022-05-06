#pragma once

#pragma pack(1)

// メイン画面でiBushouIDの武将が死亡。全状態の武将対応。
int FuncDoDeathInMain(int iBushouID);

// 戦争画面でiBushouIDが死亡。ActiveBushouIDが実行者。実行者が居ないのであれば、指定しなくとも良い。
int FuncDoDeathinBattle(int iKilledBushouID, int iCriminalBushouID=-1);
