#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_variables)]
#![feature(const_fn)]
#![feature(const_transmute)]


include!(concat!(env!("OUT_DIR"), "/adapter.rs"));

pub struct ByValIter<'a, T: Copy>(&'a [T], usize);

impl<'a, T: Copy> ByValIter<'_, T> {
    pub fn new(src: &'a [T]) -> ByValIter<'_, T> {
        ByValIter(src, !0)
    }
}

impl<'a, T> ::std::iter::Iterator for ByValIter<'a, T> where T: Copy {
    type Item = T;
    
    fn next(&mut self) -> Option<<Self as Iterator>::Item> {
        self.1 = self.1.wrapping_add(1);
        if self.1 < self.0.len() { return Some(self.0[self.1].clone()); }
        self.1 = !0;
        None
    }
}

use std::mem::{transmute, size_of};
use std::ptr::{copy_nonoverlapping, null_mut};


//extern  fn host_event(place: u32, receiver: *mut Receiver, transmitter: *mut Transmitter, pack: *mut Pack, event: HOST_EVENT){ }


impl Pack {
    #[inline]
    pub  fn new(meta: &Meta) -> *mut Pack {
        unsafe {
            pack_alloc( meta, null_mut(), std::usize::MAX)
        }
    }
    
    #[inline]
    pub fn base_bytes<'t>(pack: *mut Pack) -> &'t [u8] { unsafe { ::std::slice::from_raw_parts((*pack).bytes.as_ptr(), (*(*pack).meta).packMinBytes as usize) } }
    #[inline]
    pub fn base_bytes_mut<'t>(pack: *mut Pack) -> &'t mut [u8] { unsafe { ::std::slice::from_raw_parts_mut((*pack).bytes.as_mut_ptr(), (*(*pack).meta).packMinBytes as usize) } }
    pub fn free(pack: *mut Pack) { unsafe { pack_alloc( null_mut(), pack, std::usize::MAX ); } }
}

unsafe impl std::marker::Sync for Pack {}

impl CursorBase {
    #[inline(always)]
    pub fn get_field(&self) -> &Field {
        unsafe {
            let meta = self.meta.as_ref().unwrap();
            &*(meta.fields.as_slice(meta.fields_count as usize)[(self.field_bit - meta.field_0_bit as i32) as usize])
        }
    }
    #[inline(always)]
    pub fn pack_size(&mut self) -> usize { unsafe { if self.LAST_BYTE < 0 { pack_size(self) } else { self.LAST_BYTE as usize } } }
    
    pub fn eq(&mut self, other: &mut Self) -> bool {
        unsafe {
            (*self.pack).meta == (*other.pack).meta && {
                let len = self.pack_size();
                other.pack_size() == len && (*self.pack).bytes.as_slice(len) == (*other.pack).bytes.as_slice(len)
            }
        }
    }
}

static Ol: [u8; 9] = [0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff];
static lO: [u8; 9] = [0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff];

#[inline]
pub fn set_bytes(src: u64, bytes: usize, dst: *mut u8, byte: usize) {
    unsafe {
        let src = (&src as *const u64) as *const u8;
        let dst = dst.offset(byte as isize);
        
        #[cfg(target_endian = "little")]
            copy_nonoverlapping(src, dst, bytes);
        #[cfg(not(target_endian = "little"))]
            for p in 0..bytes as isize {
            *dst.offset(p) = *src.offset(8 - p);
        }
    }
}

#[inline]
pub fn get_bytes(src: *const u8, byte: usize, bytes: usize) -> u64 {
    let mut ret: u64 = 0;
    unsafe {
        let src = src.offset(byte as isize);
        let dst = (&mut ret as *mut u64) as *mut u8;
        
        #[cfg(target_endian = "little")]
            copy_nonoverlapping(src, dst, bytes);
        #[cfg(not(target_endian = "little"))]
            for p in 0..bytes as isize {
            *dst.offset(8 - p) = *src.offset(p);
        }
    }
    ret
}

#[inline]
pub fn set_bits(src: u64, bits: usize, dst: *mut u8, bit: usize) {
    unsafe {
        let mut dst = dst.offset((bit >> 3) as isize);
        let bit = (bit & 7) as usize;
        
        if 8 < bit + bits
        {
            let mut bits = bits;
            let mut src = src;
            let src_ptr = (&src as *const u64) as *const u8;
            
            if 0 < bit //отработка начального смещени¤ bit в первом байте
            {
                *dst = (*dst & Ol[bit] | //младшие биты первого байта dst которые нужно сохранить остальное в нули
                    (*src_ptr & Ol[       //выдел¤ем младщие биты из источника которые станут старшими в первом байте dst
                        8 - bit     // число старших бит которые предстоит заполнить
                        ]) << bit) as u8;      //сместили их на старшие места
                
                src >>= 8 - bit;        //выбрасываем уже передананные биты
                bits -= 8 - bit;        //уменьшаем на длинну обработанных бит
                dst = dst.offset(1);//первый байт отработан
            }
            
            set_bytes(src, bits >> 3, dst, 0);
            
            if 0 < bits & 7                 //ещё остались побайтно недокопированные биты
            {
                dst = dst.offset((bits >> 3) as isize);                //смещаемс¤, дл¤ простоты выражений
                *dst = (*dst & lO[8 - (bits & 7)] // маской типа 11100000 сохран¤ем старшие биты dst уничтожа¤ младшие
                    |
                    ((src >> ((bits >> 3) << 3)) as u8) //преобразуем источник в байтовый массив вытаскива¤ из него последний байт
                        & Ol[bits & 7]) as u8;               //и выдел¤¤ из этого байта недокопированные биты
            }
        } else { *dst = *dst & (Ol[bit] | lO[8 - bit - bits]) | (src as u8 & Ol[bits]) << bit; }
    }
}

#[inline]
pub fn get_bits(src: *const u8, bit: usize, bits: usize) -> u64 {
    unsafe {
        let mut src = src.offset((bit >> 3) as isize);
        let mut bit = (bit & 7) as usize;
        if bit + bits < 9 { return ((*src >> bit) & Ol[bits]) as u64; }
        
        let mut bits = bits;
        
        if 8 < bits >> 3 { bits = 8 << 3; }
        
        let mut dst = get_bytes(src, 0, (bit + bits) >> 3);//копируем из src в dst как есть всe что пможно скопировать
        
        dst >>= bit; //всЄ скопированное смещаем !!! >>> !!!
        src = src.offset(((bit + bits) >> 3) as isize);
        bit = bit + bits & 7; //число недокопированных бит
        if 0 < bit { dst |= ((*src & Ol[bit]) as u64) << bits - bit; }
        
        dst
    }
}

#[inline(always)]
pub fn get_bit(src: *const u8, bit: usize) -> bool {
    unsafe { *src.offset((bit >> 3) as isize) & 1 << (bit & 7) != 0 }
}

#[inline(always)]
pub fn set_bit(src: bool, bit: usize, dst: *mut u8) {
    unsafe {
        if src { *dst.offset((bit >> 3) as isize) |= 1 << (bit & 7); } else { *dst.offset((bit >> 3) as isize) &= !(1 << (bit & 7)); }
    }
}

#[repr(C)]
pub struct Cursors(pub [Cursor; AD_HOC_NESTED_MAX as usize]);

impl Cursors {
    pub const fn new() -> Cursors { unsafe { transmute([0u8; ::std::mem::size_of::<Cursors>()]) } }
    pub fn unwrap(&mut self) -> *mut Pack {
        let pack = self.0[0].base.pack;
        if pack.is_null() { return ::std::ptr::null_mut(); }
        for cur in self.0.iter_mut() {
            cur.base.pack = ::std::ptr::null_mut();
            cur.base.meta = ::std::ptr::null_mut();
            cur.base.bytes = ::std::ptr::null_mut();
        }
        pack
    }
    
    #[inline(always)]
    pub fn wrap(&mut self, pack: *mut Pack) -> &mut Cursor
    {
        self.0[0].wrap(pack);
        &mut self.0[0]
    }
}


impl Cursor {
    #[inline(always)]
    pub fn get_str(&mut self) -> &mut str { unsafe { std::str::from_utf8_unchecked_mut(std::slice::from_raw_parts_mut(self.base.bytes.offset(self.BYTE as isize), self.item_len as usize)) } }
    
    #[inline(always)]
    pub fn set_str(&mut self, src: &str) { unsafe { copy_nonoverlapping(src.as_ptr(), self.base.bytes.offset(self.BYTE as isize), self.item_len as usize); } }
    
    #[inline(always)]
    pub fn set_field(&mut self, fbit: usize, item_len: i32) -> bool {
        unsafe { crate::set_field(self, fbit, item_len) }
    }
    
    #[inline(always)]
    pub fn set_item(&mut self, item: usize, len: i32) -> bool {
        unsafe { crate::set_item(self, item, len) }
    }
    
    
    pub fn reset(&mut self) -> bool { unsafe { reset_cursor(self) } }
    
    pub fn wrap(&mut self, pack: *mut Pack)
    {
        self.base.pack = pack;
        if !pack.is_null()
        {
            self.base.meta = unsafe { (*pack).meta };
            self.base.bytes = unsafe { (*pack).bytes.as_mut_ptr() };
            self.reset();
        } else {
            self.base.meta = ::std::ptr::null_mut();
            self.base.bytes = ::std::ptr::null_mut();
        }
    }
    
    
    pub fn eq(&mut self, other: &mut Self) -> bool {
        unsafe {
            (*self.base.pack).meta == (*other.base.pack).meta && {
                let len = self.base.pack_size();
                other.base.pack_size() == len && (*self.base.pack).bytes.as_slice(len) == (*other.base.pack).bytes.as_slice(len)
            }
        }
    }
}


impl Transmitter {
    pub const fn new(pull: unsafe extern "C" fn(dst: *mut Transmitter) -> *const Pack) -> Transmitter
    {
        Transmitter {
            time: 0,
            flow: unsafe { transmute([0u8; ::std::mem::size_of::<Flow>()]) },
            pull: Some(pull),
            event:None,
        }
    }
}

impl Receiver {
    pub const fn new(dispatch: unsafe extern "C" fn(dst: *mut Receiver, id: usize, pack: *mut Pack) -> *const Meta) -> Receiver
    {
        Receiver {
            time: 0,
            flow: unsafe { transmute([0u8; ::std::mem::size_of::<Flow>()]) },
            event:None,
            dispatch: Some(dispatch),
            bits: 0,
        }
    }
}


unsafe impl std::marker::Sync for Field {}

unsafe impl std::marker::Sync for Meta {}

#[macro_export]
macro_rules! self_by_field_ptr {
($field_ptr:ident, $field_name:ident) => ({
        {
        let &Self { $field_name: ref fld, .. } = &*(0_usize as *const Self);
        std::mem::transmute($field_ptr as usize - fld as *const _ as usize)
        }
    });
}

