#ifndef __SILKYBRIGHTNESS_H__
#define __SILKYBRIGHTNESS_H__

#define IOFORMAT 0
#define SB_ANDROID_PLATFORM

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef SB_ANDROID_PLATFORM
#define LOG_TAG "SB"
#include "PQLoggingMechanism.h"


//#else

//#define SB_LOGD(fmt, ...) printf(fmt"\n", __VA_ARGS__)

#endif






struct SilkyBrightnessInput {
    unsigned int brightness_in;
    unsigned int BLMapping;  // ex: 8192 to 2048
    unsigned int RGain;
    unsigned int GGain;
    unsigned int BGain;
    unsigned int mode;
    int ALS_value;
    unsigned int CCORR_in[3][3];
};

struct SilkyBrightnessOutput {
    unsigned int CCORR_out[3][3];
    unsigned int activeTrigger;
    int brightness_out;
    float gamma_RGain;
    float gamma_GGain;
    float gamma_BGain;
};

struct SilkyBrightnessInitParam {
    unsigned int *sb_als_th;
    unsigned int *max_level;
    unsigned int *real_backlight_bit;
    unsigned int *HW_sel;
    unsigned int *gain_num; // 1 or 3
    unsigned int *BL_sel;
    unsigned int is13Bit;
};

struct SBSWReg{
    int sb_fw_en;
    int sb_als_th;
    int max_level;
    int real_backlight_bit;
    int HW_sel;
    int gain_num;
    int BL_sel;
    int is13Bit;
};

class SilkyBrightness {
private:


public:
  // Debug logger
  PQLoggingMechanism *pLogMech;
  enum log_tag_id
  {
      SB_Input = 0,
      SB_Output,
      SB_SWReg,
      SB_Info,
      log_tag_len
  };
  char **log_tag;
  int log_state_list[log_tag_len];

    SBSWReg * pSBSWReg;


    SilkyBrightness()
    {
        memset(this, 0, sizeof(*this));
        pSBSWReg    =  new SBSWReg;
#ifdef SB_ANDROID_PLATFORM
        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
        "SB_Input",
        "SB_Output",
        "SB_SWReg",
        "SB_Info"
        };

        for(int i = 0; i < log_tag_len; ++i)
        log_state_list[i] = ANDROID_LOG_DISABLE;
#endif
    };

    ~SilkyBrightness()
    {
        delete pSBSWReg;
#ifdef SB_ANDROID_PLATFORM
        delete pLogMech;
        delete log_tag;
#endif
    };

    void setEnabled(bool enabled);

    bool getEnabled();

    void setALSthred(unsigned int als_thred);

    unsigned int getALSthred();

    void onInitCommon();
    void onInitPlatform(const SilkyBrightnessInitParam &initParam);
    void onCalculate(const SilkyBrightnessInput &input, SilkyBrightnessOutput *output);


    // Return true if success
    bool setTuningField(int field, unsigned int value);
    bool getTuningField(int field, unsigned int *value);

};

#endif
