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


BOOL �����L�����P��(int i1st_BushouID, int i2nd_BushouID) {

	// ���������A�Ƃ������ƁB
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// ���g>���g (�����̐g���̕������Ƃ�������)
			if ( nb6bushouname[i1st_BushouID].position > nb6bushouname[i2nd_BushouID].position ){
				return true;
			}
	}
	return false;
}

BOOL �����L�����Q��(int i1st_BushouID, int i2nd_BushouID) {

	// �����֌W��A�������̔т����A��������A�Ƃ������ƁB
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= i2nd_BushouID && i2nd_BushouID < GAMEDATASTRUCT_BUSHOU_NUM) {

			// ��������&������� 
			if ( is�����͑���̌���( i1st_BushouID, i2nd_BushouID ) && getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) ) {
				return true;
			}
			// ����͎����̉Ɛb
			if ( is����͎����̉Ɛb( i1st_BushouID, i2nd_BushouID ) ) {
				return true;
			}
			// �����Ƃɏ������Ă��� & ������� & ���g<���g(�����̕����g��������)
			if ( is�����Ƒ���͓����喼�Ə���( i1st_BushouID, i2nd_BushouID ) &&  getYearsOld(i1st_BushouID) >= getYearsOld(i2nd_BushouID) && 
				nb6bushouname[i1st_BushouID].position < nb6bushouname[i2nd_BushouID].position ) {
					return true;
			}
	}
	return false;
}

int �����L�����s��(int i1st_BushouID, int i2nd_BushouID) {
	// ����̕��������Ȃ��̂ł���΁A1�ƂȂ�B
	if ( i2nd_BushouID == 0xFFFF ) {
		return 1;
	}

	// ���L�����Q�𖞂����B(�������̔тł��A��������)
	else if ( �����L�����Q��( i1st_BushouID, i2nd_BushouID ) ) {
		return 1;
	}
	// ���L�����P�𖞂����B(��������)
	else if ( �����L�����P��( i1st_BushouID, i2nd_BushouID ) ) {
		return 2;
	}
	// ���L�����P�����L�����Q���������Ȃ�(�����̕����g���͏�)
	else {
		// ����ȊO
		return 3;
	}
}



// �`�ł� �`�� �`����
string ���ł���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �ł� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�ł�";

				// �� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "��";

				// ���� �i���j 
			} else {
				return "����";
			}

			// Group B
		} else {
			// �ł� �i���j 
			return "�ł�";
		}
	}

	return "���ł���";
}



// �`�ł��� �`�ł����邩 �`��
string ���ł�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �ł��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�ł���";

			// �� �i���j 
			} else {
				return "��";
			}

		// Group B
		} else {
			// �ł��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�ł���";

			// �ł����邩 ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "�ł����邩";

			// �ł��� �i���j 
			} else {
				return "�ł���";
			}
		}
	}

	return "���ł�����";
}



// �ł��傤 �`����낤 �`�ł��낤 �`���낤
string ���ł��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {

	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �ł��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�ł��傤";

			// �ł��낤 ����=3 || ����=1 || ����=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "�ł��낤";

			// ���낤 ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "���낤";

			// ����낤 (��)
			} else {
				return "����낤";
			}

		// Group B
		} else {
			// �ł��傤
			return "�ł��傤";
		}
	}

	return "���ł��傤��";
}



// �`�ł��� �`������� �`�ł����� �`������
string ���ł�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �ł��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�ł���";

			// �ł����� ����=3 || ����=1 || ����=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "�ł�����";

			// ������ ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "������";

			// ������� (��)
			} else {
				return "�������";
			}

		// Group B
		} else {
			// �ł���
			return "�ł���";
		}
	}

	return "���ł�����";
}



// �`����܂� �`�������܂� �`������ �`���� �`�������
string ������܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ����܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂�";

			// ������� ����=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "�������";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {
			// ����܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂�";

			// ������ ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "������";

			// ����܂� ����=�����喼�Ə��� 
			} else if ( is�����Ƒ���͓����喼�Ə���(i1st_BushouID,i2nd_BushouID) ) {
				return "����܂�";

			// �������܂� �i���j 
			} else {
				return "�������܂�";

			}
		}
	}

	return "������܂���";
}


// �`����܂��� �`�������܂��� �`������� �`�Ȃ� �`�������� �`����܂���
string ������܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ����܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// �������� ����=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "��������";

			// �Ȃ� (��)
			} else {
				return "�Ȃ�";
			}

		// Group B
		} else {
			// ����܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// ������� ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "�������";

			// ����܂��� ����=�����喼�Ə��� 
			} else if ( is�����Ƒ���͓����喼�Ə���(i1st_BushouID,i2nd_BushouID) ) {
				return "����܂�";

			// �������܂��� �i���j 
			} else {
				return "�������܂�";

			}
		}
	}

	return "������܂���";
}




// �`���܂� �`������ �`����
string �����܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂�";

			// ���� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "����";

			// ������ (��)
			} else {
				return "������";
			}

		// Group B
		} else {
			// ������ ����=5 || ���j
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "������";

			// ���܂� (��)
			} else {
				return "���܂�";
			}
		}
	}

	return "�����܂���";
}

// �`���܂��� �`�������� �`���� �`���� �`���܂���
string �����܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂���";

			// ���� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "����";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {
			// ���܂��� ����=1 || ����=3 || ����
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂���";

			// �������� ����=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "��������";

			// ���܂��� (��)
			} else {
				return "���܂���";
			}
		}
	}

	return "�����܂���";
}



// �`���܂��傤 �`�������� �`���悤
string �����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���܂��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂��傤";

			// ���悤 ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "���悤";

			// �������� (��)
			} else {
				return "��������";
			}

		// Group B
		} else {
			// �������� ����=5 || ���j
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "��������";

			// ���܂��傤 (��)
			} else {
				return "���܂��傤";
			}
		}
	}

	return "�����܂��傤��";
}



// �`���܂��傤 �`����  (�����܂��傤�Ȃ�)
string �����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���܂��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂��傤";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {
			// ���܂��傤 (��)
			return "���܂��傤";
		}
	}

	return "�����܂��傤��";
}



// �`���܂��傤 �`����  (�s���܂��傤�Ȃ�)
string �����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���܂��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���܂��傤";

				// ���� (��)
			} else {
				return "����";
			}

			// Group B
		} else {
			// ���܂��傤 (��)
			return "���܂��傤";
		}
	}

	return "�����܂��傤��";
}


// �`���܂��傤 �`����  (�H�c �����܂��傤���̕����ǂ���)
string �����܂��傤�Q��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���� (��)
			return "����";

			// Group B
		} else {
			// ���܂��傤 (��)
			return "���܂��傤";
		}
	}

	return "�����܂��傤�Q��";
}



// �`���܂��傤 �`�Ƃ�  (�҂��܂��傤�Ȃ�)
string �����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �Ƃ� (��)
			return "�Ƃ�";

			// Group B
		} else {
			// ���܂��傤 (��)
			return "���܂��傤";
		}
	}

	return "�����܂��傤��";
}


// 29 �`�݂܂��傤 �`����  (�U�ߍ��݂܂��傤�Ȃ�)
string ���݂܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���� (��)
			return "����";

			// Group B
		} else {
			// �݂܂��傤 (��)
			return "�݂܂��傤";
		}
	}

	return "�����܂��傤��";
}


// �`�܂��傤 �`�悤  (���̑��̈�ʃp�^�[���B���Ă܂��傤�Ȃ�)
string ���܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �܂��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�܂��傤";

				// �悤 (��)
			} else {
				return "�悤";
			}

			// Group B
		} else {
			// �܂��傤 (��)
			return "�܂��傤";
		}
	}

	return "���܂��傤��";
}



// �`��܂��傤 �`�낤  (�Q��܂��傤�Ȃ�)
string ����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ��܂��傤 ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "��܂��傤";

				// �낤 (��)
			} else {
				return "�낤";
			}

			// Group B
		} else {
			// ��܂��傤 (��)
			return "��܂��傤";
		}
	}

	return "����܂��傤��";
}


// �`���܂��傤 �`����  (�}���܂��傤�Ȃ�)
string �����܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���� (��)
			return "����";

		// Group B
		} else {
			// ���܂��傤 (��)
			return "���܂��傤";
		}
	}

	return "�����܂��傤��";
}


// �`�т܂��傤 �`�ڂ��@(�V�т܂��傤�Ȃ�)
string ���т܂��傤��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �ڂ� (��)
			return "�ڂ�";

		// Group B
		} else {
			// �т܂��傤 (��)
			return "�т܂��傤";
		}
	}

	return "���т܂��傤��";
}



// 35 �`���܂� �`������ �`���� �`���� �`������� �`����܂�
string �����܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ����܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂�";

			// �������� ����=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "�������";

			// ���� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "����";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {
			// ����܂� ����=1 or ����=3 or ���� 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂�";

			// ������ ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "������";

			// ���܂� (���j 
			} else {
				return "���܂�";

			}
		}
	}

	return "�����܂���";
}



// 37 �`���܂��� �`������� �`����� �`����� �`�������� �`����܂���
string �����܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ����܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// �������� ����=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "��������";

			// ����� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "�����";

			// ����� (��)
			} else {
				return "�����";
			}

		// Group B
		} else {
			// ����܂��� ����=1 or ����=3 or ���� 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// ������� ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "�������";

			// ���܂��� (���j 
			} else {
				return "���܂���";

			}
		}
	}

	return "�����܂���";
}



// 39 �`���܂��� �`�������� �`������ �`���� �`��������� �`����܂���
string �����܂�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ����܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// ��������� ����=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 3 ) {
				return "���������";

			// ���� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "����";

			// ������ (��)
			} else {
				return "������";
			}

		// Group B
		} else {
			// ����܂��� ����=1 or ����=3 or ���� 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "����܂���";

			// �������� ����=5 || ���E=1or3(�E��or����)
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouref[i1st_BushouID].job == 0x10 || nb6bushouref[i1st_BushouID].job == 0x30 ) {
				return "��������";

			// ���܂��� (���j 
			} else {
				return "���܂���";

			}
		}
	}

	return "�����܂�����";
}



// �`�܂� �`�� �`��܂�
string ���܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�܂�";

			// �� (��)
			} else {
				return "��";
			}

		// Group B
		} else {

			// �܂� ����=�����喼�Ə��� 
			if ( is�����Ƒ���͓����喼�Ə���(i1st_BushouID,i2nd_BushouID) ) {
				return "�܂�";

			// ��܂��i���j 
			} else {
				return "��܂�";

			}
		}
	}

	return "���܂���";
}



// �`��܂��� �`���
string ����܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ��܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "��܂���";

			// ��� (��)
			} else {
				return "���";
			}

		// Group B
		} else {

			// ��܂���i���j 
			return "��܂���";
		}
	}

	return "����܂���";
}


// �`�܂��� �`�� �`�˂� �`�܂���
string ���܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�܂���";

			// �˂� ����=2 && ���g<=3
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 && nb6bushouname[i2nd_BushouID].position <= 3 ) {
				return "�˂�";

			// �� (��)
			} else {
				return "��";
			}

		// Group B
		} else {
			// �܂��� ����=1 || ����=5
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "�܂���";

			// �܂��� (���j 
			} else {
				return "�܂���";

			}
		}
	}

	return "���܂���";
}



// �`�܂��� �`��
string ���܂�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�܂���";

			// �� (��)
			} else {
				return "��";
			}

		// Group B
		} else {

			// �܂��� (���j 
			return "�܂���";

		}
	}

	return "���܂�����";
}



// �`��܂��� �`����
string ����܂�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ��܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "��܂���";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {

			// ��܂��� (���j 
			return "��܂���";

		}
	}

	return "����܂�����";
}



// �`�܂���ł��� �`�Ȃ�����
string ���܂���ł�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �܂���ł��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�܂���ł���";

			// �Ȃ����� (��)
			} else {
				return "�Ȃ�����";
			}

		// Group B
		} else {

			// �܂���ł��� (���j 
			return "�܂���ł���";

		}
	}

	return "���܂���ł�����";
}



// �`�v���܂� �`�����܂� �`�v��
string ���v���܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �v���܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�v���܂�";

			// �v�� (��)
			} else {
				return "�v��";
			}

		// Group B
		} else {

			// �����܂� ����=1 or ����=3 or ����
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�����܂�";

			// �v���܂� (��)
			} else {
				return "�v���܂�";
			}

		}
	}

	return "���v���܂���";
}


// �`�Q��܂��� �`�Q�サ�܂��� �`�Q����
string ���Q��܂�����(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �Q��܂��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�Q��܂���";

			// �Q���� (��)
			} else {
				return "�Q����";
			}

		// Group B
		} else {

			// �Q��܂��� ����
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�Q��܂���";

			// �Q�サ�܂��� ����=1 or ����=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "�Q�サ�܂���";

			// �Q��܂��� (��)
			} else {
				return "�Q��܂���";
			}

		}
	}

	return "���Q��܂�����";
}



// �`�������� �`������ �`����
string ���������ꁓ(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ������ ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "������";

			// ���� (��)
			} else {
				return "����";
			}

		// Group B
		} else {

			// ������ ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "������";

			// �������� (��)
			} else {
				return "��������";
			}

		}
	}

	return "���������ꁓ";
}



// �`���݂܂��� �`�\����Ȃ� �`���܂� �`���܂� �`���߂�Ȃ���
string �����݂܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ���߂�Ȃ��� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���߂�Ȃ���";

			// ���܂� ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "���܂�";

			// ���܂� (��)
			} else {
				return "���܂�";
			}

		// Group B
		} else {
			// �\����Ȃ� ����=5 or ���j 
			if ( nb6bushouref[i1st_BushouID].tone == 5 || nb6bushouname[i1st_BushouID].sex == 0 ) {
				return "�\����Ȃ�";

			// ���݂܂��� (���j 
			} else {
				return "���݂܂���";

			}
		}
	}

	return "�����݂܂���";
}

// �`�肢�܂� �`���肢������ �`�肨�� �`�肤 �`���肢���܂�
string ���肢�܂���(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// �肢�܂� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "�肢�܂�";

			// �肤 ����=2 || ����=4
			} else if ( nb6bushouref[i1st_BushouID].tone == 2 || nb6bushouref[i1st_BushouID].tone == 4 ) {
				return "�肤";

			// �肨�� (��)
			} else {
				return "�肨��";
			}

		// Group B
		} else {
			// ���肵�܂� ����=1 or ����=3 or ���� 
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "���肢���܂�";

			// ���肢������ ����=5
			} else if ( nb6bushouref[i1st_BushouID].tone == 5 ) {
				return "��������";

			// �肢�܂� (���j 
			} else {
				return "�肢�܂�";

			}
		}
	}

	return "���肢�܂���";
}



// �`�悢 �`��낵��
string ���悢��(int i1st_BushouID, int i2nd_BushouID, int i3rd_BushouID) {
	if ( 0 <= i1st_BushouID && i1st_BushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ���L�����s�̂ǂꂪ���Ă͂܂邩�H �ǂ�ɂ����Ă͂܂�Ȃ������痪�L�����x�łǂ��炩�ɓ��Ă͂܂�B
		int Group = �����L�����s��(i1st_BushouID,i2nd_BushouID) ;

		// Group A
		if ( Group == 1 ) {

			// ��낵�� ���� 
			if ( nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "��낵��";

			// �悢 (��)
			} else {
				return "�悢";
			}

		// Group B
		} else {

			// ��낵�� ����=1 or ����=3 or ����
			if ( nb6bushouref[i1st_BushouID].tone == 1 || nb6bushouref[i1st_BushouID].tone == 3 || nb6bushouname[i1st_BushouID].sex == 1 ) {
				return "��낵��";

			// �悢 (��)
			} else {
				return "�悢";
			}

		}
	}

	return "���悢��";
}
