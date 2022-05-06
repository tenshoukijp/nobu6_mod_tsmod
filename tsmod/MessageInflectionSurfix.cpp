

#include "WinTarget.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "EXServiceExecute.h"
#include "MessageInflection.h"
#include "MessageInflectionSurfix.h"
#include "HumanRelation.h"



// �`�� �`��
string �����i�P�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�P�j��";
}




// �`�� �`�Ȃ�
string �����i�Q�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �Ȃ� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�Ȃ�";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�Q�j��";
}





// �`�� �`��
string �����i�R�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�R�j��";
}



// �`�� �`������
string �����i�S�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ������ ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "������";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�S�j��";
}





// �`�� �`�� �`���� �`��
string �����i�P�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// ���� ����=3  
		} else if ( nb6bushouref[i1st_BushouID].tone == 3) {
			return "����";

		// �� ����=2&���g=1
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i1st_BushouID].position==1 ) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�P�j��";
}




// �`�� �`�� �`���� �`���
string �����i�Q�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ��� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "���";

		// ���� ����=3  
		} else if ( nb6bushouref[i1st_BushouID].tone == 3) {
			return "����";

		// �� ����=2&���g=1
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i1st_BushouID].position==1 ) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "�����i�Q�j��";
}







// �`�� �`��
string ���ȁi�P�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "���ȁi�P�j��";
}





// �`�� �`��
string ���ȁi�Q�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "���ȁi�Q�j��";
}




// �`(����) �`��
string ����i�P�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// (����) �i���j 
		} else {
			return "";
		}
	}

	return "����i�P�j��";
}




// �`�� �`�킢
string ����i�Q�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �킢 ����=2&���50 
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && getYearsOld(i1st_BushouID) >= 50 ) {
			return "�킢";

		// �� �i���j 
		} else {
			return "��";

		}
	}

	return "����i�Q�j";
}






// �`�Ȃ� �`�̂� �`�˂� 
string ���Ȃ���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �˂� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�˂�";

		// �̂� ����=2&���50 
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 && getYearsOld(i1st_BushouID) >= 50 ) {
			return "�̂�";

		// �Ȃ� �i���j 
		} else {
			return "�Ȃ�";

		}
	}

	return "���Ȃ���";
}






// �`�� �`(����)
string �����i�P�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// (����) ��������&������� 
		if ( is�����͑���̌���( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
			return "";

		// �� �i���j 
		} else {
			return "��";

		}
	}

	return "�����i�P�j��";
}




// �`�� �`(����)
string �����i�Q�j��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// (����) ��������&������� 
		if ( is�����͑���̌���( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
			return "";

		// �� ����=1or3or���� 
		} else if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "��";

		} else {
			return "";
		}
	}

	return "�����i�Q�j��";
}







// �͂͂� �ӂӂ� ������ ���͂� ��͂� �ӂ� ���ӂ� ���ق� 
string ���͂͂́�(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���ق� ����&���25 
		if ( nb6bushouname[i1st_BushouID].sex == 1 && getYearsOld(i1st_BushouID) >= 25 ) {
			return "���ق�";

		// ���ӂ� ����
		} else if ( nb6bushouname[i1st_BushouID].sex == 1  ) {
			return "���ӂ�";

		// ��͂� ����=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2  ) {
			return "��͂�";

		// �ӂ� ����=0
		} else if ( nb6bushouref[i1st_BushouID].tone == 0  ) {
			return "�ӂ�";

		// ���͂� ���20 
		} else if ( getYearsOld(i1st_BushouID) <= 20 ) {
			return "���͂�";

		// ������ ���B�ȉ�&���큅50
		} else if ( nb6bushouref[i1st_BushouID].aptitudebat <= 1 && getBushouBat(i1st_BushouID) <= 50 ) {
			return "������";

		// �ӂӂ� ���B�ȉ�
		} else if ( nb6bushouref[i1st_BushouID].aptitudebat <= 1 ) {
			return "�ӂӂ�";

		// �͂͂� �i���j 
		} else {
			return "�͂͂�";
		}
	}

	return "���͂͂́�";
}



// �`�� �`��
string ������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "������";
}




// ���ނ� �܂� 
string �����ނށ�(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �܂� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�܂�";

		// ���ނ� �i���j 
		} else {
			return "���ނ�";
		}
	}

	return "�����ނށ�";
}



// ���� �܂�
string ��������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �܂� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�܂�";

		// ���� �i���j 
		} else {
			return "����";
		}
	}

	return "��������";
}




// �܂�ʁc �������낭����܂��� 
string ���܂�ʁ�(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �������낭����܂��� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�������낭����܂���";

		// �܂�ʁc �i���j 
		} else {
			return "�܂�ʁc";
		}
	}

	return "���܂�ʁc��";
}




// ��� �� 
string �������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "��";

		// ��� �i���j 
		} else {
			return "���";
		}
	}

	return "�������";
}




// �� �Ă�
string ������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �Ă� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1) {
			return "�Ă�";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "������";
}





// �� ���� 
string ���ꁓ(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���� ����<����  
		if ( getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {
			return "����";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "���ꁓ";
}






// ���񂺂悤 ����悤 ���񂺂܂��傤 
string �����񂺂悤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���񂺂܂��傤 ��������&����<���� 
		if ( is�����͑���̌���( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) < getYearsOld(i2nd_BushouID) ) {
			return "���񂺂܂��傤";

		// ���񂺂܂��傤 ����  
		} else if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "���񂺂܂��傤";

		// ����悤
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "����悤";

		// ���񂺂悤 �i���j 
		} else {
			return "���񂺂悤";
		}
	}

	return "�����񂺂悤��";
}


// �ꑰ �e�q �o�� �o�� �Z�� �v�w �Z�� �o�� �o�� �Z�� 
string ���ꑰ��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		if ( is�����͑���̐e(i1st_BushouID, i2nd_BushouID ) ) {
			return "�e�q";

		// �Z��
		} else if ( is�����Ƒ���͌Z��(i1st_BushouID, i2nd_BushouID) ) {
			return "�Z��";

		// �o��
		} else if ( is�����Ƒ���͎o��(i1st_BushouID, i2nd_BushouID) ) {
			return "�o��";

		// �o��
		} else if ( is�����Ƒ���͎o��(i1st_BushouID, i2nd_BushouID) ) {
			return "�o��";

		// �Z�� 
		} else if ( is�����Ƒ���͌Z��(i1st_BushouID, i2nd_BushouID) ) {
			return "�Z��";

		} else if ( is�����͑���̕v(i1st_BushouID, i2nd_BushouID) ) {
			return "�v�w";

		} else if ( is�����͑���̍�(i1st_BushouID, i2nd_BushouID) ) {
			return "�v�w";

		} else {
			return "�ꑰ";

		}
	}

	return "���ꑰ��";
}




// �ł����� �����A�͂͂� �ł�����邼�� �ł��� 
string ���ł����道(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �ł��� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "���񂺂܂��傤";

		// �����A�͂͂� ����=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "�����A" + ���͂͂́�(i1st_BushouID, i2nd_BushouID,i3rd_BushouID);

		// �ł�����邼�� ����=3 
		} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "�ł�����邼��";

		// �ł����� �i���j 
		} else {
			return "�ł�����";
		}
	}

	return "���ł����道";
}




// ���� ���܂��� 
string �����ȁ�(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���܂��� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "���܂���";


		// ���� �i���j 
		} else {
			return "����";
		}
	}

	return "�����ȁ�";
}





// �Ă݂� �Ȃ���
string ���Ă݂恓(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �Ȃ��� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "�Ȃ���";

		// �Ă݂� �i���j 
		} else {
			return "�Ă݂�";
		}
	}

	return "���Ă݂恓";
}



// �� �܂�
string ���恓(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �܂� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "�܂�";

		// �� �i���j 
		} else {
			return "��";
		}
	}

	return "���恓";
}
