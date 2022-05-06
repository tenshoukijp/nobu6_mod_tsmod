#pragma once

#pragma pack(1)


#define GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE		844623    // [通常のセーブデータのファイルサイズ]
#define GAMEDATASTRUCT_SAVEDATA_SLOT_NUM			8		  // [セーブデータのスロット枠の数]
#define GAMEDATASTRUCT_MAPFLAG_NUM					3000	  // ラベル付きのフラグ数


struct NB6BUSHOU_SAVEDATA_EX {
	// 以下３バイト
	byte ansatsu_cnt;	// 該当武将が１季節にどれだけ暗殺しているか
	byte undef_1;	    // 未定義
	byte undef_2;	    // 未定義

	// 以下４バイト
	byte aptitudeken:4;	// 剣の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeyumi:4;	// 弓の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeyari:4;	// 槍の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudeono:4;	// 斧の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudemuchi:4;	// 鞭の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetuchi:4;	// 槌の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetue:4;	// 杖の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitude_undef1:4;	// 

	// 以下４バイト
	byte education;			//教養
	byte command;			//統率
	byte undef_4;	    // 未定義
	byte undef_5;	    // 未定義

	// 以下４バイト
	byte aptitudenougyou:4;	// 農業の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudesyougyou:4;// 商業の適正  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudechikujo:4;	// 築城の適性 (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudegaikou:4;	// 外交の適正  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte aptitudetouyou:4;	// 登用の適正  (0=E,1=D,2=C,3=B,4=A,5=S)
	byte undef_6:4;	    // 未定義
	byte undef_7;	    // 未定義

	WORD ruiseki_exp_gov;   // 累積政治熟練度。一季節ごとに半減。
	WORD ruiseki_exp_bat;   // 累積戦闘熟練度。一季節ごとに半減。
	WORD ruiseki_exp_int;   // 累積智謀熟練度。一季節ごとに半減。
	WORD undef_8;

	int array[44];
};

struct NB6CASTLE_SAVEDATA_EX {
	char name[9];	//予備
	int array[10];
};

struct NB6GUNDAN_SAVEDATA_EX {
	char name[17];	//予備
	int array[10];
};

struct NB6DAIMYO_SAVEDATA_EX {
	char name[17];	//予備
	int array[10];
};

struct NB6KAHOU_SAVEDATA_EX {
	char name[13];	//予備
	int array[10];
};

struct NB6KANI_SAVEDATA_EX {
	char name[13];	//予備
	int array[10];
};

struct NB6CHIMEI_SAVEDATA_EX {
    char name[13];	//予備
	int array[5];
};

struct NB6FLAGS_SAVEDATA_EX {
	byte flag1 : 1;	
	byte flag2 : 1;	
	byte flag3 : 1;	
	byte flag4 : 1;	
	byte flag5 : 1;	
	byte flag6 : 1;	
	byte flag7 : 1;	
	byte flag8 : 1;	
	byte _undef : 6;
	int value : 18;	
};

struct NB6MAPFLAGS_SAVEDATA_EX {
	char name[9];   // フラグ名  
	byte flag1 : 1;	
	byte flag2 : 1;	
	byte flag3 : 1;	
	byte flag4 : 1;	
	byte flag5 : 1;	
	byte flag6 : 1;	
	byte flag7 : 1;	
	byte flag8 : 1;	
	byte _undef : 6;
	int value : 18;	// フラグ値。
};

// 用途未定。予備。何か新設すると、ここの数を削って言ってサイズ帳尻を合わせる。
struct NB6UNDEF_SAVEDATA_EX {
	int _undef[10000];
};


// 全部の拡張セーブデータの構造体
struct SAVEDATA_EX {
	NB6BUSHOU_SAVEDATA_EX bushou_ex[GAMEDATASTRUCT_BUSHOU_NUM];
	NB6CASTLE_SAVEDATA_EX castle_ex[GAMEDATASTRUCT_CASTLE_NUM];
	NB6GUNDAN_SAVEDATA_EX gundan_ex[GAMEDATASTRUCT_GUNDAN_NUM];
	NB6DAIMYO_SAVEDATA_EX daimyo_ex[GAMEDATASTRUCT_DAIMYO_NUM];
	NB6KAHOU_SAVEDATA_EX kahou_ex[GAMEDATASTRUCT_KAHOU_NUM];
	NB6KANI_SAVEDATA_EX kani_ex[GAMEDATASTRUCT_KANI_NUM];
	NB6CHIMEI_SAVEDATA_EX chimei_ex[GAMEDATASTRUCT_CHIMEI_NUM];
	NB6FLAGS_SAVEDATA_EX flag_ex[10000];
	NB6MAPFLAGS_SAVEDATA_EX mapflag_ex[GAMEDATASTRUCT_MAPFLAG_NUM];
	NB6UNDEF_SAVEDATA_EX _undef;
};

// 拡張セーブデータの全セーブデータ共通部
struct SAVEDATA_EX_COMMON {
	int _undef[1000];
};

extern SAVEDATA_EX nb6savedata_ex;

// 拡張部分をセーブする。
bool SaveSaveDataEx(int iSelectID );

// 拡張部分をロードする。
bool LoadSaveDataEx(int iSelectID );


extern SAVEDATA_EX nb6savedata_ex;
extern SAVEDATA_EX_COMMON nb6savedata_ex_common; // 全セーブデータ共通部
