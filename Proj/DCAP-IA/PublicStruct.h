#ifndef PUBLICSTRUCT_DCAP_IA_H
#define PUBLICSTRUCT_DCAP_IA_H
//����DCAP-IA��Ŀ���õ����ݽṹ���Լ�һЩ�������Ŀ��صĹ�ͬ������װ

#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>

#define EQ_CHANNELS	 (16)	//ֻ��16֧������EQ������16���ϵ�ͨ�����ڼ������Ƶ����������ҪEQ����Ƶ����ֻ��Ԥ����Ŀǰ��ʵ��
#define EQ_BANDS (31)  //31��band


#define EQ_CONFIG_FILE_NAME ("ConfigEQ.ini")




//#pragma pack(push,1)     //Align by 1 byte




//�����������ľ���ϵ��
struct  PerChEQCoeff
{
    float bandgain[EQ_BANDS];     //����Ƶ������ dbΪ��λ
    float outgain;                          //������           dbΪ��λ

    PerChEQCoeff()
    {
            outgain=0;
            for (int n=0;n<EQ_BANDS;n++)
            {
               this->bandgain[n]=0;
            }
    }

    bool operator ==(const PerChEQCoeff& R) const
    {
        if (this->outgain!=R.outgain)
        {
            return false;
        }
        for (int n=0;n<EQ_BANDS;n++)
        {
            if (this->bandgain[n]!=R.bandgain[n])
            {
                return false;
            }
        }
        return true;
    }
    bool operator !=(const PerChEQCoeff& R) const
    {
        return !this->operator==(R);
    }
    void print()const
    {
            for (int nband=0;nband<EQ_BANDS;nband++)
            {
                printf("%.1f ",bandgain[nband]);
            }
            printf("level:%.1f\n",outgain);
    }
};

inline void printChanged(const PerChEQCoeff& newCoef,const PerChEQCoeff& oldCoef)
{
    for (int nband=0;nband<EQ_BANDS;nband++)
    {
        if (newCoef.bandgain[nband]!=oldCoef.bandgain[nband])
        {
                printf("band[%d] gain:%.2f ",nband,newCoef.bandgain[nband]);
        }
    }
    if (newCoef.outgain!=oldCoef.outgain)
    {
            printf("level:%.2f",newCoef.outgain);
    }
}

//����������16��ͨ���ľ���ϵ��
struct  EQCoeffAll
{
    bool bEQEnabled;   //����ʹ��
    enum 
    {
        NUM_CH=EQ_CHANNELS,
        NUM_BAND=EQ_BANDS,
    };

    PerChEQCoeff chCoeff[NUM_CH];
    bool operator ==(const EQCoeffAll& R)
    {
        for (int n=0;n<NUM_CH;n++)
        {
            if (this->chCoeff[n]!=R.chCoeff[n])
            {
                return false;
            }
        }
        return true;
    }
    bool operator !=(const EQCoeffAll& R)
    {
        return !this->operator==(R);
    }
    void print()const
    {
        printf("EqCoefAll:isEqEnabled:%d\n",bEQEnabled);
        for (int ch=0;ch<NUM_CH;ch++)
        {
            printf("ch[%d]",ch);
            for (int nband=0;nband<NUM_BAND;nband++)
            {
                printf("%.1f ",chCoeff[ch].bandgain[nband]);
            }
           printf("level:%.1f \n",chCoeff[ch].outgain);
        }
    }
};

//#pragma pack(pop)       //



//��ѹ ������ �� �ֱ���ת����
inline float dBtoGainbyPower(float db)            //������--����
{
    return ::pow(10,db/10.0f);
}
inline float GaintodBbyPower(float gain)         //������--����
{
    return 10.0f*::log10(gain);
}
inline float dBtoGainbyAmplitude(float db)      //��ѹ ����
{
    return ::pow(10,db/20.0f);
}
inline float GaintodBbyAmplitude(float gain)   //��ѹ ����
{
    return 20.0*::log10(gain);
}

inline float DBtoGain(float db)
{
    return dBtoGainbyAmplitude(db);   //gain����Ҫ�����������ȣ�����Ҫ�û��ڷ��ȵ�����ת����ʽ
}

inline float GaintoDB(float gain)
{
    return GaintodBbyAmplitude(gain);
    // return GaintodBbyPower(gain);
}

//////////////////////////////////////////////////////////////////////////
// Add Fader to Gain Convert [8/28/2017 Killerlife]
// Base on Dolby CP750 Manual:
// When the fader knob is rotated between readings of 0 and 4.0,
// the output level changes in 20dB steps between -90 and -10dB.
// When the fader knob is rotated between readings 4.0 and 10, 
// the output level changes in 3.33dB steps between -10 and 10 dB

//������ťֵ������
inline float FaderToGain(float fade)
{
    float dB;
    if(fade < 0.0f)
    {
        dB = -90.0;
    }
    else if(fade >= 0.0f && fade <= 4.0f)
    {
        dB = -90.0f + fade * 20.0;
    }
    else if(fade > 4.0f && fade <= 10.0f)
    {
        dB = -10.0f + (fade - 4)  * 10 / 3;
    }
    else
    {
        dB = 10.0f;
    }
    return DBtoGain(dB);
}


//��������ť�ȼ�������
inline float FaderToDB(float fade)
{
    float gain=FaderToGain(fade);
    float db=GaintodBbyAmplitude(gain);
    return db;
}



//������Ϣ
struct NetAdapterInformation 
{
    int index;
    std::string name;
    std::string ip;
    std::string netmask;
    std::string gateway;
    std::string dns;
    std::string dhcp;
};

////Dcap-UI��½����ȡ�õ�Ȩ��
//enum loginPrivileges
//{
//    loginPri_Unknow=-2,        //û���κ�Ȩ��,��ʼ״̬
//    loginPri_failed=-1,            //��½ֱ�ӷ���ʧ��
//    loginPri_admin=0,            //������
//    loginPri_users,                  //��ͨ�û�
//
//    //����Ȩ��
//    //loginPri_technician,    
//    //loginPri_manager,      
//    //loginPri_operator       
//};



struct logDate
{
    unsigned short year;
    unsigned char  month;
    unsigned char  day;
};

struct STime
{
    unsigned char hour;
    unsigned char minute;
    unsigned char seconds;
};

struct logRequest
{
    logDate dateAfter;
    logDate datebefore;
    int logType;
};


#endif
