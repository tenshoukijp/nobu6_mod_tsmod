#pragma once


namespace –ğE {
	enum { –³‚µ = 0, ªˆÎ‘å«ŒR = 1, ŠÇ—Ì = 2, ŠÖ“ŒŠÇ—Ì = 3, ‰œB’T‘è = 4, ¼‘’T‘è = 5, ‹ãB’T‘è = 6, ‰HB’T‘è = 7 };
}

// ã‚Ì‘S•”ˆê—ñ‚Å‚Í‚¢‚Á‚½ó‘Ô‚©‚ç‚P‚Â‚P‚Â‚ğæ‚èo‚·‚½‚ß‚É•K—v‚Æ‚È‚é‘Š‘ÎƒAƒhƒŒƒXéŒ¾ 
int GAMEDATASTRUCT_POSITIONTYPELIST_POINTER[];

// –ğEƒ^ƒCƒv‚ÌID‚©‚ç•¶š—ñ‚ğ“¾‚é
char* getPositionTypeName(byte type);


// ³‹K‰»‚³‚ê‚½–ğE”Ô†‚©‚ç‘å–¼—p‚Ì–ğE”Ô†‚ğ“¾‚éB
int DeNormalizeDaimyoYakusyokuValue(int iYakusyokuValue);

// ‘å–¼ID‚©‚ç³‹K‰»‚³‚ê‚½–ğEID‚ğ“¾‚é
int getNormalizedDaimyoPosition(int iDaimyoID);

// ŒR’cID‚©‚ç³‹K‰»‚³‚ê‚½–ğEID‚ğ“¾‚é
int getNormalizedGundanPosition(int iGundanID);

// ‘å–¼‚Ì–ğE‚Ì•t‚¯‘Ö‚¦‚ğ‚İ‚é
bool tryChangeDaimyoYakusyoku(int iDaimyoID, int iYakusyokuValue);

// ŒR’c‚Ì–ğE‚Ì•t‚¯‘Ö‚¦‚ğ‚İ‚é
bool tryChangeGundanYakusyoku(int iGundanID, int iYakusyokuValue);

