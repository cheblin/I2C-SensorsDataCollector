package org.company;

import org.unirail.AdHoc.*;

public interface PowerProfiler {
	class Device implements InC {
		public interface ServerMain extends Device.CommonPacks {
			/**
			 Stop device command
			 It's stop I2C sensors data request timer. Only listen PC commands
			 */
			@id(1)
			class Stop {}
			
			/**
			 Prepare and start I2C sensors data request timer.
			 */
			@id(2)
			class Start {}
			
			/**
			 Device vertion request.
			 */
			@id(3)
			class GetDeviceVersion {}
			
			
			/**
			 Request current device configuration, PC should be prepare receive two DeviceConfiguration packs
			 First pack - Sensors configuration
			 Second  - Sensor Reading configuration
			 */
			@id(4)
			class GetConfiguration {}
			
			/**
			 Set device configuration, PC will send two DeviceConfiguration packs
			 First pack - Sensors configuration
			 Second  - Sensor Reading configuration
			 */
			@id(5)
			class SetConfiguration {}
		}
		
		interface CommonPacks {
			/**
			 Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration
			 */
			@id(6)
			class BusConfiguration {
				@I byte  multiplier; // множитель периода опроса, time << multiplier
				@I short time;       // период опроса регистров чтения в us
				@I short clk_khz;    // частота i2c шины
			}
			
			/**
			 Mini assembler instructions buffer starts with a
			 RW-instruction byte, it switch device in the Read/Write mode and hold the number of Readings/Writings I2C Bus.
			 .
			 every Read instruction consume 2 bytes
			 address
			 register
			 .
			 every Write instruction consume 4 bytes
			 address
			 writing_value - 2 bytes
			 register
			 .
			 so InstructionsPack can hold maximum 127 Readings or 64 Writings
			 .
			 if RW- < 128 - it contains number of rest Readings  from I2C
			 if 128 < RW - it contains (RW - 128) number of rest Writings  into I2C
			 .
			 If this pack is not following after RequestConfiguration:
			 .
			 if it contains read Sensor data instructions
			 it will be proceeded many times until fill all SensorsData, and then SensorsData send
			 .
			 if it contains any write Sensors registers instructions
			 it will be proceeded only once
			 */
			@id(7)
			class InstructionsPack {
				@I           byte  Length;// bytes amount
				@I @__(256) byte  Instructions;//instructions buffer
				static final short RW_mode = 128;
			}
		}
	}
	
	class Client implements InCS, InCPP, InTS, InKT, InRUST {
		public interface ClientMain extends Device.CommonPacks {
			/**
			 Device version reply
			 */
			@id(0)
			class DeviceVersion {
				@I short DevID;
				@I short Ver;
			}
			
			/**
			 Device Error information
			 */
			@id(8)
			class DeviceError {
				Errors error_id;
				@I short param1;
				
				enum Errors {
					SensorsDataOverflow,
					CannotReadSensor,
				}
			}
			
			/**
			 Bulk sensors data. Sequentially ordered by address sensors data reading
			 */
			@id(9)
			class SensorsData {
				@I @__(1000) short values;//1000 how many measure contains in a pack
			}
		}
	}
	
	class Communication extends StdProtocol implements Device.ServerMain, Client.ClientMain {}
}
