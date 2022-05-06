#pragma once

/*
【武将のアルゴリズム】:
0…名将と呼ばれた人物。有名武将に多い。信長秀吉家康ほか、斎藤道三や松永久秀も。
総合的なバランスの良いタイプだが、comの場合兵力ばかり増やし、あまり戦争しない。
1…猛将に多い。柴田勝家や柿崎景家や南部晴政ら。
2…戦闘に強い武将が多い。剣豪など。
3…戦闘嫌い。今川氏真ら。政治好き？
4…板部岡江雪等、坊主・茶人等の文化人系武将。政治好き？
5…忍者武将や、久武親直・金地院崇伝など。調略好き。
6…目立たないザコ武将。無能タイプ？
7…オリジナル姫。性格は不明。
255…ランダム姫。性格は不明。
*/

/*
【口調】:
0 無口			 217 真田幸村・長宗我部元親・島津義久・忍者全員
1 礼儀正しい	 152 前田利家・明智光秀・立花宗茂・徳川光圀
2 粗暴			 119 柴田勝家・龍造寺隆信・可児才蔵・佐々木小次郎
3 高貴			 30 上杉憲政・今川義元・足利義晴・一条兼定
4 生意気		 180 伊達政宗・三好長慶・鈴木重秀・由比正雪
5 威厳			 143 織田信長・徳川家康・武田信玄・上杉謙信
6 饒舌			 145 豊臣秀吉・斎藤道三・松永久秀・足利義昭
7 一般			 314 本多忠勝・吉川元春・山中鹿之助・史実姫全員
*/

/*
[武将データ(47*532)]

0-1	前の武将
2-3 後ろの武将
4-5	？
6	政才
7	戦才
8	智才
9-10	政熟   ( min 500 ～ max 2000 )
11-12	戦熟   ( min 500 ～ max 2000 )
13-14	智熟   ( min 500 ～ max 2000 )
15	魅力
16	野望
17	忠誠
18-19	勲功
20	血筋 0xFFは無縁
21-22	大名(大名番号)  所属大名【大名番号】;	//大名
23-24	軍団(軍団番号)  所属軍団【軍団番号】;	//軍団
25-26	居城		    所属居城【城番号】;	//居城
27-28	内応先 FFFF 無し
29	兵数
30	訓練
31	兵忠
32	兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)
33	仕官
34	妻 他に結婚済みフラグあり？
35	生年
36-37	親顔NO
38	思考
39	aaaabbbb a-相性 b-義理
40	00aabbcc a-智適 b-戦適 c-政適
41	技能
42	職業 0x00無 0x10 忍者 0x20 茶人 0x30 剣豪 0x40 僧侶 0x50 海賊 0x60 高家 0x70 商人 0x80 庶民 0x90 切支丹 0xA0 騎士 0xB0 魔術士 0xC0 魔族
43-44	兵科・騎馬鉄砲・大砲・鉄甲船
0123 4445 5566 6777
0=無関係？,1=鉄甲船,2=騎馬鉄砲,3=大砲
4,5,6,7=水軍,鉄砲,騎馬,足軽の兵科、MAX5
45	aabbbccc a-独立 b-勇猛 c-寿命
46	1234aaa5 1=会見 2-遺恨 3 内応 4-行動 5独立のビットの続き
a-口調
*/

struct NB6BUSHOU
{
	WORD prev;
	WORD next;
	WORD _undef2;		//must 0xFFFF
	byte maxgov;		//政才
	byte maxbat;		//戦才
	byte maxint;		//智才
	WORD expgov;		//政熟
	WORD expbat;		//戦熟
	WORD expint;		//智熟
	char charm;			//魅力
	char ambition;		//野望
	char loyal;			//忠誠
	WORD merits;		//勲功
	byte blood;			//血筋 0xFFは無縁
	WORD attach;		//大名  所属大名【大名番号】;	//大名
	WORD gundan;		//軍団  所属軍団【軍団番号】;	//軍団
	WORD castle;		//居城  所属居城【城番号】;	//居城
	WORD rise;			//内応先 0xFFFF 内応先の大名番号
	byte soldier;		//兵数
	byte training;		//訓練
	byte solloyal;		//兵忠
	byte form;			//兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)
	byte work;			//仕官年数
	byte spouse;		//妻 (武将配列番号で500番以上が妻や姫となる条件である。相手が500番の時に、ここに0という数値が入る。0⇒0+500⇒帰蝶 といった具合。
	//妻 この番号が255なら妻はいない。255以外の時(例えば3)は、その番号に500足した【武将番号】(例えば503)の女性が妻ということ)
	// そして該当の武将(妻姫武将)の_妻も同じ(例えば3)の値になっていること。
	byte birth;			//生年 - 1454
	WORD parent;		//親顔NO
	byte algo;			//思考
	byte duty : 4;		//義理
	byte affinity : 4;	//相性
	//00aa bbcc a-智適 b-戦適 c-政適
	byte aptitudegov : 2;	//c-政適 (2=A,1=B,0=C)
	byte aptitudebat : 2;	//b-戦適 (2=A,1=B,0=C)
	byte aptitudeint : 2;	//a-智適 (2=A,1=B,0=C)
	byte undef_5 : 2;		//
	byte tech_ikkatsu : 1;	//技能・一喝
	byte tech_ansatsu : 1;	//技能・暗殺
	byte tech_ryusyutsu : 1;	//技能・流出
	byte tech_sendou : 1;		//技能・煽動
	byte tech_ryugen : 1;		//技能・流言
	byte tech_benzetsu : 1;	//技能・弁舌
	byte tech_yakiuchi : 1;	//技能・焼討
	byte tech_chohatsu : 1;	//技能・挑発
	byte job;			//職業 0x00 無 0x10 忍者 0x20 茶人 0x30 剣豪 0x40 僧侶 0x50 海賊 0x60 高家 0x70 商人 0x80 庶民 0x90 切支丹 0xA0 騎士 0xB0 魔術士 0xC0 魔族

	byte aptitudeasi : 3;		//足軽適正
	byte aptitudekib : 3;		//騎馬適正
	byte _aptitudegun_hi : 2; //鉄砲適正上位ビット
	byte _aptitudegun_lo : 1; //鉄砲適正下位ビット
	byte aptitudesui : 3;		//水軍適正

	byte biggun : 1;		//大砲
	byte horsegun : 1;	//騎馬鉄砲
	byte steelship : 1;	//鉄甲船
	byte _undef : 1;		//0
	byte life : 3;		//寿命
	byte brave : 3;		//勇猛
	byte _independence_hi : 2; //独立上位ビット
	byte _independence_lo : 1; //独立下位ビット
	byte tone : 3;		//口調
	byte bEnd : 1;		//行動
	byte bRise : 1;		//内応
	byte grudge : 1;		//遺恨
	byte kaiken : 1;		//会見
};


struct NB6BUSHOUCOPY
{
	byte soldier;		//兵数
};


// 武将の年齢を得る
int getYearsOld(int iBushouID);

/// <summary>
/// 対象の武将の年齢を設定する。
/// 武将年齢は何歳でも自由に設定出来るわけではなく、
/// 計算結果として、「1455年生まれより早く生まれたこと」になる年齢は設定できない
/// 計算結果として、「1708年生まれより遅く生まれたこと」になる年齢も設定できない
/// <para>返り値：年齢設定に成功すれば真・失敗すれば偽</para>
/// </summary>
/// <param name="武将番号【配列用】">対象となる武将の「配列用」の番号</param>
/// <param name="年齢">新たな年齢</param>
/// <returns>返り値：年齢設定に成功すれば真・失敗すれば偽</returns>
bool setYearsOld(int iBushouID, int iYearsOld);

enum APTITUDE { APTITUDE_ASIGARU = 1, APTITUDE_KIBA = 2, APTITUDE_TEPPOU = 3, APTITUDE_SUIGUN = 4, APTITUDE_YUMI = 5, APTITUDE_YARI = 6 };

// 足軽・騎馬・鉄砲・水軍の適正を数値で得る。(0=E,1=D,2=C,3=B,4=A,5=S)
int getBushouAptitude(int iBushouID, APTITUDE eAptitude);

// 足軽・騎馬・鉄砲・水軍の適正を数値で得る。(0=E,1=D,2=C,3=B,4=A,5=S)
void setBushouAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// 弓の適性値を設定する(ScenarioMod等からの設定) (0=E,1=D,2=C,3=B,4=A,5=S) 値の取得も兼ねる。
int WINAPI Extern_setYumiAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);

// 槍の適性値を設定する(ScenarioMod等からの設定) (0=E,1=D,2=C,3=B,4=A,5=S) 値の取得も兼ねる。
int WINAPI Extern_setYariAptitude(int iBushouID, APTITUDE eAptitude, int iTekisei);


// 武将の戦闘値
int getBushouBat(int iBushouID);
// 武将の智謀値
int getBushouInt(int iBushouID);
// 武将の政治値
int getBushouGov(int iBushouID);

// 家宝などを含めない素の政治才能
int getBushouPureGovMax(int iBushouID);

// 家宝などを含めない素の戦闘才能
int getBushouPureBatMax(int iBushouID);

// 家宝などを含めない素の智謀才能
int getBushouPureIntMax(int iBushouID);

// 家宝などを含めない素の野望
int getBushouPureAmbition(int iBushouID);

// 官位などを含めない素の魅力
int getBushouPureCharm(int iBushouID);

// 家宝による政治の上げ幅
int getBushouGovMaxUpByKahou(int iBushouID);

// 家宝による戦闘の上げ幅
int getBushouBatMaxUpByKahou(int iBushouID);

// 家宝による智謀の上げ幅
int getBushouIntMaxUpByKahou(int iBushouID);

// 家宝による野望の上げ幅
int getBushouAmbitionUpByKahou(int iBushouID);

// 官位による魅力の上げ幅
int getBushouCharmUpByKani(int iBushouID);


// 武将の独立値を得る
int getBushouIndependence(int iBushouID);

// 武将の独立値を設定する
void setBushouIndependence(int iBushouID, int iIndependence);


// 顔IDから武将IDを求める。(配列用ではなくて+1された値が返ってくる)
int getBushouIDFromFaceID(int iFaceID);

// 対象の武将IDが現在鉄甲船状態かどうか(鉄甲船を持っているかどうかではなく、戦場において、今鉄甲船の形状かどうか)
extern int iCurTekkousenForm[GAMEDATASTRUCT_BUSHOU_NUM];

// bfile.nb6に存在する列伝の数
extern int iExistRetudenNumOfBFileN6P;



struct NB6KAIKENIKOU {
	byte _undef1; // 基本0xFF
	byte _undef2; // 基本0
	byte _undef3; // 基本0
	byte ai; // 基本0x24が多い(0x24は勢力ＡＩを継承する？)。
	// 0:静観 1:開発 2:商業 3:築城 4:施し 5:戦争 6:徴兵 7:訓練 8:編成兵数 9:騎馬編成 10:鉄砲編成
	byte depth; // 大抵0, 8。 たまに、9, A
	byte _undef4; // 基本0
	byte _undef5; // 基本0
	byte _undef6; // 基本0
};

// 対象武将の会見意向のAIを得る
// 0:静観 1:開発 2:商業 3:築城 4:施し 5:戦争 6:徴兵 7:訓練 8:編成兵数 9:騎馬編成 10:鉄砲編成
// 11:前線移動,12:後方移動,13:米購入,14:鉄砲購入,15:騎馬購入,16:家宝購入,17:米売却,18:講義師範
// 19:講義習得,20:登用,21:昇進,22:褒美,23:同盟締結,24:共同戦線:25:友好親善,26:手切れ,27:脅迫
// 28:朝廷工作,29:内応,30住民流出,31:一揆扇動,32:流言飛語,33:武将暗殺,34:焼き討ち,35:合戦誘発
extern int getKaikenIkouAI(int iBushouID);

// 対象武将の会見意向ＡＩを設定する。
extern void setKaikenIkouAI(int iBushouID, int iAI);


/*
武将名前テーブル(18*500と姫等用に18*32)

[18バイト]
0-6		名字
7-13	名前
14-15	武将ナンバー(下位12bitは列伝？)
16	上位4ビットは身分(b0000=隠居,b0001=大名
　b0010=宿老,b0011=家老
 　b0100=部将,b0101=侍大将
  　b0110=足軽頭)
   下位4ビット	0大名,1軍長,2現役,3隠居,4浪人,5姫,6?,7死亡

   17	性 0-男 1-女
   */
struct NB6BUSHOUNAME
{
	char familyname[7];	//名字
	char fastname[7];	//名前
	WORD number;		//武将ナンバー(下位12bitは列伝？)
	byte State : 4;		//0大名,1軍長,2現役,3隠居,4浪人,5姫,6,待機,7死亡
	byte position : 4;	//身分		(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
	byte sex : 1;			//性 0-男 1-女
	byte battleRetreat : 1;	//1:戦場で退却した。0:通常通り
	byte battleDeath : 1;	//1:戦場で戦死した。0:通常通り
	byte _undef : 5;
};


// State 現役 → State 浪人へ
bool setGenekiToRounin(int iBushouID);

// State 浪人 → State 現役へ
bool setRouninToGeneki(int iBushouID);

// 現役をその城の城主とする。
// 特定の「現役」の武将を、その城の城主にする。
// 元の城主が「軍団長」や「大名」だと、その人物の立場となる。
bool setGenekiToJyosyu(int iBushouID);

// 武将を指定城に移動する。
bool setBushouTransport(int iBushouID, int iNewCastleID);

// 指定武将の伴侶のiBushouIDを得る。伴侶が居ない場合には、0xFFFFが返ってくる。
int getHanryoBushouID(int iBushouID);

// 姫(他の武将の妻含む)を、指定の武将と結婚させる、同盟関係等は一切操作されない。
// withKodokuDeathは、誰かが離婚して天蓋孤独になったら死亡処理を行うこと
bool setHimeMarriage(int iHimeID, int iOttoID, BOOL withKodokuDeath = true);

// 指定の姫を離婚させる。
// withKodokuDeathは、誰かが離婚して天蓋孤独になったら死亡処理を行うこと
bool setHimeDivorce(int iHimeID, BOOL withKodokuDeath = true);



// 2人の武将の相性の差を得る
int getBushouAishoDiff(int iBushouAID, int iBushouBID);


/*
* 噴出しなどにおける、｢主体・客体・第３者」のID
*/
struct NB6FUKIDASHIRELATEPERSONS {
	WORD i1stPersonID;	// 自分のID(通常武将ID)
	WORD i2ndPersonID;	// 相手のID(通常武将ID。イベント顔や商人顔は531,農民顔は532となる)
	WORD i3rdPersonID;	// 第３者のID(通常1stPersonと2ndPersonの会話の中で出てくる人のID);
	DWORD _undef1;
	DWORD _undef2;
	DWORD _undef3;
	DWORD _undef4;
	DWORD _undef5;
};


/*
武将列伝関連の情報
*/
struct NB6BUSHOURETSUDEN {
	string name;
	string yomigana;	// よみがな
	string born;	    // 誕生年文字列(????)とかもあるため
	string death;	    // 死亡年文字列(????)とかもあるため
	string retsuden;	// 列伝文字列
};




/*
最大兵数 [0:隠居,1:大名,2:宿老～6:足軽]までとして格納してゆく
*/
struct NB6SOLDIERMAX
{
	WORD soldiermax;
};

/*
最大兵数 [0:隠居,1:大名,2:宿老～6:足軽]までとして格納してゆく。官位や身分などをふまえた変形パラメタ
*/
struct NB6SOLDIERMAX_CUSTOM
{
	WORD soldiermax;
};


/*
身分昇進のために必要となる勲功値
*/
struct NB6REQUIREDMERITS
{
	DWORD merits;
};

