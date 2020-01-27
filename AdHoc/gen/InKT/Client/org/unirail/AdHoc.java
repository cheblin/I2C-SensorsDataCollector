
// AdHoc protocol - data interchange format
// Copyright 2019 Chikirev Sirguy, Unirail Group. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
package org.unirail;

import java.nio.charset.StandardCharsets;

import static java.lang.Character.MAX_SURROGATE;
import static java.lang.Character.MIN_SURROGATE;

public abstract class AdHoc {
	
	final static         char[] Ol = {0, 1, 3, 7, 15, 31, 63, 127, 255};
	private final static char[] lO = {0, 128, 192, 224, 240, 248, 252, 254, 255};
	
	public static long get_bytes( byte[] src, int BYTE, int bytes ) {
		int hi = 0, lo = 0;
		switch (bytes)
		{
			case 8:
				hi |= (src[BYTE + 7] & 0xFF) << 24;
			case 7:
				hi |= (src[BYTE + 6] & 0xFF) << 16;
			case 6:
				hi |= (src[BYTE + 5] & 0xFF) << 8;
			case 5:
				hi |= (src[BYTE + 4] & 0xFF);
			case 4:
				lo |= (src[BYTE + 3] & 0xFF) << 24;
			case 3:
				lo |= (src[BYTE + 2] & 0xFF) << 16;
			case 2:
				lo |= (src[BYTE + 1] & 0xFF) << 8;
			case 1:
				lo |= (src[BYTE] & 0xFF);
		}
		return ((hi & 0xFFFFFFFFL) << 32) | (lo & 0xFFFFFFFFL);
	}
	
	public static int set_bytes( long src, int bytes, byte[] dst, int BYTE ) {
		int hi = (int) (src >>> 32), lo = (int) (src & 0xFFFFFFFFL);
		switch (bytes)
		{
			case 8:
				dst[BYTE + 7] = (byte) (hi >>> 24);
			case 7:
				dst[BYTE + 6] = (byte) (hi >>> 16);
			case 6:
				dst[BYTE + 5] = (byte) (hi >>> 8);
			case 5:
				dst[BYTE + 4] = (byte) (hi & 0xFF);
			case 4:
				dst[BYTE + 3] = (byte) (lo >>> 24);
			case 3:
				dst[BYTE + 2] = (byte) (lo >>> 16);
			case 2:
				dst[BYTE + 1] = (byte) (lo >>> 8);
			case 1:
				dst[BYTE] = (byte) (lo & 0xFF);
		}
		return BYTE + bytes;
	}
	
	public static void set_0( byte[] dst, int bit, int bits ) {
		int dst_byte = bit >> 3;
		bit &= 7;
		if (8 < bit + bits)
		{
			if (0 < bit) 
			{
				dst[dst_byte] &= Ol[bit];
				if ((bits -= 8 - bit) == 0) return;
				dst_byte++;
			}
			if (0 < (bits & 7)) dst[dst_byte + (bits >> 3)] &= lO[8 - (bits & 7)]; 
			if ((bits >>= 3) == 0) return; 
			for (int i = dst_byte + bits; dst_byte <= --i; )   
			     dst[i] = 0;
		}
		else dst[dst_byte] &= Ol[bit] | lO[8 - (bit + bits)];
	}
	
	public static void set_bits( long src, int bits, byte[] dst, int bit ) {
		int dst_byte = bit >> 3;
		bit &= 7;
		if (8 < bit + bits)
		{
			if (0 < bit)  
			{
				dst[dst_byte] = (byte) (dst[dst_byte] & Ol[bit] |   
				                        (src & Ol[
						                               8 - bit 
						                               ]) << bit);
				dst_byte++;
				src >>>= 8 - bit;
				bits -= 8 - bit;
			}
			for (int BYTE = 0, bytes = bits >> 3; BYTE < bytes; BYTE++, src >>>= 8)  
			     dst[dst_byte++] = (byte) (src & 0xFF);
			if (0 < (bits &= 7))   
				dst[dst_byte] = (byte) (dst[dst_byte] & lO[8 - bits]  
				                        |
				                        src & Ol[bits]);  
		}
		else dst[dst_byte] = (byte) (dst[dst_byte] & (Ol[bit] | lO[8 - bit - bits]) | (src & Ol[bits]) << bit);
	}
	
	
	public static void copy_bits( byte[] src, int src_bit, int bits, byte[] dst, int dst_bit ) {
		if (bits < 1 || src == dst && src_bit == dst_bit) return;
		int count = bits >> 6;
		bits &= 0x3F; 
		if (src == dst && src_bit < dst_bit)  
		{
			
			src_bit += count * 64;
			dst_bit += count * 64;
			if (0 < bits) set_bits( get_bits( src, src_bit, bits ), bits, dst, dst_bit );
			for (; 0 < count--; src_bit -= 64, dst_bit -= 64, set_bits( get_bits( src, src_bit, 64 ), 64, dst, dst_bit )) ;
		}
		else
		{
			for (; 0 < count; set_bits( get_bits( src, src_bit, 64 ), 64, dst, dst_bit ), src_bit += 64, dst_bit += 64, count--) ;
			if (0 < bits) set_bits( get_bits( src, src_bit, bits ), bits, dst, dst_bit );
		}
	}
	
	public static int first_1( byte[] bytes, int bit, int bits, boolean existence ) {
		if (bits < 1) return -1;
		int _1BYTE = bit >> 3;
		int v      = bytes[_1BYTE] & 0xFF;
		bit &= 7;
		if (bits == 1) return (v & 1 << bit) == 0 ? -1 : 0;
		int add = 0;
s:
		{
			if (0 < bit)  
			{
				if (0 < (v >>= bit))
				{
					if (bit + bits < 8 && (v & Ol[bits]) == 0) return -1;   
					break s; 
				}
				if (bit + bits < 8) return -1;
				bits -= add = 8 - bit;
				_1BYTE++;
			}
			else if (bits < 9)
				if (v == 0 || (v & Ol[bits]) == 0) return -1;
				else break s;
			final int last = _1BYTE + (bits >> 3);
			for (int BYTE = _1BYTE; BYTE < last; BYTE++)
				if (0 < (v = bytes[BYTE] & 0xFF))
				{
					add += BYTE - _1BYTE << 3;
					break s;
				}
			if ((bits &= 7) == 0 || (v = (bytes[last] & 0xFF) & Ol[bits]) == 0) return -1;
			add += last - _1BYTE << 3;
		}
		if (existence) return Integer.MAX_VALUE;
		for (int i = 0; ; i++)
			if ((v >> i & 1) == 1) return add + i;
	}
	
	
	public static int bits2bytes( int bits ) { return bits < 1 ? 0 : 1 + (bits - 1 >> 3); }
	
	public static long get_bits( byte[] src, int bit, int bits ) {
		int src_byte = bit >> 3;
		bit &= 7;
		if (64 < bits) bits = 64;
		else if (bit + bits < 9) return (src[src_byte] & 0xFF) >> bit & Ol[bits];
		long dst = 0;
		for (int i = 0, last = bit + bits >> 3 << 3; i < last; i += 8)   
		     dst |= (src[src_byte++] & 0xFFL) << i;
		dst >>>= bit; 
		bit = bit + bits & 7; 
		if (0 < bit) dst |= (long) (src[src_byte] & Ol[bit]) << (bits - bit);
		return dst;
	}
	
	
	public static final class Pack {
		public final Meta   meta; 
		public       byte[] bytes;
		
		public Pack( Meta meta ) { this.meta = meta; }
		
		public static class Bytes {
			Meta meta;
			
			public void init( Meta meta ) {
				this.meta = meta;
				bytes     = new byte[meta.packMinBytes];
			}
			
			public byte[] bytes;
			public int    origin;
			public int    shift;
			
			
			public Pack unwrap() {
				if (meta == null) return null;
				
				Pack ret = new Pack( meta );
				ret.bytes = bytes;
				
				meta  = null;
				bytes = null;
				
				return ret;
			}
			public void wrap( Pack src ) {
				origin    = 0;
				bytes     = src.bytes;
				meta      = src.meta;
				src.bytes = null;
			}
			
			
			public final boolean equal( Bytes bytes, int len ) {
				if (len == 0) return false;
				for (; 0 < --len; ) if (this.bytes[origin + len] != bytes.bytes[bytes.origin + len]) return false;
				return true;
			}
		}
		
		public static class Meta {
			public final int id;
			
			private final int _2;
			private final int _4;
			private final int _8;
			
			public final int packMinBytes; 
			private final char    BITS_lenINbytes_bits;
			private final int     field_0_bit;
			public final  Field[] fields;
			
			
			public Meta( int id )                                                            { this( id, 0, 0, 0, 0, 0, 0, 0 ); }
			
			public Meta( int id, int _2, int _4, int _8, int packMinBytes, int field_0_bit ) { this( id, _2, _4, _8, packMinBytes, field_0_bit, 0, 0 ); }
			
			public Meta( int id, int _2, int _4, int _8, int packMinBytes, int field_0_bit, int BITS_lenINbytes_bits, int fields ) {
				this.id                   = id;
				this._2                   = _2;
				this._4                   = _4;
				this._8                   = _8;
				this.BITS_lenINbytes_bits = (char) BITS_lenINbytes_bits;
				this.field_0_bit          = field_0_bit;
				this.fields               = 0 < fields ? new Field[fields] : null;
				this.packMinBytes         = packMinBytes;
			}
			
			
			public static class Field {
				private final char    type;
				private final boolean varint; 
				private final int     length;
				private final byte    size; 
				public final  Meta    datatype; 
				
				public final  int[] dims; 
				
				
				private final int   const_dims_total; 
				
				
				private final char field_info_bits; 
				private final char var_dims_count; 
				private final char sparse_bits;
				
				public Field( int type, boolean varint, int length, int size, int const_dims_total, int field_info_bits, int sparse_bits, int var_dims_count, Meta datatype, int... dims ) {
					this.type             = (char) type;
					this.varint           = varint;
					this.length           = length;
					this.size             = (byte) size;
					this.const_dims_total = const_dims_total;
					this.field_info_bits  = (char) field_info_bits;
					this.var_dims_count   = (char) var_dims_count;
					this.sparse_bits      = (char) sparse_bits;
					this.datatype         = datatype;
					this.dims             = dims;
				}
				
				
				static class CursorBase extends Bytes {
					
					@Override public void init( Meta meta ) {
						super.init( meta );
						reset();
					}
					@Override public void wrap( Pack src ) {
						super.wrap( src );
						reset();
					}
					
					int subitem_len = 0;
					
					
					int BIT_S = -1, BIT_E;
					
					int BYTE_S = -1, BYTE_E = -1; 
					
					int LAST_BYTE;
					int LAST_field_bit;
					int LAST_BIT;
					
					
					int field_0_bit() {return meta.field_0_bit + origin * 8;}
					
					public int field_bit;
					
					int first_field_bit()   {return first_1( bytes, origin * 8 + meta.field_0_bit, BIT_E - (origin * 8 + meta.field_0_bit), false );}
					
					int next_field_bit()    {return field_bit - meta.field_0_bit < meta.fields.length - 1 ? first_1( bytes, origin * 8 + field_bit + 1, BIT_E - (origin * 8 + field_bit + 1), false ) : -1;}
					
					public Field getField() { return meta.fields[field_bit - meta.field_0_bit]; }
					
					boolean reset() {
						field_bit   = -1;
						BYTE_E      = BYTE_S = origin + meta.packMinBytes + (meta.BITS_lenINbytes_bits == 0 ? 0 : (int) get_bits( bytes, field_0_bit() - meta.BITS_lenINbytes_bits, meta.BITS_lenINbytes_bits ));
						BIT_E       = BIT_S = BYTE_E << 3;
						subitem_len = 0;
						if (meta.fields == null) 
							LAST_BIT = (LAST_BYTE = BYTE_S) * 8;
						else if (first_field_bit() < 0) 
						{
							LAST_BYTE = BYTE_E;
							LAST_BIT  = BIT_E;
						}
						else LAST_BYTE = LAST_BIT = -1; 
						LAST_field_bit = -1;
						return true;
					}
					
					int setLASTS() {
						int delta = field_bit < 0 ? first_field_bit() : next_field_bit();
						if (delta < 0)
						{
							LAST_BYTE      = BYTE_E;
							LAST_field_bit = field_bit;
							LAST_BIT       = BIT_E;
							return LAST_BYTE;
						}
						
						final int
								BIT_E_FX = BIT_E,
								BIT_S_FX = BIT_S,
								BYTE_S_FX = BYTE_S,
								BYTE_E_FX = BYTE_E,
								item_size_FX = subitem_len,
								field_bit_FX = field_bit;
						if (field_bit < 0) field_bit = meta.field_0_bit - 1; 
						do     
						{
							field_bit += delta + 1;
							BIT_S  = BIT_E;
							BYTE_S = BYTE_E;
							final Field fld = getField();
							if (0 < fld.length)
								if (0 < fld.size)
									BYTE_E += fld.const_dims_total * fld.length * fld.size;    
								else
									BIT_E += fld.const_dims_total * fld.length * fld.size;
							else
								setBounds( fld );
						}
						while (-1 < (delta = next_field_bit()));
						
						LAST_BYTE      = BYTE_E;
						LAST_BIT       = BIT_E;
						LAST_field_bit = field_bit;
						BIT_E          = BIT_E_FX;
						BIT_S          = BIT_S_FX;
						BYTE_S         = BYTE_S_FX;
						BYTE_E         = BYTE_E_FX;
						subitem_len    = item_size_FX;
						field_bit      = field_bit_FX;
						return LAST_BYTE;
					}
					
					void setBounds( Field fld ) {
						subitem_len = fld.size;
						int bit   = BIT_E;
						int count = 1;
						switch (fld.type)
						{
							case 1: 
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BIT_E -= fld.field_info_bits;
								BYTE_E += fld.const_dims_total * count * -fld.length * fld.size;
								break;
							case 3: 
								BIT_E -= fld.field_info_bits;
								int arrays_length = (int) get_bits( bytes, BIT_E, -fld.length );
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) arrays_length *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BYTE_E += fld.const_dims_total * arrays_length * fld.size;
								break;
							case 5: 
								count = fld.const_dims_total;
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BIT_E -= fld.field_info_bits;
								int all_arrays_sum = 0;
								while (0 < count--) all_arrays_sum += get_bits( bytes, BIT_E -= -fld.length, -fld.length );
								BYTE_E += all_arrays_sum * fld.size;
								break;
							case 7: 
								subitem_len = 0;
								count = 1;
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BIT_E -= fld.field_info_bits + count * fld.const_dims_total * -fld.length * fld.size;
								break;
							case 9: 
								BIT_E -= fld.field_info_bits;
								arrays_length = (int) get_bits( bytes, BIT_E, -fld.length );
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) arrays_length *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BIT_E -= fld.const_dims_total * arrays_length * fld.size;
								break;
							case 11: 
								count = fld.const_dims_total;
								if (0 < fld.var_dims_count) for (int i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits( bytes, bit -= -fld.dims[i], -fld.dims[i] );
								BIT_E -= fld.field_info_bits;
								while (0 < count--)
								{
									int bits = (int) (get_bits( bytes, BIT_E -= -fld.length, -fld.length ) * fld.size);
									BIT_E -= bits;
								}
								break;
							default: 
								if ((count = (int) get_bits( bytes, BIT_E -= fld.field_info_bits, fld.sparse_bits )) == 0) return;
								bit = BIT_E; 
								switch (fld.type)
								{
									case 2: 
										BIT_E -= count;
										while (BIT_E < bit--) if ((bytes[bit >>> 3] & 1 << (bit & 7)) == 0) count--;
										BYTE_E += -fld.length * count * fld.size; 
										break;
									case 4: 
										BIT_E -= count;
										arrays_length = (int) get_bits( bytes, bit + 2 * fld.sparse_bits, -fld.length );
										while (BIT_E < bit--) if ((bytes[bit >>> 3] & 1 << (bit & 7)) == 0) count--;
										BYTE_E += count * arrays_length * fld.size;
										break;
									case 6: 
										all_arrays_sum = 0;
										while (0 < count--) if ((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) all_arrays_sum += get_bits( bytes, bit -= -fld.length, -fld.length );
										BIT_E = bit;
										BYTE_E += all_arrays_sum * fld.size;
										break;
									case 8: 
										
										for (final int bits = -fld.length * fld.size; 0 < count--; )
											if ((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) bit -= bits;
										BIT_E = bit;
										break;
									case 10: 
										
										for (final int bits = (int) get_bits( bytes, bit + 2 * fld.sparse_bits, -fld.length ) * fld.size; 0 < count--; )
											if ((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) bit -= bits;
										BIT_E = bit;
										break;
									case 12: 
										while (0 < count--) if ((bytes[--bit >> 3] & 1 << (bit & 7)) != 0)
										{
											int len = (int) get_bits( bytes, bit -= -fld.length, -fld.length );
											bit -= len * fld.size;
										}
										BIT_E = bit;
										break;
									default:
										assert false;
								}
						}
					}
				}
			}
		}
		public static class Cursor extends Meta.Field.CursorBase {
			public final int[] D;
			
			public int BIT = -1; 
			public int BYTE; 
			public int item_len; 
			
			int field_item_0;
			int field_item; 
			int field_items;
			int field_items_total;
			
			
			public Cursor( Cursor prev, int nested_max, int dims_max ) {
				D         = new int[dims_max];
				this.prev = prev;
				next      = 1 < nested_max ? new Cursor( this, nested_max - 1, dims_max ) : null;
			}
			
			public int length() { return (-1 < super.LAST_BYTE ? super.LAST_BYTE : setLASTS()) - super.origin; }
			
			final Cursor prev;
			final Cursor next;
			
			public Cursor next( int origin ) {
				next.bytes  = bytes;
				next.meta   = getField().datatype;
				next.origin = origin;
				next.reset();
				next.LAST_BYTE = origin + item_len;
				return next;
			}
			
			
			@Override boolean reset() {
				super.reset();
				BYTE         = BYTE_S;
				BIT          = BIT_S;
				item_len     = 0;
				field_item_0 = -1;
				field_item   = Integer.MAX_VALUE;
				field_items  = 0;
				return true;
			}
			
			void init( Meta.Field fld ) {
				switch (fld.type)
				{
					case 1: 
						item_len = -fld.length;
						BIT = BIT_E;
						break;
					case 3: 
						item_len = (int) get_bits( bytes, BIT = BIT_S - fld.field_info_bits, -fld.length );
						break;
					case 5: 
					case 11: 
						item_len = 0;
						BIT = BIT_S - fld.field_info_bits; 
						field_item_0 = 0;
						field_item = Integer.MAX_VALUE;
						field_items = field_items_total;
						break;
					case 7: 
						item_len = -fld.length;
						field_item_0 = 0;
						BIT = BIT_E; 
						field_item = (field_items = field_items_total) == 0 ? Integer.MAX_VALUE : 0;
						break;
					case 9: 
						item_len = (int) get_bits( bytes, BIT = BIT_S - fld.field_info_bits, -fld.length );
						field_item_0 = 0;
						BIT = BIT_E; 
						field_item = (field_items = field_items_total) == 0 ? Integer.MAX_VALUE : 0;
						break;
					default:
						BIT = BIT_S - fld.field_info_bits;
						switch (fld.type)
						{
							case 2: 
							case 6: 
							case 8: 
							case 12: 
								item_len = -fld.length;
								BIT = BIT_S - fld.field_info_bits; 
								break;
							case 4: 
							case 10: 
								item_len = (int) get_bits( bytes, (BIT = BIT_S - fld.field_info_bits) + 2 * fld.sparse_bits, -fld.length );
								break;
							default:
								assert false;
						}
						field_item = Integer.MAX_VALUE;
						field_items = (int) get_bits( bytes, BIT, fld.sparse_bits );
						field_item_0 = (int) get_bits( bytes, BIT + fld.sparse_bits, fld.sparse_bits );
				}
				BYTE = BYTE_S;
			}
			
			@Override public Pack unwrap() {
				Pack ret = super.unwrap();
				for (Cursor cur = this; (cur = cur.next) != null; cur.bytes = null, cur.meta = null) ;
				return ret;
			}
			
			@Override public void wrap( Pack src ) {
				origin = 0;
				bytes  = src.bytes;
				meta   = src.meta;
				reset();
			}
			
			
			public boolean set_field( int fbit, int each_item_size, int... var_dims ) {
				if (field_bit == fbit) return true;
				if (each_item_size < 0 &&  
				    (BIT_E <= 8 * origin + fbit || 
				     (bytes[origin + (fbit >> 3)] & 1 << (fbit & 7)) == 0)) return false;
				Meta.Field fld;
search:
				{
					int delta;
					if (field_bit == -1 || fbit < field_bit && reset())    
					{
						if ((delta = first_field_bit()) == -1 || fbit < meta.field_0_bit + delta) break search;  
						field_bit = meta.field_0_bit - 1;
					}
					else if ((delta = next_field_bit()) == -1 || fbit < field_bit + 1 + delta) break search; 
					do 
					{
						BIT_S  = BIT_E;
						BYTE_S = BYTE_E;
						fld    = meta.fields[(field_bit += 1 + delta) - meta.field_0_bit];
						if (0 < fld.length)
							if (0 < fld.size)
								BYTE_E += fld.const_dims_total * fld.length * fld.size;    
							else
								BIT_E += fld.const_dims_total * fld.length * fld.size;
						else setBounds( fld );
					}
					while (-1 < (delta = next_field_bit()) && field_bit + 1 + delta < fbit);
					if (field_bit == fbit ||  
					    each_item_size < 0) 
					{
						item_len    = fld.length;
						subitem_len = fld.size;
						BIT         = BIT_E;
						BYTE        = BYTE_S;
						if (fld.length < 1)   
						{
							field_items_total = fld.const_dims_total;
							if (0 < fld.var_dims_count)   
							{
								D[fld.var_dims_count] = 0;
								for (int i = 0, bit = BIT_S, dim; i < fld.dims.length; i++)
									if ((dim = fld.dims[i]) < 1) field_items_total *= D[i] = (int) get_bits( bytes, bit -= -dim, -dim );
									else D[i] = dim;
							}
							init( fld );
						}
						return field_bit == fbit;
					}
				}
				if (each_item_size < 0) return false; 
				
				fld = meta.fields[fbit - meta.field_0_bit];
				if (fld.type == 0)
				{
					if (0 < fld.size)
					{
						insert( fbit, 0, fld.const_dims_total * fld.length * fld.size );
						BYTE_E = (BYTE = BYTE_S) + fld.const_dims_total * (item_len = fld.length) * (subitem_len = fld.size);
					}
					else
					{
						insert( fbit, fld.const_dims_total * fld.length * -fld.size, 0 );
						BIT = BIT_E = BIT_S + fld.const_dims_total * (item_len = fld.length) * fld.size;
					}
					return true;
				}
				int total = fld.const_dims_total;
				if (0 < fld.var_dims_count)  
					for (int i = 0, var = 0; i < fld.dims.length; i++)
						if (fld.dims[i] < 0)
							total *= D[i] = var_dims[var++]; 
						else D[i] = fld.dims[i];
				switch (fld.type)
				{
					case 1: 
						insert( fbit, fld.field_info_bits, total * -fld.length * fld.size );
						break;
					case 2: 
					case 6: 
					case 8: 
					case 12: 
						insert( fbit, fld.field_info_bits, 0 );
						break;
					case 3: 
						if (fld.datatype != null) 
							each_item_size = Math.max( fld.datatype.packMinBytes, each_item_size );
						insert( fbit, fld.field_info_bits, total * each_item_size * fld.size );
						set_bits( each_item_size, -fld.length, bytes, BIT_S - fld.field_info_bits ); 
						break;
					case 4: 
						insert( fbit, fld.field_info_bits, 0 );
						set_bits( each_item_size, -fld.length, bytes, BIT_S - fld.field_info_bits + 2 * fld.sparse_bits ); 
						break;
					case 5: 
					case 11: 
						insert( fbit, fld.field_info_bits + total * -fld.length, 0 );
						break;
					case 7: 
						
						insert( fbit, fld.field_info_bits + total * -fld.length * fld.size, 0 );
						break;
					case 9: 
						
						insert( fbit, fld.field_info_bits + total * each_item_size * fld.size, 0 );
						set_bits( each_item_size, -fld.length, bytes, BIT_S - fld.field_info_bits ); 
						break;
					case 10: 
						insert( fbit, fld.field_info_bits, 0 );
						set_bits( each_item_size, -fld.length, bytes, BIT_S - fld.field_info_bits + 2 * fld.sparse_bits ); 
						break;
					default:
						assert false;
				}
				field_items_total = total;
				if (0 < fld.var_dims_count)  
					for (int i = 0, bit = BIT_S; i < fld.dims.length; i++)
						if (fld.dims[i] < 0) set_bits( D[i], -fld.dims[i], bytes, bit -= -fld.dims[i] );  
				setBounds( fld );
				init( fld );
				return true;
			}
			
			
			public boolean set_item( int item, int length ) {
				final Meta.Field fld = getField();
				if (field_item == item)
				{
					if (fld.type == 5  
					    && -1 < length && length != item_len)  
					{
						if (fld.datatype != null) 
							length = Math.max( fld.datatype.packMinBytes, length );
						
						resize_bytes( (length - item_len) * subitem_len );
						set_bits( item_len = length, -fld.length, bytes, BIT );
					}
					return true;
				}
				int       bit         = BIT;
				int       _field_item = field_item;
				final int len_bits    = -fld.length;
				switch (fld.type)
				{
					default:
						
						
						
						
						assert false;
						return false;
					case 2: 
					case 4: 
						if (field_items == 0 ||
						    item < field_item_0 ||
						    field_item_0 + field_items <= item ||
						    (bytes[(bit = BIT_S - fld.field_info_bits - 1 - (item - field_item_0)) >> 3] & 1 << (bit & 7)) == 0)
						{
							if (length < 0) return false; 
							if (item < field_item_0 || field_items == 0)
							{
								BIT  = BIT_S - fld.field_info_bits; 
								BYTE = BYTE_S;
								final int ins_items = field_items == 0 ? 1 : field_item_0 - item;
								insert( field_bit, ins_items, item_len * fld.size );
								BIT = BIT_S - fld.field_info_bits - 1; 
								set_bits( field_item_0 = item, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits + fld.sparse_bits );  
								set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
							}
							else if (item < field_item_0 + field_items)
							{
								
								
								if (item < field_item)
								{
									field_item = field_item_0;
									BIT        = BIT_S - fld.field_info_bits - 1;
									BYTE       = BYTE_S;
								}
								else 
								{
									BIT--;
									field_item++;
									BYTE += item_len * fld.size;
								}
								for (final int bytes = item_len * fld.size; field_item < item; field_item++, BIT--)
									if ((this.bytes[BIT >> 3] & 1 << (BIT & 7)) != 0) BYTE += bytes;    
								assert (this.bytes[BIT >>> 3] & 1 << (BIT & 7)) == 0;   
								insert( field_bit, 0, item_len * fld.size );
							}
							else 
							{
								final int ins_items = item + 1 - (field_item_0 + field_items);
								BIT  = BIT_E;
								BYTE = BYTE_E;
								insert( field_bit, ins_items, item_len * fld.size );
								
								BIT  = BIT_E;
								BYTE = BYTE_E - item_len * fld.size;
								set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
							}
							this.bytes[BIT >> 3] |= 1 << (BIT & 7);  
							break;
						}
						
						if (item < field_item)
						{
							field_item = field_item_0;
							BIT        = BIT_S - fld.field_info_bits - 1;
							BYTE       = BYTE_S;
						}
						else 
						{
							BIT--;
							field_item++;
							BYTE += item_len * fld.size;
						}
						for (final int bytes = item_len * fld.size; ; field_item++, BIT--)
							if ((this.bytes[BIT >> 3] & 1 << (BIT & 7)) != 0) if (field_item == item) break;
							else BYTE += bytes; 
						return true;
					case 5: 
						if (field_items_total - 1 < item || get_bits( bytes, BIT_S - fld.field_info_bits - (item + 1) * len_bits, len_bits ) == 0)  
						{
							if (length < 0) return false;  
							if (field_items_total - 1 < item) throw new RuntimeException( "No room for item=" + item + ". The field_items_total =" + field_items_total );
							if (item < field_item)
							{
								field_item = 0;
								BIT        = BIT_S - fld.field_info_bits;
								BYTE       = BYTE_S;
								item_len   = (int) get_bits( this.bytes, BIT -= len_bits, len_bits );
							}
							for (; field_item < item; BYTE += item_len * fld.size, item_len = (int) get_bits( this.bytes, BIT -= len_bits, len_bits ), field_item++) ;
							if (item_len != 0) throw new RuntimeException( "Already allocated" );
							insert( field_bit, 0, length * fld.size );
							set_bits( length, len_bits, this.bytes, BIT );
							item_len = length;
							break;
						}
						if (item < field_item)
						{
							field_item = 0;
							BIT        = BIT_S - fld.field_info_bits;
							item_len   = (int) get_bits( bytes, BIT -= len_bits, len_bits );
							BYTE       = BYTE_S;
						}
						for (; field_item < item; BYTE += item_len * fld.size, item_len = (int) get_bits( bytes, BIT -= len_bits, len_bits ), field_item++) ;  
						
						if (-1 < length && length != item_len)   
						{
							resize_bytes( (length - item_len) * subitem_len );
							set_bits( item_len = length, -fld.length, this.bytes, BIT );
						}
						return true;
					case 6: 
						if (field_item_0 <= item && item < field_item_0 + field_items)
						{
							int _BYTE = BYTE;
							if (item < _field_item)
							{
								_field_item = field_item_0;
								bit         = BIT_S - fld.field_info_bits - 1;
								_BYTE       = BYTE_S;
							}
							else 
							{
								bit--;
								_field_item++;
								_BYTE += item_len * fld.size;
							}
							
							for (; ; bit--, _field_item++)
								if ((bytes[bit >>> 3] & 1 << (bit & 7)) != 0)
								{
									bit -= len_bits;
									if (_field_item == item)    
									{
										field_item = _field_item;
										BYTE       = _BYTE;
										BIT        = bit;
										item_len   = (int) get_bits( bytes, bit, len_bits );
										return true;
									}
									_BYTE += get_bits( bytes, bit, len_bits ) * fld.size;
								}
								else if (_field_item == item) break;
						}
						if (length < 0) return false; 
						if (item < field_item_0 || field_items == 0)
						{
							BIT  = BIT_S - fld.field_info_bits;
							BYTE = BYTE_S;
							final int ins_items = field_items == 0 ? 1 : field_item_0 - item;
							insert( field_bit, ins_items + len_bits, length * fld.size );
							BIT = BIT_S - fld.field_info_bits - 1;
							set_bits( field_item_0 = item, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits + fld.sparse_bits );  
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						else if (item < field_item_0 + field_items)
						{
							if (item < field_item)
							{
								field_item = field_item_0;
								BIT        = BIT_S - fld.field_info_bits - 1;
								BYTE       = BYTE_S;
							}
							else
							{
								BIT--;
								field_item++;
								BYTE += item_len * fld.size;
							}
							
							for (; field_item < item; BIT--, field_item++)   
								if ((this.bytes[BIT >>> 3] & 1 << (BIT & 7)) != 0)    
								{
									BIT -= len_bits;
									item_len = (int) get_bits( this.bytes, BIT, len_bits );
									BYTE += item_len * fld.size;
								}
							assert (this.bytes[BIT >>> 3] & 1 << (BIT & 7)) == 0;   
							insert( field_bit, len_bits, length * fld.size );
						}
						else
						{
							final int ins_items = item - (field_item_0 + field_items) + 1;
							BIT  = BIT_E;
							BYTE = BYTE_E;
							insert( field_bit, ins_items + len_bits, length * fld.size );
							BIT  = BIT_E + len_bits;
							BYTE = BYTE_E - length * fld.size;
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						this.bytes[BIT >>> 3] |= 1 << (BIT & 7);  
						set_bits( item_len = length, len_bits, this.bytes, BIT -= len_bits ); 
						break;
					case 8: 
					case 10: 
						if (field_item_0 <= item && item < field_item_0 + field_items)
						{
							if (item < _field_item)   
							{
								_field_item = field_item_0;
								bit         = BIT_S - fld.field_info_bits - 1;
							}
							else 
							{
								bit--;
								_field_item++;
							}
							
							for (int bits = item_len * fld.size; ; bit--, _field_item++)   
								if ((bytes[bit >>> 3] & 1 << (bit & 7)) != 0)
								{
									bit -= bits;
									if (_field_item == item)    
									{
										field_item = item;
										BIT        = bit;
										return true;
									}
								}
								else if (_field_item == item) break;
						}
						if (length < 0) return false; 
						int bits = item_len * fld.size;
						if (field_item == item) throw new RuntimeException();
						if (item < field_item_0 || field_items == 0)
						{
							BIT  = BIT_S - fld.field_info_bits;
							BYTE = BYTE_S;
							int ins_items = field_items == 0 ? 1 : field_item_0 - item;
							insert( field_bit, ins_items + bits, 0 );
							BIT = BIT_S - fld.field_info_bits - 1;
							set_bits( field_item_0 = item, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits + fld.sparse_bits );  
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						else if (item < field_item_0 + field_items)
						{
							if (item < field_item)
							{
								field_item = field_item_0;
								BIT        = BIT_S - fld.field_info_bits - 1;
							}
							else
							{
								BIT--;
								field_item++;
							}
							
							
							for (; field_item < item; BIT--, field_item++)   
								if ((this.bytes[BIT >>> 3] & 1 << (BIT & 7)) != 0) BIT -= bits;   
							assert (this.bytes[BIT >>> 3] & 1 << (BIT & 7)) == 0;   
							insert( field_bit, bits, 0 );
						}
						else
						{
							final int ins_items = item + 1 - (field_item_0 + field_items);
							BIT = BIT_E;
							insert( field_bit, ins_items + bits, 0 );
							BIT = BIT_E + bits; 
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						this.bytes[BIT >>> 3] |= 1 << (BIT & 7);  
						BIT -= bits;
						break;
					case 11: 
						if (item < field_items_total)  
						{
							if (item < _field_item)
							{
								_field_item = -1;
								bit         = BIT_S - fld.field_info_bits;
							}
							else if (item == field_item) return true;
							int _items;
							do
							{
								bits = fld.size * (_items = (int) get_bits( bytes, bit -= len_bits, len_bits ));
								bit -= bits;
								_field_item++;
							}
							while (_field_item < item);
							if (0 < _items)
							{
								BIT        = bit;
								item_len   = _items;
								field_item = _field_item;
								return true;
							}
						}
						if (length < 0) return false; 
						if (field_items_total - 1 < item) throw new RuntimeException( "No room for item=" + item + ". The field_items_total =" + field_items_total );
						if (item < field_item)
						{
							field_item = -1;
							BIT        = BIT_S - fld.field_info_bits;
						}
						while (field_item < item)
						{
							bits = (int) (fld.size * get_bits( this.bytes, BIT -= len_bits, len_bits ));
							BIT -= bits;
							field_item++;
						}
						if (get_bits( this.bytes, BIT, len_bits ) != 0) throw new RuntimeException( "Already allocated" );
						insert( field_bit, length * fld.size, 0 );
						set_bits( length, len_bits, this.bytes, BIT );
						BIT -= length * fld.size; 
						item_len = length;
						break;
					case 12: 
						if (field_item_0 <= item && item < field_item_0 + field_items)
						{
							if (item < _field_item)
							{
								_field_item = field_item_0;
								bit         = BIT_S - fld.field_info_bits - 1;
							}
							else 
							{
								bit--;
								_field_item++;
							}
							
							for (; ; bit--, _field_item++)
								if ((bytes[bit >>> 3] & 1 << (bit & 7)) != 0)
								{
									bit -= len_bits;
									if (_field_item == item)    
									{
										field_item = _field_item;
										item_len   = (int) get_bits( bytes, bit, len_bits );
										BIT        = bit - item_len * fld.size;
										return true;
									}
									bit -= get_bits( bytes, bit, len_bits ) * fld.size;
								}
								else if (_field_item == item) break;
						}
						if (length < 0) return false; 
						if (item < field_item_0 || field_items == 0)
						{
							BIT = BIT_S - fld.field_info_bits;
							final int ins_items = field_items == 0 ? 1 : field_item_0 - item;
							insert( field_bit, ins_items + len_bits + length * fld.size, 0 );
							BIT = BIT_S - fld.field_info_bits - 1;
							set_bits( field_item_0 = item, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits + fld.sparse_bits );  
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						else if (item < field_item_0 + field_items)
						{
							if (item < field_item)
							{
								field_item = field_item_0;
								BIT        = BIT_S - fld.field_info_bits - 1;
							}
							else
							{
								BIT--;
								field_item++;
							}
							
							for (; field_item < item; BIT--, field_item++)   
								if ((this.bytes[BIT >>> 3] & 1 << (BIT & 7)) != 0)    
								{
									BIT -= len_bits;
									item_len = (int) get_bits( this.bytes, BIT, len_bits );
									BIT -= item_len * fld.size;
								}
							assert (this.bytes[BIT >>> 3] & 1 << (BIT & 7)) == 0;   
							insert( field_bit, len_bits + length * fld.size, 0 );
						}
						else
						{
							final int ins_items = item - (field_item_0 + field_items) + 1;
							BIT = BIT_E;
							insert( field_bit, ins_items + len_bits + length * fld.size, 0 );
							BIT = BIT_E + length * fld.size + len_bits;
							set_bits( field_items += ins_items, fld.sparse_bits, this.bytes, BIT_S - fld.field_info_bits );  
						}
						this.bytes[BIT >>> 3] |= 1 << (BIT & 7);  
						set_bits( item_len = length, len_bits, this.bytes, BIT -= len_bits ); 
						BIT -= length * fld.size; 
						break;
				}
				field_item = item;
				return false;
			}
			
			
			
			private void resize_bytes( final int diff ) {
				Cursor cur = this;
				while (cur.prev != null) cur = cur.prev;   
				if (cur.LAST_BYTE < 0) cur.setLASTS();
				byte[] new_bytes = new byte[cur.LAST_BYTE + diff];
				System.arraycopy( bytes, 0, new_bytes, 0, BYTE );
				if (diff < 0)
					System.arraycopy( bytes, BYTE - diff, new_bytes, BYTE, cur.LAST_BYTE - (BYTE - diff) );   
				else
					System.arraycopy( bytes, BYTE, new_bytes, BYTE + diff, cur.LAST_BYTE - BYTE );
				bytes = new_bytes;
				for (; ; )
				{
					if (-1 < cur.LAST_BYTE) cur.LAST_BYTE += diff;
					cur.BYTE_E += diff;
					cur.bytes = new_bytes;
					if (cur == this) return;
					if (-1 < cur.field_bit)  
					{
						int length = cur.getField().length;
						if (length < 0)
							set_bits( cur.item_len += diff, -length, bytes, cur.BIT );
					}
					cur = cur.next;
				}
			}
			
			public String decode() { return new String( bytes, BYTE, item_len, StandardCharsets.UTF_8 );}
			
			public char[] encode( String src, int index, char[] reuse ) {
				final int len  = src.length();
				int       size = len;
				
				if (reuse == null || reuse.length < len) reuse = src.toCharArray();
				else src.getChars( 0, len, reuse, 0 );
				
				for (int i = 0; ; i++)
					if (i == len)
					{
						if (index < 0)
							set_field( -index - 1, len );
						else
							set_item( index, len );
						
						for (i = 0; i < len; i++)
						     bytes[BYTE + i] = (byte) reuse[i];
						
						return reuse;
					}
					else if ('\u0080' <= reuse[i])
					{
						for (char ch; i < len; i++)
							if ((ch = reuse[i]) < 0x800) size += ((0x7f - ch) >>> 31);
							else
							{
								size += 2;
								if (MIN_SURROGATE <= ch && ch <= MAX_SURROGATE)
									if (Character.codePointAt( reuse, i ) == ch) size -= 3; 
									else i++;
							}
						break;
					}
				
				if (index < 0)
					set_field( -index - 1, size );
				else
					set_item( index, size );
				
				
				for (int i = 0, ii = BYTE; i < len; )
				{
					final char ch = reuse[i++];
					
					if (ch < 0x80) bytes[ii++] = (byte) ch;
					else if (ch < 0x800)
					{
						bytes[ii++] = (byte) (0xc0 | (ch >> 6));
						bytes[ii++] = (byte) (0x80 | (ch & 0x3f));
					}
					else if (Character.MIN_SURROGATE <= ch && ch <= (Character.MAX_SURROGATE))
					{
						final int uc = Character.codePointAt( reuse, i );
						if (uc == ch) bytes[ii++] = (byte) '?';
						else
						{
							bytes[ii++] = (byte) (0xf0 | ((uc >> 18)));
							bytes[ii++] = (byte) (0x80 | ((uc >> 12) & 0x3f));
							bytes[ii++] = (byte) (0x80 | ((uc >> 6) & 0x3f));
							bytes[ii++] = (byte) (0x80 | (uc & 0x3f));
							i++;  
						}
					}
					else
					{
						bytes[ii++] = (byte) (0xe0 | ((ch >> 12)));
						bytes[ii++] = (byte) (0x80 | ((ch >> 6) & 0x3f));
						bytes[ii++] = (byte) (0x80 | (ch & 0x3f));
					}
				}
				return reuse;
			}
			void insert( final int fbit, final int bits, final int bytes ) {
				if (field_bit != fbit)    
				{
					BIT      = BIT_S = BIT_E; 
					BYTE     = BYTE_S = BYTE_E; 
					item_len = 0;
				}
				if (LAST_BYTE < 0) setLASTS();
				final int old_pack_data_bytes = LAST_BYTE;
				int       add_to_bits_bytes; 
				if (BIT == LAST_BIT && BYTE == old_pack_data_bytes)     
				{
					
					add_to_bits_bytes = meta.BITS_lenINbytes_bits == 0 ? 0 : bits2bytes( fbit + bits - (LAST_BIT - origin * 8) + 1 );
					if (0 < add_to_bits_bytes || 0 < bytes)     
					{
						resize_bytes( add_to_bits_bytes + bytes );
						if (0 < add_to_bits_bytes)
						{
							
							copy_bits( this.bytes, BIT,
									
									(old_pack_data_bytes << 3) - BIT,
									this.bytes, BIT + (add_to_bits_bytes << 3) );
							set_0( this.bytes, BIT, add_to_bits_bytes << 3 );
							
						}
					}
					
				}
				else 
				{
					
					
					
					final int add_bits_bits = meta.BITS_lenINbytes_bits == 0 ? 0 : bits - (LAST_BIT - (LAST_field_bit + 1)); 
					add_to_bits_bytes = meta.BITS_lenINbytes_bits == 0 ? 0 : bits2bytes( add_bits_bits ); 
					if (0 < add_to_bits_bytes || 0 < bytes)     
					{
						resize_bytes( add_to_bits_bytes + bytes );
						if (0 < bits)                                                       
						{
							
							copy_bits( this.bytes, BIT,
									(BYTE << 3) - BIT,
									this.bytes, BIT + (add_to_bits_bytes << 3) );
							
							
							copy_bits( this.bytes, LAST_BIT, 
									BIT - LAST_BIT,
									this.bytes, LAST_BIT + (add_to_bits_bytes << 3) - bits );
							
							
							if (0 < (add_to_bits_bytes << 3) - bits)                                
								set_0( this.bytes, LAST_BIT, (add_to_bits_bytes << 3) - bits ); 
							set_0( this.bytes, BIT + (add_to_bits_bytes << 3) - bits, bits );
							
						}
					}
					else 
					{
						copy_bits( this.bytes, LAST_BIT,
								
								BIT - LAST_BIT,
								this.bytes, LAST_BIT - bits );
						set_0( this.bytes, BIT - bits, bits ); 
					}
				}
				if (0 < meta.BITS_lenINbytes_bits && 0 < add_to_bits_bytes)    
				{
					int old_value = (int) get_bits( this.bytes, field_0_bit() - meta.BITS_lenINbytes_bits, meta.BITS_lenINbytes_bits );
					set_bits( old_value + add_to_bits_bytes, meta.BITS_lenINbytes_bits, this.bytes, field_0_bit() - meta.BITS_lenINbytes_bits );
				}
				final int zazor_delta = (add_to_bits_bytes << 3) - bits; 
				LAST_BIT += zazor_delta;
				BIT += add_to_bits_bytes << 3;
				BYTE += add_to_bits_bytes;
				if (fbit == field_bit)    
				{
					BIT_E += zazor_delta;
					BIT_S += add_to_bits_bytes << 3;
					BYTE_S += add_to_bits_bytes;
				}
				else 
				{
					BIT_S  = BIT_E = BIT;
					BYTE_E = BYTE_S = BYTE;
					this.bytes[origin + (fbit >> 3)] |= 1 << (fbit & 7);  
					if (LAST_field_bit < fbit) LAST_field_bit = fbit;
					field_bit = fbit;
				}
			}
		}
	}
	
	
	public abstract static class Channel {
		private static class Flow {
			
			final Pack.Meta.Field.CursorBase cursors[];
			char level;              
			long Uvalue;
			char crc;
			
			protected STATE state = STATE.STANDBY;
			
			public Flow( int nested_max ) {
				cursors = new Pack.Meta.Field.CursorBase[nested_max]; 
				for (int i = 0; i < nested_max; i++) cursors[i] = new Pack.Meta.Field.CursorBase();
			}
			
			public enum STATE {
				STANDBY, PACK_ID, VARINT, VARINT_BR, BYTES, BYTES_BR
			}
			
			MODE mode = MODE.NONE;
			
			enum MODE {
				OPTS_INFO, SET, NONE, CRC
			}
			
			Pack.Meta.Field.CursorBase next() {
				int                        delta = -1;
				Pack.Meta.Field.CursorBase cur   = cursors[level];
				for (boolean to_end = false; ; )
				{
start:
					{
						
						if (cur.field_bit == -1)  
						{
							
							switch (cur.subitem_len)
							{
								case 0: 
									cur.BYTE_S = cur.BYTE_E = cur.origin; 
									if (0 < cur.meta._2)
									{
										cur.BYTE_E += cur.meta._2 * (cur.subitem_len = 2);
										state = STATE.VARINT;
										return cur;
									}
								case 2:
									if (0 < cur.meta._4)
									{
										cur.BYTE_E += cur.meta._4 * (cur.subitem_len = 4);
										state = STATE.VARINT;
										return cur;
									}
								case 4:
									if (0 < cur.meta._8)
									{
										cur.BYTE_E += cur.meta._8 * (cur.subitem_len = 8);
										state = STATE.VARINT;
										return cur;
									}
								case 8:
									if (cur.BYTE_S < (cur.BYTE_E = cur.origin + cur.meta.packMinBytes))  
									{
										state           = STATE.BYTES;
										cur.subitem_len = 1;
										return cur;
									}
									mode = MODE.NONE;
								default:
									
									if (cur.meta.fields == null)
									{
										to_end = true;
										break;
									}
									final int fix = cur.BYTE_S; 
									cur.reset();
									if (mode == MODE.NONE && fix < cur.BYTE_E)  
									{
										cur.BYTE_S      = fix;
										state           = STATE.BYTES;
										mode            = MODE.OPTS_INFO;
										cur.subitem_len = 1; 
										return cur;
									}
									mode = MODE.NONE;
									cur.BIT_E = cur.BIT_S = (cur.BYTE_S = cur.BYTE_E) << 3;
									if ((delta = cur.first_field_bit()) == -1) to_end = true;
									else
										cur.field_bit = cur.meta.field_0_bit - 1; 
							}
						}
						else if ((delta = cur.next_field_bit()) < 0) to_end = true;
						do
						{
next_field:
							{
								if (!to_end)
									do
										loop:
												{
													Pack.Meta.Field fld = cur.meta.fields[(cur.field_bit += delta + 1) - cur.meta.field_0_bit];
													state      = fld.varint ? STATE.VARINT : STATE.BYTES;
													cur.BIT_S  = cur.BIT_E;
													cur.BYTE_S = cur.BYTE_E;
													if (0 < fld.length)  
														if (0 < fld.size)
															cur.BYTE_E += fld.const_dims_total * fld.length * (cur.subitem_len = fld.size); 
														else
															cur.BIT_E += fld.const_dims_total * fld.length * fld.size; 
													else
														cur.setBounds( fld ); 
													
													if (fld.datatype != null)  
													{
														cur             = cursors[++level];
														cur.origin      = cursors[level - 1].BYTE_S;
														cur.bytes       = cursors[level - 1].bytes;
														cur.meta        = fld.datatype;
														cur.field_bit   = -1;    
														cur.subitem_len = 0;
														break start;
													}
													if (cur.BYTE_S == cur.BYTE_E)  
													{
														if ((delta = cur.next_field_bit()) == -1) break loop;
														continue;
													}
													return cur;
												}
									while (true);
								else
									to_end = false;
								while (0 < level)
								{
									if (cur.BYTE_E == cursors[level - 1].BYTE_E)  
									{
										cur = cursors[--level];
										
										if ((delta = cur.next_field_bit()) == -1) continue;
										break next_field;
									}
									
									cur.origin      = cur.BYTE_E; 
									cur.field_bit   = -1;                      
									cur.subitem_len = 0;
									break start;
								}
								
								return null;
							}
						}
						while (true);
					}
				}
			}
			
			
		}
		
		public abstract static class Transmitter extends java.io.InputStream {
			final Flow flow;
			final int  id_bytes;
			
			protected abstract Pack pullSendingPack();
			
			public Transmitter( int nested_max, int id_bytes ) {
				flow          = new Flow( nested_max );
				this.id_bytes = id_bytes;
			}
			
			protected void failure( String reason ) { System.out.println( "failure: " + reason ); }
			
			@Override public int read()             {final byte[] bytes = new byte[1]; return 0 < read( bytes, 0, 1 ) ? bytes[0] & 0xFF : -1; }
			
			@Override public int read( byte[] dst, int BYTE, int bytes ) {
				final int fix = BYTE;
				for (Pack.Meta.Field.CursorBase cur = flow.cursors[flow.level]; 0 < bytes--; BYTE++)
				{
					switch (flow.state)
					{
						case STANDBY:
							final Pack pack = pullSendingPack();
							if (pack == null) return BYTE - fix;    
							
							cur.wrap( pack );
							cur.field_bit = 8 * (id_bytes - 1);
							
							flow.state = Flow.STATE.PACK_ID;
							flow.Uvalue = pack.meta.id; 
						case PACK_ID:
							dst[BYTE] = (byte) (0xFF & flow.Uvalue >>> cur.field_bit);
							if (-1 < (cur.field_bit -= 8)) continue;
							
							cur.field_bit = -1;
							flow.mode = Flow.MODE.NONE;
							
							if (cur.meta.fields != null)   
							{
								flow.state = Flow.STATE.VARINT;
								cur.setLASTS();
								flow.Uvalue = cur.LAST_BYTE + 1 - (cur.origin + cur.meta.packMinBytes);
								cur.BIT_E   = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0; 
								continue;
							}
							flow.Uvalue = 0;
							cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
							
							break;
						case BYTES:
							dst[BYTE] = cur.bytes[cur.BYTE_S++];
							if (cur.BYTE_S < cur.BYTE_E) continue;
							break;
						case VARINT: 
							if ((flow.Uvalue & ~0x7FL) != 0)   
							{
								dst[BYTE] = (byte) (flow.Uvalue & 0x7F | 0x80);
								flow.Uvalue >>>= 7;
								continue;
							}
							dst[BYTE] = (byte) flow.Uvalue;
							if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E)   
							{
								flow.Uvalue = get_bytes( cur.bytes, cur.BYTE_S, cur.subitem_len );
								continue;
							}
					}
					if ((cur = flow.next()) != null)
					{
						if (flow.state == Flow.STATE.VARINT) flow.Uvalue = get_bytes( cur.bytes, cur.BYTE_S, cur.subitem_len );
						continue;
					}
					flow.state = Flow.STATE.STANDBY;
					flow.mode  = Flow.MODE.NONE;
					cur        = flow.cursors[flow.level = 0];
					for (Pack.Meta.Field.CursorBase c : flow.cursors)
					{
						c.meta  = null;
						c.bytes = null;
					}
				}
				return BYTE - fix;
			}
			
			public abstract static class Advanced extends Transmitter {
				public Advanced( int nested_max, int id_bytes ) { super( nested_max, id_bytes ); }
				
				@Override public int read( byte[] dst, int BYTE, int bytes ) {
					final int                  fix = BYTE;
					Pack.Meta.Field.CursorBase cur = flow.cursors[flow.level];
					for (int t; 0 < bytes--; t = flow.mode != Flow.MODE.CRC ? flow.crc = crc16( dst[BYTE], flow.crc ) : 0, BYTE++)
					{
						switch (flow.state)
						{
							case STANDBY:
								final Pack pack = pullSendingPack();
								if (pack == null) return BYTE - fix;    
								cur.wrap( pack );
								cur.field_bit = 8 * (id_bytes - 1);
								
								flow.state = Flow.STATE.PACK_ID;
								flow.Uvalue = pack.meta.id; 
								flow.crc = 0;
								dst[BYTE] = BR;
								continue; 
							case PACK_ID:
								dst[BYTE] = (byte) (0xFF & flow.Uvalue >>> cur.field_bit);
								if (-1 < (cur.field_bit -= 8)) continue;
								
								cur.field_bit = -1;
								flow.mode = Flow.MODE.NONE;
								
								if (cur.meta.fields != null)   
								{
									flow.state = Flow.STATE.VARINT;
									
									cur.setLASTS();
									flow.Uvalue = cur.LAST_BYTE + 1 - (cur.origin + cur.meta.packMinBytes);
									cur.BIT_E   = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0; 
									continue;
								}
								
								flow.Uvalue = 0;
								cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
								break;
							case BYTES:
								if (flow.mode == Flow.MODE.CRC)
								{
									switch (cur.subitem_len)    
									{
										case 4:
											cur.subitem_len = (dst[BYTE] = (byte) (flow.crc >>> 8)) == BR ? 3 : 2;
											continue;
										case 3:
											dst[BYTE] = BR;
											cur.subitem_len = 2;
											continue;
										case 2:
											if ((dst[BYTE] = (byte) (flow.crc & 0xFF)) != BR) break;
											cur.subitem_len = 1;
											continue;
										case 1:
											dst[BYTE] = BR;
									}
									flow.state = Flow.STATE.STANDBY;
									flow.mode  = Flow.MODE.NONE;
									cur        = flow.cursors[flow.level = 0];
									
									for (Pack.Meta.Field.CursorBase c : flow.cursors)
									{
										c.meta  = null;
										c.bytes = null;
									}
									continue;
								}
								if ((flow.Uvalue = dst[BYTE] = cur.bytes[cur.BYTE_S++]) == BR)
								{
									flow.state = Flow.STATE.BYTES_BR;
									continue; 
								}
							case BYTES_BR:
								flow.state = Flow.STATE.BYTES;
								dst[BYTE] = (byte) flow.Uvalue;
								if (cur.BYTE_S < cur.BYTE_E) continue;
								break;
							case VARINT: 
								if ((flow.Uvalue & ~0x7FL) != 0)   
								{
									dst[BYTE] = (byte) (flow.Uvalue & 0x7F | 0x80);
									flow.Uvalue >>>= 7;
									continue;
								}
								
								if (flow.Uvalue == BR)
								{
									flow.state = Flow.STATE.VARINT_BR;
									dst[BYTE]  = BR;
									continue;
								}
							case VARINT_BR:
								flow.state = Flow.STATE.VARINT;
								dst[BYTE] = (byte) flow.Uvalue;
								if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E)   
								{
									flow.Uvalue = get_bytes( cur.bytes, cur.BYTE_S, cur.subitem_len );
									continue;
								}
						}
						if ((cur = flow.next()) != null)
						{
							if (flow.state == Flow.STATE.VARINT) flow.Uvalue = get_bytes( cur.bytes, cur.BYTE_S, cur.subitem_len );
							continue;
						}
						cur             = flow.cursors[flow.level = 0];
						flow.state      = Flow.STATE.BYTES;
						flow.mode       = Flow.MODE.CRC;
						cur.subitem_len = 4; 
					}
					return BYTE - fix;
				}
			}
		}
		
		public abstract static class Receiver extends java.io.OutputStream {
			int bits = 0; 
			volatile int time;
			
			final Flow flow;
			final int  id_bytes;
			
			protected void failure( String reason ) { System.out.println( "failure: " + reason ); }
			
			public Receiver( int nested_max, int id_bytes ) {
				flow          = new Flow( nested_max );
				this.id_bytes = id_bytes;
			}
			
			protected abstract Pack.Meta dispatch( int id, Pack pack );
			
			@Override public void write( int b ) {
				byte[] bytes = new byte[1];
				bytes[0] = (byte) (b & 0xFF);
				write( bytes, 0, 1 );
			}
			
			@Override public void write( byte[] src, int BYTE, int bytes ) {
				time = ~0; 
				if (time == 0)   
				{
					time       = ~0; 
					flow.state = Flow.STATE.STANDBY;
					
					failure( "Receive timeout" );
				}
				for (Pack.Meta.Field.CursorBase cur = flow.cursors[flow.level]; 0 < bytes--; BYTE++)   
				{
					switch (flow.state)
					{
						case STANDBY:
							flow.Uvalue = 0;
							bits = 0;
							flow.state = Flow.STATE.PACK_ID;
						case PACK_ID:
							flow.Uvalue = flow.Uvalue << 8 | src[BYTE] & 0xFF;
							if (++bits < id_bytes) continue;
							flow.mode = Flow.MODE.NONE;
							final Pack.Meta meta = dispatch( (int) flow.Uvalue, null );
							if (meta == null)
							{
								failure( "Unrecognized package ID = " + flow.Uvalue );
								flow.state = Flow.STATE.STANDBY;
								continue;
							}
							
							cur.meta = meta; 
							flow.Uvalue = 0;
							bits = 0;
							cur.field_bit = -1;
							cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
							
							if (meta.fields != null)    
							{
								flow.state = Flow.STATE.VARINT;
								flow.mode  = Flow.MODE.OPTS_INFO;
								continue;
							}
							else 
							{
								cur.bytes = new byte[meta.packMinBytes];
								cur.reset();
							}
							
							
							break;
						case BYTES: 
							cur.bytes[cur.BYTE_S++] = src[BYTE]; 
							if (cur.BYTE_S < cur.BYTE_E) continue;
							break;
						case VARINT: 
							flow.Uvalue |= (src[BYTE] & 0x7FL) << bits;
							bits += 7;
							if ((src[BYTE] & 0x80) != 0) continue;
							bits = 0;
							
							if (flow.mode == Flow.MODE.OPTS_INFO)
							{
								cur.bytes   = new byte[cur.meta.packMinBytes + (int) flow.Uvalue]; 
								flow.Uvalue = 0;
								flow.mode   = Flow.MODE.NONE;
								break;
							}
							
							set_bytes( flow.Uvalue, cur.subitem_len, cur.bytes, cur.BYTE_S );
							flow.Uvalue = 0;
							if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) continue;
							break;
					}
					if ((cur = flow.next()) != null) continue;
					cur = flow.cursors[flow.level = 0];
					dispatch( cur.meta.id, cur.unwrap() );
					for (Pack.Meta.Field.CursorBase c : flow.cursors)
					{
						c.meta  = null;
						c.bytes = null;
					}
					flow.state = Flow.STATE.STANDBY;
					flow.mode  = Flow.MODE.NONE;
				}
			}
			
			public abstract static class Advanced extends Receiver {
				public Advanced( int nested_max, int id_bytes ) { super( nested_max, id_bytes ); }
				
				int bits = 0; 
				
				
				@Override public void write( byte[] src, int BYTE, int bytes ) {
					time = ~0; 
					if (time == 0)   
					{
						time       = ~0; 
						flow.state = Flow.STATE.STANDBY;
						failure( "Receive timeout" );
					}
					Pack.Meta.Field.CursorBase cur = flow.cursors[flow.level];
					for (int t; 0 < bytes--; t = flow.mode != Flow.MODE.CRC ? flow.crc = crc16( src[BYTE], flow.crc ) : 0, BYTE++)   
					{
						switch (flow.state)
						{
							case STANDBY:
								flow.crc = 0;
								flow.Uvalue = 0;
								bits = 0;
								if (src[BYTE] == BR) flow.state = Flow.STATE.PACK_ID;
								continue;
							case PACK_ID:
								if (src[BYTE] == BR)   
								{
									failure( " After BR expect helper ID but got +BR" );
									flow.state = Flow.STATE.STANDBY;
									continue;
								}
								flow.Uvalue = flow.Uvalue << 8 | src[BYTE] & 0xFF;
								if (++bits < id_bytes) continue;
								flow.mode = Flow.MODE.NONE;
								final Pack.Meta meta = dispatch( (int) flow.Uvalue, null );
								if (meta == null)
								{
									failure( "Unrecognized package ID = " + flow.Uvalue );
									flow.state = Flow.STATE.STANDBY;
									continue;
								}
								
								cur.meta = meta; 
								flow.Uvalue = 0;
								bits = 0;
								cur.field_bit = -1;
								cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
								
								if (meta.fields != null)    
								{
									flow.state = Flow.STATE.VARINT;
									flow.mode  = Flow.MODE.OPTS_INFO;
									continue;
								}
								else 
								{
									cur.bytes = new byte[meta.packMinBytes];
									cur.reset();
								}
								
								break;
							case BYTES: 
								if (src[BYTE] == BR)
								{
									flow.state = Flow.STATE.BYTES_BR;
									continue;
								}
							case BYTES_BR:
								if (flow.state == Flow.STATE.BYTES_BR)
								{
									if (src[BYTE] != BR)
									{
										failure( "waiting for second BR but got " + src[BYTE] );
										flow.state = Flow.STATE.STANDBY;
										continue;
									}
									flow.state = Flow.STATE.BYTES;
								}
								if (flow.mode == Flow.MODE.CRC)
									switch (cur.subitem_len)
									{
										case 2:
											flow.Uvalue = (src[BYTE] & 0xFF) << 8;
											cur.subitem_len = 1;
											continue;
										case 1:
											if ((flow.Uvalue | src[BYTE] & 0xFF) == flow.crc) dispatch( cur.meta.id, cur.unwrap() );   
											else failure( "CRC error" );
											for (Pack.Meta.Field.CursorBase c : flow.cursors)
											{
												c.meta  = null;
												c.bytes = null;
											}
											flow.state = Flow.STATE.STANDBY;
											flow.mode = Flow.MODE.NONE;
											continue;
									}
								cur.bytes[cur.BYTE_S++] = src[BYTE]; 
								if (cur.BYTE_S < cur.BYTE_E) continue;
								break;
							case VARINT: 
								if (src[BYTE] == BR)
								{
									flow.state = Flow.STATE.VARINT_BR;
									continue;
								}
							case VARINT_BR:
								if (flow.state == Flow.STATE.VARINT_BR)
								{
									if (src[BYTE] != BR)
									{
										failure( "waiting for second BR but got " + src[BYTE] );
										flow.state = Flow.STATE.STANDBY;
										continue;
									}
									flow.state = Flow.STATE.VARINT;
								}
								flow.Uvalue |= (src[BYTE] & 0x7FL) << bits;
								bits += 7;
								if ((src[BYTE] & 0x80) != 0) continue;
								bits = 0;
								
								if (flow.mode == Flow.MODE.OPTS_INFO)
								{
									cur.bytes   = new byte[cur.meta.packMinBytes + (int) flow.Uvalue]; 
									flow.Uvalue = 0;
									flow.mode   = Flow.MODE.NONE;
									break;
								}
								
								set_bytes( flow.Uvalue, cur.subitem_len, cur.bytes, cur.BYTE_S );
								flow.Uvalue = 0;
								if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) continue;
								break;
						}
						if ((cur = flow.next()) != null) continue;
						cur             = flow.cursors[flow.level = 0];
						flow.state      = Flow.STATE.BYTES;
						flow.mode       = Flow.MODE.CRC;
						cur.subitem_len = 2;
					}
				}
			}
		}
		
		private static final char tab[] = {0, 4129, 8258, 12387, 16516, 20645, 24774, 28903, 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935};
		
		
		private static char crc16( byte data, char crc ) {
			int UBYTE = data & 0xFF;
			crc = (char) (tab[(crc >> 12 ^ UBYTE >> 4) & 0x0F] ^ crc << 4);
			return (char) (tab[(crc >> 12 ^ UBYTE & 0x0F) & 0x0F] ^ crc << 4);
		}
		
		private static final byte BR = 0x55;
		
	}
	
	
	public static byte[] bytes( int... ints ) {
		byte[] bytes = new byte[ints.length];
		for (int i = ints.length - 1; -1 < i; i--) bytes[i] = (byte) ints[i];
		return bytes;
	}
	
	public static char[] chars( int... ints ) {
		char[] chars = new char[ints.length];
		for (int i = ints.length - 1; -1 < i; i--) chars[i] = (char) ints[i];
		return chars;
	}
	
	public static short[] shorts( int... ints ) {
		short[] shorts = new short[ints.length];
		for (int i = ints.length - 1; -1 < i; i--) shorts[i] = (short) ints[i];
		return shorts;
	}
}