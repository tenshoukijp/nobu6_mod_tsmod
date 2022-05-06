#pragma once

#pragma pack(1)


// 汎用名の生成。無限にそれっぽい名前を生み出す
// 名前と名字を列伝1300個を元に自動生成する。名字用と名前用の２つのバッファーを渡すこと。
void getFactoryGeneralName(char *bufFamilyName, char *bufFirstName);


// ゲームで仕様されている姫の名前
char *getRandomHimeName();