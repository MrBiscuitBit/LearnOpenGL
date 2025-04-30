#pragma once

#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
    extern int log_enabled;

    #define LOG(...) do{if(log_enabled){printf("[LOG] "); printf(__VA_ARGS__); printf("\n");}}while(0)
    #define WRN(...) do{printf("[WRN] "); printf(__VA_ARGS__); printf("\n");}while(0)
    #define ERR(...) do{printf("[ERR] "); printf(__VA_ARGS__); printf("\n");}while(0)
#else
    #define LOG(...) do{ }while(0)
    #define WRN(...) do{ }while(0)
    #define ERR(...) do{ }while(0)
#endif

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;