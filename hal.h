// hal.h - Omege UOS 硬件抽象层接口
#ifndef _HAL_H
#define _HAL_H

typedef enum _RETURN_FIRMWARE {
    HaltFirmware,
    RebootFirmware,
    ShutdownFirmware
} RETURN_FIRMWARE;

VOID HalInitializeProcessor(ULONG ProcessorNumber);
VOID HalReturnToFirmware(RETURN_FIRMWARE Action);
ULONG HalGetProcessorCount(VOID);
PHYSICAL_ADDRESS HalGetMemorySize(VOID);   // 返回物理内存大小

#endif