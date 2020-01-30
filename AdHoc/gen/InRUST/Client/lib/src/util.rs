
use ad_hoc_sys as sys;

use std::str::from_utf8_unchecked_mut;
use std::slice::from_raw_parts_mut;
use std::mem::transmute;
use std::ptr::copy_nonoverlapping;
use crate::org::company as host;
use host::Client as packs;

#[repr(C)]
pub struct Meta0(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta0 {}
pub static mut DeviceVersion: Meta0 = Meta0(0, 0, 0, 0, 4, None, 0, 0, 0, []);
unsafe extern "C" fn meta1alloc(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack { transmute(&host::Client::Stop::meta_) }

#[repr(C)]
pub struct Meta1(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta1 {}
pub static mut Stop: Meta1 = Meta1(1, 0, 0, 0, 0, Some(meta1alloc), 0, 0, 0, []);
unsafe extern "C" fn meta2alloc(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack { transmute(&host::Client::Start::meta_) }

#[repr(C)]
pub struct Meta2(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta2 {}
pub static mut Start: Meta2 = Meta2(2, 0, 0, 0, 0, Some(meta2alloc), 0, 0, 0, []);
unsafe extern "C" fn meta3alloc(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack { transmute(&host::Client::GetDeviceVersion::meta_) }

#[repr(C)]
pub struct Meta3(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta3 {}
pub static mut GetDeviceVersion: Meta3 = Meta3(3, 0, 0, 0, 0, Some(meta3alloc), 0, 0, 0, []);
unsafe extern "C" fn meta4alloc(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack { transmute(&host::Client::GetConfiguration::meta_) }

#[repr(C)]
pub struct Meta4(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta4 {}
pub static mut GetConfiguration: Meta4 = Meta4(4, 0, 0, 0, 0, Some(meta4alloc), 0, 0, 0, []);
unsafe extern "C" fn meta5alloc(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack { transmute(&host::Client::SetConfiguration::meta_) }

#[repr(C)]
pub struct Meta5(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta5 {}
pub static mut SetConfiguration: Meta5 = Meta5(5, 0, 0, 0, 0, Some(meta5alloc), 0, 0, 0, []);
#[repr(C)]
pub struct Meta6(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta6 {}
pub static mut BusConfiguration: Meta6 = Meta6(6, 0, 0, 0, 5, None, 0, 0, 0, []);
///[multiplier](Client::BusConfiguration::multiplier).

pub struct Item_V(pub *mut u8);
impl Item_V {
	pub fn get(&mut self) -> u8 {
		let src = &mut self.0;
		let dst = sys::get_bytes(self.0, 0, 1 as usize) as u8;
		(dst) as u8
	}
	pub fn set(&mut self, src: u8) { sys::set_bytes((src) as u64, 1 as usize, self.0, 0); }
}
///[time](Client::BusConfiguration::time).

pub struct Item_f(pub *mut u8);
impl Item_f {
	pub fn get(&mut self) -> u16 {
		let src = &mut self.0;
		let dst = sys::get_bytes(self.0, 1, 2 as usize) as u16;
		(dst) as u16
	}
	pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 1); }
}
///[clk_khz](Client::BusConfiguration::clk_khz).

pub struct Item_l(pub *mut u8);
impl Item_l {
	pub fn get(&mut self) -> u16 {
		let src = &mut self.0;
		let dst = sys::get_bytes(self.0, 3, 2 as usize) as u16;
		(dst) as u16
	}
	pub fn set(&mut self, src: u16) { sys::set_bytes((src) as u64, 2 as usize, self.0, 3); }
}

#[repr(C)]
pub struct Meta7(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta7 {}
pub static mut InstructionsPack: Meta7 = Meta7(7, 0, 0, 0, 257, None, 0, 0, 0, []);
///[Length](Client::InstructionsPack::Length).

pub struct Item_A(pub *mut u8);
impl Item_A {
	pub fn get(&mut self) -> u8 {
		let src = &mut self.0;
		let dst = sys::get_bytes(self.0, 0, 1 as usize) as u8;
		(dst) as u8
	}
	pub fn set(&mut self, src: u8) { sys::set_bytes((src) as u64, 1 as usize, self.0, 0); }
}

///[Instructions](Client::InstructionsPack::Instructions).

pub struct ItemArray_C {
	pub bytes:  *mut u8,
	pub len:    usize,
	pub offset: usize,
	pub index:  usize,
}
impl ItemArray_C {
	pub fn get(&mut self, index: usize) -> u8 {
		let dst = sys::get_bytes(self.bytes, self.offset + index * 1, 1 as usize) as u8;
		(dst) as u8
	}
	pub fn set(&mut self, index: usize, src: u8) { sys::set_bytes((src) as u64, 1 as usize, self.bytes, self.offset + index * 1); }
}

impl Iterator for ItemArray_C {
	type Item = u8;

	fn next(&mut self) -> Option<<Self as Iterator>::Item> {
		self.index = self.index.wrapping_add(1);
		if self.index < self.len {
			return Some(self.get(self.index));
		}
		self.index = !0;
		None
	}
}

#[repr(C)]
pub struct Meta8(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta8 {}
pub static mut DeviceError: Meta8 = Meta8(8, 0, 0, 0, 3, None, 0, 0, 0, []);
#[repr(C)]
pub struct Meta9(pub u16, u32, u32, u32, pub u32, pub Option<unsafe extern "C" fn(pack: *mut sys::Pack, len: usize) -> *mut sys::Pack>, u16, u32, u16, [*const sys::Field; 0]);
unsafe impl std::marker::Sync for Meta9 {}
pub static mut SensorsData: Meta9 = Meta9(9, 0, 0, 0, 2000, None, 0, 0, 0, []);
///[values](Client::SensorsData::values).

pub struct ItemArray_Z {
	pub bytes:  *const u8,
	pub len:    usize,
	pub offset: usize,
	pub index:  usize,
}
impl ItemArray_Z {
	pub fn get(&mut self, index: usize) -> u16 {
		let dst = sys::get_bytes(self.bytes, self.offset + index * 2, 2 as usize) as u16;
		(dst) as u16
	}
}

impl Iterator for ItemArray_Z {
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

pub const RECEIVE_REQ_MAX_BYTES: u32 = 2000u32;
pub const RECEIVE_FULL_MAX_BYTES: u32 = 2000u32;

pub const SEND_REQ_MAX_BYTES: u32 = 257u32;
pub const SEND_FULL_MAX_BYTES: u32 = 257u32;
