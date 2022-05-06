#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

#include "SetNeedKoudouryoku.h"

/*
�@�e��s���ɕK�v�Ȓl�̐ݒ�B�_�p�b�`���̈ڐA�B
 */


void SetNewNeedKoudouryokuValue( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}


void InitNeedKoudouryoku() {

	/*
	 *******************
		�s���ȃR�}���h�̍s���͂𒲂ׂ鎞�́A�uOnSwitchMainScreenSubCommand.cpp�v�ɋL�ڂ���Ă���ꏊ����u���C�N�𒣂��ăW�����v���Ă����΂��������n�j
	 *******************
	 */

	// �u�u�`�v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKougi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKougi );

	// �u��I�v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHirou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHirou );

	// �u�Ґ��v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHensei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHensei );

	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChohei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChohei );

	// �u�o�p�v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTouyou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTouyou );

	// �u�J���v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHoubi );

	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBusshi );

	// �u�푈�v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueSensou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueSensou );

	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueDoumei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueDoumei );

	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKonin );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKonin );

	// �u��؂�v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTegire );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueTegire );

	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku );

	// �u����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChotei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueChotei );
	
	// �u�����v�̒l
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueNaiou );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueNaiou );
	
	// �u�d���v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_03, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_04, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_05, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_06, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_07, (char)TSModCommand::AllGundan::cmdNeedKoudouValueBouryaku );

	// �u�]��v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo );

	// �u�R�c�v->�u�C���v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei );
	
	// �u�R�c�v->�u�ĔC�v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// �u�R�c�v->�u���z�v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// �u�R�c�v->�u�����v�̃A�h���X
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_01, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );
	SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_02, (char)TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd );

	// �u�����E�P���E���i�v�̃A�h���X
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
		�s���ȃR�}���h��iKoudouValue�𒲂ׂ鎞�́A�uOnSwitchMainScreenSubCommand.cpp�v�ɋL�ڂ���Ă���ꏊ����u���C�N�𒣂��ăW�����v���Ă����΂��������n�j
	 *******************
	 */

	switch ( iCommandAdress ) {
		case CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01:
			{
		
				// �u�u�`�v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KOUGI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKougi = iKoudouValue;
				break;
			 }
		case CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01:
			{
				// �u��I�v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HIROU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHirou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01:
			{
				// �u�Ґ��v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HENSEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHensei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOHEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueChohei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01:
			{
				// �u�o�p�v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TOUYOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueTouyou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01:
			{
				// �u�J���v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HOUBI_ADDRESS_03, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHoubi = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_02, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_BUSSHI_ADDRESS_03, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueBusshi = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01:
			{
				// �u�푈�v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_SENSOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueSensou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_DOUMEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueDoumei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KONIN_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKonin = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01:
			{
				// �u��؂�v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_TEGIRE_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueTegire = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_KYOHAKU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueKyohaku = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01:
			{
				// �u����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_CHOTEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueChotei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01:
			{
				// �u�����v�̒l
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_NAIOU_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueNaiou = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_BOURYAKU_ADDRESS_01:
			{
				// �u�d���v�̃A�h���X
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
				// �u�]��v�̃A�h���X
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_HYOUJO_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueHyoujo = iKoudouValue;
				break;
			}
	
		case CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01:
			{
				// �u�R�c�v->�u�C���v�̃A�h���X
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_NINMEI_ADDRESS_02, (char)iKoudouValue );
				TSModCommand::AllGundan::cmdNeedKoudouValueGundanNinmei = iKoudouValue;
				break;
			}
		case CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01:
			{
				// �u�R�c�v->�u�ĔC�v�̃A�h���X
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_SAININ_ADDRESS_02, (char)iKoudouValue );

				// �u�R�c�v->�u���z�v�̃A�h���X
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_BUNPAI_ADDRESS_02, (char)iKoudouValue );

				// �u�R�c�v->�u�����v�̃A�h���X
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_01, (char)iKoudouValue );
				SetNewNeedKoudouryokuValue( CMD_NEED_KOUDOUVALUE_GUNDAN_GOURYU_ADDRESS_02, (char)iKoudouValue );

				TSModCommand::AllGundan::cmdNeedKoudouValueGundanSubCmd = iKoudouValue;

				break;
			}
		case CMD_NEED_KOUDOUVALUE_5NINGUMI_ADDRESS_01:
			{

				// �u�����E�P���E���i�v�̃A�h���X
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


// �O���Q�Ɨp
void WINAPI Extern_OverWriteNeedKoudouryoku(int iCommandAdress, int iKoudouValue) {
	OverWriteNeedKoudouryoku(iCommandAdress, iKoudouValue);
}
