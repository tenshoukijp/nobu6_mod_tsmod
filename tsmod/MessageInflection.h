#pragma once

#include <string>
using namespace std;

// 00（相手に敬意を払う必要がない場合の自称） 
string ％わし％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 02（その他の場合の自称） 
string ％私％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 03（所有形容詞）  
string ％我が％			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 04～07 相手をさげすむ場合「以外」の二人称
string ％相手％		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);

// 08 相手をさげすむ場合の二人称
string ％罵倒相手％	(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);

// 09～0D 三名をさげすむ場合「以外」の三人称
string ％三名％		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID);

// 0E（第三者をさげすむ場合の三人称） 
string ％罵倒三名％	(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID);