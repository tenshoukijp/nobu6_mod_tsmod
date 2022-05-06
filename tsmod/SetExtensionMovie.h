#pragma once

#pragma pack(1)

struct NB6MOVIEPOINTER {
	char *pAddressMovieName;	
};
// extern NB6MOVIEPOINTER nb6_moviepointer_ex[36];
extern NB6MOVIEPOINTER	   *const nb6_moviepointer_ex;

void setExtensionMoviePointer(); 
