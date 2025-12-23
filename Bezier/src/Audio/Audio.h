#include <irrKlang.h>

#pragma once
class Audio
{
	static irrklang::ISoundEngine* SoundEngine;
public:
	static void PlaySound(const char* file);
};

