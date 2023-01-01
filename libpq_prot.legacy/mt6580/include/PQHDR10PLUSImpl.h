#ifndef __HDR10PLUS_FW__
#define __HDR10PLUS_FW__

#ifdef __cplusplus
extern "C"
{
#endif

#include "PQHDRImpl.h"

//#define __Local_Sim_Debug__
#define OPTIMIZE_TONE_FIX
#define GAIN_TABLE_OPTIMIZATION
//#define HDR10PLUS_SMOOTHING

//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define TgtMaxLumCoeff            10   //10
#define BezierCal_Bits            12
#define BezierCal_NORM      ((1 << BezierCal_Bits)-1) //16383
#define Lum_Bits                  10
#define Lum_NORM            (((1 <<Lum_Bits))-1)  //1023

#define LOWEST_GAIN_VALUE                                8192  // Gain = Value/65535
#define META_ST2094_40_MAX_SIZE                            15
#define META_ST2094_40_MAX_NUM_DIST_SIZE                   15
#define META_ST2094_40_MAX_NUM_BZ_COEFF_SIZE               14
#define META_2094_40_PERCENTILE_DIVIDE                      1
#define GAIN_POINT_COUNT_INTERVAL                          16

#define  DYNAMIC_COMPRESS_ORDER  10
#define Num_HDR10pInternalReg_Dump_cmparam_FW_Reg 176
#define Num_HDR10pInternalReg_Dump_cmparam_metadata 56
#define Num_HDR10pInternalReg_Dump_cmparam ( Num_HDR10pInternalReg_Dump_cmparam_FW_Reg + Num_HDR10pInternalReg_Dump_cmparam_metadata )

#define META_JSON_2094_100K_PERCENTILE_DIVIDE    10.0
#define META_MAX_PSLL_SIZE                       15
#define META_MAX_PCOEFF_SIZE                     14

#define HDR10plus_TBL_SIZE 2

static const UINT32 NPCOEFF = DYNAMIC_COMPRESS_ORDER - 1;

#define inPixBoost_POINTS 100

#define Gain_Curve_Slice_BITS 3

//SEI message semantics for ST 2094 - 40
typedef struct __Dynamic_MetaData__
{
  UINT32 country_code;                       //The value shall be 0xB5
  UINT32 terminal_provider_code;             //The value shall be 0x003C
  UINT32 terminal_provider_oriented_code;    //ST 2094-40 shall be 0x0001
  UINT32 application_identifier;             //Application_identifier shall be set to 4
  UINT32 application_version;                //Application_version shall be set to 0.
  UINT32 num_windows;
  UINT32 targeted_system_display_maximum_luminance;
  UINT32 targeted_system_display_actual_peak_luminance_flag;//=0
  UINT32 maxscl[3];

  UINT32 average_maxrgb;
  UINT32 num_distribution_maxrgb_percentiles;
  UINT32 distribution_maxrgb_percentages[10]; // 1%, 5%, 10%, 25%, 50%, 75%, 90%, 95%, 98%, 99%
  UINT32 distribution_maxrgb_percentiles[10];
  UINT32 fraction_bright_pixels; //N.A.
  UINT32 mastering_display_actual_peak_luminance_flag; //N.A.
  UINT32 tone_mapping_flag;
  UINT32 knee_point_x;
  UINT32 knee_point_y;
  UINT32 num_bezier_curve_anchors; //2~15
  UINT32 bezier_curve_anchors[15];
  UINT32 color_saturation_mapping_flag; //=0
  UINT32 color_saturation_weight; //N.A.

} __Dynamic_MetaData__;

typedef struct __Dynamic_MetaData_Float__
{
  int frame;

  //Luminance Parameters
  //const bool isIndexInRange(const int idx) { return percentileOrder && idx >= 0 && idx < percentileOrder; }
  double avgRGBLuminance;
  double maxRLuminance;
  double maxGLuminance;
  double maxBLuminance;
  int percentileOrder;
  double   Percentile[15];
  int      Percentage[15];

  //EBZCurveParameters
  //const bool isIndexInRange(const int idx) const  { return order && idx >= 0 && idx < (order - 1); }
  int order;
  double KPx;
  double KPy;
  double PCoeff[14];

  //Scene2094Metadata
  float    toneMappingReferenceMonitorPeak;


} __Dynamic_MetaData_Float__;

typedef struct __Double_Type__
{
  INT32 exp;
  UINT64 mantissa;
  INT32 m_bit;
}__Double_Type__;

#pragma pack(1)
typedef struct HDR10PLUS_FWReg
{
  UINT32 DebugFlags;
  UINT32 Pmax;
  UINT32 Pmin;
  UINT32 wa_T;
  UINT32 wb_T;
  UINT32 wa_Pmin_T;
  UINT32 wb_Pmin_T;
  UINT32 ForceProfileA;
  UINT32 Norm_Coeff_Multi;        // Norm = (Norm * Norm_Coeff_Multi) / Norm_Coeff_Divide
  UINT32 Norm_Coeff_Divide;
  UINT32 Gain_Curve_No;

  UINT32 BlendingRatio;

  //Tone Mapping Slope (adjust curve linear slope)
  UINT32 TMS_MIN;
  UINT32 TMS_MAX;
  UINT32 TMS_LB;
  UINT32 TMS_UB;

  //Linear and Bezier Curve Blending (CB)
  UINT32 CB_MAX;
  UINT32 CB_MIN;
  UINT32 CB_LB;
  UINT32 CB_UB;

  //Gain Curve Level Adjustment. (GCL)
  UINT32 GCL_MIN;
  UINT32 GCL_MAX;
  UINT32 GCL_LB;
  UINT32 GCL_UB;

  //p1 Condition
  UINT32 P1_MAX1;
  UINT32 P1_MIN1;
  UINT32 P1_MIN2;
  UINT32 P1_MAX2;
  UINT32 P1_LB;
  UINT32 P1_UB;

  //p2 Condition
  UINT32 P2_MIN1;
  UINT32 P2_MIN2;
  UINT32 P2_MAX2;
  UINT32 P2_LB;
  UINT32 P2_UB;

  //P2toP9 Global Gain Curve Adustment
  UINT32 GGC_MIN;
  UINT32 GGC_MAX;
  UINT32 GGC_LB;
  UINT32 GGC_UB;

  // Defined relative shape bounds (P2~P9 coefficient)
  UINT32 P2toP9_MAX[DYNAMIC_COMPRESS_ORDER - 2];
  UINT32 P2toP9_MIN[DYNAMIC_COMPRESS_ORDER - 2];

  // Thresholds to compute relative shape of curve (P2~P9 coefficient)
  UINT32 P2toP9_LB;
  UINT32 P2toP9_UB;

  // Adjust bezier linear curve of order 10
  UINT32 P1toP9_Linear[9];

  //Profile B: weight of reference curve
  UINT32 refCurve_Weight;

  UINT32 panel_nits;

  //Smooth
  int low_flicker_mode_en;
  int Norm_Change_Step;
  int norm_chase_converge_period;
  int chase_gap;
  int chase_max_speed;
  int Curve_Change_Step;
  int curve_chase_converge_period;
  int curve_chase_max_speed;

  // HDR10+ Adaptive
  UINT32 hdr10plus_adaptive_en;
  float hdr10plus_adaptive_param[5];
  float hdr10plus_adaptive_inPixGm;
  float hdr10plus_adaptive_lighten_speed;
  float hdr10plus_adaptive_darken_speed;
  float hdr10plus_adaptive_ALI_range;

  // 3x3 gamut_matrix
  UINT32 hdr10plus_c00;
  UINT32 hdr10plus_c01;
  UINT32 hdr10plus_c02;
  UINT32 hdr10plus_c10;
  UINT32 hdr10plus_c11;
  UINT32 hdr10plus_c12;
  UINT32 hdr10plus_c20;
  UINT32 hdr10plus_c21;
  UINT32 hdr10plus_c22;

  // Reserved space for future register expansion
  UINT32 Reserved_Array[81];
}HDR10PLUS_FWReg;

typedef struct HDR10PLUS_custXMLBuf{
  unsigned int Size;//Byte
  HDR10PLUS_FWReg iHDR10plusRegEntry[HDR10plus_TBL_SIZE];
  unsigned int CheckCode;
  unsigned int checksum;
}HDR10PLUS_custXMLBuf;
#pragma pack()
typedef struct HDR10PLUS_FWMemberVariable
{

    UINT32 srcMaxL;
    UINT32 srcTgtMaxL;
    UINT32 normMaxL;
    UINT32 percentile50;                    //percentile at 50%
    UINT32 percentile99;                    //percentile at 99%
    UINT32 tgtL;

    //Linear curve table for Bezier formula,
    UINT32 BezierOrder_10[10];
    UINT32 preBezierOrder[DYNAMIC_COMPRESS_ORDER];

    //Tone Mapping Slope (adjust curve linear slope) (TMS)
    UINT32 TMS_MIN;
    UINT32 TMS_MAX;
    UINT32 TMS_LB;
    UINT32 TMS_UB;
    INT32 preNorm;
    BOOL temp_tone_mapping_flag;

    //PreComputed Linear Bezier Table
    UINT32 Bezier_Linear[16][16];

    //PreComputed inPixLuminance
    float inPixLuminance_LUT[Gain_Curve_POINTS];

    //Early stopping flag for linear mapping
    BOOL linear_mapping_flag;
    BOOL linearGainCurve_init_flag;
    BOOL previous_curve_linear_flag;

    //Early termination flag for normal mapping
    BOOL current_curve_update_flag;
    BOOL normalGainCurve_init_flag;

    //HDR10+ Adaptive
    float hdr10p_adaptive_strength;
    float hdr10p_adaptive_sectionFactor[5];
    float hdr10p_inPixBoost_LUT[inPixBoost_POINTS];
    UINT32 hdr10p_adaptive_linear_anchors[9];
    UINT32 currALI;

    //GainCurve Generation
    UINT32 GainCurveGenCycle;

} HDR10PLUS_FWMemberVariable;


// =================== feedback parameters per frame ================== //
typedef struct HDR10PLUS_FWFeedbackMemberVariable
{
    //struct HDR10PLUS_FWMemberVariable pPreHDR10PLUS_FWMV;
    UINT32 preBezierOrder[DYNAMIC_COMPRESS_ORDER];
    INT32 preNorm;
    struct __Dynamic_MetaData__ PreDynamicMetadta;

    // HDR10+ lastFrame Calculated Info
    UINT32 pre_FW_knee_point_x;
    UINT32 pre_FW_knee_point_y;
    UINT32 pre_FW_bezier_anchors[9];

    // HDR10+ Adaptive
    float pre_hdr10p_adaptive_strength;
    UINT32 previousALI; // Previous input ALI information.

} HDR10PLUS_FWFeedbackMemberVariable;


extern struct __Dynamic_MetaData__ HDR10PLUS_Frame_Info;

#ifndef __MAIN_PATH_ONLY__
extern HDR10PLUS_FWReg *pHDR10PLUS_FWReg_SUB;
extern struct __Dynamic_MetaData__ HDR10PLUS_Frame_Info_SUB;
#endif

#ifdef __Local_Sim__
    void read_dynamic_metadata(INT32 frame_no, struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);
#endif

class CPQHDR10PLUSFW
{

private:
    float getNormalizeNits(IN float sceneSourceMaxL, IN float PanelNits);
    void  st2094_40_setToneMappingCurve(DHDROUTPUT *output);
    float st2094_40_ToneMappingCurveGen(IN const float inPixLuminance);

    int MetadataAnalysis();
    void init_HDR10PLUS_FBMV(struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);
    void setting_by_feedback_10plus(struct __Dynamic_MetaData__ *pPreDynamicMetadta, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV, struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);
    void feedback_10plus_setting(struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);
    void setting_by_SWReg(DHDRINPUT *input, DHDROUTPUT *output, struct HDR10PLUS_FWReg *pHDR10PLUS_FWReg);
    void metaData_filtering(struct __Dynamic_MetaData__ *lastFrameInfo, struct __Dynamic_MetaData__ *currFrameInfo);

    UINT32 initFlag;

    // HDR10+ Adaptive Ver1.0.10
    void hdr10plus_adaptive_paramCalc(IN const DHDRINPUT *input, IN const struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV, OUT struct HDR10PLUS_FWMemberVariable *pHDR10PLUS_FWMV);
    void hdr10p_adaptive_strengthCalc(IN const UINT32 inputALI, IN const struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV, OUT struct HDR10PLUS_FWMemberVariable *pHDR10PLUS_FWMV);
    float ALI_2_strength(IN const UINT32 inputALI);
    void hdr10p_adaptive_strength_smoothing(IN const UINT32 currALI, IN const struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV, OUT float *output_strength);
    void hdr10p_adaptive_sectionParamCalc(IN const struct __Dynamic_MetaData__ *pHDR10PLUS_Frame_Info, IN const struct HDR10PLUS_FWReg *pHDR10PLUS_FWReg, OUT struct HDR10PLUS_FWMemberVariable *pHDR10PLUS_FWMV);
    void hdr10p_adaptive_inPixBoostCalc(struct HDR10PLUS_FWMemberVariable *pHDR10PLUS_FWMV);
    void hdr10p_adaptive_linearCurveCalc(UINT32 *bezier_linear_anchors);
    void adaptive_TMS_adjust(UINT32 *Norm);
    void adaptive_color_adjust(DHDROUTPUT *output);
    UINT32 _Set2sCompBlend(UINT32 v1, UINT32 v2, UINT32 blendingRatio, UINT32 bits);

public:
    void getLogState();

    void onLoadXML(unsigned char* setting);
    void Application_Selector(struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);
    void XMLApplication_Selector(IN APPLICATION_E Application, struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info, OUT int *pHDR10plus_sel);
    void custParamMapping(int HDR10plus_sel);
    void XMLCustParamMapping(int *HDR10plus_sel);
    HDR10PLUS_FWReg  iHDR10plusRegEntry[HDR10plus_TBL_SIZE];
    void initHDR10plusRegEntry();

    float Knee_point_smoothFrameCnt;
    float Knee_point_smoothingRatio;
    //int Norm_smoothFrameCnt;
    //float Norm_smoothingRatio;
    void initHDRShareMemory(struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);

    //void DrvHDR10Plus_onInitCommon();
    void DrvHDR10Plus_onInitCommon(struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);
    void DrvHDR10Plus_onCalculate(DHDRINPUT *input, DHDROUTPUT *output, struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info,
                                   struct HDR10PLUS_FWReg *pHDR10PLUS_FWReg, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);

    void tone_mapping_curve_adjust(DHDRINPUT *input, struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info, struct HDR10PLUS_FWReg *pHDR10PLUS_FWReg, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);
    void set_tone_mapping(DHDRINPUT *input, DHDROUTPUT *output, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV);// , struct __Dynamic_MetaData__ HDR10PLUS_Frame_Info);
    void setHDR10plusFWFWMemberVariable(HDRFWMemberVariable* pHDRFWMV);
    void initReg();
    void setHDR10PLUS_FWFeedbackMemberVariable(struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV_lastFrame, struct HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV_currentFrame);
    void DumpRegToFile(FILE *fb, __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);

    void HDR10plus_BezierCurveCoeff_Gen();
    void HDR10plus_EBZCurveParameters_SetValue(UINT32 sx, UINT32 sy, UINT32 * ps, UINT32 order);
    void initDynamicMetadata(struct __Dynamic_MetaData__ *HDR10PLUS_Frame_Info);
    void onInitPlatform(char *tableName);

    void MetadataDynamic(__Dynamic_MetaData__ *metadata_array, int Panel_Nit); // E2E HDR pofile B generation function


    float applyCurve(IN float data, IN struct __Dynamic_MetaData__  param);
    UINT32  HDR10plus_calcP1(UINT32 sx, UINT32 sy, UINT32 tgtL, UINT32  *p1_red_gain);
    UINT32 HDR10plus_rampWeight(IN UINT32  v1, IN UINT32  v2, IN UINT32  t1, IN UINT32  t2, IN UINT32  t);

    __Double_Type__ double_mul_compute(__Double_Type__ coef_int, UINT64 factor);
    __Double_Type__ double_div_compute(__Double_Type__ coef_int, UINT64 factor);
    __Double_Type__ double_add_compute(__Double_Type__ sum_int, __Double_Type__ coef_int);
    __Double_Type__ double_mul_compute_simplify(__Double_Type__ coef_int, UINT64 factor, INT32 factor_bit);
    __Double_Type__ double_div_compute_simplify(__Double_Type__ coef_int, UINT64 factor, INT32 factor_bit);
    __Double_Type__ double_mul_div_compute_simplify(__Double_Type__ coef_int, UINT64 factor_mul, INT32 factor_mul_bit, UINT64 factor_div, INT32 factor_div_bit);

    //HDRFWReg *pHDRFWReg; //copy fw_csr to *pvHDRPQINFO and point by pDynamicHDR->pHDRFWRe;

    PQLoggingMechanism *pLogMech;
    enum log_tag_id
    {
      HDREntryFlag = 0,
      HDRInputDHDRINPUT_HDRFWReg,
      HDRInputFirmwareMemberVariable,
      HDROutputDHDROUTPUT,
      HDROutputGainCurve,
      HDRTime,
      HDRContentFrame,
      HDRContentStream,
      log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];

    __Dynamic_MetaData__ HDR10PLUS_Frame_Info;
    __Dynamic_MetaData__ *pHDR10PLUS_Frame_Info;
    HDR10PLUS_FWReg vHDR10PLUS_FWReg;
    HDR10PLUS_FWReg *pHDR10PLUS_FWReg;

    HDRFWMemberVariable *pHDRFWMV;
    HDRFWMemberVariable vHDRFWMV;

    HDR10PLUS_FWMemberVariable HDR10PLUS_FWMV;
    HDR10PLUS_FWMemberVariable *pHDR10PLUS_FWMV;

    __Dynamic_MetaData__* pPreDynamicMetadta;
    __Dynamic_MetaData__  PreDynamicMetadta;

    HDR10PLUS_FWFeedbackMemberVariable *pHDR10PLUS_FBMV;
    HDR10PLUS_FWFeedbackMemberVariable HDR10PLUS_FBMV;

    UINT32 RGBmaxHistogram_1_ori[hist_bins];

    CPQHDR10PLUSFW()
    {
        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
        "HDREntryFlag",
        "HDRInputDHDRINPUT_HDRFWReg",
        "HDRInputFirmwareMemberVariable",
        "HDROutputDHDROUTPUT",
        "HDROutputGainCurve",
        "HDRTime",
        "HDRContentFrame",
        "HDRContentStream"
        };

        for (int i = 0; i < log_tag_len; ++i){
          log_state_list[i] = ANDROID_LOG_DISABLE;
        }

        pHDR10PLUS_Frame_Info = &HDR10PLUS_Frame_Info;
        pHDRFWMV = &vHDRFWMV;
        pHDR10PLUS_FWMV = &HDR10PLUS_FWMV;
        pHDR10PLUS_FWReg = &vHDR10PLUS_FWReg;
        pPreDynamicMetadta = &PreDynamicMetadta;
        pHDR10PLUS_FBMV = &HDR10PLUS_FBMV;
        Knee_point_smoothFrameCnt = 0;
        Knee_point_smoothingRatio = 0;
        //Norm_smoothFrameCnt = 0;
        //Norm_smoothingRatio = 0;
        initReg();
        initHDR10plusRegEntry();
        //DrvHDR10Plus_onInitCommon();
        initFlag = 0;

    }

    ~CPQHDR10PLUSFW();

};
#ifdef __cplusplus
}
#endif

#ifdef __Local_Sim__
    extern FILE *kxky_fp;
    extern FILE *errfp;
#endif

#endif