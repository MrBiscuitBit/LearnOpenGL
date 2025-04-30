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

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;