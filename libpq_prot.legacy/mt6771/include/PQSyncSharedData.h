#ifndef __PQSYNCSHAREDDATA_H__
#define __PQSYNCSHAREDDATA_H__

#define PQSYNCSHAREDDATA_VERSION 202204221000

#include "PQSharedData.h"

class PQAlgSyncData {
private:
    PQAlgSyncData(){}
    ~PQAlgSyncData(){}
public:
    static int SetPQAlgSharedData(class PQAlgSharedData *pqalg_data);
};

#endif
