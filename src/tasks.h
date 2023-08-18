#pragma once

#include <stdint.h>

extern "C" {

void* ISPCAlloc(void** handlePtr, int64_t size, int32_t alignment);

void ISPCLaunch(void** handlePtr,
                void* f,
                void* data,
                int count0,
                int count1,
                int count2);

void ISPCSync(void* handle);

}  // extern "C"
