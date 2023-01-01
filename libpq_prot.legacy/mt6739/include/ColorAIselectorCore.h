#ifndef __COLORAISELECTORCORE_H__
#define __COLORAISELECTORCORE_H__

#define UNUSED_AICOLOR(reserve) (void)(reserve)

#include "PQSDR2HDRImpl.h"

struct ColorAISelInitParam {
    unsigned int reserved; // not used
};

class ColorAISelector {
private:
    // Scene threshold
    int NNScenePortraitTh; // Portrait
    int NNSceneFoodTh; // Food
    int NNSceneSkyTh; // Sky
    int NNSceneGreenTh; // Green
    int NNSceneArchitectureTh; // Architecture
    int NNSceneLabTh; // Lab Scene

    int AALSceneNightTh; // Night
    int AALSceneSnowTh;
    bool Custom_ColorAISelector; // 0: Default; 1: Custom_AIPQ_selector

    void logMechanism(void); // for Android log
    PQLoggingMechanism *pLogMech; // for Android log
    enum log_tag_id
    {
        COLORBasic = 0,
        COLORDetail,
        log_tag_len
    };
    char **log_tag;
    int log_state_list[log_tag_len];
public:

    ColorAISelector()
    {
        // Scene threshold
        NNScenePortraitTh = 500; // Portrait
        NNSceneFoodTh = 500; // Food
        NNSceneSkyTh = 500; // Sky
        NNSceneGreenTh = 500; // Green
        NNSceneArchitectureTh = 500; // Architecture
        NNSceneLabTh = 500; // Lab Scene

        AALSceneNightTh = 500; // Night
        AALSceneSnowTh = 500; // Snow

        Custom_ColorAISelector = 0; // Default

        pLogMech = new PQLoggingMechanism(log_tag_len);
        log_tag = new char*[log_tag_len] {
            "COLORBasic",
            "COLORDetail"
        };

        for(int i = 0; i < log_tag_len; ++i)
            log_state_list[i] = ANDROID_LOG_DISABLE;

    };

    ~ColorAISelector()
    {
        delete pLogMech;
        delete[] log_tag;
    };

    void setWhoSelector(bool who);
    void PQ_selector( IN PQSelectorInput pqSelectorInput, OUT int &color_sel);
    void AIPQ_selector( const float NNSceneProbability[15], const float AALSceneProbability[5], const int wholefbwidth, const int wholefbheight, const int srcwidth, const int srcheight, const int QPsum, int &color_sel );

    // for custom
    void Custom_AIPQ_selector( const float NNSceneProbability[15], const float AALSceneProbability[5], const int wholefbwidth, const int wholefbheight, const int srcwidth, const int srcheight, const int QPsum, int &color_sel )
    {
        // NNSceneProbability[0]: Portrait // [1]: Sky // [2]: Green // [3]: Food // [4]: Architecture
        // AALSceneProbability[0]: Night // [1]: Snow
        // wholefbwidth, wholefbheight: Frame buffer size (fixed size)
        // srcwidth, srcheight: video size (depends on scenario)
        // Qpsum: From Video decoder QP
        // color_sel: Color setting number
        UNUSED_AICOLOR(NNSceneProbability);
        UNUSED_AICOLOR(AALSceneProbability);
        UNUSED_AICOLOR(wholefbwidth);
        UNUSED_AICOLOR(wholefbheight);
        UNUSED_AICOLOR(srcwidth);
        UNUSED_AICOLOR(srcheight);
        UNUSED_AICOLOR(QPsum);


        // output tuning set
        color_sel = 2;

    };

};

#endif
