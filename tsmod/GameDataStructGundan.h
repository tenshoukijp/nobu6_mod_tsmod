#pragma once

/*			�R�c���(21*214)

[21�o�C�g]

0-1	�O�̌R�c
2-3 ���̌R�c
4-5	���閼�i���o�[
6-7	�R�c���i���o�[(�������O�e�[�u���̏���)
8-9	�喼
10	�s����
11-12	��
13-14	��
15-16	�R�n
17-18	�S�C
19	�H
20	aaaabbbb A ��E B �R�c�ԍ�
0 ���� 1 ���Α叫�R 2 �Ǘ� 3 �֓��Ǘ� 4 ���B�T�� 5 �����T�� 6 ��B�T�� 7 �H�B�T�� �� getNormalizedGundanPosition

*/
struct NB6GUNDAN
{
	WORD prev;		//�O�̌R�c
	WORD next;		//���̌R�c
	WORD residence;	//���閼�i���o�[
	WORD leader;	//�R�c���i���o�[(�������O�e�[�u���̏���)
	WORD attach;	//����(�喼) (�喼�ԍ��e�[�u���̏���)
	byte act;		//�s����
	WORD money;		//��
	WORD rise;		//��
	WORD horse;		//�n
	WORD gun;		//�S�C
	byte _undef;	//must 0?
	byte number : 4;	//1-8 �� �R�c�ԍ�
	byte position : 4;//��E  (0 ���� 1 ���Α叫�R 2 �Ǘ� 3 �֓��Ǘ� 4 ���B�T�� 5 �����T�� 6 ��B�T�� 7 �H�B�T��) �� getNormalizedGundanPosition
};

/// ����(���̏u��)�A�V�ċL�ɑ��݂���R�c�́u�R�c�ԍ��y�z��p�z�v�����X�g�œ���B
vector<int> getValidGundanList();


// �����喼�ɑ�����R�c�ԍ��y�z��p�z�̃��X�g�𓾂�
vector<int> getGovernedGundanList(int iDaimyoID);

// �R�c�ɏ������镐�����X�g�𓾂�
vector<int> getGundanBushouList(int iGundanID);

// �R�c�ɏ������镺���𓾂�
int getGundanHeisuCnt(int iGundanID);


// �Ƃ���喼���͂̓����R�c���肠�Ĕԍ����X�g�𓾂�B
vector<int> getGovernedGundanWariateNumList(int iDaimyoID);

// �Ƃ���喼�z���̌R�c���ɂ���R�c�u���v�����ԍ��̃��X�g�𓾂�B
vector<int> getNotGovernedGundanWariateNumList(int iDaimyoID);

// �R�c�C���ԍ��̕ύX(����)�����݂�B���s�����false
bool tryChangeGundanWariateNum(int iGundanID, int iNewWariateNum);

// �w��̌R�c�̏�������喼�𐨗͂��z���āA�ύX����B
bool setGundanReAttachDaimyo(int iGundanID, int iNewDaimyoID);

// �Ƃ���喼���ł̂`�R�c�̏���a�R�c�ւƃA�^�b�`�ύX
bool Set_ChangeCastleAttachGundan(int iCastleID, int iNewGundanID);


struct MAKEGUNDANKOUHOINFO {
	int m_iDaimyoID;                     /// <summary>�ǂ̑喼�ɂ��Ă̏��Ȃ̂��B</summary>
	BOOL CanMake;                            /// <summary>���ǁA�ȉ��̂R�v�f����l���āA�R�c�̐V�݂͉\�Ȃ̂��A�s�\�Ȃ̂��B</summary>
	vector<int> vGundanIDList;        /// <summary>�R�c�����ԍ��ŗ]���Ă���ԍ��̃��X�g�BGet_�R�c�������ԍ����X�g(...) �Ɠ����l</summary>
	vector<int> vCastleIDList;    /// <summary>�����R�c��V�ݏo����Ƃ���΁A�����̏邪���ƂȂ�A�Ƃ������X�g</summary>
	vector<int> vBushouIDList;  /// <summary>�����R�c��V�ݏo����Ƃ���΁A�����̕������V���ȌR�c���Ƃ��ĔC�����꓾��A�Ƃ������X�g</summary>
};

/// <summary>
/// <para>�Ƃ���喼�ƂɌR�c��V�݉\���ǂ�����₢���킹��</para>
/// <para>���̌��ʁA�u�s�v�u�ǂ̕�������₩�v�u�ǂ̏邪��₩�v�Ƃ�������񂪓�����</para>
/// <para>���̊֐��œ���ꂽ�������ɁA Set_�R�c�V��(...)�֐��ŌR�c�����ۂɐV�݂��邱�ƂƂȂ�B</para>
/// <para>�Ԃ�l�F</para>
/// <para>MAKEGUNDANKOUHOINFO�̒l���Ԃ��Ă���B</para>
/// </summary>
/// <param name="_m_iBushouID">�R�c���ɂ�����_m_iBushouID</param>
/// <param name="_m_iCastleID">�R�c��V�݂�����_m_iCastleID</param>
/// <param name="���S�`�F�b�N�p�R�c�V�݌���">Get_�R�c�V�݌����(...)�֐��̌��ʒl��n���K�v������B</param>
/// <returns>
/// <para>MAKEGUNDANKOUHOINFO�̒l���Ԃ��Ă���B</para>
/// </returns>
MAKEGUNDANKOUHOINFO Get_MakeGundanKouhoInfo(int m_iDaimyoID);


/// <summary>
/// <para>�Ƃ���喼�ƂɌR�c��V�݂���</para>
/// <para>�Ԃ�l�F</para>
/// <para>�V�݂ł����ꍇ�́A_iGundanID���Ԃ��Ă���</para>
/// <para>�V�݂ł��Ȃ������ꍇ0xFFFF���Ԃ��Ă���B</para>
/// <para>Get_MakeGundanKouhoInfo()�̕Ԃ�l�Ɋ�Â��āA�g�����Ƃ��z�肳��Ă���B</para>
/// </summary>
/// <param name="_m_iBushouID">�R�c���ɂ�����_m_iBushouID</param>
/// <param name="_m_iCastleID">�R�c��V�݂�����_m_iCastleID</param>
/// <param name="check">Get_�R�c�V�݌����(...)�֐��̌��ʒl��n���K�v������B</param>
/// <returns>
/// <para>�V�݂ł����ꍇ�́A_iGundanID���Ԃ��Ă���B</para>
/// <para>�V�݂ł��Ȃ������ꍇ0xFFFF���Ԃ��Ă���B</para>
/// </returns>
int Set_MakeGundan(int _m_iBushouID, int _m_iCastleID, MAKEGUNDANKOUHOINFO check);


/// <summary>
/// <para>�Ƃ���喼�Ƃɏ������Ă���R�c��Ɨ������A�喼�Ƃ���</para>
/// <para>�Ԃ�l�F</para>
/// <para>�Ɨ��ł����ꍇ�́A�喼�ԍ��y�z��p�z���Ԃ��Ă���</para>
/// <para>�Ɨ��ł��Ȃ������ꍇ0xFFFF���Ԃ��Ă���B</para>
/// </summary>
/// <param name="�R�c�ԍ��y�z��p�z">�Ɨ����������R�c�ԍ��y�z��p�z</param>
/// <returns>
/// <para>�Ɨ��ł����ꍇ�́A�喼�ԍ��y�z��p�z���Ԃ��Ă���</para>
/// <para>�Ɨ��ł��Ȃ������ꍇ0xFFFF���Ԃ��Ă���B</para>
/// </returns>
int Set_IndependentGundan(int iGungdanID);
