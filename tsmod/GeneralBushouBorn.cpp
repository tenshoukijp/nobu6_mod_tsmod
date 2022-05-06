#define _CRT_SECURE_NO_WARNINGS

/*
 �ėp�����a������
 */

#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "FactoryGeneralBushouName.h"
#include "ScenarioMod.h"

int aptitude_coef() {
	int num = rand()%10+rand()%10;
	if ( num == 18 ) { return 5; }
	if ( num >= 16 ) { return 4; }
	if ( num >= 13 ) { return 3; }
	if ( num >=  9 ) { return 2; }
	if ( num >=  4 ) { return 1; }
	if ( num >=  0 ) { return 0; }
	return 0;
};
void bornAGeneralBushou(int iBushouID, int iCastleID, int iForthFaceID = -1) {
	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM &&
		0 <= iCastleID && iCastleID < GAMEDATASTRUCT_CASTLE_NUM ) {
		// �܂������̋����̏ꏊ
		nb6bushouref[iBushouID].castle = iCastleID+1;

		// ���݂̘Q�l�̐擪�̐l
		int iOldRoninHeader = nb6castle[iCastleID].ronin;

		nb6castle[iCastleID].ronin = iBushouID+1; // �Q�l�̐擪�͎���

		// �����̑O�͋��Ȃ�
		nb6bushouref[iBushouID].prev = 0xFFFF;

		// �����̎������܂Ő擪�������l
		nb6bushouref[iBushouID].next = iOldRoninHeader;

		nb6bushouref[iBushouID].birth = nb6year.year - 15 - rand() % 35; // 15�΁`50��

		int maxgov = 8 + rand()%46+rand()%46;
		int maxbat = 14 + rand()%43+rand()%43;
		int maxint = 2 + rand()%49+rand()%49;
		if ( maxgov < 8 ) { maxgov = 8; }
		if ( maxgov > 100 ) { maxgov = 100; }
		if ( maxbat < 14 ) { maxbat = 14; }
		if ( maxbat > 100 ) { maxbat = 100; }
		if ( maxint < 6) { maxint = 5; }
		if ( maxint > 100 ) { maxint = 100; }
		nb6bushouref[iBushouID].maxgov = maxgov;
		nb6bushouref[iBushouID].maxbat = maxbat;
		nb6bushouref[iBushouID].maxint = maxint; 
		nb6bushouref[iBushouID].expgov = 300 + getYearsOld(iBushouID) * 20;
		nb6bushouref[iBushouID].expbat = 300 + getYearsOld(iBushouID) * 20;
		nb6bushouref[iBushouID].expint = 300 + getYearsOld(iBushouID) * 20;
		nb6bushouref[iBushouID].charm =  15 + rand() % 44 + rand() % 43;
		nb6bushouref[iBushouID].ambition = 6 + rand() % 48 + rand() % 48;
		nb6bushouref[iBushouID].loyal = 0;
		nb6bushouref[iBushouID].merits = 0;
		nb6bushouref[iBushouID].blood = 0xFF;
		nb6bushouref[iBushouID].attach = 0xFFFF;
		nb6bushouref[iBushouID].gundan = 0xFFFF;
		nb6bushouref[iBushouID].rise = 0;
		nb6bushouref[iBushouID].soldier = 0;
		nb6bushouref[iBushouID].training = 0;
		nb6bushouref[iBushouID].solloyal = 0;
		nb6bushouref[iBushouID].form = 0; // ���y
		nb6bushouref[iBushouID].work = 0;
		nb6bushouref[iBushouID].spouse = 0xFF;
		nb6bushouref[iBushouID].parent = 0xFFFF; // �e�͖���

		nb6bushouref[iBushouID].algo = 3; // �e�͖���
		/*
		0�c�����ƌĂ΂ꂽ�l���B�L�������ɑ����B�M���G�g�ƍN�ق��A�֓����O�⏼�i�v�G���B
		  �����I�ȃo�����X�̗ǂ��^�C�v�����Acom�̏ꍇ���͂΂��葝�₵�A���܂�푈���Ȃ��B
		1�c�ҏ��ɑ����B�ēc���Ƃ�`��i�Ƃ�암������B
		2�c�퓬�ɋ��������������B�����ȂǁB
		3�c�퓬�����B���쎁�^��B�����D���H
		4�c�����]�ᓙ�A�V��E���l���̕����l�n�����B�����D���H
		5�c�E�ҕ�����A�v���e���E���n�@���`�ȂǁB�����D���B
		6�c�ڗ����Ȃ��U�R�����B���\�^�C�v�H
		*/
		nb6bushouref[iBushouID].duty = rand() % 8; // 0�`7		
		nb6bushouref[iBushouID].affinity = rand() % 8; // ���� 0�`7		
		nb6bushouref[iBushouID].aptitudegov = nb6bushouref[iBushouID].maxgov / 40; //c-���K		
		nb6bushouref[iBushouID].aptitudebat = nb6bushouref[iBushouID].maxbat / 40; //b-��K		
		nb6bushouref[iBushouID].aptitudeint = nb6bushouref[iBushouID].maxint / 40; //a-�q�K		

		// �኱�h���
		if ( nb6bushouref[iBushouID].aptitudegov == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudegov--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudegov++; }
		}
		// �኱�h���
		if ( nb6bushouref[iBushouID].aptitudebat == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudebat--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudebat++; }
		}
		// �኱�h���
		if ( nb6bushouref[iBushouID].aptitudeint == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudeint--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudeint++; }
		}

		nb6bushouref[iBushouID].tech_ikkatsu = (rand() % 10 == 0);	//�Z�\�E�ꊅ  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_ansatsu = (rand() % 30 == 0);	//�Z�\�E�ÎE  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_ryusyutsu = (rand() % 10 == 0);	//�Z�\�E���o  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_sendou = (rand() % 10 == 0); //�Z�\�E����  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_ryugen = (rand() % 10 == 0);	//�Z�\�E����  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_benzetsu = (rand() % 10 == 0);	//�Z�\�E�ِ�  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_yakiuchi = (rand() % 10 == 0);	//�Z�\�E�ē�  �㏑���Ȃ̂ł����OK
		nb6bushouref[iBushouID].tech_chohatsu = (rand() % 10 == 0);	//�Z�\�E����  �㏑���Ȃ̂ł����OK


		int ashi = aptitude_coef();
		setBushouAptitude( iBushouID, APTITUDE_ASIGARU, ashi ); // A,S�ɂ͂Ȃ�ɂ����悤��

		int kiba = aptitude_coef();
		if ( rand()%4 == 0 && kiba > 0 ) {
			kiba--;
		}
		setBushouAptitude( iBushouID, APTITUDE_KIBA, kiba ); // A,S�ɂ͂Ȃ�ɂ����悤��

		int tepu = aptitude_coef();
		if ( rand()%3 == 0 && tepu > 0 ) {
			tepu--;
		}
		// �S�C�ɂ��ẮA�`�����Ă邩���ĂȂ����ŕ�����B
		// �M���V���[�Y���n�m�ɂ��Ă���ƁA�S�C�`���Ɋւ�炸�S�C�����͕t���̂ŁA�����炪�n�m���Ǝ����I
		if ( isTeppouDenrai() || TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) {
			setBushouAptitude( iBushouID, APTITUDE_TEPPOU, tepu ); // A,S�ɂ͂Ȃ�ɂ����悤��
		} else {
			setBushouAptitude( iBushouID, APTITUDE_TEPPOU, 0 ); // �S�C�`�����ĂȂ������͂d
		}

		int suig = aptitude_coef();
		if ( rand()%2 == 0 && suig > 0 ) {
			suig--;
		}
		setBushouAptitude( iBushouID, APTITUDE_SUIGUN, suig ); // A,S�ɂ͂Ȃ�ɂ����悤��

		int yumi = aptitude_coef()-1;
		int yari = aptitude_coef()-1;

		// �|��TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
		// �|�̓K�����R�s�[
		// �|�������Ƃ����t���O��ON�Ȃ̂ł���΁A0�ŏ㏑������B
		if ( TSModCommand::World::cmdTheYumiNoExists ) {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
		} else {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = yumi;
		}
		// ����TSMod.txt�̐ݒ�ɂ͊֌W�Ȃ��ݒ肷��B
		// ���̓K�����R�s�[
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = yari;

		// ���ꑮ���́A��ʕ����͎������킹�Ȃ��̂ŁA�S�ăN���A
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

		int job = rand() % 20;
		// �ǉ������Ƃ��āA���y�`�ȏ�ŁA�����̂Q�{�ȏ�퓬������
		if ( job <= 1 && 
			getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 4 && 
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov*2 ) { // ���y�`�ȏ�
			nb6bushouref[iBushouID].job = 0x10;	// �E��
		} else if ( job == 2 ) {
			nb6bushouref[iBushouID].job = 0x20;	// ���l

			// �ǉ������Ƃ��Đ퓬����ԍ���
		} else if ( job <= 4 && 
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov &&
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxint ) {
			nb6bushouref[iBushouID].job = 0x30;	// ����
		} else if ( job == 5 ) {
			nb6bushouref[iBushouID].job = 0x40;	// �m��

			// ���R�a�ȏ�ŃT�C�R���V�ȉ�
		} else if ( job <= 7 && getBushouAptitude(iBushouID, APTITUDE_SUIGUN) >= 3 && nb6bushouref[iBushouID].maxbat >= 50 ) {
			nb6bushouref[iBushouID].job = 0x50;	// �C��

		} else if ( job == 8 ) {
			nb6bushouref[iBushouID].job = 0x60;	// ����
		} else {

			// �W���u�͂Ȃ�
			nb6bushouref[iBushouID].job = 0;	
		}

		nb6bushouref[iBushouID].biggun = 0;    //��C
		nb6bushouref[iBushouID].horsegun = 0;  //�R�n�S�C
		nb6bushouref[iBushouID].steelship = 0; //�S�b�D

		nb6bushouref[iBushouID].life = rand() % 8; //����
		nb6bushouref[iBushouID].brave = rand() % 8; //�E��

		setBushouIndependence( iBushouID, rand() % 8 ); // �Ɨ�
		nb6bushouref[iBushouID].tone = rand() % 8;
		nb6bushouref[iBushouID].bEnd = 0;
		nb6bushouref[iBushouID].bRise = 0; //����
		nb6bushouref[iBushouID].grudge = 0; //�⍦
		nb6bushouref[iBushouID].kaiken = 0; //�

		int fnumber;
		 // �m�������l
		if ( nb6bushouref[iBushouID].job == 0x40 ||nb6bushouref[iBushouID].job == 0x20  ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::���l�m��, rand()%16, rand()%4, rand()%4 );

		// ����
		} else if ( nb6bushouref[iBushouID].job == 0x30 ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType:: ����, rand()%16, rand()%4, rand()%4 );

		// �E��
		} else if ( nb6bushouref[iBushouID].job == 0x10 ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType:: �E��, rand()%16, rand()%3, rand()%4 ); // �E�҂͖ڂ̂S�Ԗڂ�����������݂����Ȃ̂ɂȂ��Ă�̂ŏ��O

		// �S���l���Ⴂ
		} else if ( nb6bushouref[iBushouID].maxgov < 50 &&
					nb6bushouref[iBushouID].maxbat < 50 &&
			        nb6bushouref[iBushouID].maxgov < 50 ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}���s�N, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����V, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�m���V�N, rand()%16, rand()%4, rand()%4 );
			}

		// ��������ԍ����Ȃ��
		} else if ( nb6bushouref[iBushouID].maxgov >= nb6bushouref[iBushouID].maxbat &&
			        nb6bushouref[iBushouID].maxgov >= nb6bushouref[iBushouID].maxint ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�m�����, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�m���s�N, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�m�����V, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�m���V�N, rand()%16, rand()%4, rand()%4 );
			}

		// �퓬����ԍ����Ȃ��
		} else if ( nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov &&
			        nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxint ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�Z�����, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�Z���s�N, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�Z�����V, rand()%16, rand()%4, rand()%4 );
			}

		// �q�d����ԍ����Ȃ��
		} else if ( nb6bushouref[iBushouID].maxint >= nb6bushouref[iBushouID].maxgov &&
			        nb6bushouref[iBushouID].maxint >= nb6bushouref[iBushouID].maxbat ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}���s�N, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����V, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}���V�N, rand()%16, rand()%4, rand()%4 );
			}

		// ����ȊO(�q�d����ԍ����̂Ɠ���)
		} else {
			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}���s�N, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}�����V, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::�}���V�N, rand()%16, rand()%4, rand()%4 );
			}
		}

		nb6bushouname[iBushouID].number = fnumber;

		// ���O�𐶐�
		getFactoryGeneralName( nb6bushouname[iBushouID].familyname, nb6bushouname[iBushouID].fastname );

		nb6bushouname[iBushouID].position = 6; // �g���͑��y
		nb6bushouname[iBushouID].State = 4; // ��Ԃ͘Q�l
		nb6bushouname[iBushouID].sex = 0; // �j

		// ��ԍ����w�肳��Ă���Ȃ�΁c
		if ( iForthFaceID != -1 ) {
			// ����߂�B�������X�]�v�Ȃ��Ƃ͂��Ȃ��B
			nb6bushouname[iBushouID].number = iForthFaceID;

		// �ȉ��ėp�琶���^�C�v(��`�Ȃ�)
		} else {

			// �Ō�ɒN���̎q���ɂȂ�\��������B
			// ������̐l���������20�Έȏ�N��Ȃ�A���̐l�̎q���ɂȂ�\��������B
			for ( int iOyaBushouID=0; iOyaBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iOyaBushouID++ ) {
				// �����������Q�l�Ɠ�����ɂ���
				if ( nb6bushouref[iOyaBushouID].castle == iCastleID+1 ) {
					 // �e��16�Έȏ�A�������N��
					if ( getYearsOld(iOyaBushouID) >= getYearsOld(iBushouID)+16 ) {

						// �e�͍ŏ����炢��1300�l�̕���
						if ( nb6bushouname[iOyaBushouID].number < 1300 ) {
							// �e�̊�ԍ����Z�b�g
							nb6bushouref[iBushouID].parent = nb6bushouname[iOyaBushouID].number;
							// �����𓯂���
							nb6bushouref[iBushouID].blood =  nb6bushouref[iOyaBushouID].blood;

							if ( nb6bushouname[iOyaBushouID].familyname && strlen( nb6bushouname[iOyaBushouID].familyname ) > 1 ) {
								// �e�̖��O��Ⴄ
								strcpy( nb6bushouname[iBushouID].familyname, nb6bushouname[iOyaBushouID].familyname );

								if ( nb6bushouname[iOyaBushouID].State <= 2 && !isPlayerBushou(iOyaBushouID) ) { // �e�͑喼�E�R�c���E�����ŁA�v���C���[�����ȊO
									// �Q�l��������
									setRouninToGeneki( iBushouID );
								}
								break;
							}

						// ��`�͖����l���e
						} else {
							// �����𓯂���
							nb6bushouref[iBushouID].blood =  nb6bushouref[iOyaBushouID].blood;

							if ( nb6bushouname[iOyaBushouID].familyname && strlen( nb6bushouname[iOyaBushouID].familyname ) > 1 ) {
								// �e�̖��O��Ⴄ
								strcpy( nb6bushouname[iBushouID].familyname, nb6bushouname[iOyaBushouID].familyname );
								if ( nb6bushouname[iOyaBushouID].State <= 2 && !isPlayerBushou(iOyaBushouID) ) { // �e�͑喼�E�R�c���E�����E�B���ŁA�v���C���[�����ȊO
									// �Q�l��������
									setRouninToGeneki( iBushouID );
								}
								break;
							}
						}
					}
				}
			}
		}

		// ScenarioMod�ւ̉��z�����a���ʒm
		if (p_snOnBornGeneralBushou) {
			p_snOnBornGeneralBushou(iBushouID);
		}
	}
}


// TSMod.txt�̐ݒ�ɂ���ẮA���S�g�ɒǉ��ŁA�ėp������a��������B
// �ėp�������a�������B�i��������a��)
void GeneralBushouBornExecute() {

	// ���z������o�ꂳ����t���O��ON�ɂ��Ă��Ȃ��Ȃ�΁A�������Ȃ��B
	if ( !TSModCommand::World::cmdVirtualBushouAppear ) {
		return;
	}

	// �V�i���I�J�n�N�ƍ��̐���ꏏ�Ȃ�A�������Ȃ��B
	if ( getYearAD() == getStartYearADOfScenarios() ) {
		return;
	}


	vector<int> vDeathArray;

	// ���S�҂̃J�E���g�𐔂���B
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM-32; iBushouID++ ) { // ���̕�32�X���b�g�ɂ͓���Ȃ��B
		// ���S�҃��X�g����
		if ( nb6bushouname[iBushouID].State == 7 ) { // 7 == ���S
			if ( nb6bushouref[iBushouID].prev == 0xFFFF && nb6bushouref[iBushouID].next == 0xFFFF ) { // 7 == ���S���Ă��āA�����N���X�g���؂�Ă�l
				vDeathArray.push_back(iBushouID);
			}
		}
	}

	// �c��g��10�ɂȂ�܂ŁA���S�Ҙg�ɐ����҂𐶐�����B
	int iDeathRemainCnt = vDeathArray.size();

	// ���S�g�ɕ������ǂ�ǂ�a��������B
	for ( vector<int>::iterator it=vDeathArray.begin(); it != vDeathArray.end(); it++, iDeathRemainCnt-- ) {
		if ( *it < GAMEDATASTRUCT_BUSHOU_NUM-32 ) { // 500�����̂ݏ���������
			// 10�l�ȏ㎀�S�Ҙg���̂����Ă���Ȃ�΁c
			if ( iDeathRemainCnt >= 10 ) {
				// �ǂ��̏�ł������_����
				int iCastleID = rand() % GAMEDATASTRUCT_CASTLE_NUM;
				bornAGeneralBushou(*it, iCastleID);
			}
		}
	}
}




// �ėp�������a�������B�i1�l�a���j�B�O���Q�Ɨp
int WINAPI Extern_GeneralABushouBornExecute(int iForthFaceID) {

	int iTargetBushouIDSlot = -1;

	// ���S�҂̃J�E���g�𐔂���B
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM-32; iBushouID++ ) { // ���̕�32�X���b�g�ɂ͓���Ȃ��B
		// ���S�҃��X�g����
		if ( nb6bushouname[iBushouID].State == 7 ) { // 7 == ���S
			if ( nb6bushouref[iBushouID].prev == 0xFFFF && nb6bushouref[iBushouID].next == 0xFFFF ) { // 7 == ���S���Ă��āA�����N���X�g���؂�Ă�l
				iTargetBushouIDSlot = iBushouID;
				break;
			}
		}
	}

	// �g���c���ĂȂ������玸�s��Ԃ�
	if (iTargetBushouIDSlot == -1) {
		return -1;
	}

	if ( iTargetBushouIDSlot < GAMEDATASTRUCT_BUSHOU_NUM-32 ) { // 500�����̂ݏ���������
		// �ǂ��̏�ł������_����
		int iCastleID = rand() % GAMEDATASTRUCT_CASTLE_NUM;
		// �����a��
		bornAGeneralBushou(iTargetBushouIDSlot, iCastleID, iForthFaceID);
		// �X���b�g�̔ԍ���Ԃ��B
		return iTargetBushouIDSlot;
	}

	return -1;
}
