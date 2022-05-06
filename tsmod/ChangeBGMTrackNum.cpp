#include "TSModCommand.h"
#include "CommonGlobalHandle.h"

/*
 ��Q�R�c�ȉ��ɁA�R�}���h�̎w�����\�B�_�p�b�`���̈ڐA�B
 */


void WritePatchChangeBGMTrackNum( int iAddress, char Value ) {
	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(iAddress), &Value, 1, NULL); //1�o�C�g�̂ݏ�������
}

#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_01 0x411A9C
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_02 0x411D41
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_03 0x412919
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_04 0x4138A3
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_05 0x413C2F
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_06 0x419709
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_07 0x419BF4
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_08 0x419C3A
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_09 0x41B488
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_10 0x42337F
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_11 0x42369E
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_12 0x4298F6
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_13 0x42B8D3
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_14 0x42BA2B
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_15 0x42C4EE
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_16 0x42C92B
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_17 0x42CEDC
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_18 0x42D7F1
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_19 0x430F84
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_20 0x431108
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_21 0x43D52C
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_22 0x43DA67
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_23 0x43DCBA
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_24 0x43E8C9
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_25 0x43EE98
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_26 0x4487D6
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_27 0x4487DF
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_28 0x452F91
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_29 0x4581BD
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_30 0x4584CA
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_31 0x458934
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_32 0x458A4E
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_33 0x458E8D
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_34 0x45901B
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_35 0x459254
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_36 0x459574
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_37 0x4599B8
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_38 0x459ACF
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_39 0x459D12
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_40 0x45A338
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_41 0x45A5DE
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_42 0x45AA37
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_43 0x45AD04
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_44 0x45B8B9
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_45 0x47EBBB
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_46 0x482ECE
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_47 0x482EDD
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_48 0x483227
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_49 0x48A158
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_50 0x48A73F
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_51 0x48A8E7
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_52 0x48AAA0
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_53 0x48AAC0
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_54 0x48AC73
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_55 0x48AE16
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_56 0x48B066
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_57 0x48B889
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_58 0x48BAF6
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_59 0x49D77A
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_60 0x4B2993
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_61 0x4B29A2
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_62 0x4B29AF
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_63 0x4B29C2
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_64 0x4B29D5

#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_65 0x4BE4F8
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_66 0x4BE500
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_67 0x4BE504
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_68 0x4BE50C
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_69 0x4BE514
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_70 0x4BE51C
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_71 0x4CB9F8
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_72 0x4CB9FA
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_73 0x4CB9FC
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_74 0x4D1A88
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_75 0x4D1A89
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_76 0x4D1A8A
#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_77 0x4D1A8B

#define CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_78 0x436B51 // �_�p�b�`�ɂȂ������ʂɒǉ��B�V�i���I�I����ʂő喼��I��Ő��O�ŃL�����Z���������̉��y




void SetChangeBGMTrackNum() {
	/*
	   �_�p�b�`�ɂ���āA�a�f�l�̃g���b�N�i���o�[��ύX�����ꍇ�B
	   �����f�t�H���g�̂P�R�ȔŁA�E���A�R�W�Ȕ�
	0x411a9c: 03 1A ��
	0x411d41: 0A 0C ��
	0x412919: 06 1C ��
	0x4138a3: 0A 22 ��
	0x413c2f: 03 1A ��
	0x419709: 08 18 ��
	0x419bf4: 03 1A ��
	0x419c3a: 08 19 ��
	0x41b488: 03 1A ��
	0x42337f: 08 0B ��
	0x42369e: 03 1A ��
	0x4298f6: 0A 0D ��
	0x42b8d3: 08 0B ��
	0x42ba2b: 08 0B ��
	0x42c4ee: 06 13 ��
	0x42c92b: 03 1A ��
	0x42cedc: 03 1A ��
	0x42d7f1: 03 1A ��
	0x430f84: 0D 1D ��
	0x431108: 0D 1D ��
	0x43d52c: 03 1A ��
	0x43da67: 03 1A ��
	0x43dcba: 0D 1D ��
	0x43e8c9: 03 1A ��
	0x43ee98: 0D 1D ��
	0x4487d6: 0A 23 ��
	0x4487df: 0A 22 ��
	0x452f91: 07 17 ��
	0x4581bd: 0D 1D ��
	0x4584ca: 0D 1D ��
	0x458934: 03 1A ��
	0x458a4e: 06 13 ��
	0x458e8d: 03 1A ��
	0x45901b: 06 13 ��
	0x459254: 0D 1D ��
	0x459574: 0D 1D ��
	0x4599b8: 03 1A ��
	0x459acf: 06 13 ��
	0x459d12: 0D 1D ��
	0x45a338: 03 1A ��
	0x45a5de: 06 12 ��
	0x45aa37: 03 1A ��
	0x45ad04: 0D 1D ��
	0x45b8b9: 0D 1D ��
	0x47ebbb: 0A 1E ��
	0x482ece: 06 13 ��
	0x482edd: 06 12 ��
	0x483227: 0A 0D ��
	0x48a158: 06 12 ��
	0x48a73f: 0D 1D ��
	0x48a8e7: 03 1A ��
	0x48aaa0: 0A 0D ��
	0x48aac0: 06 13 ��
	0x48ac73: 0D 1D ��
	0x48ae16: 0B 14 ��
	0x48b066: 06 1C ��
	0x48b889: 06 25 ��
	0x48baf6: 0D 1D ��
	0x49d77a: 06 16 ��
	0x4b2993: 0B 26 ��
	0x4b29a2: 08 24 ��
	0x4b29af: 08 24 ��
	0x4b29c2: 0A 0C ��
	0x4b29d5: 06 25 ��

	�t�@�C���́c
	000BD0F8: 01 02�@��4BE4F8
	000BD100: 03 05  ��4BE500
	000BD104: 04 05  ��4BE504
	000BD10C: 06 03  ��4BE50C
	000BD114: 08 07  ��4BE514
	000BD11C: 0A 09  ��4BE51C
	000CA5F8: 1F 0A  ��4CB9F8
	000CA5FA: 20 0A  ��4CB9FA
	000CA5FC: 21 0A	 ��4CB9FC
	000D0688: 0E 04�@��4D1A88
	000D0689: 0F 0B�@��4D1A89
	000D068A: 10 06�@��4D1A8A
	000D068B: 11 04�@��4D1A8B
	*/

	// �w���\�ł���΁c
	if ( TSModCommand::Environment::cmdBGMMode == 0 || TSModCommand::Environment::cmdBGMMode == 1 ||  TSModCommand::Environment::cmdBGMMode == 3 ) { // CD13�Ȃ��AINMM13�ȂȂ�΃f�t�H���蓖��

		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_01, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_02, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_03, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_04, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_05, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_06, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_07, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_08, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_09, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_10, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_11, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_12, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_13, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_14, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_15, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_16, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_17, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_18, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_19, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_20, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_21, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_22, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_23, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_24, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_25, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_26, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_27, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_28, (char)0x07);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_29, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_30, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_31, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_32, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_33, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_34, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_35, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_36, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_37, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_38, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_39, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_40, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_41, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_42, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_43, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_44, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_45, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_46, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_47, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_48, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_49, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_50, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_51, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_52, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_53, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_54, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_55, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_56, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_57, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_58, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_59, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_60, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_61, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_62, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_63, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_64, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_65, (char)0x02);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_66, (char)0x05);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_67, (char)0x05);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_68, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_69, (char)0x07);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_70, (char)0x09);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_71, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_72, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_73, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_74, (char)0x04);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_75, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_76, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_77, (char)0x04);

		// �ȉ��_�p�b�`�ɂȂ������ʂɒǉ��B�V�i���I�I����ʂő喼��I��Ő��O�ŃL�����Z���������̉��y
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_78, (char)0x03);

	// �s�\�ł���΁c
	} else if ( TSModCommand::Environment::cmdBGMMode == 2 ) { // INMM38��

		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_01, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_02, (char)0x0C);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_03, (char)0x1C);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_04, (char)0x22);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_05, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_06, (char)0x18);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_07, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_08, (char)0x19);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_09, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_10, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_11, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_12, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_13, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_14, (char)0x0B);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_15, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_16, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_17, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_18, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_19, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_20, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_21, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_22, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_23, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_24, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_25, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_26, (char)0x23);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_27, (char)0x22);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_28, (char)0x17);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_29, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_30, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_31, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_32, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_33, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_34, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_35, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_36, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_37, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_38, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_39, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_40, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_41, (char)0x12);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_42, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_43, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_44, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_45, (char)0x1E);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_46, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_47, (char)0x12);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_48, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_49, (char)0x12);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_50, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_51, (char)0x1A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_52, (char)0x0D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_53, (char)0x13);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_54, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_55, (char)0x14);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_56, (char)0x1C);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_57, (char)0x25);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_58, (char)0x1D);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_59, (char)0x16);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_60, (char)0x26);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_61, (char)0x24);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_62, (char)0x24);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_63, (char)0x0C);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_64, (char)0x25);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_65, (char)0x01);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_66, (char)0x03);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_67, (char)0x04);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_68, (char)0x06);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_69, (char)0x08);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_70, (char)0x0A);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_71, (char)0x1F);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_72, (char)0x20);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_73, (char)0x21);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_74, (char)0x0E);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_75, (char)0x0F);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_76, (char)0x10);
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_77, (char)0x11);

		// �ȉ��_�p�b�`�ɂȂ������ʂɒǉ��B�V�i���I�I����ʂő喼��I��Ő��O�ŃL�����Z���������̉��y
		WritePatchChangeBGMTrackNum( CMD_CHANGE_BGM_TRACK_NUM_ADDRESS_78, (char)0x13);
	}
}