
#ifdef __cplusplus
extern "C"
{
#endif

#pragma once

#include "AdHoc.h"

typedef struct
{
    uint8_t * bytes ;
    size_t  len;
    size_t  BIT;
} BitsArray;

typedef struct
{
    uint8_t * bytes ;
    size_t  BIT;
} BitsValue;

typedef struct
{
    uint8_t * bytes ;
    size_t  len;
} BytesArray;

typedef struct
{
    uint8_t * bytes ;
} BytesValue;

#define AD_HOC_RECEIVE_REQ_MAX_BYTES 257
#define AD_HOC_RECEIVE_FULL_MAX_BYTES 257



#define AD_HOC_SEND_REQ_MAX_BYTES 2000
#define AD_HOC_SEND_FULL_MAX_BYTES 2000



static inline bool is_equal_data(Pack *a, Pack *b) { return (a && b && a == b) || (a->meta == b->meta && memcmp(a->bytes, b->bytes, a->meta->packMinBytes) == 0); }



extern Meta m0_DeviceVersion;
extern Meta m1_Stop;
extern Meta m2_Start;
extern Meta m3_GetDeviceVersion;
extern Meta m4_GetConfiguration;
extern Meta m5_SetConfiguration;
extern Meta m6_BusConfiguration;
extern Meta m7_InstructionsPack;
extern Meta m8_DeviceError;
extern Meta m9_SensorsData;


typedef  enum
{
    e_Errors_SensorsDataOverflow = 0,
    e_Errors_CannotReadSensor = 1
} e_Errors;


/**
*Raw pack contains pointer to the Meta information and bytes. It can be acquired via new_channels_functions.
*/
typedef Pack  DeviceVersion_0;

/**
* Raw packs without optional fields no needs any navigator, to explore there data
* To access pack fields data, functions need only pack bytes. So highlight it.
*/
typedef uint8_t p0_DeviceVersion;
//Pack data bytes
static inline p0_DeviceVersion * p0_DeviceVersion_from(DeviceVersion_0 * pack) { return pack->bytes;}

/*MThe macro to simplifies writing code to export data from the pack.
* Macro is expecting and calls the following functions..
static inline void <DST>_DevID_SET( uint16_t * src, <DST> * dst  ){}
static inline void <DST>_Ver_SET( uint16_t * src, <DST> * dst  ){}
*/

#define p0_DeviceVersion_PUSH_INTO(DST)\
	static inline void p0_DeviceVersion_push_into_##DST ( p0_DeviceVersion * src, DST * dst) {\
		DST##_DevID_SET( p0_DevID_GET( src  ), dst  );\
		DST##_Ver_SET( p0_Ver_GET( src  ), dst  );\
	}

/*The macro to simplify writing code to import data into the pack from external sources.
* Macro is expecting and calls the following functions..
static inline uint16_t <SRC>_DevID_GET( <SRC> * src ){}
static inline uint16_t <SRC>_Ver_GET( <SRC> * src ){}
*/

#define p0_DeviceVersion_PULL_FROM(SRC)\
	static inline void p0_DeviceVersion_pull_from_##SRC ( SRC * src, p0_DeviceVersion * dst) {\
		p0_DevID_SET( SRC##_DevID_GET(src ), dst  );\
		p0_Ver_SET( SRC##_Ver_GET(src ), dst  );\
	}

/**
*Raw pack contains pointer to the Meta information and bytes. It can be acquired via new_channels_functions.
*/
typedef Pack  BusConfiguration_6;

/**
* Raw packs without optional fields no needs any navigator, to explore there data
* To access pack fields data, functions need only pack bytes. So highlight it.
*/
typedef uint8_t p6_BusConfiguration;
//Pack data bytes
static inline p6_BusConfiguration * p6_BusConfiguration_from(BusConfiguration_6 * pack) { return pack->bytes;}

/*MThe macro to simplifies writing code to export data from the pack.
* Macro is expecting and calls the following functions..
static inline void <DST>_multiplier_SET( uint8_t * src, <DST> * dst  ){}
static inline void <DST>_time_SET( uint16_t * src, <DST> * dst  ){}
static inline void <DST>_clk_khz_SET( uint16_t * src, <DST> * dst  ){}
*/

#define p6_BusConfiguration_PUSH_INTO(DST)\
	static inline void p6_BusConfiguration_push_into_##DST ( p6_BusConfiguration * src, DST * dst) {\
		DST##_multiplier_SET( p6_multiplier_GET( src  ), dst  );\
		DST##_time_SET( p6_time_GET( src  ), dst  );\
		DST##_clk_khz_SET( p6_clk_khz_GET( src  ), dst  );\
	}

/*The macro to simplify writing code to import data into the pack from external sources.
* Macro is expecting and calls the following functions..
static inline uint8_t <SRC>_multiplier_GET( <SRC> * src ){}
static inline uint16_t <SRC>_time_GET( <SRC> * src ){}
static inline uint16_t <SRC>_clk_khz_GET( <SRC> * src ){}
*/

#define p6_BusConfiguration_PULL_FROM(SRC)\
	static inline void p6_BusConfiguration_pull_from_##SRC ( SRC * src, p6_BusConfiguration * dst) {\
		p6_multiplier_SET( SRC##_multiplier_GET(src ), dst  );\
		p6_time_SET( SRC##_time_GET(src ), dst  );\
		p6_clk_khz_SET( SRC##_clk_khz_GET(src ), dst  );\
	}

/**
*Raw pack contains pointer to the Meta information and bytes. It can be acquired via new_channels_functions.
*/
typedef Pack  InstructionsPack_7;

/**
* Raw packs without optional fields no needs any navigator, to explore there data
* To access pack fields data, functions need only pack bytes. So highlight it.
*/
typedef uint8_t p7_InstructionsPack;
//Pack data bytes
static inline p7_InstructionsPack * p7_InstructionsPack_from(InstructionsPack_7 * pack) { return pack->bytes;}

/**
* This struct hold information about none primitive field parameters and used by functions to access field data
*/
typedef BytesArray  V7_Instructions;
//Maximum field array length constant
#define P7_Instructions_len  ( 256 )

/*MThe macro to simplifies writing code to export data from the pack.
* Macro is expecting and calls the following functions..
static inline void <DST>_Length_SET( uint8_t * src, <DST> * dst  ){}
static inline void <DST>_Instructions_SET( V7_Instructions * src, <DST> * dst  ){}
*/

#define p7_InstructionsPack_PUSH_INTO(DST)\
	static inline void p7_InstructionsPack_push_into_##DST ( p7_InstructionsPack * src, DST * dst) {\
		DST##_Length_SET( p7_Length_GET( src  ), dst  );\
		V7_Instructions item_Instructions = p7_Instructions_GET( src  );\
       DST##_Instructions_SET( &item_Instructions, dst );\
	}

/*The macro to simplify writing code to import data into the pack from external sources.
* Macro is expecting and calls the following functions..
static inline uint8_t <SRC>_Length_GET( <SRC> * src ){}
static inline uint8_t <SRC>_Instructions_GET( <SRC> * src, V7_Instructions * dst ){}
*/

#define p7_InstructionsPack_PULL_FROM(SRC)\
	static inline void p7_InstructionsPack_pull_from_##SRC ( SRC * src, p7_InstructionsPack * dst) {\
		p7_Length_SET( SRC##_Length_GET(src ), dst  );\
       V7_Instructions item_Instructions = p7_Instructions_SET( NULL, dst  );\
       SRC##_Instructions_GET( src, &item_Instructions );\
	}

/**
*Raw pack contains pointer to the Meta information and bytes. It can be acquired via new_channels_functions.
*/
typedef Pack  DeviceError_8;

/**
* Raw packs without optional fields no needs any navigator, to explore there data
* To access pack fields data, functions need only pack bytes. So highlight it.
*/
typedef uint8_t p8_DeviceError;
//Pack data bytes
static inline p8_DeviceError * p8_DeviceError_from(DeviceError_8 * pack) { return pack->bytes;}

/*MThe macro to simplifies writing code to export data from the pack.
* Macro is expecting and calls the following functions..
static inline void <DST>_param1_SET( uint16_t * src, <DST> * dst  ){}
static inline void <DST>_error_id_SET( e_Errors * src, <DST> * dst  ){}
*/

#define p8_DeviceError_PUSH_INTO(DST)\
	static inline void p8_DeviceError_push_into_##DST ( p8_DeviceError * src, DST * dst) {\
		DST##_param1_SET( p8_param1_GET( src  ), dst  );\
		DST##_error_id_SET( p8_error_id_GET( src  ), dst  );\
	}

/*The macro to simplify writing code to import data into the pack from external sources.
* Macro is expecting and calls the following functions..
static inline uint16_t <SRC>_param1_GET( <SRC> * src ){}
static inline e_Errors <SRC>_error_id_GET( <SRC> * src ){}
*/

#define p8_DeviceError_PULL_FROM(SRC)\
	static inline void p8_DeviceError_pull_from_##SRC ( SRC * src, p8_DeviceError * dst) {\
		p8_param1_SET( SRC##_param1_GET(src ), dst  );\
		p8_error_id_SET( SRC##_error_id_GET(src ), dst  );\
	}

/**
*Raw pack contains pointer to the Meta information and bytes. It can be acquired via new_channels_functions.
*/
typedef Pack  SensorsData_9;

/**
* Raw packs without optional fields no needs any navigator, to explore there data
* To access pack fields data, functions need only pack bytes. So highlight it.
*/
typedef uint8_t p9_SensorsData;
//Pack data bytes
static inline p9_SensorsData * p9_SensorsData_from(SensorsData_9 * pack) { return pack->bytes;}

/**
* This struct hold information about none primitive field parameters and used by functions to access field data
*/
typedef BytesArray  V9_values;
//Maximum field array length constant
#define P9_values_len  ( 1000 )

/*MThe macro to simplifies writing code to export data from the pack.
* Macro is expecting and calls the following functions..
static inline void <DST>_values_SET( V9_values * src, <DST> * dst  ){}
*/

#define p9_SensorsData_PUSH_INTO(DST)\
	static inline void p9_SensorsData_push_into_##DST ( p9_SensorsData * src, DST * dst) {\
		V9_values item_values = p9_values_GET( src  );\
       DST##_values_SET( &item_values, dst );\
	}

/*The macro to simplify writing code to import data into the pack from external sources.
* Macro is expecting and calls the following functions..
static inline uint16_t <SRC>_values_GET( <SRC> * src, V9_values * dst ){}
*/

#define p9_SensorsData_PULL_FROM(SRC)\
	static inline void p9_SensorsData_pull_from_##SRC ( SRC * src, p9_SensorsData * dst) {\
       V9_values item_values = p9_values_SET( NULL, dst  );\
       SRC##_values_GET( src, &item_values );\
	}


#ifdef __cplusplus
}
#endif
