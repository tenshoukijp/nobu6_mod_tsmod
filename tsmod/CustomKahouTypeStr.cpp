#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <windows.h>

#include "GameDataStruct.h"
#include "TSModCommand.h"

#include "KahouType.h"


CUSTOM_KAHOU_TYPE_STRING nb6kahou_custom_type_string[GAMEDATASTRUCT_KAHOU_NUM] = {""};


// �ƕ��ނ̃^�C�v�̃J�X�^��������ւ̃|�C���^�[�𓾂�
char *GetOverWriteKahouTypeMeiPointer(int iKahouID) {

	// .jp����ƕ�̑��݂��n�m
	if ( !TSModCommand::World::cmdCustomKahouExists ) {
		return NULL;
	}

	char *p = nb6kahou[iKahouID].name;
	if ( strstr( p, "�c��" ) ) {
		return "�c��";
	} else if ( strcmp( p, "�������q��" ) == 0 ) {
		return "��";
	} else if ( strcmp( p, "�u�燓�ہڃg" ) == 0 ) { // �u�������q���g
		return "�F����";
	} else if ( strcmp( p, "�o���o���b�T" ) == 0 ) {
		return "�F����";
	} else if ( strcmp( p, "�x�C�I�E���t" ) == 0 ) {
		return "�F����";
	} else if ( strcmp( p, "�g���X�^��" ) == 0 ) {
		return "�F����";
	} else if ( strstr( p, "�����" ) ) {
		return "�����";
	} else if ( strcmp( p, "��" ) == 0 ) {
		return "����";
	} else if ( strcmp( p, "��" ) == 0 ) {
		return "���w";
	} else if ( strcmp( p, "�Ή�" ) == 0 ) {
		return "���F";
	} else if ( strcmp( p, "�璹" ) == 0 ) {
		return "���F";
	} else if ( strcmp( p, "���p�t�c�t" ) == 0 ) {
		return "�t";
	} else if ( strcmp( p, "���t" ) == 0 ) {
		return "�t";
	} else if ( strcmp( p, "�T���V�I��" ) == 0 ) {
		return "��";
	} else if ( strcmp( p, "�|�}�~������" ) == 0 ) { // ���v�`���[���[�Y
		return "��";
	} else if ( strcmp( p, "���_�̃��`" ) == 0 ) {
		return "��";
	} else if ( strstr( p, "�̏�" ) ) {
		return "��";
	} else if ( strstr( p, "�̏e" ) ) {
		return "�S�C";
	} else if ( strcmp( p, "�T�U���N���X" ) == 0 ) {
		return "�";
	} else if ( strcmp( p, "���̓�" ) == 0 ) {
		return "�ߑ�";
	} else if ( strcmp( p, "�[���̃��[�u" ) == 0 ) {
		return "�ߑ�";
	} else if ( strcmp( p, "�`��" ) == 0 ) {
		return "�`��";
	} else if ( strcmp( p, "�g����l�`" ) == 0 ) {
		return "�l�`";
	} else if ( strstr( p, "�̈�" ) ) {
		return "��";
	} else if ( strcmp( p, "�l�l�̎g�k" ) == 0) {
		return "�G��";
	} else if ( strcmp( p, "�S���S�^�̋u" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�ԗւ̐���" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "���_�̒a��" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�Ō�̐R��" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�ӂ��҂̓V��" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�l�̐}" ) == 0 ) {
		return "�f�`";
	} else if ( strcmp( p, "���i���U" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "��܎t�̏ё�" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�}�M�̗�q" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�L���X�g����" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "��ٍ��m" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�g���h�̕��i" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�o�x���̓�" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�o�x���̓�" ) == 0 ) {
		return "�G��";
	} else if ( strcmp( p, "�j�L" ) == 0) {
		return "�j��";
	} else if ( strcmp( p, "���q" ) == 0) {
		return "���@��";
	} else if ( strcmp( p, "���q" ) == 0 ) {
		return "���@��";
	} else if ( strcmp( p, "�Z��" ) == 0 ) {
		return "���@��";
	} else if ( strcmp( p, "�O��" ) == 0 ) {
		return "���@��";
	} else if ( strcmp( p, "�b�z�R��" ) == 0 ) {
		return "�R�w��";
	} else if ( strcmp( p, "���𗬕��@��" ) == 0 ) {
		return "���@��";
	} else if ( strcmp( p, "����_������" ) == 0 ) {
		return "���@��";
	} else if ( strstr( p, "���p��" ) ) {
		return "���@��";
	} else if ( strstr( p, "�|�p��" ) ) {
		return "���@��";
	} else if ( strcmp( p, "����W�C" ) == 0) {
		return "�E�p��";
	} else if ( strcmp( p, "���󑠂̏�" ) == 0 ) {
		return "�L�^��";
	} else if ( strcmp( p, "���b�S�Ȏ��T" ) == 0 ) {
		return "���T";
	} else if ( strcmp( p, "�����@" ) == 0 ) {
		return "������";
	} else if ( strcmp( p, "�O�������[��" ) == 0 ) {
		return "������";
	} else if ( strcmp( p, "�����ꖂ�@" ) == 0 ) {
		return "������";
	} else if ( strcmp( p, "�΋򂢒��̉H" ) == 0 ) {
		return "�H��";
	} else if ( strcmp( p, "���ʐ�" ) == 0 ) {
		return "����";
	} else if ( strcmp( p, "���҂̐�" ) == 0 ) {
		return "�_��";
	} else if ( strcmp( p, "�K���X�̓�Z" ) == 0 ) {
		return "����";
	} else if ( strcmp( p, "�����̃��_��" ) == 0 ) {
		return "����";
	} else if ( strcmp( p, "������" ) == 0 ) {
		return "������";
	} else if ( strcmp( p, "�y���_���g" ) == 0 ) {
		return "���g��";
	} else if ( strcmp( p, "�T���G��" ) == 0 ) {
		return "����";
	} else if ( strcmp( p, "�����̏��_" ) == 0 ) {
		return "�Α�";
	} else if ( strcmp( p, "�����N���E�X" ) == 0 ) {
		return "�Α�";
	} else if ( strcmp( p, "�Z��" ) == 0 ) {
		return "�Z��";
	} else if ( strstr( p, "��" ) && strstr( p, "�̏�" ) ) {
		return "���";
	} else if ( strcmp( p, "����q��" ) == 0 ) {
		return "���ꑜ";
	} else if ( strcmp( p, "���̃����v" ) == 0 ) {
		return "�m��";
	} else if ( strcmp( p, "������" ) == 0 ) {
		return "��";
	}

	return NULL;
}