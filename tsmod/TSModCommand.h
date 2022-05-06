#pragma once

enum	{ TIMER_EVENT_ID_MENU_HIDE = 30 };

// TSMod.txtの特殊設定変更コマンド指定
namespace TSModCommand {

	extern int gurdian1[1000];

	// この世に… 
	namespace World {
		// 鉄砲が存在しなかったならば
		extern int cmdTheTeppouNoExists;
		// 弓が存在しなかったならば
		extern int cmdTheYumiNoExists;
		// カスタム家宝が存在するならば
		extern int cmdCustomKahouExists;
		// 仮想武将が登場するか
		extern int cmdVirtualBushouAppear;
	}

	namespace AllCastle {
		/// 徴兵可能数増加割合
		extern int cmdIncreaseRatioCanDraftSoldier;

		// ＣＰＵ戦での城防御力係数。
		extern int cmdCastleDefenceCoefOfNPC;

		// 落城ショートカット処理。
		extern int cmdShortCutFallCastle;
	}


	// 全ての勢力が… 
	namespace AllGundan {
		// 出陣時に出陣可能な部隊数は以下と定められる
		extern int cmdTheNumberOfTheMaxGoToWarFix;

		// 「講義」に必要な行動値
		extern int cmdNeedKoudouValueKougi;
		// 「披露」に必要な行動値
		extern int cmdNeedKoudouValueHirou;
		// 「編成」に必要な行動値
		extern int cmdNeedKoudouValueHensei;
		// 「徴兵」に必要な行動値
		extern int cmdNeedKoudouValueChohei;
		// 「登用」に必要な行動値
		extern int cmdNeedKoudouValueTouyou;
		// 「褒美」に必要な行動値
		extern int cmdNeedKoudouValueHoubi;
		// 「物資」に必要な行動値
		extern int cmdNeedKoudouValueBusshi;
		// 「戦争」に必要な行動値
		extern int cmdNeedKoudouValueSensou;
		// 「同盟」に必要な行動値
		extern int cmdNeedKoudouValueDoumei;
		// 「婚姻」に必要な行動値
		extern int cmdNeedKoudouValueKonin;
		// 「手切れ」に必要な行動値
		extern int cmdNeedKoudouValueTegire;
		// 「脅迫」に必要な行動値
		extern int cmdNeedKoudouValueKyohaku;
		// 「朝廷」に必要な行動値
		extern int cmdNeedKoudouValueChotei;
		// 「内応」に必要な行動値
		extern int cmdNeedKoudouValueNaiou;
		// 「謀略」に必要な行動値
		extern int cmdNeedKoudouValueBouryaku;
		// 「評定」に必要な行動値
		extern int cmdNeedKoudouValueHyoujo;
		// 「軍団」=>「任命」に必要な行動値
		extern int cmdNeedKoudouValueGundanNinmei;
		// 「軍団」=>分配」「合流」「再任」に必要な行動値
		extern int cmdNeedKoudouValueGundanSubCmd;
		// 「内政・訓練・昇進」に必要な行動値
		extern int cmdNeedKoudouValue5Ningumi;
	}

	// 自分の大名勢力が…
	namespace MyselfGundan {
		// 金と米を夏と冬にもらえる
		extern int cmdGetMoneyAndRiseOnSeasonChangeLv;	

		// 兵力が自動的に回復する
		extern int cmdRegenerationSoldierAutomatically;

		// 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
		extern int cmdIncreaseSpecialProductByTheThing;

	};

	// 他の大名勢力が…
	namespace OthersGundan {
		// 金と米を夏と冬にもらえる
		extern int cmdGetMoneyAndRiseOnSeasonChangeLv;

		// プレイヤの進捗と、プレイヤ以外の全体の軍団の進捗を見つつ、プレイヤ以外の軍団は互いに資源バランスを取ろうとします。
		extern int cmdPlayerAndTheWholeInfluenceEachOtherData;

		// 兵力が自動的に回復する
		extern int cmdRegenerationSoldierAutomatically;

		// 特産品に関連した項目が自動的に増える(鉄砲・馬・国際港)
		extern int cmdIncreaseSpecialProductByTheThing;
	};


	// 全ての大名が…
	namespace AllDaimyo {
		// 征夷大将軍
		extern int cmdSeiiTaishogunNeedCastleNum;

		// 誰かが統一してもエンディングを迎えない。
		extern int cmdTheSengokouPeriodIsEndless;
	};

	// 自分の大名が…
	namespace MyselfDaimyo {
		// 役職就任をもってゲームエンドとする。
		extern int cmdGameEndConditionIsGettingPosition;
		// 第２軍団以下に直接指示が可能。
		extern int cmdCanInstructDirectOtherGundan;
		// 評定は１季節に複数回可能
		extern int cmdCanMultiHyoujoInSeason;
	};

	// 他の大名が…
	namespace OtherDaimyo {
		// 他の勢力の大名は、脅迫コマンドを実行しない。
		extern int cmdNeverThreat;
	};

	namespace OtherBushou {

		// 戦闘中は全ての技能が使えたら
		extern int cmdCanUseAllAbilityInBattle;
	};

	// 全ての武将が…
	namespace AllBushou {

		// 忍者だったら最大兵士数が数分の一に制限される
		extern int cmdNinjaMustBeAShadow;	

		// 忍者は勲功が上がらない
		extern int cmdNinjaNotHaveAWayOfPromotion; 

		// 官位や役職があれば、それが兵士の最大数の増加に繋がる
		extern int cmdPositionInfluenceSoldierMax;	

		// 大名の兵数のベース
		extern int cmdSoldierBaseMaxOfDaimy;	
		// 宿老の兵数のベース
		extern int cmdSoldierBaseMaxOfSyuku;
		// 家老の兵数のベース
		extern int cmdSoldierBaseMaxOfKarou;
		// 部将の兵数のベース
		extern int cmdSoldierBaseMaxOfBusyo;
		// 侍大将の兵数のベース
		extern int cmdSoldierBaseMaxOfSamra;	
		// 足軽頭の兵数のベース
		extern int cmdSoldierBaseMaxOfAshig;	


		// 拡張された新職業(出自)を導入する。
		extern int cmdExtendedJob; 

		// 全ての武将は戦時に暗殺返しをすることがある。
		extern int cmdAntiAssassinationInBattle;

		// もしも老齢しても能力が減衰しない
		extern int cmdNoDeclineTalentParam;

		// もしもすべての武将の勇猛が特定の値であったらな…
		extern int cmdBraveIsTheValue;

		// もしも全ての武将の寿命が短かったなら…あるいは長かったなら…
		extern int cmdLifeIsTheValue;

		// もしも全ての武将の歳を取るのが遅くなったなら…
		extern int cmdLifeIsTheTimes;

		// もしも全ての武将に義理などなく独立が常であったなら…
		extern int cmdNoDutiesAndMaxIndependences;

	    // もしも技能が未開発であったなら…
		extern int cmdAbilityIsUnderdeveloped;

		// もしも政熟・戦熟・智熟がＭＡＸであったなら…
		extern int cmdFixExperienceMax;
		
		// 熟練度や技能を自動で習得してゆく。
		extern int cmdAutoUpExperienceAndTech;

		// 急には成長しない。
		extern int cmdNotGrowUpSuddenly;

		// 上長との相性の良し悪しが、毎季節、忠誠に影響する
		extern int cmdAffinityWithSuperiorInfluenceLoyalty;

		// (死亡頻度にかかわらず)毎季死亡処理をする。
		extern int cmdSetWorkToDieEverySeason;

		// ＣＰＵ登場姫も姫武将にする。
		extern int cmdAppearanceHimeBecomeBushou;

		// 顔なし武将が旗揚げするパッチ。
		extern int cmdNonFaceBushouIndependences;

		// 浪人になった時、勲功がある程残る
		extern int cmdNonMeritoriousReset;

		// 戦争をみていない時の戦闘熟練・智謀熟練の過剰な上昇を抑える。
		extern int cmdSuppressRiseOfExpValueInBattle;

		// 門を壊す際にダメージを受けるかどうか。
		extern int cmdReceiveDamageOfBreakDoor;

		// 死亡頻度の寿命への影響力をカットする。
		extern int cmdDeathFrequencyNotInfluenceLife;

		// その能力は歴代シリーズを考慮したものである。
		extern int cmdParamConsiderNobunagaSeries;

		// 武将解放時に家臣の忠誠度下降
		extern int cmdDegreeLoyaltyOnCaptiveLibra;
	};


	namespace UltraBushou {

		// ウルトラ武将がいる
		extern int cmdExists;

		// 突撃による相手の戦死有り(デフォルト有り)
		extern int cmdTotsugekiTargetDeath;
	};


	namespace Environment {
		// タイトルバーとメニューバーは非表示
		extern int cmdNoTitleBarAndMenuBar;

		// 解像度を大きくするモード
		extern int cmdWideResolution;

		// 解像度をモバイル用にする
		extern int cmdMobileResolution;

		// ＢＧＭのモード
		extern int cmdBGMMode;

		// インターネットへの接続の許可
		extern int cmdInternetConnectPermission;

		// デモモードではスクリーンセイバーの発行を防止する
		extern int cmdNoScreenSaverInDemo;

	};

	namespace Debug {

		// AllocConsole型(全員が使える方の)デバッグウィンドウをオープンにする。
		extern int cmdDebugWindowOpen;
	};


	// コマンドをセットする
	void setCommandSwitch(char *szCurCmdTokenBuf, int byteCurCmdLv);

	extern int gurdian2[1000];

};


