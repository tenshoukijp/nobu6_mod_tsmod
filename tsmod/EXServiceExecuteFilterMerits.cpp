#include "WinTarget.h"

#include "EXBushou.h"
#include "EXBushou_All.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"




// �M���Ɋւ������ȃt�B���^�[�T�[�r�X
void EXSuperService_FilterMerits() {


	// �E�҂͌M�������܂炸�A�o���o���Ȃ��Ɋւ���t���O���L��
	if (TSModCommand::AllBushou::cmdNinjaNotHaveAWayOfPromotion ) {

		// �����ɑ΂��Č�������
		for (int iTargetBushouID=0; iTargetBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iTargetBushouID++) {

			// �Ώۂ̕������E�҂ł���΁c 
			if (nb6bushouref[iTargetBushouID].job == 0x10) { // 0x10==�E�� GameDataStruct.h������

				// ����������V���Ȓl�͌M���Œ�l
				WORD rewrite_merits = 0;

				// �h�V�`���y���܂łȂ祥� (�喼��B���͏��������Ȃ�)
				if ( 2 <= nb6bushouname[iTargetBushouID].position && nb6bushouname[iTargetBushouID].position <= 6 ) {

					// �g�����ϓ����Ă���Υ���g���𑫌y���ɖ߂�
					if ( nb6bushouname[iTargetBushouID].position != 6 ) { // 6 ���y�����GameDataStruct������

						// �����I�ɑ��y����
						nb6bushouname[iTargetBushouID].position = 6;
					}

					// �Y�������̌M���l���A�Ȃ�炩�̕ϓ������Ă���Υ�� �M�����O�ɏ���������
					if ( nb6bushouref[iTargetBushouID].merits != rewrite_merits ) { 

						nb6bushouref[iTargetBushouID].merits = 0;
					}

				} 

			} // end �E�҂Ȃ�
		} // end for
	} // end TSModCommand�t���O
}

