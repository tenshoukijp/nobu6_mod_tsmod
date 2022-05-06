#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>

const char *suuji[] = {
    "Z", "ˆê", "“ñ", "O", "l",
    "ŒÜ", "˜Z", "µ", "”ª", "‹ã",
};

const char *keta[] = {
    "Z",
    "ˆê", "\", "•S", "ç",
    "–œ", "\", "•S", "ç",
    "‰­", "\", "•S", "ç",
    "’›", "\", "•S", "ç",
};

char *arabia2kanji(const char *arb, char *knj)
{
    unsigned i, kt1, kt2, num, len, flag;
    char *sav = knj;
    *knj = '\0';
    flag = 0;
    len = strlen(arb);
    if (len < sizeof(keta)/sizeof(keta[0])) {
        for (i=0; i<len; i++) {
            num = *(arb+i) - '0'; /* ”’l */
            kt1 = len - i; /* Œ…ˆÊ’u */
            kt2 = kt1 % 4;
            if (num && (num > 1 || kt2 == 1 )) {
                strcat(knj, suuji[num]);
                flag = 1;
            }
            if (kt1 > 1 && (num || kt2 == 1)) {
                if (flag || num == 1) {
                    strcat(knj, keta[kt1]);
                    flag = 1;
                }
            }
            if (kt2 == 1) flag = 0;
        }
    }
    return sav;
}