#include "WinTarget.h"


/**
 * 0.5�𑫂��Ă���؂�̂Ă邱�Ƃŏ����_�ȉ����l�̌ܓ�����
 * @param[in] x �l�̌ܓ����s�����������_��
 * @return �l�̌ܓ��̌���
 */
int round_������(double a) {
    return int(a+.5-(a<0)); 
}

double round_�񕪂̈�z����(double a) {
    double b = a * 2;
	b = round_������(b+.5-(b<0));
	return b/2;
}
