/* boostのanyコンテナ。どのような型でも変数へと代入できる。
 * any_cast は失敗すると bad_any_cast を投げます。
 *
 * Microsoftのvariant型とおなじような感じであるが、anyの方が、記述が全然自然である。
 */

/*

#include <boost/any.hpp>

struct Hoge {
  Hoge() :
    value(0)
  {
    std::cout << "Hoge()" << std::endl;
  }
  
  ~Hoge() {
    std::cout << "~Hoge()" << std::endl;
  }

  int value;
};


int main() {
  boost::any value = Hoge();

  {
    std::cout << "Test 1" << std::endl;
    auto v = boost::any_cast<Hoge>(value);
    v.value += 1;
  }
  
  {
    std::cout << "Test 2" << std::endl;
    auto& v = boost::any_cast<Hoge&>(value);
    v.value += 1;
    
    std::cout << v.value << std::endl;
  }    
  
  {
    std::cout << "Test 3" << std::endl;
    auto& v = boost::any_cast<Hoge&>(value);
    v.value += 1;
    
    std::cout << v.value << std::endl;
  }    
}


*/