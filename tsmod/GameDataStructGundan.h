#pragma once

/*			軍団情報(21*214)

[21バイト]

0-1	前の軍団
2-3 後ろの軍団
4-5	居城名ナンバー
6-7	軍団長ナンバー(武将名前テーブルの順番)
8-9	大名
10	行動力
11-12	金
13-14	米
15-16	騎馬
17-18	鉄砲
19	？
20	aaaabbbb A 役職 B 軍団番号
0 無し 1 征夷大将軍 2 管領 3 関東管領 4 奥州探題 5 西国探題 6 九州探題 7 羽州探題 ⇒ getNormalizedGundanPosition

*/
struct NB6GUNDAN
{
	WORD prev;		//前の軍団
	WORD next;		//後ろの軍団
	WORD residence;	//居城名ナンバー
	WORD leader;	//軍団長ナンバー(武将名前テーブルの順番)
	WORD attach;	//所属(大名) (大名番号テーブルの順番)
	byte act;		//行動力
	WORD money;		//金
	WORD rise;		//米
	WORD horse;		//馬
	WORD gun;		//鉄砲
	byte _undef;	//must 0?
	byte number : 4;	//1-8 ⇒ 軍団番号
	byte position : 4;//役職  (0 無し 1 征夷大将軍 2 管領 3 関東管領 4 奥州探題 5 西国探題 6 九州探題 7 羽州探題) ⇒ getNormalizedGundanPosition
};

/// 現在(その瞬間)、天翔記に存在する軍団の「軍団番号【配列用】」をリストで得る。
vector<int> getValidGundanList();


// 同じ大名に属する軍団番号【配列用】のリストを得る
vector<int> getGovernedGundanList(int iDaimyoID);

// 軍団に所属する武将リストを得る
vector<int> getGundanBushouList(int iGundanID);

// 軍団に所属する兵数を得る
int getGundanHeisuCnt(int iGundanID);


// とある大名勢力の統治軍団割りあて番号リストを得る。
vector<int> getGovernedGundanWariateNumList(int iDaimyoID);

// とある大名配下の軍団下にある軍団「未」割当番号のリストを得る。
vector<int> getNotGovernedGundanWariateNumList(int iDaimyoID);

// 軍団任命番号の変更(入替)を試みる。失敗すればfalse
bool tryChangeGundanWariateNum(int iGundanID, int iNewWariateNum);

// 指定の軍団の所属する大名を勢力を越えて、変更する。
bool setGundanReAttachDaimyo(int iGundanID, int iNewDaimyoID);

// とある大名内でのＡ軍団の城をＢ軍団へとアタッチ変更
bool Set_ChangeCastleAttachGundan(int iCastleID, int iNewGundanID);


struct MAKEGUNDANKOUHOINFO {
	int m_iDaimyoID;                     /// <summary>どの大名についての情報なのか。</summary>
	BOOL CanMake;                            /// <summary>結局、以下の３要素から考えて、軍団の新設は可能なのか、不可能なのか。</summary>
	vector<int> vGundanIDList;        /// <summary>軍団割当番号で余っている番号のリスト。Get_軍団未割当番号リスト(...) と同じ値</summary>
	vector<int> vCastleIDList;    /// <summary>もし軍団を新設出来るとすれば、これらの城が候補となる、というリスト</summary>
	vector<int> vBushouIDList;  /// <summary>もし軍団を新設出来るとすれば、これらの武将が新たな軍団長として任命され得る、というリスト</summary>
};

/// <summary>
/// <para>とある大名家に軍団を新設可能かどうかを問い合わせる</para>
/// <para>その結果、「可不可」「どの武将が候補か」「どの城が候補か」といった情報が得られる</para>
/// <para>この関数で得られた情報を元に、 Set_軍団新設(...)関数で軍団を実際に新設することとなる。</para>
/// <para>返り値：</para>
/// <para>MAKEGUNDANKOUHOINFOの値が返ってくる。</para>
/// </summary>
/// <param name="_m_iBushouID">軍団長にしたい_m_iBushouID</param>
/// <param name="_m_iCastleID">軍団を新設したい_m_iCastleID</param>
/// <param name="安全チェック用軍団新設候補情">Get_軍団新設候補情報(...)関数の結果値を渡す必要がある。</param>
/// <returns>
/// <para>MAKEGUNDANKOUHOINFOの値が返ってくる。</para>
/// </returns>
MAKEGUNDANKOUHOINFO Get_MakeGundanKouhoInfo(int m_iDaimyoID);


/// <summary>
/// <para>とある大名家に軍団を新設する</para>
/// <para>返り値：</para>
/// <para>新設できた場合は、_iGundanIDが返ってくる</para>
/// <para>新設できなかった場合0xFFFFが返ってくる。</para>
/// <para>Get_MakeGundanKouhoInfo()の返り値に基づいて、使うことが想定されている。</para>
/// </summary>
/// <param name="_m_iBushouID">軍団長にしたい_m_iBushouID</param>
/// <param name="_m_iCastleID">軍団を新設したい_m_iCastleID</param>
/// <param name="check">Get_軍団新設候補情報(...)関数の結果値を渡す必要がある。</param>
/// <returns>
/// <para>新設できた場合は、_iGundanIDが返ってくる。</para>
/// <para>新設できなかった場合0xFFFFが返ってくる。</para>
/// </returns>
int Set_MakeGundan(int _m_iBushouID, int _m_iCastleID, MAKEGUNDANKOUHOINFO check);


/// <summary>
/// <para>とある大名家に所属している軍団を独立させ、大名とする</para>
/// <para>返り値：</para>
/// <para>独立できた場合は、大名番号【配列用】が返ってくる</para>
/// <para>独立できなかった場合0xFFFFが返ってくる。</para>
/// </summary>
/// <param name="軍団番号【配列用】">独立させたい軍団番号【配列用】</param>
/// <returns>
/// <para>独立できた場合は、大名番号【配列用】が返ってくる</para>
/// <para>独立できなかった場合0xFFFFが返ってくる。</para>
/// </returns>
int Set_IndependentGundan(int iGungdanID);
