#define _CRT_SECURE_NO_WARNINGS

#include "GameDataStruct.h"
#include <algorithm>
#include "Arabia2Kanji.h"

/*
 YYYY/MM/DD���猳���𓾂�
 */

struct NB6GENGO {
	char gengou[9];
	int start_YMD;
};

NB6GENGO gengo_list[] = {
	""	  ,        0,
	"�哯",  8060608,
	"�O�m",  8101020,
	"�V��",  8240208,
	"���a",  8340214,
	"�Ï�",  8480716,
	"�m��",  8510601,
	"�čt",  8541223,
	"�V��",  8570320,
	"���",  8590520,
	"���c",  8770601,
	"�m�a",  8850311,
	"����",  8890530,
	"����",  8980520,
	"����",  9010831,
	"����",  9230529,
	"����",  9310516,
	"�V�c",  9380622,
	"�V��",  9470515,
	"�V��",  9571121,
	"���a",  9610305,
	"�N��",  9640819,
	"���a",  9680908,
	"�V�\",  9700503,
	"�V��",  9740116,
	"�匳",  9760811,
	"�V��",  9781231,
	"�i��",  9830529,
	"���a",  9850519,
	"�i��",  9870505,
	"�i�N",  9890910,
	"����",  9901126,
	"����",  9950325,
	"����",  9990201,
	"���O", 10040808,
	"���a", 10120208,
	"���m", 10170521,
	"����", 10210317,
	"����", 10240819,
	"����", 10280818,
	"����", 10370509,
	"���v", 10401216,
	"����", 10441216,
	"�i��", 10460522,
	"�V��", 10530202,
	"�N��", 10580919,
	"����", 10650904,
	"���v", 10690506,
	"����", 10740916,
	"����", 10771205,
	"�i��", 10810322,
	"����", 10840315,
	"����", 10870511,
	"�Õ�", 10950123,
	"�i��", 10970103,
	"����", 10971227,
	"�N�a", 10990915,
	"����", 11040308,
	"�Ï�", 11060513,
	"�V�m", 11080909,
	"�V�i", 11100731,
	"�i�v", 11130825,
	"���i", 11180425,
	"�ۈ�", 11200509,
	"�V��", 11240518,
	"�厡", 11260215,
	"�V��", 11310228,
	"����", 11320921,
	"�ۉ�", 11350610,
	"�i��", 11410813,
	"�N��", 11420525,
	"�V�{", 11440328,
	"�v��", 11450812,
	"�m��", 11510214,
	"�v��", 11541204,
	"�ی�", 11560518,
	"����", 11590509,
	"�i��", 11600218,
	"����", 11610924,
	"����", 11630504,
	"�i��", 11650714,
	"�m��", 11660923,
	"�É�", 11690506,
	"����", 11710527,
	"����", 11750816,
	"����", 11770829,
	"�{�a", 11810825,
	"���i", 11820629,
	"����", 11840527,
	"����", 11850909,
	"���v", 11900516,
	"����", 11990523,
	"���m", 12010319,
	"���v", 12040323,
	"���i", 12060605,
	"����", 12071116,
	"����", 12110423,
	"����", 12140118,
	"���v", 12190527,
	"�剞", 12220525,
	"���m", 12241231,
	"�Ø\", 12250528,
	"����", 12280118,
	"����", 12290331,
	"��i", 12320423,
	"�V��", 12330525,
	"����", 12341127,
	"�Ò�", 12351101,
	"��m", 12381230,
	"����", 12390313,
	"�m��", 12400805,
	"����", 12430318,
	"��", 12470405,
	"����", 12490502,
	"�N��", 12561024,
	"����", 12570331,
	"����", 12590420,
	"����", 12600524,
	"�O��", 12610322,
	"���i", 12640327,
	"����", 12750522,
	"�O��", 12780323,
	"����", 12880529,
	"�i�m", 12930906,
	"����", 12990525,
	"����", 13021210,
	"�Ì�", 13030916,
	"����", 13060118,
	"���c", 13081122,
	"����", 13110517,
	"���a", 13120427,
	"����", 13170316,
	"����", 13190518,
	"����", 13210322,
	"����", 13241225,
	"�×�", 13260528,
	"����", 13290922,
	"���O", 13310911,
	"����", 13340305,
	"�", 13381011,
	"�N�i", 13420601,
	"��a", 13451115,
	"�ω�", 13500404,
	"���a", 13521104,
	"����", 13560429,
	"�N��", 13610504,
	"�厡", 13621011,
	"����", 13680307,
	"�i�a", 13750329,
	"�N��", 13790409,
	"�i��", 13810320,
	"����", 13840319,
	"�Ìc", 13871005,
	"�N��", 13890307,
	"����", 13900412,
	"���i", 13940802,
	"����", 14280610,
	"�i��", 14291003,
	"�Ëg", 14410310,
	"����", 14440223,
	"��", 14490816,
	"����", 14520810,
	"�N��", 14550906,
	"���\", 14571016,
	"����", 14610201,
	"����", 14660314,
	"���m", 14670409,
	"����", 14690608,
	"����", 14870809,
	"����", 14890916,
	"����", 14920812,
	"���T", 15010318,
	"�i��", 15040316,
	"��i", 15210923,
	"���\", 15280903,
	"�V��", 15320829,
	"�O��", 15551107,
	"�i�\", 15580318,
	"���T", 15700527,
	"�V��", 15730825,
	"���\", 15930110,
	"�c��", 15961216,
	"���a", 16150905,
	"���i", 16240417,
	"����", 16450113,
	"�c��", 16480407,
	"����", 16521020,
	"����", 16550518,
	"����", 16580821,
	"����", 16610523,
	"����", 16731030,
	"�V�a", 16811109,
	"�勝", 16840405,
	"���\", 16881023,
	"��i", 17040416,
	"����", 17110611,
	"����", 17160809,
	"����", 17360607,
	"����", 17410412,
	"����", 17440403,
	"����", 17480805,
	"���", 17511214,
	"���a", 17640630,
	"���i", 17721210,
	"�V��", 17810425,
	"����", 17890219,
	"���a", 18010319,
	"����", 18040322,
	"����", 18180526,
	"�V��", 18310123,
	"�O��", 18450109,
	"�Éi", 18480401,
	"����", 18550115,
	"����", 18600408,
	"���v", 18610329,
	"����", 18640327,
	"�c��", 18650501,
	"����", 18681023,
	"�吳", 19120730,
	"���a", 19261225,
	"����", 19890108,
	"�ߘa", 20190501,
};



// �N�E���E����茳��������𓾂�B���E�����ȗ������ꍇ�́A12��31��(�N���̓�)�ƌ��Ȃ��B
// ���R�͓V�ċL�ł́A�Ƃ���N�̓r���Ō������ς���Ă���ꍇ�A�V�����̕�����ɍ̗p���邽�߁B
NB6WAREKI getWarekiFromYMD(int iYear, int iMonth, int iDay) {

	// �Ԃ�l�^
	NB6WAREKI result = {0};

	if ( iYear==-1 ) {
		iYear=getYearAD();
	}
	if ( iMonth==-1 ) {
		iMonth=12;
	}
	if ( iDay==-1 ) {
		iDay=31;
	}
	int iYMD = iYear*10000+iMonth*100+iDay;

	int GENGO_NUM = sizeof(gengo_list)/sizeof(gengo_list[0]);
	for ( int i=GENGO_NUM-1; i >= 0; i-- ) {
		if ( iYMD >= gengo_list[i].start_YMD ) {
			// �����𕡐�
			strcpy( result.gengou, gengo_list[i].gengou );

			int iStartGengoYear = gengo_list[i].start_YMD / 10000; // �J�n�N�𓾂�B
			result.iyear = (iYear-iStartGengoYear)+1; // �a����ł̔N������B���N�͂O�Ƃ�������

			// ���N�͓��ʂȌ�����
			if ( result.iyear == 1 ) {
				strcpy( result.kanjiyear, "��" ); // �J�n�N�͓��ʂɁu���N�v�Ƃ���������������B
				return result;

			// ����ȊO�͌v�Z
			} else {
				// ���N�ȊO�͂P���ϊ����K�v�B
				_itoa( result.iyear, result.kanjiyear, 10); // ��[�����ɐ��l�������ɂ������̂��R�s�[
				char szBuf[15]; // �S�p�����p�o�b�t�@
				arabia2kanji( (const char *)result.kanjiyear, szBuf ); // ���p�������S�p����
				strcpy( result.kanjiyear, szBuf ); // ���p�����ɏ㏑��

				return result;
			}
		}
	}

	// �����܂ŗ����画��ł��Ȃ��������́B�����l�I�Ȃ��̂�Ԃ�
	strcpy( result.gengou, "");
	strcpy( result.kanjiyear, "");
	result.iyear = 0;
	return result;
}

