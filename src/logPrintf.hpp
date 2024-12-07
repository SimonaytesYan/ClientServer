#pragma once

#define DEBUG

#ifdef DEBUG
    #define LOG_PRINTF(...) \
        fprintf(stderr, __VA_ARGS__);
#else
    #define LOG_PRINTF(...) 
#endif
