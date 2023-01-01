#ifndef __BLULIGHT_DEFENDER_H__
#define __BLULIGHT_DEFENDER_H__

#define CLR_PARTIAL_Y_SIZE 16
#define CLR_PQ_PARTIALS_CONTROL 5
#define CLR_PURP_TONE_SIZE 3
#define CLR_SKIN_TONE_SIZE 8
#define CLR_GRASS_TONE_SIZE 6
#define CLR_SKY_TONE_SIZE 3
#ifndef _WIN32
#include "PQLoggingMechanism.h"
#endif
#ifdef _WIN32
#define BL_LOGD(fmt, ...) printf(fmt"\n", ##__VA_ARGS__)
#else

#define LOG_TAG "BLD"
#include <android/log.h>
#include <utils/Timers.h>
#define BL_LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define BL_LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define BL_LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define BL_LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)

#endif  // _WIN32

#define IS_FIELD_LEGAL(f, s) (f > sizeof(s) - sizeof(unsigned int) || (f & 0x3) != 0)

struct ColorRegisters {
    unsigned int GLOBAL_SAT  ;
    unsigned int CONTRAST    ;
    unsigned int BRIGHTNESS  ;
    unsigned int PARTIAL_Y    [CLR_PARTIAL_Y_SIZE];
    unsigned int PURP_TONE_S  [CLR_PQ_PARTIALS_CONTROL][CLR_PURP_TONE_SIZE];
    unsigned int SKIN_TONE_S  [CLR_PQ_PARTIALS_CONTROL][CLR_SKIN_TONE_SIZE];
    unsigned int GRASS_TONE_S [CLR_PQ_PARTIALS_CONTROL][CLR_GRASS_TONE_SIZE];
    unsigned int SKY_TONE_S   [CLR_PQ_PARTIALS_CONTROL][CLR_SKY_TONE_SIZE];
    unsigned int PURP_TONE_H  [CLR_PURP_TONE_SIZE];
    unsigned int SKIN_TONE_H  [CLR_SKIN_TONE_SIZE];
    unsigned int GRASS_TONE_H [CLR_GRASS_TONE_SIZE];
    unsigned int SKY_TONE_H   [CLR_SKY_TONE_SIZE];
    unsigned int CCORR_COEF   [3][3];
};

struct BluLightInfo{
    unsigned int ColorHist[256][3];
    unsigned int bin_num;
    unsigned int W;
    unsigned int H;
    float BL_index;
    float BL_chromaticity1;
    float BL_chromaticity2;
    float X_sensor;
    float Y_sensor;
    float Z_sensor;
    bool BL_updateHist;
    int BL_fenceHist;
    int BL_fenceIntelligent;
};

struct BluLightOutput {
    int Panel_cct;
    float Panel_index2;
    int Panel_level;
    float Panel_chromaticity1;
    float Panel_chromaticity2;
};

struct BluLightInitParam {
    unsigned int reserved; // not used
    unsigned int is13Bit;
};
#pragma pack(1)
struct Manual_tune {
    float H, S1, S2, Y;
    int Hidx;
};
struct XYZ {
    float X;
    float Y;
    float Z;
};

struct RGB_XYZ {
    XYZ R;
    XYZ G;
    XYZ B;
    XYZ W;
};
struct BluLightReg {
    float cT2Deg;
    float cT1Deg;
    float strengthLimit;
    RGB_XYZ Col_XYZ;
    float tuneDx;
    float tuneDy;

    float regWgt1, regWgt2;
    float regThr1, regThr2;
    float sbyhWgt[20];
    Manual_tune tune[7];

    float bld_mod_ratio;
    int bld_mod_deg;
    float bld_level_para1;
    float bld_level_para2;
    float bld_level_para3;
    float bld_th1;
    int bld_th2;
    int Vavg;
    int lowv_level;
    int nonlinear_en;
    float index_cali;
    int xy_cali_en;
    int bld_th3;
    int manual;
    int time_delay;
    float index2_th;
    int common_mode;
    float reserved[6];

    float cTOriCT;
    int remainShiftEn;
    float shiftDx;
    float shiftDy;
    float Trg_x;
    float Trg_y;
    float XYZ2RGB[9];
    float tuneCCORR[9];
    float invCCORR[9];
};
#pragma pack()
class BluLightDefender {
private:
    bool m_isEnabled;
    bool sbd_en;
    int m_strength;
    int m_percentage;
    int m_nonlinear_en;
    unsigned int m_debugFlag;
    unsigned int group_flag;
    int flag_frame0;
    float pre_index2;
    float pre_chromaticity1;
    float pre_chromaticity2;
    int pre_islowVavg;
    unsigned int pre_Cus_CCORR[3][3];
    int sceneChanged_flag;
    int tuning_flag;
    float RGB2XYZ_CalCCT[9];
    nsecs_t begin;
    nsecs_t end;

    RGB_XYZ Apanel;
    float spect0255[3][256][81];
    float Org_x1,Org_y1,yellow_x2, yellow_y2,nonBlu_rotx,nonBlu_roty,Trg_NEW_x,Trg_NEW_y,Trg_OLD_x,Trg_OLD_y;
    ColorRegisters tempreg;
    ColorRegisters backupreg;
    void PanelValueUpdate(BluLightReg *BLReg,int UI_strength);
    void TargetWhiteUpdate(BluLightReg *BLReg, int UI_strength);
    void CVApanelCIE1931Cal(float spect0255[3][256][81],const unsigned int RGBhistogram[256][3],const unsigned int bin_num, const unsigned int Width, const unsigned int Height, float *x, float *y, float *bl);
public:
    BluLightReg *BLReg;
    BluLightDefender();
    ~BluLightDefender();

    void setEnabled(bool enabled) {
        tuning_flag = 1;
        m_isEnabled = enabled;
    }

    bool isEnabled() {
        return m_isEnabled;
    }

    void setStrength(int strength); // 0 ~ 255

    int getStrength() {
        return m_strength;
    }
    int getPercentage() {
        return m_percentage;
    }
    void setEnabled_Intelligent(bool enabled);
    bool isEnabled_Intelligent();
    bool isCommonMode();
    void setNonlinear_BLU(int nonlinear_en);
    void onInitCommon(const BluLightInitParam &initParam);
    void onInitCommon_intelligent(const BluLightInitParam &initParam);
#ifndef _WIN32
    void onInitPlatform(const BluLightInitParam &initParam);
    void onLoadXML(unsigned char* setting);
#endif
    void onCalculate(const ColorRegisters &input, ColorRegisters *output);
    void onCalculate_intelligent(const ColorRegisters &input, ColorRegisters *output, const BluLightInfo &Info, BluLightOutput *BLoutput);
    void getResult(ColorRegisters *output);
    void setDebugFlag(unsigned int debugFlag) {
        m_debugFlag = debugFlag;
    }

    // Return true if success
    bool setTuningField(int field, unsigned int value);
    bool getTuningField(int field, unsigned int *value);

    void readField(unsigned int field, unsigned int *value);
    void writeField(unsigned int field, unsigned int value);
    bool isFieldLegal(unsigned int field);

#ifndef _WIN32

  // Debug logger
  void logMechanism();
  PQLoggingMechanism *pLogMech;
  enum log_tag_id
  {
      BLD = 0,
      log_tag_len
  };
  char **log_tag;
  int log_state_list[log_tag_len];
#endif  // _WIN32
};

#endif
