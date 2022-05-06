#pragma once


#include "WinTarget.h"
#include <mmsystem.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"


class EXBushou {
	protected:
		bool m_isUltraBushou;			// この人物独特のスーパーモード(ON/OFF切り替え)

		DWORD m_curTimeGetTime;
		DWORD m_preTimeGetTime;

		int m_nb6bushou_ix;			// この武将の配列ID(512までのやつ)

		byte m_lifetimes;			// この武将の歳を取らない具合

		char m_name[13];			// 全角６文字+\0

	public:

		EXBushou();

		// スーパーモードをセット
		void setSuperMode(bool isUltraBushou) {
			m_isUltraBushou = isUltraBushou;
		}

		// 武将名の文字列を得る(最初は空のはずなので注意)
		char *getName() {
			return m_name;
		}
		// 武将名を設定する
		void setName(char *name);
	
		// NB6BUSHOUデータ上でのIDを得る(最初は-1のはずなので注意)
		// @return: -1=none, 0～531 が存在する場合に返ってくる武将IX
		int getNb6BushouIX();

		// NB6BUSHOUデータ上でのIDをセット
		// 事前にIDがわかっている場合は名前よりこちらの方が良い
		void setNb6BushouIX(int iBushouID);

		// 兵士をリジェネ的に増やす時に利用する
		// @param : RefleshTime 兵士が1リフレ増えるのにかかる時間。ミリ秒。
		//		  : 長ければ長いほど増えにくい。
		// @param : isForce 時間にかかわらず増やす。また兵数０でも増やす際に、１とする。この時は訓練値も自動で設定される。
		void checkRegenerationNB6BushouSoldier(int RefleshTime, int isForce = 0);

		// 大砲を修復する。
		void checkRestoreNB6BushouTaihou();

		// 歳を取るのが数倍遅い
		void checkTimesNB6BushouLife();

		// 歳を取らない具合を設定
		void setLifeTimes(byte lifetimes) {
			m_lifetimes = lifetimes;
		}

};

