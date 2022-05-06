#pragma pack(1)

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <map>

#include "OnigRegex.h"
#include "OutputDebugStream.h"

using namespace std;


// ���K�\���}�b�`���O�p�̊֐�
// @str_target

int OnigMatch( string str_target, string str_regex, Matches* pmatches) {

	// �܂��A�������ʂ͍폜
	if ( pmatches ) {
		pmatches->clear();
	}

	// �ǂ��炩�����g����Ȃ�Afalse��Ԃ��B
	if ( str_target.length() == 0 || str_regex.length() == 0 ) {
		return false;
	}

	int r;
	OnigUChar *start, *range, *end;
	regex_t* reg;
	OnigErrorInfo einfo;
	OnigRegion *region;

	UChar* pattern = (UChar* )str_regex.c_str(); // "��(.+?)(����)��|[��-��]+";
	UChar* str     = (UChar* )str_target.c_str(); // "����\x0A����\x0A������";

	// ���K�\���̍쐬�B SJIS
	r = onig_new(&reg, pattern, pattern + strlen((char* )pattern),
		ONIG_OPTION_IGNORECASE|ONIG_OPTION_MULTILINE, ONIG_ENCODING_SJIS, ONIG_SYNTAX_DEFAULT, &einfo);

	if (r != ONIG_NORMAL) {
		char s[ONIG_MAX_ERROR_MESSAGE_LEN];
		onig_error_code_to_str((OnigUChar *)s, r, &einfo);
		OutputDebugStream("���K�\���R���p�C���G���[: %s\n", s);
		return false;
	}

	region = onig_region_new();

	end   = str + strlen((char* )str);
	start = str;
	range = end;

	// ���K�\���Ō�������B
	r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);
	if (r >= 0) {
		int i;

		// �}�b�`���������S�̂ƁA( )�̒��g�̕\���B
		// fprintf(stderr, "match at %d\n", r);
		for (i = 0; i < region->num_regs; i++) {

			if ( pmatches ) {
				int len = region->end[i]-region->beg[i];
				// �������R�s�[���邽�߂̗̈�𓮓I�Ɋm��
				char *cp = new char[len+1]; // �ŏ��ƍŌ�̍������m�ہBend���P���߂Ȃ̂ŁA�����NULL���������m�ۂ���Ă���B

				// ��������R�s�[
				// �@�@�@�@�@�@���R�s�[�J�n�ʒu      ���R�s�[�o�C�g��
				strncpy( cp, (char *)(str)+region->beg[i], len );
				cp[len] = '\0'; // �Ō��NULL����������B

				// OutputDebugStream("%s", cp);
				pmatches->insert( Matches::value_type( i, cp ) );

				// ���I�Ȋm�ۗ̈���
				delete [] cp;
			}
		}
	}
	else if (r == ONIG_MISMATCH) {
		// OutputDebugStream("search fail\n");
	}
	else { /* error */
		char s[ONIG_MAX_ERROR_MESSAGE_LEN];
		onig_error_code_to_str((OnigUChar *)s, r);
		// OutputDebugStream("���K�\���}�b�`���s�G���[: %s\n", s);
		return false;
	}

	onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
	onig_free(reg);

	if ( r >= 0 ) {
		return true;
	} else {
		return false;
	}
}

// ���ʊi�[�p�ϐ��p��
static map<int, string> extern_matches;

// ��L�֐��̊O���Q�Ɨp
int WINAPI Extern_OnigMatchAlloc(char *str_target, char *str_regex, int *pmatches) {

	int ret = false;
	// ���ʊi�[�p�ϐ����L���Ȃ�΁c
	if (pmatches) {
		// �O��̊i�[���ʏ���
		extern_matches.clear();

		ret = OnigMatch(str_target, str_regex, &extern_matches); // ���ʂ��i�[

		// �����L���ȏ�񂪊i�[����Ă���B
		if (extern_matches.size() > 0) {

			int imax = -1; // map�Ɋi�[����Ă���ő��index���������߂�B�������̈�m�ې�����̂��߁B
			// �}�b�`���Ă��镪���
			for (map<int, string>::iterator it = extern_matches.begin(); it != extern_matches.end(); it++) {
				// �L�[�̒l���Aimax���傫����΁A���ꂪ�ő�̃L�[�̐��l
				if (it->first > imax ) {
					imax = it->first;
				}
			}

			// �ő��index��0�ȏ�ł���΁Amap�ɒ��g���P�ȏ�����Ă�B
			if (imax >= 0 ) {

				// �ŏ�����ő�܂ŁB
				for ( int i=0; i<=imax; i++) {
					// �����Ώۂ̃L�[�����݂��Ȃ���΁c
					if (extern_matches.count(i) == 0) {
						pmatches[i] = NULL; // ���Ƃ��Ƃ��̃L�[�͗L���ł͂Ȃ��B
					} else {
						pmatches[i] = (int)(extern_matches[i].c_str()); // char *�̃A�h���X��int�̐��l�A�h���X�ւƂ���B
					}
				}
				pmatches[imax+1] = 0xFFFF; // �Ō�ɔԕ��p�̒l
			}
		}

	} else {
		ret = OnigMatch(str_target, str_regex); // �}�b�`�̂�
	}

	return ret;
}

void WINAPI Extern_OnigMatchFree(int *pmatches) {
}
