#include "Audio.h"
#include <string>
void Audio::PlaySound(const char* file)
{
    std::string filePath = "res/audio/";
    filePath += file;
    filePath += ".wav";
    SoundEngine->play2D(filePath.c_str());
}
irrklang::ISoundEngine* Audio::SoundEngine = irrklang::createIrrKlangDevice();