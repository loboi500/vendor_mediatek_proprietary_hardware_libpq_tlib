#ifndef __ADAPTIVETCC_H__
#define __ADAPTIVETCC_H__

#include "PQTCCImpl.h"

class TAdaptiveTCC
{
private:
    CPQTCCFW *PQTCCFW;
public:
    TAdaptiveTCC(CPQTCCFW *InputPQTCCFW);
    ~TAdaptiveTCC();

    int onCalculate(TCCInput *pTCCInput, TCCOutput *pTCCOutput, const void *ISPInfo, TCC_C2G *C2GMatrix);
    int DumpReg(char* MDPDumpBuffer, int& MDPCount, int MaxMDPSize, TCCInput *pTCCInput, TCCOutput *pTCCOutput);
};
#endif