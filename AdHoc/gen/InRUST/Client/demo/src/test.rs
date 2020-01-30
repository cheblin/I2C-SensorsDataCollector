
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
			pub fn DevID(&mut self) -> Item_c { Item_c(self.0) }
			pub fn Ver(&mut self) -> Item_X { Item_X(self.0) }
		}
	}
	///[DevID](Client::DeviceVersion::DevID).

	pub struct Item_c(pub *mut u8);
	impl Item_c {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[Ver](Client::DeviceVersion::Ver).

	pub struct Item_X(pub *mut u8);
	impl Item_X {
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
			pub fn param1(&mut self) -> Item_v { Item_v(self.0) }
			pub fn error_id(&mut self) -> Item_z { Item_z(self.0) }
		}
	}
	///[param1](Client::DeviceError::param1).

	pub struct Item_v(pub *mut u8);
	impl Item_v {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[error_id](Client::DeviceError::error_id).

	pub struct Item_z(pub *mut u8);
	impl Item_z {
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
			pub fn values(&mut self, src: Option<&mut dyn Iterator<Item = u16>>) -> ItemArraySET_Z {
				let dst = self.0;

				let len = 1000 as usize;
				let offset = 0;
				let bytes = dst;
				let mut array = ItemArraySET_Z { bytes, len, offset, index: !0 };
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

	pub struct ItemArraySET_Z {
		pub bytes:  *mut u8,
		pub len:    usize,
		pub offset: usize,
		pub index:  usize,
	}
	impl ItemArraySET_Z {
		pub fn get(&mut self, index: usize) -> u16 {
			let dst = sys::get_bytes(self.bytes, self.offset + index * 2, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, index: usize, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.bytes, self.offset + index * 2); }
	}

	impl Iterator for ItemArraySET_Z {
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
	dst.DevID().set(48246_u16);
	dst.Ver().set(45456_u16);
}
fn read_DeviceVersion(src: &mut packs::DeviceVersion::Data_) {
	assert_eq!(src.DevID(), 48246_u16);
	assert_eq!(src.Ver(), 45456_u16);
}
fn on_Stop() {}
fn on_Start() {}
fn on_GetDeviceVersion() {}
fn on_GetConfiguration() {}
fn on_SetConfiguration() {}
fn write_BusConfiguration(dst: &mut packs::BusConfiguration::Data_) {
	dst.multiplier().set(140_u8);
	dst.time().set(8141_u16);
	dst.clk_khz().set(8623_u16);
}
fn read_BusConfiguration(src: &mut packs::BusConfiguration::Data_) {
	assert_eq!(src.multiplier().get(), 140_u8);
	assert_eq!(src.time().get(), 8141_u16);
	assert_eq!(src.clk_khz().get(), 8623_u16);
}

fn write_InstructionsPack(dst: &mut packs::InstructionsPack::Data_) {
	dst.Length().set(98_u8);
	let norm: [u8; 256] = [
	                       221_u8, 102_u8, 196_u8, 212_u8, 0_u8, 23_u8, 29_u8, 215_u8, 50_u8, 251_u8, 54_u8, 209_u8, 56_u8, 160_u8, 43_u8, 217_u8, 226_u8, 61_u8, 90_u8, 26_u8, 115_u8, 219_u8, 51_u8, 38_u8, 50_u8, 232_u8, 68_u8, 181_u8, 183_u8, 55_u8, 227_u8, 128_u8, 113_u8, 211_u8, 69_u8, 59_u8, 155_u8, 187_u8, 98_u8, 11_u8, 36_u8, 183_u8, 7_u8, 145_u8, 80_u8, 58_u8, 169_u8, 23_u8, 79_u8, 109_u8, 27_u8, 39_u8, 205_u8, 74_u8, 102_u8, 181_u8, 252_u8, 130_u8, 193_u8, 44_u8, 115_u8, 160_u8, 110_u8, 43_u8, 157_u8, 95_u8, 47_u8, 168_u8, 52_u8, 173_u8, 192_u8, 163_u8, 121_u8, 212_u8, 225_u8, 14_u8, 224_u8, 152_u8, 230_u8, 60_u8, 208_u8, 85_u8, 1_u8, 34_u8, 128_u8, 19_u8, 46_u8, 136_u8, 246_u8, 83_u8, 249_u8, 67_u8, 77_u8, 36_u8, 65_u8, 172_u8, 100_u8, 222_u8, 115_u8, 48_u8, 189_u8, 246_u8, 174_u8, 117_u8, 12_u8, 233_u8, 88_u8, 145_u8, 26_u8, 216_u8, 173_u8, 191_u8, 48_u8, 78_u8, 108_u8, 144_u8, 111_u8, 41_u8, 204_u8, 190_u8, 8_u8, 218_u8, 56_u8, 6_u8, 156_u8, 33_u8, 110_u8, 185_u8, 32_u8, 2_u8, 111_u8, 228_u8, 219_u8, 81_u8, 147_u8, 179_u8, 46_u8, 175_u8, 197_u8, 228_u8, 90_u8, 214_u8, 11_u8, 245_u8, 4_u8, 2_u8, 191_u8, 234_u8, 151_u8, 46_u8, 248_u8, 203_u8, 79_u8, 2_u8, 124_u8, 236_u8,
	                       124_u8, 95_u8, 197_u8, 41_u8, 168_u8, 8_u8, 140_u8, 217_u8, 116_u8, 176_u8, 196_u8, 246_u8, 189_u8, 250_u8, 18_u8, 24_u8, 66_u8, 40_u8, 212_u8, 40_u8, 120_u8, 154_u8, 88_u8, 85_u8, 165_u8, 187_u8, 96_u8, 48_u8, 100_u8, 52_u8, 36_u8, 227_u8, 93_u8, 43_u8, 177_u8, 156_u8, 191_u8, 224_u8, 119_u8, 32_u8, 22_u8, 206_u8, 122_u8, 119_u8, 143_u8, 216_u8, 147_u8, 139_u8, 208_u8, 47_u8, 148_u8, 104_u8, 38_u8, 172_u8, 253_u8, 62_u8, 195_u8, 255_u8, 41_u8, 224_u8, 94_u8, 178_u8, 40_u8, 240_u8, 138_u8, 175_u8, 100_u8, 88_u8, 249_u8, 192_u8, 30_u8, 67_u8, 56_u8, 158_u8, 239_u8, 124_u8, 65_u8, 187_u8, 244_u8, 242_u8, 151_u8, 172_u8, 17_u8, 245_u8, 246_u8, 32_u8, 202_u8, 134_u8, 214_u8, 156_u8, 157_u8, 12_u8, 59_u8, 115_u8, 234_u8, 202_u8, 75_u8, 128_u8, 29_u8, 180_u8,
	];
	dst.Instructions(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_InstructionsPack(src: &mut packs::InstructionsPack::Data_) {
	assert_eq!(src.Length().get(), 98_u8);
	let norm: [u8; 256] = [
	                       221_u8, 102_u8, 196_u8, 212_u8, 0_u8, 23_u8, 29_u8, 215_u8, 50_u8, 251_u8, 54_u8, 209_u8, 56_u8, 160_u8, 43_u8, 217_u8, 226_u8, 61_u8, 90_u8, 26_u8, 115_u8, 219_u8, 51_u8, 38_u8, 50_u8, 232_u8, 68_u8, 181_u8, 183_u8, 55_u8, 227_u8, 128_u8, 113_u8, 211_u8, 69_u8, 59_u8, 155_u8, 187_u8, 98_u8, 11_u8, 36_u8, 183_u8, 7_u8, 145_u8, 80_u8, 58_u8, 169_u8, 23_u8, 79_u8, 109_u8, 27_u8, 39_u8, 205_u8, 74_u8, 102_u8, 181_u8, 252_u8, 130_u8, 193_u8, 44_u8, 115_u8, 160_u8, 110_u8, 43_u8, 157_u8, 95_u8, 47_u8, 168_u8, 52_u8, 173_u8, 192_u8, 163_u8, 121_u8, 212_u8, 225_u8, 14_u8, 224_u8, 152_u8, 230_u8, 60_u8, 208_u8, 85_u8, 1_u8, 34_u8, 128_u8, 19_u8, 46_u8, 136_u8, 246_u8, 83_u8, 249_u8, 67_u8, 77_u8, 36_u8, 65_u8, 172_u8, 100_u8, 222_u8, 115_u8, 48_u8, 189_u8, 246_u8, 174_u8, 117_u8, 12_u8, 233_u8, 88_u8, 145_u8, 26_u8, 216_u8, 173_u8, 191_u8, 48_u8, 78_u8, 108_u8, 144_u8, 111_u8, 41_u8, 204_u8, 190_u8, 8_u8, 218_u8, 56_u8, 6_u8, 156_u8, 33_u8, 110_u8, 185_u8, 32_u8, 2_u8, 111_u8, 228_u8, 219_u8, 81_u8, 147_u8, 179_u8, 46_u8, 175_u8, 197_u8, 228_u8, 90_u8, 214_u8, 11_u8, 245_u8, 4_u8, 2_u8, 191_u8, 234_u8, 151_u8, 46_u8, 248_u8, 203_u8, 79_u8, 2_u8, 124_u8, 236_u8,
	                       124_u8, 95_u8, 197_u8, 41_u8, 168_u8, 8_u8, 140_u8, 217_u8, 116_u8, 176_u8, 196_u8, 246_u8, 189_u8, 250_u8, 18_u8, 24_u8, 66_u8, 40_u8, 212_u8, 40_u8, 120_u8, 154_u8, 88_u8, 85_u8, 165_u8, 187_u8, 96_u8, 48_u8, 100_u8, 52_u8, 36_u8, 227_u8, 93_u8, 43_u8, 177_u8, 156_u8, 191_u8, 224_u8, 119_u8, 32_u8, 22_u8, 206_u8, 122_u8, 119_u8, 143_u8, 216_u8, 147_u8, 139_u8, 208_u8, 47_u8, 148_u8, 104_u8, 38_u8, 172_u8, 253_u8, 62_u8, 195_u8, 255_u8, 41_u8, 224_u8, 94_u8, 178_u8, 40_u8, 240_u8, 138_u8, 175_u8, 100_u8, 88_u8, 249_u8, 192_u8, 30_u8, 67_u8, 56_u8, 158_u8, 239_u8, 124_u8, 65_u8, 187_u8, 244_u8, 242_u8, 151_u8, 172_u8, 17_u8, 245_u8, 246_u8, 32_u8, 202_u8, 134_u8, 214_u8, 156_u8, 157_u8, 12_u8, 59_u8, 115_u8, 234_u8, 202_u8, 75_u8, 128_u8, 29_u8, 180_u8,
	];
	assert!(src.Instructions(None).eq(sys::ByValIter::new(&norm)));
}

fn write_DeviceError(dst: &mut crate::test::utils::DeviceError::Data_) {
	dst.param1().set(3728_u16);
	dst.error_id().set(packs::Errors::CannotReadSensor);
}
fn read_DeviceError(src: &mut packs::DeviceError::Data_) {
	assert_eq!(src.param1(), 3728_u16);
	assert_eq!(src.error_id(), packs::Errors::CannotReadSensor);
}

fn write_SensorsData(dst: &mut crate::test::utils::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         10525_u16, 23956_u16, 1979_u16, 42406_u16, 62098_u16, 10361_u16, 65036_u16, 3012_u16, 56534_u16, 46941_u16, 7338_u16, 35880_u16, 42546_u16, 22527_u16, 62691_u16, 25763_u16, 51851_u16, 8306_u16, 22200_u16, 54738_u16, 13814_u16, 17183_u16, 49298_u16, 63854_u16, 51894_u16, 416_u16, 35738_u16, 37162_u16, 37047_u16, 44730_u16, 11580_u16, 12634_u16, 23290_u16, 21102_u16, 56542_u16, 63658_u16, 34357_u16, 15317_u16, 52423_u16, 37227_u16, 5951_u16, 31287_u16, 13752_u16, 18579_u16, 50364_u16, 15981_u16, 30365_u16, 41393_u16, 22898_u16, 31575_u16, 44671_u16, 38856_u16, 63612_u16, 40786_u16, 16100_u16, 18345_u16, 36155_u16, 62754_u16, 1964_u16, 59673_u16, 14057_u16, 11729_u16, 36945_u16, 46493_u16, 14368_u16, 12204_u16, 59392_u16, 4325_u16, 32951_u16, 227_u16, 51646_u16, 51284_u16, 51464_u16, 23187_u16, 35997_u16, 45499_u16, 29767_u16, 16004_u16, 7592_u16, 14637_u16, 43737_u16, 11631_u16, 33325_u16, 51856_u16, 30485_u16, 28449_u16, 61936_u16, 21130_u16, 34809_u16, 43375_u16, 56698_u16, 27992_u16, 8257_u16, 45417_u16, 28662_u16, 11567_u16, 34784_u16, 29904_u16, 3265_u16, 44469_u16, 30077_u16, 32032_u16, 33354_u16, 43138_u16, 60836_u16, 9861_u16, 31832_u16,
	                         34183_u16, 12705_u16, 21833_u16, 7456_u16, 45261_u16, 58754_u16, 41116_u16, 28537_u16, 47304_u16, 59082_u16, 52407_u16, 58173_u16, 16932_u16, 44594_u16, 39106_u16, 13163_u16, 36561_u16, 4058_u16, 56982_u16, 34177_u16, 65202_u16, 52581_u16, 16460_u16, 10535_u16, 53017_u16, 7977_u16, 55725_u16, 18922_u16, 21394_u16, 47374_u16, 31970_u16, 8926_u16, 37450_u16, 29425_u16, 41523_u16, 20654_u16, 55963_u16, 206_u16, 19240_u16, 35472_u16, 11731_u16, 6854_u16, 27379_u16, 30329_u16, 1238_u16, 57452_u16, 6354_u16, 35473_u16, 48718_u16, 8538_u16, 36967_u16, 11155_u16, 33437_u16, 12947_u16, 2304_u16, 7083_u16, 42729_u16, 16997_u16, 24550_u16, 46527_u16, 59309_u16, 19231_u16, 52462_u16, 7564_u16, 29309_u16, 56252_u16, 13342_u16, 17501_u16, 48808_u16, 50547_u16, 2236_u16, 31350_u16, 60503_u16, 61469_u16, 47881_u16, 421_u16, 12457_u16, 11372_u16, 31970_u16, 25894_u16, 42079_u16, 60340_u16, 39709_u16, 55736_u16, 29374_u16, 14566_u16, 6585_u16, 14138_u16, 28953_u16, 10265_u16, 7501_u16, 36915_u16, 34301_u16, 30462_u16, 51704_u16, 9696_u16, 30126_u16, 40708_u16, 30880_u16, 56044_u16, 25121_u16, 13776_u16, 45350_u16, 7102_u16, 54539_u16, 13383_u16, 35333_u16, 48023_u16,
	                         51355_u16, 24678_u16, 63790_u16, 35415_u16, 43150_u16, 15414_u16, 14606_u16, 30016_u16, 8044_u16, 1888_u16, 2893_u16, 17659_u16, 21170_u16, 56152_u16, 20681_u16, 53933_u16, 30054_u16, 27740_u16, 56567_u16, 47000_u16, 27517_u16, 15378_u16, 12999_u16, 43120_u16, 6528_u16, 44961_u16, 10451_u16, 64842_u16, 31728_u16, 58765_u16, 33687_u16, 47535_u16, 31031_u16, 55887_u16, 25420_u16, 50643_u16, 9602_u16, 38705_u16, 24773_u16, 10880_u16, 65164_u16, 65269_u16, 62456_u16, 47349_u16, 12872_u16, 58628_u16, 48981_u16, 14399_u16, 30942_u16, 37116_u16, 2808_u16, 30840_u16, 43482_u16, 24567_u16, 44718_u16, 3215_u16, 2187_u16, 19545_u16, 25919_u16, 62035_u16, 22781_u16, 19215_u16, 60836_u16, 63547_u16, 7191_u16, 60103_u16, 33580_u16, 17435_u16, 15133_u16, 4393_u16, 23117_u16, 9943_u16, 22507_u16, 5391_u16, 36791_u16, 26381_u16, 64787_u16, 1928_u16, 55873_u16, 30667_u16, 33232_u16, 7697_u16, 29024_u16, 17438_u16, 57946_u16, 2920_u16, 1949_u16, 56619_u16, 62990_u16, 58377_u16, 12055_u16, 15687_u16, 31632_u16, 49608_u16, 35916_u16, 53297_u16, 36327_u16, 48357_u16, 58521_u16, 51118_u16, 63568_u16, 6150_u16, 39567_u16, 26029_u16, 18410_u16, 16070_u16, 54944_u16, 5720_u16,
	                         7130_u16, 16773_u16, 25727_u16, 28280_u16, 22345_u16, 49093_u16, 15740_u16, 40965_u16, 10850_u16, 53772_u16, 32510_u16, 60986_u16, 40783_u16, 8794_u16, 10876_u16, 36159_u16, 13187_u16, 39809_u16, 36641_u16, 26572_u16, 13590_u16, 47963_u16, 19826_u16, 26940_u16, 36370_u16, 41160_u16, 42345_u16, 61124_u16, 33901_u16, 30896_u16, 29324_u16, 37045_u16, 22354_u16, 48608_u16, 16202_u16, 12051_u16, 50088_u16, 27217_u16, 25678_u16, 48256_u16, 48681_u16, 33148_u16, 31396_u16, 30626_u16, 20113_u16, 57229_u16, 10397_u16, 12040_u16, 20130_u16, 48670_u16, 32746_u16, 10310_u16, 11146_u16, 38464_u16, 22124_u16, 9595_u16, 57416_u16, 64335_u16, 48518_u16, 7993_u16, 18576_u16, 12014_u16, 10697_u16, 51341_u16, 43023_u16, 44791_u16, 5333_u16, 34826_u16, 11506_u16, 49108_u16, 43849_u16, 4674_u16, 2584_u16, 55598_u16, 50143_u16, 65364_u16, 41404_u16, 5869_u16, 9304_u16, 27518_u16, 25661_u16, 59095_u16, 39561_u16, 7108_u16, 44822_u16, 41040_u16, 47451_u16, 46858_u16, 33660_u16, 57571_u16, 61073_u16, 11775_u16, 39181_u16, 20082_u16, 7821_u16, 53221_u16, 36944_u16, 31786_u16, 61083_u16, 45082_u16, 45348_u16, 13110_u16, 41789_u16, 19864_u16, 62137_u16, 45073_u16, 62335_u16,
	                         47469_u16, 6527_u16, 52211_u16, 64361_u16, 36699_u16, 30956_u16, 42214_u16, 39933_u16, 25437_u16, 53669_u16, 65179_u16, 55095_u16, 64589_u16, 6734_u16, 7990_u16, 14566_u16, 45756_u16, 16952_u16, 10865_u16, 45404_u16, 40852_u16, 36024_u16, 21128_u16, 32943_u16, 2649_u16, 61368_u16, 29385_u16, 18749_u16, 1270_u16, 10399_u16, 1175_u16, 16108_u16, 1205_u16, 63475_u16, 54250_u16, 60002_u16, 9043_u16, 29092_u16, 42479_u16, 43602_u16, 23654_u16, 29466_u16, 36061_u16, 42531_u16, 21730_u16, 36179_u16, 15077_u16, 16158_u16, 31253_u16, 57101_u16, 48129_u16, 63753_u16, 34338_u16, 9388_u16, 1556_u16, 60909_u16, 10287_u16, 57364_u16, 38565_u16, 35902_u16, 60878_u16, 51204_u16, 59646_u16, 60851_u16, 30978_u16, 25898_u16, 2812_u16, 40319_u16, 59035_u16, 46071_u16, 6156_u16, 39549_u16, 23018_u16, 46331_u16, 50372_u16, 33890_u16, 27251_u16, 34318_u16, 63928_u16, 15249_u16, 42505_u16, 62763_u16, 2350_u16, 8627_u16, 57547_u16, 30513_u16, 60737_u16, 60494_u16, 52191_u16, 52441_u16, 7388_u16, 46218_u16, 6048_u16, 36092_u16, 28593_u16, 63300_u16, 61702_u16, 57796_u16, 741_u16, 4403_u16, 54680_u16, 14073_u16, 621_u16, 50998_u16, 7427_u16, 25851_u16, 12792_u16, 54107_u16,
	                         8772_u16, 46179_u16, 44653_u16, 19101_u16, 3955_u16, 58733_u16, 38728_u16, 53796_u16, 50370_u16, 36031_u16, 33410_u16, 62719_u16, 29797_u16, 29135_u16, 46834_u16, 51815_u16, 38031_u16, 39292_u16, 10237_u16, 28654_u16, 55895_u16, 31692_u16, 50892_u16, 17600_u16, 16661_u16, 17313_u16, 58780_u16, 25431_u16, 31236_u16, 15599_u16, 11740_u16, 34716_u16, 61521_u16, 9651_u16, 64604_u16, 19473_u16, 42772_u16, 25862_u16, 4619_u16, 31451_u16, 6754_u16, 26760_u16, 8000_u16, 26150_u16, 40671_u16, 49737_u16, 12460_u16, 31164_u16, 51333_u16, 26231_u16, 49898_u16, 2863_u16, 29223_u16, 19595_u16, 35488_u16, 19645_u16, 48309_u16, 58948_u16, 11101_u16, 46948_u16, 52788_u16, 27109_u16, 19902_u16, 18414_u16, 34840_u16, 27184_u16, 32400_u16, 41343_u16, 46341_u16, 7223_u16, 45169_u16, 31271_u16, 3022_u16, 56816_u16, 29286_u16, 31165_u16, 6583_u16, 24963_u16, 26609_u16, 64396_u16, 50620_u16, 40460_u16, 446_u16, 34029_u16, 8846_u16, 6599_u16, 17127_u16, 16169_u16, 37460_u16, 7070_u16, 34327_u16, 30913_u16, 44394_u16, 9391_u16, 13423_u16, 40895_u16, 3005_u16, 34031_u16, 48256_u16, 63581_u16, 29485_u16, 16111_u16, 18078_u16, 4655_u16, 64234_u16, 12359_u16, 9448_u16, 26029_u16,
	                         27775_u16, 30526_u16, 31442_u16, 26572_u16, 11035_u16, 13950_u16, 23409_u16, 42792_u16, 33542_u16, 9795_u16, 37074_u16, 55941_u16, 36948_u16, 52179_u16, 22535_u16, 54652_u16, 36005_u16, 19030_u16, 49565_u16, 47386_u16, 46440_u16, 21228_u16, 47993_u16, 55324_u16, 45691_u16, 23627_u16, 37070_u16, 26737_u16, 6540_u16, 38507_u16, 4038_u16, 31370_u16, 6690_u16, 62506_u16, 57249_u16, 3360_u16, 61016_u16, 550_u16, 3933_u16, 13669_u16, 47266_u16, 6321_u16, 4940_u16, 41542_u16, 22981_u16, 46081_u16, 63070_u16, 54301_u16, 3436_u16, 26735_u16, 58073_u16, 47303_u16, 47357_u16, 24767_u16, 55442_u16, 10720_u16, 3080_u16, 59055_u16, 25986_u16, 42037_u16, 60397_u16, 9752_u16, 30288_u16, 64029_u16, 24573_u16, 60350_u16, 14877_u16, 52522_u16, 19768_u16, 4868_u16, 13021_u16, 7236_u16, 18810_u16, 58288_u16, 58586_u16, 47801_u16, 37428_u16, 63579_u16, 27550_u16, 22869_u16, 19362_u16, 27456_u16, 36266_u16, 11111_u16, 37185_u16, 4772_u16, 12961_u16, 10647_u16, 50357_u16, 11921_u16, 48668_u16, 47477_u16, 57395_u16, 56093_u16, 6898_u16, 24073_u16, 60299_u16, 41165_u16, 31011_u16, 357_u16, 17567_u16, 25267_u16, 11375_u16, 55020_u16, 32200_u16, 9663_u16, 2677_u16, 43796_u16,
	                         37217_u16, 40221_u16, 41212_u16, 6659_u16, 63598_u16, 62991_u16, 42383_u16, 21907_u16, 21163_u16, 55109_u16, 15990_u16, 59523_u16, 50721_u16, 26793_u16, 26010_u16, 31371_u16, 37037_u16, 22280_u16, 61629_u16, 30762_u16, 56916_u16, 26297_u16, 32977_u16, 59437_u16, 50338_u16, 51978_u16, 8521_u16, 2919_u16, 59641_u16, 23790_u16, 38357_u16, 17848_u16, 56411_u16, 16860_u16, 38048_u16, 38785_u16, 54381_u16, 61933_u16, 37565_u16, 11905_u16, 44973_u16, 34931_u16, 43854_u16, 13884_u16, 14164_u16, 8954_u16, 39251_u16, 34118_u16, 13197_u16, 22760_u16, 17765_u16, 27830_u16, 1278_u16, 62362_u16, 30620_u16, 51698_u16, 5825_u16, 45464_u16, 52332_u16, 23481_u16, 43500_u16, 25599_u16, 10656_u16, 2459_u16, 60982_u16, 34952_u16, 8472_u16, 36617_u16, 63480_u16, 1338_u16, 62202_u16, 56306_u16, 45482_u16, 8320_u16, 5658_u16, 18749_u16, 53028_u16, 14983_u16, 38774_u16, 52046_u16, 7738_u16, 48292_u16, 43304_u16, 9407_u16, 55026_u16, 13487_u16, 60102_u16, 3279_u16, 55969_u16, 62324_u16, 9475_u16, 48331_u16, 65252_u16, 30466_u16, 40996_u16, 58128_u16, 10221_u16, 26158_u16, 44295_u16, 20557_u16, 60283_u16, 20975_u16, 23889_u16, 63532_u16, 17488_u16, 27864_u16, 50736_u16,
	                         32227_u16, 54077_u16, 17422_u16, 24415_u16, 9288_u16, 56395_u16, 24286_u16, 47867_u16, 29860_u16, 40292_u16, 21165_u16, 48868_u16, 21517_u16, 3809_u16, 60632_u16, 30008_u16, 59903_u16, 36559_u16, 9805_u16, 30093_u16, 19030_u16, 64348_u16, 42327_u16, 43512_u16, 46973_u16, 40068_u16, 11899_u16, 1762_u16, 17117_u16, 60009_u16, 43333_u16, 9737_u16, 24639_u16, 51967_u16, 56352_u16, 2630_u16, 26610_u16, 2751_u16, 33665_u16, 32483_u16, 47321_u16, 22305_u16, 60216_u16, 39160_u16, 54865_u16, 53822_u16, 16854_u16, 12626_u16, 27012_u16, 52771_u16, 18081_u16, 63106_u16, 53984_u16, 42706_u16, 3170_u16, 16963_u16, 34147_u16, 49740_u16, 47727_u16, 29081_u16, 26105_u16, 5370_u16, 30916_u16, 40990_u16, 13652_u16, 30907_u16, 22711_u16, 10245_u16, 31360_u16, 44256_u16, 7428_u16, 26445_u16, 47593_u16, 44600_u16, 45887_u16, 4369_u16, 36654_u16, 10477_u16, 5296_u16, 45403_u16, 13660_u16, 44220_u16, 56306_u16, 2357_u16, 11513_u16, 2868_u16, 34473_u16, 56626_u16, 49181_u16, 32620_u16, 64299_u16, 25972_u16, 7089_u16, 40218_u16, 48691_u16, 23193_u16, 65420_u16, 34017_u16, 15219_u16, 47820_u16, 22041_u16, 62206_u16, 31091_u16, 35092_u16, 25495_u16, 56633_u16, 29230_u16,
	                         34788_u16, 2741_u16, 60471_u16, 62372_u16, 25525_u16, 49378_u16, 55357_u16, 17222_u16, 25138_u16, 33764_u16, 64536_u16, 33445_u16, 63212_u16, 42144_u16, 28650_u16, 2336_u16, 10555_u16, 47966_u16, 9385_u16, 22507_u16, 5344_u16, 59893_u16, 22880_u16, 40804_u16, 40050_u16, 64917_u16, 62906_u16, 24210_u16, 55131_u16, 37342_u16, 2766_u16, 58837_u16,
	];
	dst.values(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_SensorsData(src: &mut packs::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         10525_u16, 23956_u16, 1979_u16, 42406_u16, 62098_u16, 10361_u16, 65036_u16, 3012_u16, 56534_u16, 46941_u16, 7338_u16, 35880_u16, 42546_u16, 22527_u16, 62691_u16, 25763_u16, 51851_u16, 8306_u16, 22200_u16, 54738_u16, 13814_u16, 17183_u16, 49298_u16, 63854_u16, 51894_u16, 416_u16, 35738_u16, 37162_u16, 37047_u16, 44730_u16, 11580_u16, 12634_u16, 23290_u16, 21102_u16, 56542_u16, 63658_u16, 34357_u16, 15317_u16, 52423_u16, 37227_u16, 5951_u16, 31287_u16, 13752_u16, 18579_u16, 50364_u16, 15981_u16, 30365_u16, 41393_u16, 22898_u16, 31575_u16, 44671_u16, 38856_u16, 63612_u16, 40786_u16, 16100_u16, 18345_u16, 36155_u16, 62754_u16, 1964_u16, 59673_u16, 14057_u16, 11729_u16, 36945_u16, 46493_u16, 14368_u16, 12204_u16, 59392_u16, 4325_u16, 32951_u16, 227_u16, 51646_u16, 51284_u16, 51464_u16, 23187_u16, 35997_u16, 45499_u16, 29767_u16, 16004_u16, 7592_u16, 14637_u16, 43737_u16, 11631_u16, 33325_u16, 51856_u16, 30485_u16, 28449_u16, 61936_u16, 21130_u16, 34809_u16, 43375_u16, 56698_u16, 27992_u16, 8257_u16, 45417_u16, 28662_u16, 11567_u16, 34784_u16, 29904_u16, 3265_u16, 44469_u16, 30077_u16, 32032_u16, 33354_u16, 43138_u16, 60836_u16, 9861_u16, 31832_u16,
	                         34183_u16, 12705_u16, 21833_u16, 7456_u16, 45261_u16, 58754_u16, 41116_u16, 28537_u16, 47304_u16, 59082_u16, 52407_u16, 58173_u16, 16932_u16, 44594_u16, 39106_u16, 13163_u16, 36561_u16, 4058_u16, 56982_u16, 34177_u16, 65202_u16, 52581_u16, 16460_u16, 10535_u16, 53017_u16, 7977_u16, 55725_u16, 18922_u16, 21394_u16, 47374_u16, 31970_u16, 8926_u16, 37450_u16, 29425_u16, 41523_u16, 20654_u16, 55963_u16, 206_u16, 19240_u16, 35472_u16, 11731_u16, 6854_u16, 27379_u16, 30329_u16, 1238_u16, 57452_u16, 6354_u16, 35473_u16, 48718_u16, 8538_u16, 36967_u16, 11155_u16, 33437_u16, 12947_u16, 2304_u16, 7083_u16, 42729_u16, 16997_u16, 24550_u16, 46527_u16, 59309_u16, 19231_u16, 52462_u16, 7564_u16, 29309_u16, 56252_u16, 13342_u16, 17501_u16, 48808_u16, 50547_u16, 2236_u16, 31350_u16, 60503_u16, 61469_u16, 47881_u16, 421_u16, 12457_u16, 11372_u16, 31970_u16, 25894_u16, 42079_u16, 60340_u16, 39709_u16, 55736_u16, 29374_u16, 14566_u16, 6585_u16, 14138_u16, 28953_u16, 10265_u16, 7501_u16, 36915_u16, 34301_u16, 30462_u16, 51704_u16, 9696_u16, 30126_u16, 40708_u16, 30880_u16, 56044_u16, 25121_u16, 13776_u16, 45350_u16, 7102_u16, 54539_u16, 13383_u16, 35333_u16, 48023_u16,
	                         51355_u16, 24678_u16, 63790_u16, 35415_u16, 43150_u16, 15414_u16, 14606_u16, 30016_u16, 8044_u16, 1888_u16, 2893_u16, 17659_u16, 21170_u16, 56152_u16, 20681_u16, 53933_u16, 30054_u16, 27740_u16, 56567_u16, 47000_u16, 27517_u16, 15378_u16, 12999_u16, 43120_u16, 6528_u16, 44961_u16, 10451_u16, 64842_u16, 31728_u16, 58765_u16, 33687_u16, 47535_u16, 31031_u16, 55887_u16, 25420_u16, 50643_u16, 9602_u16, 38705_u16, 24773_u16, 10880_u16, 65164_u16, 65269_u16, 62456_u16, 47349_u16, 12872_u16, 58628_u16, 48981_u16, 14399_u16, 30942_u16, 37116_u16, 2808_u16, 30840_u16, 43482_u16, 24567_u16, 44718_u16, 3215_u16, 2187_u16, 19545_u16, 25919_u16, 62035_u16, 22781_u16, 19215_u16, 60836_u16, 63547_u16, 7191_u16, 60103_u16, 33580_u16, 17435_u16, 15133_u16, 4393_u16, 23117_u16, 9943_u16, 22507_u16, 5391_u16, 36791_u16, 26381_u16, 64787_u16, 1928_u16, 55873_u16, 30667_u16, 33232_u16, 7697_u16, 29024_u16, 17438_u16, 57946_u16, 2920_u16, 1949_u16, 56619_u16, 62990_u16, 58377_u16, 12055_u16, 15687_u16, 31632_u16, 49608_u16, 35916_u16, 53297_u16, 36327_u16, 48357_u16, 58521_u16, 51118_u16, 63568_u16, 6150_u16, 39567_u16, 26029_u16, 18410_u16, 16070_u16, 54944_u16, 5720_u16,
	                         7130_u16, 16773_u16, 25727_u16, 28280_u16, 22345_u16, 49093_u16, 15740_u16, 40965_u16, 10850_u16, 53772_u16, 32510_u16, 60986_u16, 40783_u16, 8794_u16, 10876_u16, 36159_u16, 13187_u16, 39809_u16, 36641_u16, 26572_u16, 13590_u16, 47963_u16, 19826_u16, 26940_u16, 36370_u16, 41160_u16, 42345_u16, 61124_u16, 33901_u16, 30896_u16, 29324_u16, 37045_u16, 22354_u16, 48608_u16, 16202_u16, 12051_u16, 50088_u16, 27217_u16, 25678_u16, 48256_u16, 48681_u16, 33148_u16, 31396_u16, 30626_u16, 20113_u16, 57229_u16, 10397_u16, 12040_u16, 20130_u16, 48670_u16, 32746_u16, 10310_u16, 11146_u16, 38464_u16, 22124_u16, 9595_u16, 57416_u16, 64335_u16, 48518_u16, 7993_u16, 18576_u16, 12014_u16, 10697_u16, 51341_u16, 43023_u16, 44791_u16, 5333_u16, 34826_u16, 11506_u16, 49108_u16, 43849_u16, 4674_u16, 2584_u16, 55598_u16, 50143_u16, 65364_u16, 41404_u16, 5869_u16, 9304_u16, 27518_u16, 25661_u16, 59095_u16, 39561_u16, 7108_u16, 44822_u16, 41040_u16, 47451_u16, 46858_u16, 33660_u16, 57571_u16, 61073_u16, 11775_u16, 39181_u16, 20082_u16, 7821_u16, 53221_u16, 36944_u16, 31786_u16, 61083_u16, 45082_u16, 45348_u16, 13110_u16, 41789_u16, 19864_u16, 62137_u16, 45073_u16, 62335_u16,
	                         47469_u16, 6527_u16, 52211_u16, 64361_u16, 36699_u16, 30956_u16, 42214_u16, 39933_u16, 25437_u16, 53669_u16, 65179_u16, 55095_u16, 64589_u16, 6734_u16, 7990_u16, 14566_u16, 45756_u16, 16952_u16, 10865_u16, 45404_u16, 40852_u16, 36024_u16, 21128_u16, 32943_u16, 2649_u16, 61368_u16, 29385_u16, 18749_u16, 1270_u16, 10399_u16, 1175_u16, 16108_u16, 1205_u16, 63475_u16, 54250_u16, 60002_u16, 9043_u16, 29092_u16, 42479_u16, 43602_u16, 23654_u16, 29466_u16, 36061_u16, 42531_u16, 21730_u16, 36179_u16, 15077_u16, 16158_u16, 31253_u16, 57101_u16, 48129_u16, 63753_u16, 34338_u16, 9388_u16, 1556_u16, 60909_u16, 10287_u16, 57364_u16, 38565_u16, 35902_u16, 60878_u16, 51204_u16, 59646_u16, 60851_u16, 30978_u16, 25898_u16, 2812_u16, 40319_u16, 59035_u16, 46071_u16, 6156_u16, 39549_u16, 23018_u16, 46331_u16, 50372_u16, 33890_u16, 27251_u16, 34318_u16, 63928_u16, 15249_u16, 42505_u16, 62763_u16, 2350_u16, 8627_u16, 57547_u16, 30513_u16, 60737_u16, 60494_u16, 52191_u16, 52441_u16, 7388_u16, 46218_u16, 6048_u16, 36092_u16, 28593_u16, 63300_u16, 61702_u16, 57796_u16, 741_u16, 4403_u16, 54680_u16, 14073_u16, 621_u16, 50998_u16, 7427_u16, 25851_u16, 12792_u16, 54107_u16,
	                         8772_u16, 46179_u16, 44653_u16, 19101_u16, 3955_u16, 58733_u16, 38728_u16, 53796_u16, 50370_u16, 36031_u16, 33410_u16, 62719_u16, 29797_u16, 29135_u16, 46834_u16, 51815_u16, 38031_u16, 39292_u16, 10237_u16, 28654_u16, 55895_u16, 31692_u16, 50892_u16, 17600_u16, 16661_u16, 17313_u16, 58780_u16, 25431_u16, 31236_u16, 15599_u16, 11740_u16, 34716_u16, 61521_u16, 9651_u16, 64604_u16, 19473_u16, 42772_u16, 25862_u16, 4619_u16, 31451_u16, 6754_u16, 26760_u16, 8000_u16, 26150_u16, 40671_u16, 49737_u16, 12460_u16, 31164_u16, 51333_u16, 26231_u16, 49898_u16, 2863_u16, 29223_u16, 19595_u16, 35488_u16, 19645_u16, 48309_u16, 58948_u16, 11101_u16, 46948_u16, 52788_u16, 27109_u16, 19902_u16, 18414_u16, 34840_u16, 27184_u16, 32400_u16, 41343_u16, 46341_u16, 7223_u16, 45169_u16, 31271_u16, 3022_u16, 56816_u16, 29286_u16, 31165_u16, 6583_u16, 24963_u16, 26609_u16, 64396_u16, 50620_u16, 40460_u16, 446_u16, 34029_u16, 8846_u16, 6599_u16, 17127_u16, 16169_u16, 37460_u16, 7070_u16, 34327_u16, 30913_u16, 44394_u16, 9391_u16, 13423_u16, 40895_u16, 3005_u16, 34031_u16, 48256_u16, 63581_u16, 29485_u16, 16111_u16, 18078_u16, 4655_u16, 64234_u16, 12359_u16, 9448_u16, 26029_u16,
	                         27775_u16, 30526_u16, 31442_u16, 26572_u16, 11035_u16, 13950_u16, 23409_u16, 42792_u16, 33542_u16, 9795_u16, 37074_u16, 55941_u16, 36948_u16, 52179_u16, 22535_u16, 54652_u16, 36005_u16, 19030_u16, 49565_u16, 47386_u16, 46440_u16, 21228_u16, 47993_u16, 55324_u16, 45691_u16, 23627_u16, 37070_u16, 26737_u16, 6540_u16, 38507_u16, 4038_u16, 31370_u16, 6690_u16, 62506_u16, 57249_u16, 3360_u16, 61016_u16, 550_u16, 3933_u16, 13669_u16, 47266_u16, 6321_u16, 4940_u16, 41542_u16, 22981_u16, 46081_u16, 63070_u16, 54301_u16, 3436_u16, 26735_u16, 58073_u16, 47303_u16, 47357_u16, 24767_u16, 55442_u16, 10720_u16, 3080_u16, 59055_u16, 25986_u16, 42037_u16, 60397_u16, 9752_u16, 30288_u16, 64029_u16, 24573_u16, 60350_u16, 14877_u16, 52522_u16, 19768_u16, 4868_u16, 13021_u16, 7236_u16, 18810_u16, 58288_u16, 58586_u16, 47801_u16, 37428_u16, 63579_u16, 27550_u16, 22869_u16, 19362_u16, 27456_u16, 36266_u16, 11111_u16, 37185_u16, 4772_u16, 12961_u16, 10647_u16, 50357_u16, 11921_u16, 48668_u16, 47477_u16, 57395_u16, 56093_u16, 6898_u16, 24073_u16, 60299_u16, 41165_u16, 31011_u16, 357_u16, 17567_u16, 25267_u16, 11375_u16, 55020_u16, 32200_u16, 9663_u16, 2677_u16, 43796_u16,
	                         37217_u16, 40221_u16, 41212_u16, 6659_u16, 63598_u16, 62991_u16, 42383_u16, 21907_u16, 21163_u16, 55109_u16, 15990_u16, 59523_u16, 50721_u16, 26793_u16, 26010_u16, 31371_u16, 37037_u16, 22280_u16, 61629_u16, 30762_u16, 56916_u16, 26297_u16, 32977_u16, 59437_u16, 50338_u16, 51978_u16, 8521_u16, 2919_u16, 59641_u16, 23790_u16, 38357_u16, 17848_u16, 56411_u16, 16860_u16, 38048_u16, 38785_u16, 54381_u16, 61933_u16, 37565_u16, 11905_u16, 44973_u16, 34931_u16, 43854_u16, 13884_u16, 14164_u16, 8954_u16, 39251_u16, 34118_u16, 13197_u16, 22760_u16, 17765_u16, 27830_u16, 1278_u16, 62362_u16, 30620_u16, 51698_u16, 5825_u16, 45464_u16, 52332_u16, 23481_u16, 43500_u16, 25599_u16, 10656_u16, 2459_u16, 60982_u16, 34952_u16, 8472_u16, 36617_u16, 63480_u16, 1338_u16, 62202_u16, 56306_u16, 45482_u16, 8320_u16, 5658_u16, 18749_u16, 53028_u16, 14983_u16, 38774_u16, 52046_u16, 7738_u16, 48292_u16, 43304_u16, 9407_u16, 55026_u16, 13487_u16, 60102_u16, 3279_u16, 55969_u16, 62324_u16, 9475_u16, 48331_u16, 65252_u16, 30466_u16, 40996_u16, 58128_u16, 10221_u16, 26158_u16, 44295_u16, 20557_u16, 60283_u16, 20975_u16, 23889_u16, 63532_u16, 17488_u16, 27864_u16, 50736_u16,
	                         32227_u16, 54077_u16, 17422_u16, 24415_u16, 9288_u16, 56395_u16, 24286_u16, 47867_u16, 29860_u16, 40292_u16, 21165_u16, 48868_u16, 21517_u16, 3809_u16, 60632_u16, 30008_u16, 59903_u16, 36559_u16, 9805_u16, 30093_u16, 19030_u16, 64348_u16, 42327_u16, 43512_u16, 46973_u16, 40068_u16, 11899_u16, 1762_u16, 17117_u16, 60009_u16, 43333_u16, 9737_u16, 24639_u16, 51967_u16, 56352_u16, 2630_u16, 26610_u16, 2751_u16, 33665_u16, 32483_u16, 47321_u16, 22305_u16, 60216_u16, 39160_u16, 54865_u16, 53822_u16, 16854_u16, 12626_u16, 27012_u16, 52771_u16, 18081_u16, 63106_u16, 53984_u16, 42706_u16, 3170_u16, 16963_u16, 34147_u16, 49740_u16, 47727_u16, 29081_u16, 26105_u16, 5370_u16, 30916_u16, 40990_u16, 13652_u16, 30907_u16, 22711_u16, 10245_u16, 31360_u16, 44256_u16, 7428_u16, 26445_u16, 47593_u16, 44600_u16, 45887_u16, 4369_u16, 36654_u16, 10477_u16, 5296_u16, 45403_u16, 13660_u16, 44220_u16, 56306_u16, 2357_u16, 11513_u16, 2868_u16, 34473_u16, 56626_u16, 49181_u16, 32620_u16, 64299_u16, 25972_u16, 7089_u16, 40218_u16, 48691_u16, 23193_u16, 65420_u16, 34017_u16, 15219_u16, 47820_u16, 22041_u16, 62206_u16, 31091_u16, 35092_u16, 25495_u16, 56633_u16, 29230_u16,
	                         34788_u16, 2741_u16, 60471_u16, 62372_u16, 25525_u16, 49378_u16, 55357_u16, 17222_u16, 25138_u16, 33764_u16, 64536_u16, 33445_u16, 63212_u16, 42144_u16, 28650_u16, 2336_u16, 10555_u16, 47966_u16, 9385_u16, 22507_u16, 5344_u16, 59893_u16, 22880_u16, 40804_u16, 40050_u16, 64917_u16, 62906_u16, 24210_u16, 55131_u16, 37342_u16, 2766_u16, 58837_u16,
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
	fn Instructions(&mut self, src: &mut crate::util_::ItemArray_C) { self.dst.Instructions(Some(src)); }
}

pub struct InstructionsPack_src_adapter<'t> {
	pub src: &'t mut packs::InstructionsPack::Data_,
}

impl packs::InstructionsPack::SRC_ for InstructionsPack_src_adapter<'_> {
	fn Length(&mut self) -> u8 { self.src.Length().get() }
	fn Instructions(&mut self, dst: &mut crate::util_::ItemArray_C) {
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
	fn values(&mut self, src: &mut crate::util_::ItemArray_Z) { self.dst.values(Some(src)); }
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
