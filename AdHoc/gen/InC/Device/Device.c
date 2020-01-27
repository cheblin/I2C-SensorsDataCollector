

#include "Device.h"


Meta m0_DeviceVersion = {0, 0, 0, 0,  4, NULL};
static Pack* meta1alloc(Pack* pack)
{
    static const Pack empty = {.meta = &m1_Stop};
    return (Pack*)&empty;
}
Meta m1_Stop = {1, 0, 0, 0,  0, meta1alloc };
static Pack* meta2alloc(Pack* pack)
{
    static const Pack empty = {.meta = &m2_Start};
    return (Pack*)&empty;
}
Meta m2_Start = {2, 0, 0, 0,  0, meta2alloc };
static Pack* meta3alloc(Pack* pack)
{
    static const Pack empty = {.meta = &m3_GetDeviceVersion};
    return (Pack*)&empty;
}
Meta m3_GetDeviceVersion = {3, 0, 0, 0,  0, meta3alloc };
static Pack* meta4alloc(Pack* pack)
{
    static const Pack empty = {.meta = &m4_GetConfiguration};
    return (Pack*)&empty;
}
Meta m4_GetConfiguration = {4, 0, 0, 0,  0, meta4alloc };
static Pack* meta5alloc(Pack* pack)
{
    static const Pack empty = {.meta = &m5_SetConfiguration};
    return (Pack*)&empty;
}
Meta m5_SetConfiguration = {5, 0, 0, 0,  0, meta5alloc };
Meta m6_BusConfiguration = {6, 0, 0, 0,  5, NULL};
Meta m7_InstructionsPack = {7, 0, 0, 0,  257, NULL};
Meta m8_DeviceError = {8, 0, 0, 0,  3, NULL};
Meta m9_SensorsData = {9, 0, 0, 0,  2000, NULL};

