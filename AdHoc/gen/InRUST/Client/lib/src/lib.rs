
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]
#![feature(const_transmute)]
#[macro_use]
extern crate bitflags;

#[path = "util.rs"]
pub mod util_;
pub use ad_hoc_sys as sys;
pub mod org {
	pub mod company {

		pub mod Client {
			use std::str;
			use std::mem::transmute;
			use crate::util_;

			/**
			*Device version reply */

			pub mod DeviceVersion {
				use crate::org::company::Client as packs;
				use crate::sys;

				pub static meta_: &sys::Meta = unsafe { ::std::mem::transmute(&crate::util_::DeviceVersion) };

				pub struct Pack_(pub *mut sys::Pack);

				impl Pack_ {
					pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
				}
				pub struct Data_(pub *mut u8);

				impl Data_ {
					pub fn DevID(&mut self) -> u16 {
						let src = &mut self.0;
						let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
						(dst) as u16
					}
					pub fn Ver(&mut self) -> u16 {
						let src = &mut self.0;
						let dst = sys::get_bytes(self.0, 2, 2 as usize) as u16;
						(dst) as u16
					}
				}

				pub fn push_<DST: DST_>(src: &mut Data_, dst: &mut DST) {
					dst.DevID(src.DevID());
					dst.Ver(src.Ver());
				}
				pub trait DST_ {
					fn DevID(&mut self, src: u16);
					fn Ver(&mut self, src: u16);
				}
			}
			/**
			*Stop device command
			*				 It's stop I2C sensors data request timer. Only listen PC commands */

			pub mod Stop {
				use super::*;
				use crate::sys;
				pub static meta_: &sys::Meta = unsafe { transmute(&crate::util_::Stop) };
			}
			/**
			*Prepare and start I2C sensors data request timer. */

			pub mod Start {
				use super::*;
				use crate::sys;
				pub static meta_: &sys::Meta = unsafe { transmute(&crate::util_::Start) };
			}
			/**
			*Device vertion request. */

			pub mod GetDeviceVersion {
				use super::*;
				use crate::sys;
				pub static meta_: &sys::Meta = unsafe { transmute(&crate::util_::GetDeviceVersion) };
			}
			/**
			*Request current device configuration, PC should be prepare receive two DeviceConfiguration packs
			*				 First pack - Sensors configuration
			*				 Second  - Sensor Reading configuration */

			pub mod GetConfiguration {
				use super::*;
				use crate::sys;
				pub static meta_: &sys::Meta = unsafe { transmute(&crate::util_::GetConfiguration) };
			}
			/**
			*Set device configuration, PC will send two DeviceConfiguration packs
			*				 First pack - Sensors configuration
			*				 Second  - Sensor Reading configuration */

			pub mod SetConfiguration {
				use super::*;
				use crate::sys;
				pub static meta_: &sys::Meta = unsafe { transmute(&crate::util_::SetConfiguration) };
			}
			/**
			*Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */

			pub mod BusConfiguration {
				use crate::org::company::Client as packs;
				use crate::sys;

				pub static meta_: &sys::Meta = unsafe { ::std::mem::transmute(&crate::util_::BusConfiguration) };

				pub struct Pack_(pub *mut sys::Pack);

				impl Pack_ {
					pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
				}
				pub struct Data_(pub *mut u8);

				impl Data_ {
					pub fn multiplier(&mut self) -> crate::util_::Item_l { crate::util_::Item_l(self.0) }
					pub fn time(&mut self) -> crate::util_::Item_c { crate::util_::Item_c(self.0) }
					pub fn clk_khz(&mut self) -> crate::util_::Item_p { crate::util_::Item_p(self.0) }
				}

				impl crate::org::company::Communication::ITransmittable::Pack for Pack_ {
					fn pack(self) -> *mut sys::Pack { self.0 }
				}

				pub fn push_<DST: DST_>(src: &mut Data_, dst: &mut DST) {
					dst.multiplier(src.multiplier().get());
					dst.time(src.time().get());
					dst.clk_khz(src.clk_khz().get());
				}
				pub trait DST_ {
					fn multiplier(&mut self, src: u8);
					fn time(&mut self, src: u16);
					fn clk_khz(&mut self, src: u16);
				}

				pub trait SRC_ {
					fn multiplier(&mut self) -> u8;
					fn time(&mut self) -> u16;
					fn clk_khz(&mut self) -> u16;
				}
				pub fn pull_<SRC: SRC_>(src: &mut SRC, dst: &mut Data_) {
					dst.multiplier().set(src.multiplier());
					dst.time().set(src.time());
					dst.clk_khz().set(src.clk_khz());
				}
			}
			/**
			*Mini assembler instructions buffer starts with a
			*				 RW-instruction byte, it switch device in the Read/Write mode and hold the number of Readings/Writings I2C Bus.
			*				 .
			*				 every Read instruction consume 2 bytes
			*				 address
			*				 register
			*				 .
			*				 every Write instruction consume 4 bytes
			*				 address
			*				 writing_value - 2 bytes
			*				 register
			*				 .
			*				 so InstructionsPack can hold maximum 127 Readings or 64 Writings
			*				 .
			*				 if RW- < 128 - it contains number of rest Readings  from I2C
			*				 if 128 < RW - it contains (RW - 128) number of rest Writings  into I2C
			*				 .
			*				 If this pack is not following after RequestConfiguration:
			*				 .
			*				 if it contains read Sensor data instructions
			*				 it will be proceeded many times until fill all SensorsData, and then SensorsData send
			*				 .
			*				 if it contains any write Sensors registers instructions
			*				 it will be proceeded only once */

			pub mod InstructionsPack {
				use crate::org::company::Client as packs;
				use crate::sys;

				pub static meta_: &sys::Meta = unsafe { ::std::mem::transmute(&crate::util_::InstructionsPack) };

				pub struct Pack_(pub *mut sys::Pack);

				impl Pack_ {
					pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
				}
				pub struct Data_(pub *mut u8);

				impl Data_ {
					pub fn Length(&mut self) -> crate::util_::Item_C { crate::util_::Item_C(self.0) }
					pub fn Instructions(&mut self, src: Option<&mut dyn Iterator<Item = u8>>) -> crate::util_::ItemArray_U {
						let dst = self.0;

						let len = 256 as usize;
						let offset = 1;
						let bytes = dst;
						let mut array = crate::util_::ItemArray_U { bytes, len, offset, index: !0 };
						if let Some(src) = src {
							for i in 0..len {
								if let Some(val) = src.next() {
									array.set(i, val);
								} else {
									break;
								}
							}
						}
						array
					}
				}

				impl crate::org::company::Communication::ITransmittable::Pack for Pack_ {
					fn pack(self) -> *mut sys::Pack { self.0 }
				}

				pub fn push_<DST: DST_>(src: &mut Data_, dst: &mut DST) {
					dst.Length(src.Length().get());
					dst.Instructions(&mut src.Instructions(None));
				}
				pub trait DST_ {
					fn Length(&mut self, src: u8);
					fn Instructions(&mut self, src: &mut crate::util_::ItemArray_U);
				}

				pub trait SRC_ {
					fn Length(&mut self) -> u8;
					fn Instructions(&mut self, dst: &mut crate::util_::ItemArray_U);
				}
				pub fn pull_<SRC: SRC_>(src: &mut SRC, dst: &mut Data_) {
					dst.Length().set(src.Length());
					src.Instructions(&mut dst.Instructions(None));
				}

				pub mod Instructions {
					pub const len: usize = 256usize;
				}

				pub const RW_mode: i16 = 128i16; //128 
			}
			/**
			*Device Error information */

			pub mod DeviceError {
				use crate::org::company::Client as packs;
				use crate::sys;

				pub static meta_: &sys::Meta = unsafe { ::std::mem::transmute(&crate::util_::DeviceError) };

				pub struct Pack_(pub *mut sys::Pack);

				impl Pack_ {
					pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
				}
				pub struct Data_(pub *mut u8);

				impl Data_ {
					pub fn param1(&mut self) -> u16 {
						let src = &mut self.0;
						let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
						(dst) as u16
					}
					pub fn error_id(&mut self) -> packs::Errors {
						let src = &mut self.0;
						packs::Errors::from_bits((sys::get_bits(self.0, 16, 1)) as i8).unwrap()
					}
				}

				pub fn push_<DST: DST_>(src: &mut Data_, dst: &mut DST) {
					dst.param1(src.param1());
					dst.error_id(src.error_id());
				}
				pub trait DST_ {
					fn param1(&mut self, src: u16);
					fn error_id(&mut self, src: packs::Errors);
				}
			}
			/**
			*Bulk sensors data. Sequentially ordered by address sensors data reading */

			pub mod SensorsData {
				use crate::org::company::Client as packs;
				use crate::sys;

				pub static meta_: &sys::Meta = unsafe { ::std::mem::transmute(&crate::util_::SensorsData) };

				pub struct Pack_(pub *mut sys::Pack);

				impl Pack_ {
					pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
				}
				pub struct Data_(pub *mut u8);

				impl Data_ {
					pub fn values(&mut self) -> crate::util_::ItemArray_A {
						let src = self.0;

						let len = 1000 as usize;
						let offset = 0;
						let bytes = src;
						crate::util_::ItemArray_A { bytes, len, offset, index: !0 }
					}
				}

				pub fn push_<DST: DST_>(src: &mut Data_, dst: &mut DST) { dst.values(&mut src.values()); }
				pub trait DST_ {
					fn values(&mut self, src: &mut crate::util_::ItemArray_A);
				}

				pub mod values {
					pub const len: usize = 1000usize;
				}
			}

			#[derive(PartialEq, Debug)]
			#[repr(i8)]
			pub enum Errors {
				SensorsDataOverflow = 0,
				CannotReadSensor    = 1,
			}

			impl Errors {
				pub fn from_bits(src: i8) -> Option<Errors> {
					match src {
						0 => Some(Errors::SensorsDataOverflow),
						1 => Some(Errors::CannotReadSensor),
						_ => None,
					}
				}
				pub fn bits(self) -> i8 { self as i8 }
			}
		}

		pub mod Communication {
			use crate::org::company::Client as packs;
			use crate::sys;
			use std::mem::transmute;

			pub trait IReceiver {
				/**
					* Getting reference to customer receiver by reference to sys::Receiver inside it. Mostly used self_by_field_ptr! macros
					**/
				unsafe fn into_self<'a>(receiver: *mut sys::Receiver) -> &'a mut Self;
				/**
					* Get internal sys::Receiver from Customer receiver
					**/
				fn get_receiver(&mut self) -> &mut sys::Receiver;
				/**
					* Received packs handler functions
					**/
				fn on_DeviceVersion(&mut self, pack: *mut sys::Pack);
				fn on_BusConfiguration(&mut self, pack: *mut sys::Pack);
				fn on_InstructionsPack(&mut self, pack: *mut sys::Pack);
				fn on_DeviceError(&mut self, pack: *mut sys::Pack);
				fn on_SensorsData(&mut self, pack: *mut sys::Pack);

				/**
					* Translate received byte(s) into pack(s)
					**/
				#[inline(always)]
				fn bytes_into_packs(&mut self, src: &[u8]) -> Result<usize, ::std::io::Error> {
					unsafe {
						sys::receive(src.as_ptr(), src.len(), self.get_receiver());
					}
					Ok(src.len() as usize)
				}
				/**
				 *Convenient  sys::Receiver constructor
				 **/
				fn new() -> sys::Receiver { sys::Receiver::new(Self::dispatch) }
				/**
					*Received packs to packs handler functions dispatcher
					**/
				unsafe extern "C" fn dispatch(receiver: *mut sys::Receiver, id: usize, pack: *mut sys::Pack) -> *const sys::Meta {
					let self_ = Self::into_self(receiver);
					match id {
						0 => {
							if pack.is_null() {
								return packs::DeviceVersion::meta_;
							}
							Self::on_DeviceVersion(self_, pack);
						}
						6 => {
							if pack.is_null() {
								return packs::BusConfiguration::meta_;
							}
							Self::on_BusConfiguration(self_, pack);
						}
						7 => {
							if pack.is_null() {
								return packs::InstructionsPack::meta_;
							}
							Self::on_InstructionsPack(self_, pack);
						}
						8 => {
							if pack.is_null() {
								return packs::DeviceError::meta_;
							}
							Self::on_DeviceError(self_, pack);
						}
						9 => {
							if pack.is_null() {
								return packs::SensorsData::meta_;
							}
							Self::on_SensorsData(self_, pack);
						}

						_ => panic!("unknown ID"),
					}

					::std::ptr::null_mut()
				}
			}

			pub trait ITransmitter {
				/**
					*Function that fetch next pack from sending queue to send it
					**/
				unsafe extern "C" fn dequeue(dst: *mut sys::Transmitter) -> *const sys::Pack;
				/**
				 *Add pack to the sending queue
				 **/
				fn queue(&mut self, pack: *mut sys::Pack) -> Result<(), *mut sys::Pack>;
				/**
				 *  Get reference to transmitter by customer transmitter
				 **/
				fn get_transmitter(&mut self) -> &mut sys::Transmitter;
				#[inline(always)]
				fn send_Stop(&mut self) -> bool { self.queue(sys::Pack::new(packs::Stop::meta_)).is_ok() }
				#[inline(always)]
				fn send_Start(&mut self) -> bool { self.queue(sys::Pack::new(packs::Start::meta_)).is_ok() }
				#[inline(always)]
				fn send_GetDeviceVersion(&mut self) -> bool { self.queue(sys::Pack::new(packs::GetDeviceVersion::meta_)).is_ok() }
				#[inline(always)]
				fn send_GetConfiguration(&mut self) -> bool { self.queue(sys::Pack::new(packs::GetConfiguration::meta_)).is_ok() }
				#[inline(always)]
				fn send_SetConfiguration(&mut self) -> bool { self.queue(sys::Pack::new(packs::SetConfiguration::meta_)).is_ok() }

				fn send<SRC: ITransmittable::Pack>(&mut self, src: SRC) -> Result<(), *mut sys::Pack> { self.queue(src.pack()) }

				/**
				 *Request to translate packs in sending queue into provided bytes buffer
				 **/
				#[inline(always)]
				fn packs_into_bytes(&mut self, dst: &mut [u8]) -> Result<usize, ::std::io::Error> { unsafe { Ok(sys::transmit(self.get_transmitter(), dst.as_mut_ptr(), dst.len()) as usize) } }

				fn new() -> sys::Transmitter { sys::Transmitter::new(Self::dequeue) }
			}

			pub mod ITransmittable {

				use crate::org::company::Client as packs;
				use crate::sys;
				/**
				*Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */

				pub fn BusConfiguration() -> packs::BusConfiguration::Pack_ { packs::BusConfiguration::Pack_(sys::Pack::new(packs::BusConfiguration::meta_)) }
				/**
				*Mini assembler instructions buffer starts with a
				*				 RW-instruction byte, it switch device in the Read/Write mode and hold the number of Readings/Writings I2C Bus.
				*				 .
				*				 every Read instruction consume 2 bytes
				*				 address
				*				 register
				*				 .
				*				 every Write instruction consume 4 bytes
				*				 address
				*				 writing_value - 2 bytes
				*				 register
				*				 .
				*				 so InstructionsPack can hold maximum 127 Readings or 64 Writings
				*				 .
				*				 if RW- < 128 - it contains number of rest Readings  from I2C
				*				 if 128 < RW - it contains (RW - 128) number of rest Writings  into I2C
				*				 .
				*				 If this pack is not following after RequestConfiguration:
				*				 .
				*				 if it contains read Sensor data instructions
				*				 it will be proceeded many times until fill all SensorsData, and then SensorsData send
				*				 .
				*				 if it contains any write Sensors registers instructions
				*				 it will be proceeded only once */

				pub fn InstructionsPack() -> packs::InstructionsPack::Pack_ { packs::InstructionsPack::Pack_(sys::Pack::new(packs::InstructionsPack::meta_)) }

				pub trait Pack {
					fn pack(self) -> *mut sys::Pack;
				}
			}
		}
	}
}
