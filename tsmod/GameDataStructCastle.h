#pragma once

/*
城情報(33*214)

[33バイト]

0-1		軍団所属リストの前
2-3		リストの後ろ
4-5		城主(先頭) // 城主状態などに変更した場合はリストを整頓しなおす必要がある。
6-7		浪人(先頭) // 浪人状態などに変更した場合はリストを整頓しなおす必要がある。
8-16	城名
17-18	所属 (所属軍団番号)
19		城郭
20-21	石高
22		商業
23-24	人口-徴兵可
25		民忠
26-27	徴兵可
28		兵質 0-2 (0=悪,1=普,2=良)
29		ABC0 EFGH
A 一揆 B 国際港 C 港 E 馬産地 F 鍛冶 G 本城 H 居城
30		ABCD EFGH
C 一揆扇動 G 金山 H 銀山
31		AAAAAA BB
A:城称	0x00 城 0x01 御坊 0x02 寺 0x04 御所 0x08 館 0x10 町 0x20 城、それ以外の値=何も付かない
B:｢絵｣を表示した時の、背景の種類(0=山) (1=林) (2=海) (3=平野)
32		000A000B A-巨城 B 名前変更
*/
struct NB6CASTLE
{
	WORD prev;		//軍団所属城リスト 前の城
	WORD next;		//軍団所属城リスト 後の城
	WORD master;	//城主(先頭)
	WORD ronin;		//浪人(先頭)
	char name[9];	//城名
	WORD attach;	//所属 (所属軍団番号)
	byte build;		//城郭
	WORD grain;		//石高
	byte town;		//商業
	WORD population;//人口
	byte loyal;		//民忠
	WORD soldier;	//徴兵可
	byte quality;	//兵質 0-2  (0=悪,1=普,2=良)
	byte honjo : 2;	//本城 居城 (大名の本城=1, 軍団長の本城=2);
	byte bSmith : 1;	//鍛冶
	byte bHorse : 1;	//馬産地
	byte _undef3 : 1;	//0
	byte bPort : 1;	//港
	byte blPort : 1;	//国際港
	byte _undef4 : 1;	//

	byte bSilver : 1;	//銀山
	byte bGold : 1;	//金山
	byte _undef5 : 2;	//
	byte bRevolt : 1;	//一揆扇動
	byte _undef6 : 3;	//

	byte bgback : 2;	//｢絵｣を表示した時の、背景の種類 城称(0=山) (1=林) (2=海) (3=平野)
	byte title : 6;	//城称

	byte nameEdited : 1; //名前変更
	byte _undef7 : 2;	//
	byte bLarge : 1;	//巨城
	byte _undef8 : 4;  
};

void setCastleName(int iCastleID, string name);

char nb6CastleTypeName[][5];

// 城のタイプIDから具体的な文字列を得る
char *getCastleTypeName(int iCastleType);

// 指定の大名が所持している城の数。
int getDaimyoHasCastleNum(int iDaimyoID);

// 指定の大名が所持している城の番号【配列用】のリスト
vector<int> getDaimyoHasCastleList(int iDaimyoID);

// 指定の軍団が所持している城の数。
int getGundanHasCastleNum(int iGundanID);

// 城マップが持つ堅牢度を出す。
int getCastleMapSolidDegre(int iCastleID);

// 城配下の兵数
int getCastleHeisuCnt(int iCastleID);



/*
城の位置情報など(8*214)

[8バイト]

0	地域名
1	0
2	石高最大値/10
3	商業最大値
4	Xセル座標(マップで見えてるのとは異なる区切り方 50個で区切られている)
5	Yセル座標(上と同じで、マップで見えているのとは異なる区切り方12個程度で区切られている)
6	X_offset
7	Y_offset
*/
struct NB6CASTLEMAX
{
	byte chiiki;	//所属地域番号)
	byte _undef1;	//must 0
	byte maxgrain;	//最石高/10
	byte maxtown;	//最商業
	byte _x_undef;	//セル中のXのずれ
	byte _y_undef;	//セル中のYのずれ
	byte x;			//セルX座標    グリッド。
	byte y;			//セルY座標	   グリッド。
};


// メインマップのグリッド用の構造体。。Ｘ→ Ｙ↓　で値が増加していく。
struct GRID_POSITION {
	int x;
	int y;
	GRID_POSITION(int _x, int _y);

	WORD nw; // 北西【城番号】; iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD no; // 北【城番号】;   iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD ne; // 北東【城番号】; iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD we; // 西【城番号】;   iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD ea; // 東【城番号】;   iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD sw; // 南西【城番号】; iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD so; // 南【城番号】;   iCaslteIDより1多い。 存在しない時は、0xFFFF;
	WORD se; // 南東【城番号】; iCaslteIDより1多い。 存在しない時は、0xFFFF;
};

// グリッドの方向ベクトル。Ｘ→ Ｙ↓　で値が増加していく。
struct GRID_VECTOR {
	int x;
	int y;
	GRID_VECTOR(int _x, int _y) : x(_x), y(_y) {}
	GRID_VECTOR() : x(0), y(0) {}
};

// グリッド座標同志の減算。座標Ａ→座標Ｂのベクトルを求めるのに、通常、Ｂ－Ａをするだろうから。
GRID_VECTOR operator-(GRID_POSITION &p1, GRID_POSITION &p2);


// 城ＩＤから、メイングリッド上のポジションを得る。左上グリッドが0,0。mapdataobgkのグリッドを見ればわかりやすい。
// １つのグリッドには、１つの城しかない。
// 全国マップとエリアのルール 
// 各グリッド4x3マスに分割されていて、城の位置は移動可能だが、実質6マスのどれかが使用される。 
// グリッドの左右端では「エリア内の部分」のみクリック反応がある。 
// また、グリッドの上部では、はみ出した城の屋根部分はクリック反応しない。 


GRID_POSITION getCastlePosInMainGrid(int iCastleID);

// 指定の城に対して隣接している城のリストを得る
vector<int> getRinsetsuCastles(int iCastleID);

// 指定の城から道が繋がっている隣接城のリストを得る。
vector<int> getWayConnectCastles(int iCastleID);




