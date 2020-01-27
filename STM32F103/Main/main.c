#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

#include "rw_ini.h"
#include "smbus.h"
#include "adhoc/Device.h"
#include <stm32f10x_rcc.h>


#define Timer_SetPreriod_us(x)	TIM2->ARR = x-1 /* Set the Autoreload value */
#define Timer_Stop()		TIM_Cmd(TIM2, DISABLE);

// Структура конфигурации опроса и инициализации устройства
// Выходной пакет непрерывного опроса формируется по данному описанию
typedef struct
{
	uint8_t  multiplier; // множитель периода опроса, time << multiplier
	uint16_t time;       // период опроса регистров чтения в us 
	uint16_t clk_khz;    // частота i2c шины
}            dev_cfg_t;

dev_cfg_t cfg_ini;

c_Communication channel;

RBUF_INIT(const Pack*, 4) sending_packs; //ring buffer a sending queue of 16 Packs

bool c_Communication_send(c_Communication* dst, const Pack* pack) //put packa into sending queue
{
	if (RBUF_ISFULL(sending_packs)) return false;
	RBUF_PUT(sending_packs, pack);
	return true;
}

const Pack* pull_sending_psck(Transmitter* transmitter) { return RBUF_ISEMPTY(sending_packs) ? NULL : RBUF_GET(sending_packs); } //pull out packs form sending queue

void sending_error(e_Errors error, uint16_t param); //prototype

uint8_t SD1[sizeof(SensorsData_9)]; //statically  allocated SensorsData pack
uint8_t SD2[sizeof(SensorsData_9)]; //statically  allocated SensorsData pack

SensorsData_9* sendingSensorsData = NULL; //currently sending SensorsData pack, if sending_pack == NULL sending_pack was sent

SensorsData_9* fillingSensorsData = (SensorsData_9*)SD1; //currently filling with Sensors data SensorsData pack

uint16_t filling_pack_pos = 0; //current filling position in filling_pack

void append_sensors_data(uint16_t value)
{
	static p9_SensorsData* filling_pack_bytes = ((SensorsData_9*)SD1)->bytes;                  // equal to    p9_SensorsData_from(filling_pack);
	static V9_values       sensors_data       = {((SensorsData_9*)SD1)->bytes, P9_values_len}; //equal to  v9_values_SET(NULL, filling_pack_bytes);

	if (filling_pack_pos < P9_values_len)
	{
		v9_values_SET(value, filling_pack_pos += 2, &sensors_data);
		return;
	}

	if (sendingSensorsData) //not sending yes
	{
		sending_error(e_Errors_SensorsDataOverflow, 0);//send to PC error message
		filling_pack_pos = 0; //overwrite
		return;
	}
	
	c_Communication_send(&channel, sendingSensorsData = fillingSensorsData); //dirty hack

	fillingSensorsData = fillingSensorsData == (SensorsData_9*)SD1 ? (SensorsData_9*)SD2 : (SensorsData_9*)SD1;

	filling_pack_bytes = p9_SensorsData_from(fillingSensorsData); //getting bytes
	sensors_data       = p9_values_SET(NULL, filling_pack_bytes); //getting reference to values space

	filling_pack_pos = 0;
	v9_values_SET(value, filling_pack_pos += 2, &sensors_data);
}


uint8_t IP1[sizeof(InstructionsPack_7)]; //statically  allocated InstructionsPack pack
uint8_t IP2[sizeof(InstructionsPack_7)]; //statically  allocated InstructionsPack pack
uint8_t IP3[sizeof(InstructionsPack_7)]; //statically  allocated InstructionsPack pack

InstructionsPack_7* SensorsWritingConfigInstructions = (InstructionsPack_7*)IP1; //only writing of I2C Bus sensors configuration instruction buffer.
InstructionsPack_7* SensorsReadingDataInstructions   = (InstructionsPack_7*)IP2; //mixing, reading/writing I2C Bus sensors instructions buffer. using in main loop routing 
InstructionsPack_7* ReceivedInstructions             = (InstructionsPack_7*)IP3; //buffer for receiving Instructions from PC

/**
 * Process (InstructionsPack
 */
void process_instructions(InstructionsPack_7* p7)
{
	p7_InstructionsPack*  src   = p7_InstructionsPack_from(p7);
	const uint8_t         len   = p7_Length_GET(src);
	const V7_Instructions array = p7_Instructions_GET(src);
	uint8_t*              data  = array.bytes;

	for (uint16_t i = 1, value, RW = data[0]; i < len; RW = i + data[i++])
		if (RW < P7_RW_mode) //Read mode
			for (RW = i + 2 * RW; i < RW;)
				if (SMBusReadWord(data[i++], &value, data[i++])) append_sensors_data(value);
				else
				{
					Timer_Stop();
					sending_error(e_Errors_CannotReadSensor, data[i - 2]);
					return;
				}
		else //write mode 
			for (RW = i + 4 * (RW - P7_RW_mode); i < RW;) SMBusWriteWord(data[i++], get_bytes(data, i += 2, 2), data[i++]);
}


void Timer_Init(uint16_t period_us, uint16_t multiplier)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;

	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period        = period_us - 1;          // 1 MHz down to 10 KHz (100 us)
	TIM_TimeBaseStructure.TIM_Prescaler     = (72 << multiplier) - 1; // 72 MHz Clock down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
	//	timer_count = 0;
}


/*******************************************************************************
 * Function Name : InitExtDevice.
 * Descriptioan  : Инициализация таймера опроса и устройва на SMBus
 * Input		 : None.
 * Output		 : None.
 * Return		 : None.
 *******************************************************************************/
uint8_t InitExtDevice(void)
{
	*(Meta**)&SD1 = &m9_SensorsData; //set pack metatype
	*(Meta**)&SD2 = &m9_SensorsData; //set pack metatype

	*(Meta**)IP1 = &m7_InstructionsPack; //set pack metatype
	*(Meta**)IP2 = &m7_InstructionsPack; //set pack metatype
	*(Meta**)IP3 = &m7_InstructionsPack; //set pack metatype

	if (cfg_ini.clk_khz < 100 || cfg_ini.clk_khz > 5000) cfg_ini.clk_khz = SMBus_Speed / 1000;
	SMBusInit(cfg_ini.clk_khz * 1000);

	process_instructions(SensorsWritingConfigInstructions); //initialize I2C sensorce 

	Timer_Init(cfg_ini.time, cfg_ini.multiplier);

	return 1;
}


static inline void I2C_data_collector_on_Stop(c_Communication* channel)
{
	Timer_Stop()
}

static inline void I2C_data_collector_on_Start(c_Communication* channel) { Timer_Init(cfg_ini.time, cfg_ini.multiplier); }

static inline void I2C_data_collector_on_GetDeviceVersion(c_Communication* channel)
{
	DeviceVersion_0* p0 = c_Communication_new_DeviceVersion();

	p0_DeviceVersion* dst = p0_DeviceVersion_from(p0);
	p0_DevID_SET(0x0016, dst); // DevID = 0x0016
	p0_Ver_SET(0x0006, dst);   // Ver 0.0.0.6 = 0x0006

	c_Communication_send_DeviceVersion(channel, p0); //sending DeviceVersion
}


static inline void I2C_data_collector_on_GetConfiguration(c_Communication* channel)
{
	c_Communication_send(channel, (InstructionsPack_7*)SensorsWritingConfigInstructions); //dirty hack
	c_Communication_send(channel, (InstructionsPack_7*)SensorsReadingDataInstructions);   //dirty hack
}

uint8_t            config_packs = 0; //config_packs counter
static inline void I2C_data_collector_on_SetConfiguration(c_Communication* channel) { config_packs = 2; }

static inline void I2C_data_collector_on_InstructionsPack(c_Communication* channel, InstructionsPack_7* pack)
{
	switch (config_packs)
	{
	case 2:
		{
			InstructionsPack_7* tmp          = SensorsWritingConfigInstructions;
			SensorsWritingConfigInstructions = ReceivedInstructions;
			ReceivedInstructions             = tmp;
			config_packs--;
			break;
		}
	case 1:
		{
			InstructionsPack_7* tmp        = SensorsReadingDataInstructions;
			SensorsReadingDataInstructions = ReceivedInstructions;
			ReceivedInstructions           = tmp;
			config_packs--;
			break;
		}
	}

	filling_pack_pos = 0;
	do process_instructions(pack);
	while (filling_pack_pos);
}

volatile bool scan_sensors_data;

static inline void I2C_data_collector_on_BusConfiguration(c_Communication* channel, BusConfiguration_6* p6)
{
	scan_sensors_data = false;

	p6_BusConfiguration* src = p6_BusConfiguration_from(p6);
	cfg_ini.clk_khz          = p6_clk_khz_GET(src);
	cfg_ini.multiplier       = p6_multiplier_GET(src);
	cfg_ini.time             = p6_time_GET(src);

	Timer_Init(cfg_ini.time, cfg_ini.multiplier);
	scan_sensors_data = true;
}


static inline Meta const* dispatch_pack(Receiver* receiver, size_t id, Pack* pack) //Received packs dispatcher
{
	c_Communication* channel = (c_Communication*)(uint8_t*)receiver;
	c_Communication_DISPATCHER(I2C_data_collector)
}


Pack* InstructionsPack_alloc(Pack* pack) { return pack ? NULL : ReceivedInstructions; } //artificial allocator

Pack* SensorsData_alloc(Pack* pack) //artificial allocator
{
	return pack && pack == sendingSensorsData
		       ? sendingSensorsData = NULL //signal sending_pack was sent
		       : NULL;
}

extern __IO uint8_t  Receive_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern __IO uint32_t Receive_length;


c_Communication channel = {.receiver.dispatch = dispatch_pack, .transmitter.pull = pull_sending_psck}; // communication send/receive channel

void sending_error(e_Errors error, uint16_t param)
{
	DeviceError_8*  pack = c_Communication_new_DeviceError();
	p8_DeviceError* dst  = p8_DeviceError_from(pack);
	p8_error_id_SET(error, dst);
	p8_param1_SET(param, dst);

	c_Communication_send_DeviceError(&channel, pack);
}

volatile uint32_t packet_sent    = 1;
volatile uint32_t packet_receive = 1;

int main(void)
{
	m9_SensorsData.pack_alloc      = SensorsData_alloc;      //special allocators
	m7_InstructionsPack.pack_alloc = InstructionsPack_alloc; //special allocators


	Set_System(); // in hw_config.c
	// USB Init
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
	Delay_ms(75); // Wait stabilization power voltage
	// CFG Init
	if (!ReadIniBlk(&cfg_ini, sizeof(cfg_ini)))
	{
		cfg_ini.time       = 400;
		cfg_ini.multiplier = 0;
		cfg_ini.clk_khz    = 400;
	}
	InitExtDevice();
	uint8_t send_buff[VIRTUAL_COM_PORT_DATA_SIZE];
	while (1)
	{
		if (bDeviceState == CONFIGURED)
		{
			while (CDC_Receive_DATA() && packet_receive) c_Communication_bytes_into_packs((uint8_t*)Receive_Buffer, Receive_length, &channel);

			for (int len = 0; packet_sent && 0 < (len = c_Communication_packs_into_bytes(&channel, send_buff, VIRTUAL_COM_PORT_DATA_SIZE)); CDC_Send_DATA(send_buff, len));
		}
	}
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
 * Function Name	 : assert_failed
 * Description	 : Reports the name of the source file and the source line number
 *				   where the assert_param error has occurred.
 * Input			 : - file: pointer to the source file name
 *				   - line: assert_param error line source number
 * Output		 : None
 * Return		 : None
 *******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	 /* Infinite loop */
	while (1) {}
}
#endif

/*******************************************************************************
 * Function Name	 : TIM2_IRQHandler
 * Description	 : This function handles TIM2 global interrupt request.
 * Input			 : None
 * Output		 : None
 * Return		 : None
 *******************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if (scan_sensors_data) process_instructions(SensorsReadingDataInstructions);
	}
}
