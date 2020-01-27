#include "Client.hpp"
#include <cassert>

static size_t     some_size_t    = 0;
static uint16_t     some_uint16_t    = 0;
static uint8_t     some_uint8_t    = 0;

using namespace  org::company;

void on_SensorsData(const SensorsData & p9);


void on_DeviceError(const DeviceError & p8);


void fill(const InstructionsPack & p7);


void on_InstructionsPack(const InstructionsPack & p7);


void fill(const BusConfiguration & p6);


void on_BusConfiguration(const BusConfiguration & p6);


void on_DeviceVersion(const DeviceVersion & p0);



void on_DeviceVersion(const DeviceVersion & p0)
{
    auto some_DevID = p0.DevID();
    auto some_Ver = p0.Ver();
}
void on_Stop() {}
void on_Start() {}
void on_GetDeviceVersion() {}
void on_GetConfiguration() {}
void on_SetConfiguration() {}


void on_BusConfiguration(const BusConfiguration & p6)
{
    auto some_multiplier = p6.multiplier();
    auto some_time = p6.time();
    auto some_clk_khz = p6.clk_khz();
}


void fill(const BusConfiguration & p6)
{
    p6.multiplier(some_uint8_t);
    p6.time(some_uint16_t);
    p6.clk_khz(some_uint16_t);
}


void on_InstructionsPack(const InstructionsPack & p7)
{
    auto some_Length = p7.Length();
    const auto src_Instructions = p7.Instructions();
    for(size_t index = 0; index <  InstructionsPack::Instructions_::len; index++)
        some_uint8_t = src_Instructions.get(index);
}


void fill(const InstructionsPack & p7)
{
    p7.Length(some_uint8_t);
    p7.Instructions(& some_uint8_t);
}


void on_DeviceError(const DeviceError & p8)
{
    auto some_error_id = p8.error_id();
    auto some_param1 = p8.param1();
}


void on_SensorsData(const SensorsData & p9)
{
    const auto src_values = p9.values();
    for(size_t index = 0; index <  SensorsData::values_::len; index++)
        some_uint16_t = src_values.get(index);
}

struct Communication_demo
{
    Communication<Communication_demo> channel;
    void on_DeviceVersion(Pack * pack)
    {
        DeviceVersion p0 = { pack, *pack->bytes };
        ::on_DeviceVersion(p0);
        //printf( "DeviceVersion pass\n");
    }
    void on_BusConfiguration(Pack * pack)
    {
        BusConfiguration p6 = { pack, *pack->bytes };
        ::on_BusConfiguration(p6);
        //printf( "BusConfiguration pass\n");
    }
    void on_InstructionsPack(Pack * pack)
    {
        InstructionsPack p7 = { pack, *pack->bytes };
        ::on_InstructionsPack(p7);
        //printf( "InstructionsPack pass\n");
    }
    void on_DeviceError(Pack * pack)
    {
        DeviceError p8 = { pack, *pack->bytes };
        ::on_DeviceError(p8);
        //printf( "DeviceError pass\n");
    }
    void on_SensorsData(Pack * pack)
    {
        SensorsData p9 = { pack, *pack->bytes };
        ::on_SensorsData(p9);
        //printf( "SensorsData pass\n");
    }
    static  Communication_demo * from(Receiver * receiver)
    {
        return reinterpret_cast<Communication_demo*>(reinterpret_cast<uint8_t*>(receiver) - offsetof(Communication_demo, channel) - offsetof(Communication<Communication_demo>, receiver));
    }

    RBUF_INIT(Pack*, 5) sending_out_packs;
    static const Pack* pull_sending_pack(Transmitter* transmitter)
    {
        auto channel = reinterpret_cast<Communication_demo*>(reinterpret_cast<uint8_t*>(transmitter) - offsetof(Communication_demo, channel) - offsetof(Communication<Communication_demo>, transmitter));
        if(RBUF_ISEMPTY(channel->sending_out_packs)) return nullptr;
        return RBUF_GET(channel->sending_out_packs);
    }
    static bool push_sending_pack(Transmitter* transmitter, Pack* pack)
    {
        auto channel = reinterpret_cast<Communication_demo*>(reinterpret_cast<uint8_t*>(transmitter) - offsetof(Communication_demo, channel) - offsetof(Communication<Communication_demo>, transmitter));
        if(RBUF_ISFULL(channel->sending_out_packs)) return false;
        RBUF_PUT(channel->sending_out_packs, pack)
        return true;
    }

};


int main()
{
    CURSORS(cur);
    uint8_t buffer[512];
    int bytes_out ;
    Communication_demo _Communication{};
    {
        if(! _Communication.channel.send_Stop())
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        if(! _Communication.channel.send_Start())
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        if(! _Communication.channel.send_GetDeviceVersion())
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        if(! _Communication.channel.send_GetConfiguration())
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        if(! _Communication.channel.send_SetConfiguration())
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        auto p6 = _Communication.channel.NEW.BusConfiguration() ;
        fill(p6);
        if(_Communication.channel.send(p6))
        {
            free_pack(p6.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        auto p7 = _Communication.channel.NEW.InstructionsPack() ;
        fill(p7);
        if(_Communication.channel.send(p7))
        {
            free_pack(p7.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    bytes_out = _Communication.channel.packs_into_bytes(buffer, 512); // sending packs
    _Communication.channel.bytes_into_packs(buffer, 512); // receiving bytes
    return 0;
}
