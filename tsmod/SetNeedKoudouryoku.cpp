#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

#include "SetNeedKoudouryoku.h"

/*
　各種行動に必要な値の設定。神パッチよりの移植。
 */


void SetNewNeedKoudouryokuValue( int iAddress, char Value ) {
	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1バイトのみ書き込む
}


void InitNeedKoudouryoku() {

	/*
	 *******************
		不明なコマンドの行動力を調べる時は、「OnSwitchMainScreenSubCommand.cpp」に記載されている場所からブレイクを張ってジャンプしていけばだいたいＯＫ
	 *******************
	 */

	// 「講義」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKougi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKougi );

	// 「披露」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHirou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHirou );

	// 「編成」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHensei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHensei );

	// 「徴兵」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChohei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChohei );

	// 「登用」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTouyou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTouyou );

	// 「褒美」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );

	// 「物資」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );

	// 「戦争」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueSensou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueSensou );

	// 「同盟」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueDoumei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueDoumei );

	// 「婚姻」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKonin );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKonin );

	// 「手切れ」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTegire );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTegire );

	// 「脅迫」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku );

	// 「朝廷」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChotei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChotei );
	
	// 「内応」の値
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueNaiou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueNaiou );
	
	// 「謀略」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_04, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_05, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_06, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_07, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );

	// 「評定」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo );

	// 「軍団」->「任命」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei );
	
	// 「軍団」->「再任」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// 「軍団」->「分配」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// 「軍団」->「合流」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// 「内政・訓練・昇進」のアドレス
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_04, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_05, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_12, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_13, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_14, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_15, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_16, (char)TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi );
}



void OverWriteNeedKoudouryoku(int iCommandAdress, int iKoudouValue) {

	/*
	 *******************
		不明なコマンドのiKoudouValueを調べる時は、「OnSwitchMainScreenSubCommand.cpp」に記載されている場所からブレイクを張ってジャンプしていけばだいたいＯＫ
	 *******************
	 */

	switch ( iCommandAdress ) {
		case CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01:
			{
		
				// 「講義」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKougi = iKoudouValue;
				break;
			 }
		case CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01:
			{
				// 「披露」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHirou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01:
			{
				// 「編成」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHensei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01:
			{
				// 「徴兵」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueChohei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01:
			{
				// 「登用」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueTouyou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01:
			{
				// 「褒美」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_03, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHoubi = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01:
			{
				// 「物資」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_03, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueBusshi = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01:
			{
				// 「戦争」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueSensou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01:
			{
				// 「同盟」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueDoumei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01:
			{
				// 「婚姻」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKonin = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01:
			{
				// 「手切れ」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueTegire = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01:
			{
				// 「脅迫」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01:
			{
				// 「朝廷」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueChotei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01:
			{
				// 「内応」の値
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueNaiou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_01:
			{
				// 「謀略」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_03, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_04, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_05, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_06, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_07, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_01:
			{
				// 「評定」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo = iKoudouValue;
				break;
			}
	
		case CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01:
			{
				// 「軍団」->「任命」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01:
			{
				// 「軍団」->「再任」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_02, (char)iKoudouValue );

				// 「軍団」->「分配」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_02, (char)iKoudouValue );

				// 「軍団」->「合流」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_02, (char)iKoudouValue );

				TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd = iKoudouValue;

				break;
			}
		case CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_01:
			{

				// 「内政・訓練・昇進」のアドレス
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_03, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_04, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_05, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_12, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_13, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_14, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_15, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_16, (char)iKoudouValue );

				TSModCommand::AllGundan::cmdNeedKoudouValue5Ningumi = iKoudouValue;

			break;
		}
	}
}


// 外部参照用
void WINAPI Extern_OverWriteNeedKoudouryoku(int iCommandAdress, int iKoudouValue) {
	OverWriteNeedKoudouryoku(iCommandAdress, iKoudouValue);
}
