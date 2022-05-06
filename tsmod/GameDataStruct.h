#pragma once
#pragma warning ( 3 : 4706 )

#pragma pack(1)
/*
	構造体で埋め込みが必要な場合は、コンパイラ ディレクティブ #pragma pack を使用することができます。
	ただし、#pragma pack を使用した場合、構造体の要素は整列されません。
	(構造体とかで勝手に指定のビットとはことなるようにパッキングして整列しなおすようだ。
	これを#pragma pack(1)とすることで、1bitずつの整列となるため、事実上、整列されなくなるようだ。
*/


#include "StrFormatMatch.h"
#include "ScenarioDataStruct.h"
#include "SndataN6PDataStruct.h"
#include "MapData.h"
#include "OutputDebugString.h"
#include "OutputDebugStream.h"
#include "OnigRegex.h"
#include "ToCopyClipboard.h"
#include "Menu.h"
#include "OnChangeWeather.h"
#include "3DPosition.h"
#include "2DPosition.h"
#include "FuncGetGunshiBushouID.h"
#include "Todouhuken.h"
#include "FactoryGeneralKaoFace.h"
#include "HexmapNB6.h"




#define GAMEDATASTRUCT_SAVE_OR_LOAD_SCENARIO_ID_ADDRESS  0x4CEFA0 // ロードもしくはセーブした時に、このアドレスにシナリオ番号が入る。シナリオ選択時には入らない。

#define GAMEDATASTRUCT_SCENARIO_NAME_POINTER_ARRAY_ADDRESS		0x4D3728 // ６つのシナリオ名へのポインタ配列の先頭アドレス

#define GAMEDATASTRUCT_REGISTED_BUSHOU_NUM		1300		// [登録済み武将数]
#define GAMEDATASTRUCT_RETSUDEN_NUM				1332		// [列伝の個数]


#define GAMEDATASTRUCT_YEAR_ADDRESSS			0x4CF210	// [年・季節]
#define GAMEDATASTRUCT_START_YEAR_ADDRESS		0x466A8A	// 天翔記にとっての元年(0年）(西暦1454)

#define GAMEDATASTRUCT_TURN_ADDRESSS			0x4C9355	// [ターン情報]

#define GAMEDATASTRUCT_SOBA_ADDRESSS			0x4C93FD	// [相場情報]

#define GAMEDATASTRUCT_PLAYERS8_ADDRESSS		0x4C93ED	// [プレイヤー担当大名の担当の軍団の番号]
#define GAMEDATASTRUCT_PLAYERS8_NUM				8			// [上記の最大数（＝事実上プレイヤー最大数）＝８]

#define GAMEDATASTRUCT_TOUZYOU_BUSHOU_NUM_ADDRESS       0x4CFC37    // 待機から登場した数
#define GAMEDATASTRUCT_MITOUZYOU_BUSHOU_NUM_ADDRESS     0x4CFC39    // 未登場武将の総数数

#define GAMEDATASTRUCT_BUSHOU_ADDRESSS			0x4BFD18	// [武将データ(47*532)]
#define GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS		0x4C5FA8	// [武将名前テーブル(18*500と姫等用に18*32)]
#define	GAMEDATASTRUCT_BUSHOU_NUM				532

#define GAMEDATASTRUCT_KAMON_REGULAR_NUM		16 *  7			// 正規家紋の数。ゲーム内でユーザー編集以外で出てくることに妥当性がある数
#define GAMEDATASTRUCT_KAMON_ALL_NUM			16 * 10			// 家紋の数。最後の16個はユーザーが編集したもの

#define GAMEDATASTRUCT_CASTLE_ADDRESSS			0x4BC918	// [城情報(33*214)]
#define	GAMEDATASTRUCT_CASTLE_NUM				214

#define GAMEDATASTRUCT_CASTLEMAX_ADDRESSS		0x4BE778	// [城の位置情報など(8*214)]

#define	GAMEDATASTRUCT_CASTLETYPE_NUM			7			// [城のタイプ(御所とか館とか)]の数

#define GAMEDATASTRUCT_CASTLE_HEX_ROLE_ADDRESS	0x4D5182	// 城内の役割の概念のデータのアドレス(18*16バイト)
#define GAMEDATASTRUCT_CASTLE_HEX_HIGH_ADDRESS	0x4D555A	// 城内の高さの概念のデータのアドレス(18*16バイト)


#define GAMEDATASTRUCT_FIELD_HEX_ROLE_ADDRESS	0x4D5182	// 野戦の役割の概念のデータのアドレス(41*24バイト)
#define GAMEDATASTRUCT_FIELD_HEX_UNIT_POS_ADDRESS	0x4D5932	// 野戦でユニットがいる位置がポツリポツリ。現在表示中のHEXマップでほとんどがFF、ユニットいるところは、ユニット番号が入っている。
#define GAMEDATASTRUCT_CASTLE_HEX_UNIT_POS_ADDRESS	0x4D5932	// 攻城戦でユニットがいる位置がポツリポツリ。現在表示中のHEXマップでほとんどがFF、ユニットいるところは、ユニット番号が入っている。
																// 野戦と共通のアドレス。
#define GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_ADDRESS	0x4D5D58	// HEXマップにおける各ユニットの情報(1人32bit) x 1城から出てくる８人 × １野戦あたり９城で合計 72人x32bit
#define GAMEDATASTRUCT_WAR_HEX_UNIT_INFO_NUM	72			// HEXマップにおけるユニットの最大数。1城から出てくる８人 × １野戦あたり９城

#define GAMEDATASTRUCT_CASTLE_HEX_FIRE_ROLE_ADDRESS	0x4D4DAA	// 城内の火の手関連の概念のデータのアドレス(18*16バイト)

#define GAMEDATASTRUCT_FIELD_HEX_COLS			41	// 野戦のHEXの横の数
#define GAMEDATASTRUCT_FIELD_HEX_ROWS			24	// 野戦のHEXの縦の数

#define GAMEDATASTRUCT_CASTLE_HEX_COLS			18	// 攻城戦のHEXの横の数
#define GAMEDATASTRUCT_CASTLE_HEX_ROWS			16	// 攻城戦のHEXの縦の数


#define GAMEDATASTRUCT_WAR_HEX_TURN_ADDRESS		0x4D5D48	// HEXマップにおいて、現在どの軍団の順番となっているのか。各順番は何かで割り当てられている。その順番で攻めるというわけはなく、各番号に軍団が割り当てられる模様。

#define GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_ADDRESS	0x4D4D48	//  ヘックスマップにおいて、この戦争に関わっている「大名・軍団・総大将・[攻め手・受け手・中立]」情報
#define GAMEDATASTRUCT_WAR_JOINT_DAIMYO_HEX_GROUP_INFO_NUM		9			//  戦争に関係する大名は最大９大名まで。

#define	GAMEDATASTRUCT_GUNDAN_ADDRESS			0x4C9640	// [軍団情報]
#define	GAMEDATASTRUCT_GUNDAN_NUM				214


#define	GAMEDATASTRUCT_DAIMYO_ADDRESS			0x4CE7B8	// [大名情報] 
#define	GAMEDATASTRUCT_DAIMYO_NUM				72

#define GAMEDATASTRUCT_DOUMEI_ADDRESS			0x4CF213	// [同盟情報] (A大名-B大名間の同盟値)

#define	GAMEDATASTRUCT_CHIMEI_ADDRESS			0x4BF278	// [地名情報]
#define	GAMEDATASTRUCT_CHIMEI_NUM				74


#define GAMEDATASTRUCT_SOLDIERMAX_ADDRESS		0x4BB7A0	// [最大兵数]
#define GAMEDATASTRUCT_SOLDIERMAX_NUM			7

#define GAMEDATASTRUCT_PARAMMAX_ADDRESS			0x4BB6E0	// [各種パラメタの最大値。武将系]
#define GAMEDATASTRUCT_PARAMMAX_MAIN_ADDRESS	0x4CD8A0	// [各種パラメタの最大値。メイン系]

#define	GAMEDATASTRUCT_KANI_ADDRESS				0x4CC2F0	// [官位情報]
#define	GAMEDATASTRUCT_KANI_NUM					250
#define	GAMEDATASTRUCT_KANITYPE_NUM				23			// [官位種類の数] 正一位～従八位下 まで

#define	GAMEDATASTRUCT_POSITIONTYPELIST_POINTER_ADDRESS	0x4D1554	// [役職種類]のそれぞれの文字列へのポインタのリストの先頭アドレス
#define	GAMEDATASTRUCT_POSITIONTYPELIST_ADDRESS	0x4CFC3B			// [役職種類]
#define	GAMEDATASTRUCT_POSITIONTYPE_NUM			8

#define GAMEDATASTRUCT_KAHOU_ADDRESSS			0x4D2108	// [家宝情報]
#define	GAMEDATASTRUCT_KAHOU_NUM				120

#define GAMEDATASTRUCT_KAHOUTYPELIST_POINTER_ADDRESS	0x4D1420	// [家宝種類]のそれぞれの文字列へのポインタのリストの先頭アドレス
#define	GAMEDATASTRUCT_KAHOUTYPELIST_ADDRESS	0x4CFDCC			// [家宝種類]
#define	GAMEDATASTRUCT_KAHOUTYPE_NUM			24


#define GAMEDATASTRUCT_SAVEDATAFILENAME_ADDRESS 0x4C8D72    // [セーブデータのファイル名のアドレス]

#define GAMEDATASTRUCT_REQUIREDMERITS_ADDRESS   0x4D1950    // [必要勲功値のアドレス]
#define GAMEDATASTRUCT_REQUIREDMERITS_NUM		5			// [宿老・家老・武将・侍大将・足軽頭]の５種類が定義されている。

#define GAMEDATASTRUCT_MONEY_MAX				60000		// お金のMAX
#define GAMEDATASTRUCT_RISE_MAX					60000		// お米のMAX
#define GAMEDATASTRUCT_GUN_MAX					60000		// 鉄砲のMAX
#define GAMEDATASTRUCT_KIBA_MAX					60000		// 騎馬のMAX


#define GAMEDATASTRUCT_AISHOU_NUM				16			// 相性の個数 0～15


#define GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INBATTLE	0x4BB4D2	// 戦争中何ターン経過したか
#define GAMEDATASTRUCT_HOWMUCHTURNPROCEED_INCASTLEBATTLE	0x4D5D44	// 現在表示中の攻城戦は何ターン目か（０～２）



#define GAMEDATASTRUCT_WEATHER_ADDRESSS			0x4BB4D6	// 天気のアドレス

#define GAMEDATASTRUCT_EVENT_GROUP1_ADDRESSS		0x4CFC33	// イベントアドレスその１ b00000001 鉄砲伝来(=1) b00000010(=2 キリスト教伝来、他いろいろこのフラグで管理されているっぽい。
#define GAMEDATASTRUCT_EVENT_GROUP2_ADDRESSS		0x4CFC34	// イベントアドレスその２ b01000000 大坂城築城(=1)

#define GAMEDATASTRUCT_SETTINGS_ADDRESSS		0x4C9351	// 環境設定のアドレス

#define GAMEDATASTRUCT_SETTINGSEX_ADDRESSS		0x4DEE00	// 環境設定拡張(切替)のアドレス


#define GAMEDATASTRUCT_SELECTING_BUSHOU_LIST	0x4DBD40	// 武将選択等の時に、武将リストの武将IDが入っている。0xFFFFまでが対象。


#define GAMEDATASTRUCT_FUKIDASHI_RELATE_PERSONS	0x4CD770	// 主体・相手・会話中に出てくる関連している人物。

#define GAMEDATASTRUCT_VARIABLE_VALUE_GROUP1_DIALOG	0x4DDF38	// 例の「人」や「数値」の入れ物の入力結果表示情報枠。第１グループ
#define GAMEDATASTRUCT_VARIABLE_VALUE_GROUP2_DIALOG	0x4DDF58	// 昇進時の「人」や披露時の入力結果表示情報枠。第２グループ


#define GAMEDATASTRUCT_PLAYING_BGM_ID_ADDRESS	0x4D2A7C	// 再生中のBGMのIDのアドレス(WORD)

#define GAMEDATASTRUCT_WINMM_DLL_NAME_ADDRESS	0x4E7CC0	// WINMM.DLLか_INMM.DLLなのか。


#define GAMEDATASTRUCT_BUSHOU_RETSUDEN_NUM_PATCH_ADDRESS	0x40CB5A	// TENSHOU.EXEの列伝人数制限判定部分を1932など、変更すべき場所。ニーモニックの中なので注意	

#define GAMEDATASTRUCT_FONTNAME_ADDRESS			0x4BC884    // フォント名が格納されているアドレス。

#define GAMEDATASTRUCT_MOVIE_NUM				36			// ＣＤ－ＲＯＭにあるムービーの個数。
#define GAMEDATASTRUCT_MOVIELISTPOINTER_ADDRESS				0x4C9588	// TENSHOU.EXEのkoeilogo等の各ムービーのポインタが配列となって格納されている最初の場所。

#define GAMEDATASTRUCT_MAINMAP_DATA_ADDRESS		0x4CF212	// メインマップのパレット＋メインマップのデータの格納場所

#define GAMEDATASTRUCT_TENSHOUKI_DRIVENAME_ADDRESS	0x4C9580 // 天翔記のドライブ名の文字列が入ってるアドレス。
#define GAMEDATASTRUCT_TENSHOUKI_DISKLABEL_ADDRESS	0x4CC278 // 天翔記のＣＤディスクのラベルの文字列が入ってるアドレス。

#define GAMEDATASTRUCT_HIMENAME_ARRAY			0x4CA9B0	// 姫名が羅列されている場所。1人7バイト。(全角3文字＋NULL)で100人分。

#define GAMEDATASTRUCT_SYOUNIN_NUM				10			// ゲーム中の商人。10人。
#define GAMEDATASTRUCT_GOYOUSYOUNINNAME_ARRAY	0x4D1490	// 商人名へのポインタが羅列してある場所。

#define GAMEDATASTRUCT_RESOLUTION_ADDRESS		0x4DFD40	// ここからintで連続して横・縦の情報が格納されている。

#define GAMEDATASTRUCT_FUKIDASHI_DIALOG_LAYOUT_ADDRESS	0x4DA940	// ダイアログ系レイアウトの「左上」と「右下」の基点が掲載されている。


#include "SaveDataStructEx.h"


#include "GameDataStructMainTurn.h"

#include "GameDataStructBushou.h"

#include "GameDataStructTaikiBushou.h"

#include "GameDataStructBattle.h"

#include "GameDataStructCastle.h"

#include "GameDataStructChimei.h"

#include "GameDataStructGundan.h"

#include "GameDataStructDaimyo.h"

#include "GameDataStructYakusyoku.h"

#include "GameDataStructKani.h"

#include "GameDataStructKahou.h"

#include "GameDataStructMaxmum.h"


#include "GameDataStructMainCommand.h"


#include "GameDataStructSettings.h"

#include "GameDataStructUI.h"

// externすんのメンドクサイのでここでまとめてしておく
// どうしてもTSMod.dllの実装が膨らみ重くなってしまった場合は、
// ｢変数へのポインタ｣を｢評価した｣ものへの｢参照型｣にして、extern させれば、記述を吸収しつつ爆速に出来る。
// nb6bushouについてはメモリをロックしている瞬間にTSMod.dllが書き込もうとしてもふっとばないように全面的に記述されている。(その代わり、まわりくどいし重い)
extern NB6YEAR			*const _nb6year;
#define							nb6year		_nb6year[0]
extern NB6TURN			*const _nb6turn;
#define							nb6turn		_nb6turn[0]
extern NB6SOBA			*const _nb6soba;
#define							nb6soba		_nb6soba[0]
extern NB6PLAYERS8		*const nb6players8;
extern NB6BUSHOUCOPY	nb6bushoucopy[GAMEDATASTRUCT_BUSHOU_NUM];
extern NB6BUSHOU		*const nb6bushouref;	// 参照専用
extern NB6BUSHOUNAME	*const nb6bushouname;
extern NB6BUSHOURETSUDEN   nb6bushouretsuden[GAMEDATASTRUCT_RETSUDEN_NUM];
extern NB6KAHOURETSUDEN    nb6kahouretsuden[GAMEDATASTRUCT_KAHOU_NUM];
extern NB6CASTLE		*const nb6castle;
extern NB6CASTLEMAX		*const nb6castlemax;

extern NB6GUNDAN		*const nb6gundan;
extern NB6DAIMYO		*const nb6daimyo;
extern NB6CHIMEI		*const nb6chimei;
extern NB6KANI			*const nb6kani;
extern NB6KAHOU			*const nb6kahou;

extern NB6SOLDIERMAX	nb6soldiermax[GAMEDATASTRUCT_SOLDIERMAX_NUM];
extern NB6PARAMMAX		*const _nb6parammax;
#define							nb6parammax		_nb6parammax[0]
extern NB6PARAMMAX_MAIN		*const _nb6parammax_main;
#define							nb6parammax_main	_nb6parammax_main[0]
extern NB6REQUIREDMERITS   *const nb6requiredmerits;
extern NB6SETTINGS		*const _nb6settings;
#define						    nb6settings		_nb6settings[0]
extern NB6SETTINGSEX	*const _nb6settingsex;
#define						    nb6settingsex	_nb6settingsex[0]

extern char *psznb6savedatafilename;

extern int GAMEDATASTRUCT_KAHOUTYPELIST_POINTER[GAMEDATASTRUCT_KAHOUTYPE_NUM];
extern int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[GAMEDATASTRUCT_POSITIONTYPE_NUM];

extern NB6SOLDIERMAX_CUSTOM nb6soldiermax_custom[GAMEDATASTRUCT_SOLDIERMAX_NUM];

extern NB6FUKIDASHIRELATEPERSONS *const nb6fukidashirelatepersons;


extern NB6CASTLE_HEX_ROLE *const _nb6castle_hex_role;
#define							  nb6castle_hex_role		_nb6castle_hex_role[0]
extern NB6CASTLE_HEX_HIGH *const _nb6castle_hex_high;
#define							  nb6castle_hex_high		_nb6castle_hex_high[0]

extern NB6FIELD_HEX_ROLE *const _nb6field_hex_role;
#define							 nb6field_hex_role			_nb6field_hex_role[0]

extern NB6CASTLE_HEX_ROLE *const _nb6fire_hex_role;
#define							  nb6fire_hex_role			_nb6fire_hex_role[0]

extern NB6CASTLE_HEX_UNIT_POS *const _nb6castle_hex_unit_pos;
#define							      nb6castle_hex_unit_pos		_nb6castle_hex_unit_pos[0]
extern NB6FIELD_HEX_UNIT_POS *const  _nb6field_hex_unit_pos;
#define							      nb6field_hex_unit_pos			_nb6field_hex_unit_pos[0]

extern NB6BUSHOU_HEX_UNIT_INFO *const nb6bushou_hex_unit_info;
extern NB6GUNDAN_HEX_INFO	   *const _nb6gundan_hex_info;
#define							      nb6gundan_hex_info		_nb6gundan_hex_info[0]

extern NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO *const nb6war_joint_daimyo_hex_group_info;

extern NB6FUKIDASHI_DIALOG_LAYOUT *const _nb6fukidashi_dialog_layout;
#define						nb6fukidashi_dialog_layout		_nb6fukidashi_dialog_layout[0]


