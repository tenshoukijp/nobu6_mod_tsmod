#pragma once


#include "WinTarget.h"
#include <mmsystem.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"


class EXBushou {
	protected:
		bool m_isUltraBushou;			// ���̐l���Ɠ��̃X�[�p�[���[�h(ON/OFF�؂�ւ�)

		DWORD m_curTimeGetTime;
		DWORD m_preTimeGetTime;

		int m_nb6bushou_ix;			// ���̕����̔z��ID(512�܂ł̂��)

		byte m_lifetimes;			// ���̕����̍΂����Ȃ��

		char m_name[13];			// �S�p�U����+\0

	public:

		EXBushou();

		// �X�[�p�[���[�h���Z�b�g
		void setSuperMode(bool isUltraBushou) {
			m_isUltraBushou = isUltraBushou;
		}

		// �������̕�����𓾂�(�ŏ��͋�̂͂��Ȃ̂Œ���)
		char *getName() {
			return m_name;
		}
		// ��������ݒ肷��
		void setName(char *name);
	
		// NB6BUSHOU�f�[�^��ł�ID�𓾂�(�ŏ���-1�̂͂��Ȃ̂Œ���)
		// @return: -1=none, 0�`531 �����݂���ꍇ�ɕԂ��Ă��镐��IX
		int getNb6BushouIX();

		// NB6BUSHOU�f�[�^��ł�ID���Z�b�g
		// ���O��ID���킩���Ă���ꍇ�͖��O��肱����̕����ǂ�
		void setNb6BushouIX(int iBushouID);

		// ���m�����W�F�l�I�ɑ��₷���ɗ��p����
		// @param : RefleshTime ���m��1���t��������̂ɂ����鎞�ԁB�~���b�B
		//		  : ������Β����قǑ����ɂ����B
		// @param : isForce ���Ԃɂ�����炸���₷�B�܂������O�ł����₷�ۂɁA�P�Ƃ���B���̎��͌P���l�������Őݒ肳���B
		void checkRegenerationNB6BushouSoldier(int RefleshTime, int isForce = 0);

		// ��C���C������B
		void checkRestoreNB6BushouTaihou();

		// �΂����̂����{�x��
		void checkTimesNB6BushouLife();

		// �΂����Ȃ����ݒ�
		void setLifeTimes(byte lifetimes) {
			m_lifetimes = lifetimes;
		}

};

