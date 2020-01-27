
#include "Device.h"
#include <assert.h>
#include <stddef.h>

static size_t     some_size_t    = 0;
static uint16_t     some_uint16_t    = 0;
static uint8_t     some_uint8_t    = 0;


void write_SensorsData(p9_SensorsData * const p9);

void read_SensorsData(p9_SensorsData * const p9);

void write_DeviceError(p8_DeviceError * const p8);

void read_DeviceError(p8_DeviceError * const p8);

void write_InstructionsPack(p7_InstructionsPack * const p7);

void read_InstructionsPack(p7_InstructionsPack * const p7);

void write_BusConfiguration(p6_BusConfiguration * const p6);

void read_BusConfiguration(p6_BusConfiguration * const p6);

void write_DeviceVersion(p0_DeviceVersion * const p0);

void read_DeviceVersion(p0_DeviceVersion * const p0);


void read_DeviceVersion(p0_DeviceVersion * const p0)
{
    uint16_t DevID =  p0_DevID_GET(p0) ;
    uint16_t Ver =  p0_Ver_GET(p0) ;
}



void write_DeviceVersion(p0_DeviceVersion * const p0)
{
    p0_DevID_SET(some_uint16_t, p0);
    p0_Ver_SET(some_uint16_t, p0);
}


void read_BusConfiguration(p6_BusConfiguration * const p6)
{
    uint8_t multiplier =  p6_multiplier_GET(p6) ;
    uint16_t time =  p6_time_GET(p6) ;
    uint16_t clk_khz =  p6_clk_khz_GET(p6) ;
}



void write_BusConfiguration(p6_BusConfiguration * const p6)
{
    p6_multiplier_SET(some_uint8_t, p6);
    p6_time_SET(some_uint16_t, p6);
    p6_clk_khz_SET(some_uint16_t, p6);
}


void read_InstructionsPack(p7_InstructionsPack * const p7)
{
    uint8_t Length =  p7_Length_GET(p7) ;
    V7_Instructions item_Instructions = p7_Instructions_GET(p7);
    for(size_t index = 0; index <  item_Instructions.len; index++)
        some_uint8_t  = v7_Instructions_GET(& item_Instructions, index);
}



void write_InstructionsPack(p7_InstructionsPack * const p7)
{
    p7_Length_SET(some_uint8_t, p7);
    p7_Instructions_SET(& some_uint8_t, p7);
}


void read_DeviceError(p8_DeviceError * const p8)
{
    uint16_t param1 =  p8_param1_GET(p8) ;
    e_Errors error_id =  p8_error_id_GET(p8) ;
}



void write_DeviceError(p8_DeviceError * const p8)
{
    p8_param1_SET(some_uint16_t, p8);
    p8_error_id_SET(e_Errors_SensorsDataOverflow, p8);
}


void read_SensorsData(p9_SensorsData * const p9)
{
    V9_values item_values = p9_values_GET(p9);
    for(size_t index = 0; index <  item_values.len; index++)
        some_uint16_t  = v9_values_GET(& item_values, index);
}



void write_SensorsData(p9_SensorsData * const p9)
{
    p9_values_SET(& some_uint16_t, p9);
}


typedef struct
{
    c_Communication channel;
    RBUF_INIT(Pack*, 5) sending_out_packs;
} c_Communication_DEMO;

bool c_Communication_send(c_Communication * dst, const Pack * pack)
{
    c_Communication_DEMO * ch = (c_Communication_DEMO *)dst;
    if(RBUF_ISFULL(ch->sending_out_packs)) return false;
    RBUF_PUT(ch->sending_out_packs, pack)
    return true;
}

const Pack * Communication_DEMO_pull(Transmitter * transmitter)
{
    c_Communication_DEMO * ch = (c_Communication_DEMO *)((uint8_t*)transmitter - offsetof(c_Communication_DEMO, channel) - offsetof(c_Communication, transmitter));
    if(RBUF_ISEMPTY(ch->sending_out_packs)) return NULL;
    return RBUF_GET(ch->sending_out_packs);
}

static inline void  Communication_DEMO_on_Stop(c_Communication_DEMO * channel)
{
}
static inline void  Communication_DEMO_on_Start(c_Communication_DEMO * channel)
{
}
static inline void  Communication_DEMO_on_GetDeviceVersion(c_Communication_DEMO * channel)
{
}
static inline void  Communication_DEMO_on_GetConfiguration(c_Communication_DEMO * channel)
{
}
static inline void  Communication_DEMO_on_SetConfiguration(c_Communication_DEMO * channel)
{
}
static inline void Communication_DEMO_on_BusConfiguration(c_Communication_DEMO * channel,  BusConfiguration_6 * p6)
{
    read_BusConfiguration(p6->bytes);
}
static inline void Communication_DEMO_on_InstructionsPack(c_Communication_DEMO * channel,  InstructionsPack_7 * p7)
{
    read_InstructionsPack(p7->bytes);
}
static inline Meta const * Communication_DEMO_dispatcher(Receiver* receiver, size_t id, Pack* pack)
{
    c_Communication_DEMO * channel = (c_Communication_DEMO *)((uint8_t*)receiver - offsetof(c_Communication_DEMO, channel) - offsetof(c_Communication, receiver));
    c_Communication_DISPATCHER(Communication_DEMO)
}



int main()
{
    c_Communication_DEMO Communication_instance = { .channel.receiver.dispatch = Communication_DEMO_dispatcher, .channel.transmitter.pull = Communication_DEMO_pull  };
    {
        DeviceVersion_0 * p0 = c_Communication_new_DeviceVersion();
        write_DeviceVersion(p0_DeviceVersion_from(p0));
        if(! c_Communication_send_DeviceVersion(&Communication_instance.channel, p0))
        {
            free_pack(p0);
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        BusConfiguration_6 * p6 = c_Communication_new_BusConfiguration();
        write_BusConfiguration(p6_BusConfiguration_from(p6));
        if(! c_Communication_send_BusConfiguration(&Communication_instance.channel, p6))
        {
            free_pack(p6);
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        InstructionsPack_7 * p7 = c_Communication_new_InstructionsPack();
        write_InstructionsPack(p7_InstructionsPack_from(p7));
        if(! c_Communication_send_InstructionsPack(&Communication_instance.channel, p7))
        {
            free_pack(p7);
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        DeviceError_8 * p8 = c_Communication_new_DeviceError();
        write_DeviceError(p8_DeviceError_from(p8));
        if(! c_Communication_send_DeviceError(&Communication_instance.channel, p8))
        {
            free_pack(p8);
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        SensorsData_9 * p9 = c_Communication_new_SensorsData();
        write_SensorsData(p9_SensorsData_from(p9));
        if(! c_Communication_send_SensorsData(&Communication_instance.channel, p9))
        {
            free_pack(p9);
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    // c_Communication_packs_into_bytes(uint8_t* dst, size_t bytes) for sending out packs
    //c_Communication_bytes_into_packs(uint8_t* src, size_t bytes) for receiving packs
    return 0;
}
