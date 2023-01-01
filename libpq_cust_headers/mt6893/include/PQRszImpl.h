#ifndef __PQRSZIMPL_H__
#define __PQRSZIMPL_H__
#include "PQLoggingMechanism.h"
#include "PQCZTuning.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for clearzoom
#define ISP_SCENARIO_CNT (3)  // 0: Capture(single frame), 1: Capture(multi-frame), 2; preview
#define LENS_TYPE_CNT (4)
#define ISP_SCENARIOS (13) //ISP_SCENARIO_CNT*LENS_TYPE_CNT + 1 (Refocus)
#define UR_MAX_LEVEL (11)

#ifndef RESZIER_6TAP_STEPCOUNTER_BIT
    #define RESZIER_6TAP_STEPCOUNTER_BIT 15
#endif

//#define CLEAR_ZOOM_3_0

//#define RSZ_FOR_IMAGIQ //RSZ for IMAGIQ SIMULATOR

enum RSZDebugFlags {
    eRSZDebugDisabled          = 0x0,
    eRSZDebugEn                = 0x1,
    eRSZDebugUR                = 0x2,
    eRSZDebugRSZFW             = 0x4,
    eRSZDebugAll               = 0xFF
};


#define RSZ_ANDROID_PLATFORM

#ifdef PQ_RSZ_VERSION_0
    #define RSZ_VERSION_6799
    #define CLEAR_ZOOM_3_0
#endif

typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;
typedef signed int      int32_t;

// Initial register values to Resizer HW


struct RszInitReg {
    uint32_t reserved;  // To pass compilation
};

// Initial parameter to Resizer HW
struct RszInitParam {
    uint32_t reserved;  // To pass compilation
};

// New for ClearZoom (camera sensor related settings)
struct RszRegClearZoom
{
    uint32_t IBSE_gaincontrol_coring_value;
    uint32_t IBSE_gaincontrol_coring_thr;
    uint32_t IBSE_gaincontrol_coring_zero;
    uint32_t IBSE_gaincontrol_softlimit_ratio;
    uint32_t IBSE_gaincontrol_bound;
    uint32_t IBSE_gaincontrol_limit;
    uint32_t IBSE_gaincontrol_softcoring_gain;
    // ylevel_gain
    uint32_t IBSE_ylevel_p48;
    uint32_t IBSE_ylevel_p32;
    uint32_t IBSE_ylevel_p16;
    uint32_t IBSE_ylevel_p0;
    uint32_t IBSE_ylevel_p112;
    uint32_t IBSE_ylevel_p96;
    uint32_t IBSE_ylevel_p80;
    uint32_t IBSE_ylevel_p64;
    uint32_t IBSE_ylevel_p176;
    uint32_t IBSE_ylevel_p160;
    uint32_t IBSE_ylevel_p144;
    uint32_t IBSE_ylevel_p128;
    uint32_t IBSE_ylevel_p240;
    uint32_t IBSE_ylevel_p224;
    uint32_t IBSE_ylevel_p208;
    uint32_t IBSE_ylevel_p192;
    uint32_t IBSE_ylevel_alpha;
    uint32_t IBSE_ylevel_p256;

#ifdef CLEAR_ZOOM_3_0
    uint32_t signal_enhance_mode;
    uint32_t etc_chroma_en;
    uint32_t etc_switch_max_min_en;
    uint32_t etc_ring_ctrl_en;
    uint32_t etc_sim_prot_en;
    uint32_t etc_sim_prot_blend_mode;
    uint32_t etc_luma_curve_select;
    uint32_t etc_chroma_curve_select;
    uint32_t etc_switch_current_thr;
    uint32_t etc_switch_sim_value;
    uint32_t etc_switch_blend_w1;
    uint32_t etc_switch_blend_w2;
    uint32_t etc_switch_diff_zero;
    uint32_t etc_switch_diff_thr;
    uint32_t etc_switch_diff_bound;
    uint32_t etc_ring_ctrl_thr;
    uint32_t etc_ring_ctrl_bound;
    uint32_t etc_ring_ctrl_shift_max;
    uint32_t etc_ring_ctrl_shift_gain;
    uint32_t etc_ring_ctrl_gaincontrol_zero;
    uint32_t etc_ring_ctrl_gaincontrol_base;
    uint32_t etc_ring_ctrl_gaincontrol_thr;
    uint32_t etc_ring_ctrl_gaincontrol_softcoring_gain;
    uint32_t etc_ring_ctrl_gaincontrol_softcoring_value;
    uint32_t etc_ring_ctrl_gaincontrol_gain;
    uint32_t etc_ring_ctrl_gaincontrol_coring_value;
    uint32_t etc_ring_ctrl_gaincontrol_limit;
    uint32_t etc_ring_ctrl_gaincontrol_bound;
    uint32_t etc_ring_ctrl_gaincontrol_softratio;
    uint32_t etc_sim_prot_gaincontrol_zero;
    uint32_t etc_sim_prot_gaincontrol_base;
    uint32_t etc_sim_prot_gaincontrol_thr;
    uint32_t etc_sim_prot_gaincontrol_softcoring_gain;
    uint32_t etc_sim_prot_gaincontrol_softcoring_value;
    uint32_t etc_sim_prot_gaincontrol_gain;
    uint32_t etc_sim_prot_gaincontrol_coring_value;
    uint32_t etc_sim_prot_gaincontrol_limit;
    uint32_t etc_sim_prot_gaincontrol_bound;
    uint32_t etc_sim_prot_gaincontrol_softratio;
    uint32_t etc_blend;
#endif
};


// Fields collected from Resizer HW
struct RszInput {
    uint32_t srcWidth;
    uint32_t srcHeight;
    uint32_t dstWidth;
    uint32_t dstHeight;
    uint32_t cropOffsetX;
    uint32_t cropSubpixX;
    uint32_t cropOffsetY;
    uint32_t cropSubpixY;
    uint32_t cropWidth;
    uint32_t cropHeight;

    uint32_t enable;         // = 1;
    uint32_t yuv422Tyuv444;  // = 0;
    uint32_t demoEnable;     // = 0;

    // Demo Window
    uint32_t demoOutHStart;
    uint32_t demoOutHEnd;
    uint32_t demoOutVStart;
    uint32_t demoOutVEnd;

    // 8b or 10b mode
    uint32_t powerSavingMode;

    uint32_t ispScenario; //Assign the current ISP scenario from the 12 candidates
    uint32_t is_ispScenario;

    uint32_t DebugFlagsRSZ;

    struct RszRegClearZoom iHWReg;

    uint32_t inISOSpeed;

    uint32_t srcSubpixWidth;
    uint32_t srcSubpixHeight;
    uint32_t cropSubpixWidth;
    uint32_t cropSubpixHeight;
 };

// Fields which will be set to HW registers
struct RszOutput {
    uint32_t  rszEnable;
    uint32_t  horEnable;
    uint32_t  verEnable;
    uint32_t  verticalFirst;
    uint32_t  horAlgo;
    uint32_t  verAlgo;
    uint32_t  horTable;
    uint32_t  verTable;
    uint32_t  precX;
    uint32_t  precY;
    uint32_t coeffStepX;
    uint32_t coeffStepY;
    uint32_t  IBSEEnable;//should not be used
    uint32_t  tapAdaptEnable;

    // demo window
    uint32_t demoInHStart;
    uint32_t demoInHEnd;
    uint32_t demoInVStart;
    uint32_t demoInVEnd;

    // offset
    int32_t horLumaIntOffset;
    int32_t horLumaSubOffset;
    int32_t horChromaIntOffset;
    int32_t horChromaSubOffset;
    int32_t verLumaIntOffset;
    int32_t verLumaSubOffset;
    int32_t verChromaIntOffset;
    int32_t verChromaSubOffset;

    // truncation of coefficients
    uint32_t horTruncBit;
    uint32_t verTruncBit;

    // truncation of coefficients
    uint32_t verCubicTruncEn;
    uint32_t verLumaCubicTruncBit;
    uint32_t verChromaCubicTruncBit;
    uint32_t horCubicTruncEn;
    uint32_t horLumaCubicTruncBit;
    uint32_t horChromaCubicTruncBit;
//#endif

    //Ultra Resolution
    uint32_t IBSEGainMid;
    uint32_t IBSE_clip_thr;
    uint32_t IBSE_clip_ratio;
    uint32_t IBSE_gaincontrol_gain;
    uint32_t TapAdaptSlope;

    struct RszRegClearZoom iHWReg; //ClearZoom (settings to deal with sensor noise)

    //C42, C24 control
    uint32_t c24CropEn;
    uint32_t c42odd2evenMode;
    uint32_t c42LCropEn;
    uint32_t c42InterpEn;

#ifdef CLEAR_ZOOM_3_0
    uint32_t  SignalEnhanceMode;
#endif
};

// Resizer FW registers
struct RszReg {
    uint32_t  tableMode;  // rollback mode or not
    uint32_t  defaultUpTable; // defaultTable for up scaling
    uint32_t  defaultDownTable; // defaultTable for down scaling
    uint32_t  autoTableSelection; // enable auto table selection
    uint32_t  autoAlignment; // enable auto alignment
    uint32_t  autoAlgSelection; // enable auto algorithm selelction

    uint32_t  IBSEEnable; // enable IBSE
    uint32_t  ultraResEnable; // enable Ultra Resolution
    uint32_t  autoCoefTrunc; // bit truncation for (1/2, 1]x scaling ratio

    // algorithm-switch ratios
    uint32_t  switchRatio6Tap6nTap;
    uint32_t  switchRatio6nTapAcc;

    // Ultra Resolution
    int32_t dynIBSE_gain;
    int32_t dynIBSE_gain_ratio_thr0;
    int32_t dynIBSE_gain_ratio_thr1;
    int32_t dynIBSE_gain_ratio_thr2;
    int32_t dynIBSE_gain_clip1;
    int32_t dynIBSE_gain_clip2;
    int32_t dynIBSE_gain_min;
    int32_t dynIBSE_gain_max;

    int32_t tapAdaptSlope;
    int32_t tapAdapt_slope_ratio_thr0;
    int32_t tapAdapt_slope_ratio_thr1;
    int32_t tapAdapt_slope_ratio_thr2;
    int32_t tapAdapt_slope_clip1;
    int32_t tapAdapt_slope_clip2;
    int32_t tapAdapt_slope_min;
    int32_t tapAdapt_slope_max;

    int32_t IBSE_clip_thr;
    int32_t IBSE_clip_thr_ratio_thr0;
    int32_t IBSE_clip_thr_ratio_thr1;
    int32_t IBSE_clip_thr_ratio_thr2;
    int32_t IBSE_clip_thr_clip1;
    int32_t IBSE_clip_thr_clip2;
    int32_t IBSE_clip_thr_min;
    int32_t IBSE_clip_thr_max;

    int32_t IBSE_clip_ratio;
    int32_t IBSE_clip_ratio_ratio_thr0;
    int32_t IBSE_clip_ratio_ratio_thr1;
    int32_t IBSE_clip_ratio_ratio_thr2;
    int32_t IBSE_clip_ratio_clip1;
    int32_t IBSE_clip_ratio_clip2;
    int32_t IBSE_clip_ratio_min;
    int32_t IBSE_clip_ratio_max;

    int32_t IBSE_gaincontrol_gain;
    int32_t IBSE_gaincontrol_gain_ratio_thr0;
    int32_t IBSE_gaincontrol_gain_ratio_thr1;
    int32_t IBSE_gaincontrol_gain_ratio_thr2;
    int32_t IBSE_gaincontrol_gain_clip1;
    int32_t IBSE_gaincontrol_gain_clip2;
    int32_t IBSE_gaincontrol_gain_min;
    int32_t IBSE_gaincontrol_gain_max;

// interface for ISP control
    // ispScenario 12 : refocus
    uint32_t isp_RFUpTable; // defaultTable for refocus
    uint32_t isp_RFDownTable; // defaultTable for refocus
    uint32_t isp_RF_IBSE_gain; // defaultTable for refocus
    uint32_t isp_RF_switchRatio6Tap6nTap;
    uint32_t isp_RF_switchRatio6nTapAcc;
};

class CPQRszFW
{
public:

#ifdef RSZ_ANDROID_PLATFORM
    void onCalculate(const RszInput *input, RszOutput *output);
    void onInitPlatform(const RszInitParam &initParam, RszInitReg *initReg);
#else
    void vDrvRszProc_int(const RszInput *input, RszOutput *output);
#endif

    void initialize(void);
    void onLoadTable(RszInput *input);
    void onLoadTable(RszInput *input, void* NvRamTable);
    void TuningReg2HWReg(RszInput *input);
    void FirstRoundBypass(RszOutput *output);

    int DumpReg(void* RegBuffer, char* DumpBuffer, int& BufferCount,  int MaxBufferSize, int RegNum);

    CPQRszFW()
    {
        memset(this, 0, sizeof(*this));
        #ifdef RSZ_ANDROID_PLATFORM
        initialize();
        #endif

        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
            "RSZInfo",
            "RSZReg"
        };
        for(int i = 0; i < log_tag_len; ++i)
            log_state_list[i] = ANDROID_LOG_DISABLE;
    };

    ~CPQRszFW()
    {
        delete pLogMech;
        delete log_tag;
    };


    RszReg m_rszReg;
    RszReg m_NormalrszReg;

    RszRegClearZoom m_rszRegHW;
    RszRegClearZoom m_NormalrszRegHW;

    // for ClearZoom
    RszReg RszEntrySWReg[UR_MAX_LEVEL];
    RszRegClearZoom RszEntryHWReg[UR_MAX_LEVEL];

    uint32_t RszLevel[ISP_SCENARIOS];

private:
    /****** Resizer processes, variables ******/
    void rsz_alg_det(const RszInput *reg_in, RszOutput *reg_out);
    void rsz_config(const RszInput *reg_in, RszOutput *reg_out, int Alg, int isHor);
    int rsz_tbl_select(const RszInput *reg_in, int Alg, int stepSize);
    void rsz_config_demoWindow(const RszInput *input, RszOutput *output);
    void rsz_config_ctrlRegs(const RszInput *reg_in, RszOutput *reg_out);
    void rsz_auto_align(const RszInput *reg_in, RszOutput *reg_out, int Alg, int isHor);
    void rsz_ultraRes(const RszInput *reg_in, RszOutput *reg_out);
    void rsz_ultraRes_disabled(RszOutput *reg_out);
    int32_t UltraResGetRegWOInit(int32_t in_ratio, int32_t reg_ratio_thr0, int32_t reg_ratio_thr1, int32_t reg_ratio_thr2, int32_t reg_clip1, int32_t reg_clip2, int32_t reg_min, int32_t reg_max);
    int32_t UltraResGetRegWInit(int32_t in_value, int32_t in_ratio, int32_t reg_ratio_thr0, int32_t reg_ratio_thr1, int32_t reg_ratio_thr2, int32_t reg_clip1, int32_t reg_clip2, int32_t reg_min, int32_t reg_max);
    int32_t AlphaBlending(int32_t data1, int32_t data2, int32_t alpha, int32_t bits);
    void rsz_auto_coefTrunc(const RszInput *reg_in, RszOutput *reg_out);
    void rsz_offset_check(const RszInput *reg_in, RszOutput *reg_out);
    void rsz_hwReg_in2out(const RszInput *reg_in, RszOutput *reg_out);
    void print_RszSWReg();
    void print_RszHWReg();
    //void rsz_coef_trunc_6nTap(void);

    // Debug logger
    void logMechanism(const RszInput*);
    PQLoggingMechanism *pLogMech;
    enum log_tag_id
    {
        RSZInfo = 0,
        RSZReg,
        log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];

#if 0
    RszInput oldInput;
    RszOutput oldOutput;
#endif
};


#endif  // __PQRSZIMPL_H__
