#define _CRT_SECURE_NO_WARNINGS

/*
 汎用武将誕生処理
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
		// まず自分の居る城の場所
		nb6bushouref[iBushouID].castle = iCastleID+1;

		// 現在の浪人の先頭の人
		int iOldRoninHeader = nb6castle[iCastleID].ronin;

		nb6castle[iCastleID].ronin = iBushouID+1; // 浪人の先頭は自分

		// 自分の前は居ない
		nb6bushouref[iBushouID].prev = 0xFFFF;

		// 自分の次が今まで先頭だった人
		nb6bushouref[iBushouID].next = iOldRoninHeader;

		nb6bushouref[iBushouID].birth = nb6year.year - 15 - rand() % 35; // 15歳～50歳

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
		nb6bushouref[iBushouID].form = 0; // 足軽
		nb6bushouref[iBushouID].work = 0;
		nb6bushouref[iBushouID].spouse = 0xFF;
		nb6bushouref[iBushouID].parent = 0xFFFF; // 親は無し

		nb6bushouref[iBushouID].algo = 3; // 親は無し
		/*
		0…名将と呼ばれた人物。有名武将に多い。信長秀吉家康ほか、斎藤道三や松永久秀も。
		  総合的なバランスの良いタイプだが、comの場合兵力ばかり増やし、あまり戦争しない。
		1…猛将に多い。柴田勝家や柿崎景家や南部晴政ら。
		2…戦闘に強い武将が多い。剣豪など。
		3…戦闘嫌い。今川氏真ら。政治好き？
		4…板部岡江雪等、坊主・茶人等の文化人系武将。政治好き？
		5…忍者武将や、久武親直・金地院崇伝など。調略好き。
		6…目立たないザコ武将。無能タイプ？
		*/
		nb6bushouref[iBushouID].duty = rand() % 8; // 0～7		
		nb6bushouref[iBushouID].affinity = rand() % 8; // 相性 0～7		
		nb6bushouref[iBushouID].aptitudegov = nb6bushouref[iBushouID].maxgov / 40; //c-政適		
		nb6bushouref[iBushouID].aptitudebat = nb6bushouref[iBushouID].maxbat / 40; //b-戦適		
		nb6bushouref[iBushouID].aptitudeint = nb6bushouref[iBushouID].maxint / 40; //a-智適		

		// 若干揺れる
		if ( nb6bushouref[iBushouID].aptitudegov == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudegov--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudegov++; }
		}
		// 若干揺れる
		if ( nb6bushouref[iBushouID].aptitudebat == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudebat--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudebat++; }
		}
		// 若干揺れる
		if ( nb6bushouref[iBushouID].aptitudeint == 1 ) {
			if ( rand() % 5 == 0 ) { nb6bushouref[iBushouID].aptitudeint--; }
			if ( rand() % 5 == 1 ) { nb6bushouref[iBushouID].aptitudeint++; }
		}

		nb6bushouref[iBushouID].tech_ikkatsu = (rand() % 10 == 0);	//技能・一喝  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_ansatsu = (rand() % 30 == 0);	//技能・暗殺  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_ryusyutsu = (rand() % 10 == 0);	//技能・流出  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_sendou = (rand() % 10 == 0); //技能・煽動  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_ryugen = (rand() % 10 == 0);	//技能・流言  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_benzetsu = (rand() % 10 == 0);	//技能・弁舌  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_yakiuchi = (rand() % 10 == 0);	//技能・焼討  上書きなのでこれでOK
		nb6bushouref[iBushouID].tech_chohatsu = (rand() % 10 == 0);	//技能・挑発  上書きなのでこれでOK


		int ashi = aptitude_coef();
		setBushouAptitude( iBushouID, APTITUDE_ASIGARU, ashi ); // A,Sにはなりにくいように

		int kiba = aptitude_coef();
		if ( rand()%4 == 0 && kiba > 0 ) {
			kiba--;
		}
		setBushouAptitude( iBushouID, APTITUDE_KIBA, kiba ); // A,Sにはなりにくいように

		int tepu = aptitude_coef();
		if ( rand()%3 == 0 && tepu > 0 ) {
			tepu--;
		}
		// 鉄砲については、伝来してるかしてないかで分ける。
		// 信長シリーズをＯＮにしていると、鉄砲伝来に関わらず鉄砲属性は付くので、こちらがＯＮだと自動的
		if ( isTeppouDenrai() || TSModCommand::AllBushou::cmdParamConsiderNobunagaSeries ) {
			setBushouAptitude( iBushouID, APTITUDE_TEPPOU, tepu ); // A,Sにはなりにくいように
		} else {
			setBushouAptitude( iBushouID, APTITUDE_TEPPOU, 0 ); // 鉄砲伝来してないうちはＥ
		}

		int suig = aptitude_coef();
		if ( rand()%2 == 0 && suig > 0 ) {
			suig--;
		}
		setBushouAptitude( iBushouID, APTITUDE_SUIGUN, suig ); // A,Sにはなりにくいように

		int yumi = aptitude_coef()-1;
		int yari = aptitude_coef()-1;

		// 弓はTSMod.txtの設定には関係なく設定する。
		// 弓の適性をコピー
		// 弓が無いというフラグがONなのであれば、0で上書きする。
		if ( TSModCommand::World::cmdTheYumiNoExists ) {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = 0;
		} else {
			nb6savedata_ex.bushou_ex[iBushouID].aptitudeyumi = yumi;
		}
		// 槍はTSMod.txtの設定には関係なく設定する。
		// 槍の適性をコピー
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeyari = yari;

		// 特殊属性は、一般武将は持ち合わせないので、全てクリア
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeono = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudemuchi = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudetuchi = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudetue = 0;
		nb6savedata_ex.bushou_ex[iBushouID].aptitudeken = 0;

		int job = rand() % 20;
		// 追加条件として、足軽Ａ以上で、政治の２倍以上戦闘がある
		if ( job <= 1 && 
			getBushouAptitude(iBushouID, APTITUDE_ASIGARU) >= 4 && 
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov*2 ) { // 足軽Ａ以上
			nb6bushouref[iBushouID].job = 0x10;	// 忍者
		} else if ( job == 2 ) {
			nb6bushouref[iBushouID].job = 0x20;	// 茶人

			// 追加条件として戦闘が一番高い
		} else if ( job <= 4 && 
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov &&
			nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxint ) {
			nb6bushouref[iBushouID].job = 0x30;	// 剣豪
		} else if ( job == 5 ) {
			nb6bushouref[iBushouID].job = 0x40;	// 僧侶

			// 水軍Ｂ以上でサイコロ７以下
		} else if ( job <= 7 && getBushouAptitude(iBushouID, APTITUDE_SUIGUN) >= 3 && nb6bushouref[iBushouID].maxbat >= 50 ) {
			nb6bushouref[iBushouID].job = 0x50;	// 海賊

		} else if ( job == 8 ) {
			nb6bushouref[iBushouID].job = 0x60;	// 高家
		} else {

			// ジョブはなし
			nb6bushouref[iBushouID].job = 0;	
		}

		nb6bushouref[iBushouID].biggun = 0;    //大砲
		nb6bushouref[iBushouID].horsegun = 0;  //騎馬鉄砲
		nb6bushouref[iBushouID].steelship = 0; //鉄甲船

		nb6bushouref[iBushouID].life = rand() % 8; //寿命
		nb6bushouref[iBushouID].brave = rand() % 8; //勇猛

		setBushouIndependence( iBushouID, rand() % 8 ); // 独立
		nb6bushouref[iBushouID].tone = rand() % 8;
		nb6bushouref[iBushouID].bEnd = 0;
		nb6bushouref[iBushouID].bRise = 0; //内応
		nb6bushouref[iBushouID].grudge = 0; //遺恨
		nb6bushouref[iBushouID].kaiken = 0; //会見

		int fnumber;
		 // 僧侶か茶人
		if ( nb6bushouref[iBushouID].job == 0x40 ||nb6bushouref[iBushouID].job == 0x20  ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::茶人僧侶, rand()%16, rand()%4, rand()%4 );

		// 剣豪
		} else if ( nb6bushouref[iBushouID].job == 0x30 ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType:: 剣豪, rand()%16, rand()%4, rand()%4 );

		// 忍者
		} else if ( nb6bushouref[iBushouID].job == 0x10 ) {
			fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType:: 忍者, rand()%16, rand()%3, rand()%4 ); // 忍者は目の４番目が仮装舞踏会みたいなのになってるので除外

		// 全部値が低い
		} else if ( nb6bushouref[iBushouID].maxgov < 50 &&
					nb6bushouref[iBushouID].maxbat < 50 &&
			        nb6bushouref[iBushouID].maxgov < 50 ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将若者, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将壮年, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将初老, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::知将老年, rand()%16, rand()%4, rand()%4 );
			}

		// 政治が一番高いならば
		} else if ( nb6bushouref[iBushouID].maxgov >= nb6bushouref[iBushouID].maxbat &&
			        nb6bushouref[iBushouID].maxgov >= nb6bushouref[iBushouID].maxint ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::知将若者, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::知将壮年, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::知将初老, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::知将老年, rand()%16, rand()%4, rand()%4 );
			}

		// 戦闘が一番高いならば
		} else if ( nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxgov &&
			        nb6bushouref[iBushouID].maxbat >= nb6bushouref[iBushouID].maxint ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::鎧兜若者, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::鎧兜壮年, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::鎧兜初老, rand()%16, rand()%4, rand()%4 );
			}

		// 智謀が一番高いならば
		} else if ( nb6bushouref[iBushouID].maxint >= nb6bushouref[iBushouID].maxgov &&
			        nb6bushouref[iBushouID].maxint >= nb6bushouref[iBushouID].maxbat ) { 

			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将若者, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将壮年, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将初老, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将老年, rand()%16, rand()%4, rand()%4 );
			}

		// それ以外(智謀が一番高いのと同じ)
		} else {
			if ( getYearsOld(iBushouID) <= 25 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将若者, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 35 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将壮年, rand()%16, rand()%4, rand()%4 );
			} else if ( getYearsOld(iBushouID) <= 45 ) {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将初老, rand()%16, rand()%4, rand()%4 );
			} else {
				fnumber = getFactoryGeneralKaoFace( GeneralFaceBaseType::凡将老年, rand()%16, rand()%4, rand()%4 );
			}
		}

		nb6bushouname[iBushouID].number = fnumber;

		// 名前を生成
		getFactoryGeneralName( nb6bushouname[iBushouID].familyname, nb6bushouname[iBushouID].fastname );

		nb6bushouname[iBushouID].position = 6; // 身分は足軽
		nb6bushouname[iBushouID].State = 4; // 状態は浪人
		nb6bushouname[iBushouID].sex = 0; // 男

		// 顔番号が指定されているならば…
		if ( iForthFaceID != -1 ) {
			// 顔を定める。血縁等々余計なことはしない。
			nb6bushouname[iBushouID].number = iForthFaceID;

		// 以下汎用顔生成タイプ(列伝なし)
		} else {

			// 最後に誰かの子供になる可能性がある。
			// 同じ城の人が自分より20歳以上年上なら、その人の子供になる可能性がある。
			for ( int iOyaBushouID=0; iOyaBushouID<GAMEDATASTRUCT_BUSHOU_NUM; iOyaBushouID++ ) {
				// 今生成した浪人と同じ城にいる
				if ( nb6bushouref[iOyaBushouID].castle == iCastleID+1 ) {
					 // 親は16歳以上、自分より年上
					if ( getYearsOld(iOyaBushouID) >= getYearsOld(iBushouID)+16 ) {

						// 親は最初からいる1300人の武将
						if ( nb6bushouname[iOyaBushouID].number < 1300 ) {
							// 親の顔番号をセット
							nb6bushouref[iBushouID].parent = nb6bushouname[iOyaBushouID].number;
							// 血縁を同じに
							nb6bushouref[iBushouID].blood =  nb6bushouref[iOyaBushouID].blood;

							if ( nb6bushouname[iOyaBushouID].familyname && strlen( nb6bushouname[iOyaBushouID].familyname ) > 1 ) {
								// 親の名前を貰う
								strcpy( nb6bushouname[iBushouID].familyname, nb6bushouname[iOyaBushouID].familyname );

								if ( nb6bushouname[iOyaBushouID].State <= 2 && !isPlayerBushou(iOyaBushouID) ) { // 親は大名・軍団長・現役で、プレイヤー武将以外
									// 浪人→現役へ
									setRouninToGeneki( iBushouID );
								}
								break;
							}

						// 列伝は無い人が親
						} else {
							// 血縁を同じに
							nb6bushouref[iBushouID].blood =  nb6bushouref[iOyaBushouID].blood;

							if ( nb6bushouname[iOyaBushouID].familyname && strlen( nb6bushouname[iOyaBushouID].familyname ) > 1 ) {
								// 親の名前を貰う
								strcpy( nb6bushouname[iBushouID].familyname, nb6bushouname[iOyaBushouID].familyname );
								if ( nb6bushouname[iOyaBushouID].State <= 2 && !isPlayerBushou(iOyaBushouID) ) { // 親は大名・軍団長・現役・隠居で、プレイヤー武将以外
									// 浪人→現役へ
									setRouninToGeneki( iBushouID );
								}
								break;
							}
						}
					}
				}
			}
		}

		// ScenarioModへの仮想武将誕生通知
		if (p_snOnBornGeneralBushou) {
			p_snOnBornGeneralBushou(iBushouID);
		}
	}
}


// TSMod.txtの設定によっては、死亡枠に追加で、汎用武将を誕生させる。
// 汎用武将等誕生処理。（たくさん誕生)
void GeneralBushouBornExecute() {

	// 仮想武将を登場させるフラグをONにしていないならば、何もしない。
	if ( !TSModCommand::World::cmdVirtualBushouAppear ) {
		return;
	}

	// シナリオ開始年と今の西暦が一緒なら、何もしない。
	if ( getYearAD() == getStartYearADOfScenarios() ) {
		return;
	}


	vector<int> vDeathArray;

	// 死亡者のカウントを数える。
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM-32; iBushouID++ ) { // 後ろの方32スロットには入れない。
		// 死亡者リスト生成
		if ( nb6bushouname[iBushouID].State == 7 ) { // 7 == 死亡
			if ( nb6bushouref[iBushouID].prev == 0xFFFF && nb6bushouref[iBushouID].next == 0xFFFF ) { // 7 == 死亡していて、リンクリストも切れてる人
				vDeathArray.push_back(iBushouID);
			}
		}
	}

	// 残り枠が10になるまで、死亡者枠に生存者を生成する。
	int iDeathRemainCnt = vDeathArray.size();

	// 死亡枠に武将をどんどん誕生させる。
	for ( vector<int>::iterator it=vDeathArray.begin(); it != vDeathArray.end(); it++, iDeathRemainCnt-- ) {
		if ( *it < GAMEDATASTRUCT_BUSHOU_NUM-32 ) { // 500未満のみ書き換える
			// 10人以上死亡者枠がのこっているならば…
			if ( iDeathRemainCnt >= 10 ) {
				// どこの城でもランダムで
				int iCastleID = rand() % GAMEDATASTRUCT_CASTLE_NUM;
				bornAGeneralBushou(*it, iCastleID);
			}
		}
	}
}




// 汎用武将等誕生処理。（1人誕生）。外部参照用
int WINAPI Extern_GeneralABushouBornExecute(int iForthFaceID) {

	int iTargetBushouIDSlot = -1;

	// 死亡者のカウントを数える。
	for (int iBushouID=0; iBushouID < GAMEDATASTRUCT_BUSHOU_NUM-32; iBushouID++ ) { // 後ろの方32スロットには入れない。
		// 死亡者リスト生成
		if ( nb6bushouname[iBushouID].State == 7 ) { // 7 == 死亡
			if ( nb6bushouref[iBushouID].prev == 0xFFFF && nb6bushouref[iBushouID].next == 0xFFFF ) { // 7 == 死亡していて、リンクリストも切れてる人
				iTargetBushouIDSlot = iBushouID;
				break;
			}
		}
	}

	// 枠が残ってなかったら失敗を返す
	if (iTargetBushouIDSlot == -1) {
		return -1;
	}

	if ( iTargetBushouIDSlot < GAMEDATASTRUCT_BUSHOU_NUM-32 ) { // 500未満のみ書き換える
		// どこの城でもランダムで
		int iCastleID = rand() % GAMEDATASTRUCT_CASTLE_NUM;
		// 武将誕生
		bornAGeneralBushou(iTargetBushouIDSlot, iCastleID, iForthFaceID);
		// スロットの番号を返す。
		return iTargetBushouIDSlot;
	}

	return -1;
}
