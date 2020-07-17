#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "../ProtocolNetHead.h"

//
//
//enum EType_Const
//{   
//    KL_HEAD=0x7585,             //数据报文头
//    ZERO=0x0000,
//}; 
//
//
//#pragma pack(push,1)     //设置一个字节对齐  
//typedef struct _KL_
//{
//    unsigned short head;        //报文头恒为0x7585
//    unsigned short key;          //报文类型  
//    unsigned int len;              //报文长度不包括此KL头,为了传送大报文,用int32
//    _KL_()
//    {
//        Init();
//    }
//    void Init()
//    {
//       head=KL_HEAD;
//    }
//    void toNetStream()
//    {
//
//    }
//    void fromNetStream()
//    {
//
//    }
//}KL;
//
//
//
//#pragma pack(pop)       //恢复原来对齐状态
// 




//客户端类型枚举，服务端用来区分客户端类型   Protool插件，Monitor,MPU UI等
typedef struct _ClientType_
{
    int ClientType;
}ClientType;

enum EMClientType
{
    CLIENT_UNKNOW=0x10,    //初始化时的未知状态
    CLIENT_PANNER,               //protool的panner插件
    CLIENT_MONITOR,            //monitor对象监视
    CLIENT_MPUUI,                 //mpu ui制作端
    CLIENT_DCAPUI,                //dcap ui影院播放端
    CLIENT_DESIGNER             //影院扬声器配置文件设计
};



enum PROTOCOL 
{
    //协议汇总
    //定义新报文的时候，可以暂不指定key的值, 只需要确认枚举名，  在对接时候同步此enum PROTOCOL结构  即可 by lizulin
    //待项目稳定时候再填充协议key值,

   CMD_UNDEFINED=0X00,

   //所有连接上服务器的客户端每隔一段时间主动上传心跳包，表明自己任然活着
   CMD_HEARTBEAT_UPLOAD=0X0001,  
   CMD_HEARTBEAT_UPLOAD_RESP,        //服务器收到心跳包后响应

   //获取UI登陆时Core已经运行时间,考虑两台电脑时间可能不一样如此设计,
   //此后UI收到心跳包后自己根据时钟偏移刷新实际运行时间,不需要每次从Core获取  add 20190425
   CMD_GET_SYSTEM_STARTUP_TO_NOW_TIME,     
   CMD_GET_SYSTEM_STARTUP_TO_NOW_TIME_RESP,

   CMD_GET_CORE_VERSION,    //获取Core版本号
   CMD_CORE_VERSION_RESP,

   //DCAP-IA Core-UI交互新增
   CMD_SET_MIC_VOLUME,    //设置两个麦克风的音量
   CMD_GET_MIC_VOLUME,
   CMD_MIC_VOLUME_RESP,

   CMD_GET_VOLUME_FADER_NUM,   //当前选择的音量控制器是总音量,还是mic1,mic2
   CMD_VOLUME_FADER_NUM_RESP,

   CMD_SET_EQ_ENABLED_STATE,
   CMD_GET_EQ_ENABLED_STATE,
   CMD_EQ_ENABLED_STATE_RESP,

   CMD_GET_ALL_EQ_COEFF,          //获取所有扬声器的均衡系数
   CMD_ALL_EQ_COEFF_RESP,

   CMD_SET_SPK_EQ_COEFF,         //修改设置某个扬声器的均衡系数
   CMD_GET_SPK_EQ_COEFF,         //修改设置某个扬声器的均衡系数
   CMD_SPK_EQ_COEFF_RESP,

   CMD_BACKUP_EQCONFIG_FILE,               //备份IA版的EQ配置文件到本机
   CMD_BACKUP_EQCONFIG_FILE_RESP,  

   CMD_RESTORE_EQCONFIG_FILE,              //从本机选择一个文件去恢复IA版的EQ配置文件
   CMD_RESTORE_EQCONFIG_FILE_RESP,  


    //MPU与Panner的通信
    CMD_PLAY_STATUS_CHANGED=0X2000,
    CMD_PLAY_STATUS_CHANGED_RESP,
    CMD_SET_TIME_CODE,                    //单独设置Protool播放时间码
    CMD_SET_TIME_CODE_RESP,
    CMD_SET_METADATA,                     //向MPU发送Metadata
    CMD_SET_METADATA_RESP,            //MPU收到Metadata后返回

    CMD_SET_SAMPLERATE_AND_FRAMERATE,          //手动设置MPU-Core的帧率与采样率  panner与MPU-UI复用此命令
    CMD_GET_SAMPLERATE_AND_FRAMERATE, 
    CMD_SAMPLERATE_AND_FRAMERATE_RESP,

    //未使用，可废弃
    //CMD_GET_AVAILABILITY_ID,             //向MPU请求ID池中可用的ID列表
    //CMD_AVAILABILITY_ID_LIST,            //返回MPU ID池中可用的ID列表
    //CMD_SELECT_ID,                              //通知MPU使用ID池中的ID
    //CMD_SELECT_ID_RESP,                     //MPU确认使用ID
    //CMD_RELEASE_ID,                            //通知MPU释放已占用的ID
    //CMD_RELEASE_ID_RESP,                   //MPU确认释放ID


    //MPU与Monitor的通信
    CMD_GET_METADATA_LIST=0x3000,       //Monitor向MPU发送Metadata请求
    CMD_GET_METADATA_LIST_RESP,           //MPU向Monitor返回MetadataList
    CMD_GET_SPEAKER_LIST,                        //Monitor向MPU发送SpeakerList请求
    CMD_GET_SPEAKER_LIST_RESP,               //MPU向Monitor返回SpeakerList
    CMD_GET_ROOMDIM,                             //Monitor向MPU发送Room尺寸请求
    CMD_GET_ROOMDIM_RESP,                    //MPU向Monitor返回Room尺寸

    //实时音量电平获取，Monitor与Dcap-ui与Core的交互，公用以下命令
    CMD_GET_SPEAKER_GAIN,                       //实时取扬声器音量
    CMD_GET_SPEAKER_GAIN_RESP,              //返回SPEAKER_GAIN结构32个，64，128第一个int存放扬声器个数n，后面跟着n个float



    //MPU与Designer之间的通讯
    //自动均衡相关,Core功能已经被废弃M改手动均衡
    CMD_PLAY_CALIBRATE_PINKNOISE=0x4000,      //通知MPU播放指定的PinkNoise用于校准MIC，只在中置通道中播政
    CMD_PLAY_CALIBRATE_PINKNOISE_RESP,          //0x0024	1	0房	MPU确认播放＿失败＿成功
    CMD_STOP_CALIBRATE_PINKNOISE,	                  //0x0025	0	0	通知MPU停止播放用于校准MIC的PinkNoise
    CMD_STOP_CALIBRATE_PINKNOISE_RESP,	          //0x0026	0	0	MPU确认停止播放用于校准MIC的PinkNoise
    CMD_SET_CALIBRATE_PINKNOISEGAIN,	          //Gain(float)	通知MPU设置用于校准MIC的PinkNoise的Gain
    CMD_SET_CALIBRATE_PINKNOISEGAIN_RESP,     //10房	MPU确认停止播放用于校准MIC的PinkNoise
    
    //与均衡调节页面功能重复，待整合
    //CMD_PLAY_PINKNOISE,                                      //向MPU请求在指定Speaker上播放粉噪，注意，每次只能在一个音箱上放粉噿
    //CMD_PLAY_PINKNOISE_RESP,                             //MPU确认在指定Speaker上播放粉噿
    //CMD_STOP_PINKNOISE,                                     //向MPU请求停止在指定Speaker上播放粉噿
    //CMD_STOP_PINKNOISE_RESP,                            //MPU确认在指定Speaker上停止播放粉噿

    CMD_START_AUTOEQ,                   
    CMD_START_AUTOEQ_RESP,   
    CMD_UPDATE_AUTOEQRESULT,
    CMD_UPDATE_AUTOEQRESULT_RESP,
    CMD_STOP_AUTOEQ,
    CMD_STOP_AUTOEQ_RESP,
    CMD_GET_AUTOEQPROGRESS,
    CMD_GET_AUTOEQPROGRESS_RESP,
    CMD_GET_EQSTATE,                           //取自动均衡结果
    CMD_GET_EQSTATE_RESP,
    CMD_GET_ALLEQGAIN,                      //Designer 用不到
    CMD_GET_ALLEQGAIN_RESP,             //Designer 用不到
    CMD_ENABLE_MICSINPUT,                //设置麦克风使能
    CMD_ENABLE_MICSINPUT_RESP,
    CMD_GET_MICSMETER,                     //取麦克风音量
    CMD_GET_MICSMETER_RESP,           
    CMD_ENABLE_EQ,                             //控制EnableEQ的使能       
    CMD_ENABLE_EQ_RESP,   
    CMD_MANUAL_EQ_SETGAIN,
    CMD_MANUAL_EQ_SETGAIN_RESP,
    CMD_MANUAL_EQ_SETPATTEN,
    CMD_MANUAL_EQ_SETPATTEN_RESP,

    CMD_SET_ACTIVE_MACRO,
    CMD_SET_ACTIVE_MACRO_RESP,

    //DAC配置文件 dolby atmos config，就是xml配置文件  改为LHC  即LEONIS holosound config 

    //LHC文件操作, AutoEQ功能，MPU与Designer,MPU UI以下此命令
    CMD_SET_LHC=0X5100,      //获取/发送LHC配置文件，就是xml配置文件
    CMD_SET_LHC_RESP,
    CMD_GET_LHC,                    
    CMD_GET_LHC_RESP,
    CMD_RESET_LHC,
    CMD_RESET_LHC_RESP,

    //其他窗口简单命令
    CMD_SET_SETTING=0X5200,
    CMD_SET_SETTING_RESP,
    CMD_GET_SETTING,
    CMD_GET_SETTING_RESP,
    //CMD_REBOOT_MPU,
    //CMD_REBOOT_MPU_RESP,
    CMD_GETINFO,
    CMD_GETINFO_RESP,
    CMD_GET_LOG,
    CMD_GET_LOG_RESP,
    //CMD_GET_LOG_DOWNLOAD_URL,
    //CMD_GET_LOG_DOWNLOAD_URL_RESP,
    //CMD_SET_RECORD_LTC,    //设置录制模式与protools时间码同步 废弃
    //CMD_SET_RECORD_LTC_RESP,


    //MPU与Core交互工程与编码相关命令
    CMD_EXPORT_PROJECT=0x5300,
    CMD_EXPORT_PROJECT_RESP,

    CMD_GET_DRIVER_LIST,
    CMD_GET_DRIVER_LIST_RESP,
    CMD_GET_PROJECT_LIST,
    CMD_GET_PROJECT_LIST_RESP,
    CMD_NEW_PROJECT,
    CMD_NEW_PROJECT_RESP,
    CMD_OPEN_PROJECT,
    CMD_OPEN_PROJECT_RESP,
    CMD_DEL_PROJECT,
    CMD_DEL_PROJECT_RESP,
    CMD_SAVE_PROJECT,
    CMD_SAVE_PROJECT_RESP,
    CMD_CLOSE_PROJECT,
    CMD_CLOSE_PROJECT_RESP,
    CMD_COPY_PROJECT,
    CMD_COPY_PROJECT_RESP,
    CMD_COPY_PROJECT_PROGRESS,
    CMD_COPY_PROJECT_PROGRESS_RESP,
    CMD_START_ENCODER,
    CMD_START_ENCODER_RESP,
    CMD_ENCODER_PROGRESS,
    CMD_ENCODER_PROGRESS_RESP,

    CMD_SAVE_PROJECT_AS,
    CMD_SAVE_PROJECT_AS_RESP,

    CMD_GET_PROJECTXML,
    CMD_GET_PROJECTXML_RESP,

    CMD_SET_PM_OR_MXF,     //set之后返回当前，或加是否成功频段
    CMD_GET_PM_OR_MXF,     //UI重新打开时获取当前活动PM或MXF文件路径
    CMD_PM_OR_MXF_RESP,   
    CMD_UNSET_PM_OR_MXF,
    CMD_UNSET_PM_OR_MXF_RESP,


    CMD_GET_MXF_LIST,
    CMD_GET_MXF_LIST_RESP,

    CMD_GET_CURRENT_PROJECT,
    CMD_GET_CURRENT_PROJECT_RESP,

    CMD_SET_DOWNMIX_ENABLE,
    CMD_SET_DOWNMIX_ENABLE_RESP,
    CMD_GET_DOWNMIX_STATUS,
    CMD_GET_DOWNMIX_STATUS_RESP,

    //new 20170809  
    CMD_GET_PM_AUDIO_DATA,              //MPU-UI向Core请求pm母版的一段音频数据
    CMD_GET_PM_AUDIO_DATA_RESP,     //Core返回音频数据

    CMD_GET_MEDIA_FILE_INFO,                 //设置新的PM或MXF媒体文件路径时，Core返回媒体信息，总帧数等，用在播放控制
    CMD_GET_MEDIA_FILE_INFO_RESP,   

    CMD_SET_PLAYBACK_POSITION,                         //手动设置媒体文件回放位置
    CMD_SET_PLAYBACK_POSITION_RESP,







    //DCAP界面和Core交互命令
    CMD_GET_DCAP_SERIALNO=0X6000,    //获取DCAP机器唯一序列号
    CMD_DCAP_SERIALNO_RESP,

    CMD_GET_DCAP_FW_FUN,            //获取DCAP固件支持的功能
    CMD_GET_DCAP_FW_FUN_RESP,       //0 - DCAP-IA, 1 - DCAP-IAE, 2 - DCAP-E, 3 - DCAP-EAH

    //add by zhangjing
    //CMD_GET_MACRO_CONFIG=0X6100,
    //CMD_GET_MACRO_CONFIG_RESP,
    //CMD_SET_MACRO_CONFIG,
    //CMD_SET_MACRO_CONFIG_RESP,


    //CMD_GET_EQ_CONFIG=0X6200,
    //CMD_GET_EQ_CONFIG_RESP,
    //CMD_SET_EQ_CONFIG,
    //CMD_SET_EQ_CONFIG_RESP,
    //CMD_DEL_EQ_CONFIG,
    //CMD_DEL_EQ_CONFIG_RESP,
    //CMD_GET_EQ_NAME,
    //CMD_GET_EQ_NAME_RESP,
    //CMD_FLATTEN,           //已经废弃 待删除
    //CMD_FLATTEN_RESP,  //已经废弃 待删除
    //CMD_SET_OUT_LEVEL,
    //CMD_SET_OUT_LEVEL_RESP,
    //CMD_SET_EQ_OF_CHANNEL,
    //CMD_SET_EQ_OF_CHANNEL_RESP,

    //CMD_GET_MIX_CONFIG = 0X6300,
    //CMD_GET_MIX_CONFIG_RESP,
    //CMD_SET_MIX_CONFIG,
    //CMD_SET_MIX_CONFIG_RESP,


    //DCAP-UI的渐入渐出控制
    CMD_SET_FADE_IN_OUT=0X6800,
    CMD_GET_FADE_IN_OUT,
    CMD_FADE_IN_OUT_RESP,
    //CMD_GET_FADE,
    //CMD_GET_FADE_RESP,

    //setting.ini文件配置信息通过UI操作修改相关 add 20190424 by lizulin
    CMD_GET_SOUNDCARD_CONFIG,              //获取当前配置
    CMD_SET_SOUNDCARD_CONFIG,              //修改配置
    CMD_SOUNDCARD_CONFIG_RESP,            //获取/修改后返回当前配置
    CMD_GET_SPEAKER_CROSSOVER_COEF,    //主扬声器分频系数设置
    CMD_SET_SPEAKER_CROSSOVER_COEF,
    CMD_SPEAKER_CROSSOVER_COEF_RESP,
    CMD_GET_CONTENT_SERVER_CONFIG,      //ST43014内容同步服务器配置相关
    CMD_SET_CONTENT_SERVER_CONFIG,
    CMD_CONTENT_SERVER_CONFIG_RESP,
    CMD_GET_SMA_STATE,                            //控制Core是否发起SMA授权UDP广播
    CMD_SET_SMA_STATE,
    CMD_SMA_STATE_RESP,
	CMD_GET_BINAURAL_ENABLED_STATE,	//控制是否进行Binaural双耳声学渲染
	CMD_SET_BINAURAL_ENABLED_STATE,
	CMD_BINAURAL_ENABLED_STATE_RESP,
    CMD_GET_BINAURAL_VOLUME,	//控制Binaural双耳声学输出音量
    CMD_SET_BINAURAL_VOLUME,
    CMD_BINAURAL_VOLUME_RESP,

    CMD_GET_BINAURAL_OUTPUT_MODE,	//控制BINAURAL双耳声学输出音量
    CMD_SET_BINAURAL_OUTPUT_MODE,
    CMD_BINAURAL_OUTPUT_MODE_RESP,

    CMD_SYS_LOGIN =0x7000,                 //Dcap UI登陆Dcap Core
    CMD_SYS_LOGIN_RESP,
    CMD_SYS_MODIFY_PASSWORD,         //修改密码
    CMD_SYS_MODIFY_PASSWORD_RESP,


    //简化设计，废弃
#if 0
    CMD_SYS_LOGOUT,
    CMD_SYS_LOGOUT_RESP,
    CMD_SYS_GET_USERINF,
    CMD_SYS_GET_USERINF_RESP,
    CMD_SYS_MODIFY_USERINF,
    CMD_SYS_MODIFY_USERINF_RESP,
    CMD_SYS_DELETE_USER,
    CMD_SYS_DELETE_USER_RESP,
    CMD_SYS_CREATE_USER,
    CMD_SYS_CREATE_USER_RESP,
    CMD_SYS_GET_LOG,
    CMD_SYS_GET_LOG_RESP,
#endif


    CMD_SYS_GET_NETINF=0X7100,
    CMD_SYS_GET_NETINF_RESP,
    CMD_SYS_SET_NETINF,
    CMD_SYS_SET_NETINF_RESP,
#if 0
    CMD_SYS_BACKUP,
    CMD_SYS_BACKUP_RESP,
    CMD_SYS_RESTORE,
    CMD_SYS_RESTORE_RESP,
    CMD_SYS_RESET,
    CMD_SYS_RESET_RESP,
    CMD_SYS_UPDATE,
    CMD_SYS_UPDATE_RESP,
#endif

    CMD_SYS_REBOOT=0X7200,  //Core重启
    CMD_SYS_REBOOT_RESP,
    CMD_SYS_SHUTDOWN,   //Core关机，暂不实现
    CMD_SYS_SHUTDOWN_RESP,


    CMD_SYS_FILE_TRANS=0X7300,
    CMD_SYS_FILE_TRANS_RESP,

    //ADD END.   YCS


    //Content Manager
    CMD_UPDATE_PROGRAM_LIST_HDD = 0x8000,
    CMD_UPDATE_PROGRAM_LIST_HDD_RESP,
    CMD_UPDATE_PROGRAM_LIST_USB,
    CMD_UPDATE_PROGRAM_LIST_USB_RESP,
    CMD_IS_PROGRAM_LIST_READY_HDD,
    CMD_IS_PROGRAM_LIST_READY_HDD_RESP,
    CMD_IS_PROGRAM_LIST_READY_USB,
    CMD_IS_PROGRAM_LIST_READY_USB_RESP,
    CMD_GET_PROGRAM_LIST_HDD,
    CMD_GET_PROGRAM_LIST_HDD_RESP,
    CMD_GET_PROGRAM_LIST_USB,
    CMD_GET_PROGRAM_LIST_USB_RESP,

    //获取当前正在播放的DCP工程名,其实就是电影名,等信息,UI首次登陆需要主动获取,此后播放影片变化Core会主动通知  add 20190717
    CMD_GET_CURRENT_PLAYING_PROGRAM_INFO, 
    CMD_CURRENT_PLAYING_PROGRAM_INFO_RESP,


    //add 20171216   get KDM file list
    CMD_GET_KDM_LIST_HDD,
    CMD_GET_KDM_LIST_HDD_RESP,
    CMD_GET_KDM_LIST_USB,
    CMD_GET_KDM_LIST_USB_RESP,
    CMD_COPY_KDM,                          //copy kdm file form Usb to HDD  add by 20171219
    CMD_COPY_KDM_RESP,
    CMD_DELETE_KDM,                       //delete kdm file form HDD  add by 20171219
    CMD_DELETE_KDM_RESP,
    CMD_COPY_PROGRAM,
    CMD_COPY_PROGRAM_RESP,
    CMD_GET_COPY_PROGRESS,
    CMD_GET_COPY_PROGRESS_RESP,
    CMD_DELETE_PROGRAM,
    CMD_DELETE_PROGRAM_RESP,

    //20190125新增与手动回放模式相关
    CMD_SET_PLAYBACK_LIST,                       //设置播放列表
    CMD_GET_PLAYBACK_LIST,                       //获取当前播放列表,（首次打开UI）
    CMD_PLAYBACK_LIST_RESP,                     //当前播放列表返回,//正在播放的影片序号
    CMD_SET_PLAYBACK_LOOP_STATUS,        //设置播放循环状态
    CMD_GET_PLAYBACK_LOOP_STATUS,        //获取当前播放循环状态
    CMD_PLAYBACK_LOOP_STATUS_RESP,      //当前播放循环状态返回
    CMD_SET_PLAYBACK_STATUS,                  //设置播放状态，播放，停止，暂停
    CMD_GET_PLAYBACK_STATUS,                  //获取播放状态
    CMD_PLAYBACK_STATUS_RESP,                //播放状态返回
    CMD_GET_CURRENT_MEDIA_FILE_INFO,    //当前正在播放的影片时间信息总帧数,帧率等 在列表中的序号等
    CMD_CURRENT_MEDIA_FILE_INFO_RESP,    
    CMD_SET_JUMP_PLAYBACK_LIST_INDEX,                //手动跳转到播放列表的某部电影
    CMD_SET_JUMP_PLAYBACK_LIST_INDEX_RESP,
    CMD_SET_CURRENT_PLAY_TIME,          //手动快进,
    CMD_GET_CURRENT_PLAY_TIME,          //Core端主动返回播放进度
    CMD_CURRENT_PLAY_TIME_RESP,        //当前播放进度,Core端主动发送


    CMD_UI_BEGIN=0X9000,
    CMD_UI_END,


    //均衡调节页面UI与Core交互命令,MPU-UI与DCAP-UI共用完全相同共用以下命令。
    CMD_BEGIN_EQ=0XA000,
    CMD_SET_PLAY_TEST_WAVE_STATE,              //设置播放/停止粉噪等测试波形的播放
    CMD_GET_PLAY_TEST_WAVE_STATE,             //获取播放状态
    CMD_PLAY_TEST_WAVE_STATE_RESP,            //状态返回
    CMD_SET_PLAY_TEST_WAVE_CHANNEL,        //设置播放的声道
    CMD_GET_PLAY_TEST_WAVE_CHANNEL,        //获取播放的声道
    CMD_PLAY_TEST_WAVE_CHANNEL_RESP,      //声道返回

    CMD_SET_TEST_WAVE_TYPE,        //设置/获取播放的类型粉噪/正弦波/测试音乐等
    CMD_GET_TEST_WAVE_TYPE,        //
    CMD_TEST_WAVE_TYPE_RESP,      //
    CMD_SET_TEST_WAVE_LOOP_STATE,    //设置/获取播放时是否循环
    CMD_GET_TEST_WAVE_LOOP_STATE,
    CMD_TEST_WAVE_LOOP_STATE_RESP,

    CMD_GET_EQGROUP_NAME_LIST,
    CMD_GET_EQGROUP_NAME_LIST_RESP,
    CMD_GET_CURRENT_EQGROUP_NAME,
    CMD_SET_CURRENT_EQGROUP_NAME,
    CMD_CURRENT_EQGROUP_NAME_RESP,
    CMD_GET_EQGROUP_COEFF_BY_NAME,
    CMD_GET_EQGROUP_COEFF_BY_NAME_RESP,
    CMD_SAVE_EQCONFIG_TO_FILE,
    CMD_SAVE_EQCONFIG_TO_FILE_RESP,
    CMD_ADD_EQGROUP_NAME,
    CMD_ADD_EQGROUP_NAME_RESP,
    CMD_DELETE_EQGROUP_NAME,
    CMD_DELETE_EQGROUP_NAME_RESP,
    CMD_SET_SPEAKER_EQCOEFF,
    CMD_GET_SPEAKER_EQCOEFF,
    CMD_SET_SPEAKER_EQCOEFF_RESP,
 
    

    //Room页面设置扬声器延时相关,实时刷新到Core
    CMD_SET_SPEAKER_DELAY,
    CMD_SET_SPEAKER_DELAY_RESP,
    CMD_SAVE_CONFIG_TO_FILE,   //设置完成后命令Core端的lhc conf保存
    CMD_SAVE_CONFIG_TO_FILE_RESP,


    //一些MPU/DCAP(Core-UI) Monitor Panner Designer 等共用的通讯协议命令
    CMD_SET_CLIENTTYPE=0XF100,//用在Core的Server区分Client类型
    CMD_SET_CLIENTTYPE_RESP,


    //MPUUI与Monitor的Transport控制和mute，dim功能共用以下协议
    CMD_SET_PLAYSTOPSTATE=0XF200,    //播放按钮
    CMD_GET_PLAYSTOPSTATE,
    CMD_PLAYSTOPSTATE_RESP,
    CMD_SET_RECORDSTATE,       //录制按钮
    CMD_GET_RECORDSTATE,
    CMD_RECORDSTATE_RESP,
    CMD_SET_LTCSTATE,              //LTC按钮
    CMD_GET_LTCSTATE,
    CMD_LTCSTATE_RESP,
    CMD_GET_TIME_CODE,                           //UI取时间码，以帧号为单位
    CMD_GET_TIME_CODE_RESP,

    //内部调试窗口的一些手动控制开关
    CMD_INTERNAL_TEST_BEGIN=0XF300,
    CMD_SET_RENDER_LEVEL_STATE,   //控制高级渲染,还是性能优先的普通渲染。
    CMD_GET_RENDER_LEVEL_STATE,   
    CMD_RENDER_LEVEL_STATE_RESP,  
    CMD_SET_RENDER_OBJ_STATE,       //控制DCAP播放时是否渲染对象否则作为普通5.1播放 设置
    CMD_GET_RENDER_OBJ_STATE,      //获取状态
    CMD_RENDER_OBJ_STATE_RESP,    //Core返回当前状态
    CMD_SET_REALEQ_STATE, 
    CMD_GET_REALEQ_STATE,   
    CMD_REALEQ_STATE_RESP, 
    CMD_SET_BANDEQ_STATE,
    CMD_GET_BANDEQ_STATE,   
    CMD_BANDEQ_STATE_RESP, 
    CMD_SET_BASSMRG_STATE,
    CMD_GET_BASSMRG_STATE,   
    CMD_BASSMRG_STATE_RESP, 
    CMD_SET_CHANNEL_MUTE_STATE,
    CMD_GET_CHANNEL_MUTE_STATE,   
    CMD_CHANNEL_MUTE_STATE_RESP, 
    CMD_SET_TEST_VALUE,  //手动控制系数，辅助core 调试使用
    CMD_GET_TEST_VALUE,
    CMD_TEST_VALUE_RESP,
    CMD_SET_TEST_SWITCH,
    CMD_GET_TEST_SWITCH,
    CMD_TEST_SWITCH_STATE_RESP,

    CMD_SET_KILL_SWITCH,
    CMD_GET_KILL_SWITCH,
    CMD_KILL_SWITCH_STATE_RESP,

    //以下命令已经作为外部控制公共接口，命令值不可改变 20180420  F5之后的命令作为公共命令保留字 可以容纳2500个
    CMD_SET_CHANNEL_MODE=0XF500,   //未实现  设置5.1/7.1 还是HoloSound模式
    CMD_GET_CHANNEL_MODE=0XF501,  
    CMD_CHANNEL_MODE_RESP=0XF502,
    CMD_GET_FADERSTATE=0XF503,          //fader 音量管理器控制总音量
    CMD_SET_FADERSTATE=0XF504,
    CMD_FADERSTATE_RESP=0XF505,
    CMD_SET_MUTESTATE=0XF506,
    CMD_GET_MUTESTATE=0XF507,
    CMD_MUTESTATE_RESP=0XF508,
    CMD_SET_DIMSTATE=0XF509,
    CMD_GET_DIMSTATE=0XF50A,
    CMD_DIMSTATE_RESP=0XF50B,
	CMD_ADJUSTVOLSTATE_RESP=0xF50C,
    //随机数测试报文,仅用网络通信长时间稳定性调试
    CMD_RAND_DATA_TEST,
    CMD_RAND_DATA_TEST_RESP
};

#endif







