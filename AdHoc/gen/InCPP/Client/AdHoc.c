
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
#include "AdHoc.h"

#define BR (0x55)
const uint8_t Ol[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
const uint8_t lO[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};


static inline uint16_t crc16(uint8_t byte, uint16_t crc)
{
    static const uint16_t tab[16] = {0, 4129, 8258, 12387, 16516, 20645, 24774, 28903, 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935};
    crc                           = tab[(crc >> 12 ^ byte >> 4) & 0x0F] ^ crc << 4;
    return tab[(crc >> 12 ^ byte & 0x0F) & 0x0F] ^ crc << 4;
}

Pack* pack_alloc(const Meta* meta, Pack* pack, size_t len)
{
    if(pack) return pack->meta->pack_alloc ? pack->meta->pack_alloc(pack, len) : len == SIZE_MAX ? free(pack), (Pack*)NULL : (Pack*)realloc(pack, len);
    if(meta->pack_alloc) return meta->pack_alloc(NULL, len);
    uint8_t* ret = len == SIZE_MAX ? (uint8_t*)calloc(sizeof(Pack) + meta->packMinBytes, 1) : (uint8_t*)malloc(sizeof(Pack) + meta->packMinBytes + len);
    return ret ? *(Meta**)ret = (Meta*)meta, (Pack*)ret : (Pack*)NULL;
}

static inline void set_0(uint8_t* dst, size_t bit, size_t bits)
{
    dst += bit >> 3;
    bit &= 7;
    if(8 < bit + bits)
    {
        if(bit)
        {
            *dst &= Ol[bit];
            if(!(bits -= 8 - bit)) return;
            dst++;
        }
        if(bits & 7) dst[bits >> 3] &= lO[8 - (bits & 7)];
        if(!(bits >>= 3)) return;
        for(uint16_t i = bits % sizeof(UMAX); 0 < i--; *dst = 0, dst++);
        for(UMAX* umax = (UMAX*)dst + (bits / sizeof(UMAX) - 1); (UMAX*)dst <= umax; umax--) *umax = 0;
    }
    else *dst &= Ol[bit] | lO[8 - (bit + bits)];
}
static inline void copy_bits(const uint8_t* src, size_t src_bit, size_t bits, uint8_t* dst, size_t dst_bit)
{
    if(bits < 1 || src == dst && src_bit == dst_bit) return;
    int32_t count = (bits >> 3) / sizeof(UMAX);
    const uint8_t b = sizeof(UMAX) * 8;
    bits %= b;
    if(src == dst && src_bit < dst_bit)
    {
        src_bit += count * b;
        dst_bit += count * b;
        if(0 < bits) set_bits(get_bits(src, src_bit, bits), bits, dst, dst_bit);
        for(; 0 < count--; src_bit -= b, dst_bit -= b, set_bits(get_bits(src, src_bit, b), b, dst, dst_bit));
    }
    else
    {
        for(; 0 < count; set_bits(get_bits(src, src_bit, b), b, dst, dst_bit), src_bit += b, dst_bit += b, count--);
        if(0 < bits) set_bits(get_bits(src, src_bit, bits), bits, dst, dst_bit);
    }
}

static inline int32_t first_1(const uint8_t* bytes, size_t bit, size_t bits, bool existence)
{
    int32_t _1BYTE = bit >> 3;
    int32_t v      = bytes[_1BYTE] & 0xFF;
    bit &= 7;
    if(bits == 1) return (v & 1 << bit) == 0 ? -1 : 0;
    int32_t add = 0;
    if(0 < bit)
    {
        if(0 < (v >>= bit))
        {
            if(bit + bits < 8 && (v & Ol[bits]) == 0) return -1;
            goto s;
        }
        if(bit + bits < 8) return -1;
        bits -= add = 8 - bit;
        _1BYTE++;
    }
    else if(bits < 9)
    {
        if(v == 0 || (v & Ol[bits]) == 0) return -1;
        goto s;
    }
    {
        const int32_t last = _1BYTE + (bits >> 3);
        for(int32_t BYTE = _1BYTE; BYTE < last; BYTE++)
            if(0 < (v = bytes[BYTE] & 0xFF))
            {
                add += BYTE - _1BYTE << 3;
                goto s;
            }
        if((bits &= 7) == 0 || (v = bytes[last] & 0xFF & Ol[bits]) == 0) return -1;
        add += last - _1BYTE << 3;
    }
s:
    if(existence) return 0x7fffffff;
    for(int32_t i = 0;; i++) if((v >> i & 1) == 1) return add + i;
}

static inline int32_t first_field_bit(CursorBase* cur) { return first_1(cur->bytes, cur->meta->field_0_bit, cur->BIT_E - cur->meta->field_0_bit, false); }

static inline int32_t next_field_bit(CursorBase* cur)
{
    return cur->field_bit - cur->meta->field_0_bit < cur->meta->fields_count - 1 ? first_1(cur->bytes, cur->field_bit + 1, cur->BIT_E - (cur->field_bit + 1), false) : -1;
}


static inline bool has_opt_fields(Meta const* const meta) { return meta->fields_count; }

bool reset_cursor(Cursor* const cur)
{
    Meta const* const meta = cur->base.meta;
    cur->base.field_bit    = -1;
    cur->base.BYTE_E       = cur->base.BYTE_S = cur->BYTE = meta->packMinBytes + (meta->BITS_lenINbytes_bits == 0 ? 0 : get_bits(cur->base.bytes, meta->field_0_bit - meta->BITS_lenINbytes_bits, meta->BITS_lenINbytes_bits));
    cur->base.BIT_E        = cur->base.BIT_S  = cur->BIT  = cur->BYTE << 3;
    cur->base.subitem_len  = 0;
    if(has_opt_fields(meta))
        if(first_field_bit(&cur->base) < 0)
        {
            cur->base.LAST_BYTE = cur->base.BYTE_E;
            cur->base.LAST_BIT  = cur->base.BIT_E;
        }
        else cur->base.LAST_BYTE = cur->base.LAST_BIT = -1;
    else cur->base.LAST_BIT = (cur->base.LAST_BYTE = meta->packMinBytes) * 8;
    cur->base.LAST_field_bit = -1;
    cur->item_len            = 0;
    cur->field_item_0        = -1;
    cur->field_item          = 0x7fffffff;
    cur->field_items         = 0;
    return true;
}

static void set_bounds(const Field* fld, CursorBase* cur)
{
    uint8_t* bytes   = cur->bytes;
    cur->subitem_len = fld->size;
    int32_t bit      = cur->BIT_E;
    int32_t count    = 1;
    switch(fld->type)
    {
        case 1:
        {
            if(fld->var_dims_count) for(size_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) count *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BIT_E -= fld->field_info_bits;
            cur->BYTE_E += fld->const_dims_total * count * -fld->length * fld->size;
            break;
        }
        case 3:
        {
            cur->BIT_E -= fld->field_info_bits;
            int32_t arrays_length = (int32_t)get_bits(bytes, cur->BIT_E, -fld->length);
            if(fld->var_dims_count) for(int32_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) arrays_length *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BYTE_E += fld->const_dims_total * arrays_length * fld->size;
            break;
        }
        case 5:
        {
            count = fld->const_dims_total;
            if(fld->var_dims_count) for(int32_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) count *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BIT_E -= fld->field_info_bits;
            int32_t all_arrays_sum = 0;
            while(0 < count--) all_arrays_sum += get_bits(bytes, cur->BIT_E -= -fld->length, -fld->length);
            cur->BYTE_E += all_arrays_sum * fld->size;
            break;
        }
        case 7:
        {
            cur->subitem_len = 0;
            if(fld->var_dims_count) for(int32_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) count *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BIT_E -= fld->field_info_bits + count * fld->const_dims_total * -fld->length * fld->size;
            break;
        }
        case 9:
        {
            cur->BIT_E -= fld->field_info_bits;
            int32_t arrays_length = (int32_t)get_bits(bytes, cur->BIT_E, -fld->length);
            if(fld->var_dims_count) for(int32_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) arrays_length *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BIT_E -= fld->const_dims_total * arrays_length * fld->size;
            break;
        }
        case 11:
        {
            count = fld->const_dims_total;
            if(fld->var_dims_count) for(int32_t i = 0; i < fld->dims_count; i++) if(fld->dims[i] < 0) count *= get_bits(bytes, bit -= -fld->dims[i], -fld->dims[i]);
            cur->BIT_E -= fld->field_info_bits;
            while(0 < count--) cur->BIT_E -= (int32_t)get_bits(bytes, cur->BIT_E -= -fld->length, -fld->length) * fld->size;
            break;
        }
        default:
            if(!(count = (int32_t)get_bits(bytes, cur->BIT_E -= fld->field_info_bits, fld->sparse_bits))) return;
            bit = cur->BIT_E;
            switch(fld->type)
            {
                case 2:
                {
                    cur->BIT_E -= count;
                    while(cur->BIT_E < bit--) if((bytes[bit >> 3] & 1 << (bit & 7)) == 0) count--;
                    cur->BYTE_E += -fld->length * count * fld->size;
                    break;
                }
                case 4:
                {
                    cur->BIT_E -= count;
                    int32_t arrays_length = (int32_t)get_bits(bytes, bit + 2 * fld->sparse_bits, -fld->length);
                    while(cur->BIT_E < bit--) if((bytes[bit >> 3] & 1 << (bit & 7)) == 0) count--;
                    cur->BYTE_E += count * arrays_length * fld->size;
                    break;
                }
                case 6:
                {
                    int32_t all_arrays_sum = 0;
                    while(0 < count--) if((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) all_arrays_sum += get_bits(bytes, bit -= -fld->length, -fld->length);
                    cur->BYTE_E += all_arrays_sum * fld->size;
                    cur->BIT_E = bit;
                    break;
                }
                case 8:
                {
                    for(const int32_t bits = -fld->length * fld->size; 0 < count--;) if((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) bit -= bits;
                    cur->BIT_E = bit;
                    break;
                }
                case 10:
                {
                    for(const int32_t bits = (int32_t)get_bits(bytes, bit + 2 * fld->sparse_bits, -fld->length) * fld->size; 0 < count--;) if((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) bit -= bits;
                    cur->BIT_E = bit;
                    break;
                }
                case 12:
                {
                    while(0 < count--) if((bytes[--bit >> 3] & 1 << (bit & 7)) != 0) bit -= (int32_t)get_bits(bytes, bit -= -fld->length, -fld->length) * fld->size;
                    cur->BIT_E = bit;
                    break;
                }
                default:
                    ;
            }
    }
}

static void init(Field const* fld, Cursor* curX)
{
    CursorBase*    cur   = &curX->base;
    const uint8_t* bytes = cur->bytes;
    switch(fld->type)
    {
        case 1:
            curX->item_len = -fld->length;
            curX->BIT = cur->BIT_E;
            break;
        case 3:
            curX->item_len = (int32_t)get_bits(bytes, curX->BIT = cur->BIT_S - fld->field_info_bits, -fld->length);
            break;
        case 5:
        case 11:
            curX->item_len = 0;
            curX->BIT          = cur->BIT_S - fld->field_info_bits;
            curX->field_item_0 = 0;
            curX->field_item   = 0x7fffffff;
            curX->field_items  = curX->field_items_total;
            break;
        case 7:
            curX->item_len = -fld->length;
            curX->field_item_0 = 0;
            curX->BIT          = cur->BIT_E;
            curX->field_item   = (curX->field_items = curX->field_items_total) == 0 ? 0x7fffffff : 0;
            break;
        case 9:
            curX->item_len = (int32_t)get_bits(bytes, curX->BIT = cur->BIT_S - fld->field_info_bits, -fld->length);
            curX->field_item_0 = 0;
            curX->BIT          = cur->BIT_E;
            curX->field_item   = (curX->field_items = curX->field_items_total) == 0 ? 0x7fffffff : 0;
            break;
        default:
            curX->BIT = cur->BIT_S - fld->field_info_bits;
            switch(fld->type)
            {
                case 2:
                case 6:
                case 8:
                case 12:
                    curX->item_len = -fld->length;
                    break;
                case 4:
                case 10:
                    curX->item_len = (int32_t)get_bits(bytes, curX->BIT + 2 * fld->sparse_bits, -fld->length);
                    break;
            }
            curX->field_item   = 0x7fffffff;
            curX->field_items  = (int32_t)get_bits(bytes, curX->BIT, fld->sparse_bits);
            curX->field_item_0 = (int32_t)get_bits(bytes, curX->BIT + fld->sparse_bits, fld->sparse_bits);
    }
    curX->BYTE = cur->BYTE_S;
}

static inline void _reset_cursor(CursorBase* cur)
{
    Meta const* const meta = cur->meta;
    cur->field_bit         = -1;
    cur->BYTE_E            = cur->BYTE_S = meta->packMinBytes + (meta->BITS_lenINbytes_bits == 0 ? 0 : (uint32_t)get_bits(cur->bytes, meta->field_0_bit - meta->BITS_lenINbytes_bits, meta->BITS_lenINbytes_bits));
    cur->BIT_E             = cur->BIT_S  = cur->BYTE_E << 3;
    cur->subitem_len       = 0;
    if(has_opt_fields(meta))
        if(first_field_bit(cur) < 0)
        {
            cur->LAST_BYTE = cur->BYTE_E;
            cur->LAST_BIT  = cur->BIT_E;
        }
        else cur->LAST_BYTE = cur->LAST_BIT = -1;
    else cur->LAST_BIT = (cur->LAST_BYTE = meta->packMinBytes) * 8;
    cur->LAST_field_bit = -1;
}

size_t pack_size(CursorBase* const cur)
{
    const Meta* const meta  = cur->meta;
    int32_t           delta = cur->field_bit == -1 ? first_field_bit(cur) : next_field_bit(cur);
    if(delta == -1)
    {
        cur->LAST_BYTE      = cur->BYTE_E;
        cur->LAST_field_bit = cur->field_bit;
        cur->LAST_BIT       = cur->BIT_E;
        return cur->LAST_BYTE;
    }
    CursorBase tmp;
    tmp                            = *cur;
    const int32_t meta_field_0_bit = meta->field_0_bit;
    if(cur->field_bit < 0) tmp.field_bit = meta_field_0_bit - 1;
    do
    {
        tmp.field_bit += delta + 1;
        tmp.BIT_S        = tmp.BIT_E;
        tmp.BYTE_S       = tmp.BYTE_E;
        Field const* fld = meta->fields[tmp.field_bit - meta_field_0_bit];
        if(0 < fld->length)
            if(0 < fld->size) tmp.BYTE_E += fld->const_dims_total * fld->length * fld->size;
            else tmp.BIT_E += fld->const_dims_total * fld->length * fld->size;
        else set_bounds(fld, &tmp);
    }
    while(-1 < (delta = next_field_bit(&tmp)));
    cur->LAST_BYTE      = tmp.BYTE_E;
    cur->LAST_BIT       = tmp.BIT_E;
    cur->LAST_field_bit = tmp.field_bit;
    return cur->LAST_BYTE;
}
void resize_bytes(Cursor cur[], const int32_t diff)
{
    Cursor* _init = cur;
    while(!cur->base.pack) cur--;
    const size_t   BYTE  = _init->base.bytes - cur->base.bytes + _init->BYTE;
    Pack*          pack  = cur->base.pack;
    uint8_t const* bytes = pack->bytes;
    if(cur->base.LAST_BYTE < 0) pack_size(&cur->base);
    const size_t new_size = sizeof(Pack) + cur->base.LAST_BYTE + diff;
    if(0 < diff)
    {
        pack = pack_alloc(NULL, pack, new_size);
        memcpy(pack->bytes + BYTE + diff, pack->bytes + BYTE, cur->base.LAST_BYTE - BYTE);
        memset(pack->bytes + BYTE, 0, diff);
    }
    else
    {
        memcpy(pack->bytes + BYTE, pack->bytes + BYTE - diff, cur->base.LAST_BYTE - (BYTE - diff));
        pack = pack_alloc(NULL, pack, new_size);
    }
    for(const int32_t shift = (cur->base.pack = pack)->bytes - bytes;;)
    {
        cur->base.bytes += shift;
        cur->base.BYTE_E += diff;
        if(-1 < cur->base.LAST_BYTE) cur->base.LAST_BYTE += diff;
        if(cur == _init) return;
        if(-1 < cur->base.field_bit)
        {
            Field const*  fld    = get_field(&cur->base);
            const int32_t length = fld->length;
            if(length < 0) set_bits(cur->item_len += diff, -length, cur->base.bytes, cur->BIT);
        }
        cur++;
    }
}

static inline uint16_t bits2bytes(int32_t bits) { return bits < 1 ? 0 : 1 + ((bits - 1) >> 3); }
static void            insert(Cursor curX[], const int32_t fbit, const int32_t bits, const int32_t bytes)
{
    CursorBase* cur = &curX->base;
    if(cur->field_bit != fbit)
    {
        curX->BIT      = cur->BIT_S  = cur->BIT_E;
        curX->BYTE     = cur->BYTE_S = cur->BYTE_E;
        curX->item_len = 0;
    }
    Meta const*  meta                = cur->meta;
    const size_t old_pack_data_bytes = cur->LAST_BYTE < 0 ? pack_size(cur), cur->LAST_BYTE : cur->LAST_BYTE;
    int32_t      add_to_bits_bytes;
    if(curX->BIT == cur->LAST_BIT && curX->BYTE == old_pack_data_bytes)
    {
        add_to_bits_bytes = meta->BITS_lenINbytes_bits == 0 ? 0 : bits2bytes(fbit + bits - cur->LAST_BIT + 1);
        if(0 < add_to_bits_bytes || 0 < bytes)
        {
            resize_bytes(curX, add_to_bits_bytes + bytes);
            if(add_to_bits_bytes)
            {
                copy_bits(cur->bytes, curX->BIT,
                          (old_pack_data_bytes << 3) - curX->BIT, cur->bytes, curX->BIT + (add_to_bits_bytes << 3));
                set_0(cur->bytes, curX->BIT, add_to_bits_bytes << 3);
            }
        }
    }
    else
    {
        const int32_t add_bits_bits = meta->BITS_lenINbytes_bits == 0 ? 0 : bits - (cur->LAST_BIT - (cur->LAST_field_bit + 1));
        add_to_bits_bytes = meta->BITS_lenINbytes_bits == 0 ? 0 : bits2bytes(add_bits_bits);
        if(0 < add_to_bits_bytes || 0 < bytes)
        {
            resize_bytes(curX, add_to_bits_bytes + bytes);
            if(bits)
            {
                copy_bits(cur->bytes, curX->BIT, (curX->BYTE << 3) - curX->BIT, cur->bytes, curX->BIT + (add_to_bits_bytes << 3));
                copy_bits(cur->bytes, cur->LAST_BIT,
                          curX->BIT - cur->LAST_BIT, cur->bytes, cur->LAST_BIT + (add_to_bits_bytes << 3) - bits);
                if(0 < (add_to_bits_bytes << 3) - bits)
                    set_0(cur->bytes, cur->LAST_BIT, (add_to_bits_bytes << 3) - bits);
                set_0(cur->bytes, curX->BIT + (add_to_bits_bytes << 3) - bits, bits);
            }
        }
        else
        {
            copy_bits(cur->bytes, cur->LAST_BIT,
                      curX->BIT - cur->LAST_BIT, cur->bytes, cur->LAST_BIT - bits);
            set_0(cur->bytes, curX->BIT - bits, bits);
        }
    }
    if(0 < meta->BITS_lenINbytes_bits && 0 < add_to_bits_bytes)
    {
        const int32_t old_value = (int32_t)get_bits(cur->bytes, meta->field_0_bit - meta->BITS_lenINbytes_bits, meta->BITS_lenINbytes_bits);
        set_bits(old_value + add_to_bits_bytes, meta->BITS_lenINbytes_bits, cur->bytes, meta->field_0_bit - meta->BITS_lenINbytes_bits);
    }
    const int32_t zazor_delta = (add_to_bits_bytes << 3) - bits;
    cur->LAST_BIT += zazor_delta;
    curX->BIT += add_to_bits_bytes << 3;
    curX->BYTE += add_to_bits_bytes;
    if(fbit == cur->field_bit)
    {
        cur->BIT_E += zazor_delta;
        cur->BIT_S += add_to_bits_bytes << 3;
        cur->BYTE_S += add_to_bits_bytes;
    }
    else
    {
        cur->BIT_S  = cur->BIT_E  = curX->BIT;
        cur->BYTE_E = cur->BYTE_S = curX->BYTE;
        cur->bytes[fbit >> 3] |= 1 << (fbit & 7);
        if(cur->LAST_field_bit < fbit) cur->LAST_field_bit = fbit;
        cur->field_bit = fbit;
    }
}


static inline CursorBase* _get_cursor(Flow* const flow) { return &flow->cursors[flow->level]; }

static bool next(Flow* const flow, CursorBase** const cur, uint8_t** bytes)
{
start:
    ;
    int32_t delta;
    if((*cur)->field_bit == -1)
    {
        switch((*cur)->subitem_len)
        {
            case 0:
                (*cur)->BYTE_S = (*cur)->BYTE_E = 0;
                if(0 < (*cur)->meta->_2)
                {
                    (*cur)->BYTE_E += (*cur)->meta->_2 * ((*cur)->subitem_len = 2);
                    flow->state                                               = VARINT;
                    return true;
                }
            case 2:
                if(0 < (*cur)->meta->_4)
                {
                    (*cur)->BYTE_E += (*cur)->meta->_4 * ((*cur)->subitem_len = 4);
                    flow->state                                               = VARINT;
                    return true;
                }
            case 4:
                if(0 < (*cur)->meta->_8)
                {
                    (*cur)->BYTE_E += (*cur)->meta->_8 * ((*cur)->subitem_len = 8);
                    flow->state                                               = VARINT;
                    return true;
                }
            case 8:
                if((*cur)->BYTE_S < ((*cur)->BYTE_E = (*cur)->meta->packMinBytes))
                {
                    flow->state         = BYTES;
                    (*cur)->subitem_len = 1;
                    return true;
                }
                flow->mode = NIL;
            default:
                if(!has_opt_fields((*cur)->meta)) goto end;
                const int32_t fix = (*cur)->BYTE_S;
                _reset_cursor(*cur);
                if(flow->mode == NIL && fix < (*cur)->BYTE_E)
                {
                    (*cur)->BYTE_S      = fix;
                    flow->state         = BYTES;
                    flow->mode          = OPTS_INFO;
                    (*cur)->subitem_len = 1;
                    return true;
                }
                flow->mode    = NIL;
                (*cur)->BIT_E = (*cur)->BIT_S = ((*cur)->BYTE_S = (*cur)->BYTE_E) << 3;
                if((delta = first_field_bit(*cur)) == -1) goto end;
                (*cur)->field_bit = (*cur)->meta->field_0_bit - 1;
        }
    }
    else if((delta = next_field_bit(*cur)) == -1) goto end;
next_field:
    do
    {
        Field const* fld = (*cur)->meta->fields[((*cur)->field_bit += delta + 1) - (*cur)->meta->field_0_bit];
        flow->state      = fld->varint ? VARINT : BYTES;
        (*cur)->BIT_S    = (*cur)->BIT_E;
        (*cur)->BYTE_S   = (*cur)->BYTE_E;
        if(0 < fld->length)
            if(0 < fld->size)(*cur)->BYTE_E += fld->const_dims_total * fld->length * ((*cur)->subitem_len = fld->size);
            else(*cur)->BIT_E += fld->const_dims_total * fld->length * fld->size;
        else set_bounds(fld, *cur);
        if(fld->datatype)
        {
            *bytes = (*cur)->bytes + (*cur)->BYTE_S;
            flow->level++;
            (*cur)++;
            (*cur)->meta        = fld->datatype;
            (*cur)->bytes       = *bytes;
            (*cur)->field_bit   = -1;
            (*cur)->subitem_len = 0;
            goto start;
        }
        if((*cur)->BYTE_S == (*cur)->BYTE_E)
        {
            if((delta = next_field_bit(*cur)) == -1) goto end;
            continue;
        }
        return true;
    }
    while(true);
end:
    while(flow->level)
    {
        if((*cur)->bytes + (*cur)->BYTE_E == (*cur - 1)->bytes + (*cur - 1)->BYTE_E)
        {
            flow->level--;
            (*cur)--;
            *bytes = (*cur)->bytes;
            if((delta = next_field_bit(*cur)) == -1) continue;
            goto next_field;
        }
        (*cur)->bytes       = *bytes += (*cur)->BYTE_E;
        (*cur)->field_bit   = -1;
        (*cur)->subitem_len = 0;
        goto start;
    }
    return false;
}


void receive_adv(const uint8_t* src, size_t src_bytes, Receiver* const dst)
{
    Flow*       flow = &dst->flow;
    CursorBase* cur  = _get_cursor(flow);
    dst->time        = ~(uint32_t)0;
    if(dst->time == 0)
    {
        dst->time   = ~0;
        flow->state = STANDBY;
        if(dst->event) dst->event(__LINE__, dst, flow->cursors[0].pack, AD_HOC_FAILURE_RECEIVE_TIMEOUT);
        free_pack(flow->cursors[0].pack);
        flow->cursors[0].pack = NULL;
    }
    uint8_t* bytes = cur->bytes;
    for(; src_bytes--; flow->mode != DOCRC ? flow->crc = crc16(*src, flow->crc) : 0, src++)
    {
        switch(flow->state)
        {
            case STANDBY:
                flow->crc = 0;
                flow->Uvalue = 0;
                dst->bits    = 0;
                if(*src == BR) flow->state = PACK_ID;
                continue;
            case PACK_ID:
                if(*src == BR)
                {
                    if(dst->event) dst->event(__LINE__, dst, NULL, AD_HOC_FAILURE_RECEIVE_EXPECT_ID_AFTER_BR_BUT_GET_BR);
                    flow->state = STANDBY;
                    continue;
                }
                flow->Uvalue = flow->Uvalue << 8 | *src;
                if(++dst->bits < AD_HOC_ID_BYTES) continue;
                flow->mode  = NIL;
                flow->level = 0;
                cur         = &flow->cursors[0];
                if((cur->meta = dst->dispatch(dst, flow->Uvalue, NULL)) == NULL)
                {
                    if(dst->event) dst->event(__LINE__, dst, NULL, AD_HOC_FAILURE_RECEIVE_PACK_WITH_UNEXPECTED_ID);
                    flow->state  = STANDBY;
                    flow->Uvalue = 0;
                    continue;
                }
                flow->Uvalue   = 0;
                dst->bits      = 0;
                cur->field_bit = -1;
                cur->BIT_E     = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                if(has_opt_fields(cur->meta))
                {
                    flow->state = VARINT;
                    flow->mode  = OPTS_INFO;
                    continue;
                }
                cur->pack = pack_alloc(cur->meta, NULL, 0);
                bytes     = cur->bytes = cur->pack->bytes;
                break;
            case BYTES:
                if(*src != BR) goto NOT_BR;
                flow->state = BYTES_BR;
                continue;
            case BYTES_BR:
                if(*src != BR)
                {
                    if(dst->event) dst->event(__LINE__, dst, flow->cursors[0].pack, AD_HOC_FAILURE_RECEIVE_EXPECT_SECOND_BR_BUT_GET);
                    free_pack(flow->cursors[0].pack);
                    flow->cursors[0].pack = NULL;
                    flow->state           = STANDBY;
                    continue;
                }
                flow->state = BYTES;
NOT_BR:
                if(flow->mode == DOCRC)
                    switch(cur->subitem_len)
                    {
                        case 2:
                            flow->Uvalue = (*src & (UMAX)0xFF) << 8;
                            cur->subitem_len = 1;
                            continue;
                        case 1:
                            if((flow->Uvalue | *src) == flow->crc) dst->dispatch(dst, flow->cursors[0].pack->meta->id, flow->cursors[0].pack);
                            else if(dst->event) dst->event(__LINE__, dst, flow->cursors[0].pack, AD_HOC_FAILURE_RECEIVE_CRC_ERROR);
                            free_pack(flow->cursors[0].pack);
                            flow->cursors[0].pack = NULL;
                            flow->state           = STANDBY;
                            flow->mode            = NIL;
                            continue;
                    }
                bytes[cur->BYTE_S++] = *src;
                if(cur->BYTE_S < cur->BYTE_E) continue;
                break;
            case VARINT:
                if(*src != BR) goto NOT_BR_;
                flow->state = VARINT_BR;
                continue;
            case VARINT_BR:
                if(*src != BR)
                {
                    if(dst->event) dst->event(__LINE__, dst, flow->cursors[0].pack, AD_HOC_FAILURE_RECEIVE_EXPECT_SECOND_BR_BUT_GET);
                    free_pack(flow->cursors[0].pack);
                    flow->cursors[0].pack = NULL;
                    flow->state           = STANDBY;
                    continue;
                }
                flow->state = VARINT;
NOT_BR_:
                flow->Uvalue |= (*src & (UMAX)0x7F) << dst->bits;
                dst->bits += 7;
                if((*src & 0x80) != 0) continue;
                dst->bits = 0;
                if(flow->mode == OPTS_INFO)
                {
                    cur->pack    = pack_alloc(cur->meta, NULL, flow->Uvalue);
                    bytes        = cur->bytes = cur->pack->bytes;
                    flow->Uvalue = 0;
                    flow->mode   = NIL;
                    break;
                }
                set_bytes(flow->Uvalue, cur->subitem_len, bytes, cur->BYTE_S);
                flow->Uvalue = 0;
                if((cur->BYTE_S += cur->subitem_len) < cur->BYTE_E) continue;
                break;
        }
        if(next(flow, &cur, &bytes)) continue;
        flow->state      = BYTES;
        flow->mode       = DOCRC;
        cur->subitem_len = 2;
    }
}

uint32_t transmit_adv(Transmitter* const src, uint8_t* dst, size_t dst_bytes)
{
    Flow*          flow  = &src->flow;
    CursorBase*    cur   = _get_cursor(flow);
    const uint8_t* fix   = dst;
    uint8_t*       bytes = cur->bytes;
    for(; dst_bytes--; flow->mode != DOCRC ? flow->crc = crc16(*dst, flow->crc) : 0, dst++)
    {
        switch(flow->state)
        {
            case STANDBY:
                flow->level = 0;
                cur = &flow->cursors[0];
                if((cur->pack = (Pack*)src->pull(src)) == NULL) return dst - fix;
                bytes          = cur->bytes = cur->pack->bytes;
                cur->meta      = cur->pack->meta;
                flow->state    = PACK_ID;
                flow->Uvalue   = cur->meta->id;
                cur->field_bit = 8 * (AD_HOC_ID_BYTES - 1);
                flow->crc      = 0;
                *dst           = BR;
                continue;
            case PACK_ID:
                *dst = (uint8_t)(flow->Uvalue >> cur->field_bit);
                if(-1 < (cur->field_bit -= 8)) continue;
                flow->Uvalue   = 0;
                cur->field_bit = -1;
                cur->BIT_E     = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                flow->mode     = NIL;
                if(has_opt_fields(cur->meta))
                {
                    flow->state = VARINT;
                    _reset_cursor(cur);
                    pack_size(cur);
                    flow->Uvalue = cur->LAST_BYTE + 1 - cur->meta->packMinBytes;
                    cur->BIT_E = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                    continue;
                }
                break;
            case BYTES:
                if(flow->mode == DOCRC)
                {
                    switch(cur->subitem_len)
                    {
                        case 4:
                            cur->subitem_len = (*dst = flow->crc >> 8) == BR ? 3 : 2;
                            continue;
                        case 3:
                            *dst = BR;
                            cur->subitem_len = 2;
                            continue;
                        case 2:
                            if((*dst = flow->crc & 0xFF) != BR) break;
                            cur->subitem_len = 1;
                            continue;
                        case 1:
                            *dst = BR;
                    }
                    flow->state = STANDBY;
                    flow->mode = NIL;
                    if(src->event) src->event(__LINE__, src, flow->cursors[0].pack, AFTER_SENDING);
                    free_pack(flow->cursors[0].pack);
                    flow->cursors[0].pack = NULL;
                    continue;
                }
                if((*dst = bytes[cur->BYTE_S++]) != BR) goto NOT_BR;
                flow->state = BYTES_BR;
                continue;
            case BYTES_BR:
                flow->state = BYTES;
                *dst = BR;
NOT_BR:
                if(cur->BYTE_S < cur->BYTE_E) continue;
                break;
            case VARINT:
                if(flow->Uvalue & ~(UMAX)0x7F)
                {
                    *dst = (uint8_t)(flow->Uvalue & 0x7F | 0x80);
                    flow->Uvalue >>= 7;
                    continue;
                }
                if(flow->Uvalue == BR)
                {
                    flow->state = VARINT_BR;
                    *dst        = BR;
                    continue;
                }
                *dst = flow->Uvalue & (UMAX)0xFF;
                goto NOT_BR_;
            case VARINT_BR:
                flow->state = VARINT;
                *dst = BR;
NOT_BR_:
                if((cur->BYTE_S += cur->subitem_len) < cur->BYTE_E)
                {
                    flow->Uvalue = get_bytes(bytes, cur->BYTE_S, cur->subitem_len);
                    continue;
                }
        }
        if(next(flow, &cur, &bytes))
        {
            if(flow->state == VARINT) flow->Uvalue = get_bytes(bytes, cur->BYTE_S, cur->subitem_len);
            continue;
        }
        flow->state      = BYTES;
        flow->mode       = DOCRC;
        cur->subitem_len = 4;
    }
    return dst - fix;
}


void receive(const uint8_t* src, size_t src_bytes, Receiver* const dst)
{
    Flow*       flow = &dst->flow;
    CursorBase* cur  = _get_cursor(flow);
    dst->time        = ~(uint32_t)0;
    if(dst->time == 0)
    {
        dst->time   = ~0;
        flow->state = STANDBY;
        if(dst->event) dst->event(__LINE__, dst, flow->cursors[0].pack, AD_HOC_FAILURE_RECEIVE_TIMEOUT);
        free_pack(flow->cursors[0].pack);
        flow->cursors[0].pack = NULL;
    }
    uint8_t* bytes = cur->bytes;
    for(; src_bytes--; src++)
    {
        switch(flow->state)
        {
            case STANDBY:
                flow->Uvalue = 0;
                dst->bits   = 0;
                flow->state = PACK_ID;
            case PACK_ID:
                flow->Uvalue = flow->Uvalue << 8 | *src;
                if(++dst->bits < AD_HOC_ID_BYTES) continue;
                flow->mode  = NIL;
                flow->level = 0;
                cur         = &flow->cursors[0];
                if((cur->meta = dst->dispatch(dst, flow->Uvalue, NULL)) == NULL)
                {
                    if(dst->event) dst->event(__LINE__, dst, NULL, AD_HOC_FAILURE_RECEIVE_PACK_WITH_UNEXPECTED_ID);
                    flow->state = STANDBY;
                    continue;
                }
                flow->Uvalue   = 0;
                dst->bits      = 0;
                cur->field_bit = -1;
                cur->BIT_E     = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                if(has_opt_fields(cur->meta))
                {
                    flow->state = VARINT;
                    flow->mode  = OPTS_INFO;
                    continue;
                }
                cur->pack = pack_alloc(cur->meta, NULL, 0);
                bytes     = cur->bytes = cur->pack->bytes;
                break;
            case BYTES:
                bytes[cur->BYTE_S++] = *src;
                if(cur->BYTE_S < cur->BYTE_E) continue;
                break;
            case VARINT:
                flow->Uvalue |= (*src & (UMAX)0x7F) << dst->bits;
                dst->bits += 7;
                if((*src & 0x80) != 0) continue;
                dst->bits = 0;
                if(flow->mode == OPTS_INFO)
                {
                    cur->pack    = pack_alloc(cur->meta, NULL, flow->Uvalue);
                    bytes        = cur->bytes = cur->pack->bytes;
                    flow->Uvalue = 0;
                    flow->mode   = NIL;
                    break;
                }
                set_bytes(flow->Uvalue, cur->subitem_len, bytes, cur->BYTE_S);
                flow->Uvalue = 0;
                if((cur->BYTE_S += cur->subitem_len) < cur->BYTE_E) continue;
                break;
        }
        if(next(flow, &cur, &bytes)) continue;
        dst->dispatch(dst, flow->cursors[0].pack->meta->id, flow->cursors[0].pack);
        free_pack(flow->cursors[0].pack);
        flow->cursors[0].pack = NULL;
        flow->state           = STANDBY;
        flow->mode            = NIL;
    }
}

uint32_t transmit(Transmitter* const src, uint8_t* dst, size_t dst_bytes)
{
    Flow*          flow  = &src->flow;
    CursorBase*    cur   = _get_cursor(flow);
    uint8_t*       bytes = cur->bytes;
    const uint8_t* fix   = dst;
    for(; dst_bytes--; dst++)
    {
        switch(flow->state)
        {
            case STANDBY:
                flow->level = 0;
                cur = &flow->cursors[0];
                if((cur->pack = (Pack*)src->pull(src)) == NULL) return dst - fix;
                bytes          = cur->bytes = cur->pack->bytes;
                cur->meta      = cur->pack->meta;
                flow->state    = PACK_ID;
                flow->Uvalue   = cur->meta->id;
                cur->field_bit = 8 * (AD_HOC_ID_BYTES - 1);
            case PACK_ID:
                *dst = (uint8_t)(flow->Uvalue >> cur->field_bit);
                if(-1 < (cur->field_bit -= 8)) continue;
                flow->Uvalue   = 0;
                cur->field_bit = -1;
                cur->BIT_E     = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                flow->mode     = NIL;
                if(has_opt_fields(cur->meta))
                {
                    flow->state = VARINT;
                    _reset_cursor(cur);
                    pack_size(cur);
                    flow->Uvalue = cur->LAST_BYTE + 1 - cur->meta->packMinBytes;
                    cur->BIT_E = cur->BIT_S = cur->BYTE_E = cur->BYTE_S = cur->subitem_len = 0;
                    continue;
                }
                break;
            case BYTES:
                *dst = bytes[cur->BYTE_S++];
                if(cur->BYTE_S < cur->BYTE_E) continue;
                break;
            case VARINT:
                if(flow->Uvalue & ~(UMAX)0x7F)
                {
                    *dst = (uint8_t)(flow->Uvalue & 0x7F | 0x80);
                    flow->Uvalue >>= 7;
                    continue;
                }
                *dst = (uint8_t)flow->Uvalue;
                if((cur->BYTE_S += cur->subitem_len) < cur->BYTE_E)
                {
                    flow->Uvalue = get_bytes(bytes, cur->BYTE_S, cur->subitem_len);
                    continue;
                }
        }
        if(next(flow, &cur, &bytes))
        {
            if(flow->state == VARINT) flow->Uvalue = get_bytes(bytes, cur->BYTE_S, cur->subitem_len);
            continue;
        }
        flow->state = STANDBY;
        flow->mode = NIL;
        if(src->event) src->event(__LINE__, src, flow->cursors[0].pack, AFTER_SENDING);
        free_pack(flow->cursors[0].pack);
        flow->cursors[0].pack = NULL;
    }
    return dst - fix;
}














bool set_field(Cursor* const cur, size_t fbit, int32_t all_items_vals, ...)

{
    CursorBase* base = &cur->base;
    if(base->field_bit == fbit) return true;
    if(all_items_vals < 0 &&
            (cur->base.BIT_E <= fbit ||
             (cur->base.bytes[fbit >> 3] & 1 << (fbit & 7)) == 0))
        return false;
    Meta const* const meta = base->meta;
    Field const*      fld;
    uint8_t*          bytes = base->bytes;
    int32_t           delta;
    if(base->field_bit == -1 || fbit < base->field_bit && reset_cursor(cur))
    {
        if((delta = first_field_bit(base)) == -1 || fbit < meta->field_0_bit + delta) goto insert_field;
        base->field_bit = meta->field_0_bit - 1;
    }
    else if((delta = next_field_bit(base)) == -1 || fbit < base->field_bit + 1 + delta) goto insert_field;
    do
    {
        base->BIT_S  = base->BIT_E;
        base->BYTE_S = base->BYTE_E;
        fld          = meta->fields[(base->field_bit += 1 + delta) - meta->field_0_bit];
        if(0 < fld->length)
            if(0 < fld->size) base->BYTE_E += fld->const_dims_total * fld->length * fld->size;
            else base->BIT_E += fld->const_dims_total * fld->length * fld->size;
        else set_bounds(fld, base);
    }
    while(-1 < (delta = next_field_bit(base)) && base->field_bit + 1 + delta < fbit);
    if(base->field_bit == fbit ||
            all_items_vals < 0)
    {
        cur->item_len     = fld->length;
        base->subitem_len = fld->size;
        cur->BIT          = base->BIT_E;
        cur->BYTE         = base->BYTE_S;
        if(fld->length < 1)
        {
            cur->field_items_total = fld->const_dims_total;
            if(fld->var_dims_count)
            {
                cur->D[fld->dims_count] = 0;
                for(int32_t i = 0, bit = base->BIT_S, dim; i < fld->dims_count; i++)
                    if((dim = fld->dims[i]) < 1) cur->field_items_total *= cur->D[i] = get_bits(bytes, bit -= -dim, -dim);
                    else cur->D[i]                                                    = dim;
            }
            init(fld, cur);
        }
        return base->field_bit == fbit;
    }
insert_field:
    if(all_items_vals < 0) return false;
    fld = meta->fields[fbit - meta->field_0_bit];
    if(fld->type == 0)
    {
        if(0 < fld->size)
        {
            insert(cur, fbit, 0, fld->const_dims_total * fld->length * fld->size);
            base->BYTE_E = (cur->BYTE = base->BYTE_S) + fld->const_dims_total * (cur->item_len = fld->length) * (base->subitem_len = fld->size);
        }
        else
        {
            insert(cur, fbit, fld->const_dims_total * fld->length * -fld->size, 0);
            cur->BIT = base->BIT_E = base->BIT_S + fld->const_dims_total * (cur->item_len = fld->length) * fld->size;
        }
        return true;
    }
    int32_t total = fld->const_dims_total;
    if(fld->var_dims_count)
    {
        cur->D[fld->dims_count] = 0;
        va_list ap;
        va_start(ap, all_items_vals);
        for(int32_t i = 0; i < fld->dims_count; i++)
            if(fld->dims[i] < 0) total *= cur->D[i] = va_arg(ap, uint32_t);
            else cur->D[i] = fld->dims[i];
        va_end(ap);
    }
    switch(fld->type)
    {
        case 1:
            insert(cur, fbit, fld->field_info_bits, total * -fld->length * fld->size);
            break;
        case 7:
            insert(cur, fbit, fld->field_info_bits + total * -fld->length * fld->size, 0);
            break;
        case 2:
        case 6:
        case 8:
        case 12:
            insert(cur, fbit, fld->field_info_bits, 0);
            break;
        case 5:
        case 11:
            insert(cur, fbit, fld->field_info_bits + total * -fld->length, 0);
            break;
        case 3:
            if(fld->datatype)
                all_items_vals = fld->datatype->packMinBytes < all_items_vals ? all_items_vals : fld->datatype->packMinBytes;
            insert(cur, fbit, fld->field_info_bits, total * all_items_vals * fld->size);
            set_bits(all_items_vals, -fld->length, base->bytes, base->BIT_S - fld->field_info_bits);
            break;
        case 4:
            insert(cur, fbit, fld->field_info_bits, 0);
            set_bits(all_items_vals, -fld->length, base->bytes, base->BIT_S - fld->field_info_bits + 2 * fld->sparse_bits);
            break;
        case 9:
            insert(cur, fbit, fld->field_info_bits + total * all_items_vals * fld->size, 0);
            set_bits(all_items_vals, -fld->length, base->bytes, base->BIT_S - fld->field_info_bits);
            break;
        case 10:
            insert(cur, fbit, fld->field_info_bits, 0);
            set_bits(all_items_vals, -fld->length, base->bytes, base->BIT_S - fld->field_info_bits + 2 * fld->sparse_bits);
            break;
    }
    cur->field_items_total = total;
    if(fld->var_dims_count)
        for(int32_t i = 0, bit = base->BIT_S; i < fld->dims_count; i++) if(fld->dims[i] < 0) set_bits(cur->D[i], -fld->dims[i], base->bytes, bit -= -fld->dims[i]);
    set_bounds(fld, &cur->base);
    init(fld, cur);
    return true;
}












bool set_item(Cursor* const cur, size_t item, int32_t item_vals)
{
    CursorBase const* base = &cur->base;
    Field const*      fld  = base->meta->fields[base->field_bit - base->meta->field_0_bit];
    if(cur->field_item == item)
    {
        if(fld->type == 5 &&
                -1 < item_vals && cur->item_len != item_vals)
        {
            if(fld->datatype)
                item_vals = fld->datatype->packMinBytes < item_vals ? item_vals : fld->datatype->packMinBytes;
            resize_bytes(cur, (item_vals - cur->item_len) * cur->base.subitem_len);
            set_bits(cur->item_len = item_vals, -fld->length, cur->base.bytes, cur->BIT);
        }
        return true;
    }
    const int32_t _item      = item;
    const int32_t len        = fld->length;
    const int32_t len_bits   = -len;
    uint32_t      bit        = cur->BIT;
    uint8_t*      bytes      = base->bytes;
    int32_t       field_item = cur->field_item;
    switch(fld->type)
    {
        default:
            return false;
        case 2:
        case 4:
        {
            if(cur->field_items == 0 || item < cur->field_item_0 || cur->field_item_0 + cur->field_items <= item || (bytes[(bit = base->BIT_S - fld->field_info_bits - 1 - (item - cur->field_item_0)) >> 3] & 1 << (bit & 7)) == 0)
            {
                if(item_vals < 0) return false;
                if(_item < cur->field_item_0 || cur->field_items == 0)
                {
                    cur->BIT                = base->BIT_S - fld->field_info_bits;
                    cur->BYTE               = base->BYTE_S;
                    const int32_t ins_items = cur->field_items == 0 ? 1 : cur->field_item_0 - _item;
                    insert(cur, base->field_bit, ins_items, cur->item_len * fld->size);
                    cur->BIT                   = base->BIT_S - fld->field_info_bits - 1;
                    set_bits(cur->field_item_0 = _item, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits + fld->sparse_bits);
                    set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
                }
                else if(_item < cur->field_item_0 + cur->field_items)
                {
                    if(_item < cur->field_item)
                    {
                        cur->field_item = cur->field_item_0;
                        cur->BIT        = base->BIT_S - fld->field_info_bits - 1;
                        cur->BYTE       = base->BYTE_S;
                    }
                    else
                    {
                        cur->BIT--;
                        cur->field_item++;
                        cur->BYTE += cur->item_len * fld->size;
                    }
                    for(const int32_t len_bytes = cur->item_len * fld->size; cur->field_item < _item; cur->field_item++, cur->BIT--) if((base->bytes[cur->BIT >> 3] & 1 << (cur->BIT & 7)) != 0) cur->BYTE += len_bytes;
                    insert(cur, base->field_bit, 0, cur->item_len * fld->size);
                }
                else
                {
                    const int32_t ins_items = _item + 1 - (cur->field_item_0 + cur->field_items);
                    cur->BIT                = base->BIT_E;
                    cur->BYTE               = base->BYTE_E;
                    insert(cur, base->field_bit, ins_items, cur->item_len * fld->size);
                    cur->BIT  = base->BIT_E;
                    cur->BYTE = base->BYTE_E - cur->item_len * fld->size;
                    set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
                }
                base->bytes[cur->BIT >> 3] |= 1 << (cur->BIT & 7);
                break;
            }
            if(item < cur->field_item)
            {
                cur->field_item = cur->field_item_0;
                cur->BIT        = base->BIT_S - fld->field_info_bits - 1;
                cur->BYTE       = base->BYTE_S;
            }
            else
            {
                cur->BIT--;
                cur->field_item++;
                cur->BYTE += cur->item_len * fld->size;
            }
            for(const int32_t len_bytes = cur->item_len * fld->size;; cur->field_item++, cur->BIT--)
                if((bytes[cur->BIT >> 3] & 1 << (cur->BIT & 7)) != 0)
                {
                    if(cur->field_item == item) break;
                    cur->BYTE += len_bytes;
                }
            return true;
        }
        case 5:
        {
            if(cur->field_items_total - 1 < item || !get_bits(bytes, base->BIT_S - fld->field_info_bits - (item + 1) * -fld->length, -fld->length))
            {
                if(item_vals < 0) return false;
                if(_item < cur->field_item)
                {
                    cur->field_item = 0;
                    cur->BIT        = base->BIT_S - fld->field_info_bits;
                    cur->BYTE       = base->BYTE_S;
                    cur->item_len   = (int32_t)get_bits(base->bytes, cur->BIT -= len_bits, len_bits);
                }
                for(; cur->field_item < _item; cur->BYTE += cur->item_len * fld->size, cur->item_len = (int32_t)
                        get_bits(base->bytes, cur->BIT -= len_bits, len_bits), cur->field_item++);
                insert(cur, base->field_bit, 0, item_vals * fld->size);
                set_bits(item_vals, len_bits, base->bytes, cur->BIT);
                cur->item_len = item_vals;
                break;
            }
            if(item < cur->field_item)
            {
                cur->field_item = 0;
                cur->BIT        = base->BIT_S - fld->field_info_bits;
                cur->item_len   = (int32_t)get_bits(bytes, cur->BIT -= -fld->length, -fld->length);
                cur->BYTE = base->BYTE_S;
            }
            for(; cur->field_item < item; cur->BYTE += cur->item_len * fld->size, cur->item_len = (int32_t)
                    get_bits(bytes, cur->BIT -= -fld->length, -fld->length), cur->field_item++);
            if(
                -1 < item_vals && cur->item_len != item_vals)
            {
                resize_bytes(cur, (item_vals - cur->item_len) * cur->base.subitem_len);
                set_bits(cur->item_len = item_vals, -fld->length, cur->base.bytes, cur->BIT);
            }
            return true;
        }
        case 6:
        {
            if(cur->field_item_0 <= item && item < cur->field_item_0 + cur->field_items)
            {
                int32_t BYTE = cur->BYTE;
                if(item < field_item)
                {
                    field_item = cur->field_item_0;
                    bit        = base->BIT_S - fld->field_info_bits - 1;
                    BYTE       = base->BYTE_S;
                }
                else
                {
                    bit--;
                    field_item++;
                    BYTE += cur->item_len * fld->size;
                }
                for(int32_t len_bits = -fld->length;; bit--, field_item++)
                    if((bytes[bit >> 3] & 1 << (bit & 7)) != 0)
                    {
                        bit -= len_bits;
                        if(field_item == item)
                        {
                            cur->field_item = field_item;
                            cur->BYTE       = BYTE;
                            cur->BIT        = bit;
                            cur->item_len   = (int32_t)get_bits(bytes, bit, len_bits);
                            return true;
                        }
                        BYTE += get_bits(bytes, bit, len_bits) * fld->size;
                    }
                    else if(field_item == item) break;
            }
            if(item_vals < 0) return false;
            if(_item < cur->field_item_0 || cur->field_items == 0)
            {
                cur->BIT                = base->BIT_S - fld->field_info_bits;
                cur->BYTE               = base->BYTE_S;
                const int32_t ins_items = cur->field_items == 0 ? 1 : cur->field_item_0 - _item;
                insert(cur, base->field_bit, ins_items + len_bits, item_vals * fld->size);
                cur->BIT                   = base->BIT_S - fld->field_info_bits - 1;
                set_bits(cur->field_item_0 = _item, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits + fld->sparse_bits);
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            else if(_item < cur->field_item_0 + cur->field_items)
            {
                if(_item < cur->field_item)
                {
                    cur->field_item = cur->field_item_0;
                    cur->BIT        = base->BIT_S - fld->field_info_bits - 1;
                    cur->BYTE       = base->BYTE_S;
                }
                else
                {
                    cur->BIT--;
                    cur->field_item++;
                    cur->BYTE += cur->item_len * fld->size;
                }
                for(; cur->field_item < _item; cur->BIT--, cur->field_item++)
                    if((base->bytes[cur->BIT >> 3] & 1 << (cur->BIT & 7)) != 0)
                    {
                        cur->BIT -= len_bits;
                        cur->item_len = (int32_t)get_bits(base->bytes, cur->BIT, len_bits);
                        cur->BYTE += cur->item_len * fld->size;
                    }
                insert(cur, base->field_bit, len_bits, item_vals * fld->size);
            }
            else
            {
                const int32_t ins_items = _item - (cur->field_item_0 + cur->field_items) + 1;
                cur->BIT                = base->BIT_E;
                cur->BYTE               = base->BYTE_E;
                insert(cur, base->field_bit, ins_items + len_bits, item_vals * fld->size);
                cur->BIT  = base->BIT_E + len_bits;
                cur->BYTE = base->BYTE_E - item_vals * fld->size;
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            base->bytes[cur->BIT >> 3] |= 1 << (cur->BIT & 7);
            set_bits(cur->item_len = item_vals, len_bits, base->bytes, cur->BIT -= len_bits);
            break;
        }
        case 8:
        case 10:
        {
            if(cur->field_item_0 <= item && item < cur->field_item_0 + cur->field_items)
            {
                if(item < field_item)
                {
                    field_item = cur->field_item_0;
                    bit        = base->BIT_S - fld->field_info_bits - 1;
                }
                else
                {
                    bit--;
                    field_item++;
                }
                for(const int32_t bits = cur->item_len * fld->size;; bit--, field_item++)
                    if((bytes[bit >> 3] & 1 << (bit & 7)) != 0)
                    {
                        bit -= bits;
                        if(field_item == item)
                        {
                            cur->field_item = item;
                            cur->BIT        = bit;
                            return true;
                        }
                    }
                    else if(field_item == item) break;
            }
            if(item_vals < 0) return false;
            const int32_t bits = cur->item_len * fld->size;
            if(_item < cur->field_item_0 || cur->field_items == 0)
            {
                cur->BIT                = base->BIT_S - fld->field_info_bits;
                cur->BYTE               = base->BYTE_S;
                const int32_t ins_items = cur->field_items == 0 ? 1 : cur->field_item_0 - _item;
                insert(cur, base->field_bit, ins_items + bits, 0);
                cur->BIT                   = base->BIT_S - fld->field_info_bits - 1;
                set_bits(cur->field_item_0 = _item, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits + fld->sparse_bits);
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            else if(_item < cur->field_item_0 + cur->field_items)
            {
                if(_item < cur->field_item)
                {
                    cur->field_item = cur->field_item_0;
                    cur->BIT        = base->BIT_S - fld->field_info_bits - 1;
                }
                else
                {
                    cur->BIT--;
                    cur->field_item++;
                }
                for(; cur->field_item < _item; cur->BIT--, cur->field_item++)
                    if((base->bytes[cur->BIT >> 3] & 1 << (cur->BIT & 7)) != 0) cur->BIT -= bits;
                insert(cur, base->field_bit, bits, 0);
            }
            else
            {
                const int32_t ins_items = _item + 1 - (cur->field_item_0 + cur->field_items);
                cur->BIT                = base->BIT_E;
                insert(cur, base->field_bit, ins_items + bits, 0);
                cur->BIT = base->BIT_E + bits;
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            base->bytes[cur->BIT >> 3] |= 1 << (cur->BIT & 7);
            cur->BIT -= bits;
            break;
        }
        case 11:
        {
            if(item < cur->field_items_total)
            {
                if(item < field_item)
                {
                    field_item = -1;
                    bit        = base->BIT_S - fld->field_info_bits;
                }
                else if(item == cur->field_item) return true;
                int32_t item_vars;
                do
                {
                    int32_t bits = fld->size * (item_vars = (int32_t)get_bits(bytes, bit -= -fld->length, -fld->length));
                    bit -= bits;
                    field_item++;
                }
                while(field_item < item);
                if(0 < item_vars)
                {
                    cur->BIT        = bit;
                    cur->item_len   = item_vars;
                    cur->field_item = field_item;
                    return true;
                }
            }
            if(item_vals < 0) return false;
            if(_item < cur->field_item)
            {
                cur->field_item = -1;
                cur->BIT        = base->BIT_S - fld->field_info_bits;
            }
            while(cur->field_item < _item)
            {
                const int32_t bits = (int32_t)(fld->size * get_bits(base->bytes, cur->BIT -= len_bits, len_bits));
                cur->BIT -= bits;
                cur->field_item++;
            }
            insert(cur, base->field_bit, item_vals * fld->size, 0);
            set_bits(item_vals, len_bits, base->bytes, cur->BIT);
            cur->BIT -= item_vals * fld->size;
            cur->item_len = item_vals;
            break;
        }
        case 12:
        {
            if(cur->field_item_0 <= item && item < cur->field_item_0 + cur->field_items)
            {
                if(item < field_item)
                {
                    field_item = cur->field_item_0;
                    bit        = base->BIT_S - fld->field_info_bits - 1;
                }
                else
                {
                    bit--;
                    field_item++;
                }
                for(int32_t len_bits = -fld->length;; bit--, field_item++)
                    if((bytes[bit >> 3] & 1 << (bit & 7)) != 0)
                    {
                        bit -= len_bits;
                        if(field_item == item)
                        {
                            cur->field_item = field_item;
                            cur->item_len   = (int32_t)get_bits(bytes, bit, len_bits);
                            cur->BIT        = bit - cur->item_len * fld->size;
                            return true;
                        }
                        bit -= get_bits(bytes, bit, len_bits) * fld->size;
                    }
                    else if(field_item == item) break;
            }
            if(item_vals < 0) return false;
            if(_item < cur->field_item_0 || cur->field_items == 0)
            {
                cur->BIT                = base->BIT_S - fld->field_info_bits;
                const int32_t ins_items = cur->field_items == 0 ? 1 : cur->field_item_0 - _item;
                insert(cur, base->field_bit, ins_items + len_bits + item_vals * fld->size, 0);
                cur->BIT                   = base->BIT_S - fld->field_info_bits - 1;
                set_bits(cur->field_item_0 = _item, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits + fld->sparse_bits);
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            else if(_item < cur->field_item_0 + cur->field_items)
            {
                if(_item < cur->field_item)
                {
                    cur->field_item = cur->field_item_0;
                    cur->BIT        = base->BIT_S - fld->field_info_bits - 1;
                }
                else
                {
                    cur->BIT--;
                    cur->field_item++;
                }
                for(; cur->field_item < _item; cur->BIT--, cur->field_item++)
                    if((base->bytes[cur->BIT >> 3] & 1 << (cur->BIT & 7)) != 0)
                    {
                        cur->BIT -= len_bits;
                        cur->item_len = (int32_t)get_bits(base->bytes, cur->BIT, len_bits);
                        cur->BIT -= cur->item_len * fld->size;
                    }
                insert(cur, base->field_bit, len_bits + item_vals * fld->size, 0);
            }
            else
            {
                const int32_t ins_items = _item - (cur->field_item_0 + cur->field_items) + 1;
                cur->BIT                = base->BIT_E;
                insert(cur, base->field_bit, ins_items + len_bits + item_vals * fld->size, 0);
                cur->BIT = base->BIT_E + item_vals * fld->size + len_bits;
                set_bits(cur->field_items += ins_items, fld->sparse_bits, base->bytes, base->BIT_S - fld->field_info_bits);
            }
            base->bytes[cur->BIT >> 3] |= 1 << (cur->BIT & 7);
            set_bits(cur->item_len = item_vals, len_bits, base->bytes, cur->BIT -= len_bits);
            cur->BIT -= item_vals * fld->size;
            cur->item_len = item_vals;
            break;
        }
    }
    cur->field_item = _item;
    return false;
}
