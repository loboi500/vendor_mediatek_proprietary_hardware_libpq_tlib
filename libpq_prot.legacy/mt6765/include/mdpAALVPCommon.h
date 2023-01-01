#ifndef MDPAALVPCOMMON_H
#define MDPAALVPCOMMON_H

//#define DRE_CHIP_6885
//#define DRE_ANDROID_PLATFORM


typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef signed int int32_t;

/* 32-bit DebugFlags layout:
[31:26]: Not used
[25:22]: roi_x
[21:19]: roi_y
[18:14]: roi_w
[13:10]: roi_h
[9:0]: used by MdpAALVPDebugFlags
*/


#define MDPAALVP_DEBUG_FLAG_MAX_BIT 32
#define MDPAALVP_DEBUG_FLAG_ROI_X_BIT_START 25
#define MDPAALVP_DEBUG_FLAG_ROI_X_BIT_END 22
#define MDPAALVP_DEBUG_FLAG_ROI_Y_BIT_START 21
#define MDPAALVP_DEBUG_FLAG_ROI_Y_BIT_END 19
#define MDPAALVP_DEBUG_FLAG_ROI_W_BIT_START 18
#define MDPAALVP_DEBUG_FLAG_ROI_W_BIT_END 14
#define MDPAALVP_DEBUG_FLAG_ROI_H_BIT_START 13
#define MDPAALVP_DEBUG_FLAG_ROI_H_BIT_END 10

enum MdpAALVPDebugFlags {
    eMdpAALVPDebugDisabled          = 0x0,
    eMdpAALVPDebugInput             = 0x1,
    eMdpAALVPDebugOutput            = 0x2,
    eMdpAALVPDebugTime              = 0x4,
    eMdpAALVPDebugContent           = 0x8,
    eMdpAALVPDebugDRE               = 0x40,
    eMdpAALVPDebugFilter            = 0x80,
    eMdpAALVPDebugDualPipe          = 0x100,
    eMdpAALVPDebugBasic             = 0x200,
    eMdpAALVPDebugAll               = 0x3FF
};

enum MdpAALVPTBLOffset {
    LTM_HDR_BASE            = 0,    //0~4
    LTM_HDR_PLUS_BASE       = 5,    //5~9
    LTM_VP_BASE             = 10,    //10~19
    LTM_AI_SDR2HDR          = 20,   //20~29
    LTM_GAME_HDR            = 30,   //30~35
    LTM_2ND_DISPLAY_LITEPQ  = 36,   //36
    LTM_2ND_DISPLAY_MAINPQ  = 37,   //37
    LTM_HDR_VIVID_BASE      = 38    //38~43

};

#define SCLTM_VERSION_3_0   0
#define SCLTM_VERSION_3_1   1
#define SCLTM_VERSION_3_5   2
#define Early_Term

#if defined(PQ_AAL_VERSION_0)
  #define DRE_ANDROID_PLATFORM
  #define SCLTM_VERSION_6885
#endif

#ifdef DRE_ANDROID_PLATFORM
  #if defined(SCLTM_VERSION_6771)
    #define SCLTM_VERSION SCLTM_VERSION_3_0 //P60
  #elif defined(SCLTM_VERSION_6779)
    #define SCLTM_VERSION SCLTM_VERSION_3_1 //P90
  #elif defined(SCLTM_VERSION_6885)
    #define SCLTM_VERSION SCLTM_VERSION_3_5 //5G-L
  #endif
#else
#define SCLTM_VERSION SCLTM_VERSION_3_5
#endif

// AAL internal variables for log use
struct DREVPInternalReg {
    int isGlobal;
    int blk_y_idx;
    int blk_x_idx;
    int SpikeWgt[16][16];
    int SpikeValue[16][16]; //ww
    int CompactValue[16][16];
    int HistSum[16][16]; //ww
    int HistSumFallBackWgt[16][16]; //ww
    int SkinWgt[16][16];
    int HECurveSet[16][16][17];
    int SpikeCurveSet[16][16][17];
    int CompactCurveSet[16][16][17];
    int SkinCurveSet[16][16][17];
    int LLPCurveSet[16][16][17];
    int APLCompCurveSet[16][16][17];
    int FinalCurveSet[16][16][17];
#if (SCLTM_VERSION == SCLTM_VERSION_3_1)
    int CompactWgt[16][16];
    int GreenWgt[16][16];
#elif (SCLTM_VERSION == SCLTM_VERSION_3_5)
    int CompactWgt[16][16];
    int GreenWgt[16][16];
#endif
    int FlatWgt[16][16];
    bool EarlyTerm[16][16];

};

#pragma pack(1)
// AAL FW registers
struct DREVPReg {
    int dre_fw_en;
    int dre_curve_en;
    int dre_gain_flt_en;
    int bADLWeight1;
    int bADLWeight2;
    int bADLWeight3;
    int bBSDCGain;
    int bBSACGain;
    int bBSLevel;
    int bMIDDCGain;
    int bMIDACGain;
    int bWSDCGain;
    int bWSACGain;
    int bWSLevel;
    int dre_dync_spike_wgt_min;
    int dre_dync_spike_wgt_max;
    int dre_dync_spike_th;
    int dre_dync_spike_slope;
    int bSpikeBlendmethod;
    int bSkinWgtSlope;
    int bSkinBlendmethod;
    int SkinDummy1;
    int Layer0_Ratio;
    int Layer1_Ratio;
    int Layer2_Ratio;
    int Dark_Ratio;
    int Bright_Ratio;
    int dre_dync_flt_coef_min;
    int dre_dync_flt_coef_max;
    int dre_dync_flt_ovr_pxl_th;
    int dre_dync_flt_ovr_pxl_slope;
    int dre_iir_force_range;
    int LLPValue;
    int LLPRatio;
    int APLCompRatioLow;
    int APLCompRatioHigh;
    int FltConfSlope;
    int FltConfTh;
    int BlkHistCountRatio;
    int BinIdxDiffSlope;
    int BinIdxDiffTh;
    int BinIdxDiffWgtOft;
    int APLTh;
    int APLSlope;
    int APLWgtOft;
    int APL2Th;
    int APL2Slope;
    int APL2WgtOft;
    int APL2WgtMax;
    int BlkSpaFltEn;
    int BlkSpaFltType;
    int LoadBlkCurveEn;
    int SaveBlkCurveEn;
    int Flat_Length_Th_Base;
    int Flat_Length_Slope_Base;
    int DZ_Fallback_En;
    int DZ_Size_Th;
    int DZ_Size_Slope;

#if ((SCLTM_VERSION == SCLTM_VERSION_3_1) || (SCLTM_VERSION == SCLTM_VERSION_3_5))
    int bGreenWgtSlope;
    int dre_dync_compact_wgt_min;
    int dre_dync_compact_wgt_max;
    int dre_dync_compact_th;
    int dre_dync_compact_slope;
    int bCompactBlendmethod;
    int dre_dync_compact_LowBoundBin;
    int dre_dync_compact_HighBoundBin;
    int dre_dync_compact_BinMinWeight;
    int dre_dync_compact_BinMaxWeight;
    int dre_dync_flt_coef_increase;
    int flatblknum_ratio;
    int bColorWgtSlope;
    int bColorBlendmethod;
    int ColorDummy1;
#endif

    int initialPauseFrame;
    int initialTransitionFrame;
    int earlyTermSpikeWgtTh;
    int earlyTermSkinWgtTh;
    int earlyTermFlatWgtTh;
    int deflickerEn;
    int flickerColorRatioTh;
    int flickerDetectTh;
    int flickerAmplitudeTh;
    int deflickerFallbackRatioTh;
    int deflickerFallbackSlope;
    int deflickerFallbackRatioMin;
    int deflickerFallbackRatioMax;
    int flickerDarkZoneDefine[17];
    int fixedCurveEn;
    int fixedCurveRoIX;
    int fixedCurveRoIY;
    int fixedCurveRoIW;
    int fixedCurveRoIH;
    int fixedCurve[17];
    int sceneBasedUpdateEn;
    int sceneChangeThLow;
    int sceneChangeThHigh;
    int sceneChangeLocalCountThShrink;
    int sceneChangeLocalCountMax;
    int sceneChangeLocalCountMin;
    int sceneChangeLocalThLow;
    int sceneChangeLocalThHigh;
    // reg count here: 134
    int reservedArray[266];
};
struct DREVPCustXMLBuf
{
  unsigned int Size;//Byte
  DREVPReg iDRERegEntry;
  unsigned int CheckCode;
  unsigned int checksum;
};
#pragma pack()

// AAL FW registers
struct DREVPReg_RO
{
    int g_hist_set01;
    int g_hist_set02;
    int g_hist_set03;
    int g_hist_set04;
    int g_hist_set05;
    int g_hist_set06;
    int g_hist_set07;
    int g_hist_set08;
    int g_hist_set09;
    int g_hist_set10;
    int g_hist_set11;
    int g_hist_set12;
    int g_hist_set13;
    int g_hist_set14;
    int g_hist_set15;
    int g_hist_set16;
    int g_hist_set17;
    int g_curve_set01;
    int g_curve_set02;
    int g_curve_set03;
    int g_curve_set04;
    int g_curve_set05;
    int g_curve_set06;
    int g_curve_set07;
    int g_curve_set08;
    int g_curve_set09;
    int g_curve_set10;
    int g_curve_set11;
    int g_curve_set12;
    int g_curve_set13;
    int g_curve_set14;
    int g_curve_set15;
    int g_curve_set16;
    int g_curve_set17;
};

#endif
