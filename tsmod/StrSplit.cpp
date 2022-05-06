#include <string>
#include <vector>

using namespace std;

vector<string> string_split(const string &str, const string &delim) {

	vector<string> res;
	size_t current = 0, found, delimlen = delim.size();
	while((found = str.find(delim, current)) != string::npos){
	res.push_back(string(str, current, found - current));

	current = found + delimlen;

	}

	res.push_back(string(str, current, str.size() - current));
	return res;
}