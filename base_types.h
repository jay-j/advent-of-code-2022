/* date = October 30th 2022 10:26 pm */
// Just the pure basics

#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

//~ Boring Types

#ifdef __cplusplus
extern "C" {
#endif
    
#define FALSE 0
#define TRUE 1
    
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    
    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;
    
    typedef float f32;
    typedef double f64;
    
    typedef struct string8{
        u8* str;
        u64 size;
    } string8;
    
#define TIME_SEC_TO_MS(x) ((x) * 1000)
#define TIME_SEC_TO_US(x) ((x) * 1000000)
#define TIME_SEC_TO_NS(x) ((x) * 1000000000)
    
#define TIME_NS_TO_SEC(x) ((x) / 1000000000)
#define TIME_NS_TO_MS(x)  ((x) / 1000000)
#define TIME_NS_TO_US(x)  ((x) / 1000)
    
#define STRING8_DIFFERENT 0
#define STRING8_SAME 1
    
    u8 string8_compare(string8 a, string8 b){
        // string comparison function
        // TODO horribly inefficient because it does one byte at a time
        if (a.size != b.size){
            return STRING8_DIFFERENT;
        }
        for (u64 i=0; i<a.size; ++i){
            if (a.str[i] != b.str[i]){
                return STRING8_DIFFERENT;
            }
        }
        return STRING8_SAME;
    }
    
#ifdef __cplusplus
}
#endif

#endif //BASE_TYPES_H
