
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"


/*
004CFEC4  91 AB 8C 79 93 AA 00 00 8E 98 91 E5 8F AB 00 00  足軽頭..侍大将..
004CFED4  95 94 8F AB 00 00 00 00 89 C6 98 56 00 00 00 00  部将....家老....
004CFEE4  8F 68 98 56 00 00 00 00                          宿老....○.

004D13A8  F0 FE 4C 00 EC FE 4C 00 [A0 FF 4C] 00 [F0 8E 4C] 00   隠居 大名
004D13B8  [E4 FE 4C] 00 [DC FE 4]」 00 [D4 FE 4C] 00 [CC FE 4C]  宿老～侍大将
004D13C8  [C4 FE 4C] 00 00 00 00 00 F0 8E 4C 00 E0 8E 4C 00  足軽頭


ｱﾄﾞﾚｽ      ﾃﾞｨｽｱｾﾝﾌﾞﾙ                                                                                         ｺﾒﾝﾄ
①00403B9D   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   ◎実装済  ゲーム内の実行場所は不明。ここは武将番号がEAXではなくESI
②00438F2A   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]	◎実装済　通常の武将リストの時に武将ごとにここを通過する。
③0045E428   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   ◎実装済　会見用のダイアログ
④0046002F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 軍事の編成(兵科編成) 
⑤00463E6F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 戦争時の左上の「部隊」からの情報
⑥0048F94B   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ◎未実装 昇進時のダイアログの下部で「◎◎に昇進しました」のメッセージうのところ
⑦004935E3   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 武将情報詳細画面
⑧004A1566   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 武将の助言の際に、ここを通過する。
⑨004A1C33   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 武将の「枠だけ」の噴き出しダイアログの時に、ここを通過する。(追放など)
⑩004A4B90   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ◎実装済 ゲーム内の実行場所は不明。
*/




static int GetOverRewriteMibunAddress(int iBushouID, int iPosition=-1 ) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// デフォルトは現在のポジションが入る
		if ( iPosition==-1) {
			iPosition = nb6bushouname[iBushouID].position;
		}

		// ScenarioModの呼び出し。( return しているのでScenarioModを先に入れる)
		if ( p_snOnViewingPositionName ) {
			int iPointerAddress = 0;
			p_snOnViewingPositionName( &iPointerAddress, iBushouID+1, iPosition );
			if ( iPointerAddress ) {
				return int(iPointerAddress);
			}
		}

		
		/*
		if ( TSModCommand::UltraBushou::cmdExists ) { // 以下、ちょっと重たいのでフィルター
			if ( iPosition >= 1 ) {
				if ( isLancelotTartare(iBushouID) ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// バールゼフォンだ
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "") == 0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// ヴォラック
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// バルバス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// マルティム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "㌘㌧" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "㌔㌢㍍" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// ハボリム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// オズマ
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ёжз" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ийк" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "総長";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "将校";
								return int(szMibun);
							}
						}
					}
				}
				// ベルサリア
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "女王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "副総長";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "参謀";
								return int(szMibun);
							}
						}
					}
				}
				// ウォーレン
				else if (strcmp( nb6bushouname[iBushouID].familyname,"")==0 &&  strcmp( nb6bushouname[iBushouID].fastname, "")==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "賢王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "参謀";
								return int(szMibun);
							}
						}
					}
				}
				// カノープス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "獣王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							}
						}
					}
				}
				// ギルバルド
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "獣王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							}
						}
					}
				}
				// ミルディン
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "武王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							}
						}
					}
				}
				// ギルダス
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "шщъ" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "武王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							}
						}
					}
				}
				// デネブ
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "女王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "参謀";
								return int(szMibun);
							}
						}
					}
				}
				// ラドラム
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // 大名ならば…
						static char szMibun[] = "賢王";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // 軍団長ならば…
						static char szMibun[] = "団長";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// 自分が軍師だ。
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "軍師";
									return int(szMibun);
								} else {
									static char szMibun[] = "軍師";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "参謀";
								return int(szMibun);
							}
						}
					}
				}
			}
		}
		*/
		

		int iDaimyoID = nb6bushouref[iBushouID].attach - 1;
		if ( 0 <= iDaimyoID && iDaimyoID < GAMEDATASTRUCT_DAIMYO_NUM ) {
			int iSimbol = nb6daimyo[iDaimyoID].symbol;
			if ( iSimbol == 41 && nb6bushouref[iBushouID].job == 0x40) { // 本願寺家紋帰属でかつ、僧侶
				if ( iPosition == 1 ) { // 大名相当
					static char szJotai[] = "顕座";
					return int(szJotai);
				} else if ( iPosition == 2 ) { // 宿老相当
					if ( nb6bushouref[iBushouID].blood == 41) { // 本願寺の血であれば
						static char szJotai[] = "親座";
						return int(szJotai);
					} else {
						static char szJotai[] = "直座";
						return int(szJotai);
					}
				} else if ( iPosition == 3 ) { // 家老相当
					static char szJotai[] = "特座";
					return int(szJotai);
				} else if ( iPosition == 4 ) { // 部将相当
					static char szJotai[] = "正座";
					return int(szJotai);
				} else if ( iPosition == 5 ) { // 侍大将相当
					static char szJotai[] = "上座";
					return int(szJotai);
				} else if ( iPosition == 6 ) { // 足軽頭相当
					if ( nb6bushouref[iBushouID].merits != 0 ) {
						static char szJotai[] = "本座";
						return int(szJotai);
					// 勲功が０
					} else {
						static char szJotai[] = "例座";
						return int(szJotai);
					}
				}
			} 
		}


	}


	return NULL;
}








//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingMibun1 = -1;
int iEAXPointerOfShowingMibun1 = NULL;
void OnTenshouExeShowingMibun1Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun1 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun1 = ipointer;
		}
	}
}



/*
00403B94  |. 56                   PUSH    ESI			← ここは他の同じパターンとは違ってESIになっている。他の場所はEAX
00403B95  |. E8 3E680000          CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
00403B9A  |. 83C4 04              ADD     ESP, 4
00403B9D  |. 8B0485 B0134D00      MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
00403BA4  |. A3 70DF4D00          MOV     DWORD PTR DS:[4DDF70], EAX


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun1		=0x403B95; // 関数はこのアドレスから、OnTenshouExeShowingMibun1へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun1	=0x403BA4; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun1() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun1, esi

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun1, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun1Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun1

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun1
	}
}

char cmdOnTenshouExeJumpFromShowingMibun1[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun1() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun1 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun1+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun1), cmdOnTenshouExeJumpFromShowingMibun1, 5, NULL); //5バイトのみ書き込む
}








//---------------------------------------------------------------------------------------------------------------------------











/*
 * 身分を表示するときに、武将番号を拾っておく
 * ECX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingMibun2 = -1;
int iEAXPointerOfShowingMibun2 = NULL;
void OnTenshouExeShowingMibun2Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun2 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun2 = ipointer;
		}
	}
}


/*
00438F22   . E8 B114FDFF    CALL    TENSHOU.0040A3D8
00438F27   . 83C4 04        ADD     ESP, 4
00438F2A   . 8B0485 B0134D00                     MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ;  TENSHOU.004C8EF0
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
00438F31   . EB 0F                               JMP     SHORT TENSHOU.00438F42
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun2		=0x438F22; // 関数はこのアドレスから、OnTenshouExeShowingMibun2へとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun2	=0x438F31; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun2() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun2, esi; // ESI==004D00BDみたいに下半分のビットが武将番号

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun2, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun2Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun2

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun2
	}
}

char cmdOnTenshouExeJumpFromShowingMibun2[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun2() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun2 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun2+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun2), cmdOnTenshouExeJumpFromShowingMibun2, 5, NULL); //5バイトのみ書き込む
}

















//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingMibun3 = -1;
int iEAXPointerOfShowingMibun3 = NULL;
void OnTenshouExeShowingMibun3Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun3 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun3 = ipointer;
		}
	}
}



/*
0045E420   . E8 B3BFFAFF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
0045E425   . 83C4 04        ADD     ESP, 4
0045E428   . 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
0045E42F   > 66:8B1F        MOV     BX, WORD PTR DS:[EDI]


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun3		=0x45E420; // 関数はこのアドレスから、OnTenshouExeShowingMibun3へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun3	=0x45E42F; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun3() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun3, eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun3, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun3Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun3

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun3
	}
}

char cmdOnTenshouExeJumpFromShowingMibun3[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計８バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun3() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun3 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun3+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun3), cmdOnTenshouExeJumpFromShowingMibun3, 5, NULL); //5バイトのみ書き込む
}







//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun4 = -1;
int iMibunNumberOfShowingMibun4 = -1;
int iEAXPointerOfShowingMibun4 = NULL;
void OnTenshouExeShowingMibun4Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun4 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun4 = ipointer;
		}
	}
}



/*
00460026  |. 55             PUSH    EBP
00460027  |. E8 ACA3FAFF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
0046002C  |. 83C4 04        ADD     ESP, 4
0046002F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; |Arg5
00460036  |. 6A 14          PUSH    14                               ; |Arg4 = 00000014


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun4		=0x460027; // 関数はこのアドレスから、OnTenshouExeShowingMibun4へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun4	=0x460036; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun4() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun4, EBP

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 身分番号を持ってるEAXの値を保持しておく
		mov iMibunNumberOfShowingMibun4, eax

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun4, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun4Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun4

		PUSH    EAX

		// EAXを身分の値へと戻す
		mov EAX, iMibunNumberOfShowingMibun4

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun4
	}
}

char cmdOnTenshouExeJumpFromShowingMibun4[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun4() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun4;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun4 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun4+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun4), cmdOnTenshouExeJumpFromShowingMibun4, 5, NULL); //5バイトのみ書き込む
}










//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun5 = -1;
int iMibunNumberOfShowingMibun5 = -1;
int iEAXPointerOfShowingMibun5 = NULL;
void OnTenshouExeShowingMibun5Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun5 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun5 = ipointer;
		}
	}
}



/*
00463E66  |. 56             PUSH    ESI
00463E67  |. E8 6C65FAFF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
00463E6C  |. 83C4 04        ADD     ESP, 4
00463E6F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
00463E76  |. 56             PUSH    ESI



*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun5		=0x463E67; // 関数はこのアドレスから、OnTenshouExeShowingMibun5へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun5	=0x463E76; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun5() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun5, ESI

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 身分番号を持ってるEAXの値を保持しておく
		mov iMibunNumberOfShowingMibun5, eax

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun5, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun5Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun5

		PUSH    EAX

		// EAXを身分の値へと戻す
		mov EAX, iMibunNumberOfShowingMibun5

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun5
	}
}

char cmdOnTenshouExeJumpFromShowingMibun5[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun5() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun5;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun5 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun5+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun5), cmdOnTenshouExeJumpFromShowingMibun5, 5, NULL); //5バイトのみ書き込む
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun6 = -1;
void OnTenshouExeShowingMibun6_BeforeExecute() {
}



/*
0048F93F   . 50             PUSH    EAX
0048F940   . E8 93AAF7FF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
0048F945   . 83C4 04        ADD     ESP, 4


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before		=0x48F940; // 関数はこのアドレスから、OnTenshouExeShowingMibun6_Beforeへとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun6_Before   =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6_Before	=0x48F945; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6_Before() {
	// スタックにためておく
	__asm {
		// 武将番号だけ拾って元へと戻す
		mov iBushouBangouOfShowingMibun6, EAX

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun6_Before

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun6_BeforeExecute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun6_Before
	}
}

char cmdOnTenshouExeJumpFromShowingMibun6_Before[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun6_Before() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun6_Before;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6_Before+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before), cmdOnTenshouExeJumpFromShowingMibun6_Before, 5, NULL); //5バイトのみ書き込む
}



//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iMibunNumberOfShowingMibun6 = -1;
int iEAXPointerOfShowingMibun6 = NULL;
void OnTenshouExeShowingMibun6Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun6 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun6 = ipointer;
		}
	}
}



/*
0048F94B   . FF3485 B0134D00         PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; /Arg2
				└ここを JMP TSMod.OnTSExeShowingMibun6 と書き換えてやる実際の処理
0048F952   . B8 8A1F0000             MOV     EAX, 1F8A                        ; |
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6		=0x48F94B; // 関数はこのアドレスから、OnTenshouExeShowingMibun6へとジャンプしてくる。
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6	=0x48F952; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6() {
	// スタックにためておく
	__asm {

		mov iMibunNumberOfShowingMibun6, EAX

			// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun6, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun6Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun6

		PUSH    EAX

		// EAXを身分の値へと戻す
		mov EAX, iMibunNumberOfShowingMibun6

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun6
	}
}

char cmdOnTenshouExeJumpFromShowingMibun6[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が7バイト、以後の関数で生まれる命令が合計8バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun6() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun6;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6), cmdOnTenshouExeJumpFromShowingMibun6, 7, NULL); //7バイトのみ書き込む
}




//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun7 = -1;
int iMibunNumberOfShowingMibun7 = -1;
int iEAXPointerOfShowingMibun7 = NULL;
void OnTenshouExeShowingMibun7Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun7 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun7 = ipointer;
		}
	}
}



/*
004935DA  |. 56             PUSH    ESI
004935DB  |. E8 F86DF7FF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
004935E0  |. 83C4 04        ADD     ESP, 4
004935E3  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004935EA  |. 68 DC2E4D00    PUSH    TENSHOU.004D2EDC                 ;  ASCII "%-6s"
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun7		=0x4935DB; // 関数はこのアドレスから、OnTenshouExeShowingMibun7へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun7	=0x4935EA; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun7() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun7, ESI

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 身分番号を持ってるEAXの値を保持しておく
		mov iMibunNumberOfShowingMibun7, eax

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun7, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun7Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun7

		PUSH    EAX

		// EAXを身分の値へと戻す
		mov EAX, iMibunNumberOfShowingMibun7

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun7
	}
}

char cmdOnTenshouExeJumpFromShowingMibun7[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun7() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun7;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun7 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun7+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun7), cmdOnTenshouExeJumpFromShowingMibun7, 5, NULL); //5バイトのみ書き込む
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingMibun8 = -1;
int iEAXPointerOfShowingMibun8 = NULL;
void OnTenshouExeShowingMibun8Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun8 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun8 = ipointer;
		}
	}
}



/*
004A155E  |. E8 758EF6FF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
004A1563  |. 83C4 04        ADD     ESP, 4
004A1566  |. 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]           ;  TENSHOU.004CFEE4
004A156D  |> 66:8B36        MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun8		=0x4A155E; // 関数はこのアドレスから、OnTenshouExeShowingMibun8へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun8	=0x4A156D; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun8() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun8, eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun8, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun8Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun8

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun8
	}
}

char cmdOnTenshouExeJumpFromShowingMibun8[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun8() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun8;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun8 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun8+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun8), cmdOnTenshouExeJumpFromShowingMibun8, 5, NULL); //5バイトのみ書き込む
}












//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 * EAX==004D00BDみたいに下半分のビットが武将番号
 */
int iBushouBangouOfShowingMibun9 = -1;
int iEAXPointerOfShowingMibun9 = NULL;
void OnTenshouExeShowingMibun9Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun9 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun9 = ipointer;
		}
	}
}



/*
004A1C2B  |. E8 A887F6FF               CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
004A1C30  |. 83C4 04                   ADD     ESP, 4
004A1C33  |. 8B0485 B0134D00           MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
004A1C3A  |> 66:8B36                   MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun9		=0x4A1C2B; // 関数はこのアドレスから、OnTenshouExeShowingMibun9へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun9	=0x4A1C3A; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun9() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun9, eax

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun9, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun9Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun9

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun9
	}
}

char cmdOnTenshouExeJumpFromShowingMibun9[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun9() {
	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun9;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun9 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun9+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun9), cmdOnTenshouExeJumpFromShowingMibun9, 5, NULL); //5バイトのみ書き込む
}





//---------------------------------------------------------------------------------------------------------------------------


/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun10 = -1;
int iMibunNumberOfShowingMibun10 = -1;
int iEAXPointerOfShowingMibun10 = NULL;
void OnTenshouExeShowingMibun10Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun10 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun10 = ipointer;
		}
	}
}



/*
004A4B87  |. 56             PUSH    ESI
004A4B88  |. E8 4B58F6FF    CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibunBefore と書き換えてやる実際の処理
004A4B8D  |. 83C4 04        ADD     ESP, 4
004A4B90  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004A4B97  |. 56             PUSH    ESI
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun10		=0x4A4B88; // 関数はこのアドレスから、OnTenshouExeShowingMibun10へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10 =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun10	=0x4A4B97; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun10() {
	// スタックにためておく
	__asm {

		mov iBushouBangouOfShowingMibun10, ESI

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 身分番号を持ってるEAXの値を保持しておく
		mov iMibunNumberOfShowingMibun10, eax

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13B0]

		mov iEAXPointerOfShowingMibun10, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun10Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		mov EAX, iEAXPointerOfShowingMibun10

		PUSH    EAX

		// EAXを身分の値へと戻す
		mov EAX, iMibunNumberOfShowingMibun10

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun10
	}
}

char cmdOnTenshouExeJumpFromShowingMibun10[6]="\xE9";
// 元の命令が5バイト、以後の関数で生まれる命令が合計6バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun10() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun10;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun10 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun10+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun10), cmdOnTenshouExeJumpFromShowingMibun10, 5, NULL); //5バイトのみ書き込む
}










/*
004300DA  |. 83C4 04        |ADD     ESP, 4
								// 昇進は今の身分の次の身分を出す必要があるので(4D13B0 + (EAX-1)*4) => EAX*4+4D13ACという計算式となっている。
004300DD  |. 8B0485 AC134D0>|MOV     EAX, DWORD PTR DS:[EAX*4+4D13AC]
004300E4  |> 68 88000000    |PUSH    88                                                  ; /Arg7 = 00000088
004300E9  |. 83C3 02        |ADD     EBX, 2                                              ; |
004300EC  |. FF75 F0        |PUSH    DWORD PTR SS:[EBP-10]                               ; |Arg6
004300EF  |. 50             |PUSH    EAX                                                 ; |Arg5
004300F0  |. 6A 10          |PUSH    10                                                  ; |Arg4 = 00000010
004300F2  |. FF75 E8        |PUSH    DWORD PTR SS:[EBP-18]                               ; |Arg3
004300F5  |. FF75 F4        |PUSH    DWORD PTR SS:[EBP-C]                                ; |Arg2
004300F8  |. 68 88000000    |PUSH    88                                                  ; |Arg1 = 00000088
004300FD  |. E8 DE580500    |CALL    TENSHOU.004859E0                                    ; \TENSHOU.004859E0
00430102  |. 83C4 1C        |ADD     ESP, 1C
00430105  |. FF45 F8        |INC     DWORD PTR SS:[EBP-8]
00430108  |. 8345 F4 18     |ADD     DWORD PTR SS:[EBP-C], 18
0043010C  |. 8B4D F8        |MOV     ECX, DWORD PTR SS:[EBP-8]
0043010F  |. 398E AC000000  |CMP     DWORD PTR DS:[ESI+AC], ECX
00430115  |.^0F87 66FFFFFF  \JA      TENSHOU.00430081
*/




/*
 * 身分を表示するときに、武将番号を拾っておく
 */
int iBushouBangouOfShowingMibun11 = -1;
int iMibunNumberOfShowingMibun11 = -1;
int iEAXPointerOfShowingMibun11 = NULL;
void OnTenshouExeShowingMibun11Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun11 & 0xFFFF) - 1; // 下半分が武将番号

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int inext_position = nb6bushouname[iBushouID].position-1;
		int ipointer = GetOverRewriteMibunAddress(iBushouID, inext_position);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun11 = ipointer;
		}
	}
}



/*
004300D2  |. FF75 FE        |PUSH    DWORD PTR SS:[EBP-2]
004300D5  |. E8 FEA2FDFF    |CALL    TENSHOU.0040A3D8
				└ここを JMP TSMod.OnTSExeShowingMibun11 と書き換えてやる実際の処理
004300DA  |. 83C4 04        |ADD     ESP, 4
004300DD  |. 8B0485 AC134D0>|MOV     EAX, DWORD PTR DS:[EAX*4+4D13AC]
004300E4  |> 68 88000000    |PUSH    88                                                  ; /Arg7 = 00000088

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun11		 =0x4300D5; // 関数はこのアドレスから、OnTenshouExeShowingMibun11へとジャンプしてくる。
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11  =0x40A3D8; // 元々TENSHOU.EXE内にあったCALL先
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun11	 =0x4300E4; // 関数が最後までいくと、このTENSHOU.EXE内に直接ジャンプするというなんとも危険な行為w
// この関数はTENSHOU.EXEがメッセージを読みを終えるたびに、直接実行される。
// TENSHOU.EXE内でメッセージが構築されるタイミングでこのnaked関数が呼ばれる。
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun11() {
	// スタックにためておく
	__asm {

		// 元々TENSHOU.EXE内にあったものをここで
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11

		// 元々TENSHOU.EXE内にあったものをここで
		ADD     ESP, 4

		// 身分番号を持ってるEAXの値を保持しておく
		mov iMibunNumberOfShowingMibun11, eax

		// 武将番号を引き出すためにEAXの間借り
		mov eax, [EBP-0x2] // 武将の番号
		mov iBushouBangouOfShowingMibun11, eax

		// eaxを身分番号に戻す
		mov eax, iMibunNumberOfShowingMibun11

		// 元々TENSHOU.EXE内にあったものをここで
		// (右辺値のEAXとは身分の値、「EAX*4+0x4D13B0」が各身分の文字列へのポインタが入っているアドレスですね、左辺値のEAXで受け取っている)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13AC]  // 昇進は今の身分の次の身分を出す必要があるので(4D13B0 + (EAX-1)*4) => EAX*4+4D13ACという計算式となっている。

		mov iEAXPointerOfShowingMibun11, EAX

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}
	
	// 
	OnTenshouExeShowingMibun11Execute();

	// スタックに引き出す
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// 上書きする必要があるならば上書きする。
		mov EAX, iEAXPointerOfShowingMibun11

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun11
	}
}

char cmdOnTenshouExeJumpFromShowingMibun11[8]="\xE9\x90\x90\x90\x90\x90\x90";
// 元の命令が7バイト、以後の関数で生まれる命令が合計8バイトなので… 最後１つ使わない


// ニーモニック書き換え用
void WriteAsmJumperOnTenshouExeShowingMibun11() {

	// まずアドレスを数字として扱う
	int iAddress = (int)OnTenshouExeShowingMibun11;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun11 + 5 );
		// ５というのは、0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  の命令に必要なバイト数。要するに５バイト足すと次のニーモニック命令群に移動するのだ。そしてそこからの差分がジャンプする際の目的格として利用される。
	memcpy(cmdOnTenshouExeJumpFromShowingMibun11+1, &SubAddress, 4); // +1 はE9の次から4バイト分書き換えるから。

	// 構築したニーモニック命令をTENSHOU.EXEのメモリに書き換える
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun11), cmdOnTenshouExeJumpFromShowingMibun11, 7, NULL); //7バイトのみ書き込む
}









