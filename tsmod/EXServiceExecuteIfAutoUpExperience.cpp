#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txt�Ŏw�肷��ƁA�����͑���ω����Ɏ����Œb�B����B(1�`10�Őݒ�o����)
 */
int getBeginDeclineGovYears(int iBushouID); // �����V���̊J�n����
int getBeginDeclineBatYears(int iBushouID); // �퓬�V��̊J�n����
int getBeginDeclineIntYears(int iBushouID); // �q�d�V��̊J�n����

// ����ύX���ɁA�n���x�����b�B����B(�Z�\���o����)
void EXSuperService_AutoUpExperienceAndTechOnSobaChange() {
	
	if ( ! TSModCommand::AllBushou::cmdAutoUpExperienceAndTech ) {
		return;
	}

	// �����̂ŕ����B���̌W���̒l���傫���قǁA�n���x���オ��x�������傫���A�Z�\���o����m���������B
	int iCoef = TSModCommand::AllBushou::cmdAutoUpExperienceAndTech;

	// �����E�퓬�E�q�d �̏n���x�������ŏ㏸������B
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// �܂������V��n�܂��Ă��Ȃ���΁c
		if ( getYearsOld(iBushouID ) < getBeginDeclineGovYears(iBushouID) ) {
			nb6bushouref[iBushouID].expgov += iCoef*3;
		}
		//  �܂��퓬�V��n�܂��Ă��Ȃ���΁c
		if ( getYearsOld(iBushouID ) < getBeginDeclineBatYears(iBushouID) ) {
			nb6bushouref[iBushouID].expbat += iCoef*3; 
		}
		//  �܂��q�d�V��n�܂��Ă��Ȃ���΁c
		if ( getYearsOld(iBushouID ) < getBeginDeclineIntYears(iBushouID) ) {
			nb6bushouref[iBushouID].expint += iCoef*3;
		}

		if ( nb6bushouref[iBushouID].expgov > 2000 ) {
			nb6bushouref[iBushouID].expgov = 2000; 
		}
		if ( nb6bushouref[iBushouID].expbat > 2000 ) {
			nb6bushouref[iBushouID].expbat = 2000; 
		}
		if ( nb6bushouref[iBushouID].expint > 2000 ) {
			nb6bushouref[iBushouID].expint = 2000;
		}

		// �Z�\���o���邩�ǂ����B�W�����傫���قǊo���₷���B
		if ( iCoef > 0 && rand() % (150/iCoef) == 0 ) {

			// �Z�\�������_���Ŏ�ɓ����
			int r = rand() % 8;
			switch (r) {
				case 0:
					{
						nb6bushouref[iBushouID].tech_ikkatsu |= TRUE;
					}
					break;
				case 1:
					{
						// �ÎE�͂��̂܂܂��Ɗm���������̂�1/3
						if (rand() % 3 == 1) {
							nb6bushouref[iBushouID].tech_ansatsu |= TRUE;  
						}
					}
					break;
				case 2:
					{
						nb6bushouref[iBushouID].tech_ryusyutsu |= TRUE;
					}
					break;
				case 3:
					{
						nb6bushouref[iBushouID].tech_sendou |= TRUE;
					}
					break;
				case 4:
					{
						nb6bushouref[iBushouID].tech_ryugen |= TRUE;	
					}
					break;
				case 5:
					{
						nb6bushouref[iBushouID].tech_benzetsu |= TRUE; 
					}
					break;
				case 6:
					{
						nb6bushouref[iBushouID].tech_yakiuchi |= TRUE;
					}
					break;
				case 7:
					{
						nb6bushouref[iBushouID].tech_chohatsu |= TRUE; 
					}
					break;
				default:
					;
			}
		}
	}
}

