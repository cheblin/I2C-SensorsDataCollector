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
'use strict';

export namespace org.unirail.AdHoc {
    const Ol = [0, 1, 3, 7, 15, 31, 63, 127, 255];
    const lO = [0, 128, 192, 224, 240, 248, 252, 254, 255];
    const MAX_INT = 2 ** 53 - 1;
    export const JS_HI = 0x1_0000_0000;

    export function get_bytes(src: DataView, byte: number, bytes: number): number {
        switch (bytes) {
            case 1:
                return src.getUint8(byte);
            case 2:
                return src.getUint16(byte, true);
            case 3:
                return (src.getUint8(byte) + src.getUint16(byte + 1, true)) << 8;
            case 4:
                return src.getUint32(byte, true);
            case 5:
                return src.getUint32(byte, true) + src.getUint8(byte + 4) * JS_HI;
            case 6:
                return src.getUint32(byte, true) + src.getUint16(byte + 4, true) * JS_HI;
            case 7:
                return src.getUint32(byte, true) + (src.getUint8(byte + 4) + src.getUint16(byte + 5, true)) * JS_HI;
            case 8:
                return src.getUint32(byte, true) + src.getUint32(byte + 4, true) * JS_HI;
        }
        return 0;
    }

    export function set_bytes(src: number, bytes: number, dst: DataView, byte: number) {
        switch (bytes) {
            case 1:
                dst.setUint8(byte, src);
                break;
            case 2:
                dst.setUint16(byte, src, true);
                break;
            case 3:
                dst.setUint8(byte, src);
                dst.setUint16(byte >>> 8, src, true);
                break;
            case 4:
                dst.setUint32(byte, src, true);
                break;
            case 5:
                dst.setUint32(byte, src % JS_HI, true);
                dst.setUint8(byte + 4, src / JS_HI);
            case 6:
                dst.setUint32(byte, src % JS_HI, true);
                dst.setUint16(byte + 4, src / JS_HI, true);
            case 7:
                dst.setUint32(byte, src % JS_HI, true);
                dst.setUint8(byte + 4, src / JS_HI);
                dst.setUint16(byte + 5, src / JS_HI, true);
            case 8:
                dst.setUint32(byte, src % JS_HI, true);
                dst.setUint32(byte + 4, src / JS_HI, true);
        }
    }

    export function ArrayEquals(a: ArrayBuffer, a_byte: number, b: ArrayBuffer, b_byte: number, bytes: number): number {
        if (bytes < 1 || Math.min(a.byteLength - a_byte, b.byteLength - b_byte, bytes) !== bytes) return -1;
        for (let i = 0, A = new Uint8Array(a, a_byte, bytes), B = new Uint8Array(b, b_byte, bytes); i < bytes; i++) if (A[i] !== B[i]) return i;
        return 0;
    }

    export function copy_bytes(src: ArrayBuffer, src_byte: number, dst: ArrayBuffer, dst_byte: number, bytes: number): ArrayBuffer {
        bytes = Math.min(src.byteLength - src_byte, dst.byteLength - dst_byte, bytes);

        if (0 < bytes) new Uint8Array(dst, dst_byte, bytes).set(new Uint8Array(src, src_byte, bytes));
        return dst;
    }

    export function set_0(dst: DataView, bit: number, bits: number) {
        let dst_byte = bit >> 3;
        bit &= 7;
        if (8 < bit + bits) {
            if (0 < bit) {
                dst.setUint8(dst_byte, dst.getUint8(dst_byte) & Ol[bit]);
                if ((bits -= 8 - bit) === 0) return;
                dst_byte++;
            }

            if (0 < (bits & 7)) dst.setUint8(dst_byte + (bits >> 3), dst.getUint8(dst_byte + (bits >> 3)) & lO[8 - (bits & 7)]);
            if ((bits >>= 3) === 0) return;
            for (let i = dst_byte + bits; dst_byte <= --i; ) dst.setUint8(i, 0);
        } else {
            dst.setUint8(dst_byte, dst.getUint8(dst_byte) & (Ol[bit] | lO[8 - (bit + bits)]));
        }
    }

    export function get_bits(src: DataView, bit: number, bits: number): number {
        if (32 < bits) throw new Error('bits hav to be lower then 32');

        let src_byte = bit >> 3;
        bit &= 7;

        if (bit + bits < 9) return (src.getUint8(src_byte) >> bit) & Ol[bits];
        let dst = 0;
        for (let i = 0, last = ((bit + bits) >> 3) << 3; i < last; i += 8) dst |= src.getUint8(src_byte++) << i;

        dst >>>= bit;
        bit = (bit + bits) & 7;
        if (0 < bit) dst |= (src.getUint8(src_byte) & Ol[bit]) << (bits - bit);
        return dst;
    }

    export function set_bits(src: number, bits: number, dst: DataView, bit: number) {
        if (32 < bits) throw new Error('bits hav to be lower then 32');
        let dst_byte = bit >> 3;
        bit &= 7;
        if (8 < bit + bits) {
            if (0 < bit) {
                dst.setUint8(dst_byte, (dst.getUint8(dst_byte) & Ol[bit]) | ((src & Ol[8 - bit]) << bit));
                dst_byte++;
                src >>>= 8 - bit;
                bits -= 8 - bit;
            }

            for (let BYTE = 0, bytes = bits >> 3; BYTE < bytes; BYTE++, src >>>= 8) dst.setUint8(dst_byte++, src);

            if (0 < (bits &= 7)) dst.setUint8(dst_byte, (dst.getUint8(dst_byte) & lO[8 - bits]) | (src & Ol[bits]));
        } else {
            dst.setUint8(dst_byte, (dst.getUint8(dst_byte) & (Ol[bit] | lO[8 - bit - bits])) | ((src & Ol[bits]) << bit));
        }
    }

    export function copy_bits(src: DataView, src_bit: number, bits: number, dst: DataView, dst_bit: number) {
        if (bits === 0 || (src === dst && src_bit === dst_bit)) return;
        let count = bits >>> 5;
        bits &= 0x1f;
        if (src === dst && src_bit < dst_bit) {
            src_bit += count * 32;
            dst_bit += count * 32;
            if (0 < bits) set_bits(get_bits(src, src_bit, bits), bits, dst, dst_bit);
            for (; 0 < count--; src_bit -= 32, dst_bit -= 32, set_bits(get_bits(src, src_bit, 32), 32, dst, dst_bit));
        } else {
            for (; 0 < count; set_bits(get_bits(src, src_bit, 32), 32, dst, dst_bit), src_bit += 32, dst_bit += 32, count--);
            if (0 < bits) set_bits(get_bits(src, src_bit, bits), bits, dst, dst_bit);
        }
    }

    export function first_1(bytes: DataView, bit: number, bits: number, existence: boolean): number {
        if (bits < 1) return -1;
        let _1BYTE = bit >> 3;
        let v = bytes.getUint8(_1BYTE);
        bit &= 7;
        if (bits === 1) return (v & (1 << bit)) === 0 ? -1 : 0;
        let add = 0;
        sBreak: {
            if (0 < bit) {
                if (0 < (v >>= bit)) {
                    if (bit + bits < 8 && (v & Ol[bits]) === 0) return -1;
                    break sBreak;
                }

                if (bit + bits < 8) return -1;
                bits -= add = 8 - bit;
                _1BYTE++;
            } else {
                if (bits < 9)
                    if (v === 0 || (v & Ol[bits]) === 0) return -1;
                    else break sBreak;
            }

            let last = _1BYTE + (bits >> 3);
            for (let BYTE = _1BYTE; BYTE < last; BYTE++)
                if (0 < (v = bytes.getUint8(BYTE))) {
                    add += (BYTE - _1BYTE) << 3;
                    break sBreak;
                }

            if ((bits &= 7) === 0 || (v = bytes.getUint8(last) & Ol[bits]) === 0) return -1;
            add += (last - _1BYTE) << 3;
        }

        if (existence) return MAX_INT;
        for (let i = 0; ; i++) if (((v >> i) & 1) === 1) return add + i;
    }

    export function bits2bytes(bits: number): number {
        return bits < 1 ? 0 : 1 + ((bits - 1) >> 3);
    }

    export class Pack {
        public bytes: ArrayBuffer = null!;

        constructor(public readonly meta: Pack.Meta) {}
    }

    export namespace Pack {
        export class Meta {
            public readonly fields: Array<Meta.Field>;
            constructor(public readonly id: number, public readonly _2 = 0, public readonly _4 = 0, public readonly _8 = 0, public readonly packMinBytes = 0, public readonly field_0_bit = 0, public readonly BITS_lenINbytes_bits = 0, fields = 0) {
                if (fields) this.fields = new Array<Meta.Field>(fields);
            }
        }

        export namespace Meta {
            export class Field {
                public readonly dims: number[];
                public readonly datatype: Meta;

                constructor(
                    public type = 0,
                    public varint: boolean,
                    public readonly length = 0,
                    public readonly size = 0,

                    public readonly const_dims_total = 0,

                    public readonly field_info_bits = 0,
                    public readonly sparse_bits = 0,
                    public var_dims_count = 0,
                    datatype?: Meta,

                    ...dims: number[]
                ) {
                    this.dims = dims;
                    this.datatype = datatype!;
                }
            }

            export namespace Field {
                export class CursorBase {
                    public bytes: DataView;
                    public meta: Meta;
                    public origin = 0;
                    public shift = 0;

                    public wrap(src: Meta | Pack) {
                        if (src instanceof Meta) {
                            this.meta = src;
                            this.bytes = new DataView(new ArrayBuffer(src.packMinBytes));
                        } else {
                            this.meta = src.meta;
                            this.bytes = src.bytes ? new DataView(src.bytes) : null!;
                            src.bytes = null!;
                        }
                        this.origin = 0;
                        this.reset();
                    }

                    public unwrap(): Pack | undefined {
                        if (!this.meta) return undefined;

                        let dst = new Pack(this.meta);
                        dst.bytes = this.bytes.buffer;

                        this.meta = <Meta>null!;
                        this.bytes = <DataView>null!;

                        return dst;
                    }

                    public BIT_E = 0;

                    public BIT_S = -1;
                    public BYTE_E = -1;

                    public BYTE_S = -1;

                    public field_bit = 0;
                    public LAST_BIT = 0;

                    public LAST_BYTE = 0;
                    public LAST_field_bit = 0;

                    public subitem_len = 0;

                    public field_0_bit(): number {
                        return this.meta.field_0_bit + this.origin * 8;
                    }

                    public first_field_bit(): number {
                        return first_1(this.bytes, this.origin * 8 + this.meta.field_0_bit, this.BIT_E - (this.origin * 8 + this.meta.field_0_bit), false);
                    }

                    public next_field_bit(): number {
                        return this.field_bit - this.meta.field_0_bit < this.meta.fields.length - 1 ? first_1(this.bytes, this.origin * 8 + this.field_bit + 1, this.BIT_E - (this.origin * 8 + this.field_bit + 1), false) : -1;
                    }

                    public getField(): Field {
                        return this.meta.fields[this.field_bit - this.meta.field_0_bit];
                    }

                    public reset(): boolean {
                        this.field_bit = -1;
                        this.BYTE_E = this.BYTE_S = this.origin + this.meta.packMinBytes + (this.meta.BITS_lenINbytes_bits === 0 ? 0 : get_bits(this.bytes, this.field_0_bit() - this.meta.BITS_lenINbytes_bits, this.meta.BITS_lenINbytes_bits));
                        this.BIT_E = this.BIT_S = this.BYTE_E << 3;
                        this.subitem_len = 0;
                        if (!this.meta.fields) this.LAST_BIT = (this.LAST_BYTE = this.BYTE_S) * 8;
                        else if (this.first_field_bit() < 0) {
                            this.LAST_BYTE = this.BYTE_E;
                            this.LAST_BIT = this.BIT_E;
                        } else this.LAST_BYTE = this.LAST_BIT = -1;

                        this.LAST_field_bit = -1;
                        return true;
                    }

                    public setLASTS(): number {
                        let delta = this.field_bit < 0 ? this.first_field_bit() : this.next_field_bit();
                        if (delta < 0) {
                            this.LAST_BYTE = this.BYTE_E;
                            this.LAST_field_bit = this.field_bit;
                            this.LAST_BIT = this.BIT_E;
                            return this.LAST_BYTE;
                        }

                        const BIT_E_FX = this.BIT_E;
                        const BIT_S_FX = this.BIT_S;
                        const BYTE_S_FX = this.BYTE_S;
                        const BYTE_E_FX = this.BYTE_E;
                        const item_size_FX = this.subitem_len;
                        const field_bit_FX = this.field_bit;
                        if (this.field_bit < 0) this.field_bit = this.meta.field_0_bit - 1;
                        do {
                            this.field_bit += delta + 1;
                            this.BIT_S = this.BIT_E;
                            this.BYTE_S = this.BYTE_E;
                            let fld = this.getField();
                            if (0 < fld.length)
                                if (0 < fld.size) this.BYTE_E += fld.const_dims_total * fld.length * fld.size;
                                else this.BIT_E += fld.const_dims_total * fld.length * fld.size;
                            else this.setBounds(fld);
                        } while (-1 < (delta = this.next_field_bit()));

                        this.LAST_BYTE = this.BYTE_E;
                        this.LAST_BIT = this.BIT_E;
                        this.LAST_field_bit = this.field_bit;
                        this.BIT_E = BIT_E_FX;
                        this.BIT_S = BIT_S_FX;
                        this.BYTE_S = BYTE_S_FX;
                        this.BYTE_E = BYTE_E_FX;
                        this.subitem_len = item_size_FX;
                        this.field_bit = field_bit_FX;
                        return this.LAST_BYTE;
                    }

                    public setBounds(fld: Field) {
                        this.subitem_len = fld.size;
                        let bit = this.BIT_E;
                        let count = 1;
                        let arrays_length = 0;
                        switch (fld.type) {
                            case 1:
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BIT_E -= fld.field_info_bits;
                                this.BYTE_E += fld.const_dims_total * count * -fld.length * fld.size;
                                break;
                            case 3:
                                this.BIT_E -= fld.field_info_bits;
                                arrays_length = get_bits(this.bytes, this.BIT_E, -fld.length);
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) arrays_length *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BYTE_E += fld.const_dims_total * arrays_length * fld.size;
                                break;
                            case 5:
                                count = fld.const_dims_total;
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BIT_E -= fld.field_info_bits;
                                let all_arrays_sum = 0;
                                while (0 < count--) all_arrays_sum += get_bits(this.bytes, (this.BIT_E -= -fld.length), -fld.length);
                                this.BYTE_E += all_arrays_sum * fld.size;
                                break;
                            case 7:
                                this.subitem_len = 0;
                                count = 1;
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BIT_E -= fld.field_info_bits + count * fld.const_dims_total * -fld.length * fld.size;
                                break;
                            case 9:
                                this.BIT_E -= fld.field_info_bits;
                                arrays_length = get_bits(this.bytes, this.BIT_E, -fld.length);
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) arrays_length *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BIT_E -= fld.const_dims_total * arrays_length * fld.size;
                                break;
                            case 11:
                                count = fld.const_dims_total;
                                if (0 < fld.var_dims_count) for (let i = 0; i < fld.dims.length; i++) if (fld.dims[i] < 0) count *= get_bits(this.bytes, (bit -= -fld.dims[i]), -fld.dims[i]);
                                this.BIT_E -= fld.field_info_bits;
                                while (0 < count--) {
                                    let bits = get_bits(this.bytes, (this.BIT_E -= -fld.length), -fld.length) * fld.size;
                                    this.BIT_E -= bits;
                                }
                                break;
                            default:
                                if ((count = get_bits(this.bytes, (this.BIT_E -= fld.field_info_bits), fld.sparse_bits)) === 0) return;
                                bit = this.BIT_E;
                                switch (fld.type) {
                                    case 2:
                                        this.BIT_E -= count;
                                        while (this.BIT_E < bit--) if ((this.bytes.getUint8(bit >>> 3) & (1 << (bit & 7))) === 0) count--;
                                        this.BYTE_E += -fld.length * count * fld.size;
                                        break;
                                    case 4:
                                        this.BIT_E -= count;
                                        arrays_length = get_bits(this.bytes, bit + 2 * fld.sparse_bits, -fld.length);
                                        while (this.BIT_E < bit--) if ((this.bytes.getUint8(bit >>> 3) & (1 << (bit & 7))) === 0) count--;
                                        this.BYTE_E += count * arrays_length * fld.size;
                                        break;
                                    case 6:
                                        let all_arrays_sum = 0;
                                        while (0 < count--) if ((this.bytes.getUint8(--bit >> 3) & (1 << (bit & 7))) !== 0) all_arrays_sum += get_bits(this.bytes, (bit -= -fld.length), -fld.length);
                                        this.BIT_E = bit;
                                        this.BYTE_E += all_arrays_sum * fld.size;
                                        break;
                                    case 8:
                                        for (let bits = -fld.length * fld.size; 0 < count--; ) if ((this.bytes.getUint8(--bit >> 3) & (1 << (bit & 7))) !== 0) bit -= bits;
                                        this.BIT_E = bit;
                                        break;
                                    case 10:
                                        for (let bits = get_bits(this.bytes, bit + 2 * fld.sparse_bits, -fld.length) * fld.size; 0 < count--; ) if ((this.bytes.getUint8(--bit >> 3) & (1 << (bit & 7))) !== 0) bit -= bits;
                                        this.BIT_E = bit;
                                        break;
                                    case 12:
                                        while (0 < count--)
                                            if ((this.bytes.getUint8(--bit >> 3) & (1 << (bit & 7))) !== 0) {
                                                let len = get_bits(this.bytes, (bit -= -fld.length), -fld.length);
                                                bit -= len * fld.size;
                                            }

                                        this.BIT_E = bit;
                                        break;
                                    default:
                                        return;
                                }

                                return;
                        }
                    }
                }
            }
        }
        export class Cursor extends Meta.Field.CursorBase {
            private as_pack_ = new Cursor.View(this);

            public as_pack<T extends Object>(proto: T): T {
                Object.setPrototypeOf(this.as_pack_, proto);
                return <T>(<unknown>this.as_pack_);
            }

            private as_field_ = new Cursor.View(this);

            public as_field<T extends Object>(proto: T): T {
                Object.setPrototypeOf(this.as_field_, proto);
                return <T>(<unknown>this.as_field_);
            }

            private as_item_ = new Cursor.View(this);

            public as_item<T extends Object>(proto: T): T {
                Object.setPrototypeOf(this.as_item_, proto);
                return <T>(<unknown>this.as_item_);
            }
            public D: number[];

            public BIT = -1;

            public BYTE = 0;

            private field_item = 0;

            private field_item_0 = 0;
            private field_items = 0;
            private field_items_total = 0;
            public item_len = 0;
            private readonly next_: Cursor;

            private readonly prev: Cursor;

            public constructor(nested_max: number, dims_max: number, prev?: Cursor) {
                super();
                this.D = new Array(dims_max);
                this.prev = prev!;
                this.next_ = 1 < nested_max ? new Cursor(nested_max - 1, dims_max, this) : <Cursor>null!;
            }

            public length(): number {
                return (-1 < this.LAST_BYTE ? this.LAST_BYTE : this.setLASTS()) - this.origin;
            }

            public next<T extends Object>(origin: number, proto: T): T {
                const next_ = this.next_;
                next_.bytes = this.bytes;
                next_.meta = this.getField().datatype;
                next_.origin = origin;
                next_.reset();
                next_.LAST_BYTE = origin + this.item_len;
                return next_.as_pack(proto);
            }

            public reset(): boolean {
                super.reset();
                this.BYTE = this.BYTE_S;
                this.BIT = this.BIT_S;
                this.item_len = 0;
                this.field_item_0 = -1;
                this.field_item = MAX_INT;
                this.field_items = 0;
                return true;
            }

            private init(fld: Meta.Field) {
                switch (fld.type) {
                    case 1:
                        this.item_len = -fld.length;
                        this.BIT = this.BIT_E;
                        break;
                    case 3:
                        this.item_len = get_bits(this.bytes, (this.BIT = this.BIT_S - fld.field_info_bits), -fld.length);
                        break;
                    case 5:
                    case 11:
                        this.item_len = 0;
                        this.BIT = this.BIT_S - fld.field_info_bits;
                        this.field_item_0 = 0;
                        this.field_item = MAX_INT;
                        this.field_items = this.field_items_total;
                        break;
                    case 7:
                        this.item_len = -fld.length;
                        this.field_item_0 = 0;
                        this.BIT = this.BIT_E;
                        this.field_item = (this.field_items = this.field_items_total) === 0 ? MAX_INT : 0;
                        break;
                    case 9:
                        this.item_len = get_bits(this.bytes, (this.BIT = this.BIT_S - fld.field_info_bits), -fld.length);
                        this.field_item_0 = 0;
                        this.BIT = this.BIT_E;
                        this.field_item = (this.field_items = this.field_items_total) === 0 ? MAX_INT : 0;
                        break;
                    default:
                        this.BIT = this.BIT_S - fld.field_info_bits;
                        switch (fld.type) {
                            case 2:
                            case 6:
                            case 8:
                            case 12:
                                this.item_len = -fld.length;
                                this.BIT = this.BIT_S - fld.field_info_bits;
                                break;
                            case 4:
                            case 10:
                                this.item_len = get_bits(this.bytes, (this.BIT = this.BIT_S - fld.field_info_bits) + 2 * fld.sparse_bits, -fld.length);
                                break;
                            default:
                                return;
                        }

                        this.field_item = MAX_INT;
                        this.field_items = get_bits(this.bytes, this.BIT, fld.sparse_bits);
                        this.field_item_0 = get_bits(this.bytes, this.BIT + fld.sparse_bits, fld.sparse_bits);
                        break;
                }

                this.BYTE = this.BYTE_S;
            }

            public unwrap(): Pack {
                let ret = super.unwrap();
                for (let cur: Cursor = this; (cur = cur.next_); cur.bytes = null!, cur.meta = null!);
                return ret!;
            }

            public set_field(fbit: number, each_item_size: number, ...var_dims: number[]): boolean {
                if (this.field_bit === fbit) return true;
                if (each_item_size < 0 && (this.BIT_E <= 8 * this.origin + fbit || (this.bytes.getUint8(this.origin + (fbit >> 3)) & (1 << (fbit & 7))) === 0)) return false;
                let fld: Meta.Field;

                insert_field: {
                    let delta: number;
                    if (this.field_bit === -1 || (fbit < this.field_bit && this.reset())) {
                        if ((delta = this.first_field_bit()) === -1 || fbit < this.meta.field_0_bit + delta) break insert_field;
                        this.field_bit = this.meta.field_0_bit - 1;
                    } else if ((delta = this.next_field_bit()) === -1 || fbit < this.field_bit + 1 + delta) break insert_field;

                    do {
                        this.BIT_S = this.BIT_E;
                        this.BYTE_S = this.BYTE_E;
                        fld = this.meta.fields[(this.field_bit += 1 + delta) - this.meta.field_0_bit];
                        if (0 < fld.length)
                            if (0 < fld.size) this.BYTE_E += fld.const_dims_total * fld.length * fld.size;
                            else this.BIT_E += fld.const_dims_total * fld.length * fld.size;
                        else this.setBounds(fld);
                    } while (-1 < (delta = this.next_field_bit()) && this.field_bit + 1 + delta < fbit);

                    if (this.field_bit === fbit || each_item_size < 0) {
                        this.item_len = fld.length;
                        this.subitem_len = fld.size;
                        this.BIT = this.BIT_E;
                        this.BYTE = this.BYTE_S;
                        if (fld.length < 1) {
                            this.field_items_total = fld.const_dims_total;
                            if (0 < fld.var_dims_count) {
                                this.D[fld.var_dims_count] = 0;
                                for (let i = 0, bit = this.BIT_S, dim; i < fld.dims.length; i++)
                                    if ((dim = fld.dims[i]) < 1) this.field_items_total *= this.D[i] = get_bits(this.bytes, (bit -= -dim), -dim);
                                    else this.D[i] = dim;
                            }
                            this.init(fld);
                        }
                        return this.field_bit === fbit;
                    }
                }
                if (each_item_size < 0) return false;

                fld = this.meta.fields[fbit - this.meta.field_0_bit];
                if (fld.type === 0) {
                    if (0 < fld.size) {
                        this.insert(fbit, 0, fld.const_dims_total * fld.length * fld.size);
                        this.BYTE_E = (this.BYTE = this.BYTE_S) + fld.const_dims_total * (this.item_len = fld.length) * (this.subitem_len = fld.size);
                    } else {
                        this.insert(fbit, fld.const_dims_total * fld.length * -fld.size, 0);
                        this.BIT = this.BIT_E = this.BIT_S + fld.const_dims_total * (this.item_len = fld.length) * fld.size;
                    }
                    return true;
                }

                let total = fld.const_dims_total;
                if (0 < fld.var_dims_count)
                    for (let i = 0, d = 0; i < fld.dims.length; i++)
                        if (fld.dims[i] < 0) total *= this.D[i] = var_dims[d++];
                        else this.D[i] = fld.dims[i];
                switch (fld.type) {
                    case 1:
                        this.insert(fbit, fld.field_info_bits, total * -fld.length * fld.size);
                        break;
                    case 2:
                    case 6:
                    case 8:
                    case 12:
                        this.insert(fbit, fld.field_info_bits, 0);
                        break;
                    case 3:
                        if (fld.datatype) each_item_size = fld.datatype.packMinBytes < each_item_size ? each_item_size : fld.datatype.packMinBytes;
                        this.insert(fbit, fld.field_info_bits, total * each_item_size * fld.size);
                        set_bits(each_item_size, -fld.length, this.bytes, this.BIT_S - fld.field_info_bits);
                        break;
                    case 4:
                        this.insert(fbit, fld.field_info_bits, 0);
                        set_bits(each_item_size, -fld.length, this.bytes, this.BIT_S - fld.field_info_bits + 2 * fld.sparse_bits);
                        break;
                    case 5:
                    case 11:
                        this.insert(fbit, fld.field_info_bits + total * -fld.length, 0);
                        break;
                    case 7:
                        this.insert(fbit, fld.field_info_bits + total * -fld.length * fld.size, 0);
                        break;
                    case 9:
                        this.insert(fbit, fld.field_info_bits + total * each_item_size * fld.size, 0);
                        set_bits(each_item_size, -fld.length, this.bytes, this.BIT_S - fld.field_info_bits);
                        break;
                    case 10:
                        this.insert(fbit, fld.field_info_bits, 0);
                        set_bits(each_item_size, -fld.length, this.bytes, this.BIT_S - fld.field_info_bits + 2 * fld.sparse_bits);
                        break;
                    default:
                        break;
                }

                this.field_items_total = total;
                if (0 < fld.var_dims_count) for (let i = 0, bit = this.BIT_S; i < fld.dims.length; i++) if (fld.dims[i] < 0) set_bits(this.D[i], -fld.dims[i], this.bytes, (bit -= -fld.dims[i]));
                this.setBounds(fld);
                this.init(fld);
                return true;
            }

            public set_item(item: number, length: number): boolean {
                const fld = this.getField();
                if (this.field_item === item) {
                    if (fld.type === 5 && -1 < length && length !== this.item_len) {
                        if (fld.datatype) length = fld.datatype.packMinBytes < length ? length : fld.datatype.packMinBytes;

                        this.resize_bytes((length - this.item_len) * this.subitem_len);
                        set_bits((this.item_len = length), -fld.length, this.bytes, this.BIT);
                    }
                    return true;
                }

                let bit = this.BIT;
                let _field_item = this.field_item;
                const len_bits = -fld.length;
                switch (fld.type) {
                    default:
                        return false;
                    case 2:
                    case 4:
                        if (this.field_items === 0 || item < this.field_item_0 || this.field_item_0 + this.field_items <= item || (this.bytes.getUint8((bit = this.BIT_S - fld.field_info_bits - 1 - (item - this.field_item_0)) >> 3) & (1 << (bit & 7))) === 0) {
                            if (length < 0) return false;
                            if (item < this.field_item_0 || this.field_items === 0) {
                                this.BIT = this.BIT_S - fld.field_info_bits;
                                this.BYTE = this.BYTE_S;
                                let ins_items = this.field_items === 0 ? 1 : this.field_item_0 - item;
                                this.insert(this.field_bit, ins_items, this.item_len * fld.size);
                                this.BIT = this.BIT_S - fld.field_info_bits - 1;
                                set_bits((this.field_item_0 = item), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits + fld.sparse_bits);
                                set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                            } else if (item < this.field_item_0 + this.field_items) {
                                if (item < this.field_item) {
                                    this.field_item = this.field_item_0;
                                    this.BIT = this.BIT_S - fld.field_info_bits - 1;
                                    this.BYTE = this.BYTE_S;
                                } else {
                                    this.BIT--;
                                    this.field_item++;
                                    this.BYTE += this.item_len * fld.size;
                                }

                                for (let bytes = this.item_len * fld.size; this.field_item < item; this.field_item++, this.BIT--) if ((this.bytes.getUint8(this.BIT >> 3) & (1 << (this.BIT & 7))) !== 0) this.BYTE += bytes;

                                this.insert(this.field_bit, 0, this.item_len * fld.size);
                            } else {
                                let ins_items = item + 1 - (this.field_item_0 + this.field_items);
                                this.BIT = this.BIT_E;
                                this.BYTE = this.BYTE_E;
                                this.insert(this.field_bit, ins_items, this.item_len * fld.size);

                                this.BIT = this.BIT_E;
                                this.BYTE = this.BYTE_E - this.item_len * fld.size;
                                set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                            }

                            this.bytes.setUint8(this.BIT >> 3, this.bytes.getUint8(this.BIT >> 3) | (1 << (this.BIT & 7)));
                            break;
                        }

                        if (item < this.field_item) {
                            this.field_item = this.field_item_0;
                            this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            this.BYTE = this.BYTE_S;
                        } else {
                            this.BIT--;
                            this.field_item++;
                            this.BYTE += this.item_len * fld.size;
                        }

                        for (let bytes = this.item_len * fld.size; ; this.field_item++, this.BIT--)
                            if ((this.bytes.getUint8(this.BIT >> 3) & (1 << (this.BIT & 7))) !== 0)
                                if (this.field_item === item) break;
                                else this.BYTE += bytes;
                        return true;
                    case 5:
                        if (this.field_items_total - 1 < item || get_bits(this.bytes, this.BIT_S - fld.field_info_bits - (item + 1) * len_bits, len_bits) === 0) {
                            if (length < 0) return false;
                            if (this.field_items_total - 1 < item) throw new Error('No room for item=' + item + '. The field_items_total =' + this.field_items_total);
                            if (item < this.field_item) {
                                this.field_item = 0;
                                this.BIT = this.BIT_S - fld.field_info_bits;
                                this.BYTE = this.BYTE_S;
                                this.item_len = get_bits(this.bytes, (this.BIT -= len_bits), len_bits);
                            }

                            for (; this.field_item < item; this.BYTE += this.item_len * fld.size, this.item_len = get_bits(this.bytes, (this.BIT -= len_bits), len_bits), this.field_item++);
                            if (this.item_len !== 0) throw new Error('Already allocated');
                            this.insert(this.field_bit, 0, length * fld.size);
                            set_bits(length, len_bits, this.bytes, this.BIT);
                            this.item_len = length;
                            break;
                        }

                        if (item < this.field_item) {
                            this.field_item = 0;
                            this.BIT = this.BIT_S - fld.field_info_bits;
                            this.item_len = get_bits(this.bytes, (this.BIT -= len_bits), len_bits);
                            this.BYTE = this.BYTE_S;
                        }

                        for (; this.field_item < item; this.BYTE += this.item_len * fld.size, this.item_len = get_bits(this.bytes, (this.BIT -= len_bits), len_bits), this.field_item++);

                        if (-1 < length && length !== this.item_len) {
                            this.resize_bytes((length - this.item_len) * this.subitem_len);
                            set_bits((this.item_len = length), -fld.length, this.bytes, this.BIT);
                        }

                        return true;
                    case 6:
                        if (this.field_item_0 <= item && item < this.field_item_0 + this.field_items) {
                            let _BYTE = this.BYTE;
                            if (item < _field_item) {
                                _field_item = this.field_item_0;
                                bit = this.BIT_S - fld.field_info_bits - 1;
                                _BYTE = this.BYTE_S;
                            } else {
                                bit--;
                                _field_item++;
                                _BYTE += this.item_len * fld.size;
                            }

                            for (; ; bit--, _field_item++)
                                if ((this.bytes.getUint8(bit >>> 3) & (1 << (bit & 7))) !== 0) {
                                    bit -= len_bits;
                                    if (_field_item === item) {
                                        this.field_item = _field_item;
                                        this.BYTE = _BYTE;
                                        this.BIT = bit;
                                        this.item_len = get_bits(this.bytes, bit, len_bits);
                                        return true;
                                    }

                                    _BYTE += get_bits(this.bytes, bit, len_bits) * fld.size;
                                } else if (_field_item === item) break;
                        }

                        if (length < 0) return false;
                        if (item < this.field_item_0 || this.field_items === 0) {
                            this.BIT = this.BIT_S - fld.field_info_bits;
                            this.BYTE = this.BYTE_S;
                            let ins_items = this.field_items === 0 ? 1 : this.field_item_0 - item;
                            this.insert(this.field_bit, ins_items + len_bits, length * fld.size);
                            this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            set_bits((this.field_item_0 = item), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits + fld.sparse_bits);
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        } else if (item < this.field_item_0 + this.field_items) {
                            if (item < this.field_item) {
                                this.field_item = this.field_item_0;
                                this.BIT = this.BIT_S - fld.field_info_bits - 1;
                                this.BYTE = this.BYTE_S;
                            } else {
                                this.BIT--;
                                this.field_item++;
                                this.BYTE += this.item_len * fld.size;
                            }

                            for (; this.field_item < item; this.BIT--, this.field_item++)
                                if ((this.bytes.getUint8(this.BIT >>> 3) & (1 << (this.BIT & 7))) !== 0) {
                                    this.BIT -= len_bits;
                                    this.item_len = get_bits(this.bytes, this.BIT, len_bits);
                                    this.BYTE += this.item_len * fld.size;
                                }

                            this.insert(this.field_bit, len_bits, length * fld.size);
                        } else {
                            let ins_items = item - (this.field_item_0 + this.field_items) + 1;
                            this.BIT = this.BIT_E;
                            this.BYTE = this.BYTE_E;
                            this.insert(this.field_bit, ins_items + len_bits, length * fld.size);
                            this.BIT = this.BIT_E + len_bits;
                            this.BYTE = this.BYTE_E - length * fld.size;
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        }

                        this.bytes.setUint8(this.BIT >>> 3, this.bytes.getUint8(this.BIT >> 3) | (1 << (this.BIT & 7)));
                        set_bits((this.item_len = length), len_bits, this.bytes, (this.BIT -= len_bits));
                        break;
                    case 8:
                    case 10:
                        if (this.field_item_0 <= item && item < this.field_item_0 + this.field_items) {
                            if (item < _field_item) {
                                _field_item = this.field_item_0;
                                bit = this.BIT_S - fld.field_info_bits - 1;
                            } else {
                                bit--;
                                _field_item++;
                            }

                            for (let bits_ = this.item_len * fld.size; ; bit--, _field_item++)
                                if ((this.bytes.getUint8(bit >>> 3) & (1 << (bit & 7))) !== 0) {
                                    bit -= bits_;
                                    if (_field_item === item) {
                                        this.field_item = item;
                                        this.BIT = bit;
                                        return true;
                                    }
                                } else if (_field_item === item) break;
                        }

                        if (length < 0) return false;
                        let bits = this.item_len * fld.size;
                        if (this.field_item === item) throw new Error();
                        if (item < this.field_item_0 || this.field_items === 0) {
                            this.BIT = this.BIT_S - fld.field_info_bits;
                            this.BYTE = this.BYTE_S;
                            let ins_items = this.field_items === 0 ? 1 : this.field_item_0 - item;
                            this.insert(this.field_bit, ins_items + bits, 0);
                            this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            set_bits((this.field_item_0 = item), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits + fld.sparse_bits);
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        } else if (item < this.field_item_0 + this.field_items) {
                            if (item < this.field_item) {
                                this.field_item = this.field_item_0;
                                this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            } else {
                                this.BIT--;
                                this.field_item++;
                            }

                            for (; this.field_item < item; this.BIT--, this.field_item++) if ((this.bytes.getUint8(this.BIT >>> 3) & (1 << (this.BIT & 7))) !== 0) this.BIT -= bits;

                            this.insert(this.field_bit, bits, 0);
                        } else {
                            let ins_items = item + 1 - (this.field_item_0 + this.field_items);
                            this.BIT = this.BIT_E;
                            this.insert(this.field_bit, ins_items + bits, 0);
                            this.BIT = this.BIT_E + bits;
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        }

                        this.bytes.setUint8(this.BIT >>> 3, this.bytes.getUint8(this.BIT >> 3) | (1 << (this.BIT & 7)));
                        this.BIT -= bits;
                        break;
                    case 11:
                        if (item < this.field_items_total) {
                            if (item < _field_item) {
                                _field_item = -1;
                                bit = this.BIT_S - fld.field_info_bits;
                            } else if (item === this.field_item) return true;

                            let _items;
                            do {
                                let bits = fld.size * (_items = get_bits(this.bytes, (bit -= len_bits), len_bits));
                                bit -= bits;
                                _field_item++;
                            } while (_field_item < item);

                            if (0 < _items) {
                                this.BIT = bit;
                                this.item_len = _items;
                                this.field_item = _field_item;
                                return true;
                            }
                        }

                        if (length < 0) return false;
                        if (this.field_items_total - 1 < item) throw new Error('No room for item=' + item + '. The field_items_total =' + this.field_items_total);
                        if (item < this.field_item) {
                            this.field_item = -1;
                            this.BIT = this.BIT_S - fld.field_info_bits;
                        }

                        while (this.field_item < item) {
                            let bits = get_bits(this.bytes, (this.BIT -= len_bits), len_bits) * fld.size;
                            this.BIT -= bits;
                            this.field_item++;
                        }

                        if (get_bits(this.bytes, this.BIT, len_bits) !== 0) throw new Error('Already allocated');
                        this.insert(this.field_bit, length * fld.size, 0);
                        set_bits(length, len_bits, this.bytes, this.BIT);
                        this.BIT -= length * fld.size;
                        this.item_len = length;
                        break;
                    case 12:
                        if (this.field_item_0 <= item && item < this.field_item_0 + this.field_items) {
                            if (item < _field_item) {
                                _field_item = this.field_item_0;
                                bit = this.BIT_S - fld.field_info_bits - 1;
                            } else {
                                bit--;
                                _field_item++;
                            }

                            for (; ; bit--, _field_item++)
                                if ((this.bytes.getUint8(bit >>> 3) & (1 << (bit & 7))) !== 0) {
                                    bit -= len_bits;
                                    if (_field_item === item) {
                                        this.field_item = _field_item;
                                        this.item_len = get_bits(this.bytes, bit, len_bits);
                                        this.BIT = bit - this.item_len * fld.size;
                                        return true;
                                    }

                                    bit -= get_bits(this.bytes, bit, len_bits) * fld.size;
                                } else if (_field_item === item) break;
                        }

                        if (length < 0) return false;
                        if (item < this.field_item_0 || this.field_items === 0) {
                            this.BIT = this.BIT_S - fld.field_info_bits;
                            let ins_items = this.field_items === 0 ? 1 : this.field_item_0 - item;
                            this.insert(this.field_bit, ins_items + len_bits + length * fld.size, 0);
                            this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            set_bits((this.field_item_0 = item), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits + fld.sparse_bits);
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        } else if (item < this.field_item_0 + this.field_items) {
                            if (item < this.field_item) {
                                this.field_item = this.field_item_0;
                                this.BIT = this.BIT_S - fld.field_info_bits - 1;
                            } else {
                                this.BIT--;
                                this.field_item++;
                            }

                            for (; this.field_item < item; this.BIT--, this.field_item++)
                                if ((this.bytes.getUint8(this.BIT >>> 3) & (1 << (this.BIT & 7))) !== 0) {
                                    this.BIT -= len_bits;
                                    this.item_len = get_bits(this.bytes, this.BIT, len_bits);
                                    this.BIT -= this.item_len * fld.size;
                                }

                            this.insert(this.field_bit, len_bits + length * fld.size, 0);
                        } else {
                            let ins_items = item - (this.field_item_0 + this.field_items) + 1;
                            this.BIT = this.BIT_E;
                            this.insert(this.field_bit, ins_items + len_bits + length * fld.size, 0);
                            this.BIT = this.BIT_E + length * fld.size + len_bits;
                            set_bits((this.field_items += ins_items), fld.sparse_bits, this.bytes, this.BIT_S - fld.field_info_bits);
                        }

                        this.bytes.setUint8(this.BIT >>> 3, this.bytes.getUint8(this.BIT >> 3) | (1 << (this.BIT & 7)));
                        set_bits((this.item_len = length), len_bits, this.bytes, (this.BIT -= len_bits));
                        this.BIT -= length * fld.size;
                        break;
                }

                this.field_item = item;
                return false;
            }

            private resize_bytes(diff: number) {
                let cur: Cursor = this;
                while (cur.prev) cur = cur.prev;
                if (cur.LAST_BYTE < 0) cur.setLASTS();
                const new_buffer = copy_bytes(this.bytes.buffer, 0, new ArrayBuffer(cur.LAST_BYTE + diff), 0, this.BYTE);
                if (diff < 0) copy_bytes(this.bytes.buffer, this.BYTE - diff, new_buffer, this.BYTE, cur.LAST_BYTE - (this.BYTE - diff));
                else copy_bytes(this.bytes.buffer, this.BYTE, new_buffer, this.BYTE + diff, cur.LAST_BYTE - this.BYTE);

                const new_bytes = (this.bytes = new DataView(new_buffer));
                for (;;) {
                    if (-1 < cur.LAST_BYTE) cur.LAST_BYTE += diff;
                    cur.BYTE_E += diff;
                    cur.bytes = new_bytes;
                    if (cur === this) return;
                    if (-1 < cur.field_bit) {
                        let length = cur.getField().length;
                        if (length < 0) set_bits((cur.item_len += diff), -length, this.bytes, cur.BIT);
                    }

                    cur = cur.next_;
                }
            }
            private insert(fbit: number, bits: number, bytes: number) {
                if (this.field_bit !== fbit) {
                    this.BIT = this.BIT_S = this.BIT_E;
                    this.BYTE = this.BYTE_S = this.BYTE_E;
                    this.item_len = 0;
                }

                if (this.LAST_BYTE < 0) this.setLASTS();
                let old_pack_data_bytes = this.LAST_BYTE;
                let add_to_bits_bytes;
                if (this.BIT === this.LAST_BIT && this.BYTE === old_pack_data_bytes) {
                    add_to_bits_bytes = this.meta.BITS_lenINbytes_bits === 0 ? 0 : bits2bytes(fbit + bits - (this.LAST_BIT - this.origin * 8) + 1);
                    if (0 < add_to_bits_bytes || 0 < bytes) {
                        this.resize_bytes(add_to_bits_bytes + bytes);
                        if (0 < add_to_bits_bytes) {
                            copy_bits(this.bytes, this.BIT, (old_pack_data_bytes << 3) - this.BIT, this.bytes, this.BIT + (add_to_bits_bytes << 3));
                            set_0(this.bytes, this.BIT, add_to_bits_bytes << 3);
                        }
                    }
                } else {
                    let add_bits_bits = this.meta.BITS_lenINbytes_bits === 0 ? 0 : bits - (this.LAST_BIT - (this.LAST_field_bit + 1));
                    add_to_bits_bytes = this.meta.BITS_lenINbytes_bits === 0 ? 0 : bits2bytes(add_bits_bits);
                    if (0 < add_to_bits_bytes || 0 < bytes) {
                        this.resize_bytes(add_to_bits_bytes + bytes);
                        if (0 < bits) {
                            copy_bits(this.bytes, this.BIT, (this.BYTE << 3) - this.BIT, this.bytes, this.BIT + (add_to_bits_bytes << 3));

                            copy_bits(this.bytes, this.LAST_BIT, this.BIT - this.LAST_BIT, this.bytes, this.LAST_BIT + (add_to_bits_bytes << 3) - bits);

                            if (0 < (add_to_bits_bytes << 3) - bits) set_0(this.bytes, this.LAST_BIT, (add_to_bits_bytes << 3) - bits);
                            set_0(this.bytes, this.BIT + (add_to_bits_bytes << 3) - bits, bits);
                        }
                    } else {
                        copy_bits(
                            this.bytes,
                            this.LAST_BIT,

                            this.BIT - this.LAST_BIT,
                            this.bytes,
                            this.LAST_BIT - bits
                        );
                        set_0(this.bytes, this.BIT - bits, bits);
                    }
                }

                if (0 < this.meta.BITS_lenINbytes_bits && 0 < add_to_bits_bytes) {
                    let old_value = get_bits(this.bytes, this.field_0_bit() - this.meta.BITS_lenINbytes_bits, this.meta.BITS_lenINbytes_bits);
                    set_bits(old_value + add_to_bits_bytes, this.meta.BITS_lenINbytes_bits, this.bytes, this.field_0_bit() - this.meta.BITS_lenINbytes_bits);
                }

                let zazor_delta = (add_to_bits_bytes << 3) - bits;
                this.LAST_BIT += zazor_delta;
                this.BIT += add_to_bits_bytes << 3;
                this.BYTE += add_to_bits_bytes;
                if (fbit === this.field_bit) {
                    this.BIT_E += zazor_delta;
                    this.BIT_S += add_to_bits_bytes << 3;
                    this.BYTE_S += add_to_bits_bytes;
                } else {
                    this.BIT_S = this.BIT_E = this.BIT;
                    this.BYTE_E = this.BYTE_S = this.BYTE;
                    this.bytes.setUint8(this.origin + (fbit >> 3), this.bytes.getUint8(this.origin + (fbit >> 3)) | (1 << (fbit & 7)));
                    if (this.LAST_field_bit < fbit) this.LAST_field_bit = fbit;
                    this.field_bit = fbit;
                }
            }
        }

        export namespace Cursor {
            export class View {
                public data_: Cursor;

                constructor(data?: Cursor) {
                    this.data_ = data!;
                }
            }

            const utf8decoder = new TextDecoder();
            const utf8encoder = new TextEncoder();

            export class UTF8 extends Uint8Array {
                constructor(cur: Cursor, src?: string, index: number = 0) {
                    if (src) {
                        let size = 0;

                        for (let Len = src.length, ch = 0, ch2 = 0, i = 0; i !== Len; ) {
                            ch = src.charCodeAt(i);
                            i += 1;
                            if (0xd800 <= ch && ch <= 0xdbff) {
                                if (i === Len) {
                                    size += 3;
                                    break;
                                }
                                ch2 = src.charCodeAt(i);
                                if (0xdc00 <= ch2 && ch2 <= 0xdfff) {
                                    ch = (ch - 0xd800) * 0x400 + ch2 - 0xdc00 + 0x10000;
                                    i += 1;
                                    if (0xffff < ch) {
                                        size += 4;
                                        continue;
                                    }
                                } else size += 3;
                            }
                            if (ch <= 0x007f) size += 1;
                            else if (ch <= 0x07ff) size += 2;
                            else size += 3;
                        }

                        if (index < 0) cur.set_field(-index - 1, size);
                        else cur.set_item(index, size);

                        super(cur.bytes.buffer, cur.BYTE, size);

                        utf8encoder.encodeInto(src, this);
                    } else super(cur.bytes.buffer, cur.BYTE, cur.item_len);
                }

                toString(): string {
                    return utf8decoder.decode(this);
                }
            }
        }
    }

    export namespace Channel {
        export function bytes_transfer(src: Transmitter, dst: Receiver, buff: Uint8Array = new Uint8Array(new ArrayBuffer(1024))) {
            for (let len = 0, max = buff.byteLength; 0 < (len = src.packs_into_bytes(buff, 0, max)); dst.bytes_into_packs(buff, 0, len));
        }

        const BR = 0x55;
        const tab: ReadonlyArray<number> = [0, 4129, 8258, 12387, 16516, 20645, 24774, 28903, 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935];

        function crc16(data: number, crc: number): number {
            crc = 0xffff & (tab[((crc >> 12) ^ (data >> 4)) & 0x0f] ^ (crc << 4));
            return 0xffff & (tab[((crc >> 12) ^ (data & 0x0f)) & 0x0f] ^ (crc << 4));
        }

        const tab32 = (function() {
            let c;
            const ret = new Uint32Array(new ArrayBuffer(255 * 4));
            for (let n = 0; n < 256; n++) {
                c = n;
                for (let k = 0; k < 8; k++) c = c & 1 ? 0xedb88320 ^ (c >>> 1) : c >>> 1;

                ret[n] = c;
            }
            return ret;
        })();

        export function crc32(data: number, crc?: number): number {
            let ret = crc || 0 ^ -1;

            ret = (ret >>> 8) ^ tab32[(ret ^ data) & 0xff];

            return (ret ^ -1) >>> 0;
        }

        export class Flow {
            readonly cursors: Pack.Meta.Field.CursorBase[];
            level = 0;
            UvalueH = 0;
            Uvalue = 0;
            crc = 0;

            state: Flow.STATE = Flow.STATE.STANDBY;

            constructor(nested_max: number) {
                this.cursors = new Array<Pack.Meta.Field.CursorBase>(nested_max);
                for (let i = 0; i < nested_max; i++) this.cursors[i] = new Pack.Meta.Field.CursorBase();
            }

            pullUInt(src: DataView, byte: number, bytes: number) {
                switch (bytes) {
                    case 2:
                        this.Uvalue = src.getUint16(byte, true);
                        this.UvalueH = 0;
                        break;
                    case 4:
                        this.Uvalue = src.getUint32(byte, true);
                        this.UvalueH = 0;
                        break;
                    case 8:
                        this.Uvalue = src.getUint32(byte, true);
                        this.UvalueH = src.getUint32(byte + 4, true);
                }
            }

            pushUInt(dst: DataView, byte: number, bytes: number) {
                switch (bytes) {
                    case 2:
                        dst.setUint16(byte, this.Uvalue, true);
                        break;
                    case 4:
                        dst.setUint32(byte, this.Uvalue, true);
                        break;
                    case 8:
                        dst.setUint32(byte, this.Uvalue, true);
                        dst.setUint32(byte + 4, this.UvalueH, true);
                }
            }

            mode: Flow.MODE = Flow.MODE.NONE;

            public next(): Pack.Meta.Field.CursorBase | null {
                let delta = -1;
                let cur: Pack.Meta.Field.CursorBase = this.cursors[this.level];
                for (let to_end = false; ; ) {
                    start: {
                        if (cur.field_bit === -1) {
                            switch (cur.subitem_len) {
                                case 0:
                                    cur.BYTE_S = cur.BYTE_E = cur.origin;
                                    if (0 < cur.meta._2) {
                                        cur.BYTE_E += cur.meta._2 * (cur.subitem_len = 2);
                                        this.state = Flow.STATE.VARINT;
                                        return cur;
                                    }
                                case 2:
                                    if (0 < cur.meta._4) {
                                        cur.BYTE_E += cur.meta._4 * (cur.subitem_len = 4);
                                        this.state = Channel.Flow.STATE.VARINT;
                                        return cur;
                                    }
                                case 4:
                                    if (0 < cur.meta._8) {
                                        cur.BYTE_E += cur.meta._8 * (cur.subitem_len = 8);
                                        this.state = Flow.STATE.VARINT;
                                        return cur;
                                    }
                                case 8:
                                    if (cur.BYTE_S < (cur.BYTE_E = cur.origin + cur.meta.packMinBytes)) {
                                        this.state = Flow.STATE.BYTES;
                                        cur.subitem_len = 1;
                                        return cur;
                                    }
                                    this.mode = Flow.MODE.NONE;
                                default:
                                    if (!cur.meta.fields) {
                                        to_end = true;
                                        break;
                                    }
                                    const fix = cur.BYTE_S;
                                    cur.reset();
                                    if (this.mode === Flow.MODE.NONE && fix < cur.BYTE_E) {
                                        cur.BYTE_S = fix;
                                        this.state = Flow.STATE.BYTES;
                                        this.mode = Flow.MODE.OPTS_INFO;
                                        cur.subitem_len = 1;
                                        return cur;
                                    }
                                    this.mode = Flow.MODE.NONE;
                                    cur.BIT_E = cur.BIT_S = (cur.BYTE_S = cur.BYTE_E) << 3;
                                    if ((delta = cur.first_field_bit()) === -1) to_end = true;
                                    else cur.field_bit = cur.meta.field_0_bit - 1;
                            }
                        } else if ((delta = cur.next_field_bit()) < 0) to_end = true;
                        do {
                            next_field: {
                                if (!to_end)
                                    do
                                        loop: {
                                            let fld = cur.meta.fields[(cur.field_bit += delta + 1) - cur.meta.field_0_bit];
                                            this.state = fld.varint ? Flow.STATE.VARINT : Flow.STATE.BYTES;
                                            cur.BIT_S = cur.BIT_E;
                                            cur.BYTE_S = cur.BYTE_E;
                                            if (0 < fld.length)
                                                if (0 < fld.size) cur.BYTE_E += fld.const_dims_total * fld.length * (cur.subitem_len = fld.size);
                                                else cur.BIT_E += fld.const_dims_total * fld.length * fld.size;
                                            else cur.setBounds(fld);

                                            if (fld.datatype) {
                                                cur = this.cursors[++this.level];
                                                cur.origin = this.cursors[this.level - 1].BYTE_S;
                                                cur.bytes = this.cursors[this.level - 1].bytes;
                                                cur.meta = fld.datatype;
                                                cur.field_bit = -1;
                                                cur.subitem_len = 0;
                                                break start;
                                            }
                                            if (cur.BYTE_S === cur.BYTE_E) {
                                                if ((delta = cur.next_field_bit()) === -1) break loop;
                                                continue;
                                            }
                                            return cur;
                                        }
                                    while (true);
                                else to_end = false;
                                while (0 < this.level) {
                                    if (cur.BYTE_E === this.cursors[this.level - 1].BYTE_E) {
                                        cur = this.cursors[--this.level];

                                        if ((delta = cur.next_field_bit()) === -1) continue;
                                        break next_field;
                                    }

                                    cur.origin = cur.BYTE_E;
                                    cur.field_bit = -1;
                                    cur.subitem_len = 0;
                                    break start;
                                }

                                return null;
                            }
                        } while (true);
                    }
                }
            }
        }

        export namespace Flow {
            export const enum STATE {
                STANDBY,
                PACK_ID,
                VARINT,
                VARINT_BR,
                BYTES,
                BYTES_BR
            }

            export enum MODE {
                OPTS_INFO,
                SET,
                NONE,
                CRC
            }
        }

        export abstract class Transmitter {
            readonly flow: Flow;
            readonly id_bytes: number;
            bits = 0;

            protected abstract pullSendingPack(): Pack | null;

            protected constructor(nested_max: number, id_bytes: number) {
                this.flow = new Flow(nested_max);
                this.id_bytes = id_bytes;
            }

            public event: (reason: string) => void;

            public packs_into_bytes(dst: Uint8Array, byte: number, bytes: number): number {
                const fix = byte;
                for (let cur = this.flow.cursors[this.flow.level]; 0 < bytes--; byte++) {
                    switch (this.flow.state) {
                        case Flow.STATE.STANDBY:
                            const pack = this.pullSendingPack();
                            if (!pack) return byte - fix;

                            cur.wrap(pack);
                            cur.field_bit = 8 * (this.id_bytes - 1);

                            this.flow.state = Flow.STATE.PACK_ID;
                            this.flow.Uvalue = pack.meta.id;
                            this.flow.UvalueH = 0;
                        case Flow.STATE.PACK_ID:
                            dst[byte] = this.flow.Uvalue >>> cur.field_bit;
                            if (-1 < (cur.field_bit -= 8)) continue;

                            cur.field_bit = -1;
                            this.flow.mode = Flow.MODE.NONE;

                            if (cur.meta.fields) {
                                this.flow.state = Flow.STATE.VARINT;
                                cur.setLASTS();
                                this.flow.Uvalue = cur.LAST_BYTE + 1 - (cur.origin + cur.meta.packMinBytes);
                                cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
                                continue;
                            }
                            this.flow.Uvalue = 0;
                            cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;

                            break;
                        case Flow.STATE.BYTES:
                            dst[byte] = cur.bytes.getUint8(cur.BYTE_S++);
                            if (cur.BYTE_S < cur.BYTE_E) continue;
                            break;
                        case Flow.STATE.VARINT:
                            if ((this.flow.Uvalue & ~0x7f) !== 0 || 0 < this.bits) {
                                dst[byte] = (this.flow.Uvalue & 0x7f) | 0x80;
                                this.flow.Uvalue >>>= 7;
                                this.bits -= 7;
                                continue;
                            }

                            if (this.flow.UvalueH) {
                                this.flow.Uvalue |= this.flow.UvalueH << 4;
                                this.flow.UvalueH = 0;

                                if ((this.flow.Uvalue & ~0x7f) !== 0) {
                                    dst[byte] = (this.flow.Uvalue & 0x7f) | 0x80;
                                    this.flow.Uvalue >>>= 7;
                                    continue;
                                }
                            }

                            dst[byte] = this.flow.Uvalue;
                            if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) {
                                this.flow.pullUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                                continue;
                            }
                    }
                    if ((cur = this.flow.next()!)) {
                        if (this.flow.state === Flow.STATE.VARINT) {
                            this.flow.pullUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                            this.bits = this.flow.UvalueH ? 28 : 0;
                        }
                        continue;
                    }
                    this.flow.state = Flow.STATE.STANDBY;
                    this.flow.mode = Flow.MODE.NONE;
                    cur = this.flow.cursors[(this.flow.level = 0)];
                    for (let c of this.flow.cursors) {
                        c.meta = <Pack.Meta>null!;
                        c.bytes = <DataView>null!;
                    }
                }
                return byte - fix;
            }
        }

        export namespace Transmitter {
            export abstract class Advanced extends Transmitter {
                protected constructor(nested_max: number, id_bytes: number) {
                    super(nested_max, id_bytes);
                }

                packs_into_bytes(dst: Uint8Array, BYTE: number, bytes: number): number {
                    const fix = BYTE;
                    let cur = this.flow.cursors[this.flow.level];
                    for (let t; 0 < bytes--; t = this.flow.mode !== Flow.MODE.CRC ? (this.flow.crc = crc16(dst[BYTE], this.flow.crc)) : 0, BYTE++) {
                        switch (this.flow.state) {
                            case Flow.STATE.STANDBY:
                                const pack = this.pullSendingPack();
                                if (!pack) return BYTE - fix;
                                cur.wrap(pack);
                                cur.field_bit = 8 * (this.id_bytes - 1);

                                this.flow.state = Flow.STATE.PACK_ID;
                                this.flow.Uvalue = pack.meta.id;
                                this.flow.UvalueH = 0;
                                this.flow.crc = 0;
                                dst[BYTE] = BR;
                                continue;
                            case Flow.STATE.PACK_ID:
                                dst[BYTE] = this.flow.Uvalue >>> cur.field_bit;
                                if (-1 < (cur.field_bit -= 8)) continue;

                                cur.field_bit = -1;
                                this.flow.mode = Flow.MODE.NONE;

                                if (cur.meta.fields) {
                                    this.flow.state = Flow.STATE.VARINT;

                                    cur.setLASTS();
                                    this.flow.Uvalue = cur.LAST_BYTE + 1 - (cur.origin + cur.meta.packMinBytes);
                                    cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
                                    continue;
                                }

                                this.flow.Uvalue = 0;
                                cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;
                                break;
                            case Flow.STATE.BYTES:
                                if (this.flow.mode === Flow.MODE.CRC) {
                                    switch (cur.subitem_len) {
                                        case 4:
                                            cur.subitem_len = (dst[BYTE] = this.flow.crc >>> 8) === BR ? 3 : 2;
                                            continue;
                                        case 3:
                                            dst[BYTE] = BR;
                                            cur.subitem_len = 2;
                                            continue;
                                        case 2:
                                            if ((dst[BYTE] = this.flow.crc & 0xff) !== BR) break;
                                            cur.subitem_len = 1;
                                            continue;
                                        case 1:
                                            dst[BYTE] = BR;
                                    }
                                    this.flow.state = Flow.STATE.STANDBY;
                                    this.flow.mode = Flow.MODE.NONE;
                                    cur = this.flow.cursors[(this.flow.level = 0)];

                                    for (let c of this.flow.cursors) {
                                        c.meta = <Pack.Meta>null!;
                                        c.bytes = <DataView>null!;
                                    }
                                    continue;
                                }
                                if ((this.flow.Uvalue = dst[BYTE] = cur.bytes.getUint8(cur.BYTE_S++)) === BR) {
                                    this.flow.state = Flow.STATE.BYTES_BR;
                                    continue;
                                }
                            case Flow.STATE.BYTES_BR:
                                this.flow.state = Flow.STATE.BYTES;
                                dst[BYTE] = this.flow.Uvalue;
                                if (cur.BYTE_S < cur.BYTE_E) continue;
                                break;
                            case Flow.STATE.VARINT:
                                if ((this.flow.Uvalue & ~0x7f) !== 0 || 0 < this.bits) {
                                    dst[BYTE] = (this.flow.Uvalue & 0x7f) | 0x80;
                                    this.flow.Uvalue >>>= 7;
                                    this.bits -= 7;
                                    continue;
                                }

                                if (this.flow.UvalueH) {
                                    this.flow.Uvalue |= this.flow.UvalueH << 4;
                                    this.flow.UvalueH = 0;

                                    if ((this.flow.Uvalue & ~0x7f) !== 0) {
                                        dst[BYTE] = (this.flow.Uvalue & 0x7f) | 0x80;
                                        this.flow.Uvalue >>>= 7;
                                        continue;
                                    }
                                }

                                if (this.flow.Uvalue === BR) {
                                    this.flow.state = Flow.STATE.VARINT_BR;
                                    dst[BYTE] = BR;
                                    continue;
                                }
                            case Flow.STATE.VARINT_BR:
                                this.flow.state = Flow.STATE.VARINT;
                                dst[BYTE] = this.flow.Uvalue;
                                if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) {
                                    this.flow.pullUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                                    continue;
                                }
                        }
                        if ((cur = this.flow.next()!)) {
                            if (this.flow.state === Flow.STATE.VARINT) {
                                this.flow.pullUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                                this.bits = this.flow.UvalueH ? 28 : 0;
                            }
                            continue;
                        }
                        cur = this.flow.cursors[(this.flow.level = 0)];
                        this.flow.state = Flow.STATE.BYTES;
                        this.flow.mode = Flow.MODE.CRC;
                        cur.subitem_len = 4;
                    }
                    return BYTE - fix;
                }
            }
        }

        export abstract class Receiver {
            bits = 0;
            time = 0;

            readonly flow: Flow;
            readonly id_bytes: number;

            public event: (reason: string) => void;

            protected constructor(nested_max: number, id_bytes: number) {
                this.flow = new Flow(nested_max);
                this.id_bytes = id_bytes;
            }

            protected abstract dispatch(id: number, pack?: Pack): Pack.Meta | null;

            public bytes_into_packs(src: Uint8Array, byte: number, bytes: number) {
                this.time = ~0;
                if (this.time === 0) {
                    this.time = ~0;
                    this.flow.state = Flow.STATE.STANDBY;

                    this.event?.call('Receive timeout');
                }
                for (let cur = this.flow.cursors[this.flow.level]; 0 < bytes--; byte++) {
                    switch (this.flow.state) {
                        case Flow.STATE.STANDBY:
                            this.flow.Uvalue = 0;
                            this.flow.UvalueH = 0;
                            this.bits = 0;
                            this.flow.state = Flow.STATE.PACK_ID;
                        case Flow.STATE.PACK_ID:
                            this.flow.Uvalue = (this.flow.Uvalue << 8) | src[byte];
                            if (++this.bits < this.id_bytes) continue;
                            this.flow.mode = Flow.MODE.NONE;

                            const meta = this.dispatch(this.flow.Uvalue);
                            if (!meta) {
                                this.event?.call('Unrecognized package ID = ' + this.flow.Uvalue);
                                this.flow.state = Flow.STATE.STANDBY;
                                continue;
                            }

                            cur.meta = meta;
                            this.flow.Uvalue = 0;
                            this.bits = 0;
                            cur.field_bit = -1;
                            cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;

                            if (meta.fields) {
                                this.flow.state = Flow.STATE.VARINT;
                                this.flow.mode = Flow.MODE.OPTS_INFO;
                                continue;
                            } else {
                                cur.bytes = new DataView(new ArrayBuffer(meta.packMinBytes));
                                cur.reset();
                            }

                            break;
                        case Flow.STATE.BYTES:
                            cur.bytes.setUint8(cur.BYTE_S++, src[byte]);
                            if (cur.BYTE_S < cur.BYTE_E) continue;
                            break;
                        case Flow.STATE.VARINT:
                            this.flow.Uvalue |= (src[byte] & 0x7f) << this.bits;
                            this.bits += 7;
                            if ((src[byte] & 0x80) !== 0) {
                                if (this.bits == 28) {
                                    this.flow.UvalueH = this.flow.Uvalue;
                                    this.flow.Uvalue = 0;
                                    this.bits = 0;
                                }
                                continue;
                            }
                            this.bits = 0;
                            if (this.flow.UvalueH)
                                if (this.flow.Uvalue < 0b10000) this.flow.Uvalue = ((this.flow.Uvalue << 28) >>> 0) + this.flow.UvalueH;
                                else {
                                    const tmp = this.flow.Uvalue;
                                    this.flow.Uvalue = (((tmp & 0b1111) << 28) >>> 0) + this.flow.UvalueH;
                                    this.flow.UvalueH = tmp >> 4;
                                }

                            if (this.flow.mode === Flow.MODE.OPTS_INFO) {
                                cur.bytes = new DataView(new ArrayBuffer(cur.meta.packMinBytes + this.flow.Uvalue));
                                this.flow.Uvalue = 0;
                                this.flow.mode = Flow.MODE.NONE;
                                break;
                            }

                            this.flow.pushUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                            this.flow.Uvalue = 0;
                            this.flow.UvalueH = 0;
                            if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) continue;
                            break;
                    }
                    if ((cur = this.flow.next()!)) continue;
                    cur = this.flow.cursors[(this.flow.level = 0)];
                    this.dispatch(cur.meta.id, cur.unwrap());
                    for (let c of this.flow.cursors) {
                        c.meta = <Pack.Meta>null!;
                        c.bytes = <DataView>null!;
                    }
                    this.flow.state = Flow.STATE.STANDBY;
                    this.flow.mode = Flow.MODE.NONE;
                }
            }
        }

        export namespace Receiver {
            export abstract class Advanced extends Receiver {
                protected constructor(nested_max: number, id_bytes: number) {
                    super(nested_max, id_bytes);
                }

                bits = 0;

                public bytes_into_packs(src: Uint8Array, BYTE: number, bytes: number) {
                    this.time = ~0;
                    if (this.time === 0) {
                        this.time = ~0;
                        this.flow.state = Flow.STATE.STANDBY;
                        this.event?.call('Receive timeout');
                    }
                    let cur = this.flow.cursors[this.flow.level];
                    for (let t; 0 < bytes--; t = this.flow.mode !== Flow.MODE.CRC ? (this.flow.crc = crc16(src[BYTE], this.flow.crc)) : 0, BYTE++) {
                        switch (this.flow.state) {
                            case Flow.STATE.STANDBY:
                                this.flow.crc = 0;
                                this.flow.Uvalue = 0;
                                this.flow.UvalueH = 0;
                                this.bits = 0;
                                if (src[BYTE] === BR) this.flow.state = Flow.STATE.PACK_ID;
                                continue;
                            case Flow.STATE.PACK_ID:
                                if (src[BYTE] === BR) {
                                    this.event?.call(' After BR expect helper ID but got +BR');
                                    this.flow.state = Flow.STATE.STANDBY;
                                    continue;
                                }
                                this.flow.Uvalue = (this.flow.Uvalue << 8) | src[BYTE];
                                if (++this.bits < this.id_bytes) continue;
                                this.flow.mode = Flow.MODE.NONE;
                                const meta = this.dispatch(this.flow.Uvalue);
                                if (!meta) {
                                    this.event?.call('Unrecognized package ID = ' + this.flow.Uvalue);
                                    this.flow.state = Flow.STATE.STANDBY;
                                    continue;
                                }

                                cur.meta = meta;
                                this.flow.Uvalue = 0;
                                this.bits = 0;
                                cur.field_bit = -1;
                                cur.BIT_E = cur.BIT_S = cur.BYTE_E = cur.BYTE_S = cur.subitem_len = 0;

                                if (meta.fields) {
                                    this.flow.state = Flow.STATE.VARINT;
                                    this.flow.mode = Flow.MODE.OPTS_INFO;
                                    continue;
                                } else {
                                    cur.bytes = new DataView(new ArrayBuffer(meta.packMinBytes));
                                    cur.reset();
                                }

                                break;
                            case Flow.STATE.BYTES:
                                if (src[BYTE] === BR) {
                                    this.flow.state = Flow.STATE.BYTES_BR;
                                    continue;
                                }
                            case Flow.STATE.BYTES_BR:
                                if (this.flow.state === Flow.STATE.BYTES_BR) {
                                    if (src[BYTE] !== BR) {
                                        this.event?.call('waiting for second BR but got ' + src[BYTE]);
                                        this.flow.state = Flow.STATE.STANDBY;
                                        continue;
                                    }
                                    this.flow.state = Flow.STATE.BYTES;
                                }
                                if (this.flow.mode === Flow.MODE.CRC)
                                    switch (cur.subitem_len) {
                                        case 2:
                                            this.flow.Uvalue = (src[BYTE] & 0xff) << 8;
                                            cur.subitem_len = 1;
                                            continue;
                                        case 1:
                                            if ((this.flow.Uvalue | (src[BYTE] & 0xff)) === this.flow.crc) this.dispatch(cur.meta.id, cur.unwrap());
                                            else this.event?.call('CRC error');
                                            for (let c of this.flow.cursors) {
                                                c.meta = <Pack.Meta>null!;
                                                c.bytes = <DataView>null!;
                                            }
                                            this.flow.state = Flow.STATE.STANDBY;
                                            this.flow.mode = Flow.MODE.NONE;
                                            continue;
                                    }
                                cur.bytes.setUint8(cur.BYTE_S++, src[BYTE]);
                                if (cur.BYTE_S < cur.BYTE_E) continue;
                                break;
                            case Flow.STATE.VARINT:
                                if (src[BYTE] === BR) {
                                    this.flow.state = Flow.STATE.VARINT_BR;
                                    continue;
                                }
                            case Flow.STATE.VARINT_BR:
                                if (this.flow.state === Flow.STATE.VARINT_BR) {
                                    if (src[BYTE] !== BR) {
                                        this.event?.call('waiting for second BR but got ' + src[BYTE]);
                                        this.flow.state = Flow.STATE.STANDBY;
                                        continue;
                                    }
                                    this.flow.state = Flow.STATE.VARINT;
                                }
                                this.flow.Uvalue |= (src[BYTE] & 0x7f) << this.bits;
                                this.bits += 7;

                                if ((src[BYTE] & 0x80) !== 0) {
                                    if (this.bits == 28) {
                                        this.flow.UvalueH = this.flow.Uvalue;
                                        this.flow.Uvalue = 0;
                                        this.bits = 0;
                                    }
                                    continue;
                                }
                                this.bits = 0;
                                if (this.flow.UvalueH)
                                    if (this.flow.Uvalue < 0b10000) this.flow.Uvalue = ((this.flow.Uvalue << 28) >>> 0) + this.flow.UvalueH;
                                    else {
                                        const tmp = this.flow.Uvalue;
                                        this.flow.Uvalue = (((tmp & 0b1111) << 28) >>> 0) + this.flow.UvalueH;
                                        this.flow.UvalueH = tmp >> 4;
                                    }

                                if (this.flow.mode === Flow.MODE.OPTS_INFO) {
                                    cur.bytes = new DataView(new ArrayBuffer(cur.meta.packMinBytes + this.flow.Uvalue));
                                    this.flow.Uvalue = 0;
                                    this.flow.mode = Flow.MODE.NONE;
                                    break;
                                }

                                this.flow.pushUInt(cur.bytes, cur.BYTE_S, cur.subitem_len);
                                this.flow.Uvalue = 0;
                                this.flow.UvalueH = 0;
                                if ((cur.BYTE_S += cur.subitem_len) < cur.BYTE_E) continue;
                                break;
                        }
                        if ((cur = this.flow.next()!)) continue;
                        cur = this.flow.cursors[(this.flow.level = 0)];
                        this.flow.state = Flow.STATE.BYTES;
                        this.flow.mode = Flow.MODE.CRC;
                        cur.subitem_len = 2;
                    }
                }
            }
        }
    }
}
