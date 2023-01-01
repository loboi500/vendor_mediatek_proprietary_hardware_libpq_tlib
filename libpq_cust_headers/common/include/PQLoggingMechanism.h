#ifndef PQLOGGINGMECHANISM_H
#define PQLOGGINGMECHANISM_H

#include <string>

//#define LogMech_LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "PQLogMech", "%s: " fmt, __FUNCTION__, ##__VA_ARGS__)
#define ANDROID_LOG_DISABLE 100 // Bigger than log_level

class PQLoggingMechanism
{
    public:
        PQLoggingMechanism() = default;
        PQLoggingMechanism(int len);
        ~PQLoggingMechanism(){};

        void logStateUpdate(char**, int*);

    private:
        int getLogLevel(const char*);
        int getPropertyLevel(std::string);

        int log_tag_len;

};

#endif
