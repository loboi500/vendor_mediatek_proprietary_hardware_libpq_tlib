/* This file must synchronize 2 places(libpq/libaal) simultaneously */
#ifndef __PQSHAREDDATA_H__
#define __PQSHAREDDATA_H__

#define PQSHAREDDATA_VERSION 202204202100

#include <pthread.h>

/* DRE */
struct PQAlgDreSharedData {
    int currALI;
    int DREALIThresholdVal;
    bool dre_frm_trigger;
    bool dre_en;
};

/* for sync data between AAL service & PQ service */
class PQAlgSharedData {
private:
public:
    PQAlgSharedData()
    {
        pthread_rwlock_init(&dre_rwlock, NULL);
        dre_data.dre_en = 0;
    }
    ~PQAlgSharedData()
    {
        pthread_rwlock_destroy(&dre_rwlock);
    }
    /* DRE */
    struct PQAlgDreSharedData dre_data = {};
    pthread_rwlock_t dre_rwlock;
};

#endif
/* This file must synchronize 2 places(libpq/libaal) simultaneously */
