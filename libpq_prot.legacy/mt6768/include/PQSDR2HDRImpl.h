#ifndef __SDR2HDR_FW__
#define __SDR2HDR_FW__

#ifdef __cplusplus
extern "C++"
{
#endif

#include "PQHDRImpl.h"

#define SDR2HDR_TBL_SIZE 7
#define SDR2HDR_LOGD(fp, fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "PQ_HDR", "%s: " fmt, __FUNCTION__, ##__VA_ARGS__ )
#define LINEAR_MAPPING
#define IMG_SIZE               65536    //image size = 256x256 =65536
#define QUANTIZE_STD_VALUE     0.006171903107315302 // V1: 8bits
//#define QUANTIZE_STD_VALUE 0.00004803110641660169   // V2: 16bits 0331
//#define QUANTIZE_STD_VALUE 0.000050585949793457985  // V3: 16bits 0401
#define QUANTIZE_MEAN         0
#define NNSlopeNum            16

enum SDR2HDRDebugFlags {
    eSDR2HDRDebugDisabled = 0x0,
    eSDR2HDRDebugEntryFlag = 0x1,
    eSDR2HDRDebugInputDSDR2HDRINPUT_SDR2HDRFWReg = 0x2,
    eSDR2HDRDebugInputFirewareMemberVariable = 0x4,
    eSDR2HDRDebugOutputDSDR2HDROUTPUT = 0x8,
    eSDR2HDRDebugOutputGainCurve = 0x10,
    eSDR2HDRDebugTime = 0x20,
    eSDR2HDRDebugContent_Frame = 0x40,
    eSDR2HDRDebugContent_Stream = 0x80,
    eSDR2HDRDebugAll = 0xFF
};

typedef struct DynLumInfo
{
    UINT32 RGBmaxHistogram_1_ori[57];         // Histogram.
    UINT32 Hist_nits[57];                 // Brightness.
    UINT32 Hist_pdf[57];                  // Histogram probability density distribution.
    UINT32 Hist_cdf[57];                  // Histogram Cumulative distribution.
    UINT32 Hist_avg_nits[57];             // Content Average nits.
    UINT32 hist_70_100_percent_nits[33];  // brightness at specific cumulative distributions;
                                          //[0] = 70%, [1] = 71%….[20]= 90%,[21]=91…[29]=99%,[30]=99.5,
                                          //[31]=99.95, [32]=99.99, brightness at specific cumulative distributions.
    INT32  aihdr_tgt_nits;                // Target nits for AI-HDR at T0 -1 frame.
    INT32  cust_tgt_nits;                 // Target nits for custom tone mapping curve at T0 -1 frame.
    INT32  tgt_nits;                      // Target nits at T0 -1 frame.
    INT32  tgt_nits_1;                    // Target nits  at T0 -2 frame.
} DynLumInfo;

struct PQSelectorInfo {
    UINT32 AALSceneProbability[5];
    INT32 wholefbwidth;
    INT32 wholefbheight;
    INT32 cwidth;
    INT32 cheight;
    INT32 QPsum;
    INT32 specialOutputColorFormat;
};

typedef struct Aslope{
    UINT32  alpha_initial;
    UINT32  aihdr_initial;
} Aslope;

typedef struct AIHDR_INPUT{
    INT32  tgt_nits;
} AIHDR_INPUT;


typedef struct AIHDR_OUTPUT{
    //float  Slope[16];             // 0 - 255, final slope = (slope - QUANTIZE_MEAN) x QUANTIZE_STD_VALUE
    //unsigned short Slope[16];
    unsigned char Slope[16];
} AIHDR_OUTPUT;

typedef struct AISCENE_OUTPUT{
       UINT32 Scene_Probability[15];
}AISCENE_OUTPUT;

typedef struct AICCORR_OUTPUT{

    UINT32 CT_Info[5];     // Array[Kr, Kg, Kb, x, y]

}AICCORR_OUTPUT;

typedef struct AIUI_OUTPUT{

    UINT32 UI_Probability;

}AIUI_OUTPUT;

typedef struct PQSelectorInput
{
     enum PQ_APPLICATIOM_E pqApplication;
   struct   PQSelectorInfo pqSelectorInfo;
   struct       DynLumInfo LumParam;
   struct   AISCENE_OUTPUT outAISCENE;   //from AI-Scene Detector output
   struct      AIUI_OUTPUT outAIUI;    //from AI-UI output
      int    DREChromaHist[8][16]; //HW calculated Skin histogram for SCLTM;
};

// =================== SDR2HDR SW cust registers ================== //
typedef struct custSDR2HDRFWReg
{
    UINT32 panel_nits;
    UINT32 dynamic_rendering;
    UINT32 static_mode_gain_curve_idx;
    UINT32 tgt_hist_idx_array[10];
    UINT32 sw_p[17];
    UINT32 sw_slope[16];
    UINT32 dynamic_mode_fix_gain_curve_en;
    UINT32 dynamic_mode_fixed_gain_curve_idx;

#pragma region Add_for_AI-SDR2HDR
    UINT32 tone_mapping_fw_refine_test_en;
    UINT32 aihdr_tgt_nit_LB;    //lower Bound at x-axis
    UINT32 aihdr_tgt_nit_UB;    //Upper Bound at x-axis
    UINT32 aihdr_tgt_nit_MaxMappingValue;   //Max y value
    UINT32 aihdr_tgt_nit_MinMappingValue;   //Min y value

    UINT32 cust_tgt_nit_LB;    //lower Bound at x-axis
    UINT32 cust_tgt_nit_UB;    //Upper Bound at x-axis
    UINT32 cust_tgt_nit_MaxMappingValue;   //Max y value
    UINT32 cust_tgt_nit_MinMappingValue;   //Min y value

    UINT32 tgt_nit_blending_weight_LB;    //lower Bound at x-axis
    UINT32 tgt_nit_blending_weight_UB;    //Upper Bound at x-axis
    UINT32 tgt_nit_blending_weight_MaxMappingValue;   //Max y value
    UINT32 tgt_nit_blending_weight_MinMappingValue;   //Min y value

    UINT32 TMcurve_BlendingWeight_LB;    //lower Bound at x-axis
    UINT32 TMcurve_BlendingWeight_UB;    //Upper Bound at x-axis
    UINT32 TMcurve_BlendingWeight_MaxMappingValue;   //Max y value
    UINT32 TMcurve_BlendingWeight_MinMappingValue;   //Min y value

    UINT32 tone_mapping_curve_type;  // 0:blending curve between AI-HDR and cust TM curve
                                     // 1:only AI-HDR curve, 2:only custom TM curve

    UINT32 fixed_tgt_nits_BlendingWeight;
    UINT32 fixed_TMcurve_BlendingWeight;

    UINT32 min_aihdr_tgt_nits;
    UINT32 middle_aihdr_tgt_nits;
    UINT32 max_aihdr_tgt_nits;
    UINT32 clip_p0;
    UINT32 clip_p1;
    UINT32 clip_p2;

    UINT32 fallback_ratio_Portrait;
    UINT32 fallback_ratio_Sky;
    UINT32 fallback_ratio_Green;
    UINT32 fallback_ratio_Food;
    UINT32 fallback_ratio_Architecture;

#pragma endregion Add_for_AI-SDR2HDR

}custSDR2HDRFWReg;

/* ........begin Dynamic HDR Rendering, local parameters......... */
typedef struct SDR2HDRFWMemberVariable
{
    INT32 gain_curve_idx_1;
    INT32 aihdr_tgt_nits, aihdr_tgt_nits_1, cust_tgt_nits, cust_tgt_nits_1, tgt_nits, tgt_nits_1, tgt_nits_2;
    INT32 fw_tgt_nits, fw_tgt_nits_1;
    INT32 panel_sdr_nits, panel_sdr_nits_1;

    INT32 sdr_avg_70_100_percent_nits_buf0[fw_stat_bins];
    INT32 sdr_avg_70_100_percent_nits_buf1[fw_stat_bins];
    INT32 hist_70_100_percent_nits_buf0[fw_stat_bins];
    INT32 hist_70_100_percent_nits_buf1[fw_stat_bins];
    INT32 *sdr_avg_70_100_percent_nits, *sdr_avg_70_100_percent_nits_1;
    INT32 *hist_70_100_percent_nits, *hist_70_100_percent_nits_1;

    INT32 Hist_nits[hist_bins];
    INT32 Hist_pdf[hist_bins], Hist_cdf[hist_bins];
    INT32 Hist_avg_nits[hist_bins], Hist_sdr_avg_nits[hist_bins];
    INT32 *sdr_avg_70_100_percent_nits_tmp, *hist_70_100_percent_nits_tmp;

    INT32 width, height;
    INT32 panel_nits;
    INT32 hw_src_nits, hw_src_nits_1, hw_src_nits_2;
    INT32 scene_change_flag, scene_change_flag_1;

    UINT32 maxFCLL, maxFCLL_1, reference_maxCLL;
    UINT32 idx, maxFCLL_Queue[32];
    UINT32 iCurrFrmNum_Fw;

    UINT32 GainCurve[Gain_Curve_POINTS]; //local buffer
    INT32 reg_p[Tone_Map_POINTS + 1];
    INT32 reg_slope[Tone_Map_POINTS]; //local buffer
    INT32 cust_reg_p[Tone_Map_POINTS + 1];
    INT32 cust_reg_slope[Tone_Map_POINTS]; //local buffer
    INT32 cust_reg_slope_FB[Tone_Map_POINTS]; //local buffer
    INT32 aihdr_reg_p[Tone_Map_POINTS + 1];
    INT32 aihdr_reg_slope[Tone_Map_POINTS]; //local buffer
    INT32 aihdr_reg_slope_FB[Tone_Map_POINTS]; //feedback aihdr_reg_slope
    float NNScene_Probability[15];

    INT32 low_flicker_mode_case_flag; // 0: initial, 1: scene change, 2: fade-in/out , 3: different scene, 4: successive frames CASE 1, 5: successive frames CASE 2
    char out_buffer[600];
    char tmp_buffer[256];
    char out_buffer2[600];

    APPLICATION_E application;

    UINT32 tgt_nits_BlendingWeight;           //tgt_nit blending Weight
    UINT32 TMcurve_BlendingWeight; //TM Curve blending Weight

    UINT32 static_calculate_flag;

    UINT32 aihdr_tgt_mapping_range_type;
    UINT32 aihdr_tgt_mapping_range_type_1;

    float fallback_ratio_NNSceneCGPatternSet, fallback_ratio_NNSceneCGPatternSet_1;
    UINT32 fallback_frame_cnt;

    int sum_diff_low_region;
    int sum_diff_high_region;
    int diff_sum_diff;
    int add_sum_diff;

    int fixed_aihdr_curve_flag;
    int fixed_aihdr_curve_flag_1;
    int fixed_aihdr_transtion_frame_num;

    UINT32 reInitFlag; // non-zero after onStaticCalculate API, use to tell onDynamicCalculate smoothing change between curve_blending

    UINT32 min_aihdr_tgt_nits_FB;
    UINT32 middle_aihdr_tgt_nits_FB;
    UINT32 max_aihdr_tgt_nits_FB;
    UINT32 clip_p0_FB;
    UINT32 clip_p1_FB;
    UINT32 clip_p2_FB;

    UINT32 flag_tool_update;

    UINT32 RGBmaxHistogram_2[hist_bins];

    UINT32 sameSceneDelayDetect;

    float sum_diff_aihdr_tgt_nits;

} SDR2HDRFWMemberVariable;


typedef struct SDR2HDRFWReg
{
    UINT32 DebugFlags;

    UINT32 dynamic_rendering; //SW register, 1b

    UINT32 static_mode_gain_curve_idx; // 4b, 0~8

    UINT32 min_histogram_window;

    UINT32 gain_curve_boost_speed; //8b, 0~255

    UINT32 hlg_gain_curve_idx; // 4b, 0~9

    UINT32 tgt_hist_idx_array[10]; //0~32, 6b.  {20, 22, 24, 24, 25, 26, 27, 28, 29, 29}; //SW register

    UINT32 sw_p[17]; // 16+3 = 19b sw_p[Tone_Map_POINTS+1]
    UINT32 sw_slope[16]; //12+4 = 16b sw_slope[Tone_Map_POINTS]

    UINT32 high_bright; //32b, 0~400000, SW register

    UINT32 dark_scene_slope1, dark_scene_slope2, dark_scene_darken_rate; // 9b, 0~256
    UINT32 dark_scene_p1, dark_scene_p2; // 32b, 0~400000

    UINT32 normal_scene_slope1, normal_scene_slope2, normal_scene_darken_rate; // 9b, 0~256
    UINT32 normal_scene_p1, normal_scene_p2; // 32b, 0~400000

    UINT32 bright_scene_slope1, bright_scene_slope2; // 9b, 0~256
    UINT32 bright_scene_p1, bright_scene_p2; // 32b, 0~400000

    UINT32 non_bright_scene_slope, non_bright_scene_lighten_rate; // 9b, 0~256
    UINT32 non_bright_scene_p1, non_bright_scene_p2; // 32b, 0~400000


    UINT32 panel_nits_change_rate; //SW register, 9b, 0.03 = 15/512
    UINT32 tgt_nits_change_step; //SW register, 4b, 0~15
    UINT32 fade_hist_change_rate; //SW register, 9b, 0.06 = 31/512
    UINT32 fade_tgt_nits_change_rate; //SW register, 9b, 0.1 = 51/512
    UINT32 tgt_nits_assign_factor; //SW register, 5b, 0~16
    UINT32 fade_tgt_nits_assign_factor; //SW register, 5b, 0~16

    UINT32 tgt_nits_upper_limit; // SW register, 12b, 400~4095

    UINT32 nr_strength_b; //4b, NR strength
    UINT32 mode_weight; //4b, maxRGB and Y bleng

    UINT32 test_nits;

    struct __VDEC_DRV_COLORDESC_T HDR2SDR_STMDInfo;
    struct PANEL_SPEC panel_spec;

    UINT32 tone_mapping_truncate_mode;
    UINT32 min_p_end;

    //3x3 matrix (gamut) 2'complement
    UINT32 c00;
    UINT32 c01;
    UINT32 c02;
    UINT32 c10;
    UINT32 c11;
    UINT32 c12;
    UINT32 c20;
    UINT32 c21;
    UINT32 c22;
    UINT32 gamut_matrix_en;

    UINT32 dynamic_mode_fix_gain_curve_en;
    UINT32 dynamic_mode_fixed_gain_curve_idx;

    UINT32 sw_eotf_table; //0: st2084, 1:HLG, 2:SDR, 3: 709, others: random for RTL
    UINT32 sw_oetf_table; //0: 709, 1: 2.4, others: random for RTl

    UINT32 hlg_no_system_gamma; // 1b

    UINT32 BT709_c00;   // for BT.709 to panel gamut > sRGB
    UINT32 BT709_c01;
    UINT32 BT709_c02;
    UINT32 BT709_c10;
    UINT32 BT709_c11;
    UINT32 BT709_c12;
    UINT32 BT709_c20;
    UINT32 BT709_c21;
    UINT32 BT709_c22;
    UINT32 BT2020_c00;  // for BT.2020 to panel gamut > sRGB
    UINT32 BT2020_c01;
    UINT32 BT2020_c02;
    UINT32 BT2020_c10;
    UINT32 BT2020_c11;
    UINT32 BT2020_c12;
    UINT32 BT2020_c20;
    UINT32 BT2020_c21;
    UINT32 BT2020_c22;

    UINT32 low_flicker_mode_en;
    UINT32 low_flicker_mode_scene_change_nits_diff;
    UINT32 low_flicker_mode_different_scene_light_decrease;
    UINT32 low_flicker_mode_different_scene_light_increase;
    UINT32 low_flicker_mode_fade_decrease;
    UINT32 low_flicker_mode_fade_increase;
    UINT32 low_flicker_mode_same_scene_chase_gap;
    UINT32 low_flicker_mode_same_scene_chase_converge_period;
    UINT32 low_flicker_mode_same_scene_chase_max_speed;
    UINT32 low_flicker_mode_different_scene_nits_diff;

    UINT32 xpercent_histogram_tuning_en;
    UINT32 max_hist_70_100_percent_nits;
    UINT32 approach_saturate_region;

    UINT32 sce_HDR2SDR; //EPCOT only, //0: PQ2SDR1886, 1: PQ2SDR709, 2: PQ to BT2020 HLG (new request 3), 3:709 SDR to BT2020 HLG (new request 0) 4:BT2020 SDR to BT20202 HLG (new request 1) 5: PQ to BT2020 SDR (new request 2) 6: HLG_to_HLG (new request 4)

    UINT32 aihdr_target_nits; //read only register for FW
    UINT32 cust_target_nits;  //read only register for FW
    UINT32 target_nits;       //read only register for FW

    UINT32 tone_mapping_fw_refine_test_en; //tone_mapping_fw_refine
    UINT32 source_VUI_debug_en;

    UINT32 eotf_accuracy_control_en;

    UINT32 protect_rate_256;
    UINT32 protect_rate_512;
    UINT32 protect_rate_1024;
    UINT32 protect_rate_2048;
    UINT32 maxFCLL_Queue_size;

    UINT32 tone_mapping_fw_refine_slope_n; //tone_mapping_fw_refine

    UINT32 curve_blending_low_ratio;
    UINT32 curve_blending_high_ratio;

    UINT32 sdr_nits_factor_4000; //0~64, husky only
    UINT32 sdr_nits_factor_2048;
    UINT32 sdr_nits_factor_1024;
    UINT32 sdr_nits_factor_512;

    UINT32 aihdr_tgt_nit_LB;    //lower Bound at x-axis
    UINT32 aihdr_tgt_nit_UB;    //Upper Bound at x-axis
    UINT32 aihdr_tgt_nit_MaxMappingValue;   //Max y value
    UINT32 aihdr_tgt_nit_MinMappingValue;   //Min y value

    UINT32 cust_tgt_nit_LB;    //lower Bound at x-axis
    UINT32 cust_tgt_nit_UB;    //Upper Bound at x-axis
    UINT32 cust_tgt_nit_MaxMappingValue;   //Max y value
    UINT32 cust_tgt_nit_MinMappingValue;   //Min y value

    UINT32 tgt_nit_blending_weight_LB;    //lower Bound at x-axis
    UINT32 tgt_nit_blending_weight_UB;    //Upper Bound at x-axis
    UINT32 tgt_nit_blending_weight_MaxMappingValue;   //Max y value
    UINT32 tgt_nit_blending_weight_MinMappingValue;   //Min y value

    UINT32 TMcurve_BlendingWeight_LB;    //lower Bound at x-axis
    UINT32 TMcurve_BlendingWeight_UB;    //Upper Bound at x-axis
    UINT32 TMcurve_BlendingWeight_MaxMappingValue;   //Max y value
    UINT32 TMcurve_BlendingWeight_MinMappingValue;   //Min y value

    UINT32 tone_mapping_curve_type;  // 0:blending curve between AI-HDR and cust TM curve
                                     // 1:only AI-HDR curve, 2:only custom TM curve
    UINT32 aihdr_dynamic_rendering;
    UINT32 cust_dynamic_rendering;
    UINT32 tgt_nits_dynamic_rendering;
    UINT32 TMcurve_dynamic_rendering;

    UINT32 fixed_tgt_nits_BlendingWeight;
    UINT32 fixed_TMcurve_BlendingWeight;

    UINT32 tgt_nits;
    UINT32 cust_tgt_nits;
    UINT32 min_aihdr_tgt_nits;
    UINT32 middle_aihdr_tgt_nits;
    UINT32 max_aihdr_tgt_nits;
    UINT32 clip_p0;
    UINT32 clip_p1;
    UINT32 clip_p2;

    UINT32 RGBmaxHistogram_1[hist_bins];

    UINT32 fallback_ratio_Portrait;
    UINT32 fallback_ratio_Sky;
    UINT32 fallback_ratio_Green;
    UINT32 fallback_ratio_Food;
    UINT32 fallback_ratio_Architecture;

}SDR2HDRFWReg;


// =========== feedback parameters per frame =========== //
typedef struct SDR2HDRFWFeedbackMemberVariable
{
    INT32 gain_curve_idx_1;
    INT32 tgt_nits, tgt_nits_1, tgt_nits_2;
    INT32 fw_tgt_nits, fw_tgt_nits_1;
    INT32 panel_sdr_nits, panel_sdr_nits_1;

    INT32 sdr_avg_70_100_percent_nits[fw_stat_bins];
    INT32 sdr_avg_70_100_percent_nits_1[fw_stat_bins];
    INT32 hist_70_100_percent_nits[fw_stat_bins];
    INT32 hist_70_100_percent_nits_1[fw_stat_bins];

    INT32 hw_src_nits, hw_src_nits_1, hw_src_nits_2;
    INT32 scene_change_flag, scene_change_flag_1;

    UINT32 maxFCLL, maxFCLL_1, reference_maxCLL;
    UINT32 idx;
    UINT32 iCurrFrmNum_Fw;
#if 0
    INT32 reg_p[Feedback_Frame_Cnt][Tone_Map_POINTS + 1];
    INT32 reg_slope[Feedback_Frame_Cnt][Tone_Map_POINTS]; // local buffer

    INT32 aihdr_reg_p[Feedback_Frame_Cnt][Tone_Map_POINTS + 1];
    INT32 aihdr_reg_slope[Feedback_Frame_Cnt][Tone_Map_POINTS]; // local buffer

    INT32 cust_reg_p[Feedback_Frame_Cnt][Tone_Map_POINTS + 1];
    INT32 cust_reg_slope[Feedback_Frame_Cnt][Tone_Map_POINTS]; // local buffer
#else
    INT32 reg_p[Tone_Map_POINTS + 1];
    INT32 reg_slope[Tone_Map_POINTS]; // local buffer

    INT32 aihdr_reg_p[Tone_Map_POINTS + 1];
    INT32 aihdr_reg_slope[Tone_Map_POINTS]; // local buffer
    INT32 aihdr_tgt_nits_1;

    INT32 cust_reg_p[Tone_Map_POINTS + 1];
    INT32 cust_reg_slope[Tone_Map_POINTS]; // local buffer
    INT32 cust_tgt_nits_1;
    UINT32 aihdr_tgt_mapping_range_type_1;

    UINT32 fallback_ratio_NNSceneCGPatternSet_1;

    int fixed_aihdr_curve_flag_1;

    UINT32 min_aihdr_tgt_nits_FB;
    UINT32 middle_aihdr_tgt_nits_FB;
    UINT32 max_aihdr_tgt_nits_FB;
    UINT32 clip_p0_FB;
    UINT32 clip_p1_FB;
    UINT32 clip_p2_FB;
#endif

    UINT32 reInitFlag; // non-zero after onStaticCalculate API, use to tell onDynamicCalculate smoothing change between curve_blending

    UINT32 RGBmaxHistogram_2[hist_bins];

} SDR2HDRFWFeedbackMemberVariable;

//input information for each frame, HW registers & histogram, streaming resolution change
typedef struct SDR2HDRINPUT
{
    struct iHDRHWReg iHWReg;
    UINT32 cwidth, cheight, wholefbwidth, wholefbheight;
    UINT32 resolution_change; // for streaming resolution change
    UINT32 RGBmaxHistogram_Left[hist_bins];
    UINT32 RGBmaxHistogram_Right[hist_bins];
    bool dual_pipe_en; // flag for ALG to call onMergeHistogram API
    UINT32 RGBmaxHistogram_1[hist_bins];
    struct __VDEC_DRV_COLORDESC_T HDR2SDR_STMDInfo;
    struct PANEL_SPEC panel_spec;
    UINT32 hist_begin_x;
    UINT32 hist_end_x;
    UINT32 hist_begin_y;
    UINT32 hist_end_y;
    APPLICATION_E application;
    struct SDR2HDRFWFeedbackMemberVariable SDR2HDRFW_FBMV;

    struct PQSelectorInfo pqSelectorInfo;
    AICCORR_OUTPUT outAICCORR; //from AI-CCORR Output
    AISCENE_OUTPUT outAISCENE; //from AI-Scene Detector output
    AIHDR_OUTPUT   outAIHDR;   //from AI-HDR output
    AIUI_OUTPUT    outAIUI;    //from AI-UI output

}SDR2HDRHDRINPUT;

//output information for each frame HW setting
typedef struct SDR2HDROUTPUT
{
    UINT32 GainCurve[Gain_Curve_POINTS];

    UINT32 reg_p1;
    UINT32 reg_p2;
    UINT32 reg_p3;
    UINT32 reg_p4;
    UINT32 reg_p5;
    UINT32 reg_p6;
    UINT32 reg_p7;
    UINT32 reg_p8;
    UINT32 reg_p9;
    UINT32 reg_p10;
    UINT32 reg_p11;
    UINT32 reg_p12;
    UINT32 reg_p13;
    UINT32 reg_p14;
    UINT32 reg_p15;
    UINT32 reg_p16;

    UINT32 reg_slope0;
    UINT32 reg_slope1;
    UINT32 reg_slope2;
    UINT32 reg_slope3;
    UINT32 reg_slope4;
    UINT32 reg_slope5;
    UINT32 reg_slope6;
    UINT32 reg_slope7;
    UINT32 reg_slope8;
    UINT32 reg_slope9;
    UINT32 reg_slope10;
    UINT32 reg_slope11;
    UINT32 reg_slope12;
    UINT32 reg_slope13;
    UINT32 reg_slope14;
    UINT32 reg_slope15;


    UINT32 hist_begin_y;
    UINT32 hist_end_y;
    UINT32 hist_begin_x;
    UINT32 hist_end_x;

    UINT32 in_hsize;
    UINT32 in_vsize;

    //YUV2RGB
#ifdef Y2R_INPUT_FORMAT_SELECT
    UINT32 y2r_en;
    UINT32 y2r_input_format;

    UINT32 y2r_c00;
    UINT32 y2r_c01;
    UINT32 y2r_c02;
    UINT32 y2r_c10;
    UINT32 y2r_c11;
    UINT32 y2r_c12;
    UINT32 y2r_c20;
    UINT32 y2r_c21;
    UINT32 y2r_c22;
    UINT32 y2r_pre_add_0;
    UINT32 y2r_pre_add_1;
    UINT32 y2r_pre_add_2;
    UINT32 y2r_post_add_0;
    UINT32 y2r_post_add_1;
    UINT32 y2r_post_add_2;
#else
    UINT32 bt2020_in;
    UINT32 bt2020_const_luma;
#endif
    //ST2084 to linear
    UINT32 input_gamma_en;

    //3x3 matrix (gamut) 2'complement
    UINT32 c00;
    UINT32 c01;
    UINT32 c02;
    UINT32 c10;
    UINT32 c11;
    UINT32 c12;
    UINT32 c20;
    UINT32 c21;
    UINT32 c22;
    UINT32 gamut_matrix_en;

    //lbox detection
    UINT32 UPpos;
    UINT32 DNpos;

    //histogram
    UINT32 reg_hist_en;

    //GainCurve
    UINT32 reg_luma_gain_en;

    //Tone Mapping
    UINT32 tone_map_en;

    //letterbox
    UINT32 lbox_det_en;

    //NR-B
    UINT32 reg_nr_strength;
    UINT32 reg_filter_no;
    UINT32 reg_NR_en;

    //Adaptive luminance control
    UINT32 reg_maxRGB_weight;

    //SCE
#ifdef R2Y_OUTPUT_FORMAT_SELECT
    UINT32 r2y_en;
    UINT32 r2y_output_format;
    UINT32 r2y_c00;
    UINT32 r2y_c01;
    UINT32 r2y_c02;
    UINT32 r2y_c10;
    UINT32 r2y_c11;
    UINT32 r2y_c12;
    UINT32 r2y_c20;
    UINT32 r2y_c21;
    UINT32 r2y_c22;
    UINT32 r2y_pre_add_0;
    UINT32 r2y_pre_add_1;
    UINT32 r2y_pre_add_2;
    UINT32 r2y_post_add_0;
    UINT32 r2y_post_add_1;
    UINT32 r2y_post_add_2;
#endif
    //UINT32 output_gamma; //reserved
    UINT32 output_gamma_en;
    UINT32 hdr_en;
#ifdef Relay
    UINT32 hdr_relay;
#else
    UINT32 hdr_bypass;
#endif

#if EOTF_TBL_TYPE == 0 //0:only fix
    UINT32 BBC_gamma;
    UINT32 sdr_gamma;
#elif EOTF_TBL_TYPE == 1 //1: only programmalbe
    UINT32 programmable_EOTF[3][EOTF_POINTS]; // should cooperate with reg: eotf_table_sep_mode = 0 to reduce bandwidth
    UINT32 last_eotf_elementR;
    UINT32 last_eotf_posR;
    UINT32 last_eotf_elementG;
    UINT32 last_eotf_posG;
    UINT32 last_eotf_elementB;
    UINT32 last_eotf_posB;
    UINT32 eotf_accuracy_control_P1;
    UINT32 eotf_accuracy_control_P2;
    UINT32 eotf_accuracy_control_P3;
    UINT32 eotf_accuracy_control_P4;
#elif EOTF_TBL_TYPE == 2 //2: 1 fix + programmable
    UINT32 sdr_gamma;
    UINT32 programmable_EOTF[EOTF_POINTS];
    UINT32 last_eotf_element;
    UINT32 last_eotf_pos;
#elif EOTF_TBL_TYPE == 3 //3: selectable fix + prgrammable
    UINT32 input_gamma;
    UINT32 programmable_EOTF[EOTF_POINTS];
    UINT32 last_eotf_element;
    UINT32 last_eotf_pos;
#elif EOTF_TBL_TYPE == 4 //4: selectable fix
    UINT32 input_gamma;
#endif

#if OETF_TBL_TYPE == 0 //0:only fix
#elif OETF_TBL_TYPE == 1 //1: only programmalbe
    UINT32 programmable_OETF[3][OETF_POINTS];
#elif OETF_TBL_TYPE == 2 //2: 1 fix + programmable
#elif OETF_TBL_TYPE == 3 //3: selectable fix + prgrammable
    UINT32 output_gamma;
    UINT32 programmable_OETF[OETF_POINTS];
#elif OETF_TBL_TYPE == 4 //4: selectable fix
    UINT32 output_gamma;
#endif

#ifdef HLG_SYSTEM_Beta
    UINT32 hlg_beta;
#endif

    APPLICATION_E application;
    struct SDR2HDRFWFeedbackMemberVariable SDR2HDRFW_FBMV;

    AIHDR_INPUT   inAIHDR;
    DynLumInfo ImgLumInfo;

    UINT32 cust_reg_p1;
    UINT32 cust_reg_p2;
    UINT32 cust_reg_p3;
    UINT32 cust_reg_p4;
    UINT32 cust_reg_p5;
    UINT32 cust_reg_p6;
    UINT32 cust_reg_p7;
    UINT32 cust_reg_p8;
    UINT32 cust_reg_p9;
    UINT32 cust_reg_p10;
    UINT32 cust_reg_p11;
    UINT32 cust_reg_p12;
    UINT32 cust_reg_p13;
    UINT32 cust_reg_p14;
    UINT32 cust_reg_p15;
    UINT32 cust_reg_p16;

    UINT32 cust_reg_slope0;
    UINT32 cust_reg_slope1;
    UINT32 cust_reg_slope2;
    UINT32 cust_reg_slope3;
    UINT32 cust_reg_slope4;
    UINT32 cust_reg_slope5;
    UINT32 cust_reg_slope6;
    UINT32 cust_reg_slope7;
    UINT32 cust_reg_slope8;
    UINT32 cust_reg_slope9;
    UINT32 cust_reg_slope10;
    UINT32 cust_reg_slope11;
    UINT32 cust_reg_slope12;
    UINT32 cust_reg_slope13;
    UINT32 cust_reg_slope14;
    UINT32 cust_reg_slope15;

    UINT32 aihdr_reg_p1;
    UINT32 aihdr_reg_p2;
    UINT32 aihdr_reg_p3;
    UINT32 aihdr_reg_p4;
    UINT32 aihdr_reg_p5;
    UINT32 aihdr_reg_p6;
    UINT32 aihdr_reg_p7;
    UINT32 aihdr_reg_p8;
    UINT32 aihdr_reg_p9;
    UINT32 aihdr_reg_p10;
    UINT32 aihdr_reg_p11;
    UINT32 aihdr_reg_p12;
    UINT32 aihdr_reg_p13;
    UINT32 aihdr_reg_p14;
    UINT32 aihdr_reg_p15;
    UINT32 aihdr_reg_p16;

    UINT32 aihdr_reg_slope0;
    UINT32 aihdr_reg_slope1;
    UINT32 aihdr_reg_slope2;
    UINT32 aihdr_reg_slope3;
    UINT32 aihdr_reg_slope4;
    UINT32 aihdr_reg_slope5;
    UINT32 aihdr_reg_slope6;
    UINT32 aihdr_reg_slope7;
    UINT32 aihdr_reg_slope8;
    UINT32 aihdr_reg_slope9;
    UINT32 aihdr_reg_slope10;
    UINT32 aihdr_reg_slope11;
    UINT32 aihdr_reg_slope12;
    UINT32 aihdr_reg_slope13;
    UINT32 aihdr_reg_slope14;
    UINT32 aihdr_reg_slope15;
}SDR2HDRHDROUTPUT;

class CPQSDR2HDRFW
{

private:
    INT32 _set2sCompNum(INT32 val, INT32 bits);
    INT32 _Get2sCompNum(INT32 val, INT32 bits);
    void gain_curve_hw_setting(SDR2HDRHDROUTPUT *output, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV);
    void aihdr_tone_mapping_fw_refine(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void gain_curve_gen_fw(SDR2HDRINPUT *input, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void cust_tone_mapping_fw(SDR2HDRINPUT *input, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void cust_tone_mapping_fw_refine(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void aihdr_cust_tgt_nits_mapping(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void aihdr_curve_smoothing(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void aiHDRToneCurveRemap(IN SDR2HDRINPUT *input);
    void curve_blending(SDR2HDRINPUT *input, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void curve_blending_tone_mapping_fw_refine(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void tone_mapping_hw_setting(SDR2HDROUTPUT *output);
    UINT32 rampWeight(IN UINT32  v1, IN UINT32  v2, IN UINT32  t1, IN UINT32  t2, IN UINT32  t);
    void aihdr_gain_table_fw(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pSDR2HDRFWReg, SDR2HDROUTPUT *output);

    UINT32 sameSceneDetect(struct SDR2HDRFWReg *pSDR2HDRFWReg);

    void logMechanism(SDR2HDRINPUT *input); // for Android log
    PQLoggingMechanism *pLogMech; // for Android log
    enum log_tag_id
    {
        SDR2HDREntryFlag = 0,
        SDR2HDRInputSDR2HDRINPUTSDR2HDRFWReg,
        SDR2HDRInputFirewareMemberVariable,
        SDR2HDROutputSDR2HDROUTPUT,
        SDR2HDROutputGainCurve,
        SDR2HDRTime,
        SDR2HDRContentFrame,
        SDR2HDRContentStream,
        log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];

public:
    void histogram_window_fw(SDR2HDRINPUT *input, SDR2HDROUTPUT *output, struct SDR2HDRFWReg *pSDR2HDRFWReg);
    void aiHDRpreRemapping(IN  unsigned char* BGRbuffer, IN struct AIHDR_INPUT inAIHDR, OUT unsigned char* RGBAbuffer);
    void onInitCommon(void);
    void onInitPlatform(IN SDR2HDRINPUT*input, OUT SDR2HDROUTPUT *output);
    void onInitPlatform(char *tableName);  // for customer setting
    void onDynamicCalculate(IN SDR2HDRINPUT *input, OUT SDR2HDROUTPUT *output);
    void onStaticCalculate(IN SDR2HDRINPUT *input, OUT SDR2HDROUTPUT *output);
    void onHistogramMerge(IN UINT32 *RGBmaxHistogram_Left, IN UINT32 *RGBmaxHistogram_Right, OUT UINT32 *RGBmaxHistogram);
    void Application_Selector(IN APPLICATION_E Application, OUT int *HDR_sel);
    void custParamMapping(int *HDR_sel);
    void initSDR2HDRRegEntry();
    //void aiHDRToneCurveRemap(IN SDR2HDRINPUT *input, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV);
    //void aiHDRToneCurveRemap(IN SDR2HDRINPUT *input);
    //void tone_mapping_hw_setting(SDR2HDROUTPUT *output, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV);
    //void tone_mapping_hw_setting(SDR2HDROUTPUT *output);
    void tone_mapping_sw_InitSetting(OUT SDR2HDRINPUT *input, OUT SDR2HDROUTPUT *output, IN struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV);
    void setting_by_feedbackMV(SDR2HDRINPUT *input, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV);
    void feedbackMV_setting(struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, SDR2HDROUTPUT *output);
    void setting_by_SWreg(SDR2HDRINPUT *input, SDR2HDROUTPUT *output, struct SDR2HDRFWMemberVariable *pSDR2HDRFWMV, struct SDR2HDRFWReg *pHDRFWReg);
    void aiValueDefaultSet(struct Aslope *pAslope);

    custSDR2HDRFWReg iSDR2HDRRegEntry[SDR2HDR_TBL_SIZE];
    SDR2HDRFWMemberVariable *pSDR2HDRFWMV;
    SDR2HDRFWMemberVariable vSDR2HDRFWMV;

    SDR2HDRFWReg *pSDR2HDRFWReg;
    SDR2HDRFWReg vSDR2HDRFWReg;
    UINT32 RGBmaxHistogram_1_ori[hist_bins];

    CPQSDR2HDRFW()
    {
        pSDR2HDRFWReg = &vSDR2HDRFWReg;
        pSDR2HDRFWMV = &vSDR2HDRFWMV;

        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
            "SDR2HDREntryFlag",
            "SDR2HDRInputSDR2HDRINPUTSDR2HDRFWReg",
            "SDR2HDRInputFirewareMemberVariable",
            "SDR2HDROutputSDR2HDROUTPUT",
            "SDR2HDROutputGainCurve",
            "SDR2HDRTime",
            "SDR2HDRContentFrame",
            "SDR2HDRContentStream"
        };

        for(int i = 0; i < log_tag_len; ++i)
            log_state_list[i] = ANDROID_LOG_DISABLE;

        onInitCommon();
        initSDR2HDRRegEntry();
    };

    ~CPQSDR2HDRFW();

};

#ifdef __cplusplus
}
#endif

#endif
