/*
メイン画面のコマンドの「サブコマンド」は、以下の分岐でジャンプしていくようだ。
コマンドに対応する関数ポインタのように実装されているのであろう。
たとえば、軍団コマンドであれば、
「任命」「分配」「合流」「入替」「再任」が分岐していく。
004107BE  |. FF1485 70B94B0>CALL    DWORD PTR DS:[EAX*4+4BB970]
*/



/*
 「機能」からのサブコマンドは以下のように分岐する

	// メニューの「機能」呼び出し。EAXが左から、0:記録, 1:呼出, ... call 0x4cb788すると自動的に記録。
	00452F9E   . FF1485 88B74C0>CALL DWORD PTR DS:[EAX*4+4CB788]
*/
