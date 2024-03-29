
#include "CommonGlobalHandle.h"
#include "CurProcessMemory.h"
#include "CurProcessGameMemory.h"
#include "GameDataStruct.h"
#include "TSModCommand.h"
#include "EXBushou_All.h"
#include "ScenarioMod.h"


/*
004CFEC4  91 AB 8C 79 93 AA 00 00 8E 98 91 E5 8F AB 00 00  «yª..å«..
004CFED4  95 94 8F AB 00 00 00 00 89 C6 98 56 00 00 00 00  «....ÆV....
004CFEE4  8F 68 98 56 00 00 00 00                          hV.....

004D13A8  F0 FE 4C 00 EC FE 4C 00 [A0 FF 4C] 00 [F0 8E 4C] 00   B å¼
004D13B8  [E4 FE 4C] 00 [DC FE 4]v 00 [D4 FE 4C] 00 [CC FE 4C]  hV`å«
004D13C8  [C4 FE 4C] 00 00 00 00 00 F0 8E 4C 00 E0 8E 4C 00  «yª


±ÄÞÚ½      ÃÞ¨½±¾ÝÌÞÙ                                                                                         ºÒÝÄ
@00403B9D   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   ÀÏ  Q[àÌÀsêÍs¾B±±Í«ÔªEAXÅÍÈ­ESI
A00438F2A   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]	ÀÏ@ÊíÌ«XgÌÉ«²ÆÉ±±ðÊß·éB
B0045E428   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]   ÀÏ@ï©pÌ_CAO
C0046002F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ÀÏ RÌÒ¬(ºÈÒ¬) 
D00463E6F   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ÀÏ íÌ¶ãÌuàv©çÌîñ
E0048F94B   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ¢À ¸iÌ_CAOÌºÅuÉ¸iµÜµ½vÌbZ[W¤ÌÆ±ë
F004935E3   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ÀÏ «îñÚ×æÊ
G004A1566   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ÀÏ «Ì¾ÌÛÉA±±ðÊß·éB
H004A1C33   MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0] ÀÏ «Ìug¾¯vÌ¬«oµ_CAOÌÉA±±ðÊß·éB(ÇúÈÇ)
I004A4B90   PUSH    DWORD PTR DS:[EAX*4+4D13B0] ÀÏ Q[àÌÀsêÍs¾B
*/




static int GetOverRewriteMibunAddress(int iBushouID, int iPosition=-1 ) {

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {

		// ftHgÍ»ÝÌ|WVªüé
		if ( iPosition==-1) {
			iPosition = nb6bushouname[iBushouID].position;
		}

		// ScenarioModÌÄÑoµB( return µÄ¢éÌÅScenarioModðæÉüêé)
		if ( p_snOnViewingPositionName ) {
			int iPointerAddress = 0;
			p_snOnViewingPositionName( &iPointerAddress, iBushouID+1, iPosition );
			if ( iPointerAddress ) {
				return int(iPointerAddress);
			}
		}

		
		/*
		if ( TSModCommand::UltraBushou::cmdExists ) { // ÈºA¿åÁÆd½¢ÌÅtB^[
			if ( iPosition >= 1 ) {
				if ( isLancelotTartare(iBushouID) ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// o[[tH¾
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðIðJðK") == 0 && strcmp( nb6bushouname[iBushouID].fastname, "ðLðMðN") == 0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// HbN
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðsðtðu" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðvðwðx" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// ooX
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðUðVðW" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðXðYðZ" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// }eB
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "cd" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "`ab" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// n{
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðIðJðK" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðdðeðf" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// IY}
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "vwx" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "yz{" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "·";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "«Z";
								return int(szMibun);
							}
						}
					}
				}
				// xTA
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ð^ð_ð`" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðaðbðc" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "·";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "Qd";
								return int(szMibun);
							}
						}
					}
				}
				// EH[
				else if (strcmp( nb6bushouname[iBushouID].familyname,"ðqðr")==0 &&  strcmp( nb6bushouname[iBushouID].fastname, "ðnðoðp")==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "«¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "Qd";
								return int(szMibun);
							}
						}
					}
				}
				// Jm[vX
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðOðu" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ð[ð\ð]" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "b¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							}
						}
					}
				}
				// Moh
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðgðhði" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ððð" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "b¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							}
						}
					}
				}
				// ~fB
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðOðPðQ" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðRðSðT" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							}
						}
					}
				}
				// M_X
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðGðHðF" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							}
						}
					}
				}
				// flu
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðjðk" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ðlðm" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "Qd";
								return int(szMibun);
							}
						}
					}
				}
				// h
				else if ( strcmp( nb6bushouname[iBushouID].familyname, "ðyðzð{" )==0 && strcmp( nb6bushouname[iBushouID].fastname, "ð|ð}ð~" )==0 ) {
					if ( nb6bushouname[iBushouID].State == 0 ) { // å¼ÈçÎc
						static char szMibun[] = "«¤";
						return int(szMibun);
					} else if ( nb6bushouname[iBushouID].State == 1 ) { // Rc·ÈçÎc
						static char szMibun[] = "c·";
						return int(szMibun);
					} else {
						int iGundanID = nb6bushouref[iBushouID].gundan - 1;
						if ( 0 <= iGundanID && iGundanID < GAMEDATASTRUCT_GUNDAN_NUM ) {
							int iGunshiBushouID = getGunshiBushouID(iGundanID);
							// ©ªªRt¾B
							if ( iGunshiBushouID == iBushouID ) {
								if ( nb6gundan[iGundanID].number == 1 ) {
									static char szMibun[] = "Rt";
									return int(szMibun);
								} else {
									static char szMibun[] = "Rt";
									return int(szMibun);
								}
							} else {
								static char szMibun[] = "Qd";
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
			if ( iSimbol == 41 && nb6bushouref[iBushouID].job == 0x40) { // {èÆäA®Å©ÂAmµ
				if ( iPosition == 1 ) { // å¼
					static char szJotai[] = "°À";
					return int(szJotai);
				} else if ( iPosition == 2 ) { // hV
					if ( nb6bushouref[iBushouID].blood == 41) { // {èÌÅ êÎ
						static char szJotai[] = "eÀ";
						return int(szJotai);
					} else {
						static char szJotai[] = "¼À";
						return int(szJotai);
					}
				} else if ( iPosition == 3 ) { // ÆV
					static char szJotai[] = "ÁÀ";
					return int(szJotai);
				} else if ( iPosition == 4 ) { // «
					static char szJotai[] = "³À";
					return int(szJotai);
				} else if ( iPosition == 5 ) { // å«
					static char szJotai[] = "ãÀ";
					return int(szJotai);
				} else if ( iPosition == 6 ) { // «yª
					if ( nb6bushouref[iBushouID].merits != 0 ) {
						static char szJotai[] = "{À";
						return int(szJotai);
					// M÷ªO
					} else {
						static char szJotai[] = "áÀ";
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
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 * EAX==004D00BDÝ½¢Éº¼ªÌrbgª«Ô
 */
int iBushouBangouOfShowingMibun1 = -1;
int iEAXPointerOfShowingMibun1 = NULL;
void OnTenshouExeShowingMibun1Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun1 & 0xFFFF) - 1; // º¼ªª«Ô

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun1 = ipointer;
		}
	}
}



/*
00403B94  |. 56                   PUSH    ESI			© ±±Í¼Ì¯¶p^[ÆÍáÁÄESIÉÈÁÄ¢éB¼ÌêÍEAX
00403B95  |. E8 3E680000          CALL    TENSHOU.0040A3D8
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
00403B9A  |. 83C4 04              ADD     ESP, 4
00403B9D  |. 8B0485 B0134D00      MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
00403BA4  |. A3 70DF4D00          MOV     DWORD PTR DS:[4DDF70], EAX


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun1		=0x403B95; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun1ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun1	=0x403BA4; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun1() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun1, esi

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun1() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun1;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun1 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun1+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun1), cmdOnTenshouExeJumpFromShowingMibun1, 5, NULL); //5oCgÌÝ«Þ
}








//---------------------------------------------------------------------------------------------------------------------------











/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 * ECX==004D00BDÝ½¢Éº¼ªÌrbgª«Ô
 */
int iBushouBangouOfShowingMibun2 = -1;
int iEAXPointerOfShowingMibun2 = NULL;
void OnTenshouExeShowingMibun2Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun2 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
00438F31   . EB 0F                               JMP     SHORT TENSHOU.00438F42
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun2		=0x438F22; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun2ÖÆWvµÄ­éB
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun2	=0x438F31; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun2() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun2, esi; // ESI==004D00BDÝ½¢Éº¼ªÌrbgª«Ô

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun1

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun2() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun2;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun2 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun2+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun2), cmdOnTenshouExeJumpFromShowingMibun2, 5, NULL); //5oCgÌÝ«Þ
}

















//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 * EAX==004D00BDÝ½¢Éº¼ªÌrbgª«Ô
 */
int iBushouBangouOfShowingMibun3 = -1;
int iEAXPointerOfShowingMibun3 = NULL;
void OnTenshouExeShowingMibun3Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun3 & 0xFFFF) - 1; // º¼ªª«Ô

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun3 = ipointer;
		}
	}
}



/*
0045E420   . E8 B3BFFAFF    CALL    TENSHOU.0040A3D8
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
0045E425   . 83C4 04        ADD     ESP, 4
0045E428   . 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
0045E42F   > 66:8B1F        MOV     BX, WORD PTR DS:[EDI]


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun3		=0x45E420; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun3ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun3	=0x45E42F; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun3() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun3, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun3

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªvWoCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun3() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun3;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun3 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun3+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun3), cmdOnTenshouExeJumpFromShowingMibun3, 5, NULL); //5oCgÌÝ«Þ
}







//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun4 = -1;
int iMibunNumberOfShowingMibun4 = -1;
int iEAXPointerOfShowingMibun4 = NULL;
void OnTenshouExeShowingMibun4Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun4 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
0046002C  |. 83C4 04        ADD     ESP, 4
0046002F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; |Arg5
00460036  |. 6A 14          PUSH    14                               ; |Arg4 = 00000014


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun4		=0x460027; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun4ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun4	=0x460036; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun4() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun4, EBP

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// gªÔðÁÄéEAXÌlðÛµÄ¨­
		mov iMibunNumberOfShowingMibun4, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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

		// EAXðgªÌlÖÆß·
		mov EAX, iMibunNumberOfShowingMibun4

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun4
	}
}

char cmdOnTenshouExeJumpFromShowingMibun4[6]="\xE9";
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun4() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun4;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun4 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun4+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun4), cmdOnTenshouExeJumpFromShowingMibun4, 5, NULL); //5oCgÌÝ«Þ
}










//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun5 = -1;
int iMibunNumberOfShowingMibun5 = -1;
int iEAXPointerOfShowingMibun5 = NULL;
void OnTenshouExeShowingMibun5Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun5 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
00463E6C  |. 83C4 04        ADD     ESP, 4
00463E6F  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
00463E76  |. 56             PUSH    ESI



*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun5		=0x463E67; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun5ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun5	=0x463E76; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun5() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun5, ESI

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun5

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// gªÔðÁÄéEAXÌlðÛµÄ¨­
		mov iMibunNumberOfShowingMibun5, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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

		// EAXðgªÌlÖÆß·
		mov EAX, iMibunNumberOfShowingMibun5

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun5
	}
}

char cmdOnTenshouExeJumpFromShowingMibun5[6]="\xE9";
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun5() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun5;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun5 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun5+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun5), cmdOnTenshouExeJumpFromShowingMibun5, 5, NULL); //5oCgÌÝ«Þ
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun6 = -1;
void OnTenshouExeShowingMibun6_BeforeExecute() {
}



/*
0048F93F   . 50             PUSH    EAX
0048F940   . E8 93AAF7FF    CALL    TENSHOU.0040A3D8
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
0048F945   . 83C4 04        ADD     ESP, 4


*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before		=0x48F940; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun6_BeforeÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun6_Before   =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6_Before	=0x48F945; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6_Before() {
	// X^bNÉ½ßÄ¨­
	__asm {
		// «Ô¾¯EÁÄ³ÖÆß·
		mov iBushouBangouOfShowingMibun6, EAX

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun6_Before() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun6_Before;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6_Before+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6_Before), cmdOnTenshouExeJumpFromShowingMibun6_Before, 5, NULL); //5oCgÌÝ«Þ
}



//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iMibunNumberOfShowingMibun6 = -1;
int iEAXPointerOfShowingMibun6 = NULL;
void OnTenshouExeShowingMibun6Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun6 & 0xFFFF) - 1; // º¼ªª«Ô

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun6 = ipointer;
		}
	}
}



/*
0048F94B   . FF3485 B0134D00         PUSH    DWORD PTR DS:[EAX*4+4D13B0]      ; /Arg2
				¤±±ð JMP TSMod.OnTSExeShowingMibun6 Æ«·¦ÄâéÀÛÌ
0048F952   . B8 8A1F0000             MOV     EAX, 1F8A                        ; |
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun6		=0x48F94B; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun6ÖÆWvµÄ­éB
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun6	=0x48F952; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun6() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iMibunNumberOfShowingMibun6, EAX

			// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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

		// EAXðgªÌlÖÆß·
		mov EAX, iMibunNumberOfShowingMibun6

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun6
	}
}

char cmdOnTenshouExeJumpFromShowingMibun6[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ³Ì½ßª7oCgAÈãÌÖÅ¶Üêé½ßªv8oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun6() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun6;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun6 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun6+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun6), cmdOnTenshouExeJumpFromShowingMibun6, 7, NULL); //7oCgÌÝ«Þ
}




//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun7 = -1;
int iMibunNumberOfShowingMibun7 = -1;
int iEAXPointerOfShowingMibun7 = NULL;
void OnTenshouExeShowingMibun7Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun7 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
004935E0  |. 83C4 04        ADD     ESP, 4
004935E3  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004935EA  |. 68 DC2E4D00    PUSH    TENSHOU.004D2EDC                 ;  ASCII "%-6s"
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun7		=0x4935DB; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun7ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun7	=0x4935EA; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun7() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun7, ESI

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun7

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// gªÔðÁÄéEAXÌlðÛµÄ¨­
		mov iMibunNumberOfShowingMibun7, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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

		// EAXðgªÌlÖÆß·
		mov EAX, iMibunNumberOfShowingMibun7

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun7
	}
}

char cmdOnTenshouExeJumpFromShowingMibun7[6]="\xE9";
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun7() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun7;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun7 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun7+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun7), cmdOnTenshouExeJumpFromShowingMibun7, 5, NULL); //5oCgÌÝ«Þ
}






//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 * EAX==004D00BDÝ½¢Éº¼ªÌrbgª«Ô
 */
int iBushouBangouOfShowingMibun8 = -1;
int iEAXPointerOfShowingMibun8 = NULL;
void OnTenshouExeShowingMibun8Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun8 & 0xFFFF) - 1; // º¼ªª«Ô

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun8 = ipointer;
		}
	}
}



/*
004A155E  |. E8 758EF6FF    CALL    TENSHOU.0040A3D8
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
004A1563  |. 83C4 04        ADD     ESP, 4
004A1566  |. 8B0485 B0134D0>MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]           ;  TENSHOU.004CFEE4
004A156D  |> 66:8B36        MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun8		=0x4A155E; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun8ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun8	=0x4A156D; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun8() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun8, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun8

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun8() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun8;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun8 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun8+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun8), cmdOnTenshouExeJumpFromShowingMibun8, 5, NULL); //5oCgÌÝ«Þ
}












//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 * EAX==004D00BDÝ½¢Éº¼ªÌrbgª«Ô
 */
int iBushouBangouOfShowingMibun9 = -1;
int iEAXPointerOfShowingMibun9 = NULL;
void OnTenshouExeShowingMibun9Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun9 & 0xFFFF) - 1; // º¼ªª«Ô

	if ( 0 <= iBushouID && iBushouID < GAMEDATASTRUCT_BUSHOU_NUM ) {
		int ipointer = GetOverRewriteMibunAddress(iBushouID);

		if ( ipointer ) {
			iEAXPointerOfShowingMibun9 = ipointer;
		}
	}
}



/*
004A1C2B  |. E8 A887F6FF               CALL    TENSHOU.0040A3D8
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
004A1C30  |. 83C4 04                   ADD     ESP, 4
004A1C33  |. 8B0485 B0134D00           MOV     EAX, DWORD PTR DS:[EAX*4+4D13B0]
004A1C3A  |> 66:8B36                   MOV     SI, WORD PTR DS:[ESI]

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun9		=0x4A1C2B; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun9ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun9	=0x4A1C3A; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun9() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun9, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun9

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun9() {
	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun9;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun9 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun9+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun9), cmdOnTenshouExeJumpFromShowingMibun9, 5, NULL); //5oCgÌÝ«Þ
}





//---------------------------------------------------------------------------------------------------------------------------


/*
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun10 = -1;
int iMibunNumberOfShowingMibun10 = -1;
int iEAXPointerOfShowingMibun10 = NULL;
void OnTenshouExeShowingMibun10Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun10 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibunBefore Æ«·¦ÄâéÀÛÌ
004A4B8D  |. 83C4 04        ADD     ESP, 4
004A4B90  |. FF3485 B0134D0>PUSH    DWORD PTR DS:[EAX*4+4D13B0]
004A4B97  |. 56             PUSH    ESI
*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun10		=0x4A4B88; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun10ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10 =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun10	=0x4A4B97; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun10() {
	// X^bNÉ½ßÄ¨­
	__asm {

		mov iBushouBangouOfShowingMibun10, ESI

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun10

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// gªÔðÁÄéEAXÌlðÛµÄ¨­
		mov iMibunNumberOfShowingMibun10, eax

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
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

	// X^bNÉø«o·
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

		// EAXðgªÌlÖÆß·
		mov EAX, iMibunNumberOfShowingMibun10

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun10
	}
}

char cmdOnTenshouExeJumpFromShowingMibun10[6]="\xE9";
// ³Ì½ßª5oCgAÈãÌÖÅ¶Üêé½ßªv6oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun10() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun10;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun10 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun10+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun10), cmdOnTenshouExeJumpFromShowingMibun10, 5, NULL); //5oCgÌÝ«Þ
}










/*
004300DA  |. 83C4 04        |ADD     ESP, 4
								// ¸iÍ¡ÌgªÌÌgªðo·Kvª éÌÅ(4D13B0 + (EAX-1)*4) => EAX*4+4D13ACÆ¢¤vZ®ÆÈÁÄ¢éB
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
 * gªð\¦·éÆ«ÉA«ÔðEÁÄ¨­
 */
int iBushouBangouOfShowingMibun11 = -1;
int iMibunNumberOfShowingMibun11 = -1;
int iEAXPointerOfShowingMibun11 = NULL;
void OnTenshouExeShowingMibun11Execute() {

	int iBushouID = (iBushouBangouOfShowingMibun11 & 0xFFFF) - 1; // º¼ªª«Ô

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
				¤±±ð JMP TSMod.OnTSExeShowingMibun11 Æ«·¦ÄâéÀÛÌ
004300DA  |. 83C4 04        |ADD     ESP, 4
004300DD  |. 8B0485 AC134D0>|MOV     EAX, DWORD PTR DS:[EAX*4+4D13AC]
004300E4  |> 68 88000000    |PUSH    88                                                  ; /Arg7 = 00000088

*/
int pTenshouExeJumpFromToOnTenshouExeShowingMibun11		 =0x4300D5; // ÖÍ±ÌAhX©çAOnTenshouExeShowingMibun11ÖÆWvµÄ­éB
int pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11  =0x40A3D8; // ³XTENSHOU.EXEàÉ Á½CALLæ
int pTenshouExeReturnLblFromOnTenshouExeShowingMibun11	 =0x4300E4; // ÖªÅãÜÅ¢­ÆA±ÌTENSHOU.EXEàÉ¼ÚWv·éÆ¢¤ÈñÆàë¯Ès×w
// ±ÌÖÍTENSHOU.EXEªbZ[WðÇÝðI¦é½ÑÉA¼ÚÀs³êéB
// TENSHOU.EXEàÅbZ[Wª\z³êé^C~OÅ±ÌnakedÖªÄÎêéB
__declspec( naked ) void WINAPI OnTenshouExeShowingMibun11() {
	// X^bNÉ½ßÄ¨­
	__asm {

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		call pTenshouExeJumpCallFromToOnTenshouExeShowingMibun11

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		ADD     ESP, 4

		// gªÔðÁÄéEAXÌlðÛµÄ¨­
		mov iMibunNumberOfShowingMibun11, eax

		// «Ôðø«o·½ßÉEAXÌÔØè
		mov eax, [EBP-0x2] // «ÌÔ
		mov iBushouBangouOfShowingMibun11, eax

		// eaxðgªÔÉß·
		mov eax, iMibunNumberOfShowingMibun11

		// ³XTENSHOU.EXEàÉ Á½àÌð±±Å
		// (EÓlÌEAXÆÍgªÌlAuEAX*4+0x4D13B0vªegªÌ¶ñÖÌ|C^ªüÁÄ¢éAhXÅ·ËA¶ÓlÌEAXÅó¯æÁÄ¢é)
		MOV     EAX, DWORD PTR DS:[EAX*4+0x4D13AC]  // ¸iÍ¡ÌgªÌÌgªðo·Kvª éÌÅ(4D13B0 + (EAX-1)*4) => EAX*4+4D13ACÆ¢¤vZ®ÆÈÁÄ¢éB

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

	// X^bNÉø«o·
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop edx
		pop ecx
		pop ebx
		pop eax

		// ã«·éKvª éÈçÎã«·éB
		mov EAX, iEAXPointerOfShowingMibun11

		jmp pTenshouExeReturnLblFromOnTenshouExeShowingMibun11
	}
}

char cmdOnTenshouExeJumpFromShowingMibun11[8]="\xE9\x90\x90\x90\x90\x90\x90";
// ³Ì½ßª7oCgAÈãÌÖÅ¶Üêé½ßªv8oCgÈÌÅc ÅãPÂgíÈ¢


// j[jbN«·¦p
void WriteAsmJumperOnTenshouExeShowingMibun11() {

	// Ü¸AhXðÆµÄµ¤
	int iAddress = (int)OnTenshouExeShowingMibun11;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeShowingMibun11 + 5 );
		// TÆ¢¤ÌÍA0046C194  -E9 ????????  JMP TSMod.OnTSExeShowingMibunBefore  Ì½ßÉKvÈoCgBv·éÉToCg«·ÆÌj[jbN½ßQÉÚ®·éÌ¾B»µÄ»±©çÌ·ªªWv·éÛÌÚIiÆµÄp³êéB
	memcpy(cmdOnTenshouExeJumpFromShowingMibun11+1, &SubAddress, 4); // +1 ÍE9Ì©ç4oCgª«·¦é©çB

	// \zµ½j[jbN½ßðTENSHOU.EXEÌÉ«·¦é
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeShowingMibun11), cmdOnTenshouExeJumpFromShowingMibun11, 7, NULL); //7oCgÌÝ«Þ
}









