#pragma once

#pragma pack(1)

#include <string>
#include <vector>

using namespace std;




// �I���^�C�v�̃��j���[�̕\���B�e���j���[�̕������ menu_list �ɓ���Ă������ƁBposX��poxY�͕\��������W�B�w�͉E�ցA�x�͉��� 
// �����I�������E�N���b�N�ŕ����ꍇ�́A0xFFFF���l�Ƃ��ĕԂ��Ă���B
// �w��x���w�肵�Ȃ��Ƃ��́A�^�񒆂�����ɔz�u����B
int SelectMenuDialog(vector<string> menu_list, int posX=-1, int posY=-1, int MinMenuWidth=0x70);
