#include "WinTarget.h"


/**
 * 0.5を足してから切り捨てることで小数点以下を四捨五入する
 * @param[in] x 四捨五入を行う浮動小数点数
 * @return 四捨五入の結果
 */
int round_整数化(double a) {
    return int(a+.5-(a<0)); 
}

double round_二分の一吸着化(double a) {
    double b = a * 2;
	b = round_整数化(b+.5-(b<0));
	return b/2;
}
