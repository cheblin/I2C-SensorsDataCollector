
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
			pub fn DevID(&mut self) -> Item_v { Item_v(self.0) }
			pub fn Ver(&mut self) -> Item_g { Item_g(self.0) }
		}
	}
	///[DevID](Client::DeviceVersion::DevID).

	pub struct Item_v(pub *mut u8);
	impl Item_v {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[Ver](Client::DeviceVersion::Ver).

	pub struct Item_g(pub *mut u8);
	impl Item_g {
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
			pub fn param1(&mut self) -> Item_e { Item_e(self.0) }
			pub fn error_id(&mut self) -> Item_O { Item_O(self.0) }
		}
	}
	///[param1](Client::DeviceError::param1).

	pub struct Item_e(pub *mut u8);
	impl Item_e {
		pub fn get(&mut self) -> u16 {
			let src = &mut self.0;
			let dst = sys::get_bytes(self.0, 0, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 0); }
	}
	///[error_id](Client::DeviceError::error_id).

	pub struct Item_O(pub *mut u8);
	impl Item_O {
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
			pub fn values(&mut self, src: Option<&mut dyn Iterator<Item = u16>>) -> ItemArraySET_A {
				let dst = self.0;

				let len = 1000 as usize;
				let offset = 0;
				let bytes = dst;
				let mut array = ItemArraySET_A { bytes, len, offset, index: !0 };
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

	pub struct ItemArraySET_A {
		pub bytes:  *mut u8,
		pub len:    usize,
		pub offset: usize,
		pub index:  usize,
	}
	impl ItemArraySET_A {
		pub fn get(&mut self, index: usize) -> u16 {
			let dst = sys::get_bytes(self.bytes, self.offset + index * 2, 2 as usize) as u16;
			(dst) as u16
		}
		pub fn set(&mut self, index: usize, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.bytes, self.offset + index * 2); }
	}

	impl Iterator for ItemArraySET_A {
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
	dst.DevID().set(13269_u16);
	dst.Ver().set(3841_u16);
}
fn read_DeviceVersion(src: &mut packs::DeviceVersion::Data_) {
	assert_eq!(src.DevID(), 13269_u16);
	assert_eq!(src.Ver(), 3841_u16);
}
fn on_Stop() {}
fn on_Start() {}
fn on_GetDeviceVersion() {}
fn on_GetConfiguration() {}
fn on_SetConfiguration() {}
fn write_BusConfiguration(dst: &mut packs::BusConfiguration::Data_) {
	dst.multiplier().set(116_u8);
	dst.time().set(48751_u16);
	dst.clk_khz().set(21546_u16);
}
fn read_BusConfiguration(src: &mut packs::BusConfiguration::Data_) {
	assert_eq!(src.multiplier().get(), 116_u8);
	assert_eq!(src.time().get(), 48751_u16);
	assert_eq!(src.clk_khz().get(), 21546_u16);
}

fn write_InstructionsPack(dst: &mut packs::InstructionsPack::Data_) {
	dst.Length().set(58_u8);
	let norm: [u8; 256] = [
	                       200_u8, 249_u8, 15_u8, 129_u8, 190_u8, 134_u8, 68_u8, 96_u8, 125_u8, 225_u8, 163_u8, 140_u8, 80_u8, 42_u8, 16_u8, 28_u8, 187_u8, 241_u8, 219_u8, 90_u8, 131_u8, 4_u8, 209_u8, 19_u8, 60_u8, 4_u8, 128_u8, 39_u8, 102_u8, 251_u8, 54_u8, 94_u8, 123_u8, 135_u8, 130_u8, 100_u8, 124_u8, 219_u8, 104_u8, 7_u8, 144_u8, 53_u8, 82_u8, 156_u8, 62_u8, 217_u8, 71_u8, 70_u8, 113_u8, 20_u8, 32_u8, 21_u8, 115_u8, 164_u8, 111_u8, 241_u8, 17_u8, 133_u8, 195_u8, 187_u8, 196_u8, 116_u8, 93_u8, 86_u8, 164_u8, 91_u8, 162_u8, 160_u8, 227_u8, 139_u8, 143_u8, 117_u8, 152_u8, 129_u8, 250_u8, 11_u8, 95_u8, 44_u8, 69_u8, 179_u8, 31_u8, 50_u8, 68_u8, 140_u8, 126_u8, 22_u8, 23_u8, 55_u8, 181_u8, 70_u8, 139_u8, 87_u8, 46_u8, 159_u8, 67_u8, 16_u8, 208_u8, 35_u8, 77_u8, 111_u8, 111_u8, 185_u8, 232_u8, 181_u8, 205_u8, 50_u8, 117_u8, 44_u8, 144_u8, 173_u8, 139_u8, 218_u8, 124_u8, 12_u8, 32_u8, 192_u8, 181_u8, 161_u8, 38_u8, 131_u8, 216_u8, 209_u8, 31_u8, 56_u8, 85_u8, 222_u8, 107_u8, 119_u8, 74_u8, 55_u8, 232_u8, 218_u8, 49_u8, 216_u8, 148_u8, 203_u8, 141_u8, 175_u8, 15_u8, 129_u8, 117_u8, 57_u8, 241_u8, 139_u8, 19_u8, 29_u8, 173_u8, 151_u8, 30_u8, 111_u8, 100_u8, 164_u8, 137_u8, 2_u8,
	                       41_u8, 166_u8, 154_u8, 180_u8, 54_u8, 28_u8, 117_u8, 91_u8, 208_u8, 84_u8, 250_u8, 131_u8, 149_u8, 59_u8, 35_u8, 62_u8, 138_u8, 167_u8, 207_u8, 61_u8, 147_u8, 0_u8, 49_u8, 180_u8, 133_u8, 121_u8, 116_u8, 25_u8, 251_u8, 74_u8, 133_u8, 166_u8, 255_u8, 249_u8, 89_u8, 9_u8, 209_u8, 76_u8, 57_u8, 19_u8, 239_u8, 180_u8, 151_u8, 23_u8, 172_u8, 134_u8, 239_u8, 77_u8, 34_u8, 224_u8, 19_u8, 162_u8, 171_u8, 148_u8, 76_u8, 4_u8, 251_u8, 39_u8, 63_u8, 107_u8, 155_u8, 249_u8, 83_u8, 105_u8, 12_u8, 40_u8, 154_u8, 176_u8, 70_u8, 167_u8, 211_u8, 109_u8, 114_u8, 241_u8, 83_u8, 188_u8, 224_u8, 16_u8, 137_u8, 159_u8, 213_u8, 80_u8, 119_u8, 236_u8, 47_u8, 20_u8, 63_u8, 106_u8, 140_u8, 60_u8, 227_u8, 182_u8, 5_u8, 26_u8, 102_u8, 82_u8, 122_u8, 138_u8, 238_u8, 107_u8, 222_u8, 178_u8,
	];
	dst.Instructions(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_InstructionsPack(src: &mut packs::InstructionsPack::Data_) {
	assert_eq!(src.Length().get(), 58_u8);
	let norm: [u8; 256] = [
	                       200_u8, 249_u8, 15_u8, 129_u8, 190_u8, 134_u8, 68_u8, 96_u8, 125_u8, 225_u8, 163_u8, 140_u8, 80_u8, 42_u8, 16_u8, 28_u8, 187_u8, 241_u8, 219_u8, 90_u8, 131_u8, 4_u8, 209_u8, 19_u8, 60_u8, 4_u8, 128_u8, 39_u8, 102_u8, 251_u8, 54_u8, 94_u8, 123_u8, 135_u8, 130_u8, 100_u8, 124_u8, 219_u8, 104_u8, 7_u8, 144_u8, 53_u8, 82_u8, 156_u8, 62_u8, 217_u8, 71_u8, 70_u8, 113_u8, 20_u8, 32_u8, 21_u8, 115_u8, 164_u8, 111_u8, 241_u8, 17_u8, 133_u8, 195_u8, 187_u8, 196_u8, 116_u8, 93_u8, 86_u8, 164_u8, 91_u8, 162_u8, 160_u8, 227_u8, 139_u8, 143_u8, 117_u8, 152_u8, 129_u8, 250_u8, 11_u8, 95_u8, 44_u8, 69_u8, 179_u8, 31_u8, 50_u8, 68_u8, 140_u8, 126_u8, 22_u8, 23_u8, 55_u8, 181_u8, 70_u8, 139_u8, 87_u8, 46_u8, 159_u8, 67_u8, 16_u8, 208_u8, 35_u8, 77_u8, 111_u8, 111_u8, 185_u8, 232_u8, 181_u8, 205_u8, 50_u8, 117_u8, 44_u8, 144_u8, 173_u8, 139_u8, 218_u8, 124_u8, 12_u8, 32_u8, 192_u8, 181_u8, 161_u8, 38_u8, 131_u8, 216_u8, 209_u8, 31_u8, 56_u8, 85_u8, 222_u8, 107_u8, 119_u8, 74_u8, 55_u8, 232_u8, 218_u8, 49_u8, 216_u8, 148_u8, 203_u8, 141_u8, 175_u8, 15_u8, 129_u8, 117_u8, 57_u8, 241_u8, 139_u8, 19_u8, 29_u8, 173_u8, 151_u8, 30_u8, 111_u8, 100_u8, 164_u8, 137_u8, 2_u8,
	                       41_u8, 166_u8, 154_u8, 180_u8, 54_u8, 28_u8, 117_u8, 91_u8, 208_u8, 84_u8, 250_u8, 131_u8, 149_u8, 59_u8, 35_u8, 62_u8, 138_u8, 167_u8, 207_u8, 61_u8, 147_u8, 0_u8, 49_u8, 180_u8, 133_u8, 121_u8, 116_u8, 25_u8, 251_u8, 74_u8, 133_u8, 166_u8, 255_u8, 249_u8, 89_u8, 9_u8, 209_u8, 76_u8, 57_u8, 19_u8, 239_u8, 180_u8, 151_u8, 23_u8, 172_u8, 134_u8, 239_u8, 77_u8, 34_u8, 224_u8, 19_u8, 162_u8, 171_u8, 148_u8, 76_u8, 4_u8, 251_u8, 39_u8, 63_u8, 107_u8, 155_u8, 249_u8, 83_u8, 105_u8, 12_u8, 40_u8, 154_u8, 176_u8, 70_u8, 167_u8, 211_u8, 109_u8, 114_u8, 241_u8, 83_u8, 188_u8, 224_u8, 16_u8, 137_u8, 159_u8, 213_u8, 80_u8, 119_u8, 236_u8, 47_u8, 20_u8, 63_u8, 106_u8, 140_u8, 60_u8, 227_u8, 182_u8, 5_u8, 26_u8, 102_u8, 82_u8, 122_u8, 138_u8, 238_u8, 107_u8, 222_u8, 178_u8,
	];
	assert!(src.Instructions(None).eq(sys::ByValIter::new(&norm)));
}

fn write_DeviceError(dst: &mut crate::test::utils::DeviceError::Data_) {
	dst.param1().set(18527_u16);
	dst.error_id().set(packs::Errors::SensorsDataOverflow);
}
fn read_DeviceError(src: &mut packs::DeviceError::Data_) {
	assert_eq!(src.param1(), 18527_u16);
	assert_eq!(src.error_id(), packs::Errors::SensorsDataOverflow);
}

fn write_SensorsData(dst: &mut crate::test::utils::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         27795_u16, 51341_u16, 30794_u16, 26795_u16, 27472_u16, 15753_u16, 61453_u16, 5385_u16, 13348_u16, 32529_u16, 410_u16, 23422_u16, 35003_u16, 58212_u16, 7151_u16, 21498_u16, 7438_u16, 32131_u16, 46408_u16, 37741_u16, 50038_u16, 45165_u16, 64606_u16, 48240_u16, 48007_u16, 6765_u16, 16443_u16, 23691_u16, 16218_u16, 29972_u16, 58007_u16, 32459_u16, 48614_u16, 25201_u16, 12146_u16, 40283_u16, 59435_u16, 58609_u16, 8610_u16, 58987_u16, 25634_u16, 18729_u16, 57175_u16, 60189_u16, 27328_u16, 35922_u16, 58374_u16, 49575_u16, 31411_u16, 10382_u16, 57855_u16, 20947_u16, 45126_u16, 53540_u16, 3498_u16, 21432_u16, 14734_u16, 38785_u16, 38566_u16, 60619_u16, 35969_u16, 63886_u16, 52331_u16, 49771_u16, 29254_u16, 392_u16, 198_u16, 3665_u16, 60857_u16, 31837_u16, 7979_u16, 47626_u16, 35228_u16, 36950_u16, 62703_u16, 25767_u16, 29891_u16, 14137_u16, 4646_u16, 13157_u16, 58008_u16, 61584_u16, 6432_u16, 7337_u16, 25026_u16, 58898_u16, 34141_u16, 9898_u16, 21440_u16, 3060_u16, 33163_u16, 49537_u16, 52511_u16, 23324_u16, 52035_u16, 38797_u16, 58368_u16, 34044_u16, 18429_u16, 15052_u16, 59932_u16, 33025_u16, 56055_u16, 35163_u16, 53565_u16, 1065_u16, 7418_u16, 10558_u16,
	                         54414_u16, 44879_u16, 28735_u16, 33111_u16, 2763_u16, 7962_u16, 18643_u16, 51629_u16, 17670_u16, 46238_u16, 14795_u16, 31142_u16, 27075_u16, 15110_u16, 11314_u16, 30999_u16, 32228_u16, 29047_u16, 17379_u16, 582_u16, 39098_u16, 62366_u16, 1632_u16, 56837_u16, 1315_u16, 61633_u16, 35145_u16, 47743_u16, 23523_u16, 6963_u16, 28697_u16, 53175_u16, 30103_u16, 42444_u16, 40902_u16, 36846_u16, 21414_u16, 7349_u16, 49931_u16, 15488_u16, 43455_u16, 8169_u16, 41638_u16, 50739_u16, 4615_u16, 15806_u16, 61632_u16, 34027_u16, 37768_u16, 23662_u16, 49562_u16, 45052_u16, 65248_u16, 6727_u16, 48327_u16, 25150_u16, 15495_u16, 24892_u16, 51036_u16, 47364_u16, 21953_u16, 48622_u16, 59466_u16, 57220_u16, 17619_u16, 46112_u16, 18097_u16, 41248_u16, 22882_u16, 44061_u16, 14377_u16, 34544_u16, 38996_u16, 21127_u16, 60812_u16, 19298_u16, 31255_u16, 460_u16, 20128_u16, 28435_u16, 28152_u16, 2968_u16, 59223_u16, 35807_u16, 2934_u16, 30632_u16, 40050_u16, 8756_u16, 48974_u16, 25439_u16, 17692_u16, 50515_u16, 54909_u16, 58151_u16, 55231_u16, 57926_u16, 19417_u16, 34321_u16, 32381_u16, 24319_u16, 40317_u16, 32580_u16, 24293_u16, 19256_u16, 35498_u16, 8597_u16, 15344_u16, 3938_u16,
	                         21260_u16, 57167_u16, 27168_u16, 6934_u16, 37735_u16, 7241_u16, 29988_u16, 50265_u16, 49132_u16, 29992_u16, 60725_u16, 43912_u16, 33246_u16, 60659_u16, 32769_u16, 38979_u16, 30953_u16, 59920_u16, 54854_u16, 35043_u16, 24562_u16, 29080_u16, 23762_u16, 29171_u16, 50467_u16, 31458_u16, 18539_u16, 2480_u16, 26780_u16, 6029_u16, 56710_u16, 55109_u16, 44686_u16, 1315_u16, 44046_u16, 4104_u16, 7886_u16, 27821_u16, 46567_u16, 64564_u16, 35038_u16, 22634_u16, 56605_u16, 58421_u16, 63094_u16, 31570_u16, 29011_u16, 51500_u16, 21299_u16, 50775_u16, 14706_u16, 48177_u16, 38029_u16, 25003_u16, 36004_u16, 36222_u16, 30081_u16, 39517_u16, 5394_u16, 13324_u16, 43154_u16, 39764_u16, 15697_u16, 44402_u16, 48039_u16, 33447_u16, 55777_u16, 60843_u16, 7664_u16, 61369_u16, 16627_u16, 9891_u16, 46801_u16, 27188_u16, 7146_u16, 42733_u16, 9922_u16, 64924_u16, 1762_u16, 4424_u16, 22611_u16, 23725_u16, 61393_u16, 33513_u16, 9529_u16, 1234_u16, 32876_u16, 35678_u16, 61367_u16, 21286_u16, 46712_u16, 19434_u16, 54697_u16, 3943_u16, 58124_u16, 49692_u16, 38830_u16, 23126_u16, 32330_u16, 21287_u16, 56618_u16, 5387_u16, 52262_u16, 4213_u16, 27788_u16, 5978_u16, 25395_u16, 46344_u16,
	                         33172_u16, 5313_u16, 22972_u16, 30078_u16, 40776_u16, 5853_u16, 58862_u16, 43981_u16, 63884_u16, 32570_u16, 41114_u16, 64046_u16, 58608_u16, 59956_u16, 45163_u16, 25773_u16, 34206_u16, 53311_u16, 4723_u16, 23710_u16, 33005_u16, 51695_u16, 33573_u16, 16563_u16, 31127_u16, 14022_u16, 306_u16, 36040_u16, 33378_u16, 37531_u16, 30368_u16, 54906_u16, 56724_u16, 56442_u16, 61675_u16, 56497_u16, 60968_u16, 52685_u16, 44638_u16, 39694_u16, 50205_u16, 59341_u16, 52432_u16, 44526_u16, 28694_u16, 65206_u16, 47432_u16, 32557_u16, 15818_u16, 14409_u16, 64797_u16, 27178_u16, 64911_u16, 29759_u16, 5587_u16, 41976_u16, 55266_u16, 65178_u16, 24281_u16, 8932_u16, 45235_u16, 15760_u16, 21761_u16, 37317_u16, 51321_u16, 40838_u16, 21963_u16, 17524_u16, 31503_u16, 26529_u16, 3231_u16, 16755_u16, 52512_u16, 39663_u16, 60770_u16, 27493_u16, 3734_u16, 40152_u16, 21742_u16, 33919_u16, 63302_u16, 40207_u16, 44438_u16, 12440_u16, 14845_u16, 25094_u16, 28254_u16, 20556_u16, 36158_u16, 13601_u16, 57589_u16, 46791_u16, 6542_u16, 1127_u16, 21872_u16, 58357_u16, 44750_u16, 59999_u16, 49903_u16, 27240_u16, 24302_u16, 2117_u16, 31244_u16, 43324_u16, 64708_u16, 49381_u16, 14682_u16,
	                         58539_u16, 11133_u16, 15941_u16, 33171_u16, 53573_u16, 44248_u16, 24899_u16, 24696_u16, 63438_u16, 2604_u16, 38189_u16, 20939_u16, 34493_u16, 7075_u16, 31379_u16, 19390_u16, 34841_u16, 53089_u16, 48855_u16, 3279_u16, 25352_u16, 14315_u16, 17768_u16, 59219_u16, 5918_u16, 16492_u16, 47031_u16, 16640_u16, 45262_u16, 32938_u16, 11335_u16, 37524_u16, 13580_u16, 19191_u16, 28599_u16, 48409_u16, 62988_u16, 36929_u16, 32386_u16, 27406_u16, 48237_u16, 58815_u16, 4443_u16, 46633_u16, 30970_u16, 35190_u16, 31766_u16, 45235_u16, 40942_u16, 3905_u16, 57938_u16, 9688_u16, 63993_u16, 8629_u16, 55474_u16, 2040_u16, 28190_u16, 20537_u16, 6847_u16, 15760_u16, 61036_u16, 64553_u16, 27582_u16, 14042_u16, 58651_u16, 50860_u16, 42529_u16, 46956_u16, 30057_u16, 43148_u16, 38565_u16, 42364_u16, 11929_u16, 22530_u16, 50737_u16, 59237_u16, 44987_u16, 45361_u16, 6356_u16, 25949_u16, 26919_u16, 6868_u16, 42185_u16, 52151_u16, 18515_u16, 21672_u16, 2311_u16, 9906_u16, 44916_u16, 60597_u16, 27024_u16, 20035_u16, 64817_u16, 58214_u16, 19128_u16, 39136_u16, 8531_u16, 63156_u16, 46799_u16, 62599_u16, 14499_u16, 60880_u16, 64287_u16, 39459_u16, 3246_u16, 20170_u16, 38136_u16,
	                         9106_u16, 3502_u16, 32230_u16, 525_u16, 36399_u16, 24698_u16, 60063_u16, 26576_u16, 52879_u16, 60904_u16, 55225_u16, 7194_u16, 64821_u16, 439_u16, 45013_u16, 61064_u16, 52287_u16, 57318_u16, 11365_u16, 28637_u16, 14538_u16, 14178_u16, 51329_u16, 42853_u16, 56287_u16, 50924_u16, 48325_u16, 20332_u16, 54938_u16, 27559_u16, 14534_u16, 39927_u16, 9329_u16, 22862_u16, 58781_u16, 29518_u16, 61849_u16, 32940_u16, 47602_u16, 62663_u16, 48399_u16, 26898_u16, 26225_u16, 25033_u16, 612_u16, 58255_u16, 50929_u16, 56325_u16, 3411_u16, 58590_u16, 38963_u16, 28000_u16, 650_u16, 32140_u16, 4997_u16, 45067_u16, 5734_u16, 9428_u16, 38064_u16, 14225_u16, 5872_u16, 60014_u16, 7480_u16, 20932_u16, 21554_u16, 31159_u16, 11319_u16, 62649_u16, 15714_u16, 23208_u16, 204_u16, 29788_u16, 56566_u16, 14835_u16, 36481_u16, 44867_u16, 57787_u16, 6442_u16, 53599_u16, 24419_u16, 60849_u16, 47219_u16, 35169_u16, 29107_u16, 6172_u16, 31879_u16, 64781_u16, 29920_u16, 24135_u16, 54554_u16, 17633_u16, 37430_u16, 45171_u16, 48033_u16, 36468_u16, 20286_u16, 33330_u16, 25170_u16, 43878_u16, 16469_u16, 2022_u16, 41816_u16, 10500_u16, 21596_u16, 30162_u16, 25812_u16, 58338_u16, 52193_u16,
	                         52355_u16, 43738_u16, 21982_u16, 15680_u16, 39187_u16, 63015_u16, 1334_u16, 36575_u16, 9948_u16, 63432_u16, 26220_u16, 27148_u16, 30751_u16, 4760_u16, 32052_u16, 14499_u16, 37557_u16, 12947_u16, 18874_u16, 10985_u16, 53697_u16, 52977_u16, 13361_u16, 59382_u16, 18730_u16, 33002_u16, 10867_u16, 40625_u16, 8791_u16, 44822_u16, 46926_u16, 27959_u16, 8860_u16, 23365_u16, 44054_u16, 62991_u16, 37561_u16, 47092_u16, 42701_u16, 48642_u16, 49973_u16, 44353_u16, 62904_u16, 10408_u16, 48315_u16, 19879_u16, 54667_u16, 24814_u16, 23893_u16, 6268_u16, 52025_u16, 4046_u16, 26323_u16, 22383_u16, 57677_u16, 24754_u16, 3842_u16, 8444_u16, 64526_u16, 65035_u16, 25513_u16, 47719_u16, 13277_u16, 56180_u16, 16414_u16, 9070_u16, 34597_u16, 32612_u16, 32621_u16, 26046_u16, 4750_u16, 14253_u16, 49958_u16, 64669_u16, 56351_u16, 45732_u16, 45479_u16, 52700_u16, 33748_u16, 31077_u16, 2360_u16, 48131_u16, 28804_u16, 62174_u16, 64686_u16, 48024_u16, 53889_u16, 55809_u16, 5651_u16, 38321_u16, 64704_u16, 18207_u16, 27260_u16, 50263_u16, 18821_u16, 21645_u16, 50062_u16, 31697_u16, 44034_u16, 31827_u16, 6500_u16, 56959_u16, 31872_u16, 43832_u16, 28796_u16, 16122_u16, 38982_u16,
	                         28809_u16, 54802_u16, 22473_u16, 22469_u16, 52247_u16, 47207_u16, 2813_u16, 5047_u16, 54026_u16, 38727_u16, 36186_u16, 13033_u16, 16671_u16, 51025_u16, 13040_u16, 54365_u16, 38875_u16, 60635_u16, 24895_u16, 16920_u16, 45755_u16, 51790_u16, 62122_u16, 37913_u16, 30937_u16, 33974_u16, 46965_u16, 49546_u16, 16490_u16, 11752_u16, 57856_u16, 42373_u16, 12920_u16, 9401_u16, 10694_u16, 7155_u16, 11396_u16, 64948_u16, 27906_u16, 7606_u16, 59956_u16, 35773_u16, 51743_u16, 6280_u16, 8456_u16, 35302_u16, 58953_u16, 53746_u16, 40300_u16, 12058_u16, 11505_u16, 48356_u16, 49003_u16, 28308_u16, 64067_u16, 45086_u16, 16995_u16, 57894_u16, 38032_u16, 24981_u16, 32510_u16, 52010_u16, 41561_u16, 57462_u16, 15621_u16, 6603_u16, 1639_u16, 55154_u16, 53569_u16, 48969_u16, 49773_u16, 46519_u16, 51917_u16, 31810_u16, 13205_u16, 6615_u16, 60740_u16, 49596_u16, 63903_u16, 30136_u16, 4306_u16, 13435_u16, 938_u16, 1975_u16, 32852_u16, 42452_u16, 3553_u16, 35482_u16, 766_u16, 3974_u16, 23450_u16, 62305_u16, 60298_u16, 6872_u16, 27153_u16, 56524_u16, 22135_u16, 24962_u16, 54647_u16, 49446_u16, 47620_u16, 56426_u16, 37903_u16, 31430_u16, 13493_u16, 3370_u16, 21767_u16, 53268_u16,
	                         42786_u16, 49105_u16, 20381_u16, 35785_u16, 4307_u16, 13832_u16, 14521_u16, 2617_u16, 52268_u16, 43487_u16, 16799_u16, 26598_u16, 63296_u16, 32052_u16, 44278_u16, 32608_u16, 18990_u16, 27288_u16, 3696_u16, 46221_u16, 12115_u16, 14894_u16, 64315_u16, 55909_u16, 11180_u16, 26376_u16, 48559_u16, 5277_u16, 48262_u16, 60144_u16, 24702_u16, 22367_u16, 18341_u16, 64022_u16, 9680_u16, 47778_u16, 29003_u16, 28461_u16, 62020_u16, 40383_u16, 55699_u16, 58334_u16, 3105_u16, 44278_u16, 51932_u16, 46778_u16, 39451_u16, 52734_u16, 48399_u16, 27149_u16, 6622_u16, 51134_u16, 53447_u16, 8748_u16, 10290_u16, 28531_u16, 25995_u16, 43111_u16, 30633_u16, 7361_u16, 34865_u16, 26574_u16, 9070_u16, 17136_u16, 6333_u16, 30331_u16, 10239_u16, 9198_u16, 62279_u16, 37466_u16, 9644_u16, 51782_u16, 17687_u16, 56464_u16, 29139_u16, 17999_u16, 26245_u16, 21638_u16, 53049_u16, 38219_u16, 1541_u16, 13690_u16, 57205_u16, 2536_u16, 28217_u16, 43532_u16, 44106_u16, 10361_u16, 49991_u16, 29552_u16, 2660_u16, 38470_u16, 34282_u16, 26914_u16, 65103_u16, 9197_u16, 1022_u16, 29818_u16, 8893_u16, 60307_u16, 62300_u16, 39592_u16, 5993_u16, 63266_u16, 10570_u16, 13907_u16, 4638_u16, 15830_u16,
	                         15853_u16, 46752_u16, 39703_u16, 1421_u16, 19415_u16, 45682_u16, 23439_u16, 11844_u16, 52563_u16, 11330_u16, 7124_u16, 24657_u16, 58595_u16, 5224_u16, 44890_u16, 53223_u16, 2707_u16, 21681_u16, 26009_u16, 4745_u16, 36401_u16, 10122_u16, 58877_u16, 33342_u16, 34730_u16, 40712_u16, 5772_u16, 1084_u16, 59959_u16, 55020_u16, 54600_u16,
	];
	dst.values(Some(&mut sys::ByValIter::new(&norm)));
}
fn read_SensorsData(src: &mut packs::SensorsData::Data_) {
	let norm: [u16; 1000] = [
	                         27795_u16, 51341_u16, 30794_u16, 26795_u16, 27472_u16, 15753_u16, 61453_u16, 5385_u16, 13348_u16, 32529_u16, 410_u16, 23422_u16, 35003_u16, 58212_u16, 7151_u16, 21498_u16, 7438_u16, 32131_u16, 46408_u16, 37741_u16, 50038_u16, 45165_u16, 64606_u16, 48240_u16, 48007_u16, 6765_u16, 16443_u16, 23691_u16, 16218_u16, 29972_u16, 58007_u16, 32459_u16, 48614_u16, 25201_u16, 12146_u16, 40283_u16, 59435_u16, 58609_u16, 8610_u16, 58987_u16, 25634_u16, 18729_u16, 57175_u16, 60189_u16, 27328_u16, 35922_u16, 58374_u16, 49575_u16, 31411_u16, 10382_u16, 57855_u16, 20947_u16, 45126_u16, 53540_u16, 3498_u16, 21432_u16, 14734_u16, 38785_u16, 38566_u16, 60619_u16, 35969_u16, 63886_u16, 52331_u16, 49771_u16, 29254_u16, 392_u16, 198_u16, 3665_u16, 60857_u16, 31837_u16, 7979_u16, 47626_u16, 35228_u16, 36950_u16, 62703_u16, 25767_u16, 29891_u16, 14137_u16, 4646_u16, 13157_u16, 58008_u16, 61584_u16, 6432_u16, 7337_u16, 25026_u16, 58898_u16, 34141_u16, 9898_u16, 21440_u16, 3060_u16, 33163_u16, 49537_u16, 52511_u16, 23324_u16, 52035_u16, 38797_u16, 58368_u16, 34044_u16, 18429_u16, 15052_u16, 59932_u16, 33025_u16, 56055_u16, 35163_u16, 53565_u16, 1065_u16, 7418_u16, 10558_u16,
	                         54414_u16, 44879_u16, 28735_u16, 33111_u16, 2763_u16, 7962_u16, 18643_u16, 51629_u16, 17670_u16, 46238_u16, 14795_u16, 31142_u16, 27075_u16, 15110_u16, 11314_u16, 30999_u16, 32228_u16, 29047_u16, 17379_u16, 582_u16, 39098_u16, 62366_u16, 1632_u16, 56837_u16, 1315_u16, 61633_u16, 35145_u16, 47743_u16, 23523_u16, 6963_u16, 28697_u16, 53175_u16, 30103_u16, 42444_u16, 40902_u16, 36846_u16, 21414_u16, 7349_u16, 49931_u16, 15488_u16, 43455_u16, 8169_u16, 41638_u16, 50739_u16, 4615_u16, 15806_u16, 61632_u16, 34027_u16, 37768_u16, 23662_u16, 49562_u16, 45052_u16, 65248_u16, 6727_u16, 48327_u16, 25150_u16, 15495_u16, 24892_u16, 51036_u16, 47364_u16, 21953_u16, 48622_u16, 59466_u16, 57220_u16, 17619_u16, 46112_u16, 18097_u16, 41248_u16, 22882_u16, 44061_u16, 14377_u16, 34544_u16, 38996_u16, 21127_u16, 60812_u16, 19298_u16, 31255_u16, 460_u16, 20128_u16, 28435_u16, 28152_u16, 2968_u16, 59223_u16, 35807_u16, 2934_u16, 30632_u16, 40050_u16, 8756_u16, 48974_u16, 25439_u16, 17692_u16, 50515_u16, 54909_u16, 58151_u16, 55231_u16, 57926_u16, 19417_u16, 34321_u16, 32381_u16, 24319_u16, 40317_u16, 32580_u16, 24293_u16, 19256_u16, 35498_u16, 8597_u16, 15344_u16, 3938_u16,
	                         21260_u16, 57167_u16, 27168_u16, 6934_u16, 37735_u16, 7241_u16, 29988_u16, 50265_u16, 49132_u16, 29992_u16, 60725_u16, 43912_u16, 33246_u16, 60659_u16, 32769_u16, 38979_u16, 30953_u16, 59920_u16, 54854_u16, 35043_u16, 24562_u16, 29080_u16, 23762_u16, 29171_u16, 50467_u16, 31458_u16, 18539_u16, 2480_u16, 26780_u16, 6029_u16, 56710_u16, 55109_u16, 44686_u16, 1315_u16, 44046_u16, 4104_u16, 7886_u16, 27821_u16, 46567_u16, 64564_u16, 35038_u16, 22634_u16, 56605_u16, 58421_u16, 63094_u16, 31570_u16, 29011_u16, 51500_u16, 21299_u16, 50775_u16, 14706_u16, 48177_u16, 38029_u16, 25003_u16, 36004_u16, 36222_u16, 30081_u16, 39517_u16, 5394_u16, 13324_u16, 43154_u16, 39764_u16, 15697_u16, 44402_u16, 48039_u16, 33447_u16, 55777_u16, 60843_u16, 7664_u16, 61369_u16, 16627_u16, 9891_u16, 46801_u16, 27188_u16, 7146_u16, 42733_u16, 9922_u16, 64924_u16, 1762_u16, 4424_u16, 22611_u16, 23725_u16, 61393_u16, 33513_u16, 9529_u16, 1234_u16, 32876_u16, 35678_u16, 61367_u16, 21286_u16, 46712_u16, 19434_u16, 54697_u16, 3943_u16, 58124_u16, 49692_u16, 38830_u16, 23126_u16, 32330_u16, 21287_u16, 56618_u16, 5387_u16, 52262_u16, 4213_u16, 27788_u16, 5978_u16, 25395_u16, 46344_u16,
	                         33172_u16, 5313_u16, 22972_u16, 30078_u16, 40776_u16, 5853_u16, 58862_u16, 43981_u16, 63884_u16, 32570_u16, 41114_u16, 64046_u16, 58608_u16, 59956_u16, 45163_u16, 25773_u16, 34206_u16, 53311_u16, 4723_u16, 23710_u16, 33005_u16, 51695_u16, 33573_u16, 16563_u16, 31127_u16, 14022_u16, 306_u16, 36040_u16, 33378_u16, 37531_u16, 30368_u16, 54906_u16, 56724_u16, 56442_u16, 61675_u16, 56497_u16, 60968_u16, 52685_u16, 44638_u16, 39694_u16, 50205_u16, 59341_u16, 52432_u16, 44526_u16, 28694_u16, 65206_u16, 47432_u16, 32557_u16, 15818_u16, 14409_u16, 64797_u16, 27178_u16, 64911_u16, 29759_u16, 5587_u16, 41976_u16, 55266_u16, 65178_u16, 24281_u16, 8932_u16, 45235_u16, 15760_u16, 21761_u16, 37317_u16, 51321_u16, 40838_u16, 21963_u16, 17524_u16, 31503_u16, 26529_u16, 3231_u16, 16755_u16, 52512_u16, 39663_u16, 60770_u16, 27493_u16, 3734_u16, 40152_u16, 21742_u16, 33919_u16, 63302_u16, 40207_u16, 44438_u16, 12440_u16, 14845_u16, 25094_u16, 28254_u16, 20556_u16, 36158_u16, 13601_u16, 57589_u16, 46791_u16, 6542_u16, 1127_u16, 21872_u16, 58357_u16, 44750_u16, 59999_u16, 49903_u16, 27240_u16, 24302_u16, 2117_u16, 31244_u16, 43324_u16, 64708_u16, 49381_u16, 14682_u16,
	                         58539_u16, 11133_u16, 15941_u16, 33171_u16, 53573_u16, 44248_u16, 24899_u16, 24696_u16, 63438_u16, 2604_u16, 38189_u16, 20939_u16, 34493_u16, 7075_u16, 31379_u16, 19390_u16, 34841_u16, 53089_u16, 48855_u16, 3279_u16, 25352_u16, 14315_u16, 17768_u16, 59219_u16, 5918_u16, 16492_u16, 47031_u16, 16640_u16, 45262_u16, 32938_u16, 11335_u16, 37524_u16, 13580_u16, 19191_u16, 28599_u16, 48409_u16, 62988_u16, 36929_u16, 32386_u16, 27406_u16, 48237_u16, 58815_u16, 4443_u16, 46633_u16, 30970_u16, 35190_u16, 31766_u16, 45235_u16, 40942_u16, 3905_u16, 57938_u16, 9688_u16, 63993_u16, 8629_u16, 55474_u16, 2040_u16, 28190_u16, 20537_u16, 6847_u16, 15760_u16, 61036_u16, 64553_u16, 27582_u16, 14042_u16, 58651_u16, 50860_u16, 42529_u16, 46956_u16, 30057_u16, 43148_u16, 38565_u16, 42364_u16, 11929_u16, 22530_u16, 50737_u16, 59237_u16, 44987_u16, 45361_u16, 6356_u16, 25949_u16, 26919_u16, 6868_u16, 42185_u16, 52151_u16, 18515_u16, 21672_u16, 2311_u16, 9906_u16, 44916_u16, 60597_u16, 27024_u16, 20035_u16, 64817_u16, 58214_u16, 19128_u16, 39136_u16, 8531_u16, 63156_u16, 46799_u16, 62599_u16, 14499_u16, 60880_u16, 64287_u16, 39459_u16, 3246_u16, 20170_u16, 38136_u16,
	                         9106_u16, 3502_u16, 32230_u16, 525_u16, 36399_u16, 24698_u16, 60063_u16, 26576_u16, 52879_u16, 60904_u16, 55225_u16, 7194_u16, 64821_u16, 439_u16, 45013_u16, 61064_u16, 52287_u16, 57318_u16, 11365_u16, 28637_u16, 14538_u16, 14178_u16, 51329_u16, 42853_u16, 56287_u16, 50924_u16, 48325_u16, 20332_u16, 54938_u16, 27559_u16, 14534_u16, 39927_u16, 9329_u16, 22862_u16, 58781_u16, 29518_u16, 61849_u16, 32940_u16, 47602_u16, 62663_u16, 48399_u16, 26898_u16, 26225_u16, 25033_u16, 612_u16, 58255_u16, 50929_u16, 56325_u16, 3411_u16, 58590_u16, 38963_u16, 28000_u16, 650_u16, 32140_u16, 4997_u16, 45067_u16, 5734_u16, 9428_u16, 38064_u16, 14225_u16, 5872_u16, 60014_u16, 7480_u16, 20932_u16, 21554_u16, 31159_u16, 11319_u16, 62649_u16, 15714_u16, 23208_u16, 204_u16, 29788_u16, 56566_u16, 14835_u16, 36481_u16, 44867_u16, 57787_u16, 6442_u16, 53599_u16, 24419_u16, 60849_u16, 47219_u16, 35169_u16, 29107_u16, 6172_u16, 31879_u16, 64781_u16, 29920_u16, 24135_u16, 54554_u16, 17633_u16, 37430_u16, 45171_u16, 48033_u16, 36468_u16, 20286_u16, 33330_u16, 25170_u16, 43878_u16, 16469_u16, 2022_u16, 41816_u16, 10500_u16, 21596_u16, 30162_u16, 25812_u16, 58338_u16, 52193_u16,
	                         52355_u16, 43738_u16, 21982_u16, 15680_u16, 39187_u16, 63015_u16, 1334_u16, 36575_u16, 9948_u16, 63432_u16, 26220_u16, 27148_u16, 30751_u16, 4760_u16, 32052_u16, 14499_u16, 37557_u16, 12947_u16, 18874_u16, 10985_u16, 53697_u16, 52977_u16, 13361_u16, 59382_u16, 18730_u16, 33002_u16, 10867_u16, 40625_u16, 8791_u16, 44822_u16, 46926_u16, 27959_u16, 8860_u16, 23365_u16, 44054_u16, 62991_u16, 37561_u16, 47092_u16, 42701_u16, 48642_u16, 49973_u16, 44353_u16, 62904_u16, 10408_u16, 48315_u16, 19879_u16, 54667_u16, 24814_u16, 23893_u16, 6268_u16, 52025_u16, 4046_u16, 26323_u16, 22383_u16, 57677_u16, 24754_u16, 3842_u16, 8444_u16, 64526_u16, 65035_u16, 25513_u16, 47719_u16, 13277_u16, 56180_u16, 16414_u16, 9070_u16, 34597_u16, 32612_u16, 32621_u16, 26046_u16, 4750_u16, 14253_u16, 49958_u16, 64669_u16, 56351_u16, 45732_u16, 45479_u16, 52700_u16, 33748_u16, 31077_u16, 2360_u16, 48131_u16, 28804_u16, 62174_u16, 64686_u16, 48024_u16, 53889_u16, 55809_u16, 5651_u16, 38321_u16, 64704_u16, 18207_u16, 27260_u16, 50263_u16, 18821_u16, 21645_u16, 50062_u16, 31697_u16, 44034_u16, 31827_u16, 6500_u16, 56959_u16, 31872_u16, 43832_u16, 28796_u16, 16122_u16, 38982_u16,
	                         28809_u16, 54802_u16, 22473_u16, 22469_u16, 52247_u16, 47207_u16, 2813_u16, 5047_u16, 54026_u16, 38727_u16, 36186_u16, 13033_u16, 16671_u16, 51025_u16, 13040_u16, 54365_u16, 38875_u16, 60635_u16, 24895_u16, 16920_u16, 45755_u16, 51790_u16, 62122_u16, 37913_u16, 30937_u16, 33974_u16, 46965_u16, 49546_u16, 16490_u16, 11752_u16, 57856_u16, 42373_u16, 12920_u16, 9401_u16, 10694_u16, 7155_u16, 11396_u16, 64948_u16, 27906_u16, 7606_u16, 59956_u16, 35773_u16, 51743_u16, 6280_u16, 8456_u16, 35302_u16, 58953_u16, 53746_u16, 40300_u16, 12058_u16, 11505_u16, 48356_u16, 49003_u16, 28308_u16, 64067_u16, 45086_u16, 16995_u16, 57894_u16, 38032_u16, 24981_u16, 32510_u16, 52010_u16, 41561_u16, 57462_u16, 15621_u16, 6603_u16, 1639_u16, 55154_u16, 53569_u16, 48969_u16, 49773_u16, 46519_u16, 51917_u16, 31810_u16, 13205_u16, 6615_u16, 60740_u16, 49596_u16, 63903_u16, 30136_u16, 4306_u16, 13435_u16, 938_u16, 1975_u16, 32852_u16, 42452_u16, 3553_u16, 35482_u16, 766_u16, 3974_u16, 23450_u16, 62305_u16, 60298_u16, 6872_u16, 27153_u16, 56524_u16, 22135_u16, 24962_u16, 54647_u16, 49446_u16, 47620_u16, 56426_u16, 37903_u16, 31430_u16, 13493_u16, 3370_u16, 21767_u16, 53268_u16,
	                         42786_u16, 49105_u16, 20381_u16, 35785_u16, 4307_u16, 13832_u16, 14521_u16, 2617_u16, 52268_u16, 43487_u16, 16799_u16, 26598_u16, 63296_u16, 32052_u16, 44278_u16, 32608_u16, 18990_u16, 27288_u16, 3696_u16, 46221_u16, 12115_u16, 14894_u16, 64315_u16, 55909_u16, 11180_u16, 26376_u16, 48559_u16, 5277_u16, 48262_u16, 60144_u16, 24702_u16, 22367_u16, 18341_u16, 64022_u16, 9680_u16, 47778_u16, 29003_u16, 28461_u16, 62020_u16, 40383_u16, 55699_u16, 58334_u16, 3105_u16, 44278_u16, 51932_u16, 46778_u16, 39451_u16, 52734_u16, 48399_u16, 27149_u16, 6622_u16, 51134_u16, 53447_u16, 8748_u16, 10290_u16, 28531_u16, 25995_u16, 43111_u16, 30633_u16, 7361_u16, 34865_u16, 26574_u16, 9070_u16, 17136_u16, 6333_u16, 30331_u16, 10239_u16, 9198_u16, 62279_u16, 37466_u16, 9644_u16, 51782_u16, 17687_u16, 56464_u16, 29139_u16, 17999_u16, 26245_u16, 21638_u16, 53049_u16, 38219_u16, 1541_u16, 13690_u16, 57205_u16, 2536_u16, 28217_u16, 43532_u16, 44106_u16, 10361_u16, 49991_u16, 29552_u16, 2660_u16, 38470_u16, 34282_u16, 26914_u16, 65103_u16, 9197_u16, 1022_u16, 29818_u16, 8893_u16, 60307_u16, 62300_u16, 39592_u16, 5993_u16, 63266_u16, 10570_u16, 13907_u16, 4638_u16, 15830_u16,
	                         15853_u16, 46752_u16, 39703_u16, 1421_u16, 19415_u16, 45682_u16, 23439_u16, 11844_u16, 52563_u16, 11330_u16, 7124_u16, 24657_u16, 58595_u16, 5224_u16, 44890_u16, 53223_u16, 2707_u16, 21681_u16, 26009_u16, 4745_u16, 36401_u16, 10122_u16, 58877_u16, 33342_u16, 34730_u16, 40712_u16, 5772_u16, 1084_u16, 59959_u16, 55020_u16, 54600_u16,
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
	fn Instructions(&mut self, src: &mut crate::util_::ItemArray_U) { self.dst.Instructions(Some(src)); }
}

pub struct InstructionsPack_src_adapter<'t> {
	pub src: &'t mut packs::InstructionsPack::Data_,
}

impl packs::InstructionsPack::SRC_ for InstructionsPack_src_adapter<'_> {
	fn Length(&mut self) -> u8 { self.src.Length().get() }
	fn Instructions(&mut self, dst: &mut crate::util_::ItemArray_U) {
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
	fn values(&mut self, src: &mut crate::util_::ItemArray_A) { self.dst.values(Some(src)); }
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
