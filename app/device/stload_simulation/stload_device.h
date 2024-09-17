/**
 * @file stload_device.h
 * @author hhool (hhool@outlook.com)
 * @brief st load device simulation header file
 * @version 0.1
 * @date 2024-09-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_STLOAD_SIMULATION_STLOAD_DEVICE_H_
#define APP_DEVICE_STLOAD_SIMULATION_STLOAD_DEVICE_H_

// dll export and import define for windows
#ifdef _WIN32
#ifdef STLOAD_SIMULATION_EXPORTS
#define STLOAD_SIMULATION_API extern __declspec(dllexport)
#else
#define STLOAD_SIMULATION_API extern __declspec(dllimport)
#endif
#else
#define STLOAD_SIMULATION_API
#endif
 
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#define CALL __stdcall
#else
#define CALL
error "Only Windows is supported"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// DTC500.h: interface for the DTC500 class.

//////////////////////////////////////////////////////////////////////////
//
#define MAXCORR 5  // 线性修正的段数
// #define PI			3.14159
#define FILEFLAGS 0xF1E556A2  // 保存时的文件头标志
// 硬件通道号预定义CH_XXXX
#define MAXCHANNEL 16  // 最多16通道
#define CH_POSI 0      // 位移
#define CH_LOAD 1      // 负荷
#define CH_EXTN 2      // 引伸计
#define CH_BIGU 3      // 大变形上
#define CH_BIGD 4      // 大变形下
#define CH_EXT1 5      // 扩展1
#define CH_EXT2 6      // 扩展2
#define CH_EXT3 7      // 扩展3
#define CH_EXT4 8      // 扩展4
#define CH_EXT5 9      // 扩展5
#define CH_TIME 10     // 时间
#define POS_SP 11      // 位移速度
#define LOAD_SP 12     // 力速度
#define EXTN_SP 13     // 引伸计速度
#define MACHINE 14     // 主机

#define NONE 100  // 无

// 标志采样相应的位,采样时,当所有需要采样的都至少完成一次采样后
// 才送给上位机一个消息SMP_XXXX
#define SMP_LOAD 0x00000001  // 负荷
#define SMP_EXTN 0x00000002  // 引伸计
#define SMP_EXT1 0x00000004  // 扩展1
#define SMP_EXT2 0x00000008  // 扩展2
#define SMP_EXT3 0x00000010  // 扩展3
#define SMP_EXT4 0x00000020  // 扩展4
#define SMP_EXT5 0x00000040  // 扩展5
#define SMP_EXT6 0x00000080  // 扩展6
#define SMP_BIGU 0x00000100  // 大变形上
#define SMP_POSI 0x00000200  // 位移
#define SMP_BIGD 0x00000400  // 大变形下
#define SMP_SPD 0x80000000   // 速度
#define SMP_ALL 0x800007FF   // 所有的都需要

// 控制方式CTRL_XXXX
#define CTRL_LOAD 0  // 力控
#define CTRL_EXTN 1  // 变形控
#define CTRL_POSI 2  // 位移控

// 结束条件END_XXXX
#define END_LOAD 0  // 力相对量,AD码
#define END_EXTN 1  // 变形相对量,AD码
#define END_POSI 2  // 位移相对量,脉冲数
#define END_TIME 3  // 时间相对量,0.01s

// 运行方向DIR_XXXX
#define DIR_NO 0     // 方向由下位机自己定义
#define DIR_UP 1     // 正向
#define DIR_DOWN -1  // 负向

#define KP_DEST 0  // 保持指定值
#define KP_CURR 1  // 保持当前值

// 属于DSP内部消息,
#define DSPMSG WM_USER + 4000  // 发送给主窗口的消息
// lParam的值如下DSP_XXXX
#define DSP_KBSTOP 0x1     // 停止
#define DSP_RUN 0x2        // 运行
#define DSP_QKUP 0x4       // 快上
#define DSP_QKDWN 0x8      // 快下
#define DSP_SLWUP 0x10     // 慢上
#define DSP_SLWDWN 0x20    // 慢下
#define DSP_CMDEND 0x40    // 命令完成状态
#define DSP_BD 0x80        // 标定
#define DSP_LDCTRL 0x100   // 力控运行
#define DSP_EXCTRL 0x200   // 变形控运行
#define DSP_PSCTRL 0x400   // 位移控运行
#define DSP_TOPLMT 0x800   // 上限位
#define DSP_LOWLMT 0x1000  // 下限位
#define DSP_KBRUN 0x2000   // 小键盘运行命令
#define DSP_KBPRT 0x4000   // 小键盘试样保护命令
#define DSP_ADC 0x8000     // 被控量ADC转换标志,内部标志

#define DSP_BDREV 0x10000      // 小键盘状态
#define DSP_BATERR 0x20000     // 电池报警
#define DSP_SWITCH 0x40000     // 命令状态交替
#define DSP_PCSTS 0x80000      // PC状态
#define DSP_XXXX 0x100000      // 给定/自动位移脉冲,内部标志
#define DSP_OVERLD 0x200000    // 力超载
#define DSP_DIR 0x400000       // 向上/向下
#define DSP_OVERDIR 0x800000   // 超载时的方向
#define DSP_TEST 0x1000000     // 是否是试验状态
#define DSP_HIT 0x2000000      // 防碰撞运行
#define DSP_NGTV 0x4000000     // 负方向标志
#define DSP_PSTV 0x8000000     // 正方向标志
#define DSP_AUTOSW 0x10000000  // 自动切换
#define DSP_PSXOR 0x20000000   // 位移和上下的异或值
#define DSP_LDXOR 0x40000000   // 力和上下的异或值
#define DSP_EXXOR 0x80000000   // 引伸计与上下的异或值
#define DSP_MASK 0xFFFFFFFF    // 已经定义的状态位

#define CODE_MASK10 0xFFFFFFC0   // 10万码
#define CODE_MASK20 0xFFFFFFE0   // 20万码
#define CODE_MASK50 0xFFFFFFF0   // 50万码
#define CODE_MASK100 0xFFFFFFF8  // 100万码

#define CODE_MASK0 0xFFFFFFFF  // 不滤
#define CODE_MASK1 0xFFFFFFFE  // 滤一位
#define CODE_MASK2 0xFFFFFFFC  // 滤二位
#define CODE_MASK3 0xFFFFFFF8  // 滤三位
#define CODE_MASK4 0xFFFFFFF0  // 滤四位
#define CODE_MASK5 0xFFFFFFE0  // 滤五位
#define CODE_MASK6 0xFFFFFFC0  // 滤六位

// 属于DLL自己发送的消息,告诉主程序某些信息
#define DLLMSG WM_USER + 4001
// lParam的值如下DLL_XXXX
#define DLL_SAMPLE 0x00000001  // 一次采样完成
#define DLL_FEEDBK 0x00000002  // 通讯反馈
#define DLL_198OK 0x00000004   // 198号命令发送成功
#define DLL_199OK 0x00000008   // 199号命令发送成功
#define DLL_200OK 0x00000010   // 200号命令发送成功
#define DLL_201OK 0x00000020   // 201号命令发送成功
#define DLL_202OK 0x00000040   // 202号命令发送成功
#define DLL_203OK 0x00000080   // 203号命令发送成功
#define DLL_SETOK 0x00010000   // 初始发送命令完成

// 有关单位
#define UNT_FORCE_N 0x0001    // 牛
#define UNT_FORCE_KN 0x0002   // 千牛
#define UNT_FORCE_CN 0x0004   // 厘牛
#define UNT_FORCE_KGF 0x0008  // 公斤力
#define UNT_FORCE_LBF 0x0010  // 磅力
#define UNT_FORCE_G 0x0020    // 克

#define UNT_LENTH_MM 0x0101  // 毫米
#define UNT_LENTH_IN 0x0102  // 英寸

// 机型
#define MACHINE_HUT_SP 0x00  // 液压单空间
#define MACHINE_HUT_DP 0x01  // 液压双空间
#define MACHINE_HHCT 0x02    // 液压压力机
#define MACHINE_XHCT 0x03    // 油电混动压力机
#define MACHINE_ETM 0x04     // 电拉万能拉力机
#define MACHINE_EHCT 0x05    // 电拉压力
#define MACHINE_ETMF 0x06    // 压折
#define MACHINE_ETT 0x07     // 扭转

// 传感器位置
#define SENSOR_MIDBEAM_DOWN 0x00  // 中横梁下置
#define SENSOR_MIDBEAM_UP 0x02    // 中横梁上置
#define SENSOR_UPBEAM 0x04        // 上横梁
#define SENSOR_BOTTOMBEAM 0x06    // 下横梁

// 试验空间
#define SPACE_DOWN 0x00  // 下空间
#define SPACE_UP 0x01    // 上空间

//================导出数据结构=========================
/*************   INDATA  *************
OUTDATA.DoT		//数据类型
OUTDATA.DoD		//非扩展数据
OUTDATA.DoE		//扩展数据号
OUTDATA.Doe		//扩展数据
*************************************/
// #define DoT BOUT[0]
#define DoD _outdod._DoD
#define DoE _outdod._outdoe._DoE
#define Doe _outdod._outdoe._Doe
typedef struct {
  // BYTE BOUT[5];
  union {
    DWORD _DoD;
    struct {
      WORD _Doe;
      WORD _DoE;
    } _outdoe;
  } _outdod;
  BYTE DoT;
} OUTDATA, *POUTDATA;  // 输出的数据结构

/*************   INDATA  *************
INDATA.DiT		//数据类型
INDATA.DiD		//非扩展数据
INDATA.DiE		//扩展数据号
INDATA.Die		//扩展数据
*************************************/

/*************   CMDDATA  *************
CMDDATA.CMD_		//数据类型
CMDDATA.DWD_		//全部数据
CMDDATA.Def_		//方向由谁定义
CMDDATA.Dir_		//方向
CMDDATA.Pri_		//优先级
CMDDATA.Ctl_		//控制方式
CMDDATA.End_		//终止条件
*************************************/

#define Def_ _outcmd._cmd._Def
#define DWD_ _outcmd._DoD
#define Dir_ _outcmd._cmd._Dir
#define Pri_ _outcmd._cmd._B2
#define Ctl_ _outcmd._cmd._B1
#define End_ _outcmd._cmd._B0
typedef struct {
  union {
    DWORD _DoD;
    struct {
      unsigned _B0 : 8;
      unsigned _B1 : 8;
      unsigned _B2 : 8;
      unsigned _res : 6;
      unsigned _Dir : 1;
      unsigned _Def : 1;
    } _cmd;
  } _outcmd;
  BYTE CMD_;
} CMDDATA, *PCMDDATA;  // 命令的数据结构(其实也是输出)

typedef struct {
  HANDLE hDeviceHandle;
  ULONG nSize;
  POUTDATA pBuffer;
  HANDLE hEvent;
} WRSTRUCT, *PWRSTRUCT;

typedef struct {
  HANDLE hDeviceHandle;
  ULONG nSize;
  // PINDATA	pBuffer;
  HANDLE hEvent;
} RDSTRUCT, *PRDSTRUCT;

typedef struct {
  double Ycode;  // 实际对应的码
  double Xcode;  // 理论码
} CORR, *PCORR;  // 线修的数据结构

typedef struct {
  BYTE SendByteArray[22];
} SENDDATA;

typedef struct {
  BYTE Count;
  SENDDATA SendCmd[100];
} SENDMDBUFFUR;

typedef union {
  float flt;
  DWORD dwd;
} FLT_DWD, *PFLT_DWD;  // 用于浮点和双字节的转换

typedef union {
  struct {
    unsigned ws : 23;  // 尾数
    unsigned jm : 8;   // 阶码
    // unsigned jf : 1;	//阶码符号
    unsigned wf : 1;  // 尾数符号
  } dwd;
  float flt;  // 浮点数
} FLT386;     // 386浮点结构

typedef union {
  struct {
    unsigned ws : 15;  // 尾数
    unsigned wf : 1;   // 尾数符号
    unsigned jm : 16;  // 阶码
    // unsigned jf : 1;	//阶码符号
  } dwd;
  DWORD lng;  // 保存浮点
} FLTDSP;     // DSP浮点结构

typedef struct {
  int nSoftChannel;        // 软件通道号
  int nHardChannel;        // 硬件通道号
  long lExitSaveCode;      // 退出前保存的绝对码
  double dScale;           // 标定系数
  int nCorrNum;            // 线修段数
  double dLine1CorrX[10];  // 线修1
  double dLine1CorrY[10];  // 线修1
  double dLine2CorrX[10];  // 线修2
  double dLine2CorrY[10];  // 线修2
  int nDir;                // 运行方向与AD码增加方向的关系
  int nSign;               // AD码增加方向与值增加方向的关系
  DWORD dwReserved;        // 保留
  double dValue[4];        // 值
} SAVEITEM, *PSAVEITEM;    // 每个传感器的数据

// 上海协会联网
typedef struct {
  char sSystemPrecision[64];
  int nWorkSpace;
  float fMaxSpeed;
  double dSpeedDnScale;
  int nMotorPlus;
} MACINEIFO, *PMACINEIFO;

typedef struct {
  int Count;
  double StandardValue[20];
  double OrigValue[20];
} LINEMODIL, *PLINEMODIL;

typedef struct {
  int nEncoderPulse;
  double dWhorlDistance;
  double dMaxSpace;
  double dPulleyDia;
  CORR BDInfo[20];
  int nBDCount;

} POSITIONSENSOR, *PPOSITIONSENSOR;

typedef struct {
  char sSensorName[64];
  double dStandardLoad;
  long lFullCode;
  double dScaleValue;
  double dCorrectionFactor;
  double dDelicacy;
  LINEMODIL lineModiTensile;
  LINEMODIL lineModiCompress;
  int nBDCount;
  CORR BDInfo[20];

} LOADSENSOR, *PLOADSENSOR;

typedef struct {
  char sSensorName[64];
  double dMaxDeform;
  double dScaleValue;
  double dDelicacy;
  LINEMODIL lineModiTensile;
  LINEMODIL lineModiCompress;
  double dCorrectionFactor;
  int nBDCount;
  CORR BDInfo[20];

} EXTENSION, *PEXTENSION;

typedef struct {
  double dPulleyDia;
  int nEncoderPlus;
  double dUpFoodModi;
  double dDownFoodModi;

} LONGTRAVEL, *PLONGTRAVEL;

typedef struct {
  char sSensorName[64];
  double dStandValue;
  long lFullCode;
  double dScaleValue;
  double dDelicacy;
  LINEMODIL lineModiTensile;
  double dCorrectionFactor;
  int nBDCount;
  CORR BDInfo[20];
} EXTENDSENSOR, *PEXTENDSENSOR;

typedef struct {
  /*	//这部分还是由主程序保存
          double	dQckSpd;			//
          double	dSlwSpd;
          double	dADCSpd;
          double	dADCGain;
          int		nLoopMod;
          int		nKP;
          int		nKI;
          int		nPD;
  */
  DWORD dwReserved;  // 保留
} SAVECOMM, *PSAVECOMM;

typedef struct {
  DWORD dwFileFlag;      // 文件头
  DWORD dwReserved;      // 保留
  long lItemNum;         // 数据项数
  long lItemsize;        // 单个数据项的大小(字节数)
  long lCommsize;        // 公共数据的大小(字节数)
  DWORD dwCheck;         // 数据校验
} SAVEHEAD, *PSAVEHEAD;  // 保存的数据头

typedef struct {
  float fLoadSpeed;   // 力速度
  float fExtnSpeed;   // 变形速度
  float fExtnSwitch;  // 引伸计切换值
  float fExtnValue;   // 开始判断最大力的引伸计值
  float fKeepTime;    // 保持时间
  float fPosiSpeed;   // 位移速度
  float fThreshold;   // 阈值
  long nDir;          // 方向
  BOOL bPriority;     // 优先
} SEND198, *PSEND198;

typedef struct {
  float fLoadSpeed;   // 力速度
  float fExtnSpeed;   // 变形速度
  float fExtnSwitch;  // 引伸计切换值
  float fExtnValue;   // 开始判断最大力的引伸计值
  float fKeepTime;    // 保持时间
  //	float	fUnloadSpeed;	//卸载的力速度
  float fUnloadValue;  // 卸载的终点负荷
  float fThreshold;    // 阈值
  long nDir;           // 方向
  BOOL bPriority;      // 优先
} SEND199, *PSEND199;

typedef struct {
  BYTE byControl;    // 控制方式
  BYTE byEnd;        // 结束方式
  float fSpeed;      // 控制速度
  float fValue;      // 结束值
  float fThreshold;  // 结束阈值
  BOOL bPriority;    // 优先级
  long nDir;         // 方向
  float fKeepValue;  // 保载相对值
  long nKeepDatum;   // 保载量的基准
} SENDPARAM, *PSENDPARAM;

typedef struct {
  float fLoadSpeed;  // 力速度
  float fExtnSpeed;  // 变形速度
  float fPosiSpeed;  // 位移速度
  // float	fPosiSwitch;	//位移切换值
  float fExtnSwitch;  // 引伸强制计切换值(力控到变形控的强制切换点)
  float fExtnValue;  // 变形相对量(变形控到位移控的切换点)
  float fThreshold;  // 阈值
  long nDir;         // 方向
  BOOL bPriority;    // 优先
} SEND201, *PSEND201;

typedef struct {
  float fLoadSpeed;   // 力速度
  float fExtnSpeed;   // 变形速度
  float fExtnSwitch;  // 引伸计切换值
  // float	fPosiSwitch;	//位移切换值
  float fExtnValue;  // 变形相对量
  float fThreshold;  // 阈值
  long nDir;         // 方向
  BOOL bPriority;    // 优先
} SEND202, *PSEND202;

typedef struct {
  float fLoadSpeed;   // 力速度
  float fPosiSpeed;   // 位移速度
  float fPosiSwitch;  // 位移切换值
  float fThreshold;   // 阈值
  long nDir;          // 方向
  BOOL bPriority;     // 优先
} SEND203, *PSEND203;

typedef struct {
  long lControl;   // 控制方式
  float fFreq;     // 频率
  float fDis;      // 振幅
  float fAve;      // 均值
  long lCycCount;  // 周期数
  BOOL bPriority;  // 优先
} SEND211, *PSEND211;

typedef struct {
  long lControl;   // 控制方式
  float fFreq;     // 频率
  float fDis;      // 振幅
  float fAve;      // 均值
  long lCycCount;  // 周期数
  float lTime;     // 平台时间
  BOOL bPriority;  // 优先
} SEND216, *PSEND216;

//////////////////////////////////////////////////////////////////////////
//

STLOAD_SIMULATION_API BOOL __stdcall SetDestWnd(HWND hDestWnd);

STLOAD_SIMULATION_API BOOL __stdcall LoadHareWareParameters(int nMachineType);
STLOAD_SIMULATION_API char* __stdcall GetLoadSensors();
STLOAD_SIMULATION_API char* __stdcall GetExtensions();
STLOAD_SIMULATION_API char* __stdcall GetExtendSensors();

STLOAD_SIMULATION_API BOOL __stdcall OpenDevice(long Unit);  // NOLINT
STLOAD_SIMULATION_API BOOL __stdcall CloseDevice();

STLOAD_SIMULATION_API void __stdcall SetTestDir(long dir);   // NOLINT
STLOAD_SIMULATION_API BOOL __stdcall CarryPID(long channel,  // NOLINT
                                    long Kp,       // NOLINT
                                    long Ki,       // NOLINT
                                    long Kd);      // NOLINT

STLOAD_SIMULATION_API BOOL __stdcall OnLine(long channelNo,  // NOLINT
                                  int nLoadIndex,
                                  int nExtensionIndex,
                                  int nExtendIndex,
                                  int rate,
                                  int machineType,
                                  int DTCType,
                                  BYTE sensorPosition,
                                  BYTE CloseOfflineCheck,
                                  int dataBlockSize,
                                  BOOL isAE);
STLOAD_SIMULATION_API BOOL __stdcall OffLine();

STLOAD_SIMULATION_API BOOL __stdcall Carry200(long control,  // NOLINT
                                    long end,      // NOLINT
                                    float speed,
                                    float value,
                                    float threshold,
                                    BOOL priority,
                                    long dir,        // NOLINT
                                    float keepvalue, /* 0 */
                                    long keepdatum,  // NOLINT /* 1 */
                                    long TestModle   // NOLINT /* 0* */
);                                                   // NOLINT
STLOAD_SIMULATION_API BOOL __stdcall Carry210(long lOpen);     // NOLINT
STLOAD_SIMULATION_API BOOL __stdcall StopRun();
STLOAD_SIMULATION_API BOOL __stdcall BeforeGetSample();
STLOAD_SIMULATION_API void __stdcall AfterGetSample();
STLOAD_SIMULATION_API BOOL __stdcall StartRead();
STLOAD_SIMULATION_API BOOL __stdcall EndRead();

STLOAD_SIMULATION_API void __stdcall SetSectCorrA(long channel, double corr);  // NOLINT
STLOAD_SIMULATION_API BOOL __stdcall SetResolve(long channel,                  // NOLINT
                                      DWORD Resolve,
                                      float Nominal);

STLOAD_SIMULATION_API double __stdcall GetTestTime();
STLOAD_SIMULATION_API double __stdcall GetLoad();
STLOAD_SIMULATION_API double __stdcall GetPosi();
STLOAD_SIMULATION_API double __stdcall GetExtn();
STLOAD_SIMULATION_API double __stdcall GetExt1();
STLOAD_SIMULATION_API DWORD __stdcall GetTestStatus();

STLOAD_SIMULATION_API BOOL __stdcall TareTime();
STLOAD_SIMULATION_API BOOL __stdcall TareLoad();
STLOAD_SIMULATION_API BOOL __stdcall TareExtn();
STLOAD_SIMULATION_API BOOL __stdcall TareExt1();
STLOAD_SIMULATION_API BOOL __stdcall TarePosi();

#if defined(__cplusplus)
}
#endif

#endif  // APP_DEVICE_STLOAD_SIMULATION_STLOAD_DEVICE_H_
