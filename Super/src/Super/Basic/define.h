#ifndef DEFINE_H
#define DEFINE_H

//Super库全局常量枚举等的定义

//namespace Super
//namespace
//{
    //RingBuffer消息队列等待时间
    enum EWaitTime {
        Wait_Zero= 0,       
        Wait_5ms=5,    
        Wait_10ms=10,  
        Wait_20ms=20, 
        Wait_Dafault=Wait_5ms,    
        Wait_INFINITE=0xFFFFFFFF,
    };

    enum EMqPush {          
        Push_ALL=0,          //推送给某个队列自身以及所有子队列;
        Push_Self,              //仅推送给某个队列自身,不推送给子队列
        Push_AllChild,        //推送给某个队列的所有子队列
        Push_OneChild,      //推送给某特定ID的子队列,每个队列都有唯一ID。根据根据业务需求外部设定
        Push_OneType,      //推送给某特定类型Type子队列，子队列可以类型可以相同，可以根据业务需求外部设定
        Push_ExceptOne,   //推送给所有子队列，除了某个特定Msgqueue指针
    };
//}




namespace Sp 
{

    enum AspectRatioMode {
        IgnoreAspectRatio,
        KeepAspectRatio,
        KeepAspectRatioByExpanding
    };


    enum GlobalColor {
        color0,
        color1,
        black,
        white,
        darkGray,
        gray,
        lightGray,
        red,
        green,
        blue,
        cyan,
        magenta,
        yellow,
        darkRed,
        darkGreen,
        darkBlue,
        darkCyan,
        darkMagenta,
        darkYellow,
        transparent
    };

}


#endif
