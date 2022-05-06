#pragma once


/*
[ ���ݒ�]

B �푈�����錩�Ȃ� / BGM / SE / ���[�r�[
000ebbcd d=SE c=BGM e=���[�r�[

B 0?
B ���b�Z�[�W���x
B ��Փx
*/
struct NB6SETTINGS
{
	byte se : 1;				// �r�d�͗L����
	byte bgm : 1;				// �a�f�l�͗L����
	byte other_war : 2;		// �����̐푈�@����=2 �C��=1 ���Ȃ�=0
	byte movie_is_off : 1;	// ���[�r�[�͖������@(�����Ȃ�1�ŁA�L���Ȃ�0�Ƃ����Ȃ�Ƃ�������ɂ����c) IF���Ŏg���Ƃ��� == (�C�R�[������)���ȗ�����ƊԈႤ�B���[�r�[::�n�m�A���[�r�[::�n�e�e��enum���g������!!
	byte _undef1 : 3;
	byte _undef2;
	byte messagespd;		// ���b�Z�[�W���x	0 �` 4(��) �\�����ʂ�
	byte difficult;			// ��Փx			0 �` 4 �\�����ʂ�
};



// SE���L�����H
bool isSEValid();

// BGM���L�����H
bool isBGMValid();

// Movie���L�����H
bool isMovieValid();

// _INMM���[�h���H
bool is_InmmMode();

// �f�����[�h���H
bool is_DemoMode();

/*
[ ���ݒ�g�� ]

0-1   W �N���\��( ����=1 ����=0 )
2-3   W �\���ʒu�L��(�}�E�X=1 �L��=0 )
4-5   W ?
6-7   W ?
8-9   W �\���ؑփ}�b�v( �A��=1, ��A��=0 )
10-11 W ?
12-13 W �푈�p�x(�D��=1, ���m=0)
14-15 W �v���p�x(����=1, ���Ȃ�=0)
16-17 W ���S�p�x(����=1, ���Ȃ�=0)
18-19 W ?
20-21 W �����̐푈(�C��=2, �]��=1, ����=0)
22-23 W �ҏW�@�\(�L��=1, ����=0)
24-25 W ���\��(�ŏ�=2, ���Ȃ�=1,����=0 )
26-27 W ?
28-29 W �����s���u��(�L��=1, ����=0)
30-31 W �P�����u��(����=2, �]��=1, ����=0 )
*/

struct NB6SETTINGSEX
{
	WORD yeardisp;		// �N���\��( ����=1 ����=0 )
	WORD remlocdisp;	// �\���ʒu�L��(�}�E�X=1 �L��=0 )
	WORD undef1;
	WORD undef2;
	WORD mapdisp;		// �\���ؑփ}�b�v( �A��=1, ��A��=0 )
	WORD undef3;
	WORD warfreq;		// �푈�p�x(�D��=1, ���m=0)
	WORD keiryakufreq;	// �v���p�x(����=1, ���Ȃ�=0)
	WORD deathfreq;		// ���S�p�x(����=1, ���Ȃ�=0)
	WORD undef4;
	WORD myselfwardisp;	// �����̐푈(�C��=2, �]��=1, ����=0)
	WORD edit;			// �ҏW�@�\(�L��=1, ����=0)
	WORD infodisp;		// ���\��(�ŏ�=2, ���Ȃ�=1,����=0 )
	WORD undef5;
	WORD bushoushigan;	// �����s���u��(�L��=1, ����=0)
	WORD himeshigan;	// �P�����u��(����=2, �]��=1, ����=0 )
};



// �S�C�`��
bool isTeppouDenrai();

// �S�C�`������߂�
void UnsetTeppouDenrai();

// �L���X�g���`��
bool isChristDenrai();

// �L���X�g���`������߂�
void UnsetChristDenrai();

// ���y�邪���ݍς݂��ǂ���
bool isAzuchiCastleComplete();

// ���邪���ݍς݂��ǂ���
bool isOsakaCastleComplete();


