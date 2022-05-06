#pragma once

#pragma pack(1)

#include <sys/types.h>
#include <sys/stat.h>

void CreateKahouMovieMCI(char *szMovieFileName, int pos_x, int pos_y );

void DestroyKahouMovieMCI();

void MoveKahouMovieMCI( int pos_x, int pos_y );
