
#ifdef __cplusplus
extern "C"
{
#endif


#pragma once

#include "Utils_.h"



static inline uint16_t p0_DevID_GET(p0_DeviceVersion * src)
{
    return ((get_bytes(src,  0, 2)))    ;
}


static inline void p0_DevID_SET(uint16_t  src, p0_DeviceVersion * dst)
{
    set_bytes((src), 2, dst,  0);
}


static inline uint16_t p0_Ver_GET(p0_DeviceVersion * src)
{
    return ((get_bytes(src,  2, 2)))    ;
}


static inline void p0_Ver_SET(uint16_t  src, p0_DeviceVersion * dst)
{
    set_bytes((src), 2, dst,  2);
}


static inline uint8_t p6_multiplier_GET(p6_BusConfiguration * src)    //множитель периода опроса, time << multiplier
{
    return        src[0]    ;
}


static inline void p6_multiplier_SET(uint8_t  src, p6_BusConfiguration * dst)    //множитель периода опроса, time << multiplier
{
    dst[0] = (uint8_t)(src) ;
}


static inline uint16_t p6_time_GET(p6_BusConfiguration * src)    //период опроса регистров чтения в us
{
    return ((get_bytes(src,  1, 2)))    ;
}


static inline void p6_time_SET(uint16_t  src, p6_BusConfiguration * dst)    //период опроса регистров чтения в us
{
    set_bytes((src), 2, dst,  1);
}


static inline uint16_t p6_clk_khz_GET(p6_BusConfiguration * src)    //частота i2c шины
{
    return ((get_bytes(src,  3, 2)))    ;
}


static inline void p6_clk_khz_SET(uint16_t  src, p6_BusConfiguration * dst)    //частота i2c шины
{
    set_bytes((src), 2, dst,  3);
}


static inline uint8_t p7_Length_GET(p7_InstructionsPack * src)    //bytes amount
{
    return        src[0]    ;
}


static inline void p7_Length_SET(uint8_t  src, p7_InstructionsPack * dst)    //bytes amount
{
    dst[0] = (uint8_t)(src) ;
}


static inline uint8_t v7_Instructions_GET(V7_Instructions const * const  src, size_t index) {	return      src->bytes[index * 1]; }

static inline V7_Instructions p7_Instructions_GET(p7_InstructionsPack * src)      //instructions buffer
{
    return (V7_Instructions) { src + 1, 256};
}


static inline void v7_Instructions_SET(uint8_t src, size_t index, V7_Instructions * dst) {  dst->bytes[index * 1] = (uint8_t)(src) ; }

static inline V7_Instructions p7_Instructions_SET(const uint8_t src[], p7_InstructionsPack * dst)    //instructions buffer
{
    V7_Instructions  ret = { dst + 1,  256 };
    if(src)
        for(size_t i = 0 ; i < 256 ; i++)
            v7_Instructions_SET(src[i], i, &ret);
    return ret;
}
#define  P7_RW_mode ( 128 ) //128 


static inline uint16_t p8_param1_GET(p8_DeviceError * src)
{
    return ((get_bytes(src,  0, 2)))    ;
}


static inline void p8_param1_SET(uint16_t  src, p8_DeviceError * dst)
{
    set_bytes((src), 2, dst,  0);
}


static inline e_Errors p8_error_id_GET(p8_DeviceError * src)
{
    return (int8_t)(get_bits(src, 16, 1))    ;
}


static inline void p8_error_id_SET(e_Errors  src, p8_DeviceError * dst)
{
    set_bits(src, 1, dst, 16);
}


static inline uint16_t v9_values_GET(V9_values const * const  src, size_t index) {	return ((get_bytes(src->bytes,  index * 2, 2))); }

static inline V9_values p9_values_GET(p9_SensorsData * src)      //1000 how many measure contains in a pack
{
    return (V9_values) { src + 0, 1000};
}


static inline void v9_values_SET(uint16_t src, size_t index, V9_values * dst) {  set_bytes((src), 2, dst->bytes,  index * 2); }

static inline V9_values p9_values_SET(const uint16_t src[], p9_SensorsData * dst)    //1000 how many measure contains in a pack
{
    V9_values  ret = { dst + 0,  1000 };
    if(src)
        for(size_t i = 0 ; i < 1000 ; i++)
            v9_values_SET(src[i], i, &ret);
    return ret;
}



//#######################                 c_Communication                       ##################################
typedef struct
{
    Receiver receiver;
    Transmitter transmitter;
} c_Communication;


/*
As the component of receiving facilities
c_Communication_DISPATCHER(CHANNEL) macro is generating packet dispatcher source code.
The CHANNEL parameter is an arbitrary prefix of related to dispatcher functions

for example if CHANNEL parameter is MY_DISP

The macro will produce code like this

switch (id) {
	 case 1:
	    return pack ? MY_DISP_on_SensorData( channel , pack ), NULL : &m_Pack;
      ...
}

The code expects MY_DISP_on_SensorData - SensorData packet receiving handler function and pointer to the channel in the scope. You have to provide it!

if CHANNEL parameter is BLA_BLA

The macro will issue

switch (id) {
	 case 1:
	    return pack ? BLA_BLA_on_SensorData( channel , pack ), NULL : &m_Pack;
     ...
}

please use more meaningful prefixes
c_Communication_DISPATCHER(CHANNEL) macro expecting the following functions in the scope
------------------------------------

static inline void <CHANNEL>_on_Stop( <Channel> * channel ){}
static inline void <CHANNEL>_on_Start( <Channel> * channel ){}
static inline void <CHANNEL>_on_GetDeviceVersion( <Channel> * channel ){}
static inline void <CHANNEL>_on_GetConfiguration( <Channel> * channel ){}
static inline void <CHANNEL>_on_SetConfiguration( <Channel> * channel ){}
static inline void <CHANNEL>_on_BusConfiguration( <Channel> * channel, BusConfiguration_6 * pack ){}
static inline void <CHANNEL>_on_InstructionsPack( <Channel> * channel, InstructionsPack_7 * pack ){}

-----------------------------------
Please, just copy-paste code above and replace:
<CHANNEL>  with your prefix
<Channel>  with appropriate channel typedef

Particularly this macro helps to build dispatcher functions for channel receiver

static inline Meta* dispatcher (Receiver* receiver, size_t id, Pack* pack)
{
	MyChannel * channel = ...// here is the code to get the pointer to Channel by pointer to Channel Receiver
	c_Communication_DISPATCHER( BLA_BLA )
}
*/
#define c_Communication_DISPATCHER(CHANNEL)\
		switch (id)\
		{\
			default:    return NULL;\
			case 1:	return pack ? CHANNEL##_on_Stop( channel ), (Meta const*)NULL : &m1_Stop;\
			case 2:	return pack ? CHANNEL##_on_Start( channel ), (Meta const*)NULL : &m2_Start;\
			case 3:	return pack ? CHANNEL##_on_GetDeviceVersion( channel ), (Meta const*)NULL : &m3_GetDeviceVersion;\
			case 4:	return pack ? CHANNEL##_on_GetConfiguration( channel ), (Meta const*)NULL : &m4_GetConfiguration;\
			case 5:	return pack ? CHANNEL##_on_SetConfiguration( channel ), (Meta const*)NULL : &m5_SetConfiguration;\
			case 6:	return pack ? CHANNEL##_on_BusConfiguration( channel , pack ), (Meta const*)NULL : &m6_BusConfiguration;\
			case 7:	return pack ? CHANNEL##_on_InstructionsPack( channel , pack ), (Meta const*)NULL : &m7_InstructionsPack;\
		}

static inline void    c_Communication_bytes_into_packs(uint8_t* src, size_t src_bytes, c_Communication * dst) {  receive(src, src_bytes, &dst->receiver);}


static inline size_t  c_Communication_packs_into_bytes(c_Communication * src, uint8_t* dst, size_t dst_bytes) { return transmit(&src->transmitter, dst, dst_bytes);}
extern bool c_Communication_send(c_Communication * dst, const Pack * pack);


/**
 * send DeviceVersion
 * param DeviceVersion_0 * pointer to the pack that will be send
 * param c_Communication * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_Communication_send_DeviceVersion( c_Communication, DeviceVersion_0) ( c_Communication_send ( c_Communication , DeviceVersion_0)? ( (DeviceVersion_0) = NULL), true : false)
/**
 * brief Getting new pack that can be populated with data and send !!!!
 * return new pack
 */
static inline  DeviceVersion_0 *  c_Communication_new_DeviceVersion() { return new_pack(&m0_DeviceVersion);}

/**
 * send BusConfiguration
 * param BusConfiguration_6 * pointer to the pack that will be send
 * param c_Communication * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_Communication_send_BusConfiguration( c_Communication, BusConfiguration_6) ( c_Communication_send ( c_Communication , BusConfiguration_6)? ( (BusConfiguration_6) = NULL), true : false)
/**
 * brief Getting new pack that can be populated with data and send !!!!
 * return new pack
 */
static inline  BusConfiguration_6 *  c_Communication_new_BusConfiguration() { return new_pack(&m6_BusConfiguration);}

/**
 * send InstructionsPack
 * param InstructionsPack_7 * pointer to the pack that will be send
 * param c_Communication * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_Communication_send_InstructionsPack( c_Communication, InstructionsPack_7) ( c_Communication_send ( c_Communication , InstructionsPack_7)? ( (InstructionsPack_7) = NULL), true : false)
/**
 * brief Getting new pack that can be populated with data and send !!!!
 * return new pack
 */
static inline  InstructionsPack_7 *  c_Communication_new_InstructionsPack() { return new_pack(&m7_InstructionsPack);}

/**
 * send DeviceError
 * param DeviceError_8 * pointer to the pack that will be send
 * param c_Communication * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_Communication_send_DeviceError( c_Communication, DeviceError_8) ( c_Communication_send ( c_Communication , DeviceError_8)? ( (DeviceError_8) = NULL), true : false)
/**
 * brief Getting new pack that can be populated with data and send !!!!
 * return new pack
 */
static inline  DeviceError_8 *  c_Communication_new_DeviceError() { return new_pack(&m8_DeviceError);}

/**
 * send SensorsData
 * param SensorsData_9 * pointer to the pack that will be send
 * param c_Communication * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_Communication_send_SensorsData( c_Communication, SensorsData_9) ( c_Communication_send ( c_Communication , SensorsData_9)? ( (SensorsData_9) = NULL), true : false)
/**
 * brief Getting new pack that can be populated with data and send !!!!
 * return new pack
 */
static inline  SensorsData_9 *  c_Communication_new_SensorsData() { return new_pack(&m9_SensorsData);}


#ifdef __cplusplus
}
#endif
