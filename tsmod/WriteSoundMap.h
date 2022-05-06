#pragma once
#pragma pack(1)
#include <windows.h>
#include "SoundServer.h"

int CreateSoundFMap();

int WriteSoundFMap(TSoundWaveParam *pWriteData);

int CloseSoundFMap();

