#ifndef SpkInfo_Header
#define SpkInfo_Header

//������������������Qt��Ŀ���ܻ��õ���
//����������������Qt����,��Qt��Ŀ����ʹ��SpkInfoExp.h


//#include <QColor>
//#include <QString>


#include <string>
#include <vector>

//typedef unsigned int QRgb;         // RGB triplet
typedef unsigned int MyRgb;         //��QRgb��ͬ,����QColor��,��ʾ��ɫֵ,������QColor����ת��



class  RoomDim
{
public:
    RoomDim(float l=0,float w=0,float h=0)
        :length(l),width(w),height(h)
    {
    }
    float width;
    float length;
    float height;
    std::string toSrt() const
    {
        char tmpstr[64];
        sprintf(tmpstr,"L:%.2fm W:%.2fm H:%.2fm",length,width,height);
        return std::string(tmpstr);
    }
};


//�������������ƺ궨��,Core��UI��ͬ���걣��һ�� rafactor by lizulin 20200707
//��ʷ������Ϊ�˼��ݾ�Ӱ�����ã��ѹ̶����������޸�.�ж����ʱ���Դ�Сд��

////���������ͼ�д��ʽ
//�����������ַ���
#define SPKTYPE_SCREEN		"Screen"
#define SPKTYPE_SURROUND	"Surround"
#define SPKTYPE_SUBWOOFER	"Subwoofer"
#define SPKTYPE_LFE	"LFE"  //��Ϊ�豸����ʱת��   "Subwoofer"
#define SPKTYPE_UNKNOW	"UnKnow"


//���������Ϳ��ַ�����ʽ
#define SPKTYPE_SCREEN_WS		L"Screen"
#define SPKTYPE_SURROUND_WS	L"Surround"
#define SPKTYPE_SUBWOOFER_WS	L"Subwoofer"
#define SPKTYPE_LFE_WS	"LFE"  //��Ϊ�豸����ʱת��   "Subwoofer"
#define SPKTYPE_UNKNOW_WS	L"UnKnow"

//����������������ʽ,����ʹ��
////�����������ַ���
//#define SPKTYPE_SCREEN_LONG		"screenSpeaker"
//#define SPKTYPE_SURROUND_LONG	"surroundSpeaker"
//#define SPKTYPE_SUBWOOFER_LONG	"SubwooferSpeaker"
//
////���������Ϳ��ַ�����ʽ
//#define SPKTYPE_SCREEN_LONG_WS		L"screenSpeaker"
//#define SPKTYPE_SURROUND_LONG_WS	L"surroundSpeaker"
//#define SPKTYPE_SUBWOOFER_LONG_WS	L"SubwooferSpeaker"


//��Begin-End֮��Ĳ�����ȷ����������
enum ESpkType
{
    SpkType_Unknow=-1,         //δ֪������
    SpkType_Begin=SpkType_Unknow,
    SpkType_Screen,           //��Ļ����������
    SpkType_Surround,       //����������

    //LFE������Ҳ�ǵ���������,ֻ����Ӳ������ʱ���Ҿ����豸ʱ�ŵ���subwoofer,����ʱ�򵥶���Ϊһ��,
    //ע��˲����Ƿ����bug
    SpkType_Subwoofer,     //���������� 
    SpkType_LFE,                 

    SpkType_End,
    SpkType_Total=SpkType_End,
};

//�ж�������ö��ֵ�Ƿ���Ч
inline bool isValidEnum(ESpkType spkType){ return (spkType>SpkType_Begin)&&(spkType<SpkType_End);}



//��¼������λ�����Ƶ�������Ϣ  ��״ ������������ת90����ʾ
struct  SpeakerBaseInfo
{
    SpeakerBaseInfo()
    {
        m_x=0; 
        m_y=0;    
        m_z=0;
        m_delay=0; 
        m_SpkType=SpkType_Unknow;
    }
    float m_x;    //������λ��
    float m_y;    
    float m_z;

    //ע���ר�����ж�ƽ�������Ƿ��غ�,
    //bool operator ==(const SpeakerBaseInfo& R) const
    //{
    //    return (this->m_x == R.m_x && this->m_y == R.m_y);
    //}
    //refactor 20200706 Ϊ�˲��ƻ�����,��ר�ź���ʵ��
    bool isSamePos(const SpeakerBaseInfo& R)  //����Ƿ���ͬ
    {
            return (this->m_x == R.m_x && this->m_y == R.m_y);
    }


    std::string toPosStr(int Decimal=2)const    //ת����x*y*z ��ʽ���ַ��� (x,y,z)
    {
        char tmp[64];
        if (Decimal==2)
        {
            sprintf(tmp,"(%.2f,%.2f,%.2f)\n",m_x,m_y,m_z);
        }
        else
        {
            sprintf(tmp,"(%.1f,%.1f,%.1f)\n",m_x,m_y,m_z);
        }
        return std::string(tmp);
    }
    std::string sName;                     //����������
    std::string sArrayTypeName;     //������������������
    double m_delay;                        //��������ʱ��λ��ʱֵ

   //����������������screenSpeaker(��Ļ����������), surroundSpeaker(����������) SubwooferSpeaker(����������)
    std::string sSpeakerTypeName;  
    //int m_SpkType;                              //����������ö��ֵ,ESpkType
    ESpkType m_SpkType;                     //����������ö��ֵ,ESpkType
};


//׼���浽�����ļ�ʱ����������LFEҪת��Subwoofer,�������Ͳ���,Ӳ���豸�ⲻ֧��LFE���͸���
std::string SpeakerTypeNameToDeviceType(const std::string& sSpeakerTypeName);
ESpkType SpeakerTypeToDeviceType(const ESpkType& spkType);


class RoomItem;
struct BassManagerInfo
{
    RoomItem* pItem;               //���ص�ʱ�����ã������ʱ��Ҳ����� Ԥ��
    std::string  strID;		            //��ͨ������ID����
    MyRgb SubwooferColor;      //��ͨ�������ı���ɫ
    int crossoverFrequencyHz;
    int gainDb;
};


//���  ��������������Ϣ�ṹ��
struct SpeakerInfo :public SpeakerBaseInfo
{
    SpeakerInfo()
    {
        m_delay=0.0;
        m_ishighLight=false;
        m_DoutNum=-1;
        m_AoutNum=-1;
        m_Number=0;
        m_isSubwoofer=false;
        m_isAutoEQDone=false;
    }
    //SpeakerInfo& operator=(const SpeakePosInfo& R)
    //{
    //    return *this;
    //}

    bool m_ishighLight;    //�Ƿ����

    //Routing���
    std::string sOutputRef;  //ӳ�䲼����Ϣ ��SPKREP-0  ��<outputRef>OUT-BL-0ֱ�ӵ�ӳ���ϵ ����DoutNum�仯
    std::string sAmplifierRef;               //>AMP-CUSTOM-29</amplifierRef>
    std::string sSpeakerProcessorRef;  //>PROC-CUSTOM-29</speakerProcessorRef>
    std::string sAnalogOutput;

    MyRgb SubwooferBassManagerColor;    //��ͨ������ֻ��һ�̶ֹ���ɫ����ʼ��ʱ���������Ϊ����������ʱ��ı���ɫ
    std::vector<MyRgb>  VBassManagerColor;   //��Ϊ��ͨ������������ɫ�����������Ի�  
    std::vector<BassManagerInfo>  VBassManagerInfo;   //���浱ǰ���������������Щ��ͨ������

    int m_DoutNum;   //�������--��һ���������ЧID,-1��ʾ��Ч��δ�趨
    int m_AoutNum;   //ģ�����--��ʱ����
    int m_Number;    //��view��ӵ�ʱ���ÿһ��itemһ����š�//��viewά����֤itemΨһ�����ǿ���ɾ�����ȱ������


    bool  m_isSubwoofer;               //������������ǣ���ת90����ʾ
    bool  m_isAutoEQDone;            //���AutoEQ��־�����Ʋ�ͬ��ɫ�Ա�����
    MyRgb BackgroundColor;      
    std::string sID;                         //������ID�ַ���    ��"SPKREP-1""SPKREP-2" ����m_Number����

    std::vector<std::string> VArrayList;  //���� ������������,
    std::string sSpecsRef;


    //new add 20200711 ������Ŀ��������
    std::string sSpkDeviceName;     //ʵ��ѡ����������ͺ����� ���ݿ����
    std::string sAmpDeviceName;   //ʵ��ѡ��Ĺ����ͺ�����    ���ݿ����
};


enum EFreq
{
    E_LF,       //��Ƶ
    E_MF,     //��Ƶ
    E_HF,      //��Ƶ
    E_FSize,
    //E_VHF,    //�е�����������,�ݲ�֧�ִ˸���
};

enum EOhm
{
    Ohm2,         //2ŷķ
    Ohm4,         //4ŷķ
    Ohm8,         //8ŷķ
    Ohm16,       //16ŷķ
    Ohm_Size,
};

inline int OhmValue(EOhm eo)
{
    switch (eo)
    {
    case Ohm2:return 2;break;
    case Ohm4:return 4;break;
    case Ohm8:return 8;break;
    case Ohm16:return 16;break;
    default:return 0;break;
    }
}


//ȫ���εĹ���ֵFullBandPower
struct  FBPower
{
    float powerAll;              //���ڻ��ƣ�һ��ֻ��һ������
    float  power[E_FSize];    //���и�Ƶ�εĹ���
    FBPower()
    {
          setValue(0,0,0,0);
    }

    //��Ƶ�εĸ���ֵȫ�����,һ�����ڲ�����
    bool isAllEqual() const
    {
        float val=power[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (power[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

   //FBPower(float power=0)
   //{
   //       setValue(power,power,power,power);
   //}
   //FBPower(float powerAll,float powerL,float powerM)
   //{
   //       setValue(powerAll,powerL,powerM,powerM);
   //}
   FBPower(float powerAll,float powerL,float powerM,float powerH)
   {
          setValue(powerAll,powerL,powerM,powerH);
   }
   //void operator=(float power)
   //{
   //         setValue(power,power,power);
   //}
   void setValue(float powerAll,float powerL,float powerM,float powerH)
   {
        this->powerAll=powerAll;
        power[E_LF]=powerL;
        power[E_MF]=powerM;
        power[E_HF]=powerH;
   }

   //������������ȡƽ��ֵ/���ֵ/�ܺ�,��������Ϊ����ѡ�Ͳο�Ӧ���ǵ��и�Ƶ���ܺͺ��ʡ�  ����
   float getPower() const
   {
       //power=(powerL+powerM+powerH)/3;      //����ȡƽ��ֵ
       //power=powerL+powerM+powerH;           //���ʵ��и�Ƶ���ܺ�
       //power=Max3(powerL,powerM,powerH);    //����ȡ���и�Ƶ�����ֵ

       //��������Ƶ��ֵ��ֱ�ӷ���,
       if (powerAll>1)
       {
           return powerAll;
       }

       //���ظ���Ƶ��ֵ���
       return  power[E_LF]+power[E_MF]+power[E_HF];
   }
};



//ȫ������ѹ��ֵFullBandSPL spl
struct  FBSPL
{
    float  spl[E_FSize];

    //��Ƶ�εĹ���ֵȫ�����,һ�����ڲ�����
    bool isAllEqual() const
    {
        float val=spl[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (spl[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FBSPL(float spl=0)
    {
        setValue(spl,spl,spl);
    }
    FBSPL(float splL,float splM)
    {
        setValue(splL,splM,splM);
    }
    FBSPL(float splL,float splM,float splH)
    {
        setValue(splL,splM,splH);
    }
    void operator=(float spl)
    {
        setValue(spl,spl,spl);
    }
    void setValue(float splL,float splM,float splH)
    {
        spl[E_LF]=splL;
        spl[E_MF]=splM;
        spl[E_HF]=splH;
    }
};



//ȫ���ε��迹ֵFullBandImpedance 
struct  FBImpedance
{
    int  impedance[E_FSize];
    //��Ƶ�εĸ���ֵȫ�����,һ�����ڲ�����
    bool isAllEqual() const
    {
        int val=impedance[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (impedance[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FBImpedance(int impedance=0)
    {
        setValue(impedance,impedance,impedance);
    }
    FBImpedance(int impedanceL,int impedanceM)
    {
        setValue(impedanceL,impedanceM,impedanceM);
    }
    FBImpedance(int impedanceL,int impedanceM,int impedanceH)
    {
        setValue(impedanceL,impedanceM,impedanceH);
    }
    void operator=(int impedance)
    {
        setValue(impedance,impedance,impedance);
    }
    void setValue(int impedanceL,int impedanceM,int impedanceH)
    {
        impedance[E_LF]=impedanceL;
        impedance[E_MF]=impedanceM;
        impedance[E_HF]=impedanceH;
    }
    //�迹ֵת���ַ���4|8|8 ��ʽ
    std::string toStr() const
    {
        char tmp[128];
        if (impedance[E_LF]==impedance[E_MF]&&impedance[E_MF]==impedance[E_HF])
        {
            sprintf(tmp,"%d",(int)impedance[E_LF]);
        }
        //if (tmp,impedance[E_MF]!=impedance[E_HF])
        else
        {
            sprintf(tmp,"%d|%d|%d",(int)impedance[E_LF],(int)impedance[E_MF],(int)impedance[E_HF]);
        }
        return std::string(tmp);
    }
};





//ȫ�迹�Ĺ���ֵFullImpedancePower,Ϊ0��ʾ��֧�ִ��迹ֵ
struct  FIPower
{
    float  power[Ohm_Size];
    bool isAllEqual() const
    {
        float val=power[0];
        for (int n=0;n<Ohm_Size;n++)
        {
            if (power[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FIPower(float power2=0,float power4=0,float power8=0,float power16=0)
    {
        setValue(power2,power4,power8,power16);
    }
    void setValue(float power2=0,float power4=0,float power8=0,float power16=0)
    {
        power[Ohm2]=power2;
        power[Ohm4]=power4;
        power[Ohm8]=power8;
        power[Ohm16]=power16;
    }
    void setPower2(float power2)
    {
        power[Ohm2]=power2;
    }
    void setPower4(float power4)
    {
        power[Ohm4]=power4;
    }
    void setPower8(float power8)
    {
        power[Ohm8]=power8;
    }
    void setPower16(float power16)
    {
        power[Ohm16]=power16;
    }

    std::string toPowerStr()const
    {
        std::string s;
        int pw;   //old float pw;
        pw=power[Ohm2];
        if (pw>0)
        {
            s+="2��:";
            s+=IntToString(pw);
            s+="W ";
        }
         pw=power[Ohm4];
        if (pw>0)
        {
            s+="4��:";
            s+=IntToString(pw);
            s+="W ";
        }
        pw=power[Ohm8];
        if (pw>0)
        {
            s+="8��:";
            s+=IntToString(pw);
            s+="W ";
        }
        pw=power[Ohm16];
        if (pw>0)
        {
            s+="16��:";
            s+=IntToString(pw);
            s+="W ";
        }
        return s;
    }

    float getPowerOfImpedance(int ohm) const   //float ohm
    {
        float powerSelectd=0;
        //if ((int)ohm==2)
        if (abs(ohm-2) <0.001)
        {
            powerSelectd=power[Ohm2];
        }
        else if (abs(ohm-4) <0.001) //if ((int)ohm==4)
        {
            powerSelectd=power[Ohm4];
        }
        else if (abs(ohm-8) <0.001) //if ((int)ohm==8)
        {
            powerSelectd=power[Ohm8];
        }
        else if (abs(ohm-16)<0.001) //if ((int)ohm==16)
        {
            powerSelectd=power[Ohm16];
        }
        //PreDprintf("Impedance:%f power:%f\n",ohm,powerSelectd);
        return powerSelectd;
    }


    static std::string FloatToString(float val)
    {
        char buf[64];
        sprintf(buf,"%.1f",val);
        std::string tmp(buf);
        return tmp;
    }
    static std::string IntToString(int val)
    {
        char buf[64];
        sprintf(buf,"%d",val);
        std::string tmp(buf);
        return tmp;
    }
};




//һЩ����˵��
//LFE(Low Frequency Effects) ��ƵЧ������
//Subwoofer  ������ �ص���
//booth �ο�dolby һ��ָ���Ǻ�ǽ 


//����������˵��:�ο�dolby
//Screen,������Ļ��������������L/R/C
//Surround������������,����LSS/RSS LTS/RTS LRS/RRS
//Subwoofer������ѡ�͵�ʱ��LFE ��Subwoofer ��ͬһ��

#include "Super/Basic/Tstring.h"


//�������������������ṹ��
struct  PerSpkInfo
{
    std::string sFullName;          //������Ψһ��������,�ɳ�����д+����ƴ�����ɣ�����ʾ
    std::string sFactoryShort;     //������д������Ӣ��
    std::string sName;               //����������,��ͬ���ұ��뱣֤Ψһ

    std::string sSpkType;           //��������������
    ESpkType SpkType;             //����������ö��ֵ

    std::string sFactory;             //����,����������,�������Զ���������д��һ��
    std::string sSeries;               //�������ڳ�������ϵ��,����Ԥ��,����Ϊ��,

    //һЩ����̫�ң�ͬʱά���ַ�������ֵ
    std::string  sDimensions;     //���γߴ��ַ���
    std::string  sWeight;
    std::string  sPower;
    std::string  sImpedance;
    std::string  sSPL;
    float length,width,height;    //�����,��λmm
    float weight;                       //������λkg
    float power;                        //���ʵ�λW
    int impedance;                //�迹��λŷķ
    float SPL;                           //��ѹ��,��λdB

    //
    std::string  sSensitivity;               //������
    std::string  sResponseFreq;         //��ӦƵ��
    std::string  sCrossoverFreq;         //��Ƶ��
    //ֵ��ģ����
    float Sensitivity;
    float LowResponseFreq;
    float HeightResponseFreq;
    float CrossoverFreq;


    float cost;                          //���ϳɱ�,��λԪ
    std::string  sRemark;         //��ע

    //���Ͻ���֧�ָ��ָ��ֲ����ľ�ȷֵ--���ƴ���
    FBPower fbPower;                         //��Ƶ�εĹ���ֵ
    FBImpedance fbImpedance;         //��Ƶ�ε��迹ֵ
    FBSPL  fbspl;                                 //��Ƶ�ε���ѹ��ֵ
    PerSpkInfo()
    {
        length=width=height=0.0f;
        weight=0.0f;
        power=0.0f;
        impedance=0;
        cost=0.0f;
        SPL=105;
    }

    //���ַ�����Ϣ,����ˢ�¸������Ե�ʵ��ֵ,�Ա��ڱ��ֱ��ʹ��
    void updateValueFromString();
    void print()const
    {
        printf("Spk:fullName:%s\n",sFullName.c_str());
        printf("SpkType:%d str:%s\n",SpkType,sSpkType.c_str());
        //printf("Factory:%s Series:%s\n",sFactory.c_str(),sSeries.c_str());
        //printf("Dimensions:%s Weight:%s\n",sDimensions.c_str(),sWeight.c_str());
        //printf("Power:%s Impedance:%s\n",sPower.c_str(),sImpedance.c_str());
        //printf("SPL:%s Remark:%s\n",sSPL.c_str(),sRemark.c_str());

        //xml�ļ��洢����utf8����,ת�ɿ���̨gb2312֮������ʾ
        printf("Factory:%s Series:%s\n",utf8_to_gb2312(sFactory).c_str(),utf8_to_gb2312(sSeries).c_str());
        printf("Dimensions:%s Weight:%s\n",utf8_to_gb2312(sDimensions).c_str(),utf8_to_gb2312(sWeight).c_str());
        printf("Power:%s Impedance:%s\n",utf8_to_gb2312(sPower).c_str(),utf8_to_gb2312(sImpedance).c_str());
        
        printf("SPL:%s Remark:%s\n",utf8_to_gb2312(sSPL).c_str(),utf8_to_gb2312(sRemark).c_str());

        printf("Sensitivity:%s \n",utf8_to_gb2312(sSensitivity).c_str());
        printf("ResponseFreq:%s \n",utf8_to_gb2312(sResponseFreq).c_str());
        printf("CrossoverFreq:%s \n",utf8_to_gb2312(sCrossoverFreq).c_str());

        printf("Dim: L:%f W:%f H:%f Weight:%f Cost:%f\n",length,width,height,weight,cost);
        printf("power: %f impedance:%d SPL:%f\n",power,impedance,SPL);
    }
};





//�����������������ӷ�ʽ
enum EConnectType
{
    ConType_Stereo,       //������ģʽ,ÿ������ͨ������һ��������
    ConType_Bridge,       //�Ž�ģʽ��ÿ��������ͨ������һ��������
    ConType_Parallel,     //����ģʽ���ݲ�֧��
    ConType_Unknow,    //
};



//���ʷŴ������ݿ����,�ⲻ��̫��,����xmlʵ�֡�


//�������ʷŴ������������ṹ��
struct  PerAmpInfo
{
    std::string sFullName;      //����Ψһ��������,�ɳ�����д+����ƴ�����ɣ�����ʾ
    std::string sFactoryShort; //������д������Ӣ��
    std::string sName;           //��������,��ͬ���ұ��뱣֤Ψһ

    std::string sAmpType;     //������������,��ʱֻ��һ��
    //ESpkType AmpType;      //����������

    std::string sFactory;             //����,����������,�������Զ���������д��һ��
    std::string sSeries;               //�������ڳ�������ϵ��,����Ԥ��,����Ϊ��,

    //һЩ����̫�ң�ͬʱά���ַ�������ֵ
    std::string  sDimensions;     //���γߴ��ַ���
    std::string  sWeight;
    unsigned int ChannelNum;    //ͨ��������,һ�㹦��2ͨ��,LEONIS����32ͨ��
    std::string  sPower;
    std::string  sBridgePower;     //�Žӹ��ʺܶ�ʱ�򲢷����������ʵ�2��,�õ����б�ʾ
    std::string  sImpedance;


    float length,width,height;    //�����,��λmm
    float weight;                        //������λkg
    //float power;                      //���ʵ�λW
    int impedance;                   //�迹��λŷķ

    std::string sVoltageGain;   //��ѹ����Voltage Gain(dB)
    float voltageGain;

    //
    float cost;                       //���ϳɱ�,��λԪ
    std::string  sRemark;      //��ע


    //������������������ģʽ��ռ�õ�ͨ��������ʱ�Ŵ˴���
    EConnectType conType;       //����ģʽ
    unsigned int perSpkUseChs;          //ÿһ����������ռ�ù��ŵ�ͨ����

    //���Ͻ���֧�ָ��ָ��ֲ����ľ�ȷֵ--���ƴ���
    FIPower fiPower;                           //���迹ֵ��Ӧ�Ĺ���,Ϊ0��ʾ��֧�ִ��迹

    //�߼�֧�ִ���
    bool bSupportBridge;                     //�Ƿ�֧���Ž�ģʽ
    FIPower fiBridgePower;                 //�Ž�ģʽ,���迹ֵ��Ӧ�Ĺ���,Ϊ0��ʾ��֧�ִ��迹

    //�������ʱ���Žӹ��ʵ������������ʵ�2��,�����ڿ����������ȷ����

    PerAmpInfo()
    {
        length=width=height=0.0f;
        weight=0.0f;
        //power=0.0f;
        impedance=0.0f;
        cost=0.0f;

        voltageGain=0;


        conType=ConType_Stereo;       //����ģʽ
        perSpkUseChs=1;                     //ÿһ����������ռ�ù��ŵ�ͨ����
    }

    //�Ӹ����ַ�������������ֵ������ʵ������
    void updateValueFromString();

    //��ȡ�����������ӷ�ʱĳ���迹(2/4/816)���������ֵ,�Ҳ�������0
    float getPowerOfImpedance(float ohm) const
    {
        return fiPower.getPowerOfImpedance(ohm);
    }
    //��ȡ�����Žӽӷ�ʱĳ���迹(2/4/816)���������ֵ,�Ҳ�������0
    float getBridgePowerOfImpedance(float ohm) const
    {
        return fiBridgePower.getPowerOfImpedance(ohm);
    }


    float powerSelectd;          //�����Ѿ�ѡ��������������,��ʱ����ԭpower
    int impedanceSelectd;     //ѡ�õ��迹ֵ,������ʾ

    void print()const
    {
        printf("Spk:fullName:%s\n",sFullName.c_str());
        //printf("Factory:%s Series:%s\n",sFactory.c_str(),sSeries.c_str());
        //printf("Dimensions:%s Weight:%s\n",sDimensions.c_str(),sWeight.c_str());
        //printf("Power:%s Impedance:%s\n",sPower.c_str(),sImpedance.c_str());
        //printf("Remark:%s\n",sRemark.c_str());

        //xml�ļ��洢����utf8����,ת�ɿ���̨gb2312֮������ʾ
        printf("Factory:%s Series:%s\n",utf8_to_gb2312(sFactory).c_str(),utf8_to_gb2312(sSeries).c_str());
        printf("Dimensions:%s Weight:%s\n",utf8_to_gb2312(sDimensions).c_str(),utf8_to_gb2312(sWeight).c_str());
        printf("Power:%s\n",utf8_to_gb2312(sPower).c_str());
        printf("BridgePower:%s\n",utf8_to_gb2312(sBridgePower).c_str());
        printf("Impedance:%s\n",utf8_to_gb2312(sImpedance).c_str());
        printf("VoltageGain:%s\n",utf8_to_gb2312(sVoltageGain).c_str());
        printf("Remark:%s\n",utf8_to_gb2312(sRemark).c_str());

        printf("Dim: L:%f W:%f H:%f Weight:%f Channels:%u Cost:%f\n"
            ,length,width,height,weight,ChannelNum,cost);

        printf("powerSelectd: %d impedance:%f\n",powerSelectd,impedance);
    }

};







////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////������Ҫ��������RoomConfig��������ò����������ڲ�ͬģ���޸Ĵ���
//class  SpeakerItem
//{
//public:
//	SpeakerItem();
//	~SpeakerItem();
//};





class  RoomConfig
{
public:
	RoomConfig();    //ʹ��Ĭ��ֵ
	~RoomConfig();
	void setDefault();    //��λΪ0
	void setZero();    //��λΪ0

    //��������Ϣ��ȡ�򻯵��������ߴ�,��designerģ��
    RoomDim getRoomDim()const;

    //�ɰ�Designer��Ҫ�ֶ�ָ��,Ӱ��ÿһ�������������,
    //IA-conf tool�����ݳ����,�Զ����ݹ�ʽ����ÿһ������������  2019
    //�󲿷�������ͬ���ñ��ṹ

    //2019IA-conf tool�°����
    //�򻯵�Ӱ�����䳤���,
    //double roomWidth;          //ǰ��ǽ�����ֵ
    //double roomLength;        //����ǽ����,һ���ǵȳ���
    double roomHeight;        //ǰ�к�,���߶�,һ��ȡ�����������߶�λ��
    //double roomScreenWidth;   //���ܻṹ��ǰ��ǽ���ȳ���Ӱ��ģ��,�Ľ���
    //double roomBoothWidth;
    unsigned int grade;               //�Զ�Ӱ�����õ���,���и�,�ֱ�Ϊ0,1,2
    std::string gradeToStr()const;    //���ε��ַ���


    //�ɰ�Designer����
	//ScreenWall
	double ScreenWallLength;       //ǰǽ���� ��ӫĻ�ࣩ
	double FlatWidth;                   //�ڷ���Ļ���
	double FlatHeight;                 //�ڷ���Ļ�߶�
    //��Ļ������ǽλ��ƫ��,�ο�dolby
    double FlatScreenDisToLeftWall;          

	double ScopeWidth;                  //�����Ļ���
	double ScopeHeight;                 //�����Ļ�߶�
    double ScreenBottomHeight;    //�ذ嵽��Ļ�ײ��߶�
    //��Ļ������ǽλ��ƫ��,�ο�dolby
    double ScopeScreenDisToLeftWall;          

	unsigned int TotalSpeakerofScreenWall;     //ǰǽ����������  ��Ļ����������
	int TotalSpeakerofLFE;                                //LFE�����ڸ���  


	//LeftRightWall
	double LeftRightWallLength;                  //����ǽ����
	unsigned int SpeakersPerLeftRightWall;  //����ǽÿ������������
	double AverangeDistanceOfSpeaker;      //����ǽ.������֮���ƽ������
	double DistanceWalltoScreen;                //����ǽ,��һ����������ǰǽ����

    //����ϯ����,��һ�����ε���ǽ����,�����Ժ�ȷ������ϯ�¶ȵ�  new add 
    //��ʱ���� LeftRightWallLength-DistanceWalltoScreen
	double DistanceFirstRowSeatToBoothWall;                


	//BoothWall
	double BoothWallLength;                         //��ǽ����
	unsigned int TotalSpeakerofBoothWall;    //��ǽ��ͨ�������ܸ���������������������               
	unsigned int TotalSpeakersofSubwoofer;  //��ǽ�����ڸ���


	//ceiling  floor ����Ӱ���߶�
	double FloorElevationat23point;
	double FloorElevationatbooth;
	double CeilingElevationatscreen;                    //����Ļ���߶�
	double CeilingElevationat23point;                  //��2/3���߶�
	double CeilingElevationatbooth;                    //�ں�ǽ���߶�
	unsigned int SpeakersofCeillingPerSide;        //��ǽ���컨�壬���Ҳ࣬ÿ������������


    //�ó�����+ӰԺ��+Ӱ����
    std::string CinemaName()const
    {
        return strCity+" "+strCinema+" "+strAuditorium;
    }

    //RoomID�Ի���Ĳ�����Ϣ
    std::string strCompany;         //ӰԺ������˾
    std::string strCinema;            //ӰԺ����
	std::string strAuditorium;      //ӰԺ��Ӱ������
	std::string strCity;                 //���ڳ���
	std::string strProvince;          //����ʡ
	std::string strCountry;           //���ڹ���/����
	std::string strRemark;       //������ע��Ϣ

    //2019IA-conf tool����
    std::string strDetailedAddress;   //Ӱ����ϸ��ַ
    std::string strContactPerson;     //�ۺ���ϵ��
    std::string strContactTel;           //��ϵ�˵绰
    std::string strContactEmail;       //��ϵ������

    std::string strCreateDate;            //�������� ���ȷ�Ϻ��Զ�����,   ����
};

RoomConfig* GetRoomConfig();
//void ReleaseRoomConfig(RoomConfig* pConfigRoom);

//#pragma pack(pop)       //�ָ�ԭ������״̬


#endif





