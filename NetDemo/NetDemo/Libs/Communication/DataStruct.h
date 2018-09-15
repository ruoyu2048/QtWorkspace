#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <iostream>
#include <string>
#include <string.h>
#include <QByteArray>

using namespace std;

#define COMMAXLEN 1024

enum CommType{
    TcpServer,
    TcpClient,
    UDP,
    Serial
};

enum ConnType{
    Ready,
    Connecting,
    Timeout,
    Connected,
    Disconnected
};

struct CTTest{
    char cCmd;
    char cWorkMode;
    ushort nShort;
    float fData;
    char cRes[5];
    CTTest(){
        memset(&cCmd,'\0',sizeof(CTTest));
        cCmd = 0x20;
    }
};

//<1>航迹点

//(1)目标坐标
struct Tar_Coordinate{
    float fDistance;        //目标径向距离
    float fSpeed;           //目标径向速度
    float fDirection;       //目标方位
    float fTestDirection;   //目标测站方位
    float fLongitude;       //经度
    float fLatitude;        //纬度
    float fHeigh;           //高度
    float fXCoord;          //目标X坐标
    float fYCoord;          //目标Y坐标
    float fXMercator;       //目标墨卡托坐标X
    float fYMercator;       //目标墨卡托坐标Y
    Tar_Coordinate(){
        memset(&fDirection,'0',sizeof(Tar_Coordinate));
    }
};

//(2)目标数组
struct Tar_Group{
    double          dTarTime;       //目标时标
    uchar           cWorkMode;      //搜跟模式
    uchar           cBeamPos;       //波位号
    uchar           cBeamNo;        //波束号
    uchar           cBeamCnt;       //测角波束数
    float           fCenterRCS;     //质心RCS值
    float           fMaxRCS;        //最大RCS值
    ushort          uSNR_1;         //重频信噪比1
    uchar           cSize_1;        //重频径向尺寸1
    uchar           cDoppler_1;     //重频多普勒带宽1
    ushort          uSNR_2;         //重频信噪比2
    uchar           cSize_2;        //重频径向尺寸2
    uchar           cDoppler_2;     //重频多普勒带宽2
    ushort          uSNR_3;         //重频信噪比3
    uchar           cSize_3;        //重频径向尺寸3
    uchar           cDoppler_3;     //重频多普勒带宽3
    ushort          uTarRelated;    //目标关联结果
    ushort          uFateDetect;    //虚警预判结果
    ushort          uTarDisinguish; //目标识别结果
    Tar_Coordinate  tarCoord;       //目标坐标
    Tar_Group(){
        memset(&dTarTime,'0',sizeof(Tar_Group));
    }
};


struct TrackPoint{
    uchar       cCmdByte;       //命令字
    uchar       cRaderAddr;     //雷达站址编号
    ushort      uTarBatchNum;   //目标批号
    ushort      uTarPointCnt;   //目标点个数
    float       fTarLineSpeed;  //目标线速度
    float       fTarHXJ;        //目标航向角
    ushort      uThreaten;      //威胁度
    Tar_Group   tarGroup;       //目标点数组
    uchar       cFirstOut;      //首次输出、更新、消批标志
    uchar       cTarAttri;      //目标属性
    TrackPoint(){
        memset(&cCmdByte,'0',sizeof(TrackPoint));
    }
};

struct FrameHead{
    char cHead;             //帧头
    char cDesAdd;           //目的地址
    char cSrcAdd;           //原地址
    char cType;             //消息类型
    //int  iLenth;            //信息区长度

    FrameHead(){
        //memset(this,0,sizeof(FrameHead));
        memset(&cHead,0,sizeof(FrameHead));
        cHead = 0xAA;
    }
};

struct FrameTail{
    char cCheck;        //校验位
    char cTail;         //包尾
    FrameTail(){
        memset(&cCheck,0,sizeof(FrameTail));
        cTail = 0xA5;
    }
};

//中频源(Mid)
struct MidInfo{
    char cLDState;          //雷达状态
    char cWorkMode;         //工作模式
    char cPulseInput;       //脉冲输入
    char cOLMid;            //中频源输出幅度(Output Level of Mid)
    char cDSMid;            //设备状态(Device of Mid)
    char cReserve[4];       //备用
    MidInfo(){
        memset(&cLDState,0,sizeof(MidInfo));
    }
};

//4.1监控软件与中频源的接口
//设备控制指令
struct CSCI_01_Cmd{
    char cCmd;
    char cWorkMode;
    char cDataRate;
    CSCI_01_Cmd(){
        memset(&cCmd,0,sizeof(CSCI_01_Cmd));
    }
};

//通用设备控制回执
struct CmdFeedback{
    char cCmd;
    char cResault;
    char cFailInfo;
    char cReserve[4];
    CmdFeedback(){
        memset(&cCmd,0,sizeof(CmdFeedback));
    }
};

//4.2监控软件与源及变频组件的接口
//设备控制指令(0xC0)
struct CSCI_02_Cmd{
    char cCmd;  //控制命令
    char cFreqPoint;//频点
    CSCI_02_Cmd(){
        memset(&cCmd,0,sizeof(CSCI_02_Cmd));
    }
};

//设备控制回执(0xC0)
//struct CmdFeedback;

//4.3　监控软件与校正源的接口
//设备控制指令(0xC0)
struct CSCI_03_Cmd{
    char cCmd;  //控制命令
    char cOnOff;    //开关状态
    char cFreqPoint;//频点
    CSCI_03_Cmd(){
        memset(&cCmd,0,sizeof(CSCI_03_Cmd));
    }
};

//通用设备控制回执
//struct CmdFeedback;

//4.4监控软件与发射组件的接口
//(1)功率和波位切换
struct CSCI_04_Cmd_FWChange{
    char cCmd;  //控制命令
    char cSendFreq[8];//发射功率
    char cWaveVal[8];//波位对应的移相值
    CSCI_04_Cmd_FWChange(){
        memset(&cCmd,0,sizeof(CSCI_04_Cmd_FWChange));
    }
};

//(2)发射组件开关指令
struct CSCI_04_Cmd_OnOff{
    char cCmd;  //0x04-发射组件开关
    char cOnOff;    //0—关闭，1—打开
    char cWaveVal[8];//波位号对应的移相值（0-63）
    CSCI_04_Cmd_OnOff(){
        memset(&cCmd,0,sizeof(CSCI_04_Cmd_OnOff));
    }
};

//(3)发射组件地址更改
struct CSCI_04_Cmd_AddrChange{
    char cCmd;  //0x05-发射组件开关
    char cSendAddr; //发射组件地址
    CSCI_04_Cmd_AddrChange(){
        memset(&cCmd,0,sizeof(CSCI_04_Cmd_AddrChange));
    }
};

//通用设备控制回执
//struct CmdFeedback;

//4.5　监控软件与信号处理板DSP1的接口
//4.5.1.1设备装订指令(与DSP2通用)
struct CSCI_05_Cmd_Book{
    char cCmd;  //0x03-设备装订指令
    char cAddrCode;//站址编号
    char cRealNorth[2];//真北角
    char cAddrLong[4];//站址经度
    char cAddrLati[4];//站址纬度
    char cAddrHigh[2];//站址高度
    char cGPS[2];//GPS安装角与天线法线夹角
    char cAddrInfo;//站址信息选择
    char cSave1;//保留
    char cDistance[2];//距离零值
    char cSpeed[2];//速度零值
    char cDirection[2];//方位零值
    char cPitch[2];//俯仰零值
    char cSave2[2];//保留
    char cChannel[4*32];//通道一致性修正
    char cOrtho[4*32];//正交一致性修正
    char cWFactor[4*32*6*32];//波束形成系数
    char cReserve[32];
    CSCI_05_Cmd_Book(){
        memset(&cCmd,0,sizeof(CSCI_05_Cmd_Book));
    }
};

//4.5.1.2　算法参数下发（DSP1、DSP2、处理终端三者相同，由接收方处理关心区域）
struct CSCI_05_Cmd_ParaSend{
    char cCmd;//0x04-算法参数下发
    char cSearchMode;//搜跟模式
    char cDataRate;//数据率
    char cSendPower;//发射功率
    char cWorkFreqP;//工作频点
    char cNode1;//节点1数据录取开关。1：打开；2：关闭
    char cNode2;//节点2数据录取开关。1：打开；2：关闭
    char cNode3;//节点3数据录取开关。1：打开；2：关闭
    char cNode4;//节点4数据录取开关。1：打开；2：关闭
    char cWorkMode;//工作模式：1：正常；2：自检；3：方向图测试；4：通道一致性校正；5：正交一致性校正 6：模拟数据
    char cSimNode;//数据模拟节点：1：节点1；2：节点2；3：节点3；4：节点4；
    char cSave1[5];
    char cGiveUpP[2];//距离向舍弃点数，脉冲压缩
    char cFuncSel;//距离窗函数选择，脉冲压缩
    char cSave2[5];
    char cDoupler;//多普勒窗函数选择，动目标识别
    char cMTI;//MTI滤波器选择，动目标识别
    char cLeft[2];//剩余杂波图波门数，动目标识别，0~MTD/8
    char cSave3[4];//保留，动目标识别
    char cLeftEnable;//剩余杂波图检测使能，1：打开；2：关闭
    char cLeftLimit;//剩余杂波图检测门限，单位：dB
    char cIterNum;//杂波图积累迭代系数，单位：0.01
    char cSave4[5];
    char cSpeedCFAR;//检测最小波门速度CFAR，单位：0.1m/s
    char cSelCFAR;//检测背景选择CFAR，1：两边平均；2：两边取小；3：两边取大
    char cEnableCFAR;//逆影过滤使能CFAR，1：打开；2：关闭
    char cCalCFAR;//列和计算方式CFAR
    char cSlideCFAR;//滑窗长度CFAR
    char cProCFAR;//保护窗长度CFAR
    char cTableCFAR;//门限表单变更方式CFAR，0：不修改；1：基于初始值浮动；2：绝对值；
    char cChangeCFAR;//门限变更值CFAR，单位：dB
    char cMaskCFAR[4];//门限表单变更掩码CFAR，最多支持32张门限表单
    char cStartCFAR[2];//门限变更起始距离CFAR，单位：m
    char cEndCFAR[2];//门限变更结束距离CFAR，单位：m
    char cSave5[4];
    char cSingle[2];//单类最大点数，关联解算
    char cDisLimit;//距离聚类波门门限，关联解算
    char cCPI;//关联解算，与CPI相关，乘以比例系数
    char cCore;//重心提取方式，关联解算，高4位：1：单类单目标；2：单类多目标；低4位：1：全部点平均；2：幅度峰值点；3：最大与相邻次大点加权
    char cCharacter;//特征提取方式，关联解算
    char cRelate;//关联方式关联解算，1：一对一关联；2：一对多关联
    char cRelateCIP;//关联距离门限关联解算，单位：m，与CPI相关，乘以比例系数
    char cRelateLimit;//关联幅度门限关联解算，单位：dB
    char cDFCStandard;//DFC标准差门限关联解算，单位：0.1m/s
    char cDFCSpeed;//DFC速度上限关联解算 单位：m/s
    char cUncoupled;//解耦合方式关联解算 0：不解耦合；1：三重频平均；2：取最近重频
    char cSave6[4];
    char cWEnable;//波束融合使能波束融合 1：打开；2：关闭
    char cWSpeed;//波束融合速度门限 单位：0.1m/s
    char cWDistance;//波束融合距离门限 单位：m
    char cWRange;//波束融合幅度门限 单位：0.1dB
    char cWAngle1;//波束融合角度门限1  单位：0.1°
    char cWAngle2;//波束融合角度门限2  单位：0.1°
    char cWTestAng;//波束融合测角方式：单边测角；2：双边测角
    char cWTestAngRate;//波束融合测角幅度比上限单位：dB
    char cWDistanceLow[2];//波束融合目标距离下限，单位：m
    char cWDistanceUp[2];//波束融合目标速度下限，单位：0.1m/s
    char cWSpeedLow[2];//波束融合目标速度下限，单位：0.1m/s
    char cWSpeedUp[2];//波束融合目标速度上限，单位：0.1m/s
    char cSave7[8];
    char cDelay[2];//处理时延 单位：1ms
    char cDisplay[2];//显示批号上限
    char cMaxPoint[2];//每帧数据最大点数
    char cPointCycle;//点迹数据删除周期数
    char cTmpCycle;//临时航迹删除周期数
    char cSteadyCycle;//稳定航迹删除周期数
    char cSteadyNotRel;//稳定航迹不相关周期数
    char cTmpToSteady;//临时航迹升级到稳定航迹点数
    char cTrackDistance[4];//航迹融合距离门限
    char cTrackSpeed[4];//航迹融合速度门限
    char cMinSpeed[4];//目标运动最小线速度
    char cMaxSpeed[4];//目标运动最大线速度
    char cPosition[4];//位置相关门下限
    char cWarpDirection[4];//径向距离标准差
    char cDirecAngel[4];//方位角标准差
    char cWarpSpeed[4];//径向速度标准差
    char cLargeRate[4];//相关门放大倍数
    char cWCenter[4];//波束中心间隔
    char cMaxWarpSpeed[4];//最大径向速度相关门
    char cRange[4];//幅度相关门
    char cDirecLeft[2];//方位角左限，单位：0.01°
    char cDirecRight[2];//方位角右限，单位：0.01°
    char cSave8[32];//备用
    CSCI_05_Cmd_ParaSend(){
        memset(&cCmd,0,sizeof(CSCI_05_Cmd_ParaSend));
    }
};

//4.5.1.3　波位更新
struct CSCI_05_Cmd_WaveUpdate{
    char cCmd;//0x05-波位控制命令字
    char cWaveNum;//0～31,波位号
    char cReserve;//备用
};

//4.5.1.4　一致性校正指令
struct CSCI_05_Cmd_ConsistencyCorrection{
    char cCmd;//0x06-校正控制命令字
    char cCorrtype;//校正类型：0幅相校正，1正交校正
    char cChannelNum;//通道数量
    char cReserve;//备用
};

//4.5.1.5　波束能量查询指令（对应监控流程方向图测试）
struct CSCI_05_Cmd_BeamEnergy{
    char cCmd;//0x07-波束能量读取控制命令字
    char cTestOnOff;//测试开关：0-开，1-关闭
    char cWaveNum;//波位号
    char cBeamCnt;//波束数量
    char cReserve;//备用
};

//4.5.1.6　FPGA目标模拟指令（含DDS配置）(与DSP通用)
struct CSCI_05_Cmd_FPGA{
    char cCmd;//0x08-FPGA目标模拟指令
    char cDSSMode;//DDS控制模式
    char cDSSReset;//DDS复位
    char cDSS0Range[2];// DDS0幅度
    char cDSS1Range[2];// DDS1幅度
    char cDSS2Range[2];// DDS2幅度
    char cDSS3Range[2];// DDS3幅度
    char cDSS4Range[2];// DDS4幅度
    char cDSS5Range[2];// DDS5幅度
    char cDSS6Range[2];// DDS6幅度
    char cDSS7Range[2];// DDS7幅度
    char cDSS8Range[2];// DDS8幅度
    char cDSS9Range[2];// DDS9幅度

    char cDSS10Range[2];//...
    char cDSS11Range[2];//...
    char cDSS12Range[2];//...
    char cDSS13Range[2];//...
    char cDSS14Range[2];//...
    char cDSS15Range[2];//...
    char cDSS16Range[2];//...
    char cDSS17Range[2];//...
    char cDSS18Range[2];//...
    char cDSS19Range[2];//...

    char cDSS20Range[2];//...
    char cDSS21Range[2];//...
    char cDSS22Range[2];//...
    char cDSS23Range[2];//...
    char cDSS24Range[2];//...
    char cDSS25Range[2];//...
    char cDSS26Range[2];//...
    char cDSS27Range[2];//...
    char cDSS28Range[2];//...
    char cDSS29Range[2];//...

    char cDSS30Range[2];//...
    char cDSS31Range[2];//...
    char cDSS32Range[2];//...
    char cDSS33Range[2];//...
    char cDSS34Range[2];//...
    char cDSS35Range[2];//...
    char cDSS36Range[2];//...
    char cDSS37Range[2];//...
    char cDSS38Range[2];//...
    char cDSS39Range[2];//...

    char cDSS40Range[2];//...
    char cDSS41Range[2];//...
    char cDSS42Range[2];//...
    char cDSS43Range[2];//...
    char cDSS44Range[2];//...
    char cDSS45Range[2];//...
    char cDSS46Range[2];//...
    char cDSS47Range[2];//...
    char cDSS48Range[2];//...
    char cDSS49Range[2];//...

    char cDSS50Range[2];// ...
    char cDSS51Range[2];// ...
    char cDSS52Range[2];// ...
    char cDSS53Range[2];// ...
    char cDSS54Range[2];// ...
    char cDSS55Range[2];// ...
    char cDSS56Range[2];// ...
    char cDSS57Range[2];// ...
    char cDSS58Range[2];// ...
    char cDSS59Range[2];// ...

    char cDSS60Range[2];//...
    char cDSS61Range[2];//...
    char cDSS62Range[2];//...
    char cDSS63Range[2];//...
    char cDSS64Range[2];//DDS64幅度

    //频率控制
    char cDSS0Ctrl[4];// DDS0频率控制字
    char cDSS1Ctrl[4];// DDS1频率控制字
    char cDSS2Ctrl[4];// DDS2频率控制字
    char cDSS3Ctrl[4];// DDS3频率控制字
    char cDSS4Ctrl[4];// DDS4频率控制字
    char cDSS5Ctrl[4];// DDS5频率控制字
    char cDSS6Ctrl[4];// DDS6频率控制字
    char cDSS7Ctrl[4];// DDS7频率控制字
    char cDSS8Ctrl[4];// DDS8频率控制字
    char cDSS9Ctrl[4];// DDS9频率控制字

    char cDSS10Ctrl[4];//...
    char cDSS11Ctrl[4];//...
    char cDSS12Ctrl[4];//...
    char cDSS13Ctrl[4];//...
    char cDSS14Ctrl[4];//...
    char cDSS15Ctrl[4];//...
    char cDSS16Ctrl[4];//...
    char cDSS17Ctrl[4];//...
    char cDSS18Ctrl[4];//...
    char cDSS19Ctrl[4];//...

    char cDSS20Ctrl[4];//...
    char cDSS21Ctrl[4];//...
    char cDSS22Ctrl[4];//...
    char cDSS23Ctrl[4];//...
    char cDSS24Ctrl[4];//...
    char cDSS25Ctrl[4];//...
    char cDSS26Ctrl[4];//...
    char cDSS27Ctrl[4];//...
    char cDSS28Ctrl[4];//...
    char cDSS29Ctrl[4];//...

    char cDSS30Ctrl[4];//...
    char cDSS31Ctrl[4];//...
    char cDSS32Ctrl[4];//...
    char cDSS33Ctrl[4];//...
    char cDSS34Ctrl[4];//...
    char cDSS35Ctrl[4];//...
    char cDSS36Ctrl[4];//...
    char cDSS37Ctrl[4];//...
    char cDSS38Ctrl[4];//...
    char cDSS39Ctrl[4];//...

    char cDSS40Ctrl[4];//...
    char cDSS41Ctrl[4];//...
    char cDSS42Ctrl[4];//...
    char cDSS43Ctrl[4];//...
    char cDSS44Ctrl[4];//...
    char cDSS45Ctrl[4];//...
    char cDSS46Ctrl[4];//...
    char cDSS47Ctrl[4];//...
    char cDSS48Ctrl[4];//...
    char cDSS49Ctrl[4];//...

    char cDSS50Ctrl[4];// ...
    char cDSS51Ctrl[4];// ...
    char cDSS52Ctrl[4];// ...
    char cDSS53Ctrl[4];// ...
    char cDSS54Ctrl[4];// ...
    char cDSS55Ctrl[4];// ...
    char cDSS56Ctrl[4];// ...
    char cDSS57Ctrl[4];// ...
    char cDSS58Ctrl[4];// ...
    char cDSS59Ctrl[4];// ...

    char cDSS60Ctrl[4];//...
    char cDSS61Ctrl[4];//...
    char cDSS62Ctrl[4];//...
    char cDSS63Ctrl[4];//...
    char cDSS64Ctrl[4];//DDS64频率控制字

    //相位初始化
    char cDSS0Init[4];// DDS0相位初始化值
    char cDSS1Init[4];// DDS1相位初始化值
    char cDSS2Init[4];// DDS2相位初始化值
    char cDSS3Init[4];// DDS3相位初始化值
    char cDSS4Init[4];// DDS4相位初始化值
    char cDSS5Init[4];// DDS5相位初始化值
    char cDSS6Init[4];// DDS6相位初始化值
    char cDSS7Init[4];// DDS7相位初始化值
    char cDSS8Init[4];// DDS8相位初始化值
    char cDSS9Init[4];// DDS9相位初始化值

    char cDSS10Init[4];//...
    char cDSS11Init[4];//...
    char cDSS12Init[4];//...
    char cDSS13Init[4];//...
    char cDSS14Init[4];//...
    char cDSS15Init[4];//...
    char cDSS16Init[4];//...
    char cDSS17Init[4];//...
    char cDSS18Init[4];//...
    char cDSS19Init[4];//...

    char cDSS20Init[4];//...
    char cDSS21Init[4];//...
    char cDSS22Init[4];//...
    char cDSS23Init[4];//...
    char cDSS24Init[4];//...
    char cDSS25Init[4];//...
    char cDSS26Init[4];//...
    char cDSS27Init[4];//...
    char cDSS28Init[4];//...
    char cDSS29Init[4];//...

    char cDSS30Init[4];//...
    char cDSS31Init[4];//...
    char cDSS32Init[4];//...
    char cDSS33Init[4];//...
    char cDSS34Init[4];//...
    char cDSS35Init[4];//...
    char cDSS36Init[4];//...
    char cDSS37Init[4];//...
    char cDSS38Init[4];//...
    char cDSS39Init[4];//...

    char cDSS40Init[4];//...
    char cDSS41Init[4];//...
    char cDSS42Init[4];//...
    char cDSS43Init[4];//...
    char cDSS44Init[4];//...
    char cDSS45Init[4];//...
    char cDSS46Init[4];//...
    char cDSS47Init[4];//...
    char cDSS48Init[4];//...
    char cDSS49Init[4];//...

    char cDSS50Init[4];// ...
    char cDSS51Init[4];// ...
    char cDSS52Init[4];// ...
    char cDSS53Init[4];// ...
    char cDSS54Init[4];// ...
    char cDSS55Init[4];// ...
    char cDSS56Init[4];// ...
    char cDSS57Init[4];// ...
    char cDSS58Init[4];// ...
    char cDSS59Init[4];// ...

    char cDSS60Init[4];//...
    char cDSS61Init[4];//...
    char cDSS62Init[4];//...
    char cDSS63Init[4];//...
    char cDSS64Init[4];//DDS64相位初始化值

    char cMultiPulseF1[4];
    char cMultiPulseF2[4];
    char cMultiPulseF3[4];

    char cMultiPulseS1[4];
    char cMultiPulseS2[4];
    char cMultiPulseS3[4];

    char cReserve[32];
};

//4.5.1.7　DSP1目标模拟指令
struct CSCI_05_Cmd_DSP1Sim{
    char cCmd;//
    char cSimOnOff;
    char cSimCode;
    char cReserve[20];
};

//4.5.2.1　设备装订指令回执
//4.5.2.2　算法参数下发回执
//4.5.2.3　波位更新回执
//设备控制回执(0xC0)
//struct CmdFeedback;

//4.5.2.4　一致性校正回执
struct CSCI_05_CmdFeedback_ConsistencyCorrection{
    char cCmd;//参见控制指令
    char cCorrType;//校正类型
    char cChannelCnt;//通道数量
    char cSave[4];//保留
    char cCorrRate[64*4];//校正系数
    char cReverse[20];//保留
};

struct CSCI_05_CmdFeedback_BeamEnergy{
    char cCmd;//参见控制指令
    char cWaveNum;//波位号
    char cBeamCnt;//波束数量
    char cBeamEnergy[32*4];//波束能量
    char cPointAngle[2];//指向角度
    char cReserve[20];//备用
};

//4.5.2.6　目标模拟指令回执
//设备控制回执(0xC0)
//struct CmdFeedback;

/*****************************************************/
//4.9　监控软件与数据处理软件的接口（CSCI_10）
//4.9.1　设备自检指令
//4.9.3　雷达开关机指令
//4.9.1　方向图测试指令
//4.9.3　校正指令
//4.9.5　动目标模拟指令
//4.9.7　雷达参数还原指令
//...
struct CSCI_10_Cmd_General{
    char cCmd;
};

//4.9.5　数据率切换指令
//4.9.7　功率切换指令
struct CSCI_10_Cmd_GeneralEx{
    char cCmd;
    char cValue;
};

//4.9.2　设备自检回执
//4.9.4　雷达开关机回执
//4.9.6　数据率切换回执
//4.9.2　方向图测试回执
//4.9.4　校正回执
//4.9.6　动目标模拟回执
//4.9.8　雷达参数还原回执
//...
struct CSCI_10_CmdFeedback_General{
    char cCmd;
    char cResult;
    char cReverse[4];
};

struct CSCI_10_DevStateInfo{
    char cCmd;//0x33-处理终端状态信息
    char cTempA[4];//机箱内A点温度
    char cTempB[4];//机箱内B点温度
    char cV1[4];//电源1电压值
    char cA1[4];//电源1电流值
    char cV2[4];//电源2电压值
    char cA2[4];//电源2电流值
    char cV3[4];//电源3电压值
    char cA3[4];//电源3电流值
    char cCPU;//cpu使用率
    char cRAM;//内存使用率
    char cHard;//硬盘占用率
    char cWorkState;//处理终端是否处于工作状 0-工作，1-空闲

    char cRaderCode[5];//雷达编号(LDXXX)
    char cReverse[4];//备用
};

//4.10.1　设备控制指令
struct CSCI_11_Cmd{
    char cCmd;//0x02-控制命令字
    char cBehive;//伺服控制，0-停止，1-转动
    char cDesAngle[2];//目标角度
    char cRotateSpeed;//转速
};

//4.10.2　设备控制回执
//设备控制回执(0xC0)
//struct CmdFeedback;

struct CSCI_13_RaderState{
    char cRaderState;//
    //中频源
    char cWorkMode;//工作模式：0-单频，1-锯齿波变周期（10MHz），2-锯齿波变周期（5MHz）
    char cInput;//脉冲输入：0-无脉冲输入，1-有脉冲输入
    char cDataRate;//数据率，0~3s，1~6s，2~12s
    char cOutputRange1;//输出幅度，0~30db
    char cDevState1;//设备工作状态，0-故障，1-正常，2-中断
    char cReverse1[4];//

    //源及变频组件
    char cFreqP;
    char cLockCmd;
    char cOutputRange2;
    char cDevState2;
    char cReverse2[4];

    //校正源
    char cOnOff;//开关状态
    char cFreqP2;//频点
    char cOutputRange3;//输出幅度，0~30db
    char cDevState3;//设备工作状态，0-故障，1-正常，2-中断
    char cReverse3[4];

    //发射组件
    char cOnOff2;//开关状态0—关闭，1—打开
    char cReduce;//衰减值0-63
    char cMove;//移相值0-63
    char cTemp;//组件温度0-200，表示-50℃-150℃，量纲：1℃
    char cAState;//组件电流状态0x01-打开，0x00-关闭
    char cOutPower;//输出功率值
    char cWorkState0;//设备工作状态，0-故障，1-正常，2-中断
    char cReverse4[4];

    //DSP1
    char cDSP1State;
    char cDSP1Version;
    char cDSP1Breakdown;
    char cDSP1CustBreak1;
    char cDSP1CustBreak2;
    char cDSP1InputCheck1;

    char cFPGAVersion[2];//FPGA版本号
    char cFPGABreakdown;//FPGA故障状态
    char cFPGAInputCheck;//FPGA输入信息检测
    char cFPGACustBreak1;//FPGA自定义故障状态1
    char cFPGACustBreak2;//FPGA自定义故障状态2
    char cReverse5[4];//备用

    //DSP2
    char cDSP2State;//DSP2设备状态 0-故障，1-正常，2-中断
    char cDSP2Version;//版本号
    char cDSP2Breakdown;//故障状态
    char cDSP2CustBreak1;//自定义故障状态1
    char cDSP2CustBreak2;//自定义故障状态2
    char cDSP2InputCheck1;//输入状态检测
    char cReverse6[4];//备用

    //监控软件
    char cDevState4;//监控软件设备状态 0-故障，1-正常，2-中断
    char cCPU;//cpu使用率
    char cRAM;//内存使用率
    char cHard;//硬盘占用率
    char cReverse7[4];

    //数据处理软件
    char cDevState5;//数据处理软件设备状态 0-故障，1-正常，2-中断
    char cTempA[4];//机箱内A点温度
    char cTempB[4];//机箱内B点温度
    char cV1[4];//电源1电压值
    char cA1[4];//电源1电流值
    char cV2[4];//电源2电压值
    char cA2[4];//电源2电流值
    char cV3[4];//电源3电压值
    char cA3[4];//电源3电流值
    char cCPU2;
    char cRAM2;//内存使用率
    char cHard2;//硬盘占用率
    char cWorkState;//工作状态。0-工作，1-空闲
    char cRaderCode[5];//雷达编号(LDXXX)
    char cReverse8[4];

    //座体
    char cBaseState;//状态0-停止，1-转动.(座体)
    char cCurAngle;//当前角度
    char cRoSpeed;//转速
    char cBaseWState;//座体设备工作状态，0-故障，1-正常，2-中断
    char cReverse9[4];//备用
};

//各设备控制指令
//各设备回执指令


//点迹数据

//航迹数据
#endif // DATASTRUCT_H
