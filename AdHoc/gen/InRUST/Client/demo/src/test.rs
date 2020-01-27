
use ad_hoc_gen::org::company as host;
use host::Client as packs;
use ad_hoc_gen::sys;
use std::mem::transmute;
use std::sync::mpsc::*;
use host::Communication;

use host::Communication::IReceiver as Communication_IReceiver;
pub struct Communication_receiver {
	pub receiver: sys::Receiver,
}

impl Communication_receiver {
	pub fn new() -> Communication_receiver { Communication_receiver { receiver: <Communication_receiver as Communication::IReceiver>::new() } }
}

impl host::Communication::IReceiver for Communication_receiver {
	unsafe fn into_self<'a>(receiver: *mut sys::Receiver) -> &'a mut Self { ad_hoc_gen::sys::self_by_field_ptr!(receiver, receiver) }
	fn get_receiver(&mut self) -> &mut sys::Receiver { &mut self.receiver }

	fn on_DeviceVersion(&mut self, pack: *mut sys::Pack) {
		let mut data = packs::DeviceVersion::Data_(unsafe { (*pack).bytes.as_mut_ptr() });
		read_DeviceVersion(&mut data);
		println!("DeviceVersion")
	}

	fn on_BusConfiguration(&mut self, pack: *mut sys::Pack) {
		let mut data = packs::BusConfiguration::Data_(unsafe { (*pack).bytes.as_mut_ptr() });
		read_BusConfiguration(&mut data);
		println!("BusConfiguration")
	}

	fn on_InstructionsPack(&mut self, pack: *mut sys::Pack) {
		let mut data = packs::InstructionsPack::Data_(unsafe { (*pack).bytes.as_mut_ptr() });
		read_InstructionsPack(&mut data);
		println!("InstructionsPack")
	}

	fn on_DeviceError(&mut self, pack: *mut sys::Pack) {
		let mut data = packs::DeviceError::Data_(unsafe { (*pack).bytes.as_mut_ptr() });
		read_DeviceError(&mut data);
		println!("DeviceError")
	}

	fn on_SensorsData(&mut self, pack: *mut sys::Pack) {
		let mut data = packs::SensorsData::Data_(unsafe { (*pack).bytes.as_mut_ptr() });
		read_SensorsData(&mut data);
		println!("SensorsData")
	}
}
impl ::std::io::Write for Communication_receiver {
	#[inline(always)]
	fn write(&mut self, src: &[u8]) -> Result<usize, ::std::io::Error> { self.bytes_into_packs(src) }
	#[inline(always)]
	fn flush(&mut self) -> Result<(), ::std::io::Error> { Ok(()) }
}

use host::Communication::ITransmitter as Communication_ITransmitter;
pub struct Communication_transmitter {
	pub transmitter:   sys::Transmitter,
	pub sending_queue: (Sender<*mut sys::Pack>, Receiver<*mut sys::Pack>),
}

impl Communication_transmitter {
	pub fn new() -> Communication_transmitter { Communication_transmitter { transmitter: <Communication_transmitter as Communication::ITransmitter>::new(), sending_queue: channel::<*mut sys::Pack>() } }
}

impl host::Communication::ITransmitter for Communication_transmitter {
	unsafe extern "C" fn dequeue(dst: *mut sys::Transmitter) -> *const sys::Pack {
		let self_: &mut Self = ad_hoc_gen::sys::self_by_field_ptr!(dst, transmitter);

		if let Ok(mut pack) = self_.sending_queue.1.try_recv() {
			return pack;
		}
		return std::ptr::null_mut();
	}
	fn queue(&mut self, pack: *mut sys::Pack) -> Result<(), *mut sys::Pack> {
		if self.sending_queue.0.send(pack).is_err() {
			return Err(pack);
		}
		Ok(())
	}
	fn get_transmitter(&mut self) -> &mut sys::Transmitter { &mut self.transmitter }
}

impl ::std::io::Read for Communication_transmitter {
	#[inline(always)]
	fn read(&mut self, dst: &mut [u8]) -> Result<usize, ::std::io::Error> { self.packs_into_bytes(dst) }
}

pub mod STD_ {
	use ad_hoc_gen::org::company as host;
	use host::Client as packs;
	use ad_hoc_gen::sys;
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
		fn on_Stop(&mut self);
		fn on_Start(&mut self);
		fn on_GetDeviceVersion(&mut self);
		fn on_GetConfiguration(&mut self);
		fn on_SetConfiguration(&mut self);

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
				1 => {
					if pack.is_null() {
						return host::Client::Stop::meta_;
					}
					Self::on_Stop(self_);
				}
				2 => {
					if pack.is_null() {
						return host::Client::Start::meta_;
					}
					Self::on_Start(self_);
				}
				3 => {
					if pack.is_null() {
						return host::Client::GetDeviceVersion::meta_;
					}
					Self::on_GetDeviceVersion(self_);
				}
				4 => {
					if pack.is_null() {
						return host::Client::GetConfiguration::meta_;
					}
					Self::on_GetConfiguration(self_);
				}
				5 => {
					if pack.is_null() {
						return host::Client::SetConfiguration::meta_;
					}
					Self::on_SetConfiguration(self_);
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

		fn send<SRC: ITransmittable::Pack>(&mut self, src: SRC) -> Result<(), *mut sys::Pack> { self.queue(src.pack()) }

		/**
		 *Request to translate packs in sending queue into provided bytes buffer
		 **/
		#[inline(always)]
		fn packs_into_bytes(&mut self, dst: &mut [u8]) -> Result<usize, ::std::io::Error> { unsafe { Ok(sys::transmit(self.get_transmitter(), dst.as_mut_ptr(), dst.len()) as usize) } }

		fn new() -> sys::Transmitter { sys::Transmitter::new(Self::dequeue) }
	}

	pub mod ITransmittable {

		use crate::test::utils::*;
		use ad_hoc_gen::org::company as host;
		use host::Client as packs;
		use ad_hoc_gen::sys;

		pub fn DeviceVersion() -> DeviceVersion::Pack { DeviceVersion::Pack(sys::Pack::new(host::Client::DeviceVersion::meta_)) }

		pub fn DeviceError() -> DeviceError::Pack { DeviceError::Pack(sys::Pack::new(host::Client::DeviceError::meta_)) }

		pub fn SensorsData() -> SensorsData::Pack { SensorsData::Pack(sys::Pack::new(host::Client::SensorsData::meta_)) }

		pub trait Pack {
			fn pack(self) -> *mut sys::Pack;
		}
	}
}

use STD_::IReceiver as STD__IReceiver;
pub struct STD__receiver {
	pub receiver: sys::Receiver,
}

impl STD__receiver {
	pub fn new() -> STD__receiver { STD__receiver { receiver: <STD__receiver as STD_::IReceiver>::new() } }
}

impl STD_::IReceiver for STD__receiver {
	unsafe fn into_self<'a>(receiver: *mut sys::Receiver) -> &'a mut Self { ad_hoc_gen::sys::self_by_field_ptr!(receiver, receiver) }
	fn get_receiver(&mut self) -> &mut sys::Receiver { &mut self.receiver }
	fn on_Stop(&mut self) { println!("Stop") }
	fn on_Start(&mut self) { println!("Start") }
	fn on_GetDeviceVersion(&mut self) { println!("GetDeviceVersion") }
	fn on_GetConfiguration(&mut self) { println!("GetConfiguration") }
	fn on_SetConfiguration(&mut self) { println!("SetConfiguration") }
}
impl ::std::io::Write for STD__receiver {
	#[inline(always)]
	fn write(&mut self, src: &[u8]) -> Result<usize, ::std::io::Error> { self.bytes_into_packs(src) }
	#[inline(always)]
	fn flush(&mut self) -> Result<(), ::std::io::Error> { Ok(()) }
}

use STD_::ITransmitter as STD__ITransmitter;
pub struct STD__transmitter {
	pub transmitter:   sys::Transmitter,
	pub sending_queue: (Sender<*mut sys::Pack>, Receiver<*mut sys::Pack>),
}

impl STD__transmitter {
	pub fn new() -> STD__transmitter { STD__transmitter { transmitter: <STD__transmitter as STD_::ITransmitter>::new(), sending_queue: channel::<*mut sys::Pack>() } }
}

impl STD_::ITransmitter for STD__transmitter {
	unsafe extern "C" fn dequeue(dst: *mut sys::Transmitter) -> *const sys::Pack {
		let self_: &mut Self = ad_hoc_gen::sys::self_by_field_ptr!(dst, transmitter);

		if let Ok(mut pack) = self_.sending_queue.1.try_recv() {
			return pack;
		}
		return std::ptr::null_mut();
	}
	fn queue(&mut self, pack: *mut sys::Pack) -> Result<(), *mut sys::Pack> {
		if self.sending_queue.0.send(pack).is_err() {
			return Err(pack);
		}
		Ok(())
	}
	fn get_transmitter(&mut self) -> &mut sys::Transmitter { &mut self.transmitter }
}

impl ::std::io::Read for STD__transmitter {
	#[inline(always)]
	fn read(&mut self, dst: &mut [u8]) -> Result<usize, ::std::io::Error> { self.packs_into_bytes(dst) }
}

pub mod utils {
	use ad_hoc_gen::org::company as host;
	use host::Client as packs;
	use ad_hoc_gen::sys;

	use host::Client::Errors;

	pub mod DeviceVersion {
		use ad_hoc_gen::sys;
		use super::*;

		pub struct Pack(pub *mut sys::Pack);

		impl crate::test::STD_::ITransmittable::Pack for Pack {
			fn pack(self) -> *mut sys::Pack { self.0 }
		}
		impl Pack {
			pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
		}
		pub struct Data_(pub *mut u8);
		impl Data_ {
			pub fn DevID(&mut self) -> Item_y { Item_y(self.0) }
			pub fn Ver(&mut self) -> Item_a { Item_a(self.0) }
		}
	}
	///[DevID](Client::DeviceVersion::DevID).

	pub struct Item_y(pub *mut u8);
	impl Item_y {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[Ver](Client::DeviceVersion::Ver).

	pub struct Item_a(pub *mut u8);
	impl Item_a {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 2, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 2); }
	}
	pub mod DeviceError {
		use ad_hoc_gen::sys;
		use super::*;

		pub struct Pack(pub *mut sys::Pack);

		impl crate::test::STD_::ITransmittable::Pack for Pack {
			fn pack(self) -> *mut sys::Pack { self.0 }
		}
		impl Pack {
			pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
		}
		pub struct Data_(pub *mut u8);
		impl Data_ {
			pub fn param1(&mut self) -> Item_R { Item_R(self.0) }
			pub fn error_id(&mut self) -> Item_K { Item_K(self.0) }
		}
	}
	///[param1](Client::DeviceError::param1).

	pub struct Item_R(pub *mut u8);
	impl Item_R {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[error_id](Client::DeviceError::error_id).

	pub struct Item_K(pub *mut u8);
	impl Item_K {
		pub fn get(&mut self) -> crate::test::utils::Errors {
			let src = &mut self.0;
			crate::test::utils::Errors::from_bits((sys::get_bits(self.0, 16, 1)) as i8).unwrap()
		}
		pub fn set(&mut self, src: crate::test::utils::Errors) { sys::set_bits((src.bits()) as u64, 1, self.0, 16); }
	}
	pub mod SensorsData {
		use ad_hoc_gen::sys;
		use super::*;

		pub struct Pack(pub *mut sys::Pack);

		impl crate::test::STD_::ITransmittable::Pack for Pack {
			fn pack(self) -> *mut sys::Pack { self.0 }
		}
		impl Pack {
			pub fn data(&self) -> Data_ { Data_(unsafe { (*self.0).bytes.as_mut_ptr() }) }
		}
		pub struct Data_(pub *mut u8);
		impl Data_ {
			pub fn values(&mut self, src: Option<&mut dyn Iterator<Item = u16>>) -> ItemArraySET_J {
				let dst = self.0;

				let len = 1000 as usize;
				let offset = 0;
				let bytes = dst;
				let mut array = ItemArraySET_J { bytes, len, offset, index: !0 };
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
	}

	///[values](Client::SensorsData::values).

	pub struct ItemArraySET_J {
		pub bytes:  *mut u8,
		pub len:    usize,
		pub offset: usize,
		pub index:  usize,
	}
	impl ItemArraySET_J {
		pub fn get(&mut self, index: usize) -> u16 {
			let dst = sys::get_bytes(self.bytes, self.offset + index * 2, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, index: usize, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.bytes, self.offset + index * 2); }
	}

	impl Iterator for ItemArraySET_J {
		type Item = u16;

		fn next(&mut self) -> Option<<Self as Iterator>::Item> {
			self.index = self.index.wrapping_add(1);
			if self.index < self.len {
				return Some(self.get(self.index));
			}
			self.index = !0;
			None
		}
	}
}

fn write_DeviceVersion(dst: &mut crate::test::utils::DeviceVersion::Data_) {
	dst.DevID().set(5689_u16);
	dst.Ver().set(37862_u16);
}
fn read_DeviceVersion(src: &mut packs::DeviceVersion::Data_) {
	assert_eq!(src.DevID(), 5689_u16);
	assert_eq!(src.Ver(), 37862_u16);
}
fn on_Stop() {}
fn on_Start() {}
fn on_GetDeviceVersion() {}
fn on_GetConfiguration() {}
fn on_SetConfiguration() {}
fn write_BusConfiguration(dst: &mut packs::BusConfiguration::Data_) {
	dst.multiplier().set(168_u8);
	dst.time().set(11367_u16);
	dst.clk_khz().set(9795_u16);
}
fn read_BusConfiguration(src: &mut packs::BusConfiguration::Data_) {
	assert_eq!(src.multiplier().get(), 168_u8);
	assert_eq!(src.time().get(), 11367_u16);
	assert_eq!(src.clk_khz().get(), 9795_u16);
}

fn write_InstructionsPack(dst: &mut packs::InstructionsPack::Data_) {
	dst.Length().set(107_u8);
	let norm: [u8; 256] = [
	                       184_u8, 26_u8, 82_u8, 112_u8, 209_u8, 216_u8, 237_u8, 214_u8, 86_u8, 53_u8, 90_u8, 32_u8, 140_u8, 135_u8, 203_u8, 133_u8, 12_u8, 142_u8, 47_u8, 36_u8, 155_u8, 111_u8, 40_u8, 66_u8, 189_u8, 154_u8, 26_u8, 100_u8, 117_u8, 149_u8, 217_u8, 221_u8, 201_u8, 128_u8, 195_u8, 247_u8, 235_u8, 163_u8, 34_u8, 92_u8, 243_u8, 215_u8, 18_u8, 170_u8, 128_u8, 129_u8, 251_u8, 7_u8, 161_u8, 122_u8, 225_u8, 87_u8, 192_u8, 125_u8, 188_u8, 131_u8, 100_u8, 158_u8, 28_u8, 172_u8, 251_u8, 204_u8, 149_u8, 72_u8, 93_u8, 213_u8, 83_u8, 29_u8, 21_u8, 199_u8, 138_u8, 50_u8, 3_u8, 153_u8, 1_u8, 167_u8, 93_u8, 245_u8, 181_u8, 251_u8, 8_u8, 110_u8, 144_u8, 231_u8, 26_u8, 1_u8, 123_u8, 244_u8, 12_u8, 79_u8, 116_u8, 199_u8, 237_u8, 240_u8, 237_u8, 157_u8, 250_u8, 184_u8, 134_u8, 139_u8, 238_u8, 14_u8, 138_u8, 181_u8, 176_u8, 104_u8, 81_u8, 30_u8, 122_u8, 72_u8, 135_u8, 82_u8, 10_u8, 254_u8, 190_u8, 0_u8, 83_u8, 188_u8, 152_u8, 44_u8, 126_u8, 174_u8, 33_u8, 14_u8, 250_u8, 99_u8, 9_u8, 90_u8, 228_u8, 35_u8, 131_u8, 141_u8, 160_u8, 234_u8, 161_u8, 101_u8, 145_u8, 13_u8, 59_u8, 63_u8, 201_u8, 232_u8, 214_u8, 49_u8, 159_u8, 166_u8, 190_u8, 149_u8, 230_u8, 21_u8, 106_u8, 126_u8, 108_u8, 73_u8,
	                       233_u8, 171_u8, 190_u8, 155_u8, 113_u8, 254_u8, 149_u8, 113_u8, 13_u8, 14_u8, 241_u8, 127_u8, 91_u8, 63_u8, 51_u8, 239_u8, 146_u8, 65_u8, 59_u8, 22_u8, 83_u8, 62_u8, 199_u8, 120_u8, 25_u8, 140_u8, 252_u8, 176_u8, 8_u8, 52_u8, 81_u8, 253_u8, 83_u8, 102_u8, 42_u8, 164_u8, 9_u8, 254_u8, 207_u8, 153_u8, 220_u8, 12_u8, 64_u8, 99_u8, 196_u8, 132_u8, 228_u8, 245_u8, 156_u8, 197_u8, 148_u8, 9_u8, 221_u8, 249_u8, 238_u8, 140_u8, 238_u8, 215_u8, 155_u8, 248_u8, 141_u8, 178_u8, 144_u8, 113_u8, 90_u8, 94_u8, 124_u8, 123_u8, 225_u8, 50_u8, 185_u8, 16_u8, 101_u8, 221_u8, 193_u8, 22_u8, 189_u8, 41_u8, 8_u8, 185_u8, 77_u8, 44_u8, 109_u8, 17_u8, 36_u8, 72_u8, 108_u8, 86_u8, 141_u8, 144_u8, 90_u8, 228_u8, 59_u8, 29_u8, 98_u8, 47_u8, 50_u8, 96_u8, 57_u8, 52_u8, 227_u8, 67_u8,
	];
	dst.Instructions(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_InstructionsPack(src: &mut packs::InstructionsPack::Data_) {
	assert_eq!(src.Length().get(), 107_u8);
	let norm: [u8; 256] = [
	                       184_u8, 26_u8, 82_u8, 112_u8, 209_u8, 216_u8, 237_u8, 214_u8, 86_u8, 53_u8, 90_u8, 32_u8, 140_u8, 135_u8, 203_u8, 133_u8, 12_u8, 142_u8, 47_u8, 36_u8, 155_u8, 111_u8, 40_u8, 66_u8, 189_u8, 154_u8, 26_u8, 100_u8, 117_u8, 149_u8, 217_u8, 221_u8, 201_u8, 128_u8, 195_u8, 247_u8, 235_u8, 163_u8, 34_u8, 92_u8, 243_u8, 215_u8, 18_u8, 170_u8, 128_u8, 129_u8, 251_u8, 7_u8, 161_u8, 122_u8, 225_u8, 87_u8, 192_u8, 125_u8, 188_u8, 131_u8, 100_u8, 158_u8, 28_u8, 172_u8, 251_u8, 204_u8, 149_u8, 72_u8, 93_u8, 213_u8, 83_u8, 29_u8, 21_u8, 199_u8, 138_u8, 50_u8, 3_u8, 153_u8, 1_u8, 167_u8, 93_u8, 245_u8, 181_u8, 251_u8, 8_u8, 110_u8, 144_u8, 231_u8, 26_u8, 1_u8, 123_u8, 244_u8, 12_u8, 79_u8, 116_u8, 199_u8, 237_u8, 240_u8, 237_u8, 157_u8, 250_u8, 184_u8, 134_u8, 139_u8, 238_u8, 14_u8, 138_u8, 181_u8, 176_u8, 104_u8, 81_u8, 30_u8, 122_u8, 72_u8, 135_u8, 82_u8, 10_u8, 254_u8, 190_u8, 0_u8, 83_u8, 188_u8, 152_u8, 44_u8, 126_u8, 174_u8, 33_u8, 14_u8, 250_u8, 99_u8, 9_u8, 90_u8, 228_u8, 35_u8, 131_u8, 141_u8, 160_u8, 234_u8, 161_u8, 101_u8, 145_u8, 13_u8, 59_u8, 63_u8, 201_u8, 232_u8, 214_u8, 49_u8, 159_u8, 166_u8, 190_u8, 149_u8, 230_u8, 21_u8, 106_u8, 126_u8, 108_u8, 73_u8,
	                       233_u8, 171_u8, 190_u8, 155_u8, 113_u8, 254_u8, 149_u8, 113_u8, 13_u8, 14_u8, 241_u8, 127_u8, 91_u8, 63_u8, 51_u8, 239_u8, 146_u8, 65_u8, 59_u8, 22_u8, 83_u8, 62_u8, 199_u8, 120_u8, 25_u8, 140_u8, 252_u8, 176_u8, 8_u8, 52_u8, 81_u8, 253_u8, 83_u8, 102_u8, 42_u8, 164_u8, 9_u8, 254_u8, 207_u8, 153_u8, 220_u8, 12_u8, 64_u8, 99_u8, 196_u8, 132_u8, 228_u8, 245_u8, 156_u8, 197_u8, 148_u8, 9_u8, 221_u8, 249_u8, 238_u8, 140_u8, 238_u8, 215_u8, 155_u8, 248_u8, 141_u8, 178_u8, 144_u8, 113_u8, 90_u8, 94_u8, 124_u8, 123_u8, 225_u8, 50_u8, 185_u8, 16_u8, 101_u8, 221_u8, 193_u8, 22_u8, 189_u8, 41_u8, 8_u8, 185_u8, 77_u8, 44_u8, 109_u8, 17_u8, 36_u8, 72_u8, 108_u8, 86_u8, 141_u8, 144_u8, 90_u8, 228_u8, 59_u8, 29_u8, 98_u8, 47_u8, 50_u8, 96_u8, 57_u8, 52_u8, 227_u8, 67_u8,
	];
	assert!(src.Instructions(None).eq(sys::ByValIter::new(&norm)));
}

fn write_DeviceError(dst: &mut crate::test::utils::DeviceError::Data_) {
	dst.param1().set(860_u16);
	dst.error_id().set(packs::Errors::CannotReadSensor);
}
fn read_DeviceError(src: &mut packs::DeviceError::Data_) {
	assert_eq!(src.param1(), 860_u16);
	assert_eq!(src.error_id(), packs::Errors::CannotReadSensor);
}

fn write_SensorsData(dst: &mut crate::test::utils::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         53872_u16, 3047_u16, 10471_u16, 27008_u16, 42328_u16, 23025_u16, 37409_u16, 57434_u16, 50433_u16, 333_u16, 25836_u16, 57937_u16, 60805_u16, 19721_u16, 52146_u16, 39873_u16, 32059_u16, 37325_u16, 56580_u16, 30595_u16, 61142_u16, 35498_u16, 51912_u16, 6735_u16, 15154_u16, 28798_u16, 53378_u16, 52680_u16, 45982_u16, 39060_u16, 54375_u16, 24243_u16, 1980_u16, 19965_u16, 17100_u16, 41559_u16, 27491_u16, 12791_u16, 5751_u16, 32010_u16, 46216_u16, 49558_u16, 22383_u16, 44999_u16, 3504_u16, 63820_u16, 18793_u16, 3550_u16, 33020_u16, 65153_u16, 11718_u16, 44181_u16, 3214_u16, 7203_u16, 56073_u16, 62048_u16, 44894_u16, 27466_u16, 49316_u16, 43326_u16, 28740_u16, 63633_u16, 35273_u16, 58422_u16, 26559_u16, 62323_u16, 54045_u16, 28268_u16, 62462_u16, 54809_u16, 43936_u16, 51915_u16, 8151_u16, 55737_u16, 13045_u16, 42435_u16, 46803_u16, 63538_u16, 7970_u16, 45479_u16, 28519_u16, 57293_u16, 25958_u16, 27128_u16, 33014_u16, 37206_u16, 14509_u16, 16358_u16, 16538_u16, 48734_u16, 62945_u16, 24213_u16, 41620_u16, 14251_u16, 41481_u16, 62030_u16, 4144_u16, 30087_u16, 23306_u16, 43718_u16, 63976_u16, 972_u16, 44760_u16, 41514_u16, 35198_u16, 11063_u16, 20694_u16,
	                         6040_u16, 39760_u16, 49426_u16, 60820_u16, 44735_u16, 48637_u16, 56986_u16, 48077_u16, 48927_u16, 52066_u16, 36246_u16, 6743_u16, 10631_u16, 51519_u16, 53493_u16, 39668_u16, 12720_u16, 32664_u16, 22488_u16, 17431_u16, 65410_u16, 23195_u16, 35355_u16, 37334_u16, 38276_u16, 45882_u16, 31167_u16, 3088_u16, 34899_u16, 48997_u16, 43371_u16, 27559_u16, 6034_u16, 64061_u16, 58737_u16, 7552_u16, 52063_u16, 50303_u16, 35257_u16, 42863_u16, 31987_u16, 65373_u16, 7503_u16, 49373_u16, 8584_u16, 45069_u16, 21189_u16, 35230_u16, 47465_u16, 60525_u16, 49140_u16, 53347_u16, 41254_u16, 56551_u16, 47155_u16, 34449_u16, 13819_u16, 61832_u16, 35028_u16, 5848_u16, 48134_u16, 17267_u16, 65051_u16, 32626_u16, 16144_u16, 34568_u16, 35879_u16, 19314_u16, 35992_u16, 61717_u16, 24870_u16, 1953_u16, 49995_u16, 15023_u16, 4996_u16, 4609_u16, 11589_u16, 24830_u16, 35275_u16, 13925_u16, 51974_u16, 55021_u16, 29635_u16, 15323_u16, 52679_u16, 12812_u16, 63847_u16, 35326_u16, 19649_u16, 40934_u16, 23447_u16, 17206_u16, 39434_u16, 22277_u16, 42177_u16, 56933_u16, 4932_u16, 42613_u16, 33444_u16, 40628_u16, 23479_u16, 8927_u16, 47349_u16, 53716_u16, 42093_u16, 39274_u16, 35928_u16,
	                         41501_u16, 54161_u16, 31072_u16, 62245_u16, 26827_u16, 1197_u16, 47097_u16, 18792_u16, 64104_u16, 54423_u16, 59137_u16, 40550_u16, 25449_u16, 56116_u16, 41397_u16, 32765_u16, 32229_u16, 18865_u16, 62296_u16, 28267_u16, 36643_u16, 19344_u16, 49365_u16, 28629_u16, 7313_u16, 24306_u16, 46619_u16, 59320_u16, 8353_u16, 48442_u16, 35798_u16, 647_u16, 20166_u16, 65526_u16, 62659_u16, 14842_u16, 55602_u16, 26795_u16, 20372_u16, 57275_u16, 22162_u16, 51320_u16, 39713_u16, 58920_u16, 23685_u16, 40472_u16, 53277_u16, 30848_u16, 20998_u16, 24234_u16, 2530_u16, 35394_u16, 48642_u16, 4675_u16, 18070_u16, 61648_u16, 1802_u16, 9083_u16, 42734_u16, 4564_u16, 52479_u16, 29716_u16, 26868_u16, 20864_u16, 14390_u16, 52909_u16, 23742_u16, 9461_u16, 61481_u16, 9236_u16, 49630_u16, 40995_u16, 7518_u16, 36112_u16, 62250_u16, 15594_u16, 12634_u16, 54605_u16, 27697_u16, 12033_u16, 32479_u16, 29623_u16, 31815_u16, 33127_u16, 23638_u16, 15506_u16, 37564_u16, 10787_u16, 42175_u16, 35423_u16, 47658_u16, 38673_u16, 60520_u16, 19056_u16, 13430_u16, 12126_u16, 5558_u16, 59408_u16, 25532_u16, 14149_u16, 16065_u16, 15338_u16, 56849_u16, 18409_u16, 54929_u16, 48643_u16, 57106_u16,
	                         29194_u16, 53982_u16, 50018_u16, 65210_u16, 37300_u16, 13899_u16, 43897_u16, 51633_u16, 63562_u16, 1746_u16, 4723_u16, 55619_u16, 39797_u16, 55634_u16, 48709_u16, 60849_u16, 20989_u16, 27636_u16, 18742_u16, 57037_u16, 15183_u16, 863_u16, 1167_u16, 56915_u16, 8812_u16, 53636_u16, 39451_u16, 11639_u16, 41164_u16, 56798_u16, 54617_u16, 65187_u16, 6128_u16, 27500_u16, 32008_u16, 42841_u16, 13332_u16, 65347_u16, 13671_u16, 58767_u16, 34887_u16, 46066_u16, 17067_u16, 33744_u16, 8474_u16, 17377_u16, 44119_u16, 27753_u16, 38566_u16, 17597_u16, 24421_u16, 32926_u16, 11273_u16, 38464_u16, 21799_u16, 908_u16, 19992_u16, 10659_u16, 44272_u16, 63011_u16, 62346_u16, 37596_u16, 8671_u16, 3835_u16, 17679_u16, 18741_u16, 10305_u16, 21160_u16, 48223_u16, 40128_u16, 39176_u16, 52560_u16, 21520_u16, 7553_u16, 35249_u16, 39841_u16, 63075_u16, 3030_u16, 44701_u16, 34275_u16, 41659_u16, 49049_u16, 10297_u16, 15892_u16, 21106_u16, 60216_u16, 2089_u16, 14769_u16, 6358_u16, 60564_u16, 41957_u16, 24131_u16, 5513_u16, 42913_u16, 48127_u16, 44877_u16, 53082_u16, 855_u16, 12796_u16, 25274_u16, 13371_u16, 18670_u16, 50130_u16, 1416_u16, 50729_u16, 31188_u16, 19148_u16, 15919_u16,
	                         26197_u16, 29614_u16, 1780_u16, 8624_u16, 32184_u16, 54818_u16, 19224_u16, 5923_u16, 1839_u16, 37250_u16, 34793_u16, 23604_u16, 44632_u16, 10188_u16, 36391_u16, 52574_u16, 62074_u16, 26003_u16, 31651_u16, 24147_u16, 53291_u16, 3552_u16, 60689_u16, 2653_u16, 1791_u16, 42080_u16, 17114_u16, 55607_u16, 3413_u16, 48152_u16, 44951_u16, 14524_u16, 46581_u16, 16003_u16, 9726_u16, 54199_u16, 44606_u16, 4049_u16, 34628_u16, 19844_u16, 59999_u16, 20525_u16, 11347_u16, 40145_u16, 31770_u16, 7216_u16, 48940_u16, 26181_u16, 18206_u16, 12655_u16, 18186_u16, 28487_u16, 45007_u16, 44989_u16, 57762_u16, 65136_u16, 51608_u16, 58808_u16, 18094_u16, 53184_u16, 20675_u16, 51118_u16, 51866_u16, 22823_u16, 35662_u16, 22162_u16, 37402_u16, 40175_u16, 8008_u16, 29958_u16, 2889_u16, 56960_u16, 56445_u16, 1079_u16, 24463_u16, 2930_u16, 35545_u16, 28366_u16, 6567_u16, 1128_u16, 57910_u16, 56630_u16, 38037_u16, 64040_u16, 48454_u16, 27871_u16, 51603_u16, 14440_u16, 2626_u16, 53658_u16, 59303_u16, 33923_u16, 31453_u16, 3224_u16, 24717_u16, 37327_u16, 57241_u16, 40624_u16, 57263_u16, 6747_u16, 50162_u16, 42663_u16, 19392_u16, 23687_u16, 957_u16, 6993_u16, 44831_u16, 5276_u16,
	                         48_u16, 10269_u16, 2582_u16, 31038_u16, 45947_u16, 53345_u16, 20922_u16, 11445_u16, 34674_u16, 61001_u16, 6943_u16, 54465_u16, 43138_u16, 11945_u16, 1542_u16, 18560_u16, 10446_u16, 21983_u16, 18276_u16, 10129_u16, 36366_u16, 62443_u16, 48551_u16, 42216_u16, 29601_u16, 7612_u16, 31306_u16, 4274_u16, 25533_u16, 20954_u16, 40685_u16, 61400_u16, 24193_u16, 57776_u16, 40911_u16, 20618_u16, 16738_u16, 9521_u16, 29715_u16, 62778_u16, 13824_u16, 44731_u16, 25088_u16, 33899_u16, 3926_u16, 2463_u16, 13949_u16, 41250_u16, 39929_u16, 23241_u16, 106_u16, 30710_u16, 31674_u16, 7113_u16, 46454_u16, 58125_u16, 35548_u16, 38787_u16, 2680_u16, 59272_u16, 9908_u16, 24118_u16, 6873_u16, 15294_u16, 53110_u16, 12517_u16, 52696_u16, 64242_u16, 60240_u16, 16381_u16, 1371_u16, 61338_u16, 14155_u16, 41238_u16, 18490_u16, 1991_u16, 39466_u16, 3416_u16, 10496_u16, 37313_u16, 26458_u16, 40490_u16, 50090_u16, 26751_u16, 10263_u16, 49900_u16, 8331_u16, 34981_u16, 24533_u16, 29710_u16, 8363_u16, 48057_u16, 48923_u16, 4986_u16, 3841_u16, 13171_u16, 1091_u16, 5612_u16, 51408_u16, 16900_u16, 50467_u16, 45662_u16, 44680_u16, 28327_u16, 6015_u16, 47739_u16, 35855_u16, 42839_u16,
	                         4864_u16, 14966_u16, 4381_u16, 56803_u16, 37555_u16, 45433_u16, 8223_u16, 34034_u16, 23325_u16, 38331_u16, 45183_u16, 11587_u16, 52705_u16, 56943_u16, 42831_u16, 34128_u16, 22320_u16, 33357_u16, 154_u16, 55830_u16, 36234_u16, 58516_u16, 57258_u16, 57110_u16, 869_u16, 59158_u16, 61874_u16, 12750_u16, 14953_u16, 50500_u16, 51023_u16, 27592_u16, 26180_u16, 9208_u16, 48205_u16, 2115_u16, 59059_u16, 10072_u16, 15778_u16, 58530_u16, 4503_u16, 202_u16, 56918_u16, 25377_u16, 46712_u16, 61951_u16, 28470_u16, 16921_u16, 44758_u16, 52873_u16, 29688_u16, 7379_u16, 46236_u16, 48943_u16, 61964_u16, 36635_u16, 48785_u16, 12901_u16, 26252_u16, 60741_u16, 10367_u16, 34705_u16, 24522_u16, 60149_u16, 5388_u16, 46574_u16, 21219_u16, 2136_u16, 7569_u16, 45924_u16, 24405_u16, 47880_u16, 12655_u16, 12300_u16, 59846_u16, 30557_u16, 10085_u16, 1335_u16, 53939_u16, 5180_u16, 50345_u16, 39103_u16, 59304_u16, 36552_u16, 23943_u16, 22190_u16, 15889_u16, 11407_u16, 3794_u16, 34445_u16, 7261_u16, 83_u16, 60308_u16, 62974_u16, 36890_u16, 46631_u16, 25862_u16, 31035_u16, 6399_u16, 7106_u16, 15079_u16, 59074_u16, 55879_u16, 12595_u16, 38999_u16, 57563_u16, 10969_u16, 42294_u16,
	                         45537_u16, 2059_u16, 14858_u16, 43536_u16, 55425_u16, 22599_u16, 48043_u16, 21951_u16, 63709_u16, 53903_u16, 3378_u16, 40654_u16, 41132_u16, 12845_u16, 58245_u16, 20779_u16, 54761_u16, 6785_u16, 39985_u16, 5276_u16, 24745_u16, 26822_u16, 25376_u16, 57885_u16, 29075_u16, 49037_u16, 1977_u16, 10406_u16, 45313_u16, 46811_u16, 25931_u16, 31954_u16, 50427_u16, 58461_u16, 45980_u16, 54019_u16, 57245_u16, 18794_u16, 22951_u16, 42985_u16, 25861_u16, 56741_u16, 37082_u16, 10467_u16, 61203_u16, 36164_u16, 46822_u16, 13688_u16, 56410_u16, 39510_u16, 46548_u16, 26721_u16, 8397_u16, 31946_u16, 25239_u16, 10816_u16, 41067_u16, 23877_u16, 23567_u16, 9788_u16, 48175_u16, 12906_u16, 32488_u16, 24785_u16, 61243_u16, 30658_u16, 46932_u16, 23056_u16, 52425_u16, 6442_u16, 53930_u16, 48942_u16, 56546_u16, 43735_u16, 25413_u16, 34055_u16, 13018_u16, 15373_u16, 53077_u16, 53253_u16, 8388_u16, 54452_u16, 20179_u16, 52091_u16, 26251_u16, 49789_u16, 15079_u16, 42958_u16, 16241_u16, 25993_u16, 7101_u16, 62716_u16, 12866_u16, 39408_u16, 50847_u16, 58482_u16, 41643_u16, 9428_u16, 44576_u16, 56156_u16, 43875_u16, 33720_u16, 23237_u16, 24089_u16, 55984_u16, 14612_u16, 60920_u16,
	                         5023_u16, 44528_u16, 46877_u16, 19653_u16, 51625_u16, 59823_u16, 12623_u16, 21072_u16, 39660_u16, 62900_u16, 63253_u16, 20580_u16, 34375_u16, 39721_u16, 33371_u16, 11336_u16, 18701_u16, 29145_u16, 53088_u16, 30353_u16, 30843_u16, 26260_u16, 49071_u16, 48250_u16, 1096_u16, 38093_u16, 55988_u16, 9828_u16, 50384_u16, 26987_u16, 30914_u16, 28006_u16, 59370_u16, 21325_u16, 32481_u16, 56678_u16, 46546_u16, 35363_u16, 47284_u16, 61855_u16, 32642_u16, 7272_u16, 52659_u16, 8673_u16, 6263_u16, 44091_u16, 42171_u16, 64749_u16, 37968_u16, 1264_u16, 19593_u16, 51546_u16, 7109_u16, 58768_u16, 6173_u16, 28390_u16, 64871_u16, 20089_u16, 692_u16, 11162_u16, 12474_u16, 5208_u16, 24655_u16, 21160_u16, 56022_u16, 46612_u16, 8337_u16, 28865_u16, 6280_u16, 43489_u16, 6424_u16, 4295_u16, 64367_u16, 52730_u16, 41622_u16, 65320_u16, 12619_u16, 59451_u16, 30_u16, 49277_u16, 61127_u16, 15654_u16, 41865_u16, 35419_u16, 57915_u16, 42974_u16, 50474_u16, 1538_u16, 62990_u16, 44620_u16, 4786_u16, 9356_u16, 34730_u16, 43172_u16, 33829_u16, 12075_u16, 13804_u16, 41521_u16, 46490_u16, 37097_u16, 36175_u16, 55959_u16, 54479_u16, 26576_u16, 55620_u16, 60543_u16, 23897_u16, 37154_u16,
	                         236_u16, 11006_u16, 33561_u16, 63729_u16, 51074_u16, 3218_u16, 6724_u16, 34980_u16, 23930_u16, 9477_u16, 47078_u16, 21526_u16, 54721_u16, 50334_u16, 37917_u16, 41812_u16, 42010_u16, 17069_u16, 18441_u16, 15979_u16, 16705_u16, 53049_u16, 3126_u16, 20079_u16, 18628_u16, 36018_u16, 40826_u16, 10932_u16, 24517_u16, 54527_u16, 28523_u16, 3199_u16,
	];
	dst.values(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_SensorsData(src: &mut packs::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         53872_u16, 3047_u16, 10471_u16, 27008_u16, 42328_u16, 23025_u16, 37409_u16, 57434_u16, 50433_u16, 333_u16, 25836_u16, 57937_u16, 60805_u16, 19721_u16, 52146_u16, 39873_u16, 32059_u16, 37325_u16, 56580_u16, 30595_u16, 61142_u16, 35498_u16, 51912_u16, 6735_u16, 15154_u16, 28798_u16, 53378_u16, 52680_u16, 45982_u16, 39060_u16, 54375_u16, 24243_u16, 1980_u16, 19965_u16, 17100_u16, 41559_u16, 27491_u16, 12791_u16, 5751_u16, 32010_u16, 46216_u16, 49558_u16, 22383_u16, 44999_u16, 3504_u16, 63820_u16, 18793_u16, 3550_u16, 33020_u16, 65153_u16, 11718_u16, 44181_u16, 3214_u16, 7203_u16, 56073_u16, 62048_u16, 44894_u16, 27466_u16, 49316_u16, 43326_u16, 28740_u16, 63633_u16, 35273_u16, 58422_u16, 26559_u16, 62323_u16, 54045_u16, 28268_u16, 62462_u16, 54809_u16, 43936_u16, 51915_u16, 8151_u16, 55737_u16, 13045_u16, 42435_u16, 46803_u16, 63538_u16, 7970_u16, 45479_u16, 28519_u16, 57293_u16, 25958_u16, 27128_u16, 33014_u16, 37206_u16, 14509_u16, 16358_u16, 16538_u16, 48734_u16, 62945_u16, 24213_u16, 41620_u16, 14251_u16, 41481_u16, 62030_u16, 4144_u16, 30087_u16, 23306_u16, 43718_u16, 63976_u16, 972_u16, 44760_u16, 41514_u16, 35198_u16, 11063_u16, 20694_u16,
	                         6040_u16, 39760_u16, 49426_u16, 60820_u16, 44735_u16, 48637_u16, 56986_u16, 48077_u16, 48927_u16, 52066_u16, 36246_u16, 6743_u16, 10631_u16, 51519_u16, 53493_u16, 39668_u16, 12720_u16, 32664_u16, 22488_u16, 17431_u16, 65410_u16, 23195_u16, 35355_u16, 37334_u16, 38276_u16, 45882_u16, 31167_u16, 3088_u16, 34899_u16, 48997_u16, 43371_u16, 27559_u16, 6034_u16, 64061_u16, 58737_u16, 7552_u16, 52063_u16, 50303_u16, 35257_u16, 42863_u16, 31987_u16, 65373_u16, 7503_u16, 49373_u16, 8584_u16, 45069_u16, 21189_u16, 35230_u16, 47465_u16, 60525_u16, 49140_u16, 53347_u16, 41254_u16, 56551_u16, 47155_u16, 34449_u16, 13819_u16, 61832_u16, 35028_u16, 5848_u16, 48134_u16, 17267_u16, 65051_u16, 32626_u16, 16144_u16, 34568_u16, 35879_u16, 19314_u16, 35992_u16, 61717_u16, 24870_u16, 1953_u16, 49995_u16, 15023_u16, 4996_u16, 4609_u16, 11589_u16, 24830_u16, 35275_u16, 13925_u16, 51974_u16, 55021_u16, 29635_u16, 15323_u16, 52679_u16, 12812_u16, 63847_u16, 35326_u16, 19649_u16, 40934_u16, 23447_u16, 17206_u16, 39434_u16, 22277_u16, 42177_u16, 56933_u16, 4932_u16, 42613_u16, 33444_u16, 40628_u16, 23479_u16, 8927_u16, 47349_u16, 53716_u16, 42093_u16, 39274_u16, 35928_u16,
	                         41501_u16, 54161_u16, 31072_u16, 62245_u16, 26827_u16, 1197_u16, 47097_u16, 18792_u16, 64104_u16, 54423_u16, 59137_u16, 40550_u16, 25449_u16, 56116_u16, 41397_u16, 32765_u16, 32229_u16, 18865_u16, 62296_u16, 28267_u16, 36643_u16, 19344_u16, 49365_u16, 28629_u16, 7313_u16, 24306_u16, 46619_u16, 59320_u16, 8353_u16, 48442_u16, 35798_u16, 647_u16, 20166_u16, 65526_u16, 62659_u16, 14842_u16, 55602_u16, 26795_u16, 20372_u16, 57275_u16, 22162_u16, 51320_u16, 39713_u16, 58920_u16, 23685_u16, 40472_u16, 53277_u16, 30848_u16, 20998_u16, 24234_u16, 2530_u16, 35394_u16, 48642_u16, 4675_u16, 18070_u16, 61648_u16, 1802_u16, 9083_u16, 42734_u16, 4564_u16, 52479_u16, 29716_u16, 26868_u16, 20864_u16, 14390_u16, 52909_u16, 23742_u16, 9461_u16, 61481_u16, 9236_u16, 49630_u16, 40995_u16, 7518_u16, 36112_u16, 62250_u16, 15594_u16, 12634_u16, 54605_u16, 27697_u16, 12033_u16, 32479_u16, 29623_u16, 31815_u16, 33127_u16, 23638_u16, 15506_u16, 37564_u16, 10787_u16, 42175_u16, 35423_u16, 47658_u16, 38673_u16, 60520_u16, 19056_u16, 13430_u16, 12126_u16, 5558_u16, 59408_u16, 25532_u16, 14149_u16, 16065_u16, 15338_u16, 56849_u16, 18409_u16, 54929_u16, 48643_u16, 57106_u16,
	                         29194_u16, 53982_u16, 50018_u16, 65210_u16, 37300_u16, 13899_u16, 43897_u16, 51633_u16, 63562_u16, 1746_u16, 4723_u16, 55619_u16, 39797_u16, 55634_u16, 48709_u16, 60849_u16, 20989_u16, 27636_u16, 18742_u16, 57037_u16, 15183_u16, 863_u16, 1167_u16, 56915_u16, 8812_u16, 53636_u16, 39451_u16, 11639_u16, 41164_u16, 56798_u16, 54617_u16, 65187_u16, 6128_u16, 27500_u16, 32008_u16, 42841_u16, 13332_u16, 65347_u16, 13671_u16, 58767_u16, 34887_u16, 46066_u16, 17067_u16, 33744_u16, 8474_u16, 17377_u16, 44119_u16, 27753_u16, 38566_u16, 17597_u16, 24421_u16, 32926_u16, 11273_u16, 38464_u16, 21799_u16, 908_u16, 19992_u16, 10659_u16, 44272_u16, 63011_u16, 62346_u16, 37596_u16, 8671_u16, 3835_u16, 17679_u16, 18741_u16, 10305_u16, 21160_u16, 48223_u16, 40128_u16, 39176_u16, 52560_u16, 21520_u16, 7553_u16, 35249_u16, 39841_u16, 63075_u16, 3030_u16, 44701_u16, 34275_u16, 41659_u16, 49049_u16, 10297_u16, 15892_u16, 21106_u16, 60216_u16, 2089_u16, 14769_u16, 6358_u16, 60564_u16, 41957_u16, 24131_u16, 5513_u16, 42913_u16, 48127_u16, 44877_u16, 53082_u16, 855_u16, 12796_u16, 25274_u16, 13371_u16, 18670_u16, 50130_u16, 1416_u16, 50729_u16, 31188_u16, 19148_u16, 15919_u16,
	                         26197_u16, 29614_u16, 1780_u16, 8624_u16, 32184_u16, 54818_u16, 19224_u16, 5923_u16, 1839_u16, 37250_u16, 34793_u16, 23604_u16, 44632_u16, 10188_u16, 36391_u16, 52574_u16, 62074_u16, 26003_u16, 31651_u16, 24147_u16, 53291_u16, 3552_u16, 60689_u16, 2653_u16, 1791_u16, 42080_u16, 17114_u16, 55607_u16, 3413_u16, 48152_u16, 44951_u16, 14524_u16, 46581_u16, 16003_u16, 9726_u16, 54199_u16, 44606_u16, 4049_u16, 34628_u16, 19844_u16, 59999_u16, 20525_u16, 11347_u16, 40145_u16, 31770_u16, 7216_u16, 48940_u16, 26181_u16, 18206_u16, 12655_u16, 18186_u16, 28487_u16, 45007_u16, 44989_u16, 57762_u16, 65136_u16, 51608_u16, 58808_u16, 18094_u16, 53184_u16, 20675_u16, 51118_u16, 51866_u16, 22823_u16, 35662_u16, 22162_u16, 37402_u16, 40175_u16, 8008_u16, 29958_u16, 2889_u16, 56960_u16, 56445_u16, 1079_u16, 24463_u16, 2930_u16, 35545_u16, 28366_u16, 6567_u16, 1128_u16, 57910_u16, 56630_u16, 38037_u16, 64040_u16, 48454_u16, 27871_u16, 51603_u16, 14440_u16, 2626_u16, 53658_u16, 59303_u16, 33923_u16, 31453_u16, 3224_u16, 24717_u16, 37327_u16, 57241_u16, 40624_u16, 57263_u16, 6747_u16, 50162_u16, 42663_u16, 19392_u16, 23687_u16, 957_u16, 6993_u16, 44831_u16, 5276_u16,
	                         48_u16, 10269_u16, 2582_u16, 31038_u16, 45947_u16, 53345_u16, 20922_u16, 11445_u16, 34674_u16, 61001_u16, 6943_u16, 54465_u16, 43138_u16, 11945_u16, 1542_u16, 18560_u16, 10446_u16, 21983_u16, 18276_u16, 10129_u16, 36366_u16, 62443_u16, 48551_u16, 42216_u16, 29601_u16, 7612_u16, 31306_u16, 4274_u16, 25533_u16, 20954_u16, 40685_u16, 61400_u16, 24193_u16, 57776_u16, 40911_u16, 20618_u16, 16738_u16, 9521_u16, 29715_u16, 62778_u16, 13824_u16, 44731_u16, 25088_u16, 33899_u16, 3926_u16, 2463_u16, 13949_u16, 41250_u16, 39929_u16, 23241_u16, 106_u16, 30710_u16, 31674_u16, 7113_u16, 46454_u16, 58125_u16, 35548_u16, 38787_u16, 2680_u16, 59272_u16, 9908_u16, 24118_u16, 6873_u16, 15294_u16, 53110_u16, 12517_u16, 52696_u16, 64242_u16, 60240_u16, 16381_u16, 1371_u16, 61338_u16, 14155_u16, 41238_u16, 18490_u16, 1991_u16, 39466_u16, 3416_u16, 10496_u16, 37313_u16, 26458_u16, 40490_u16, 50090_u16, 26751_u16, 10263_u16, 49900_u16, 8331_u16, 34981_u16, 24533_u16, 29710_u16, 8363_u16, 48057_u16, 48923_u16, 4986_u16, 3841_u16, 13171_u16, 1091_u16, 5612_u16, 51408_u16, 16900_u16, 50467_u16, 45662_u16, 44680_u16, 28327_u16, 6015_u16, 47739_u16, 35855_u16, 42839_u16,
	                         4864_u16, 14966_u16, 4381_u16, 56803_u16, 37555_u16, 45433_u16, 8223_u16, 34034_u16, 23325_u16, 38331_u16, 45183_u16, 11587_u16, 52705_u16, 56943_u16, 42831_u16, 34128_u16, 22320_u16, 33357_u16, 154_u16, 55830_u16, 36234_u16, 58516_u16, 57258_u16, 57110_u16, 869_u16, 59158_u16, 61874_u16, 12750_u16, 14953_u16, 50500_u16, 51023_u16, 27592_u16, 26180_u16, 9208_u16, 48205_u16, 2115_u16, 59059_u16, 10072_u16, 15778_u16, 58530_u16, 4503_u16, 202_u16, 56918_u16, 25377_u16, 46712_u16, 61951_u16, 28470_u16, 16921_u16, 44758_u16, 52873_u16, 29688_u16, 7379_u16, 46236_u16, 48943_u16, 61964_u16, 36635_u16, 48785_u16, 12901_u16, 26252_u16, 60741_u16, 10367_u16, 34705_u16, 24522_u16, 60149_u16, 5388_u16, 46574_u16, 21219_u16, 2136_u16, 7569_u16, 45924_u16, 24405_u16, 47880_u16, 12655_u16, 12300_u16, 59846_u16, 30557_u16, 10085_u16, 1335_u16, 53939_u16, 5180_u16, 50345_u16, 39103_u16, 59304_u16, 36552_u16, 23943_u16, 22190_u16, 15889_u16, 11407_u16, 3794_u16, 34445_u16, 7261_u16, 83_u16, 60308_u16, 62974_u16, 36890_u16, 46631_u16, 25862_u16, 31035_u16, 6399_u16, 7106_u16, 15079_u16, 59074_u16, 55879_u16, 12595_u16, 38999_u16, 57563_u16, 10969_u16, 42294_u16,
	                         45537_u16, 2059_u16, 14858_u16, 43536_u16, 55425_u16, 22599_u16, 48043_u16, 21951_u16, 63709_u16, 53903_u16, 3378_u16, 40654_u16, 41132_u16, 12845_u16, 58245_u16, 20779_u16, 54761_u16, 6785_u16, 39985_u16, 5276_u16, 24745_u16, 26822_u16, 25376_u16, 57885_u16, 29075_u16, 49037_u16, 1977_u16, 10406_u16, 45313_u16, 46811_u16, 25931_u16, 31954_u16, 50427_u16, 58461_u16, 45980_u16, 54019_u16, 57245_u16, 18794_u16, 22951_u16, 42985_u16, 25861_u16, 56741_u16, 37082_u16, 10467_u16, 61203_u16, 36164_u16, 46822_u16, 13688_u16, 56410_u16, 39510_u16, 46548_u16, 26721_u16, 8397_u16, 31946_u16, 25239_u16, 10816_u16, 41067_u16, 23877_u16, 23567_u16, 9788_u16, 48175_u16, 12906_u16, 32488_u16, 24785_u16, 61243_u16, 30658_u16, 46932_u16, 23056_u16, 52425_u16, 6442_u16, 53930_u16, 48942_u16, 56546_u16, 43735_u16, 25413_u16, 34055_u16, 13018_u16, 15373_u16, 53077_u16, 53253_u16, 8388_u16, 54452_u16, 20179_u16, 52091_u16, 26251_u16, 49789_u16, 15079_u16, 42958_u16, 16241_u16, 25993_u16, 7101_u16, 62716_u16, 12866_u16, 39408_u16, 50847_u16, 58482_u16, 41643_u16, 9428_u16, 44576_u16, 56156_u16, 43875_u16, 33720_u16, 23237_u16, 24089_u16, 55984_u16, 14612_u16, 60920_u16,
	                         5023_u16, 44528_u16, 46877_u16, 19653_u16, 51625_u16, 59823_u16, 12623_u16, 21072_u16, 39660_u16, 62900_u16, 63253_u16, 20580_u16, 34375_u16, 39721_u16, 33371_u16, 11336_u16, 18701_u16, 29145_u16, 53088_u16, 30353_u16, 30843_u16, 26260_u16, 49071_u16, 48250_u16, 1096_u16, 38093_u16, 55988_u16, 9828_u16, 50384_u16, 26987_u16, 30914_u16, 28006_u16, 59370_u16, 21325_u16, 32481_u16, 56678_u16, 46546_u16, 35363_u16, 47284_u16, 61855_u16, 32642_u16, 7272_u16, 52659_u16, 8673_u16, 6263_u16, 44091_u16, 42171_u16, 64749_u16, 37968_u16, 1264_u16, 19593_u16, 51546_u16, 7109_u16, 58768_u16, 6173_u16, 28390_u16, 64871_u16, 20089_u16, 692_u16, 11162_u16, 12474_u16, 5208_u16, 24655_u16, 21160_u16, 56022_u16, 46612_u16, 8337_u16, 28865_u16, 6280_u16, 43489_u16, 6424_u16, 4295_u16, 64367_u16, 52730_u16, 41622_u16, 65320_u16, 12619_u16, 59451_u16, 30_u16, 49277_u16, 61127_u16, 15654_u16, 41865_u16, 35419_u16, 57915_u16, 42974_u16, 50474_u16, 1538_u16, 62990_u16, 44620_u16, 4786_u16, 9356_u16, 34730_u16, 43172_u16, 33829_u16, 12075_u16, 13804_u16, 41521_u16, 46490_u16, 37097_u16, 36175_u16, 55959_u16, 54479_u16, 26576_u16, 55620_u16, 60543_u16, 23897_u16, 37154_u16,
	                         236_u16, 11006_u16, 33561_u16, 63729_u16, 51074_u16, 3218_u16, 6724_u16, 34980_u16, 23930_u16, 9477_u16, 47078_u16, 21526_u16, 54721_u16, 50334_u16, 37917_u16, 41812_u16, 42010_u16, 17069_u16, 18441_u16, 15979_u16, 16705_u16, 53049_u16, 3126_u16, 20079_u16, 18628_u16, 36018_u16, 40826_u16, 10932_u16, 24517_u16, 54527_u16, 28523_u16, 3199_u16,
	];
	assert!(src.values().eq(sys::ByValIter::new(&norm)));
}

pub struct DeviceVersion_dst_adapter<'t> {
	pub dst: &'t mut crate::test::utils::DeviceVersion::Data_,
}

impl packs::DeviceVersion::DST_ for DeviceVersion_dst_adapter<'_> {
	fn DevID(&mut self, src: u16) { self.dst.DevID().set(src); }
	fn Ver(&mut self, src: u16) { self.dst.Ver().set(src); }
}

pub struct BusConfiguration_dst_adapter<'t> {
	pub dst: &'t mut packs::BusConfiguration::Data_,
}

impl packs::BusConfiguration::DST_ for BusConfiguration_dst_adapter<'_> {
	fn multiplier(&mut self, src: u8) { self.dst.multiplier().set(src); }
	fn time(&mut self, src: u16) { self.dst.time().set(src); }
	fn clk_khz(&mut self, src: u16) { self.dst.clk_khz().set(src); }
}

pub struct BusConfiguration_src_adapter<'t> {
	pub src: &'t mut packs::BusConfiguration::Data_,
}

impl packs::BusConfiguration::SRC_ for BusConfiguration_src_adapter<'_> {
	fn multiplier(&mut self) -> u8 { self.src.multiplier().get() }
	fn time(&mut self) -> u16 { self.src.time().get() }
	fn clk_khz(&mut self) -> u16 { self.src.clk_khz().get() }
}

pub struct InstructionsPack_dst_adapter<'t> {
	pub dst: &'t mut packs::InstructionsPack::Data_,
}

impl packs::InstructionsPack::DST_ for InstructionsPack_dst_adapter<'_> {
	fn Length(&mut self, src: u8) { self.dst.Length().set(src); }
	fn Instructions(&mut self, src: &mut crate::util_::ItemArray_e) { self.dst.Instructions(Some(src)); }
}

pub struct InstructionsPack_src_adapter<'t> {
	pub src: &'t mut packs::InstructionsPack::Data_,
}

impl packs::InstructionsPack::SRC_ for InstructionsPack_src_adapter<'_> {
	fn Length(&mut self) -> u8 { self.src.Length().get() }
	fn Instructions(&mut self, dst: &mut crate::util_::ItemArray_e) {
		for (index, item) in self.src.Instructions(None).enumerate() {
			dst.set(index, item);
		}
	}
}

pub struct DeviceError_dst_adapter<'t> {
	pub dst: &'t mut crate::test::utils::DeviceError::Data_,
}

impl packs::DeviceError::DST_ for DeviceError_dst_adapter<'_> {
	fn param1(&mut self, src: u16) { self.dst.param1().set(src); }
	fn error_id(&mut self, src: packs::Errors) { self.dst.error_id().set(src); }
}

pub struct SensorsData_dst_adapter<'t> {
	pub dst: &'t mut crate::test::utils::SensorsData::Data_,
}

impl packs::SensorsData::DST_ for SensorsData_dst_adapter<'_> {
	fn values(&mut self, src: &mut crate::util_::ItemArray_J) { self.dst.values(Some(src)); }
}

#[test]
fn it_works() -> ::std::io::Result<()> {
	use crate::demo;
	let buff = [0u8; 512];
	let mut curs = sys::Cursors::new();
	let mut tmp_curs = sys::Cursors::new();

	let mut STD__t = STD__transmitter::new();
	let mut STD__r = STD__receiver::new();
	let mut Communication_t = Communication_transmitter::new();
	let mut Communication_r = Communication_receiver::new();

	let mut pack = crate::test::STD_::ITransmittable::DeviceVersion();
	let mut p0 = pack.data();

	write_DeviceVersion(&mut p0);
	read_DeviceVersion(&mut packs::DeviceVersion::Data_(p0.0)); //direct test

	{
		let mut pack_p0 = crate::test::STD_::ITransmittable::DeviceVersion();
		let mut tmp_p0 = pack_p0.data();

		packs::DeviceVersion::push_(&mut packs::DeviceVersion::Data_(p0.0), &mut DeviceVersion_dst_adapter { dst: &mut tmp_p0 });

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p0.0));

		sys::Pack::free(pack_p0.0); // !!! do not forget to cleanup
	}

	if let Err(pack) = STD__t.send(pack)
	//put test pack to the  channel send buffer
	{
		sys::Pack::free(pack);
		panic!("Sending DeviceVersion error")
	} else {
		if ::std::io::copy(&mut STD__t, &mut Communication_r).is_err() {
			panic!("cannot transmit data")
		}
	}
	if !Communication_t.send_Stop() {
		panic!("Send Stop error")
	}
	if ::std::io::copy(&mut Communication_t, &mut STD__r).is_err() {
		panic!("cannot transmit data")
	}
	if !Communication_t.send_Start() {
		panic!("Send Start error")
	}
	if ::std::io::copy(&mut Communication_t, &mut STD__r).is_err() {
		panic!("cannot transmit data")
	}
	if !Communication_t.send_GetDeviceVersion() {
		panic!("Send GetDeviceVersion error")
	}
	if ::std::io::copy(&mut Communication_t, &mut STD__r).is_err() {
		panic!("cannot transmit data")
	}
	if !Communication_t.send_GetConfiguration() {
		panic!("Send GetConfiguration error")
	}
	if ::std::io::copy(&mut Communication_t, &mut STD__r).is_err() {
		panic!("cannot transmit data")
	}
	if !Communication_t.send_SetConfiguration() {
		panic!("Send SetConfiguration error")
	}
	if ::std::io::copy(&mut Communication_t, &mut STD__r).is_err() {
		panic!("cannot transmit data")
	}

	let mut pack = host::Communication::ITransmittable::BusConfiguration();
	let mut p6 = pack.data();

	write_BusConfiguration(&mut p6);
	read_BusConfiguration(&mut p6); //direct test

	{
		let mut pack_p6 = host::Communication::ITransmittable::BusConfiguration();
		let mut tmp_p6 = pack_p6.data();

		packs::BusConfiguration::push_(&mut p6, &mut BusConfiguration_dst_adapter { dst: &mut tmp_p6 });

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p6.0));

		sys::Pack::free(pack_p6.0); // !!! do not forget to cleanup
	}

	{
		let mut pack_p6 = host::Communication::ITransmittable::BusConfiguration();
		let mut tmp_p6 = pack_p6.data();

		packs::BusConfiguration::pull_(&mut BusConfiguration_src_adapter { src: &mut p6 }, &mut tmp_p6);

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p6.0));

		sys::Pack::free(pack_p6.0); // !!! do not forget to cleanup
	}

	if let Err(pack) = Communication_t.send(pack)
	//put test pack to the  channel send buffer
	{
		sys::Pack::free(pack);
		panic!("Sending BusConfiguration error")
	} else {
		if ::std::io::copy(&mut Communication_t, &mut Communication_r).is_err() {
			panic!("cannot transmit data")
		}
	}

	let mut pack = host::Communication::ITransmittable::InstructionsPack();
	let mut p7 = pack.data();

	write_InstructionsPack(&mut p7);
	read_InstructionsPack(&mut p7); //direct test

	{
		let mut pack_p7 = host::Communication::ITransmittable::InstructionsPack();
		let mut tmp_p7 = pack_p7.data();

		packs::InstructionsPack::push_(&mut p7, &mut InstructionsPack_dst_adapter { dst: &mut tmp_p7 });

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p7.0));

		sys::Pack::free(pack_p7.0); // !!! do not forget to cleanup
	}

	{
		let mut pack_p7 = host::Communication::ITransmittable::InstructionsPack();
		let mut tmp_p7 = pack_p7.data();

		packs::InstructionsPack::pull_(&mut InstructionsPack_src_adapter { src: &mut p7 }, &mut tmp_p7);

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p7.0));

		sys::Pack::free(pack_p7.0); // !!! do not forget to cleanup
	}

	if let Err(pack) = Communication_t.send(pack)
	//put test pack to the  channel send buffer
	{
		sys::Pack::free(pack);
		panic!("Sending InstructionsPack error")
	} else {
		if ::std::io::copy(&mut Communication_t, &mut Communication_r).is_err() {
			panic!("cannot transmit data")
		}
	}

	let mut pack = crate::test::STD_::ITransmittable::DeviceError();
	let mut p8 = pack.data();

	write_DeviceError(&mut p8);
	read_DeviceError(&mut packs::DeviceError::Data_(p8.0)); //direct test

	{
		let mut pack_p8 = crate::test::STD_::ITransmittable::DeviceError();
		let mut tmp_p8 = pack_p8.data();

		packs::DeviceError::push_(&mut packs::DeviceError::Data_(p8.0), &mut DeviceError_dst_adapter { dst: &mut tmp_p8 });

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p8.0));

		sys::Pack::free(pack_p8.0); // !!! do not forget to cleanup
	}

	if let Err(pack) = STD__t.send(pack)
	//put test pack to the  channel send buffer
	{
		sys::Pack::free(pack);
		panic!("Sending DeviceError error")
	} else {
		if ::std::io::copy(&mut STD__t, &mut Communication_r).is_err() {
			panic!("cannot transmit data")
		}
	}

	let mut pack = crate::test::STD_::ITransmittable::SensorsData();
	let mut p9 = pack.data();

	write_SensorsData(&mut p9);
	read_SensorsData(&mut packs::SensorsData::Data_(p9.0)); //direct test

	{
		let mut pack_p9 = crate::test::STD_::ITransmittable::SensorsData();
		let mut tmp_p9 = pack_p9.data();

		packs::SensorsData::push_(&mut packs::SensorsData::Data_(p9.0), &mut SensorsData_dst_adapter { dst: &mut tmp_p9 });

		assert!(sys::Pack::base_bytes(pack.0) == sys::Pack::base_bytes(pack_p9.0));

		sys::Pack::free(pack_p9.0); // !!! do not forget to cleanup
	}

	if let Err(pack) = STD__t.send(pack)
	//put test pack to the  channel send buffer
	{
		sys::Pack::free(pack);
		panic!("Sending SensorsData error")
	} else {
		if ::std::io::copy(&mut STD__t, &mut Communication_r).is_err() {
			panic!("cannot transmit data")
		}
	}

	Ok(())
}
