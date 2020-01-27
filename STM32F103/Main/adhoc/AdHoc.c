
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


static bool next(Flow* const flow)
{
    switch(flow->some)
    {
        case 0:
            flow->BYTE_S = flow->BYTE_E = 0;
            if(flow->pack->meta->_2)
            {
                flow->BYTE_E += flow->pack->meta->_2 * (flow->some = 2);
                flow->state                                        = VARINT;
                return true;
            }
        case 2:
            if(flow->pack->meta->_4)
            {
                flow->BYTE_E += flow->pack->meta->_4 * (flow->some = 4);
                flow->state                                        = VARINT;
                return true;
            }
        case 4:
            if(flow->pack->meta->_8)
            {
                flow->BYTE_E += flow->pack->meta->_8 * (flow->some = 8);
                flow->state                                        = VARINT;
                return true;
            }
        case 8:
            if(flow->BYTE_S < (flow->BYTE_E = flow->pack->meta->packMinBytes))
            {
                flow->state = BYTES;
                flow->some  = 1;
                return true;
            }
            flow->mode = NIL;
    }
    return false;
}


void receive_adv(const uint8_t* src, size_t src_bytes, Receiver* const dst)
{
    Flow* flow = &dst->flow;
    dst->time = ~(uint32_t)0;
    if(dst->time == 0)
    {
        dst->time   = ~0;
        flow->state = STANDBY;
        if(dst->event) dst->event(__LINE__, dst, flow->pack, AD_HOC_FAILURE_RECEIVE_TIMEOUT);
        free_pack(flow->pack);
        flow->pack = NULL;
    }
    for(; src_bytes--; flow->mode != DOCRC ? flow->crc = crc16(*src, flow->crc) : 0, src++)
    {
        const Meta* meta = NULL;
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
                flow->mode   = NIL;
                meta         = dst->dispatch(dst, flow->Uvalue, NULL);
                flow->Uvalue = 0;
                if(meta)
                {
                    dst->bits    = 0;
                    flow->BYTE_E = flow->BYTE_S = flow->some = 0;
                    flow->pack = new_pack(meta);
                    break;
                }
                if(dst->event) dst->event(__LINE__, dst, NULL, AD_HOC_FAILURE_RECEIVE_PACK_WITH_UNEXPECTED_ID);
                flow->state = STANDBY;
                continue;
            case BYTES:
                if(*src != BR) goto NOT_BR;
                flow->state = BYTES_BR;
                continue;
            case BYTES_BR:
                if(*src != BR)
                {
                    if(dst->event) dst->event(__LINE__, dst, flow->pack, AD_HOC_FAILURE_RECEIVE_EXPECT_SECOND_BR_BUT_GET);
                    free_pack(flow->pack);
                    flow->pack  = NULL;
                    flow->state = STANDBY;
                    continue;
                }
                flow->state = BYTES;
NOT_BR:
                if(flow->mode == DOCRC)
                    switch(flow->some)
                    {
                        case 2:
                            flow->Uvalue = (*src & (UMAX)0xFF) << 8;
                            flow->some = 1;
                            continue;
                        case 1:
                            if((flow->Uvalue | *src) == flow->crc) dst->dispatch(dst, flow->pack->meta->id, flow->pack);
                            else
                            {
                                if(dst->event) dst->event(__LINE__, dst, flow->pack, AD_HOC_FAILURE_RECEIVE_CRC_ERROR);
                                free_pack(flow->pack);
                            }
                            flow->pack  = NULL;
                            flow->state = STANDBY;
                            flow->mode  = NIL;
                            continue;
                    }
                flow->pack->bytes[flow->BYTE_S++] = *src;
                if(flow->BYTE_S < flow->BYTE_E) continue;
                break;
            case VARINT:
                if(*src != BR) goto NOT_BR_;
                flow->state = VARINT_BR;
                continue;
            case VARINT_BR:
                if(*src != BR)
                {
                    if(dst->event) dst->event(__LINE__, dst, flow->pack, AD_HOC_FAILURE_RECEIVE_EXPECT_SECOND_BR_BUT_GET);
                    free_pack(flow->pack);
                    flow->pack  = NULL;
                    flow->state = STANDBY;
                    continue;
                }
                flow->state = VARINT;
NOT_BR_:
                flow->Uvalue |= (*src & (UMAX)0x7F) << dst->bits;
                dst->bits += 7;
                if((*src & 0x80) != 0) continue;
                dst->bits = 0;
                set_bytes(flow->Uvalue, flow->some, flow->pack->bytes, flow->BYTE_S);
                flow->Uvalue = 0;
                if((flow->BYTE_S += flow->some) < flow->BYTE_E) continue;
                break;
        }
        if(next(flow)) continue;
        flow->state = BYTES;
        flow->mode  = DOCRC;
        flow->some  = 2;
    }
}

uint32_t transmit_adv(Transmitter* const src, uint8_t* dst, size_t dst_bytes)
{
    Flow*          flow = &src->flow;
    const uint8_t* fix  = dst;
    for(; dst_bytes--; flow->mode != DOCRC ? flow->crc = crc16(*dst, flow->crc) : 0, dst++)
    {
        switch(flow->state)
        {
            case STANDBY:
                if((flow->pack = (Pack*)src->pull(src)) == NULL) return dst - fix;
                flow->state  = PACK_ID;
                flow->Uvalue = flow->pack->meta->id;
                flow->some   = 8 * (AD_HOC_ID_BYTES - 1);
                flow->crc    = 0;
                *dst         = BR;
                continue;
            case PACK_ID:
                *dst = (uint8_t)(flow->Uvalue >> flow->some);
                if(-1 < (flow->some -= 8)) continue;
                flow->Uvalue = 0;
                flow->BYTE_E = flow->BYTE_S = flow->some = 0;
                flow->mode   = NIL;
                break;
            case BYTES:
                if(flow->mode == DOCRC)
                {
                    switch(flow->some)
                    {
                        case 4:
                            flow->some = (*dst = flow->crc >> 8) == BR ? 3 : 2;
                            continue;
                        case 3:
                            *dst = BR;
                            flow->some = 2;
                            continue;
                        case 2:
                            if((*dst = flow->crc & 0xFF) != BR) break;
                            flow->some = 1;
                            continue;
                        case 1:
                            *dst = BR;
                    }
                    flow->state = STANDBY;
                    flow->mode  = NIL;
                    if(src->event) src->event(__LINE__, src, flow->pack, AFTER_SENDING);
                    free_pack(flow->pack);
                    flow->pack = NULL;
                    continue;
                }
                if((*dst = flow->pack->bytes[flow->BYTE_S++]) != BR) goto NOT_BR;
                flow->state = BYTES_BR;
                continue;
            case BYTES_BR:
                flow->state = BYTES;
                *dst = BR;
NOT_BR:
                if(flow->BYTE_S < flow->BYTE_E) continue;
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
                if((flow->BYTE_S += flow->some) < flow->BYTE_E)
                {
                    flow->Uvalue = get_bytes(flow->pack->bytes, flow->BYTE_S, flow->some);
                    continue;
                }
        }
        if(next(flow))
        {
            if(flow->state == VARINT) flow->Uvalue = get_bytes(flow->pack->bytes, flow->BYTE_S, flow->some);
            continue;
        }
        flow->state = BYTES;
        flow->mode  = DOCRC;
        flow->some  = 4;
    }
    return dst - fix;
}

void receive(const uint8_t* src, size_t src_bytes, Receiver* const dst)
{
    Flow* flow = &dst->flow;
    dst->time  = ~(uint32_t)0;
    if(dst->time == 0)
    {
        dst->time   = ~0;
        flow->state = STANDBY;
        if(dst->event) dst->event(__LINE__, dst, flow->pack, AD_HOC_FAILURE_RECEIVE_TIMEOUT);
        free_pack(flow->pack);
        flow->pack = NULL;
    }
    for(; src_bytes--; src++)
    {
        const Meta* meta = NULL;
        switch(flow->state)
        {
            case STANDBY:
                flow->Uvalue = 0;
                dst->bits   = 0;
                flow->state = PACK_ID;
            case PACK_ID:
                flow->Uvalue = flow->Uvalue << 8 | *src;
                if(++dst->bits < AD_HOC_ID_BYTES) continue;
                flow->mode = NIL;
                meta         = dst->dispatch(dst, flow->Uvalue, NULL);
                flow->Uvalue = 0;
                if(meta)
                {
                    dst->bits    = 0;
                    flow->BYTE_E = flow->BYTE_S = flow->some = 0;
                    flow->pack = new_pack(meta);
                    break;
                }
                if(dst->event) dst->event(__LINE__, dst, NULL, AD_HOC_FAILURE_RECEIVE_PACK_WITH_UNEXPECTED_ID);
                flow->state = STANDBY;
                continue;
            case BYTES:
                flow->pack->bytes[flow->BYTE_S++] = *src;
                if(flow->BYTE_S < flow->BYTE_E) continue;
                break;
            case VARINT:
                flow->Uvalue |= (*src & (UMAX)0x7F) << dst->bits;
                dst->bits += 7;
                if((*src & 0x80) != 0) continue;
                dst->bits = 0;
                set_bytes(flow->Uvalue, flow->some, flow->pack->bytes, flow->BYTE_S);
                flow->Uvalue = 0;
                if((flow->BYTE_S += flow->some) < flow->BYTE_E) continue;
                break;
        }
        if(next(flow)) continue;
        dst->dispatch(dst, flow->pack->meta->id, flow->pack);
        flow->pack  = NULL;
        flow->state = STANDBY;
        flow->mode  = NIL;
    }
}

uint32_t transmit(Transmitter* const src, uint8_t* dst, size_t dst_bytes)
{
    Flow* flow = &src->flow;
    const uint8_t* fix = dst;
    for(; dst_bytes--; dst++)
    {
        switch(flow->state)
        {
            case STANDBY:
                if((flow->pack = (Pack*)src->pull(src)) == NULL) return dst - fix;
                flow->state  = PACK_ID;
                flow->Uvalue = flow->pack->meta->id;
                flow->some   = 8 * (AD_HOC_ID_BYTES - 1);
            case PACK_ID:
                *dst = (uint8_t)(flow->Uvalue >> flow->some);
                if(-1 < (flow->some -= 8)) continue;
                flow->Uvalue = 0;
                flow->BYTE_E = flow->BYTE_S = flow->some = 0;
                flow->mode   = NIL;
                break;
            case BYTES:
                *dst = flow->pack->bytes[flow->BYTE_S++];
                if(flow->BYTE_S < flow->BYTE_E) continue;
                break;
            case VARINT:
                if(flow->Uvalue & ~(UMAX)0x7F)
                {
                    *dst = (uint8_t)(flow->Uvalue & 0x7F | 0x80);
                    flow->Uvalue >>= 7;
                    continue;
                }
                *dst = (uint8_t)flow->Uvalue;
                if((flow->BYTE_S += flow->some) < flow->BYTE_E)
                {
                    flow->Uvalue = get_bytes(flow->pack->bytes, flow->BYTE_S, flow->some);
                    continue;
                }
        }
        if(next(flow))
        {
            if(flow->state == VARINT) flow->Uvalue = get_bytes(flow->pack->bytes, flow->BYTE_S, flow->some);
            continue;
        }
        flow->state = STANDBY;
        flow->mode = NIL;
        if(src->event) src->event(__LINE__, src, flow->pack, AFTER_SENDING);
        free_pack(flow->pack);
        flow->pack = NULL;
    }
    return dst - fix;
}
