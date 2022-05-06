/*
��@

#include <iostream>
#include "picojson.h"

using namespace std;

int main(void) {
	picojson::value v;
	const char* s = "{\"hello\" : \"world.\"}";
	picojson::parse(v, s, s + strlen(s));
	cout << "hello : " << v.get("hello").get<string>() << endl;
	return 0;
}

*/



/*
��A

���ۂɈȉ���JSON�f�[�^��picojson����G���Ă݂܂��B


{
"key1": [
"val1",
"val2",
"val3"
],
"to_delete": "delete val"
}

��܂��Ȏg�����͈ȉ��̂悤�Ȋ����ł��傤���B�c���[�̓����m�[�h�́Aobject�^(map��string , picojson::value��)��array�^(vector��picojson ::value��)�ŕ\������Ă��܂��B�P����STL�Ȃ̂ň����₷���ł��ˁB


#include ��iostream��
#include "picojson.h"

using namespace std;

int main(int argc, char** argv) {

	// create object
	picojson::value v;
	cin >> v;

	picojson::object& o = v.get<picojson ::object>();
	picojson::value& c = o["key1"];

	// read data
	cout < < o["to_delete"].get<string>() < < endl;

	// delete array item
	picojson::array& a = c.get</picojson></picojson><picojson ::array>();
	for (picojson::array::iterator it = a.begin(); it != a.end(); it++) {
		if (it->get<string>() == "val2") {
			it = a.erase(it);
		}
	}

	// delete map item
	o.erase("to_delete");

	// add item
	o.insert(make_pair("key3", picojson::value(string("val4"))));

	// output
	cout < < v << endl;
}
*/

/*
 ��B

#include "picojson.h"

#include <string>
#include <vector>

using namespace std;
using namespace picojson;

void main(){
    
    
    object o;
    array v;
    
    // 0
    v.push_back(value(-0.5000));
    v.push_back(value(0.5000));
    v.push_back(value(0.5000));
    // 1
    v.push_back(value(-0.5000));
    v.push_back(value(-0.5000));
    v.push_back(value(0.5000));
    // 2
    v.push_back(value(0.5000));
    v.push_back(value(0.5000));
    v.push_back(value(0.5000));
    // 3
    v.push_back(value(0.5000));
    v.push_back(value(-0.5000));
    v.push_back(value(0.5000));
    // 4
    v.push_back(value(0.5000));
    v.push_back(value(0.5000));
    v.push_back(value(-0.5000));
    // 5
    v.push_back(value(0.5000));
    v.push_back(value(-0.5000));
    v.push_back(value(-0.5000));
    // 6
    v.push_back(value(-0.5000));
    v.push_back(value(0.5000));
    v.push_back(value(-0.5000));
    // 7
    v.push_back(value(-0.5000));
    v.push_back(value(-0.5000));
    v.push_back(value(-0.5000));
    
    array i;
    // �O��1
    i.push_back(value(2.0));
    i.push_back(value(3.0));
    i.push_back(value(1.0));
    // �O��2
    i.push_back(value(2.0));
    i.push_back(value(1.0));
    i.push_back(value(0.0));

    i.push_back(value(4.0));
    i.push_back(value(5.0));
    i.push_back(value(3.0));

    i.push_back(value(4.0));
    i.push_back(value(3.0));
    i.push_back(value(2.0));

    i.push_back(value(6.0));
    i.push_back(value(7.0));
    i.push_back(value(5.0));

    i.push_back(value(6.0));
    i.push_back(value(5.0));
    i.push_back(value(4.0));

    i.push_back(value(0.0));
    i.push_back(value(1.0));
    i.push_back(value(7.0));

    i.push_back(value(0.0));
    i.push_back(value(7.0));
    i.push_back(value(6.0));

    i.push_back(value(4.0));
    i.push_back(value(2.0));
    i.push_back(value(0.0));

    i.push_back(value(4.0));
    i.push_back(value(0.0));
    i.push_back(value(6.0));

    i.push_back(value(3.0));
    i.push_back(value(5.0));
    i.push_back(value(7.0));

    i.push_back(value(3.0));
    i.push_back(value(7.0));
    i.push_back(value(1.0));
    
    o.insert(make_pair("v", v));
    o.insert(make_pair("i", i));
    value json = value(o);
    printf("json=%s",json.serialize().c_str());   
}

���s���ʂ�

json={"i":[2.000000,3.000000,1.000000,2.000000,1.000000,0.000000,4.000000,5.0000
00,3.000000,4.000000,3.000000,2.000000,6.000000,7.000000,5.000000,6.000000,5.000
000,4.000000,0.000000,1.000000,7.000000,0.000000,7.000000,6.000000,4.000000,2.00
0000,0.000000,4.000000,0.000000,6.000000,3.000000,5.000000,7.000000,3.000000,7.0
00000,1.000000],"v":[-0.500000,0.500000,0.500000,-0.500000,-0.500000,0.500000,0.
500000,0.500000,0.500000,0.500000,-0.500000,0.500000,0.500000,0.500000,-0.500000
,0.500000,-0.500000,-0.500000,-0.500000,0.500000,-0.500000,-0.500000,-0.500000,-
0.500000]}

*/