#include "WinTarget.h"

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"



/*
 * TSMod.txtで指定すると、武将は相場変化時に自動で鍛錬する。(1～10で設定出来る)
 */
int getBeginDeclineGovYears(int iBushouID); // 政治老成の開始時期
int getBeginDeclineBatYears(int iBushouID); // 戦闘老齢の開始時期
int getBeginDeclineIntYears(int iBushouID); // 智謀老齢の開始時期

// 相場変更時に、熟練度自動鍛錬する。(技能も覚える)
void EXSuperService_AutoUpExperienceAndTechOnSobaChange() {
	
	if ( ! TSModCommand::AllBushou::cmdAutoUpExperienceAndTech ) {
		return;
	}

	// 長いので複製。この係数の値が大きいほど、熟練度が上がる度合いが大きく、技能を覚える確率も高い。
	int iCoef = TSModCommand::AllBushou::cmdAutoUpExperienceAndTech;

	// 政治・戦闘・智謀 の熟練度を自動で上昇させる。
	for ( int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM; iBushouID++ ) {

		// まだ政治老齢が始まっていなければ…
		if ( getYearsOld(iBushouID ) < getBeginDeclineGovYears(iBushouID) ) {
			nb6bushouref[iBushouID].expgov += iCoef*3;
		}
		//  まだ戦闘老齢が始まっていなければ…
		if ( getYearsOld(iBushouID ) < getBeginDeclineBatYears(iBushouID) ) {
			nb6bushouref[iBushouID].expbat += iCoef*3; 
		}
		//  まだ智謀老齢が始まっていなければ…
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

		// 技能を覚えるかどうか。係数が大きいほど覚えやすい。
		if ( iCoef > 0 && rand() % (150/iCoef) == 0 ) {

			// 技能をランダムで手に入れる
			int r = rand() % 8;
			switch (r) {
				case 0:
					{
						nb6bushouref[iBushouID].tech_ikkatsu |= TRUE;
					}
					break;
				case 1:
					{
						// 暗殺はそのままだと確率が高いので1/3
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

