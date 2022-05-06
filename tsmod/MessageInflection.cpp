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
#include "HumanRelation.h"



/*
�y�����z:
0 ����			 217 �^�c�K���E���@�䕔���e�E���Ë`�v�E�E�ґS�� 
1 ��V������	 152 �O�c���ƁE���q���G�E���ԏ@�΁E������� 
2 �e�\			 119 �ēc���ƁE���������M�E���ˑ��E���X�؏����Y 
3 ���M			 30 �㐙�����E����`���E�����`���E������� 
4 ���ӋC		 180 �ɒB���@�E�O�D���c�E��؏d�G�E�R�䐳�� 
5 �Ќ�			 143 �D�c�M���E����ƍN�E���c�M���E�㐙���M 
6 �`��			 145 �L�b�G�g�E�֓����O�E���i�v�G�E�����`�� 
7 ���			 314 �{�������E�g�쌳�t�E�R�����V���E�j���P�S�� 
*/


string ��������		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);


// 00�i����Ɍh�ӂ𕥂��K�v���Ȃ��ꍇ�̎��́j 
string ���킵��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	
	// �Ƒ��̏ꍇ���ɔ��肷��B
	string szIsFamilyMsg = ��������( i1st_BushouID, i2nd_BushouID, i3rd_BushouID );

	if ( szIsFamilyMsg.length() > 0 ) {
		return szIsFamilyMsg;
	}

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���� ����&����>35 
		if ( nb6bushouname[i1st_BushouID].sex == 1 && getYearsOld(i1st_BushouID) > 35 ) {
			return "����";

		// �킽���� ����  
		} else if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "�킽����";

		// �܂� ����=3 
		} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "�܂�";

		// �킵 ���g=0or1 and ����>35  (0==�B��, 1==�喼)
		} else if ( (nb6bushouname[i1st_BushouID].position==0 || nb6bushouname[i1st_BushouID].position==1) && getYearsOld(i1st_BushouID) > 35 ) {
			return "�킵";

		// �� ����=2or4
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
			return "��";

		// �� ����=1 
		} else if ( nb6bushouref[i1st_BushouID].tone == 1 ) {
			return "��";

		// �킵 �i���j 
		} else {
			return "�킵";

		}
	}
	return "���킵��";
}



// 01�i����Ɛe�q�E�Z��E�v�w�ɂ���ꍇ�̎��́j
// ���̃w���p�[�Ȃ̂ŁA���ڎg��Ȃ�����!!
string ��������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		 // �� �����e�q&����<����&���� 
		 if ( is�����͑���̖�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";

		 // �� �����e�q&����>����&���j 
		 } else if ( is�����͑���̕�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";

		 // �� �����e�q&����>����&���� 
		 } else if ( is�����͑���̕�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";

		 // �� �����Z��&����<����&���j 
		 } else if ( is�����͑���̒�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";
		 
		 // �� �����Z��&����<����&���� 
		 } else if ( is�����͑���̖�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";

		 // �Z �����Z��&����>����&���j
		 } else if ( is�����͑���̌Z(i1st_BushouID, i2nd_BushouID ) ) {
			  return "�Z";

		 // �o �����Z��&����>����&����
		 } else if ( is�����͑���̎o(i1st_BushouID, i2nd_BushouID ) ) {
			  return "�o";

		 // �v �����v�w&���j
		 } else if ( is�����͑���̕v(i1st_BushouID, i2nd_BushouID ) ) {
			  return "�v";

		 // �� �����w�v&����
		 } else if ( is�����͑���̍�(i1st_BushouID, i2nd_BushouID ) ) {
			  return "��";
		 }

	}
	return "";
}



// 02�i���̑��̏ꍇ�̎��́j 
string ������(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	
	// �Ƒ��̏ꍇ���ɔ��肷��B
	string szIsFamilyMsg = ��������( i1st_BushouID, i2nd_BushouID, i3rd_BushouID );

	if ( szIsFamilyMsg.length() > 0 ) {
		return szIsFamilyMsg;
	}

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �킽���� ����  
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "�킽����";

		// �܂� ��������&����=3 
		} else if ( is�����͑���̌���(i1st_BushouID, i2nd_BushouID) && nb6bushouref[i1st_BushouID].tone == 3 ) {
			return "�܂�";

		// �킵 ��������&����>����&����>35
		} else if ( is�����͑���̌���(i1st_BushouID, i2nd_BushouID) && getYearsOld(i1st_BushouID) > getYearsOld(i2nd_BushouID) && getYearsOld(i1st_BushouID) > 35 ) {
			return "�킵";

		// �킵 ���g=(0or1)&����>35  (0==�B��, 1==�喼)
		} else if ( (nb6bushouname[i1st_BushouID].position==0 || nb6bushouname[i1st_BushouID].position==1) && getYearsOld(i1st_BushouID) > 35 ) {
			return "�킵";

		// �َ� ���E=1or3 (�E�҂�����)
		} else if ( nb6bushouref[i1st_BushouID].job==0x10 || nb6bushouref[i1st_BushouID].job==0x30 ) {
			return "�َ�";

		// �ّm ���E=4 (�m��)
		} else if ( nb6bushouref[i1st_BushouID].job==0x40 ) {
			return "�ّm";

		// �� ����=2
		} else if ( nb6bushouref[i1st_BushouID].tone == 2 ) {
			return "��";

		// ���ꂪ�� ����=5
		} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
			return "���ꂪ��";

		// �� �i���j 
		} else  {
			return "��";

		}
	}
	return "������";
}


// 03�i���L�`�e���j  
string ���䂪��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		// ���� ���� 
		if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
			return "����";
		} else {
			return "�䂪";
		}
	}
	return "���䂪��";
}


// 04�`07 ������������ޏꍇ�u�ȊO�v�̓�l��
string �����聓(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		 * 04�i����ƌ����֌W�ɂ���ꍇ�̓�l�́j   
		 */ 
		// �����e�q&����<����&���j
		if ( is����͎����̕�(i1st_BushouID, i2nd_BushouID) ) {
			return "����";

		// �����e�q&����<����&����
		} else if ( is����͎����̕�(i1st_BushouID, i2nd_BushouID) ) {
			return "���";

		// �Z�� �����Z��&����<����&���j 
		} else if ( is����͎����̌Z(i1st_BushouID, i2nd_BushouID) ) {
			return "�Z��";

		// �o�� �����Z��&����<����&���� 
		} else if ( is����͎����̎o(i1st_BushouID, i2nd_BushouID) ) {
			return "�o��";

		// ���Ȃ� �����v�w&���j 
		} else if ( is�����͑���̍�(i1st_BushouID, i2nd_BushouID) ) {
			return "���Ȃ�";

		// (����) �����e�q&����>���� (�����͑���̐e�ł���)
		} else if ( is�����͑���̐e(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (����)�a ��������&��=�喼
		} else if ( is�����͑���̌���(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].State == 0) {
			return nb6bushouname[i2nd_BushouID].fastname + string("�a");

		// (����) �����Z��&����>����&�����喼
		} else if ( is�����͑���̌Z(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].State != 0) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (����) �����v�w&���� 
		} else if ( is�����͑���̕v(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (����)�a ��������
		} else if ( is�����͑���̌���(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("�a");

		/*
		 *	05�i�������喼�ő��肪���̉Ɛb�̏ꍇ�̓�l�́j
		 */
		// ���B�� ��=�喼&���g=0 
		} else if ( is����͎����̉Ɛb(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i2nd_BushouID].position==0 ) {
			return "���B��";

		// ���� ����<���d&���30&���50&���j
		} else if ( is����͎����̉Ɛb(i1st_BushouID, i2nd_BushouID) && 
			        (getYearsOld(i1st_BushouID) < nb6bushouref[i2nd_BushouID].work) && getYearsOld(i1st_BushouID) <=30 && 
					getYearsOld(i2nd_BushouID) >= 50 && nb6bushouname[i2nd_BushouID].sex==0 ) {
			return "����";
		// �� ��=�D�c�M��(=��0)&��=�L�b�G�g(=��5) 
		} else if ( is����͎����̉Ɛb(i1st_BushouID, i2nd_BushouID) && nb6bushouname[i1st_BushouID].number == 0 && nb6bushouname[i2nd_BushouID].number == 5 ) {
			return "��";

		// (����) �i���j 
		} else if ( is����͎����̉Ɛb(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		/*
		 *	06�i���肪�����̑喼�̏ꍇ�̓�l�́j
		 */
		// �� ����<���d&���30&���50&���j 
		} else if ( is����͎����̑喼(i1st_BushouID, i2nd_BushouID) && 
					(getYearsOld(i2nd_BushouID) < nb6bushouref[i1st_BushouID].work) && getYearsOld(i2nd_BushouID) <=30 && 
					getYearsOld(i1st_BushouID) >= 50 && nb6bushouname[i2nd_BushouID].sex==0 ) {
			return "��";

		// ���ٗl ������3&33%
		} else if ( is����͎����̑喼(i1st_BushouID, i2nd_BushouID) && nb6bushouref[i1st_BushouID].tone != 3 && rand() % 3 == 0 ) {
			return "���ٗl";

		// (����)�l �i���j 
		} else if ( is����͎����̑喼(i1st_BushouID, i2nd_BushouID) ){
			return nb6bushouname[i2nd_BushouID].fastname + string("�l");

		/*
		 *	07�i���̑��̏ꍇ�̓�l�́j 
		 */
		// (����)�a ���g=1&���g=1 (1==�喼)
		} else if ( nb6bushouname[i2nd_BushouID].position==1 && nb6bushouname[i1st_BushouID].position==1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("�a");

		// (����)�l ���g=1&���g��1 
		} else if ( nb6bushouname[i2nd_BushouID].position==1 && nb6bushouname[i1st_BushouID].position!=1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("�l");

		// ���B�� ���g=0 
		} else if ( nb6bushouname[i2nd_BushouID].position==0 ) {
			return "���B��";

		// (����) ���g�����g(�����ԍ��I�ɂ͎��g�����g)&�������&����=����  (�g��=0:�B�� 1:�喼 2:�h�V 3:�ƘV �c�Ȃ̂ŁA�ԍ�����������������)
		} else if ( nb6bushouname[i1st_BushouID].position <= nb6bushouname[i2nd_BushouID].position && 
					getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) && is�����Ƒ���͓����喼�Ə���(i1st_BushouID, i2nd_BushouID) ) {
			return nb6bushouname[i2nd_BushouID].fastname;

		// (����)�l ���g>���g+1(3�K�����������̐g���Ƃ�������)&������4 
		} else if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i2nd_BushouID].position+1 && 
			nb6bushouref[i1st_BushouID].tone != 4 ) {
			return nb6bushouname[i2nd_BushouID].familyname + string("�l");

		// (����)�l ���� 
		} else if ( nb6bushouname[i2nd_BushouID].sex == 1 ) {
			return nb6bushouname[i2nd_BushouID].fastname + string("�l");

		// (����)�a �i���j 
		} else {
			return nb6bushouname[i2nd_BushouID].fastname + string("�a");
		}

	}
	return "�����聓";
}


// 08 ������������ޏꍇ�̓�l��
string ���l�|���聓		(int i1st_BushouID, int i2nd_BushouID , int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// ������� ���j&����<18&����-����>5 
		if ( nb6bushouname[i2nd_BushouID].sex == 0 && getYearsOld(i2nd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 5 ) {
			return "�������";

		// ���� ����&����<18&����-����>5 
		} else if ( nb6bushouname[i2nd_BushouID].sex == 1 && getYearsOld(i2nd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 5 ) {
			return "����";

		// ��� ����<30&����-����>20 
		} else if ( getYearsOld(i2nd_BushouID) < 30 && getYearsOld(i1st_BushouID)-getYearsOld(i2nd_BushouID) > 20 ) {
			return "���";

		// �V���ڂ� ����>60&����-����>20 
		} else if ( getYearsOld(i2nd_BushouID) > 60 && getYearsOld(i2nd_BushouID)-getYearsOld(i1st_BushouID) > 20 ) {
			return "�V���ڂ�";

		// �����V��
		} else if ( nb6bushouref[i2nd_BushouID].job == 0x40 ) {
			return "�����V��";

		// �������l
		} else if ( nb6bushouref[i2nd_BushouID].job == 0x20 ) {
			return "�������l";

		// ���V�A
		} else if ( nb6bushouref[i2nd_BushouID].job != 0x90 && nb6bushouref[i2nd_BushouID].job == 0x90 ) {
			return "���V�A";

		} else {
			return nb6bushouname[i2nd_BushouID].fastname;
		}
	}
	return "���l�|���聓";
}


// 09�`0D �O�����������ޏꍇ�u�ȊO�v�̎O�l��
string ���O����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i3rd_BushouID && i3rd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

		/*
		 * 0A09�i�喼�������֌W�ɂ��镐�����w���ꍇ�̎O�l�́j 
		 */ 
		// ���� ���O�e�q&����<�O��&�O�j (1==�喼)
		if ( nb6bushouname[i1st_BushouID].position == 1 && is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "����";

		// ��� ���O�e�q&����<�O��&�O��
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "���";

		// �Z�� ���O�Z��&����<�O��&�O�j
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is����͎����̌Z(i1st_BushouID, i3rd_BushouID) ) {
			return "�Z��";

		// �o�� ���O�Z��&����<�O��&�O��
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is����͎����̎o(i1st_BushouID, i3rd_BushouID) ) {
			return "�o��";

		// �o�� ���O�Z��&����<�O��&�O��
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is�����͑���̌���(i1st_BushouID, i3rd_BushouID) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (�O���a) �����֌W
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && is�����͑���̌���(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("�a");

		// ���B�� �O�g=0 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && nb6bushouname[i3rd_BushouID].position==0 ) {
			return "���B��";

		/*
		 * 0A�i�����E�v�w�֌W�ɂ���喼���w���ꍇ�̎O�l�́j 
		 */ 
		// �� ���O�e�q&����<�O��&�O�j
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �� ���O�e�q&����<�O��&�O��
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �Z ���O�Z��&����<�O��&�O�j
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̌Z(i1st_BushouID, i3rd_BushouID) ) {
			return "�Z";

		// �o ���O�Z��&����<�O��&�O��
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̎o(i1st_BushouID, i3rd_BushouID) ) {
			return "�o";

		// �� ���O�Z��&����>�O��&�O�j
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̒�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �� ���O�Z��&����>�O��&�O��
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is����͎����̖�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �Ɠ� ���O�v�w&����
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is�����͑���̕v(i1st_BushouID, i3rd_BushouID) ) {
			return "�Ɠ�";

		// �v ���O�v�w&���j
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is�����͑���̍�(i1st_BushouID, i3rd_BushouID) ) {
			return "�v";

		// (�O��) ���O����&����>����&�O�� 
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is�����͑���̌���(i1st_BushouID, i3rd_BushouID) && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (�O���a) �����֌W
		} else if ( nb6bushouname[i3rd_BushouID].position == 1 && is�����͑���̌���(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("�a");

		/*
		 * 0B�i�������喼�ł���A�����֌W�ł͂Ȃ��ꍇ�̎O�l�́j 
		 */ 
		// ���� ����<�O�d&���30&�O�50&�H&�O�j 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && 
			        (getYearsOld(i1st_BushouID) < nb6bushouref[i3rd_BushouID].work) && getYearsOld(i1st_BushouID) <=30 && 
					getYearsOld(i3rd_BushouID) >= 50 && nb6bushouname[i3rd_BushouID].sex==0 ) {
			return "����";

		// (�O��) ���O����&����>����&�O�� 
		} else if ( nb6bushouname[i1st_BushouID].position == 1 && nb6bushouname[i3rd_BushouID].position == 1 && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (�O���a) �����֌W
		} else if (nb6bushouname[i1st_BushouID].position == 1 &&  nb6bushouname[i3rd_BushouID].position == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("�a");

		/*
		 *	0C�i���肪�����̑喼�̏ꍇ�̎O�l�́j
		 */
		// �� ����<���d&���30&���50&���j 
		} else if ( is����͎����̑喼(i1st_BushouID, i3rd_BushouID) && 
					(getYearsOld(i3rd_BushouID) < nb6bushouref[i1st_BushouID].work) && getYearsOld(i3rd_BushouID) <=30 && 
					getYearsOld(i1st_BushouID) >= 50 && nb6bushouname[i3rd_BushouID].sex==0 ) {
			return "��";

		// ���ٗl 33%
		} else if ( is����͎����̑喼(i1st_BushouID, i3rd_BushouID) && rand() % 3 == 0 ) {
			return "���ٗl";

		// (����)�l �i���j 
		} else if ( is����͎����̑喼(i1st_BushouID, i3rd_BushouID) ){
			return nb6bushouname[i3rd_BushouID].fastname + string("�l");
		
		/*
		 * 0D�i���̑��̏ꍇ�̎O�l�́j 
		 */ 
		// �� ���O�e�q&����<�O��&�O�j
		} else if ( is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �� ���O�e�q&����<�O��&�O��
		} else if ( is����͎����̕�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �Z ���O�Z��&����<�O��&�O�j
		} else if ( is����͎����̌Z(i1st_BushouID, i3rd_BushouID) ) {
			return "�Z";

		// �o ���O�Z��&����<�O��&�O��
		} else if ( is����͎����̎o(i1st_BushouID, i3rd_BushouID) ) {
			return "�o";

		// �� ���O�Z��&����>�O��&�O�j
		} else if ( is����͎����̒�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �� ���O�Z��&����>�O��&�O��
		} else if ( is����͎����̖�(i1st_BushouID, i3rd_BushouID) ) {
			return "��";

		// �Ɠ� ���O�v�w&����
		} else if ( is�����͑���̕v(i1st_BushouID, i3rd_BushouID) ) {
			return "�Ɠ�";

		// �v ���O�v�w&���j
		} else if ( is�����͑���̍�(i1st_BushouID, i3rd_BushouID) ) {
			return "�v";

		// (�O��) ���O����&����>����&�O�� 
		} else if ( is�����͑���̌���(i1st_BushouID, i3rd_BushouID) && getYearsOld( i1st_BushouID ) > getYearsOld( i3rd_BushouID ) && nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (�O���a) �����֌W
		} else if ( is�����͑���̌���(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("�a");

		// (�O��) ���g���O�g(�����ԍ��I�ɂ͎��g���O�g)&����O��&����=�O��  (�g��=0:�B�� 1:�喼 2:�h�V 3:�ƘV �c�Ȃ̂ŁA�ԍ�����������������)
		} else if ( nb6bushouname[i1st_BushouID].position <= nb6bushouname[i3rd_BushouID].position && 
					getYearsOld(i3rd_BushouID) >= getYearsOld(i3rd_BushouID) && is�����Ƒ���͓����喼�Ə���(i1st_BushouID, i3rd_BushouID) ) {
			return nb6bushouname[i3rd_BushouID].fastname;

		// (�O��)�l ���g>�O�g+1(3�K�����������̐g���Ƃ�������)&������4 
		} else if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i3rd_BushouID].position+1 && 
			nb6bushouref[i1st_BushouID].tone != 4 ) {
			return nb6bushouname[i3rd_BushouID].familyname + string("�l");

		// (�O��)�l �O�� 
		} else if ( nb6bushouname[i3rd_BushouID].sex == 1 ) {
			return nb6bushouname[i3rd_BushouID].fastname + string("�l");

		// (����)�a �i���j 
		} else {
			return nb6bushouname[i3rd_BushouID].fastname + string("�a");
		}

	}
	return "���O����";
}




// 0E�i��O�҂��������ޏꍇ�̎O�l�́j 
string ���l�|�O����		(int i1st_BushouID, int i2nd_BushouID , int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		 0 <= i3rd_BushouID && i3rd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {
		// ������� �O�j&�O��<18&����-�O��>5 
		if ( nb6bushouname[i3rd_BushouID].sex == 0 && getYearsOld(i3rd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 5 ) {
			return "�������";

		// ���� �O��&�O��<18&����-�O��>5 
		} else if ( nb6bushouname[i3rd_BushouID].sex == 1 && getYearsOld(i3rd_BushouID) < 18 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 5 ) {
			return "����";

		// ��� �O��<30&����-�O��>20 
		} else if ( getYearsOld(i3rd_BushouID) < 30 && getYearsOld(i1st_BushouID)-getYearsOld(i3rd_BushouID) > 20 ) {
			return "���";

		// �V���ڂ� �O��>60&�O��-����>20 
		} else if ( getYearsOld(i3rd_BushouID) > 60 && getYearsOld(i3rd_BushouID)-getYearsOld(i1st_BushouID) > 20 ) {
			return "�V���ڂ�";

		// (�O��) �i���j 
		} else {
			return nb6bushouname[i3rd_BushouID].fastname;
		}
	}
	return "���l�|�O����";
}


