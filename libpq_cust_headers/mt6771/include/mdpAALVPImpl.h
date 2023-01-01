#ifndef MDPAALVPIMPL_H
#define MDPAALVPIMPL_H

/*
* header files
*/

/*
* standard header files
*/
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

#include "mdpAALVPCommon.h"
#include "mdpAALVPImplCore.h"
#include "PQSDR2HDRImpl.h"

#include "PQLoggingMechanism.h"

#define UNUSED_AIAAL(reserve)(void)(reserve)

#define TBL_SIZE 38

#define DREVP_SLOPE_BIT 15
#define DREVP_SLOPE_BIT_ROUND ( ( 1 << ( DREVP_SLOPE_BIT ) ) >> 1 )

#define DREVP_STRENGTH_BIT 7
#define DREVP_STRENGTH_BIT_ROUND ( ( 1 << ( DREVP_STRENGTH_BIT ) ) >> 1 )

#define DREVP_FLT_COEF_BIT 7
#define DREVP_FLT_COEF_RANGE ( 1 << ( DREVP_FLT_COEF_BIT ) )
#define DREVP_FLT_COEF_BIT_ROUND ( ( 1 << ( DREVP_FLT_COEF_BIT ) ) >> 1 )

#define DREVP_GAIN_INDEX_FLOAT_BIT 8
#define DREVP_GAIN_INDEX_INT_BIT 4
#define DREVP_GAIN_INDEX_BIT ( ( DREVP_GAIN_INDEX_FLOAT_BIT ) + ( DREVP_GAIN_INDEX_INT_BIT ) )
#define DREVP_GAIN_INDEX_BIT_RANGE ( 1 << ( DREVP_GAIN_INDEX_BIT ) )
#define DREVP_GAIN_INDEX_BIT_ROUND ( ( 1 << ( DREVP_GAIN_INDEX_BIT ) ) >> 1 )
#define DREVP_GAIN_INDEX_FLOAT_BIT_ROUND ( ( 1 << ( DREVP_GAIN_INDEX_FLOAT_BIT ) ) >> 1 )

#define DREVP_OVER_PXL_SHIFT_BIT 8
#define DREVP_OVER_PXL_SHIFT_BIT_ROUND ( ( 1 << ( DREVP_OVER_PXL_SHIFT_BIT ) ) >> 1 )
#define DREVP_NOR_OVER_PXL_STRNGTH_MAX 256

#define DREVP_DYNC_FLT_WGT_MAX_BIT 8
#define DREVP_DYNC_FLT_WGT_MAX_BIT_ROUND ( ( 1 << ( DREVP_DYNC_FLT_WGT_MAX_BIT ) ) >> 1 )
#define DREVP_DYNC_FLT_WGT_MAX ( 1 << ( DREVP_DYNC_FLT_WGT_MAX_BIT ) )
#define DREVP_DYNC_FLT_SLOPE_BIT 4
#define DREVP_DYNC_FLT_SLOPE_BIT_ROUND ( ( 1 << ( DREVP_DYNC_FLT_SLOPE_BIT ) ) >> 1 )

#define DREVP_DEFLICKER_DETECT_WINDOW 16

struct DebugPropertyField{
    std::string name;
    int* address;
    int max_value;
    int min_value;
    DebugPropertyField() = delete;
    DebugPropertyField(std::string name, int* address, int max_value = INT_MAX, int min_value = INT_MIN)
        :name(name), address(address), max_value(max_value), min_value(min_value) {}
};


#if defined(PQ_HDR_VERSION_0) || defined(PQ_HDR_VERSION_1) || defined(SUPPORT_HDR)
    #define SUPPORT_HDR_LICENSE
#endif

typedef unsigned int UINT32;
typedef int INT32;
typedef unsigned long long UINT64;

enum MdpAALVPEvent {
    // Configuration changed
    eMdpAALVPEvtConfigChanged   = 0x10,
    // Some configuration field has been written
    eMdpAALVPEvtFieldWritten    = 0x20,
    // Intialize, set when boot or AAL restart
    eMdpAALVPEvtInit            = 0x30,
    // Some configuration field has been read
    eMdpAALVPEvtFieldRead       = 0x40,
};

typedef enum DREVP_PROFILE_ENUM
{
    DREVP_PROFILE_BT601, //Limited range
    DREVP_PROFILE_BT709,
    DREVP_PROFILE_JPEG,
    DREVP_PROFILE_FULL_BT601 = DREVP_PROFILE_JPEG,
    DREVP_PROFILE_BT2020,     // not support for output
    DREVP_PROFILE_FULL_BT709, // not support for output
    DREVP_PROFILE_FULL_BT2020 // not support for output
} DREVP_PROFILE_ENUM;




#if (SCLTM_VERSION == SCLTM_VERSION_3_0)
struct DREVPInitParam {
    int frame_width;
    int frame_height;
    int tile_pxl_start;
    int tile_pxl_end;
    int act_win_x_start;
    int act_win_x_end;
    int isLastTile;
    int demo_win_x_start;
    int demo_win_x_end;
    int ColorFormat; // Input Video Color Space
};
#elif (SCLTM_VERSION == SCLTM_VERSION_3_1)
struct DREVPInitParam {
    int frame_width;
    int frame_height;
    int tile_pxl_x_start;
    int tile_pxl_x_end;
    int tile_pxl_y_start;
    int tile_pxl_y_end;
    int act_win_x_start;
    int act_win_x_end;
    int act_win_y_start;
    int act_win_y_end;
    int isLastTile_x;
    int isLastTile_y;
    int demo_win_x_start;
    int demo_win_x_end;
    int demo_win_y_start;
    int demo_win_y_end;
    int save_first_blk_col_flag;
    int save_last_blk_col_flag;
    int ColorFormat; // Input Video Color Space
};
#elif (SCLTM_VERSION == SCLTM_VERSION_3_5)
struct DREVPInitParam {
    int frame_width;
    int frame_height;
    int tile_pxl_x_start;
    int tile_pxl_x_end;
    int tile_pxl_y_start;
    int tile_pxl_y_end;
    int act_win_x_start;
    int act_win_x_end;
    int act_win_y_start;
    int act_win_y_end;
    int isLastTile_x;
    int isLastTile_y;
    int demo_win_x_start;
    int demo_win_x_end;
    int demo_win_y_start;
    int demo_win_y_end;
    int save_first_blk_col_flag;
    int save_last_blk_col_flag;
    int ColorFormat; // Input Video Color Space
};
#endif

//#if (SCLTM_VERSION == SCLTM_VERSION_3_5)
struct DualInfoMDP {
    int MaxHis_denominator_pipe0[mdpDREVP_BLK_NUM_X];
    int MaxHis_denominator_pipe1[mdpDREVP_BLK_NUM_X];
};
//#endif

// AAL HW registers
struct DREVPInitReg {
    int dre_blk_x_num;
    int dre_blk_y_num;
    int dre_blk_height;
    int dre_blk_width;
    int dre_blk_area;
    int dre_x_alpha_base;
    int dre_x_alpha_shift_bit;
    int dre_y_alpha_base;
    int dre_y_alpha_shift_bit;
    int dre_flat_length_th;
    int dre_flat_length_slope;
    int dre_blk_area_min;
    int HSIZE;
    int VSIZE;
#if (SCLTM_VERSION == SCLTM_VERSION_3_0)
    int act_win_x_start;
    int act_win_x_end;
    int blk_num_start;
    int blk_num_end;
    int blk_cnt_start;
    int blk_cnt_end;
    int last_tile_flag;
    int win_x_start;
    int win_x_end;
#elif (SCLTM_VERSION == SCLTM_VERSION_3_1)
    int act_win_x_start;
    int act_win_x_end;
    int act_win_y_start;
    int act_win_y_end;
    int blk_num_x_start;
    int blk_num_x_end;
    int blk_cnt_x_start;
    int blk_cnt_x_end;
    int blk_num_y_start;
    int blk_num_y_end;
    int blk_cnt_y_start;
    int blk_cnt_y_end;
    int last_tile_x_flag;
    int last_tile_y_flag;
    int win_x_start;
    int win_x_end;
    int win_y_start;
    int win_y_end;
    int save_first_blk_col_flag;
    int save_last_blk_col_flag ;
#elif (SCLTM_VERSION == SCLTM_VERSION_3_5)
    int act_win_x_start;
    int act_win_x_end;
    int act_win_y_start;
    int act_win_y_end;
    int blk_num_x_start;
    int blk_num_x_end;
    int blk_cnt_x_start;
    int blk_cnt_x_end;
    int blk_num_y_start;
    int blk_num_y_end;
    int blk_cnt_y_start;
    int blk_cnt_y_end;
    int last_tile_x_flag;
    int last_tile_y_flag;
    int win_x_start;
    int win_x_end;
    int win_y_start;
    int win_y_end;
    int dre_s_lower2;
    int dre_s_upper2;
    int dre_y_lower2;
    int dre_y_upper2;
    int dre_h_lower2;
    int dre_h_upper2;
    int dre_h_slope2;
    int dre_s_slope2;
    int dre_y_slope2;
    int dre_s_lower3;
    int dre_s_upper3;
    int dre_y_lower3;
    int dre_y_upper3;
    int dre_h_lower3;
    int dre_h_upper3;
    int dre_h_slope3;
    int dre_s_slope3;
    int dre_y_slope3;
    int bilateral_flt_en;
    int bilateral_impulse_noise_en;
    int bilateral_range_flt_slope;
    int dre_bilateral_detect_en;
    int dre_bilateral_blending_en;
    int dre_bilateral_blending_wgt;
    int save_first_blk_col_flag;
    int save_last_blk_col_flag ;
    int round_to_9b;
    int y2r_en;
    int r2y_en;
    int y2r_pre_add_0_s;
    int y2r_pre_add_1_s;
    int y2r_pre_add_2_s;
    int y2r_c00_s;
    int y2r_c01_s;
    int y2r_c02_s;
    int y2r_c10_s;
    int y2r_c11_s;
    int y2r_c12_s;
    int y2r_c20_s;
    int y2r_c21_s;
    int y2r_c22_s;
    int r2y_post_add_0_s;
    int r2y_post_add_1_s;
    int r2y_post_add_2_s;
    int r2y_c00_s;
    int r2y_c01_s;
    int r2y_c02_s;
    int r2y_c10_s;
    int r2y_c11_s;
    int r2y_c12_s;
    int r2y_c20_s;
    int r2y_c21_s;
    int r2y_c22_s;
#endif
    int dre_s_lower;
    int dre_s_upper;
    int dre_y_lower;
    int dre_y_upper;
    int dre_h_lower;
    int dre_h_upper;
    int dre_h_slope;
    int dre_s_slope;
    int dre_y_slope;
    int dre_bitplus_contour_range_th;
    int dre_bitplus_contour_range_slope;
    int dre_bitplus_diff_count_th;
    int dre_bitplus_diff_count_slope;
    int dre_bitplus_pxl_diff_th;
    int dre_bitplus_pxl_diff_slope;
    int dre_bitplus_noise_range_th;
    int dre_bitplus_noise_range_slope;
    int dre_bitplus_signchange_count_th;
    int dre_bitplus_signchange_count_slope;
    int dre_bitplus_high_lvl_out_oft;
    int dre_bitplus_high_lvl_pxl_slope;
    int dre_bitplus_high_lvl_pxl_th;
    int dre_bitplus_high_lvl_out_oft2;
    int dre_bitplus_high_lvl_pxl_slope2;
    int dre_bitplus_high_lvl_pxl_th2;
    int dre_bitplus_to_ali_wgt;
    int dre_pxl_diff_th_for_flat_pxl;
    int dre_pxl_diff_slope_for_flat_pxl;
    int dre_pxl_diff_th;
    int dre_pxl_diff_slope;
    int dre_output_mode;
    int dre_max_hist_mode;
};


// Fields collected from AAL HW
struct DREVPTopInput {
    unsigned int eventFlags;

    int DREMaxHisSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_LUMA_HIST_NUM_WEIGHT];

    int DRERGBMaxSum[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
#if (SCLTM_VERSION == SCLTM_VERSION_3_0)
    int DREChromaHist[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X]; //HW calculated Skin histogram for SCLTM_VERSION_3_0;
#elif (SCLTM_VERSION == SCLTM_VERSION_3_1)
    int DREChromaHist[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];  //Used as a virtual buffer for FD in SCLTM_VERSION_3_1
    int DREChromaHist2[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X]; //HW calculated Green histogram for SCLTM_VERSION_3_1;
#elif (SCLTM_VERSION == SCLTM_VERSION_3_5)
    int DREChromaHist[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
    int DREChromaHist2[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X]; //HW calculated Green histogram for SCLTM_VERSION_3_1;
    int DREChromaHist3[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
#endif
    int DREFlatLineCountSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

    int DRELargeDiffCountSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

    int DREMaxDiffSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

    int dre_blk_x_num;
    int dre_blk_y_num;

    int IspScenario;

    int PreWidth;
    int PreHeight;
    int CurWidth;
    int CurHeight;

    int PreFloatCurve[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_LUMA_CURVE_NUM]; // 15-bit [0, 255*128]
};

// Fields which will be set to HW registers
struct DREVPTopOutput {
    int DRECurveSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_LUMA_CURVE_NUM];
    int CurFloatCurve[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_LUMA_CURVE_NUM]; // 15-bit [0, 255*128]
};

// Game PQ share data for session switching
struct DREsharedata {
    unsigned char PreFloatCurve_game[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_LUMA_CURVE_NUM];
    unsigned char CurTgtCurve_game[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][ mdpDREVP_LUMA_CURVE_NUM ];
    unsigned char FinalCoef_game[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
    unsigned short DRESkinWgtSet_game[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
    int frameCount;
};

struct DREVPRegNorm_RO
{
    int g_vhist_set[mdpDREVP_LUMA_HIST_NUM_WEIGHT];
};


///////////////////////////////////////////////////////////////////////////////
// MDP AAL FW Processing class
///////////////////////////////////////////////////////////////////////////////
class CDREVPTopFW
{
private:
  unsigned int DebugFlags;

  int CurTgtCurve[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][ mdpDREVP_LUMA_CURVE_NUM ]; // 8-bit [0, 255]
  int CurFltCurve[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][ mdpDREVP_LUMA_CURVE_NUM ]; // 8-bit [0, 255]

  int flickerAreaWgt[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

  int pre_DarkZoneHist[DREVP_DEFLICKER_DETECT_WINDOW][mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  int pre_flatLineCount[DREVP_DEFLICKER_DETECT_WINDOW][mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  int pre_data_index;

  int DRESkinWgtSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  int pre_DRESkinWgtSet[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

  int Tmpfinal_l_curve_wgt_set[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];

  int Blk_Hist_Wgt[mdpDREVP_BLK_SFLT_SIZE_MAX][mdpDREVP_BLK_SFLT_SIZE_MAX];
  int tmp_blk_y_ary[mdpDREVP_BLK_SFLT_SIZE_MAX][mdpDREVP_BLK_NUM_Y];
  int tmp_blk_x_ary[mdpDREVP_BLK_SFLT_SIZE_MAX][mdpDREVP_BLK_NUM_X];
  int tmp_blk_y_ary2[5][mdpDREVP_BLK_NUM_Y];
  int tmp_blk_x_ary2[5][mdpDREVP_BLK_NUM_X];
  int wgt_sum_ary[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  bool boundry_inout_ary[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][mdpDREVP_BLK_SFLT_SIZE_MAX][mdpDREVP_BLK_SFLT_SIZE_MAX];
  bool boundry_inout_ary2[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X][5][5];
  int Blk_LPF_W;
  int Blk_LPF_H;
  int tmp_shift_bit;
  int tmp_shift_bit_round;
  int cen_x;
  int cen_y;

  int IIRForceRange;
  int pre_dre_dync_flt_coef_min;
  int pre_dre_dync_flt_ovr_pxl_slope;
  int pre_BlkSpaFltType;
  int pre_dre_blk_x_num;
  int pre_dre_blk_y_num;
  char inname[1000];
  char integer_string[100];
  bool isFirstFrame;
  bool isSecondFrame;
  bool isFilterFlat;
  int session_switch;
  int session_switch_skin;
  bool isLimited;
  int isUICombined;
  int FrameCount;
  int ReadDebugProperty(std::string, int*);
  void UpdateDebugProperty();
  void InitialDebugProperty();
  int globalHist[mdpDREVP_LUMA_HIST_NUM_WEIGHT];
  void EventTriggerSetting(const DREVPTopInput &input, DREVPTopOutput *output);
  void DREGainIdxCal(const DREVPTopInput &input);
  void EarlyTermination(const DREVPTopInput &input);
  unsigned int vSpikeLumaWeight( const int waHist[mdpDREVP_LUMA_HIST_NUM_WEIGHT] );
  unsigned int vAdapSecondLumaWeight( const int ColorHist, const int BlkNum );
  void DRECurveCalculation(const DREVPTopInput &input);
  void BlkFlickerFallback(const DREVPTopInput &input, DREVPTopOutput *output);
  void SceneChangeDetection(const DREVPTopInput &input);
  void G_AdaptiveLuma_Block( int dre_blk_x_num, int dre_blk_y_num, int HistNorEn, int *HisBin, int RGBMaxSum, int ChromaHistCount, int ChromaHistCount2, int *iYFtn );
  void L_AdaptiveLuma_Block( int dre_blk_x_num, int dre_blk_y_num, int HistNorEn, int *HisBin, int RGBMaxSum, int ChromaHistCount, int *iYFtn, int ChromaHistCount2 );
  void DREGainIdxFlt(const DREVPTopInput &input, DREVPTopOutput *output);
  int DynCAdaptCoef( const int NorHist[ mdpDREVP_LUMA_HIST_NUM_WEIGHT ], const int PreFloatCurve[ mdpDREVP_LUMA_CURVE_NUM ], int CurTgtCurve[ mdpDREVP_LUMA_CURVE_NUM ], int CoefBit );
  int IIR( int CurIn, int PreOutFloat, int & CurOutFloat, int CoefBit, int CurInCoef );
  void calIIRForceRange();
  void DREFinalGainCal(const DREVPTopInput &input, DREVPTopOutput *output);
  void BlkSPF(const DREVPTopInput &input, DREVPTopOutput *output);
  void BlkSTFP(const DREVPTopInput &input, DREVPTopOutput *output);
  void BlkFFP(const DREVPTopInput &input, DREVPTopOutput *output);
  void IspPreviewZoom(const DREVPTopInput &input, DREVPTopOutput *output);
  void LimitedRangeFallback(const DREVPTopInput &input, DREVPTopOutput *output);
  int GlobalAPL;
  CDREVPCoreFW * pDRECoreFW;
  DREVPCoreInput * pDRECoreIn;
  DREVPCoreOutput * pDRECoreOut;
  bool m_isHDR;

  // AIPQ Scene threshold
  int NNScenePortraitTh; // Portrait
  int NNSceneFoodTh; // Food
  int NNSceneSkyTh; // Sky
  int NNSceneGreenTh; // Green
  int NNSceneArchitectureTh; // Architecture

  int AALSceneNightTh; // Night
  int AALSceneSnowTh;
  bool Custom_SCLTMAISelector; // 0: Default; 1: Custom_AIPQ_selector
  DREVPRegNorm_RO * pDRERegNorm_RO;

  // Debug ROI mechanism
  int roi_x;
  int roi_y;
  int roi_w;
  int roi_h;
  void UpdateRoIRange(const DREVPTopInput&);
  int roi_x_start;
  int roi_y_start;
  int roi_x_end;
  int roi_y_end;

  void logMechanism(const DREVPTopInput&);
  // Debug logger
  PQLoggingMechanism *pLogMech;
  enum log_tag_id
  {
      Input = 0,
      DRE,
      DualPipe,
      DRE_SHAREMEM,
      log_tag_len
  };
  char **log_tag;
  int log_state_list[log_tag_len];

  std::vector<DebugPropertyField> debug_property_list;

  // Scene change detection
  int preBin25;
  int preBin75;
  int preLocalBin25[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  int preLocalBin75[mdpDREVP_BLK_NUM_Y][mdpDREVP_BLK_NUM_X];
  bool sceneChange;
  int sceneChangeLocalCountTh;

  // for PQ_selector
  bool pqSelectInitialTransitionFrameEnable;
  int pqSelectInitialTransitionFrame;

public:
  DREVPReg * pDREReg;
  DREVPInitReg * pDREInitReg;
  DREVPInternalReg * pDREInternalReg;
  DREVPReg iDRERegEntry[TBL_SIZE];
  DREVPReg_RO * pDREReg_RO;
  DREsharedata * pDREsharedata;

  void setDebugFlags(unsigned int debug);

  int DREVersionNum;
  void DREBlkNumCal(const DREVPInitParam &initParam, int wid, int hei);
  void calBlkSpaFltParam();
  void DREBlkHWRegCal( int Blk_Num, int Width, int &blk_width, int &dre_x_alpha_base, int &dre_x_alpha_shift_bit );
  void DREFrameHWRegCal(const DREVPInitParam &initParam);
  void DRETileHWRegCal(const DREVPInitParam &initParam);

  void onLoadXML(unsigned char* setting);
  void onLoadTable( int sel = 0);
  void initDRERegEntry();
  void DualPipeVPBlockStatistics(DREVPTopInput & pipe0, DREVPTopInput & pipe1, DREVPTopInput & full, DREVPInitParam &initParam, const DualInfoMDP &pipeinfo, const int wholefbwidth, const int wholefbheight, const int cuttingposition);

  void setShareMemory( DREsharedata *sharedata);
  void getShareMemory( DREsharedata *sharedata);
  void initShareMemory( DREsharedata *sharedata);
  void getGlobalVHist( int* read_buffer, int buffer_size);

  void PQ_selector( IN PQSelectorInput pqSelectorInput, OUT int &SCLTM_sel);
  void AIPQ_selector( const float NNSceneProbability[15], float (&AALSceneProbability)[5], const int wholefbwidth, const int wholefbheight, const int srcwidth, const int srcheight, \
  const int QPsum, int &SCLTM_sel);
  // for custom
  void Custom_AIPQ_selector( const float NNSceneProbability[15], const float AALSceneProbability[5], const int wholefbwidth, const int wholefbheight, const int srcwidth, const int srcheight, \
  const int QPsum, int &SCLTM_sel )
  {
      // NNSceneProbability[0]: Portrait // [1]: Sky // [2]: Green // [3]: Food // [4]: Architecture
      // AALSceneProbability[0]: Night // [1]: Snow
      // wholefbwidth, wholefbheight: Frame buffer size (fixed size)
      // srcwidth, srcheight: video size (depends on scenario)
      // Qpsum: From Video decoder QP
      // SCLTM_sel: setting number
      UNUSED_AIAAL(NNSceneProbability);
      UNUSED_AIAAL(AALSceneProbability);
      UNUSED_AIAAL(wholefbwidth);
      UNUSED_AIAAL(wholefbheight);
      UNUSED_AIAAL(srcwidth);
      UNUSED_AIAAL(srcheight);
      UNUSED_AIAAL(QPsum);


      // output tuning set
      SCLTM_sel = 2;
  };
  void setWhoSelector(bool who);


#ifdef DRE_ANDROID_PLATFORM
  void onCalculate(const DREVPTopInput &input, DREVPTopOutput *output);
  void onInitCommon(const DREVPInitParam &initParam, const int wholefbwidth, const int wholefbheight);
  void onInitPlatform();
  void onInitPart(const DREVPInitParam &initParam, DREVPTopOutput *output);
  // Initialization will call onInitCommon() -> onInitPlatform()
  // onInitCommon() performs common initialization.
  // onInitPlatform() will load customization parameters
  // onInitPlay() performs part of initialization
#else
  void Main(const DREVPTopInput &input, DREVPTopOutput *output);
  void Initialize(const DREVPInitParam &initParam, const int wholefbwidth, const int wholefbheight);
  void ReInitialize(const DREVPInitParam &initParam, DREVPTopOutput *output);
  FILE * ifp0;
  FILE * ifp2;
#endif


  CDREVPTopFW()
  {
    memset(this, 0, sizeof(*this));
    pDREReg = new DREVPReg;
    pDREInitReg = new DREVPInitReg;
    pDREInternalReg = new DREVPInternalReg;
    pDREReg_RO = new DREVPReg_RO;
    pDRECoreFW  = new CDREVPCoreFW;
    pDRECoreIn = new DREVPCoreInput;
    pDRECoreOut = new DREVPCoreOutput;
    pDREsharedata = new DREsharedata;
    pDRERegNorm_RO = new DREVPRegNorm_RO;
    pDRECoreFW->pDREReg = pDREReg;
    pDRECoreFW->pDREInternalReg = pDREInternalReg;

    DebugFlags = 0;
    isFirstFrame = false;
    isSecondFrame = false;
    isFilterFlat = false;
    session_switch = 32;
    session_switch_skin = 32;
    isUICombined = 1;
    FrameCount = 0;
    m_isHDR = false;
    pre_BlkSpaFltType = 0x1;
    pre_dre_blk_x_num = 16;
    pre_dre_blk_y_num = 8;
    // AIPQ Scene threshold
    NNScenePortraitTh = 500; // Portrait
    NNSceneFoodTh = 500; // Food
    NNSceneSkyTh = 500; // Sky
    NNSceneGreenTh = 500; // Green
    NNSceneArchitectureTh = 500; // Architecture
    AALSceneNightTh = 500; // Night
    AALSceneSnowTh = 500; // Snow
    Custom_SCLTMAISelector = 0; // Default

    pLogMech = new PQLoggingMechanism(log_tag_len);
    log_tag = new char*[log_tag_len] {
        "MDPAALVPInput",
        "MDPAALVPDRE",
        "MDPAALVPDualPipe"
    };

    for(int i = 0; i < log_tag_len; ++i)
        log_state_list[i] = ANDROID_LOG_DISABLE;

    InitialDebugProperty();
    //initDRERegEntry();
  };

  ~CDREVPTopFW()
  {
    delete pDREReg;
    delete pDREInitReg;
    delete pDREInternalReg;
    delete pDREReg_RO;
    delete pDRECoreFW;
    delete pDRECoreIn;
    delete pDRECoreOut;
    delete pDREsharedata;
    delete pLogMech;
    delete log_tag;
    delete pDRERegNorm_RO;

#ifndef DRE_ANDROID_PLATFORM
  if( DebugFlags & eMdpAALVPDebugDRE )
  {
    fclose( ifp0 );
  }
#endif
  };

};

#endif
