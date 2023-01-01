#ifndef __PQ_TRANSITION_H__
#define __PQ_TRANSITION_H__
////////////////General Define////////////////
#define BIT0    0x01
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80

#define TColorReg_CheckCode 0x3355AACC
#define TTransitionSettings_CheckCode 0x3355AACC

#include "BluLight_Defender.h"

#ifdef _WIN32
#define TRS_LOGD(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#else
#define LOG_TAG "TRS"
#include <android/log.h>
#define TRS_LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define TRS_LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define TRS_LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define TRS_LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)

#endif  // _WIN32
enum{
    ENUM_BrightToDark,
    ENUM_DarkToBright
};

struct TC3DReg {
    unsigned int C3D_LUT[4913][3];
};

struct TColorReg {
    ColorRegisters ColorReg;
    int Backlight;
    int CheckCode;
};

struct TPQTransition {
    TC3DReg C3DArray;
    unsigned int lut_size;
};


struct TPQTransitionInput{
    TColorReg InputColor;
};

#pragma pack(1)
struct TTransitionSettings {
    int Step;
    int Speed;      //How often to do transition. 0: every times (fastest), >0, every x
    int Mode;       //0: Base on Step and Speed, 1: Weight Mode, 2: Fixed time mode
    int CheckCode;
};

struct TPQTransitionReg{
    int Enabled;   //false: one step, true: progressive
    int DebugFlag;              //Bit 0: All Transition Data, 1: Transition Diff, 2: General Log, 3: Current Step
    TTransitionSettings BrightToDarkTransitionSettings;
    TTransitionSettings DarkToBrightTransitionSettings;
    TTransitionSettings C3DTransitionSettings;
    int Strength;   //Reserved (Gain, for easy tuning)
    int flt_coef_min;
    int flt_coef_max;
    int flt_ovr_bgain_th;
    int flt_ovr_bgain_slope;
    int flt_coef_min2;
    int flt_coef_max2;
    int flt_ovr_bgain_th2;
    int flt_ovr_bgain_slope2;
    //Debug
    int DebugTrace;
    int reserved[16];
};
#pragma pack()

struct TPQTransitionOutput
{
    TColorReg OutputColor;
};


struct TPQTransitionInitParam {
    unsigned int reserved; // not used
    unsigned int is13Bit;
};

// for share memory
struct TColorShareData
{
    TPQTransitionReg PQTransitionReg;
    TColorReg PreviousColorReg;
    TColorReg CurrentColorReg;

    int CurrentStep;
    int CurrentCount;
    int CurrentBrightDarkMode;
    int Finished;
};

class TPQTransitionProcess {
private:

    TColorReg *PreviousColorReg;
    TColorReg *TargetColorReg;
    TColorReg *CurrentColorReg;

    TC3DReg *PreviousC3DReg;
    TC3DReg *TargetC3DReg;
    TC3DReg *CurrentC3DReg;

    int CurrentStep;
    int CurrentCount;
    int CurrentBrightDarkMode;  //0: Bright to Dark, 1: Dark to Bright
    int Finished;     //To check if transition is finish yet
    int Finished_C3D;     //To check if transition is finish yet
    int FinalCoef;
    int FinalCoef_C3D;

    int PrintTColorReg(TColorReg *CurrentColorReg);
    int PrintTColorReg(TColorReg *TargetColorReg, TColorReg *PreviousColorReg, TColorReg *CurrentColorReg);

    int CheckIfFinished();      //0: not finished, 1: finished
    int CheckIfFinished_C3D();
    void onTransitionMode0(const TTransitionSettings &TransitionSettings);
    void onTransitionMode1(const TTransitionSettings &TransitionSettings);
    void onTransitionMode2(const TTransitionSettings &TransitionSettings);
    void onTransitionMode3(const TTransitionSettings &TransitionSettings, int coef);
    void onTransitionMode0_C3D(const TTransitionSettings &TransitionSettings);
    void onTransitionMode1_C3D(const TTransitionSettings &TransitionSettings);
    void onTransitionMode2_C3D(const TTransitionSettings &TransitionSettings);
    void onTransitionMode3_C3D(const TTransitionSettings &TransitionSettings, int coef);
    int onTransition(const TTransitionSettings &TransitionSettings, TPQTransitionOutput *output);
    int onTransition(const TTransitionSettings &TransitionSettings, TPQTransition *output);
    int onTransition(TPQTransitionOutput *output);         //0: not finished, 1: finished

public:
    //TColorShareData *BackupColorShareData;
    TPQTransitionReg *PQTransitionReg;

    TPQTransitionProcess();
    ~TPQTransitionProcess();

    void onInitCommon(const TPQTransition &input);
    void onInitPlatform(const TPQTransitionInitParam &initParam);
    void onLoadXML(unsigned char* setting);
    void setEnabled(bool enabled);
    bool isEnabled();
    void SetBrightToDarkStep(int step);
    void SetDarkToBrightStep(int step);
    void SetC3DStep(int step);
    void SetTRSMode(int mode);
    //void SetTRSMode_C3D(int mode);
    int GetBrightToDarkStep(void);
    int GetDarkToBrightStep(void);
    int GetC3DStep(void);

    int isFinished();           //0: not finished, 1: finished
    int isFinished_C3D();
    int AssignPQTransitionRegBrightToDarkTransitionSettings(int *trs_bd_param_ptr);
    int AssignPQTransitionRegDarkToBrightTransitionSettings(int *trs_db_param_ptr);

    int AssignPQTransitionNewTarget(const TPQTransitionInput &input);
    int AssignPQTransitionNewTarget(const TPQTransition &input);

    int onCalculate(const TPQTransitionInput &input, TPQTransitionOutput *output);  //0: finished, 1: not finished
    int onCalculate_C3D(const TPQTransition &input, TPQTransition *output);
    int onCalculate_intelligent(const TPQTransitionInput &input, TPQTransitionOutput *output);  //0: finished, 1: not finished
    void setDebugFlag(int debugFlag);

    // Return true if success
    bool setTuningField(int field, unsigned int value);
    bool getTuningField(int field, unsigned int *value);

    // Share memory
    void setColorShareMemory(TColorShareData *sharedata);
    void getColorShareMemory(TColorShareData *sharedata);
    void initColorShareMemory(TColorShareData *sharedata);
#ifndef _WIN32
    // Debug logger
    void logMechanism();
    PQLoggingMechanism *pLogMech;
    enum log_tag_id
    {
        TRS = 0,
        log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];
#endif
};

#endif
