
use ad_hoc_gen::org::company as host;
use host::Client as packs;
use ad_hoc_gen::sys;
use std::sync::mpsc::*;
use host::Communication;

const some_usize: usize = 0_usize;
const some_u16: u16 = 0u16;
const some_u8: u8 = 0u8;
const some_u8s: [u8; 1] = [0u8; 1];
const some_u16s: [u16; 1] = [0u16; 1];

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

fn read_DeviceVersion(src: &mut packs::DeviceVersion::Data_) {
	let DevID = src.DevID();
	let Ver = src.Ver();
}

fn read_BusConfiguration(src: &mut packs::BusConfiguration::Data_) {
	let multiplier = src.multiplier();
	let time = src.time();
	let clk_khz = src.clk_khz();
}

fn write_BusConfiguration(dst: &mut packs::BusConfiguration::Data_) {
	dst.multiplier().set(some_u8);
	dst.time().set(some_u16);
	dst.clk_khz().set(some_u16);
}

fn read_InstructionsPack(src: &mut packs::InstructionsPack::Data_) {
	let Length = src.Length();
	let Instructions = src.Instructions(None);
}

fn write_InstructionsPack(dst: &mut packs::InstructionsPack::Data_) {
	dst.Length().set(some_u8);
	dst.Instructions(Some(&mut sys::ByValIter::new(&some_u8s)));
}

fn read_DeviceError(src: &mut packs::DeviceError::Data_) {
	let param1 = src.param1();
	let error_id = src.error_id();
}

fn read_SensorsData(src: &mut packs::SensorsData::Data_) { let values = src.values(); }
