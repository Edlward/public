 #ifndef AudioHelper_H_
#define AudioHelper_H_

#include "global.h" 




NAME_SPACE_MAIN_BEGIN

class BASIC_API  AudioHelper
{
public:
    AudioHelper(void);
    ~AudioHelper(void);
    void initDigitalWave();  //作为单例，为外部可以控制构造顺序

    void getSound(short* p,const char* pStrNum);
    void getSound(float* p,const char* pStrNum);
private:
   short pcmS16D2205[10][12*1000];  //1000 or 1024
   short pcmS16D4410[10][24*1000];
   float pcmfloatD4410[10][24*1000];
};

BASIC_API AudioHelper* GetAudioHelper();


#ifdef UNIT_TEST
BASIC_API int Test_AudioHelper();
#endif

NAME_SPACE_MAIN_END

#endif
