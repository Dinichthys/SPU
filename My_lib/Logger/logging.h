#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <time.h>

#define LOG(level, ...)                                                                                  \
    do                                                                                                   \
    {                                                                                                    \
        time_t seconds = time (NULL);                                                                    \
        Log ({__FILE__, __func__, __LINE__, localtime(&seconds)}, level, __VA_ARGS__);                   \
    } while (0)

enum LEVEL_LOG
{
    DEBUG   = 1,
    INFO    = 2,
    WARNING = 3,
    ERROR   = 4,
};

struct logging
{
    const char* const file;

    const char* const func;

    int line;

    struct tm *now;
};

void set_log_lvl (const enum LEVEL_LOG level);
void set_log_file (FILE* const file);
void Log (const struct logging parameter, const enum LEVEL_LOG level, const char * const format, ...);

#endif // LOGGING_H
