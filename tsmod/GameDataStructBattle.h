#pragma once

//--------------------------------------------------HEX 

// ヘックスの座標軸系。Ｘ→ Ｙ↓　で値が増加していく。
struct HEX_POSITION {
	HEX_POSITION(double _x, double _y) : x(_x), y(_y) {}
	HEX_POSITION() : x(0), y(0) {}
	double x;
	double y;
};
// フィールドヘックスの座標→普通の３次元座標(左手デカルト・音と同じへ)
D3D_POSITION FieldHexPos_To_WorldPos(HEX_POSITION p);

// 普通の３次元座標(左手デカルト)→フィールドヘックスの座標へ
HEX_POSITION WorldPos_To_FieldHexPos(D3D_POSITION p);

// 城ヘックスの座標→普通の３次元座標(左手デカルト・音と同じへ)
D3D_POSITION CastleHexPos_To_WorldPos(HEX_POSITION p);

// 普通の３次元座標(左手デカルト)→城ヘックスの座標へ
HEX_POSITION WorldPos_To_CastleHexPos(D3D_POSITION p);



// フィールドの方向ベクトル。Ｘ→ Ｙ↓　で値が増加していく。
struct HEX_VECTOR {
	HEX_VECTOR(double _x, double _y) : x(_x), y(_y) {}
	HEX_VECTOR() : x(0), y(0) {}
	double x;
	double y;
};

// フィールド座標同志の減算。座標Ａ→座標Ｂのベクトルを求めるのに、通常、Ｂ－Ａをするだろうから。
HEX_VECTOR operator-(HEX_POSITION &p1, HEX_POSITION &p2);




//const int GAMEDATASTRUCT_CASTLE_HEX_POSX_MAX = GAMEDATASTRUCT_CASTLE_HEX_COLS;	// 攻城戦のHEXのHEX_POSITION型にした際ののXの最大値
//const int GAMEDATASTRUCT_CASTLE_HEX_POXY_MAX = GAMEDATASTRUCT_CASTLE_HEX_ROWS;	// 攻城戦のHEXのHEX_POSITION型にした際ののYの最大値


const int GAMEDATASTRUCT_CASTLE_HEX_CENTER_X = 9;
const int GAMEDATASTRUCT_CASTLE_HEX_CENTER_Y = 8;

namespace CASTLE_HEX_ROLE {
	enum { 枠外 = 0x00, 城内平地 = 0x10, 城内整地 = 0x11, 城外平地 = 0x12, 森 = 0x13, 堀 = 0x14, 海 = 0x14, 湖 = 0x14, 堀橋 = 0x15, 城壁 = 0x16, 閉門 = 0x17, 開門 = 0x18, 櫓 = 0x19, 柵 = 0x1A, 本丸 = 0x1B };
}


// 戦闘画面の城内ヘックスの役割のバイト列
union NB6CASTLE_HEX_ROLE {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// バイト列のインデックスとしてみた場合
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18桁、16行として見た場合;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// 戦闘画面の城内ヘックスの高さのバイト列
union NB6CASTLE_HEX_HIGH {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// バイト列のインデックスとしてみた場合
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18桁、16行として見た場合;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// 城攻時のユニットのマップ内のポジション
union NB6CASTLE_HEX_UNIT_POS {
	BYTE index[GAMEDATASTRUCT_CASTLE_HEX_COLS * GAMEDATASTRUCT_CASTLE_HEX_ROWS];				// バイト列のインデックスとしてみた場合
	struct {
		BYTE pos_x[GAMEDATASTRUCT_CASTLE_HEX_COLS];				// 18桁、16行として見た場合;
	} pos_y[GAMEDATASTRUCT_CASTLE_HEX_ROWS];
};

// あるヘックス場所のチップの役割(CASTLE_HEX_ROLE)を得る
int GetCastleHexRole(double x, double y);
int GetCastleHexRole(HEX_POSITION p);

// あるヘックス場所の高さ
int GetCastleHexHigh(double x, double y);
int GetCastleHexHigh(HEX_POSITION p);


namespace FIRE_HEX_ROLE {
	enum { 無し = 0x00, 炎上 = 0x01 };
}

// あるヘックス場所のチップの炎状態
int GetFireHexRole(double x, double y);
int GetFireHexRole(HEX_POSITION p);

const int GAMEDATASTRUCT_FIELD_HEX_CENTER_X = 20;
const int GAMEDATASTRUCT_FIELD_HEX_CENTER_Y = 12;

namespace FIELD_HEX_ROLE {
	enum { 枠外 = 0x00, 平地 = 0x1, 道 = 0x2, 湿地 = 0x3, 荒れ地 = 0x4, 森 = 0x5, 低山 = 0x6, 中山 = 0x7, 高山 = 0x8, 川 = 0x9, 橋 = 0xA, 海 = 0xB, 湖 = 0xB, 城 = 0xC /*２個連続*/ };
}


// 野戦時のヘックスの役割のバイト列
union NB6FIELD_HEX_ROLE {
	BYTE index[GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS];				// バイト列のインデックスとしてみた場合
	struct {
		BYTE pos_x[GAMEDATASTRUCT_FIELD_HEX_COLS];				// 41桁、24行として見た場合;
	} pos_y[GAMEDATASTRUCT_FIELD_HEX_ROWS];
};

// 野戦時のユニットのマップ内のポジション
union NB6FIELD_HEX_UNIT_POS {
	BYTE index[GAMEDATASTRUCT_FIELD_HEX_COLS * GAMEDATASTRUCT_FIELD_HEX_ROWS];				// バイト列のインデックスとしてみた場合
	struct {
		BYTE pos_x[GAMEDATASTRUCT_FIELD_HEX_COLS];				// 41桁、24行として見た場合;
	} pos_y[GAMEDATASTRUCT_FIELD_HEX_ROWS];
};

// 野戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
void ConvertFieldHexPosToFieldIndexPos(double x, double y, int *out_x, int *out_y);

// 攻城戦時のヘックス系の座標を、配列としての実値のindexへと変更する。
void ConvertCastleHexPosToCastleIndexPos(double x, double y, int *out_x, int *out_y);



// あるヘックス場所のチップの役割(FIELD_HEX_ROLE)を得る
int GetFieldHexRole(double x, double y);
int GetFieldHexRole(HEX_POSITION p);

// 逆に設定する。(といっても、設定したとしても、絵が変わらないので上手くいかないが)
void SetFieldHexRole(double x, double y, int eHexRole);
void SetFieldHexRole(HEX_POSITION p, int eHexRole);


// あるヘックス場所の高さ
int GetFieldHexHigh(double x, double y);
int GetFieldHexHigh(HEX_POSITION p);



// 戦争に出陣しているiBushouIDの一覧を得る。
vector<int> GetSyutujinBushouArray();
// 現在表示されているマップ限定で、出陣しているiBushouIDの一覧を得る。
vector<int> GetCurMapSyutujinBushouArray();

// 指定の武将ＩＤの武将がどこにいるのか？ 居ない場合、p(x=0xFF, y=0xFF) が返ってくる。
HEX_POSITION GetFieldHexUnitPos(int iBushouID);

// 指定の武将ＩＤの武将がどこにいるのか？ 居ない場合、p(x=0xFF, y=0xFF) が返ってくる。
HEX_POSITION GetCastleHexUnitPos(int iBushouID);

// ヘックスのＡ点とＢ点の間にあるチップのアレイを返す。
vector<int> RoleArrayOfBetweenPosToPos(HEX_POSITION pos1, HEX_POSITION pos2);


// ヘックス間隔の取得。p1→p2まで何枡移動する必要があるのか(全ての枡は行けるものと仮定する)
int getHexDistance(HEX_POSITION p1, HEX_POSITION p2);

bool Is_FieldWar();          // 現在画面に出てる戦場は、野戦
bool Is_CastleWar();         // 現在画面に出てる戦場は、攻城戦

int getCastleTurnInBattle();  // 攻城戦で何ターン目か。１～３．攻城戦でない場合は０．

// このユニット番号
struct NB6BUSHOU_HEX_UNIT_INFO {
	WORD bushou;					// 武将番号。1大きい数の方。武将配列番号に治すには-1すること。
	WORD junban;					// この値はターン順番。戦争において、この数値が若い軍団から順番が回るということ。
	int	 pos_x;						// 行内におけるpos_x。捕まったり、退却したりして、戦場から居なくなると、0xFFFF
	int	 pos_y;						// 行内におけるpos_y。捕まったり、退却したりして、戦場から居なくなると、0xFFFF
	BYTE movepower;					// 移動力残量。0x1～0xFぐらいの数値が入っている。
	WORD _undef2;					// 不明2 ユニット存在してるのはFFFF、していないのは
	BYTE _undef3;					// 不明1
	BYTE _undef4;					// 不明1
	BYTE _undef5;					// 不明1
	WORD _what1;					// 何か意味がありそう。1～8ぐらいの数値が入っている。
	WORD _undef6;					// ほぼ0x0000
	WORD _undef7;					// ほぼ0xFFFF
	BYTE konran : 4;					// 1=通常混乱 2=大混乱 3=挑発
	BYTE bEnd : 4;					// 行動済フラグ。0x2=行動済み, 0x0=未行動
	BYTE _what4;					// 何か意味がありそう。存在してる武将は、｢0x2｣が多い
	WORD _what5;					// 何か意味がありそう。存在してる武将は、｢0x70｣が多い
	int	 _undef8;					// 不明
};
/*
以下、戦闘関連の各種パラメータへのアクセス・Set系関数と思われる
0042DBF1  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DBF5  |. E8 DFFEFFFF    CALL    TENSHOU.0042DAD9
0042DBFA  |. 0FB640 0C      MOVZX   EAX, BYTE PTR DS:[EAX+C]
0042DBFE  |. 83C4 04        ADD     ESP, 4
0042DC01  \. C3             RETN
0042DC02  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC06  |. E8 CEFEFFFF    CALL    TENSHOU.0042DAD9
0042DC0B  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]
0042DC0F  |. 83C4 04        ADD     ESP, 4
0042DC12  |. 8848 0C        MOV     BYTE PTR DS:[EAX+C], CL
0042DC15  \. C3             RETN
0042DC16  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC1A  |. E8 BAFEFFFF    CALL    TENSHOU.0042DAD9
0042DC1F  |. 83C4 04        ADD     ESP, 4
0042DC22  |. 8B40 0D        MOV     EAX, DWORD PTR DS:[EAX+D]
0042DC25  \. C3             RETN
0042DC26  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC2A  |. E8 AAFEFFFF    CALL    TENSHOU.0042DAD9
0042DC2F  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC33  |. 83C4 04        ADD     ESP, 4
0042DC36  |. 66:8948 0D     MOV     WORD PTR DS:[EAX+D], CX
0042DC3A  \. C3             RETN
0042DC3B  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC3F  |. E8 95FEFFFF    CALL    TENSHOU.0042DAD9
0042DC44  |. 0FB640 11      MOVZX   EAX, BYTE PTR DS:[EAX+11]
0042DC48  |. 83C4 04        ADD     ESP, 4
0042DC4B  \. C3             RETN
0042DC4C  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC50  |. E8 84FEFFFF    CALL    TENSHOU.0042DAD9
0042DC55  |. 8A4C24 0C      MOV     CL, BYTE PTR SS:[ESP+C]
0042DC59  |. 83C4 04        ADD     ESP, 4
0042DC5C  |. 8848 11        MOV     BYTE PTR DS:[EAX+11], CL
0042DC5F  \. C3             RETN
0042DC60  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC64  |. E8 70FEFFFF    CALL    TENSHOU.0042DAD9
0042DC69  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC6D  |. 83C4 04        ADD     ESP, 4
0042DC70  |. 66:8948 12     MOV     WORD PTR DS:[EAX+12], CX
0042DC74  \. C3             RETN
0042DC75  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC79  |. E8 5BFEFFFF    CALL    TENSHOU.0042DAD9
0042DC7E  |. 83C4 04        ADD     ESP, 4
0042DC81  |. 8B40 12        MOV     EAX, DWORD PTR DS:[EAX+12]
0042DC84  \. C3             RETN
0042DC85  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC89  |. E8 4BFEFFFF    CALL    TENSHOU.0042DAD9
0042DC8E  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DC92  |. 83C4 04        ADD     ESP, 4
0042DC95  |. 0948 18        OR      DWORD PTR DS:[EAX+18], ECX
0042DC98  \. C3             RETN
0042DC99  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DC9D  |. E8 37FEFFFF    CALL    TENSHOU.0042DAD9
0042DCA2  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCA6  |. 83C4 04        ADD     ESP, 4
0042DCA9  |. F7D1           NOT     ECX
0042DCAB  |. 2148 18        AND     DWORD PTR DS:[EAX+18], ECX
0042DCAE  \. C3             RETN
0042DCAF  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCB3  |. E8 21FEFFFF    CALL    TENSHOU.0042DAD9
0042DCB8  |. 83C4 04        ADD     ESP, 4
0042DCBB  |. 8B40 18        MOV     EAX, DWORD PTR DS:[EAX+18]
0042DCBE  |. 234424 08      AND     EAX, DWORD PTR SS:[ESP+8]
0042DCC2  \. C3             RETN
0042DCC3  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCC7  |. E8 0DFEFFFF    CALL    TENSHOU.0042DAD9
0042DCCC  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCD0  |. 83C4 04        ADD     ESP, 4
0042DCD3  |. 0948 1C        OR      DWORD PTR DS:[EAX+1C], ECX
0042DCD6  \. C3             RETN
0042DCD7  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCDB  |. E8 F9FDFFFF    CALL    TENSHOU.0042DAD9
0042DCE0  |. 8B4C24 0C      MOV     ECX, DWORD PTR SS:[ESP+C]
0042DCE4  |. 83C4 04        ADD     ESP, 4
0042DCE7  |. F7D1           NOT     ECX
0042DCE9  |. 2148 1C        AND     DWORD PTR DS:[EAX+1C], ECX
0042DCEC  \. C3             RETN
0042DCED  /$ FF7424 04      PUSH    DWORD PTR SS:[ESP+4]
0042DCF1  |. E8 E3FDFFFF    CALL    TENSHOU.0042DAD9
0042DCF6  |. 83C4 04        ADD     ESP, 4
0042DCF9  |. 8B40 1C        MOV     EAX, DWORD PTR DS:[EAX+1C]
0042DCFC  |. 234424 08      AND     EAX, DWORD PTR SS:[ESP+8]
0042DD00  \. C3             RETN
*/
/*
上記値の初期化部分。戦争開始時
0042DD59  /$ 56             PUSH    ESI
0042DD5A  |. 57             PUSH    EDI
0042DD5B  |. 66:BE 0100     MOV     SI, 1
0042DD5F  |. BF FFFF0000    MOV     EDI, 0FFFF
0042DD64  |> 57             /PUSH    EDI
0042DD65  |. 56             |PUSH    ESI
0042DD66  |. E8 A2FDFFFF    |CALL    TENSHOU.0042DB0D
0042DD6B  |. 83C4 08        |ADD     ESP, 8
0042DD6E  |. 57             |PUSH    EDI
0042DD6F  |. 56             |PUSH    ESI
0042DD70  |. E8 BEFDFFFF    |CALL    TENSHOU.0042DB33
0042DD75  |. 83C4 08        |ADD     ESP, 8
0042DD78  |. 57             |PUSH    EDI                                                        ; /Arg3
0042DD79  |. 57             |PUSH    EDI                                                        ; |Arg2
0042DD7A  |. 56             |PUSH    ESI                                                        ; |Arg1
0042DD7B  |. E8 3AFEFFFF    |CALL    TENSHOU.0042DBBA                                           ; \TENSHOU.0042DBBA
0042DD80  |. 83C4 0C        |ADD     ESP, 0C
0042DD83  |. 6A 00          |PUSH    0
0042DD85  |. 56             |PUSH    ESI
0042DD86  |. E8 77FEFFFF    |CALL    TENSHOU.0042DC02
0042DD8B  |. 83C4 08        |ADD     ESP, 8
0042DD8E  |. 57             |PUSH    EDI
0042DD8F  |. 56             |PUSH    ESI
0042DD90  |. E8 91FEFFFF    |CALL    TENSHOU.0042DC26
0042DD95  |. 83C4 08        |ADD     ESP, 8
0042DD98  |. 6A 00          |PUSH    0
0042DD9A  |. 56             |PUSH    ESI
0042DD9B  |. E8 ACFEFFFF    |CALL    TENSHOU.0042DC4C
0042DDA0  |. 83C4 08        |ADD     ESP, 8
0042DDA3  |. 57             |PUSH    EDI
0042DDA4  |. 56             |PUSH    ESI
0042DDA5  |. E8 B6FEFFFF    |CALL    TENSHOU.0042DC60
0042DDAA  |. 83C4 08        |ADD     ESP, 8
0042DDAD  |. 57             |PUSH    EDI
0042DDAE  |. 56             |PUSH    ESI
0042DDAF  |. E8 E1000000    |CALL    TENSHOU.0042DE95
0042DDB4  |. 83C4 08        |ADD     ESP, 8
0042DDB7  |. 57             |PUSH    EDI
0042DDB8  |. 56             |PUSH    ESI
0042DDB9  |. 66:46          |INC     SI
0042DDBB  |. E8 D9FEFFFF    |CALL    TENSHOU.0042DC99
0042DDC0  |. 83C4 08        |ADD     ESP, 8
0042DDC3  |. 66:83FE 48     |CMP     SI, 48
0042DDC7  |.^76 9B          \JBE     SHORT TENSHOU.0042DD64
0042DDC9  |. 5F             POP     EDI
0042DDCA  |. 5E             POP     ESI
0042DDCB  \. C3             RETN
*/


// 指定武将が戦場において、行動済みかどうか
BOOL IsActionEndInBattle(int iBushouID);
// 指定武将を行動済みとする
void setIsActionEndInBattle(int iBushouID, int isEnd);

namespace KONRAN_STATUS {
	enum { 無し = 0, 混乱 = 1, 大混乱 = 2, 挑発混乱 = 3 };
}
// 対象の武将が混乱しているか。
int GetKonranStatus(int iBushouID);
// 対象の武将を混乱させる。(再描画を伴う)
BOOL SetKonranStatus(int iBushouID, int iKonranStatus);
// 対象の武将を指定のKONRAN_STATUSにする

// ヘックスマップにおける。担当ターン情報や、大名、軍団など。
struct NB6GUNDAN_HEX_INFO {
	int _undef;			// 不明
	int daimyo;			// 現在ターンの大名番号
	int gundan;			// 現在ターンの軍団番号
	int junban;		    // 現在ターンの軍団の「順番」番号。
};


// ヘックスマップにおいて、この戦争に関わっている「大名・軍団・総大将・[攻め手・受け手・中立]」情報
struct NB6WAR_JOINT_DAIMYO_HEX_GROUP_INFO {
	WORD daimyo;		// 大名番号
	WORD gundan;		// 軍団番号
	WORD soudaisho;		// 総大将の武将番号
	WORD group;			// 何番が攻側か、受側か、などは、不定。あくまでも、戦闘開始時の攻め側大名と同じグループが攻め側の援軍、受側の大名と同じグループが受側、それ以外が中立である。
	WORD _undef;		// 特になにもなし。must 0000
};

// 現在進行中の戦争に関わっているiDaimyoIDのリスト。(出陣しているかどうかは関係ない)
vector<int> GetCurWarJointDaimyoIDList();

// 現在進行中の戦争に関わっているiGundanIDのリスト。(出陣しているかどうかは関係ない)
vector<int> GetCurWarJointGundanIDList();



namespace WarStandPoint {
	//	   不明          直接攻撃側   直接守備側   攻撃側の援軍　      守備側の援軍　　中立軍 
	enum { unknown = 0, attack = 1, defend = 2, attack_enngun = 3, defend_enngun = 4, chuuritsu = 5 };
};

// 該当の軍団の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
int GetWarStandPointTheGundan(int iGundanID);

// 該当の武将の戦場における立場を返す(不明=0, 直接攻める側=1, 直接守る側=2, 攻める側の援軍=3, 守る側の援軍=4, 中立軍=5)
int GetWarStandPointTheBushou(int iBushouID);

// 現在の戦において、指定軍団の総大将(軍団長や大名のことではなく、戦場においての該当軍団の総大将。
int GetWarSoudaishoTheGundan(int iGundanID);





// 戦争ですでに何ターン経過したか
byte getHowMuchTurnProceesInBattle();

// 戦争で何ターン残っているか。メモリではなく、内部計算
// 戦闘ターン中でなければ -1 が返る。
int getRemainTurnInBattle();




// 天気情報を得る 0:晴れ 1:曇り 2:雨 3:雪
byte getWeather();

void reserveWeather(int eTenki, int iContinueTurn = 2);
