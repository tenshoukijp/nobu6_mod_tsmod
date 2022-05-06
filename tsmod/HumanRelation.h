#pragma once


bool is自分は相手の娘				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の息子				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の子				( int i1st_BushouID, int i2nd_BushouID );

bool is自分は相手の親				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の父				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の父				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の母				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の母				( int i1st_BushouID, int i2nd_BushouID );

bool is自分は相手の弟				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の弟				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の妹				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の妹				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の兄				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の兄				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の姉				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の姉				( int i1st_BushouID, int i2nd_BushouID );

bool is自分と相手は兄弟( int i1st_BushouID, int i2nd_BushouID ); // 1stが兄で、2ndが弟
bool is自分と相手は姉妹( int i1st_BushouID, int i2nd_BushouID );
bool is自分と相手は姉弟( int i1st_BushouID, int i2nd_BushouID );
bool is自分と相手は兄妹( int i1st_BushouID, int i2nd_BushouID );

bool is自分は相手の夫				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の妻				( int i1st_BushouID, int i2nd_BushouID );
bool is自分は相手の血縁				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の家臣				( int i1st_BushouID, int i2nd_BushouID );
bool is相手は自分の大名				( int i1st_BushouID, int i2nd_BushouID );
bool is自分と相手は同じ大名家所属	( int i1st_BushouID, int i2nd_BushouID ); // 大名自身も含む(大名自身も自分の大名家に所属している)

