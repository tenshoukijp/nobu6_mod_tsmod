#pragma once

#include <string>
using namespace std;

// 00�i����Ɍh�ӂ𕥂��K�v���Ȃ��ꍇ�̎��́j 
string ���킵��			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 02�i���̑��̏ꍇ�̎��́j 
string ������			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 03�i���L�`�e���j  
string ���䂪��			(int i1st_BushouID, int i2nd_BushouID = 0xFFFF	, int i3rd_BushouID = 0xFFFF);

// 04�`07 ������������ޏꍇ�u�ȊO�v�̓�l��
string �����聓		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);

// 08 ������������ޏꍇ�̓�l��
string ���l�|���聓	(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID = 0xFFFF);

// 09�`0D �O�����������ޏꍇ�u�ȊO�v�̎O�l��
string ���O����		(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID);

// 0E�i��O�҂��������ޏꍇ�̎O�l�́j 
string ���l�|�O����	(int i1st_BushouID, int i2nd_BushouID			, int i3rd_BushouID);