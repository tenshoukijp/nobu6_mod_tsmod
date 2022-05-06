#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include "ScenarioMod.h"


bool isEnteringFromLoadDialogExecute = FALSE; // ロード画面からメインゲームにいくよ

SAVEDATA_EX nb6savedata_ex = {};
SAVEDATA_EX_COMMON nb6savedata_ex_common = {}; // 全セーブデータ共通部


void SetOnSaveOfSaveDataEx() {
	/*
	 * 拡張部分をセーブする前にここで、データセッティングする。
	 * 値のコピーや加工が必要ならば
	 */
}


bool SaveSaveDataEx(int iSelectID ) {

	FILE *fp = fopen( psznb6savedatafilename, "rb+" );
	long sz;

	if( fp == NULL ){
		// "%sファイルが開けません"
		return false;

	} else {

		// セーブ前のデータへのセッティング
		SetOnSaveOfSaveDataEx();

		fseek( fp, 0, SEEK_END ); // 最後の位置に(サイズ取得のためだけ)
		sz = ftell( fp );		  // サイズを取得する。

		fseek( fp, 0, SEEK_SET ); // 最初の位置に戻す。

		// すでにセーブデータは拡張されている。
		if ( sz >= GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+sizeof(SAVEDATA_EX)*8 ) {

			int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(iSelectID)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める

			fseek( fp, seek_pos, SEEK_SET );

			// 1回で全部書き込み
			fwrite( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

			// ---最後に共通部
			seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(GAMEDATASTRUCT_SAVEDATA_SLOT_NUM)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める
			fseek( fp, seek_pos, SEEK_SET );

			// 1回で全部書き込み
			fwrite( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);

		// デフォルトのまま
		} else {

			for ( int i=0; i < GAMEDATASTRUCT_SAVEDATA_SLOT_NUM; i++ ) {
				// 選択対象のセーブデータで無い分は、白紙のSAVEDATA_EXを出力。
				if ( i != iSelectID ) {
					SAVEDATA_EX dummy_savedata_ex = {};
					int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(i)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める
					fseek( fp, 0, SEEK_END );

					// 1回で全部書き込み
					int a = fwrite( (void *)&dummy_savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

				// 選択対象だけは、nb6savedata_exのデータを出力する。
				} else if ( i==iSelectID ) {
					int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(i)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める

					fseek( fp, 0, SEEK_END );

					// 1回で全部書き込み
					int a = fwrite( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

				}
			}

			// ---最後に共通部
			fseek( fp, 0, SEEK_END );
			// 1回で全部書き込み
			fwrite( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);
		}


		fclose( fp );

		// -------------------------------------
		// ScenarioMod実行
		// -------------------------------------
		if (p_snOnSaveSaveData) {
			p_snOnSaveSaveData(iSelectID, psznb6savedatafilename);
		}

		return true;
	}

	return false;
}


void SetOnLoadOfSaveDataEx() {
	/*
	 * 拡張部分をセーブする前にここで、データセッティングする。
	 * 値のコピーや加工が必要ならば
	 */
}

bool LoadSaveDataEx(int iSelectID ) {

	FILE *fp = fopen( psznb6savedatafilename, "rb" );
	long sz;

	if( fp == NULL ){
		// %sファイルが開けません\n"
		return false;

	} else {

		fseek( fp, 0, SEEK_END ); // 最後の位置に(サイズ取得のためだけ)
		sz = ftell( fp );		  // サイズを取得する。

		fseek( fp, 0, SEEK_SET ); // 最初の位置に戻す。

		// すでにセーブデータは拡張されている。
		if ( sz >= GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+sizeof(SAVEDATA_EX)*8 ) {

			int seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(iSelectID)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める

			fseek( fp, seek_pos, SEEK_SET );

			// 1回で全部書き込み
			fread( (void *)&nb6savedata_ex, sizeof(SAVEDATA_EX), 1, fp);

			// ---最後に共通部
			seek_pos = GAMEDATASTRUCT_NORMAL_SAVEDATAFILE_SIZE+(GAMEDATASTRUCT_SAVEDATA_SLOT_NUM)*sizeof(SAVEDATA_EX); // セーブデータの選択位置からファイルシーク位置を決める
			fseek( fp, seek_pos, SEEK_SET );
			// 1回で全部書き込み
			fread( (void *)&nb6savedata_ex_common, sizeof(SAVEDATA_EX_COMMON), 1, fp);

			// ロード後のデータへのセッティング
			SetOnLoadOfSaveDataEx();


		}


		fclose( fp );

		// -------------------------------------
		// ScenarioMod実行
		// -------------------------------------
		if (p_snOnLoadSaveData) {
			p_snOnLoadSaveData(iSelectID, psznb6savedatafilename);
		}

		// ロードからメインゲームに行くよというフラグを立てる。
		isEnteringFromLoadDialogExecute = TRUE;

		return true;
	}

	return false;
}