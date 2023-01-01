#ifndef __HDR_VIVID_FW__
#define __HDR_VIVID_FW__

#ifdef __cplusplus
extern "C"
{
#endif

#include "PQHDRImpl.h"

//#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define HDRVivid_TBL_SIZE 2
#define HDRVivid_LOGD(fp, fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "PQ_HDRVivid", "%s: " fmt, __FUNCTION__, ##__VA_ARGS__ )
//******************************************************************************
// Basic Definitions
//******************************************************************************
#define HDRVivid_FW_MAX_LUMINANCE 4000
#define CUVA_PQ_MAX_LUMINANCE 10000
#define DEFAULT_MAX_DISPLAY_LUMINANCE                     810
#define SDR_ENABLED                               1
#if SDR_ENABLED
#define DEFAULT_MAX_DISPLAY_LUMINANCE_SDR                810
#endif

typedef unsigned short      uint16;
typedef short               int16;
typedef long                int32;
typedef long long           int64;

typedef unsigned int UINT32;
//typedef int INT32;
//typedef unsigned long long UINT64;
//typedef int BOOL;

#define TONE_MAPPING_NUM 2 // i max case 2
#define SPLINE3_NUM 4 // j max case 4
#define COLOR_SAT_GAIN_NUM 16 // k max case 16


//******************************************************************************
// HDR Vivid Definitions
//******************************************************************************
#define system_start_code_BIT        8
#define minimum_maxrgb_BIT           12
#define average_maxrgb_BIT           12
#define variance_maxrgb_BIT          12
#define maximum_maxrgb_BIT           12
#define tone_mapping_mode_BIT        1
#define tone_mapping_param_num_BIT   1
#define targeted_system_display_BIT  12
#define Base_flag_BIT                1
#define Base_param_m_p_BIT           14
#define Base_param_m_m_BIT           6
#define Base_param_m_a_BIT           10
#define Base_param_m_b_BIT           10
#define Base_param_m_n_BIT           6
#define Base_param_K1_BIT            2
#define Base_param_K2_BIT            2
#define Base_param_K3_BIT            4
#define Base_param_Delta_mode_BIT    3
#define Base_param_Delta_BIT         7
#define P3Spline_flag_BIT            1
#define P3Spline_num_BIT             1
#define P3Spline_TH_mode_BIT         2
#define P3Spline_TH_MB_BIT           8
#define P3Spline_TH_OFFSET_BIT       2
#define P3Spline_TH1_BIT             12
#define P3Spline_TH2_BIT             10
#define P3Spline_TH3_BIT             10
#define P3Spline_Strength_BIT        8
#define color_saturation_BIT         1
#define color_saturation_num_BIT     3
#define color_saturation_gain_BIT    8
#define CUVAHDR_METADATA_BASE_S      13
#define CUVAHDR_METADATA_BASE_B      10
#define TPA_NUM      4

    typedef struct CuvaCurve
    {
        double m_p;
        double m_m;
        double m_a;
        double m_b;
        double m_n;
        double K1;
        double K2;
        double K3;
        int curve_mintiao;
        double TH1, TH2, TH3;
        double md1, mc1, mb1, ma1;
        double md2, mc2, mb2, ma2;
        double DARKcurble_S1;
        double DARKcurble_offset;
        int curve_mintiao_high_area;
        double TH1_HIGH, TH2_HIGH, TH3_HIGH;
        double md1_high, mc1_high, mb1_high, ma1_high;
        double md2_high, mc2_high, mb2_high, ma2_high;
        int high_area_flag;
        double curve_adjust;
        double m_p_T;
        double m_a_T;
    }CuvaCurve;

typedef struct __HDRVIVID_Dynamic_MetaData__
{
    UINT32 country_code;
    UINT32 terminal_provider_code;
    UINT32 terminal_provider_oriented_code;

    UINT32 system_start_code;
    // Subset 1: information of data statistic (4 param)
    UINT32 minimum_maxrgb;
    UINT32 average_maxrgb;
    UINT32 variance_maxrgb;
    UINT32 maximum_maxrgb;
    // End of Subset 1

    UINT32 tone_mapping_mode;
    UINT32 tone_mapping_param_num;
    UINT32 targeted_system_display_maximum_luminance[2]; // value = 2080 for SDR mode
    UINT32 Base_flag[4];

    // Subset 2: base curve parameters(8 param) + 3Spline curve parameters(6 param)
    // base curve parameters (8 parameters)
    UINT32 Base_param_m_p[2];
    UINT32 Base_param_m_m[2];
    UINT32 Base_param_m_a[2];
    UINT32 Base_param_m_b[2];
    UINT32 Base_param_m_n[2];
    UINT32 Base_param_K1[2];
    UINT32 Base_param_K2[2];
    UINT32 Base_param_K3[2];

    UINT32 base_param_Delta_mode[2];
    UINT32 base_param_Delta[2];
    UINT32 P3Spline_flag[2];
    UINT32 P3Spline_num[2];

    // 3Spline curve parameters (4 parameters)
    UINT32 P3Spline_TH_mode[2][4];
    UINT32 P3Spline_TH_MB[2][4];
    UINT32 P3Spline_TH[2][4][3];
    UINT32 P3Spline_Strength[2][4];

    // color adjustment (3 parameters)
    UINT32 color_saturation_mapping_flag;
    UINT32 color_saturation_num;
    UINT32 color_saturation_gain[16];

} __HDRVIVID_Dynamic_MetaData__;

typedef struct __HDRVIVID_Dynamic_MetaData_Parsed__
{
        double country_code;
        double terminal_provider_code;
        double terminal_provider_oriented_code;

        // Each value should be divided by unit
        double system_start_code;
        // Subset 1: information of data statistic (4 param)
        double minimum_maxrgb; // unit: 4095
        double average_maxrgb; // unit: 4095
        double variance_maxrgb; // unit: 4095
        double maximum_maxrgb; // unit: 4095
                               // End of Subset 1

        double tone_mapping_mode; // unit: 1
        double tone_mapping_param_num; // unit: 1
        double targeted_system_display_maximum_luminance[2];  // unit: 4095
        double Base_flag[4]; // unit: 1

                             // Subset 2: base curve parameters(8 param) + 3Spline curve parameters(7 param)
                             // base curve parameters (8 parameters)
        double m_p_0[2]; // unit: 16383/10
        double m_m_0[2]; // unit: 10
        double m_a_0[2]; // unit: 1023
        double m_b_0[2]; // unit: 1023/0.25
        double m_n_0[2]; // unit: 10
        double K1_0[2]; // unit: 1
        double K2_0[2]; // unit: 1
        double K3_0[2]; // unit: 1

        double base_param_Delta_mode[2]; // unit: 1
        double base_param_Delta[2]; // unit: 127
        double P3Spline_flag[2]; // unit: 1
        double P3Spline_num[2]; // unit: 1

                                               // 3Spline curve parameters (7 parameters)
        double P3Spline_TH_mode[2][4]; // unit: 1
        double P3Spline_TH_MB[2][4]; // When Spline3_TH_mode = 0, Spline3_TH_MB = Spline3_TH_enable_MB[8:2] unit: 63. When Spline3_TH_mode = 1, unit: 255/1.1
        //double base_offset[2][SPLINE3_NUM]; // When Spline3_TH_mode = 0, base_offset = Spline3_TH_enable_MB[1:0] unit: 3/0.1
        double P3Spline_TH[2][4][3]; // unit: 4095
                                                              //double P3Spline_TH_Delta1[TONE_MAPPING_NUM][SPLINE3_NUM]; // unit: 1023/0.25
                                                              //double P3Spline_TH_Delta2[TONE_MAPPING_NUM][SPLINE3_NUM]; // unit: 1023/0.25
        double P3Spline_Strength[2][4]; // unit: 127 // default write by ALGO 0

                                                                 // color adjustment (3 parameters)
        double color_saturation_mapping_flag; // unit: 1
        double color_saturation_num; // unit: 1
        double color_saturation_gain[16]; // unit: 128


} __HDRVIVID_Dynamic_MetaData_Parsed__;


typedef struct HDRVIVID_FWReg
{
    UINT32 max_display_luminance;   // [0, 10000]
    UINT32 output_luminance_norm;   // [0, 10000]
    UINT32 global_gain_numerator;   // [0, 4095]
    UINT32 global_gain_denominator; // [0, 4095]
    UINT32 OETF_index; // [0: BT1886, 1: sRGB, 2: BT709]
    UINT32 EOTF_index; // [0: ST2084, 1: HLG, 2: SDR, 3: sRGB]
    UINT32 gamut_index; // see GAMUT enum
}HDRVIVID_FWReg;

// =================== feedback parameters per frame ================== //
typedef struct HDRVIVID_FWFeedbackMemberVariable
{
    // TBD
    struct __HDRVIVID_Dynamic_MetaData_Parsed__ PreDynamicMetadta;
    UINT32 normMaxL;
    UINT32 srcTgtMaxL;
    UINT32 targeted_system_display_linear;

} HDRVIVID_FWFeedbackMemberVariable;




class CPQHDRVIVIDFW
{
private:

    double GainCoeff[1024];
    double m_p;
    double m_m;
    double m_a;
    double m_b;
    double m_n;
    double K1;
    double K2;
    double K3;
    double DARKcurble_S1;
    double DARKcurble_offset;
    unsigned int base_param_Delta_mode;

    double m_maxEtemp_store;
    double m_inputMaxEtemp_store;
    double m_maxE;
    double m_inputMaxE;
    double m_minE;
    double m_inputMinE;

    double maximum_maxrgb_noLine;
    double minimum_maxrgb_noLine;
    double average_maxrgb_noLine;
    double variance_maxrgb_noLine;

    double TH1, TH2, TH3;
    double TH1_HIGH, TH2_HIGH, TH3_HIGH;
    double md1, mc1, mb1, ma1;
    double md1_high, mc1_high, mb1_high, ma1_high;
    double md2, mc2, mb2, ma2;
    double md2_high, mc2_high, mb2_high, ma2_high;
    bool curve_mintiao;
    bool curve_mintiao_high_area;
    int high_area_flag;

    double masterDisplaySet;

    __HDRVIVID_Dynamic_MetaData__ m_metadata;
    __VDEC_DRV_COLORDESC_T m_staticMetadata;
    double TML;
    double TML_linear;
    double RML;
    double RML_linear;

    void parse_dynamic_metadata(struct __HDRVIVID_Dynamic_MetaData__ *HDRVIVID_Frame_Ori_Info, struct __HDRVIVID_Dynamic_MetaData_Parsed__ *HDRVIVID_Frame_Info);

    void setting_by_feedbackMV(DHDRINPUT *input, DHDROUTPUT *output, struct __HDRVIVID_Dynamic_MetaData_Parsed__ *pPreDynamicMetadta, struct HDRVIVID_FWFeedbackMemberVariable *pHDRVIVID_FBMV, struct __HDRVIVID_Dynamic_MetaData_Parsed__ *HDRVIVID_Frame_Info);
    void feedbackMV_setting(DHDRINPUT *input, DHDROUTPUT *output, struct __HDRVIVID_Dynamic_MetaData_Parsed__ *HDRVIVID_Frame_Info, struct HDRVIVID_FWFeedbackMemberVariable *pHDRVIVID_FBMV);

    bool InitCUVAParams(const double max_display_luminance, const __HDRVIVID_Dynamic_MetaData__* metadata, const double MasterDisplay);
    double low_area_spline(double maximum_maxrgb,
        double average_maxrgb,
        double tone_mapping_param_m_p,
        double tone_mapping_param_m_m,
        double tone_mapping_param_m_a,
        double tone_mapping_param_m_b,
        double tone_mapping_param_m_n,
        double tone_mapping_param_K1,
        double tone_mapping_param_K2,
        double tone_mapping_param_K3,
        double P3Spline_TH_MB,
        double P3Spline_TH[3],
        double P3Spline_Strength,
        double maxDisplay,
        double *md1g,
        double *mc1g,
        double *mb1g,
        double *ma1g,
        double *md2g,
        double *mc2g,
        double *mb2g,
        double *ma2g,
        double *dark,
        double *DARKcurble_offset,
        int *curve_mintiao,
        double *m_a,
        unsigned int base_param_Delta_mode,
        unsigned int Base_flag);

    double spline_area_spec(double maximum_maxrgb,
        double average_maxrgb,
        double tone_mapping_param_m_p,
        double tone_mapping_param_m_m,
        double tone_mapping_param_m_a,
        double *tone_mapping_param_m_b,
        double tone_mapping_param_m_n,
        double tone_mapping_param_K1,
        double tone_mapping_param_K2,
        double tone_mapping_param_K3,
        double P3Spline_TH_MB,
        double P3Spline_TH[3],
        double P3Spline_Strength,
        double maxDisplay,
        double *md1g,
        double *mc1g,
        double *mb1g,
        double *ma1g,
        double *md2g,
        double *mc2g,
        double *mb2g,
        double *ma2g,
        double *dark,
        double *DARKcurble_offset,
        int *curve_mintiao,
        unsigned int base_param_Delta_mode);

    double spline_higharea_spec(double maximum_maxrgb,
        double average_maxrgb,
        double tone_mapping_param_m_p,
        double tone_mapping_param_m_m,
        double tone_mapping_param_m_a,
        double tone_mapping_param_m_b,
        double tone_mapping_param_m_n,
        double tone_mapping_param_K1,
        double tone_mapping_param_K2,
        double tone_mapping_param_K3,
        int   P3Spline_TH_Mode,
        double P3Spline_TH_MB,
        double P3Spline_TH[3],
        double P3Spline_Strength,
        double maxDisplay,
        double *md1g,
        double *mc1g,
        double *mb1g,
        double *ma1g,
        double *md2g,
        double *mc2g,
        double *mb2g,
        double *ma2g,
        double *dark,
        int *curve_mintiao_high_area,
        double Referncedisplay,
        unsigned int base_param_Delta_mode);

    double getCuvaBaseParameterAdjust(CuvaCurve *curve);

    double getCuvaBaseParameter(const __HDRVIVID_Dynamic_MetaData__ *metadata,
        double MasterDisplay,
        double MaxDisplay,
        double MinDisplay,
        CuvaCurve *curve,
        double *maxE,
        double *inputE);

    void AdjustCuvaParameter(double m_maxE, double m_inputMaxE, CuvaCurve *curve);
    void cuva_hdr_get_3spline(const __HDRVIVID_Dynamic_MetaData__ *metadata, double m_maxE, double m_inputMaxE, CuvaCurve *curve);
    void calculateLuminanceGainParams(const DHDRINPUT *input, DHDROUTPUT *output);
    double calc_curve(double max, CuvaCurve *TMP);
    INT32 _set2sCompNum(INT32 val, INT32 bits);
    void calculateToneMappingParams(const DHDRINPUT *input, DHDROUTPUT *output);
    void calculateCUVACurve(const DHDRINPUT *input, DHDROUTPUT *output, const __HDRVIVID_Dynamic_MetaData__ *metadata);
    void updateConfigure(const DHDRINPUT *input, DHDROUTPUT *output);
    void logMechanism(); // for Android log
    PQLoggingMechanism *pLogMech; // for Android log

    enum log_tag_id
    {
        HDRVividEntryFlag = 0,
        HDRVividInputHDRVIVIDFWReg,
        HDRVividInputFirewareMemberVariable,
        HDRVividOutputHDRVIVIDINPUT,
        HDRVividOutputHDRVIVIDOUTPUT,
        HDRVividOutputGainCurve,
        HDRVividContentFrame,
        HDRVividContentStream,
        log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];


public:
    void initHDRVIVIDShareMemory(struct HDRVIVID_FWFeedbackMemberVariable *pHDRVIVID_FBMV);

    void setHDRVIVID_FWMemberVariable(IN HDRFWMemberVariable* inHDRFWMV);
    void initReg();
    void initHDRVividRegEntry();
    void initDynamicMetadata(struct __HDRVIVID_Dynamic_MetaData__ *HDRVIVID_Frame_Info);
    void onInitCommon(struct __HDRVIVID_Dynamic_MetaData__ *HDRVIVID_Frame_Info, DHDRINPUT *input, DHDROUTPUT *output); // init VIVID_FWReg
    void onInitPlatform(char *tableName);

    void Application_Selector(IN APPLICATION_E Application, IN struct __HDRVIVID_Dynamic_MetaData__ *HDRVIVID_Frame_Info, int *HDR_Vivid_Sel);
    void custParamMapping(int *HDR_Vivid_Sel);
    HDRVIVID_FWReg iHDRVividRegEntry[HDRVivid_TBL_SIZE];

    void onCalculate(DHDRINPUT *input, DHDROUTPUT *output, struct __HDRVIVID_Dynamic_MetaData__ *HDRVIVID_Frame_Info, struct HDRVIVID_FWReg *pHDRVIVID_FWReg, struct HDRVIVID_FWFeedbackMemberVariable *pHDRVIVID_FBMV); // VIVID HDR main

    __HDRVIVID_Dynamic_MetaData__ vCuvaMetadata;
    __HDRVIVID_Dynamic_MetaData__ *pCuvaMetadata;
    HDRVIVID_FWReg *pHDRVIVID_FWReg;
    HDRVIVID_FWReg vHDRVIVID_FWReg;
    HDRVIVID_FWFeedbackMemberVariable *pHDRVIVID_FWMV;
    HDRVIVID_FWFeedbackMemberVariable HDRVIVID_FWMV;
    HDRFWMemberVariable *pHDRFWMV;
    HDRFWMemberVariable vHDRFWMV;

    CPQHDRVIVIDFW()
    {
        pCuvaMetadata        = &vCuvaMetadata;
        pHDRVIVID_FWMV       = &HDRVIVID_FWMV;
        pHDRVIVID_FWReg      = &vHDRVIVID_FWReg;
        pHDRFWMV = &vHDRFWMV;
        initReg();

        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
            "HDRVividEntryFlag",
            "HDRVividInputHDRVIVIDFWReg",
            "HDRVividInputFirewareMemberVariable",
            "HDRVividOutputHDRVIVIDINPUT",
            "HDRVividOutputHDRVIVIDOUTPUT",
            "HDRVividOutputGainCurve",
            "HDRVividContentFrame",
            "HDRVividContentStream"
        };


        for(int i = 0; i < log_tag_len; ++i)
            log_state_list[i] = ANDROID_LOG_DISABLE;

    }
    ~CPQHDRVIVIDFW();
};










#ifdef __cplusplus
}
#endif

#endif


