/*!
 * \file a53_eq_and_mic.cc
 * \date 2019/10/16 15:46
 *
 * \author jaontolt
 * Contact: jaontolt@leoniscinema.com
 *
 * \brief 
 *
 * TODO: Receive Data from R5, mix Microphone and do 8 channel EQ
 *
 * \note
*/

#include "a53_dsp.h"
#include <string.h>

#ifndef DDR_BASE_ADDR
#define DDR_BASE_ADDR	0x00000000
#endif

#define CMD_BUFFER_BASE		(DDR_BASE_ADDR + 0x40000000)
#define AUDIO_BUFFER_BASE	(DDR_BASE_ADDR + 0x42000000)

#include "struct_define_h.h"

EQ_CMD* eqCmd = (EQ_CMD*)(CMD_BUFFER_BASE);
FADE_BUF* fadeBuf = (FADE_BUF*)(CMD_BUFFER_BASE + sizeof(EQ_CMD));

STRUC_AUDIO_BUF *AudioToA53[2] = {(STRUC_AUDIO_BUF*)AUDIO_BUFFER_BASE, (STRUC_AUDIO_BUF*)(AUDIO_BUFFER_BASE + sizeof(STRUC_AUDIO_BUF))};
STRUC_AUDIO_BUF *AudioToR5[2] = {(STRUC_AUDIO_BUF*)(AUDIO_BUFFER_BASE + sizeof(STRUC_AUDIO_BUF) * 2), (STRUC_AUDIO_BUF*)(AUDIO_BUFFER_BASE + sizeof(STRUC_AUDIO_BUF)*3)};

extern float dBtoGainbyAmplitude(float db);

int main()
{
	//////////////////////////////////////////////////////////////////////////
	// TODO: Init EQ Module

	int current = 0;
	AudioToR5[0]->rwFlag.wide32 = 0;
	AudioToR5[1]->rwFlag.wide32 = 0;

	//////////////////////////////////////////////////////////////////////////
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// Read Data from R5
		//////////////////////////////////////////////////////////////////////////
		
		// Wait for data be ready [10/16/2019 jaontolt]
		while (AudioToA53[current]->rwFlag.wide8.c0 == 0)
		{

		}

		float mic1gain = dBtoGainbyAmplitude(fadeBuf->micInGain1);
		float mic2gain = dBtoGainbyAmplitude(fadeBuf->micInGain2);
		// Mix microphone 1/2 into Center speaker
		for(int i = 0; i < PerFrameSampleMax; i++)
		{
			int mix = AudioToA53[current]->data[2][i] +
				AudioToA53[current]->data[16][i] * mic1gain +
				AudioToA53[current]->data[17][i] * mic2gain;
			if(mix > MAX_INT24)
				AudioToA53[current]->data[2][i] = MAX_INT24;
			else if(mix < MIN_INT24)
				AudioToA53[current]->data[2][i] = MIN_INT24;
			else
				AudioToA53[current]->data[2][i] = mix;
		}

		if(eqCmd->eqState == EQ_BYPASS)
		{
			// Copy Data to R5 buffer directly
			for(int i = 0; i < 8; i++)
			{
				memcpy(&AudioToR5[current]->data[i][0], &AudioToA53[current]->data[i][0], PerFrameSampleMax*sizeof(int));
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// 1.Update EQCMD if have new parameter
			// 2.Do EQ
			if(eqCmd->eqState == EQ_UPDATE)
			{
				GetDsp()->SetAllSpkAmp(eqCmd->spkOutGain);
				GetDsp()->SetAllSpkGain(eqCmd->spkBandGain);
				eqCmd->eqState = EQ_DONOTHING;
			}
			for(int i = 0; i < 8; i++)
				GetDsp()->EqSpkNSample(&AudioToR5[current]->data[i][0], &AudioToA53[current]->data[i][0], i, PerFrameSampleMax);
		}
		// Notify R5 in-data for A53-1 will be release, the buffer can be recycle.
		AudioToA53[current]->rwFlag.wide8.c0 = 0x00;
		// Notify R5 EQ Data from A53-1 is ready
		AudioToR5[current]->rwFlag.wide8.c0 = 0x01;
		current = (current + 1) %2;
	}
	return 0;
}
