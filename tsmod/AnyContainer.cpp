/* boost��any�R���e�i�B�ǂ̂悤�Ȍ^�ł��ϐ��ւƑ���ł���B
 * any_cast �͎��s����� bad_any_cast �𓊂��܂��B
 *
 * Microsoft��variant�^�Ƃ��Ȃ��悤�Ȋ����ł��邪�Aany�̕����A�L�q���S�R���R�ł���B
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