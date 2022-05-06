
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"


/*
004CFEC4  91 AB 8C 79 93 AA 00 00 8E 98 91 E5 8F AB 00 00  ���y��..���叫..
004CFED4  95 94 8F AB 00 00 00 00 89 C6 98 56 00 00 00 00  ����....�ƘV....
004CFEE4  8F 68 98 56 00 00 00 00                          �h�V....��.

004D13A8  F0 FE 4C 00 EC FE 4C 00 [A0 FF 4C] 00 [F0 8E 4C] 00   �B�� �喼
004D13B8  [E4 FE 4C] 00 [DC FE 4]�v 00 [D4 FE 4C] 00 [CC FE 4C]  �h�V�`���叫
004D13C8  [C4 FE 4C] 00 00 00 00 00 F0 8E 4C 00 E0 8E 4C 00  ���y��


���ڽ      �ި�������                                                                                         ����
�@00403B9D   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   ��������  �Q�[�����̎��s�ꏊ�͕s���B�����͕����ԍ���EAX�ł͂Ȃ�ESI
�A00438F2A   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]	�������ρ@�ʏ�̕������X�g�̎��ɕ������Ƃɂ�����ʉ߂���B
�B0045E428   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   �������ρ@��p�̃_�C�A���O
�C0046002F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] �������� �R���̕Ґ�(���ȕҐ�) 
�D00463E6F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] �������� �푈���̍���́u�����v����̏��
�E0048F94B   PUSH    DWORD PTR DS:[EAX*4+4D13B0] �������� ���i���̃_�C�A���O�̉����Łu�����ɏ��i���܂����v�̃��b�Z�[�W���̂Ƃ���
�F004935E3   PUSH    DWORD PTR DS:[EAX*4+4D13B0] �������� �������ڍ׉��
�G004A1566   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] �������� �����̏����̍ۂɁA������ʉ߂���B
�H004A1C33   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] �������� �����́u�g�����v�̕����o���_�C�A���O�̎��ɁA������ʉ߂���B(�Ǖ��Ȃ�)
�I004A4B90   PUSH    DWORD PTR DS:[EAX*4+4D13B0] �������� �Q�[�����̎��s�ꏊ�͕s���B
*/




static int GetOverRewriteMibunAddress(int iBushouID, int iPosition=-1 ) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// �f�t�H���g�͌��݂̃|�W�V����������
		if ( iPosition==-1) {
			iPosition = nb6bushouname[iBushouID].position;
		}

		// ScenarioMod�̌Ăяo���B( return ���Ă���̂�ScenarioMod���ɓ����)
		if ( p_snOnViewingPositionName ) {
			int iPointerAddress = 0;
			p_snOnViewingPositionName( &iPointerAddress, iBushouID+1, iPosition );
			if ( iPointerAddress ) {
				return int(iPointerAddress);
			}
		}

		
		/*
		if ( TSModCommand::UltraBushou::cmdExists ) { // �ȉ��A������Əd�����̂Ńt�B���^�[
			if ( iPosition >= 1 ) {
				if ( isLancelotTartare(iBushouID) ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �o�[���[�t�H����
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "�L�M�N") == 0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// ���H���b�N
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�s�t�u" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�v�w�x" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �o���o�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�U�V�W" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�X�Y�Z" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �}���e�B��
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�c�d" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�`�a�b" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �n�{����
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�I�J�K" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�d�e�f" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �I�Y�}
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�v�w�x" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�y�z�{" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "���Z";
								return int(szMibun);
							}
						}
					}
				}
				// �x���T���A
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�^�_�`" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�a�b�c" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "������";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "�Q�d";
								return int(szMibun);
							}
						}
					}
				}
				// �E�H�[����
				else if (strcmp( nb6bushouname[iBushouID].familyname,"�q�r")==0 &&  strcmp( nb6bushouname[iBushouID].fastname, "�n�o�p")==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "�Q�d";
								return int(szMibun);
							}
						}
					}
				}
				// �J�m�[�v�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�O�u" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�[�\�]" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "�b��";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							}
						}
					}
				}
				// �M���o���h
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�g�h�i" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "���" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "�b��";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							}
						}
					}
				}
				// �~���f�B��
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�O�P�Q" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�R�S�T" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							}
						}
					}
				}
				// �M���_�X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�G�H�F" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "������" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							}
						}
					}
				}
				// �f�l�u
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�j�k" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�l�m" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "�Q�d";
								return int(szMibun);
							}
						}
					}
				}
				// ���h����
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "�y�z�{" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "�|�}�~" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // �喼�Ȃ�΁c
						static char szMibun[] = "����";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // �R�c���Ȃ�΁c
						static char szMibun[] = "�c��";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// �������R�t���B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								} else {
									static char szMibun[] = "�R�t";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "�Q�d";
								return int(szMibun);
							}
						}
					}
				}
			}
		}
		*/
		

		int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
		if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
			int iSimbol = nb6daimyo[iDaimyoID].symbol;
			if ( iSimbol == 41 && nb6bushouref[iBushouID].job == 0x40) { // �{�莛�Ɩ�A���ł��A�m��
				if ( iPosition == 1 ) { // �喼����
					static char szJotai[] = "����";
					return int(szJotai);
				} else if ( iPosition == 2 ) { // �h�V����
					if ( nb6bushouref[iBushouID].blood == 41) { // �{�莛�̌��ł����
						static char szJotai[] = "�e��";
						return int(szJotai);
					} else {
						static char szJotai[] = "����";
						return int(szJotai);
					}
				} else if ( iPosition == 3 ) { // �ƘV����
					static char szJotai[] = "����";
					return int(szJotai);
				} else if ( iPosition == 4 ) { // ��������
					static char szJotai[] = "����";
					return int(szJotai);
				} else if ( iPosition == 5 ) { // ���叫����
					static char szJotai[] = "���";
					return int(szJotai);
				} else if ( iPosition == 6 ) { // ���y������
					if ( nb6bushouref[iBushouID].merits != 0 ) {
						static char szJotai[] = "�{��";
						return int(szJotai);
					// �M�����O
					} else {
						static char szJotai[] = "���";
						return int(szJotai);
					}
				}
			} 
		}


	}


	return NULL;
}








//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingMibun1 = -1;
int iEAXPointerOfShowingMibun1 = NULL;
void OnTenshouExeShowingMibun1Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun1 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun1 = ipointer;
		}
	}
}



/*
00403B94  |. 56                   PUSH    ESI			�� �����͑��̓����p�^�[���Ƃ͈����ESI�ɂȂ��Ă���B���̏ꏊ��EAX
00403B95  |. E8 3E680000          CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
00403B9A  |. 83C4 04              ADD     ESP, 4
00403B9D  |. 8B0485 B0134D00      MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
00403BA4  |. A3 70DF4D00          MOV     DWORD PTR DS:[4DDF70], EAX


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun1		=0x403B95; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun1�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun1	=0x403BA4; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun1() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun1, esi

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun1, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun1Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun1

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun1
	}
}

char cmdOnTenshouExeJumpFromShowingMibun1[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun1() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun1 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun1+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun1), cmdOnTenshouExeJumpFromShowingMibun1, 5, NULL); //5�o�C�g�̂ݏ�������
}








//---------------------------------------------------------------------------------------------------------------------------











/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * ECX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingMibun2 = -1;
int iEAXPointerOfShowingMibun2 = NULL;
void OnTenshouExeShowingMibun2Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun2 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun2 = ipointer;
		}
	}
}


/*
00438F22   . E8 B114FDFF    CALL    TENSHOU.0040A3D8
00438F27   . 83C4 04        ADD     ESP, 4
00438F2A   . 8B0485 B0134D00                     MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ;  TENSHOU.004C8EF0
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
00438F31   . EB 0F                               JMP     SHORT TENSHOU.00438F42
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun2		=0x438F22; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun2�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun2	=0x438F31; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun2() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun2, esi; // ESI==004D00BD�݂����ɉ������̃r�b�g�������ԍ�

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun2, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun2Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun2

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun2
	}
}

char cmdOnTenshouExeJumpFromShowingMibun2[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun2() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun2 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun2+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun2), cmdOnTenshouExeJumpFromShowingMibun2, 5, NULL); //5�o�C�g�̂ݏ�������
}

















//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingMibun3 = -1;
int iEAXPointerOfShowingMibun3 = NULL;
void OnTenshouExeShowingMibun3Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun3 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun3 = ipointer;
		}
	}
}



/*
0045E420   . E8 B3BFFAFF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
0045E425   . 83C4 04        ADD     ESP, 4
0045E428   . 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
0045E42F   > 66:8B1F        MOV     BX, WORD PTR DS:[EDI]


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun3		=0x45E420; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun3�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun3	=0x45E42F; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun3() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun3, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun3, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun3Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun3

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun3
	}
}

char cmdOnTenshouExeJumpFromShowingMibun3[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v�W�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun3() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun3 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun3+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun3), cmdOnTenshouExeJumpFromShowingMibun3, 5, NULL); //5�o�C�g�̂ݏ�������
}







//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun4 = -1;
int iMibunNumberOfShowingMibun4 = -1;
int iEAXPointerOfShowingMibun4 = NULL;
void OnTenshouExeShowingMibun4Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun4 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun4 = ipointer;
		}
	}
}



/*
00460026  |. 55             PUSH    EBP
00460027  |. E8 ACA3FAFF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
0046002C  |. 83C4 04        ADD     ESP, 4
0046002F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; |Arg5
00460036  |. 6A 14          PUSH    14                               ; |Arg4 = 00000014


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun4		=0x460027; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun4�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun4	=0x460036; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun4() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun4, EBP

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// �g���ԍ��������Ă�EAX�̒l��ێ����Ă���
		mov iMibunNumberOfShowingMibun4, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun4, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun4Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun4

		PUSH    EAX

		// EAX��g���̒l�ւƖ߂�
		mov EAX, iMibunNumberOfShowingMibun4

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun4
	}
}

char cmdOnTenshouExeJumpFromShowingMibun4[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun4() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun4;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun4 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun4+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun4), cmdOnTenshouExeJumpFromShowingMibun4, 5, NULL); //5�o�C�g�̂ݏ�������
}










//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun5 = -1;
int iMibunNumberOfShowingMibun5 = -1;
int iEAXPointerOfShowingMibun5 = NULL;
void OnTenshouExeShowingMibun5Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun5 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun5 = ipointer;
		}
	}
}



/*
00463E66  |. 56             PUSH    ESI
00463E67  |. E8 6C65FAFF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
00463E6C  |. 83C4 04        ADD     ESP, 4
00463E6F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
00463E76  |. 56             PUSH    ESI



*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun5		=0x463E67; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun5�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun5	=0x463E76; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun5() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun5, ESI

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// �g���ԍ��������Ă�EAX�̒l��ێ����Ă���
		mov iMibunNumberOfShowingMibun5, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun5, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun5Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun5

		PUSH    EAX

		// EAX��g���̒l�ւƖ߂�
		mov EAX, iMibunNumberOfShowingMibun5

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun5
	}
}

char cmdOnTenshouExeJumpFromShowingMibun5[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun5() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun5;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun5 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun5+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun5), cmdOnTenshouExeJumpFromShowingMibun5, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun6 = -1;
void OnTenshouExeShowingMibun6_BeforeExecute() {
}



/*
0048F93F   . 50             PUSH    EAX
0048F940   . E8 93AAF7FF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
0048F945   . 83C4 04        ADD     ESP, 4


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before		=0x48F940; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun6_Before�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun6_Before   =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6_Before	=0x48F945; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6_Before() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// �����ԍ������E���Č��ւƖ߂�
		mov iBushouBangouOfShowingMibun6, EAX

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun6_Before

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun6_BeforeExecute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun6_Before
	}
}

char cmdOnTenshouExeJumpFromShowingMibun6_Before[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun6_Before() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun6_Before;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6_Before+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before), cmdOnTenshouExeJumpFromShowingMibun6_Before, 5, NULL); //5�o�C�g�̂ݏ�������
}



//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iMibunNumberOfShowingMibun6 = -1;
int iEAXPointerOfShowingMibun6 = NULL;
void OnTenshouExeShowingMibun6Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun6 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun6 = ipointer;
		}
	}
}



/*
0048F94B   . FF3485 B0134D00         PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; /Arg2
				�������� JMP TSMod.OnTSExeShowingMibun6 �Ə��������Ă����ۂ̏���
0048F952   . B8 8A1F0000             MOV     EAX, 1F8A                        ; |
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6		=0x48F94B; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun6�ւƃW�����v���Ă���B
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6	=0x48F952; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iMibunNumberOfShowingMibun6, EAX

			// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun6, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun6Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun6

		PUSH    EAX

		// EAX��g���̒l�ւƖ߂�
		mov EAX, iMibunNumberOfShowingMibun6

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun6
	}
}

char cmdOnTenshouExeJumpFromShowingMibun6[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�7�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v8�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun6() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun6;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6), cmdOnTenshouExeJumpFromShowingMibun6, 7, NULL); //7�o�C�g�̂ݏ�������
}




//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun7 = -1;
int iMibunNumberOfShowingMibun7 = -1;
int iEAXPointerOfShowingMibun7 = NULL;
void OnTenshouExeShowingMibun7Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun7 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun7 = ipointer;
		}
	}
}



/*
004935DA  |. 56             PUSH    ESI
004935DB  |. E8 F86DF7FF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
004935E0  |. 83C4 04        ADD     ESP, 4
004935E3  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004935EA  |. 68 DC2E4D00    PUSH    TENSHOU.004D2EDC                 ;  ASCII "%-6s"
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun7		=0x4935DB; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun7�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun7	=0x4935EA; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun7() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun7, ESI

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// �g���ԍ��������Ă�EAX�̒l��ێ����Ă���
		mov iMibunNumberOfShowingMibun7, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun7, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun7Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun7

		PUSH    EAX

		// EAX��g���̒l�ւƖ߂�
		mov EAX, iMibunNumberOfShowingMibun7

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun7
	}
}

char cmdOnTenshouExeJumpFromShowingMibun7[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun7() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun7;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun7 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun7+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun7), cmdOnTenshouExeJumpFromShowingMibun7, 5, NULL); //5�o�C�g�̂ݏ�������
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingMibun8 = -1;
int iEAXPointerOfShowingMibun8 = NULL;
void OnTenshouExeShowingMibun8Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun8 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun8 = ipointer;
		}
	}
}



/*
004A155E  |. E8 758EF6FF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
004A1563  |. 83C4 04        ADD     ESP, 4
004A1566  |. 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]           ;  TENSHOU.004CFEE4
004A156D  |> 66:8B36        MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun8		=0x4A155E; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun8�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun8	=0x4A156D; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun8() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun8, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun8, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun8Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun8

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun8
	}
}

char cmdOnTenshouExeJumpFromShowingMibun8[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun8() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun8;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun8 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun8+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun8), cmdOnTenshouExeJumpFromShowingMibun8, 5, NULL); //5�o�C�g�̂ݏ�������
}












//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 * EAX==004D00BD�݂����ɉ������̃r�b�g�������ԍ�
 */
int iBushouBangouOfShowingMibun9 = -1;
int iEAXPointerOfShowingMibun9 = NULL;
void OnTenshouExeShowingMibun9Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun9 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun9 = ipointer;
		}
	}
}



/*
004A1C2B  |. E8 A887F6FF               CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
004A1C30  |. 83C4 04                   ADD     ESP, 4
004A1C33  |. 8B0485 B0134D00           MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
004A1C3A  |> 66:8B36                   MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun9		=0x4A1C2B; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun9�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun9	=0x4A1C3A; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun9() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun9, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun9, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun9Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun9

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun9
	}
}

char cmdOnTenshouExeJumpFromShowingMibun9[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun9() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun9;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun9 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun9+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun9), cmdOnTenshouExeJumpFromShowingMibun9, 5, NULL); //5�o�C�g�̂ݏ�������
}





//---------------------------------------------------------------------------------------------------------------------------


/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun10 = -1;
int iMibunNumberOfShowingMibun10 = -1;
int iEAXPointerOfShowingMibun10 = NULL;
void OnTenshouExeShowingMibun10Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun10 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun10 = ipointer;
		}
	}
}



/*
004A4B87  |. 56             PUSH    ESI
004A4B88  |. E8 4B58F6FF    CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibunBefore �Ə��������Ă����ۂ̏���
004A4B8D  |. 83C4 04        ADD     ESP, 4
004A4B90  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004A4B97  |. 56             PUSH    ESI
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun10		=0x4A4B88; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun10�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10 =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun10	=0x4A4B97; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun10() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		mov iBushouBangouOfShowingMibun10, ESI

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// �g���ԍ��������Ă�EAX�̒l��ێ����Ă���
		mov iMibunNumberOfShowingMibun10, eax

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun10, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun10Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun10

		PUSH    EAX

		// EAX��g���̒l�ւƖ߂�
		mov EAX, iMibunNumberOfShowingMibun10

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun10
	}
}

char cmdOnTenshouExeJumpFromShowingMibun10[6]="\xE9";
// ���̖��߂�5�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v6�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun10() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun10;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun10 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun10+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun10), cmdOnTenshouExeJumpFromShowingMibun10, 5, NULL); //5�o�C�g�̂ݏ�������
}










/*
004300DA  |. 83C4 04        |ADD     ESP, 4
								// ���i�͍��̐g���̎��̐g�����o���K�v������̂�(4D13B0 + (EAX-1)*4) => EAX*4+4D13AC�Ƃ����v�Z���ƂȂ��Ă���B
004300DD  |. 8B0485 AC134D0>|MOV     EAX, DWORD PTR DS:[EAX*4+4D13AC]
004300E4  |> 68 88000000    |PUSH    88                                                  ; /Arg7 = 00000088
004300E9  |. 83C3 02        |ADD     EBX, 2                                              ; |
004300EC  |. FF75 F0        |PUSH    DWORD PTR SS:[EBP-10]                               ; |Arg6
004300EF  |. 50             |PUSH    EAX                                                 ; |Arg5
004300F0  |. 6A 10          |PUSH    10                                                  ; |Arg4 = 00000010
004300F2  |. FF75 E8        |PUSH    DWORD PTR SS:[EBP-18]                               ; |Arg3
004300F5  |. FF75 F4        |PUSH    DWORD PTR SS:[EBP-C]                                ; |Arg2
004300F8  |. 68 88000000    |PUSH    88                                                  ; |Arg1 = 00000088
004300FD  |. E8 DE580500    |CALL    TENSHOU.004859E0                                    ; \TENSHOU.004859E0
00430102  |. 83C4 1C        |ADD     ESP, 1C
00430105  |. FF45 F8        |INC     DWORD PTR SS:[EBP-8]
00430108  |. 8345 F4 18     |ADD     DWORD PTR SS:[EBP-C], 18
0043010C  |. 8B4D F8        |MOV     ECX, DWORD PTR SS:[EBP-8]
0043010F  |. 398E AC000000  |CMP     DWORD PTR DS:[ESI+AC], ECX
00430115  |.^0F87 66FFFFFF  \JA      TENSHOU.00430081
*/




/*
 * �g����\������Ƃ��ɁA�����ԍ����E���Ă���
 */
int iBushouBangouOfShowingMibun11 = -1;
int iMibunNumberOfShowingMibun11 = -1;
int iEAXPointerOfShowingMibun11 = NULL;
void OnTenshouExeShowingMibun11Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun11 & 0xFFFF) - 1; // �������������ԍ�

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int inext_position = nb6bushouname[iBushouID].position-1;
		int ipointer = GetOverRewriteMibunAddress(iBushouID, inext_position);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun11 = ipointer;
		}
	}
}



/*
004300D2  |. FF75 FE        |PUSH    DWORD PTR SS:[EBP-2]
004300D5  |. E8 FEA2FDFF    |CALL    TENSHOU.0040A3D8
				�������� JMP TSMod.OnTSExeShowingMibun11 �Ə��������Ă����ۂ̏���
004300DA  |. 83C4 04        |ADD     ESP, 4
004300DD  |. 8B0485 AC134D0>|MOV     EAX, DWORD PTR DS:[EAX*4+4D13AC]
004300E4  |> 68 88000000    |PUSH    88                                                  ; /Arg7 = 00000088

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun11		 =0x4300D5; // �֐��͂��̃A�h���X����AOnTenshouExeShowingMibun11�ւƃW�����v���Ă���B
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11  =0x40A3D8; // ���XTENSHOU.EXE���ɂ�����CALL��
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun11	 =0x4300E4; // �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w
// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun11() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {

		// ���XTENSHOU.EXE���ɂ��������̂�������
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11

		// ���XTENSHOU.EXE���ɂ��������̂�������
		ADD     ESP, 4

		// �g���ԍ��������Ă�EAX�̒l��ێ����Ă���
		mov iMibunNumberOfShowingMibun11, eax

		// �����ԍ��������o�����߂�EAX�̊Ԏ؂�
		mov eax, [EBP-0x2] // �����̔ԍ�
		mov iBushouBangouOfShowingMibun11, eax

		// eax��g���ԍ��ɖ߂�
		mov eax, iMibunNumberOfShowingMibun11

		// ���XTENSHOU.EXE���ɂ��������̂�������
		// (�E�Ӓl��EAX�Ƃ͐g���̒l�A�uEAX*4+0x4D13B0�v���e�g���̕�����ւ̃|�C���^�������Ă���A�h���X�ł��ˁA���Ӓl��EAX�Ŏ󂯎���Ă���)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13AC]  // ���i�͍��̐g���̎��̐g�����o���K�v������̂�(4D13B0 + (EAX-1)*4) => EAX*4+4D13AC�Ƃ����v�Z���ƂȂ��Ă���B

		mov iEAXPointerOfShowingMibun11, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun11Execute();

	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// �㏑������K�v������Ȃ�Ώ㏑������B
		mov EAX, iEAXPointerOfShowingMibun11

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun11
	}
}

char cmdOnTenshouExeJumpFromShowingMibun11[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ���̖��߂�7�o�C�g�A�Ȍ�̊֐��Ő��܂�閽�߂����v8�o�C�g�Ȃ̂Łc �Ō�P�g��Ȃ�


// �j�[���j�b�N���������p
void WriteAsmJumperOnTenshouExeShowingMibun11() {

	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeShowingMibun11;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun11 + 5 );
		// �T�Ƃ����̂́A0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromShowingMibun11+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B

	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun11), cmdOnTenshouExeJumpFromShowingMibun11, 7, NULL); //7�o�C�g�̂ݏ�������
}









