#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"

/*
* �����������ꂼ�ꕪ�򂳂���B�������̂S��char *�z��̂����ꂩ��Ԃ��B
*/

extern int isOnEditingButaiMode;


// �����̕��Ԃ������ꂩ�ł���(0=���y,1=�R�n,2=�S�C,3=�S�C�R�n
BOOL IsBushouForm(int iBushouID, int f1, int f2=-1, int f3=-1, int f4=-3) {
	if ( nb6bushouref[iBushouID].form == f1 ) return true;
	if ( f2 == -1 ) { return false; }
	if ( nb6bushouref[iBushouID].form == f2 ) return true;
	if ( f3 == -1 ) { return false; }
	if ( nb6bushouref[iBushouID].form == f3 ) return true;
	if ( f4 == -1) { return false; }
	if ( nb6bushouref[iBushouID].form == f4 ) return true;

	return false;
}

char *IsButaiArrayLoslorien[4] = {
	"�Í��R�m", // �����X���[���A��(���y)
	"�Í��R�m", // �����X���[���A��(�R�n)
	"�Í��R�m", // �����X���[���A��(�S�C)
	"�Í��R�m", // �����X���[���A��(�R�n�S�C)
};

char *IsButaiArrayLoslorienWithDetail[4] = {
	" �Í��R�m (���y) ", // �����X���[���A��(���y)
	" �Í��R�m (�R�n) ", // �����X���[���A��(�R�n)
	" �Í��R�m (�S�C) ", // �����X���[���A��(�S�C)
	" �Í��R�m (�R�S) ", // �����X���[���A��(�R�n�S�C)
};


char *IsButaiArrayIkkousyu[4] = {
	"����㓁",
	"�R�n",
	"����S�C",
	"�R�n�S�C"
};
char *IsButaiArrayIkkousyuYumi[4] = {
	"�|�m",
	"�|�R�n",
	"����S�C",
	"�R�n�S�C"
};

char *IsButaiArrayUesughi[4] = {
	"�㐙�㓁",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayUesughiYumi[4] = {
	"�㐙�|�m",
	"�|�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayShimazu[4] = {
	"���Ó���",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayDate[4] = {
	"�ɒB����",
	"�R�n",
	"�S�C",
	"�ɒB�R�S"
};

char *IsButaiArrayDateYumi[4] = {
	"�|���y",
	"�|�R�n",
	"�S�C",
	"�ɒB�R�S"
};

char *IsButaiArrayHattori[4] = {
	"�ɉ�E��",
	"�R�n",
	"�����S�C",
	"�R�n�S�C"
};

char *IsButaiArrayHattoriYumi[4] = {
	"�ɉ�E��",
	"�|�R�n",
	"�����S�C",
	"�R�n�S�C"
};
char *IsButaiArrayHuuma[4] = {
	"�����E��",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayHuumaYumi[4] = {
	"�����E��",
	"�|�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayMomochi[4] = {
	"�ɉ�E��",
	"�R�n",
	"�ɉ�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayMomochiYumi[4] = {
	"�ɉ�E��",
	"�|�R�n",
	"�ɉ�S�C",
	"�R�n�S�C"
};

char *IsButaiArraySarutobi[4] = {
	"�b��E��",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArraySarutobiYumi[4] = {
	"�b��E��",
	"�|�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayKirigakure[4] = {
	"�ɉ�E��",
	"�R�n",
	"�ɉ�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayKirigakureYumi[4] = {
	"�ɉ�E��",
	"�|�R�n",
	"�ɉ�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayNinja[4] = {
	"�E��",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};


char *IsButaiArrayOda[4] = {
	"�D�c��",
	"�R�n",
	"�D�c�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayOdaYumi[4] = {
	"�D�c�|",
	"�|�R�n",
	"�D�c�S�C",
	"�R�n�S�C"
};

char *IsButaiArraySouryo[4] = {
	"�㓁�m",
	"�R�n",
	"�R�n",
	"�R�n�S�C"
};

char *IsButaiArraySouryoYumi[4] = {
	"�|�m",
	"�|�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaHigh[4] = {
	"�Y�}��",
	"���c���{",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaYari[4] = {
	"���y",
	"���c���R",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaKatana[4] = {
	"���y",
	"���c���R",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaYumi[4] = {
	"�|���y",
	"���c�|�R",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaWoman[4] = {
	"�����m",
	"���R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayTakedaWomanYumi[4] = {
	"�����m",
	"���|�R�n",
	"�S�C",
	"�R�n�S�C"
};

char *IsButaiArrayHoujoTeppoGreat[4] = {
	"�k��哛",
	"�k��哛",
	"�k��哛",
	"�k��哛"
};

char *IsButaiArrayShiroKishi[4] = {
	"���R�m",
	"���R�n",
	"�S�C�R�m",
	"�R�S�R�m",
};
char *IsButaiArrayMahou[4] = {
	"���@",
	"���@�R�n",
	"���@�S�C",
	"���@�R�S",
};
char *IsButaiArrayMajuu[4] = {
	"���b",
	"���b�R�n",
	"���b�S�C",
	"���b�R�S",
};


char *IsButaiArrayHighKatana[4] = {
	"����",
	"���{�R�n",
	"�S�C��",
	"�R�n�S�C"
};

char *IsButaiArrayHighYari[4] = {
	"����",
	"���{���R",
	"�S�C��",
	"�R�n�S�C"
};

char *IsButaiArrayHighYumi[4] = {
	"�|��",
	"���{�|�R",
	"�S�C��",
	"�R�n�S�C"
};

char *IsButaiArrayKatanaHigh[4] = {
	"����",
	"����",
	"����",
	"����"
};

char *IsButaiArrayKatanaMid[4] = {
	"�쑾����",
	"�쑾����",
	"�쑾����",
	"�쑾����"
};

char *IsButaiArrayNaginagaHigh[4] = {
	"�㓁��",
	"�㓁��",
	"�㓁��",
	"�㓁��"
};

char *IsButaiArrayYariHigh[4] = {
	"����",
	"����",
	"����",
	"����"
};

char *IsButaiArrayYari[4] = {
	"�����y",
	"�����y",
	"�����y",
	"�����y"
};

char *IsButaiArrayKibaYariHigh[4] = {
	"�����R�n",
	"�����R�n",
	"�����R�n",
	"�����R�n"
};

char *IsButaiArrayKibaYumi[4] = {
	"�|�R�n",
	"�|�R�n",
	"�|�R�n",
	"�|�R�n"
};

char *IsButaiArrayYumiHigh[4] = {
	"�|��",
	"�|��",
	"�|��",
	"�|��"
};

char *IsButaiArrayYumi[4] = {
	"�|���y",
	"�|���y",
	"�|���y",
	"�|���y"
};

char *IsButaiArrayTeppoHigh[4] = {
	"�S�C��",
	"�S�C��",
	"�S�C��",
	"�S�C��"
};

char *IsButaiArrayTeppoGreat[4] = {
	"�哛",
	"�哛",
	"�哛",
	"�哛"
};

char *IsButaiArrayUmiueKurohune[4] = {
	"���D",
	"���D",
	"���D",
	"���D"
};
char *IsButaiArrayUmiueNihonmaru[4] = {
	"���{��",
	"���{��",
	"���{��",
	"���{��"
};
char *IsButaiArrayUmiueDaiAtake[4] = {
	"�����D",
	"�����D",
	"�����D",
	"�����D"
};
char *IsButaiArrayUmiueDaiAtakeWithDetail[4] = {
	" �����D (���y) ",
	" �����D (�R�n) ",
	" �����D (�S�C) ",
	" �����D (�R�S) "
};

char *IsButaiArrayUmiueAtake[4] = {
	"����D",
	"����D",
	"����D",
	"����D"
};
char *IsButaiArrayUmiueAtakeWithDetail[4] = {
	" ����D (���y) ",
	" ����D (�R�n) ",
	" ����D (�S�C) ",
	" ����D (�R�S) "
};
char *IsButaiArrayUmiueSenseki[4] = {
	"��ΑD",
	"��ΑD",
	"��ΑD",
	"��ΑD"
};
char *IsButaiArrayUmiueSensekiWithDetail[4] = {
	" ��ΑD (���y) ",
	" ��ΑD (�R�n) ",
	" ��ΑD (�S�C) ",
	" ��ΑD (�R�S) "
};
char *IsButaiArrayUmiueSekibune[4] = {
	"�֑D",
	"�֑D",
	"�֑D",
	"�֑D"
};
char *IsButaiArrayUmiueSekibuneWithDetail[4] = {
	" �֑D (���y) ",
	" �֑D (�R�n) ",
	" �֑D (�S�C) ",
	" �֑D (�R�S) "
};
char *IsButaiArrayUmiueIrou[4] = {
	"��O�D",
	"��O�D",
	"�S�C����",
	"�S�C����"
};
char *IsButaiArrayUmiueIrouWithDetail[4] = {
	" ��O�D (���y) ",
	" ��O�D (�R�n) ",
	" �S�C���� (�S�C) ",
	" �S�C���� (�R�S) "
};
char *IsButaiArrayUmiueMini[4] = {
	"����",
	"����",
	"�S�C����",
	"�S�C����"
};
char *IsButaiArrayUmiueMiniWithDetail[4] = {
	" ���� (���y) ",
	" ���� (�R�n) ",
	" �S�C���� (�S�C) ",
	" �S�C���� (�R�S) "
};
char *IsButaiArrayUmiueMiniYumi[4] = {
	"�|����",
	"�|����",
	"�S�C����",
	"�S�C����"
};
char *IsButaiArrayUmiueMiniYumiWithDetail[4] = {
	" �|���� (���y) ",
	" �|���� (�R�n) ",
	" �S�C���� (�S�C) ",
	" �S�C���� (�R�S) "
};

// "\xec\x8c\xec\x8d\xec\x8e";//���@�䕔
// "\xec\x8f\xec\x8d\xec\x8e";//���@�䕔
char *IsButaiArrayChosokabeYumi[4] = {
	"\xec\x8c\xec\x8d\xec\x8e�|",
	"�R�n",
	"�S�C",
	"�R�n�S�C"
};


// NULL��Ԃ��΁A�ύX���Ȃ��Ƃ������ƁB
int GetOverWriteButaiMeiArrayPointer(int iBushouID) {

	if ( !(0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) ) {
		return NULL;
	}

	// ScenarioMod�𓱓����Ă���B
	if ( p_snOnViewingButaiName ) {
		int iSceanrioModValidButaiName = p_snOnViewingButaiName( iBushouID + 1 );

		// �L���ȕύX������Ă���̂ł���΁A�����Ԃ��B
		if ( iSceanrioModValidButaiName ) {
			return iSceanrioModValidButaiName;
		}
	}


	// ���퓬���Ă��āA�C�̏�ɂ���
	HEX_POSITION p = GetFieldHexUnitPos( iBushouID );
	// �����̈ʒu�̖����𓾂�
	int iHexRole = GetFieldHexRole( p );

	//------------------------------------------�ȉ��A�喼�Ƃ����D�悷��
	// �^���^���X�Ȃ��
	if ( isLancelotTartare( iBushouID ) ) {
		// �C�̏�
		if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::�C || iHexRole == FIELD_HEX_ROLE::��) ) {
			// �S�b�D�������Ă���
			if (nb6bushouref[iBushouID].steelship) {
				return int(IsButaiArrayUmiueKurohune);
			}
		}
		// �C�̏�ȊO
		// �����Ґ����[�h
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// �C�̏�
	if ( Is_FieldWar() && (iHexRole == FIELD_HEX_ROLE::�C || iHexRole == FIELD_HEX_ROLE::��) ) {
		// �ő吨�͂̑喼
		int iDaimyoID = nb6bushouref[iBushouID].attach -1;
		int iMaximumInfluenceDaimyo = GetMaximumInfluenceDaimyo();
		// �ő吨�͂̑喼���g�ł���A��]������
		if ( nb6bushouname[iBushouID].position == 1 && (iDaimyoID == iMaximumInfluenceDaimyo) && nb6bushouref[iBushouID].ambition > 80 && nb6bushouref[iBushouID].steelship ) {
			return int(IsButaiArrayUmiueNihonmaru);
		}

		// �S�b�D�������Ă���
		if (nb6bushouref[iBushouID].steelship) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if (nb6bushouname[iBushouID].position == 1 ) {
			return int(IsButaiArrayUmiueDaiAtake);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if ( nb6bushouname[iBushouID].position == 2 ) {
			return int(IsButaiArrayUmiueAtake);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if (nb6bushouname[iBushouID].position == 3 ) {
			return int(IsButaiArrayUmiueSenseki);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if (nb6bushouname[iBushouID].position == 4 ) {
			return int(IsButaiArrayUmiueSekibune);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if (nb6bushouname[iBushouID].position == 5 ) {
			return int(IsButaiArrayUmiueIrou);
		}

		// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
		if (nb6bushouname[iBushouID].position == 6 ) {
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayUmiueMiniYumi);
			} else {
				return int(IsButaiArrayUmiueMini);
			}
		}
	}

	// ���X���[���A���Ȃ�΁c
	if ( IsLoslorien( iBushouID ) ) {
		// �����Ґ����[�h
		if ( isOnEditingButaiMode ) {
			return int(IsButaiArrayLoslorienWithDetail);
		} else {
			return int(IsButaiArrayLoslorien);
		}
	}

	// �E�H�[�����Ȃ�΁c���p�t�ł͂Ȃ��萯�p�t�Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�q�r")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�n�o�p")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// �J�m�[�v�X�Ȃ�΁c���l�ł͂Ȃ��L���l�Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�O�u")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�[�\�]")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// �M���o���h�ł���΁c�R�m�ł͂Ȃ��b�g���Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�g�h�i")==0 && strcmp(nb6bushouname[iBushouID].fastname, "���")==0 ) {
		return int(IsButaiArrayMajuu);
	}
	// �~���f�B���܂�΁A���R�m�Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�O�P�Q")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�R�S�T")==0 ) {
		return int(IsButaiArrayShiroKishi);
	}
	// �M���_�X�Ȃ�΁c���R�m�Ƃ���
	if	(strcmp(nb6bushouname[iBushouID].familyname, "�G�H�F")==0 && strcmp(nb6bushouname[iBushouID].fastname, "������")==0) {
		return int(IsButaiArrayShiroKishi);
	}		
	// �f�l�u�Ȃ�΁c���p�t�ł͂Ȃ������Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�j�k")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�l�m")==0 ) {
		return int(IsButaiArrayMahou);
	}
	// ���h�����Ȃ�΁c���p�t�ł͂Ȃ������m�Ƃ���
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�y�z�{")==0 && strcmp(nb6bushouname[iBushouID].fastname, "�|�}�~")==0 ) {
		return int(IsButaiArrayMahou);
	}



	// ���������̏ꍇ�A�������z���ύX����B
	if ( strcmp(nb6bushouname[iBushouID].familyname, "����") == 0 && nb6bushouname[iBushouID].number == 108 ) {
		// �E�҂̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2)) {
				return int(IsButaiArrayHattoriYumi);
			} 
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayHattori);
			}
		}
	}
	// ���������Y�̏ꍇ�A�������z���ύX����B
	if ( strcmp(nb6bushouname[iBushouID].familyname, "����") == 0 && nb6bushouname[iBushouID].number == 119 ) {
		// �E�҂̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0)) {
				return int(IsButaiArrayHuumaYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayHuuma);
			}
		}
	}
	// �S�n�O�g�̏ꍇ�A�������z���ύX����B
	if ( strcmp(nb6bushouname[iBushouID].familyname, "�S�n") == 0 && nb6bushouname[iBushouID].number == 328 ) {
		// �E�҂̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochiYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0,2) ) {
				return int(IsButaiArrayMomochi);
			}
		}
	}

	// ���򍲏��̏ꍇ�A�������z���ύX����B
	if ( strcmp(nb6bushouname[iBushouID].familyname, "����") == 0 && nb6bushouname[iBushouID].number == 361 ) {
		// �E�҂̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobiYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySarutobi);
			}
		}
	}

	// ���B�ˑ��̏ꍇ�A�������z���ύX����B
	if ( strcmp(nb6bushouname[iBushouID].familyname, "���B") == 0 && nb6bushouname[iBushouID].number == 362 ) {
		// �E�҂̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x10 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakureYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayKirigakure);
			}
		}
	}

	// �E�҂̏ꍇ
	if ( nb6bushouref[iBushouID].job == 0x10 ) {
		if ( IsBushouForm(iBushouID, 0) ) {
			return int(IsButaiArrayNinja);
		}
	}

	// �喼�̌��؂ŕ����Ă���
	int iDaimyoID = nb6bushouref[iBushouID].attach -1;
	int iGundanID = nb6bushouref[iBushouID].gundan -1;

	if ( (0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM) && (0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM) ) {
		int iDaimyoBushouID = nb6daimyo[iDaimyoID].attach - 1;
		int iGundanBushouID = nb6gundan[iGundanID].leader - 1;

		// �{�莛�Ƃ̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 41 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayIkkousyuYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayIkkousyu);
			}

		} 

		// �c�����㐙�ŁA�㐙���M�������́A�㐙�i���̏ꍇ�A�������z���ύX����B
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "�㐙")==0 && (nb6bushouname[iDaimyoBushouID].number == 26 || nb6bushouname[iDaimyoBushouID].number == 27) ) {

			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughiYumi);
			}
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArrayUesughi);
			}
		}

		// ���ÉƂ̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 6 ) {
			// �ʏ핐��
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi < 3 ) {
				return int(IsButaiArrayShimazu);
			}
		}

		// ���@�䕔�̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 11 ) {
			// �|���g���镐��
			if ( IsBushouForm(iBushouID, 0) && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				return int(IsButaiArrayChosokabeYumi);
			}
		}

		// �ɒB�Ƃ̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 7 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 3) ) {
				return int(IsButaiArrayDateYumi);
			}
			// ������
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && IsBushouForm(iBushouID, 0, 3) ) {
				return int(IsButaiArrayDate);
			}

		}

		// �喼���D�c�M���̏ꍇ�A�������z���ύX����B
		if ( strcmp(nb6bushouname[iDaimyoBushouID].familyname, "�D�c")==0 && nb6bushouname[iDaimyoBushouID].number == 0 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOdaYumi);
			}
			// ���n
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 0, 2) ) {
				return int(IsButaiArrayOda);
			}
			// �ʏ�
			if ( IsBushouForm(iBushouID, 2) ) {
				return int(IsButaiArrayOda);
			}
		}

		// �喼�����c�Ƃ̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 1 ) {
			// �퓬��150�ȏ�ɂȂ�����A�D�G�ȕ����ł���΁A���R�c����
			if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {
					return int(IsButaiArrayTakedaHigh);
				}
				if ( IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaHigh);
				}
			}
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
				if ( nb6bushouname[iBushouID].sex == 0 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaYumi);
				}
				if ( nb6bushouname[iBushouID].sex == 1 && IsBushouForm(iBushouID, 0, 1) ) {
					return int(IsButaiArrayTakedaWomanYumi);
				}
				// �ʏ핐��
			} else {
				if ( nb6bushouname[iBushouID].sex == 0 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaYari);
				}
				if ( nb6bushouname[iBushouID].sex == 0 && IsBushouForm(iBushouID, 1) ) {
					return int(IsButaiArrayTakedaKatana);
				}
				if ( nb6bushouname[iBushouID].sex == 1 && IsBushouForm(iBushouID, 0, 1) ) {
					return int(IsButaiArrayTakedaWoman);
				}
			}
		}

		// �k���Ƃ̏ꍇ�A�������z���ύX����B
		if ( nb6bushouref[iDaimyoBushouID].blood == 8 ) {
			// �S�C���g���镐��
			if ( IsBushouForm(iBushouID, 2) ) {
				// �퓬��170�ȏ�ɂȂ�����A�R�n�K�����`�ȏ�
				if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
					return int(IsButaiArrayHoujoTeppoGreat);
				}
			}
		}


		//------------------------------------------�ȉ��A�喼�Ƃ����D��x�͗�镁�ʂ̂���
		// �m���̏ꍇ�A�������z���ύX����B
		// �m���̏ꍇ
		if ( nb6bushouref[iBushouID].job == 0x40 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryoYumi);
			}
			if ( IsBushouForm(iBushouID, 0) ) {
				return int(IsButaiArraySouryo);
			}

		}

		// �C�n�ł���΁c
		if ( nb6bushouref[iBushouID].job==0x50 ) { // �C�������R�K���a�ȏ�

			// �S�b�D�������Ă���
			if (nb6bushouref[iBushouID].steelship) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if (nb6bushouname[iBushouID].position == 1 ) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueDaiAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueDaiAtake);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if ( nb6bushouname[iBushouID].position == 2 ) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueAtakeWithDetail);
				} else {
					return int(IsButaiArrayUmiueAtake);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if (nb6bushouname[iBushouID].position == 3 ) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSensekiWithDetail);
				} else {
					return int(IsButaiArrayUmiueSenseki);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if (nb6bushouname[iBushouID].position == 4 ) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueSekibuneWithDetail);
				} else {
					return int(IsButaiArrayUmiueSekibune);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if (nb6bushouname[iBushouID].position == 5 ) {
				// �����ҏW���[�h
				if ( isOnEditingButaiMode ) {
					return int(IsButaiArrayUmiueIrouWithDetail);
				} else {
					return int(IsButaiArrayUmiueIrou);
				}
			}

			// �B��=0, �喼=1, �h�V=2, �ƘV=3, ����=4, ���叫=5, ���y��=6
			if (nb6bushouname[iBushouID].position == 6 ) {
				if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 ) {
					// �����ҏW���[�h
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniYumiWithDetail);
					} else {
						return int(IsButaiArrayUmiueMiniYumi);
					}
				} else {
					// �����ҏW���[�h
					if ( isOnEditingButaiMode ) {
						return int(IsButaiArrayUmiueMiniWithDetail);
					} else {
						return int(IsButaiArrayUmiueMini);
					}
				}
			}
		}

		// �퓬��150�ȏ�ɂȂ�����A�D�G�ȕ����ł���΁A���R�c����
		if ( getBushouBat(iBushouID) > 75 && (iDaimyoBushouID==iGundanBushouID) && nb6bushouname[iBushouID].State != 0 && (nb6bushouref[iBushouID].maxbat + nb6bushouref[iBushouID].maxgov + nb6bushouref[iBushouID].maxint) >= 200 ) {
			// �|���g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYumi);
			}
			// �����g���镐��
			if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighYari);
			}
			if ( IsBushouForm(iBushouID, 1) ) {
				return int(IsButaiArrayHighKatana);
			}
		}

		// �S�C�̕���
		if ( IsBushouForm(iBushouID, 2) ) {

			// �퓬��170�ȏ�ɂȂ�����A�R�n�K�����`�ȏ�
			if ( getBushouBat(iBushouID) > 85 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 4) {
				return int(IsButaiArrayTeppoGreat);
			}
			// �퓬��150�ȏ�ɂȂ�����A�R�n�K�����a�ȏ�
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_TEPPOU) >= 3) {
				return int(IsButaiArrayTeppoHigh);
			}
		}

		// �|�̕���
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 0) ) {
			// �퓬��150�ȏ�ɂȂ�����A���y�K�����a�ȏ�
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYumiHigh);
			}
			return int(IsButaiArrayYumi);
		}

		// �|�̋R�n����
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi >= 3 && IsBushouForm(iBushouID, 1) ) {
			return int(IsButaiArrayKibaYumi);
		}

		// ���̋R�n����
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari > 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 1) ) {

			// �퓬��150�ȏ�ɂȂ�����A�R�n�K�����a�ȏ�
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_KIBA) >= 3) {
				return int(IsButaiArrayKibaYariHigh);
			}
		}

		// ���̑��y����
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari >= 3 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// �퓬��150�ȏ�ɂȂ�����A���y�K�����a�ȏ�
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayYariHigh);
			}
			// ���y�K�����b�ȏ�
			if ( getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 2) {
				return int(IsButaiArrayYari);
			}
		}

		// ���̑��y����(��ԓ������Ȃ�)
		if ( nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari <= 2 && nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi <= 2 && IsBushouForm(iBushouID, 0) ) {

			// �퓬��150�ȏ�ɂȂ�����A���y�K�����a�ȏ�
			if ( getBushouBat(iBushouID) > 75 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 3) {
				return int(IsButaiArrayKatanaHigh);
			}
			// �퓬��120�ȏ�ɂȂ�����A���y�K�����b�ȏ�
			if ( getBushouBat(iBushouID) > 60 && getBushouAptitude(iBushouID, APTITUDE_ASIGARU ) >= 2) {
				return int(IsButaiArrayKatanaMid);
			}

		}


	}


	return NULL;
}