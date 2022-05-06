#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"


void SetMapFlagTarget(int iFlagID, int iValue, int iFlagSlot) {
	int i = iFlagID;
	switch (iFlagSlot) {
		case 0: 
			{
				nb6savedata_ex.mapflag_ex[i].value = iValue;
				break;
			}
		case 1: 
			{
				nb6savedata_ex.mapflag_ex[i].flag1 = iValue;
				break;
			}
		case 2: 
			{
				nb6savedata_ex.mapflag_ex[i].flag2 = iValue;
				break;
			}
		case 3: 
			{
				nb6savedata_ex.mapflag_ex[i].flag3 = iValue;
				break;
			}
		case 4: 
			{
				nb6savedata_ex.mapflag_ex[i].flag4 = iValue;
				break;
			}
		case 5: 
			{
				nb6savedata_ex.mapflag_ex[i].flag5 = iValue;
				break;
			}
		case 6: 
			{
				nb6savedata_ex.mapflag_ex[i].flag6 = iValue;
				break;
			}
		case 7: 
			{
				nb6savedata_ex.mapflag_ex[i].flag7 = iValue;
				break;
			}
		case 8: 
			{
				nb6savedata_ex.mapflag_ex[i].flag8 = iValue;
				break;
			}
	}
}


int WINAPI Extern_SetMapFlag( char *szFlagName, int iFlagValue, int iFlagSlot ) {
	// すでに存在するフラグの中で探す。
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// 長さゼロはいち早く次へ
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// そのラベルが存在した。
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // ８文字のみで判定
			// フラグを設定
			SetMapFlagTarget( i, iFlagValue, iFlagSlot );

			return TRUE;
		}
	}

	// フラグが存在しないので、新しく記載する。
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// 空の部分を見つける。
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {

			strncpy( nb6savedata_ex.mapflag_ex[i].name, szFlagName, 8 ); // ８文字までコピー
			nb6savedata_ex.mapflag_ex[i].name[8] = NULL; // ９文字目に番兵配置。

			// フラグを設定
			SetMapFlagTarget( i, iFlagValue, iFlagSlot );

			// ここまで来たら設定できたのでtrueを返す。
			return TRUE;
		}
	}

	// ここまで来たということは空いてるフラグが無い。
	return FALSE;
}



int GetMapFlagTarget(int iFlagID, int iFlagSlot) {
	int i = iFlagID;
	switch (iFlagSlot) {
		case 0: 
			{
				return nb6savedata_ex.mapflag_ex[i].value;
				break;
			}
		case 1: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag1;
				break;
			}
		case 2: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag2;
				break;
			}
		case 3: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag3;
				break;
			}
		case 4: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag4;
				break;
			}
		case 5: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag5;
				break;
			}
		case 6: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag6;
				break;
			}
		case 7: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag7;
				break;
			}
		case 8: 
			{
				return nb6savedata_ex.mapflag_ex[i].flag8;
				break;
			}
	}
	return FALSE;
}


int WINAPI Extern_GetMapFlag( char *szFlagName, int iFlagSlot ) {
	// すでに存在するフラグの中で探す。
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// 長さゼロはいち早く次へ
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// そのラベルが存在した。
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // ８文字のみで判定
			// フラグを設定
			return GetMapFlagTarget( i, iFlagSlot );
		}
	}

	// ここまで来たということは該当のラエルのフラグが無い。
	return FALSE;
}


int WINAPI Extern_DeleteMapFlag( char *szFlagName ) {

	// すでに存在するフラグの中で探す。
	for ( int i=0; i < GAMEDATASTRUCT_MAPFLAG_NUM; i++ ) {
		// 長さゼロはいち早く次へ
		if ( strlen( nb6savedata_ex.mapflag_ex[i].name ) == 0 ) {
			continue;
		}

		// そのラベルが存在した。
		if ( strncmp( szFlagName, nb6savedata_ex.mapflag_ex[i].name, 8 )==0 ) { // ８文字のみで判定
			// 中身全部消去
			memset( (void *)&nb6savedata_ex.mapflag_ex[i], 0, sizeof(nb6savedata_ex.mapflag_ex[i]) );

			return TRUE;
		}
	}

	// ここまで来たということは該当のラエルのフラグが無い。
	return FALSE;
}