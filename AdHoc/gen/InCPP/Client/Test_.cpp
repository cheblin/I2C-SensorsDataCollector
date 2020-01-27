
#include "Client.hpp"
#include <string.h>
#include <stdio.h>
#include <assert.h>

using namespace  org::company;


namespace org
{
    namespace unirail
    {
        namespace test
        {

            struct DeviceVersion
            {
                Pack * pack_;
                uint8_t & data_;
                Pack* unwrap_()
                {
                    Pack* ret = pack_;
                    pack_ = nullptr;
                    return ret;
                }

                uint16_t DevID() const {return (static_cast< uint16_t >(get_bytes(&data_,  0, 2)))    ;}
                void DevID(uint16_t  src) const {    set_bytes((src), 2, &data_,  0);  }
                uint16_t Ver() const {return (static_cast< uint16_t >(get_bytes(&data_,  2, 2)))    ;}
                void Ver(uint16_t  src) const {    set_bytes((src), 2, &data_,  2);  }


            };
            struct DeviceError
            {
                Pack * pack_;
                uint8_t & data_;
                Pack* unwrap_()
                {
                    Pack* ret = pack_;
                    pack_ = nullptr;
                    return ret;
                }

                uint16_t param1() const {return (static_cast< uint16_t >(get_bytes(&data_,  0, 2)))    ;}
                void param1(uint16_t  src) const {    set_bytes((src), 2, &data_,  0);  }
                enum Errors error_id() const {return      static_cast< enum Errors >((get_bits(&data_, 16, 1)))    ;}
                void error_id(enum Errors  src) const {   set_bits(static_cast< uintptr_t >(src), 1, &data_, 16);  }


            };
            struct SensorsData
            {
                Pack * pack_;
                uint8_t & data_;
                Pack* unwrap_()
                {
                    Pack* ret = pack_;
                    pack_ = nullptr;
                    return ret;
                }


                struct values_
                {
                    uint8_t * bytes;
                    size_t length;
                    org::unirail::utils:: iterator<uint16_t, values_> begin() const { return {*this}; }
                    org::unirail::utils:: iterator<uint16_t, values_> end() const { return {*this, 1000 }; }

                    uint16_t get(size_t index) const {return (static_cast< uint16_t >(get_bytes(bytes,  index * 2, 2)));}


                    void set(uint16_t src, size_t index) const
                    {
                        set_bytes((src), 2, bytes,  index * 2);
                    }

                    static constexpr size_t len = 1000;

                };

                struct values_ values() const
                {
                    struct values_ ret {   &data_ + 0  };
                    return ret;
                }

                template< typename uint16_src_container >
                values_ values(const uint16_src_container & src) const
                {
                    auto first = src.begin();
                    struct values_ ret {   &data_ + 0  };
                    for(size_t i = 0 ; i < 1000 ; i++, ++first)
                        ret.set(*first, i);
                    return ret;
                }

                values_ values(uint16_t  * const src)  const { return  values(static_cast<uint16_t const *>(src)); }
                values_ values(uint16_t const * const src)  const
                {
                    struct values_ ret {   &data_ + 0  };
                    if(src)
                        for(size_t i = 0 ; i < 1000 ; i++)
                            ret.set(src[i], i);
                    return ret;
                }


            };





            void fill(const org::unirail::test::DeviceVersion & p0)  ;
            void on_DeviceVersion(const org::company::DeviceVersion & p0) ;
            void on_DeviceVersion(Pack* pack)
            {
                const org::company::DeviceVersion p0 = { pack, *pack->bytes };
                on_DeviceVersion(p0);
            };
            void on_Stop() {printf("Stop \n");}
            void on_Start() {printf("Start \n");}
            void on_GetDeviceVersion() {printf("GetDeviceVersion \n");}
            void on_GetConfiguration() {printf("GetConfiguration \n");}
            void on_SetConfiguration() {printf("SetConfiguration \n");}

            void fill(const org::company::BusConfiguration & p6)  ;
            void on_BusConfiguration(const org::company::BusConfiguration & p6) ;
            void on_BusConfiguration(Pack* pack)
            {
                const org::company::BusConfiguration p6 = { pack, *pack->bytes };
                on_BusConfiguration(p6);
            };

            void fill(const org::company::InstructionsPack & p7)  ;
            void on_InstructionsPack(const org::company::InstructionsPack & p7) ;
            void on_InstructionsPack(Pack* pack)
            {
                const org::company::InstructionsPack p7 = { pack, *pack->bytes };
                on_InstructionsPack(p7);
            };

            void fill(const org::unirail::test::DeviceError & p8)  ;
            void on_DeviceError(const org::company::DeviceError & p8) ;
            void on_DeviceError(Pack* pack)
            {
                const org::company::DeviceError p8 = { pack, *pack->bytes };
                on_DeviceError(p8);
            };

            void fill(const org::unirail::test::SensorsData & p9)  ;
            void on_SensorsData(const org::company::SensorsData & p9) ;
            void on_SensorsData(Pack* pack)
            {
                const org::company::SensorsData p9 = { pack, *pack->bytes };
                on_SensorsData(p9);
            };



            struct TEST_Channel
            {

                Pack* test_pack = nullptr;
                Transmitter transmitter{};
                static const Pack* pull_sending_pack(Transmitter* transmitter)
                {
                    Pack * pack = nullptr;
                    std::swap(pack, reinterpret_cast<TEST_Channel*>(reinterpret_cast<uint8_t*>(transmitter) - offsetof(TEST_Channel, transmitter))->test_pack);
                    return pack;
                }
                size_t  packs_into_bytes(uint8_t* dst, size_t dst_bytes) { return ::transmit(& transmitter, dst, dst_bytes);}
                size_t  packs_into_bytes_adv(uint8_t* dst, size_t dst_bytes) { return ::transmit_adv(& transmitter, dst, dst_bytes);}

                bool send(DeviceVersion & src)
                {
                    return  test_pack ? false : (test_pack =  src.unwrap_(), true);
                }

                bool send(DeviceError & src)
                {
                    return  test_pack ? false : (test_pack =  src.unwrap_(), true);
                }

                bool send(SensorsData & src)
                {
                    return  test_pack ? false : (test_pack =  src.unwrap_(), true);
                }


                struct NEW
                {
                    struct org::unirail::test::DeviceVersion DeviceVersion()
                    {
                        Pack* pack = new_pack(&org::company:: DeviceVersion::meta);
                        return {pack, *pack->bytes};
                    }
                    struct org::unirail::test::DeviceError DeviceError()
                    {
                        Pack* pack = new_pack(&org::company:: DeviceError::meta);
                        return {pack, *pack->bytes};
                    }
                    struct org::unirail::test::SensorsData SensorsData()
                    {
                        Pack* pack = new_pack(&org::company:: SensorsData::meta);
                        return {pack, *pack->bytes};
                    }

                } NEW;




                Receiver receiver{};

                static Meta const * dispatch(Receiver * receiver, size_t id, Pack * pack)
                {
                    using namespace org::unirail;
                    switch(id)
                    {
                        default:
                            return nullptr;
                        case 1:
                            return pack ? on_Stop(), nullptr : &org::company:: Stop::meta;
                        case 2:
                            return pack ? on_Start(), nullptr : &org::company:: Start::meta;
                        case 3:
                            return pack ? on_GetDeviceVersion(), nullptr : &org::company:: GetDeviceVersion::meta;
                        case 4:
                            return pack ? on_GetConfiguration(), nullptr : &org::company:: GetConfiguration::meta;
                        case 5:
                            return pack ? on_SetConfiguration(), nullptr : &org::company:: SetConfiguration::meta;
                    }
                }
                void  bytes_into_packs(uint8_t* src, size_t src_bytes) {  ::receive(src, src_bytes, &receiver);}
                void  bytes_into_packs_adv(uint8_t* src, size_t src_bytes) {  ::receive_adv(src, src_bytes, &receiver);}

                TEST_Channel()
                {
                    transmitter.pull = pull_sending_pack;
                    receiver.dispatch = dispatch;
                }

            };



            void fill(const org::unirail::test::DeviceVersion & p0)
            {
                p0.DevID(62480U) ;
                p0.Ver(39101U) ;
            }

            void on_DeviceVersion(const org::company::DeviceVersion & p0)
            {
                assert(p0.DevID() == 62480U);
                assert(p0.Ver() == 39101U);
                printf("DeviceVersion \n");
            }

            void fill(const org::company::BusConfiguration & p6)
            {
                p6.multiplier(63U) ;
                p6.time(39591U) ;
                p6.clk_khz(20886U) ;
            }

            void on_BusConfiguration(const org::company::BusConfiguration & p6)
            {
                assert(p6.multiplier() == 63U);
                assert(p6.time() == 39591U);
                assert(p6.clk_khz() == 20886U);
                printf("BusConfiguration \n");
            }

            void fill(const org::company::InstructionsPack & p7)
            {
                p7.Length(86U) ;
                for(const uint8_t src[] = { 191U, 233U, 78U, 96U, 49U, 190U, 196U, 226U, 229U, 150U, 145U, 246U, 188U, 238U, 87U, 15U, 143U, 139U, 242U, 134U, 202U, 44U, 166U, 173U, 47U, 20U, 3U, 208U, 71U, 89U, 50U, 132U, 37U, 47U, 9U, 176U, 89U, 184U, 108U, 80U, 80U, 109U, 59U, 190U, 155U, 191U, 63U, 228U, 30U, 109U, 9U, 81U, 252U, 205U, 144U, 171U, 157U, 203U, 116U, 135U, 188U, 73U, 174U, 155U, 225U, 51U, 47U, 60U, 13U, 132U, 98U, 47U, 39U, 90U, 84U, 115U, 159U, 41U, 45U, 190U, 172U, 74U, 205U, 136U, 150U, 79U, 241U, 168U, 80U, 46U, 46U, 97U, 185U, 214U, 225U, 75U, 100U, 167U, 220U, 148U, 200U, 110U, 83U, 200U, 155U, 229U, 144U, 143U, 166U, 17U, 44U, 160U, 212U, 11U, 109U, 106U, 14U, 134U, 255U, 50U, 162U, 147U, 165U, 131U, 175U, 145U, 70U, 110U, 62U, 250U, 234U, 149U, 30U, 74U, 21U, 248U, 25U, 150U, 149U, 73U, 56U, 87U, 113U, 158U, 99U, 222U, 106U, 132U, 55U, 160U, 88U, 238U, 116U, 184U, 20U, 39U, 6U, 139U, 122U, 151U, 5U, 201U, 234U, 67U, 236U, 108U, 60U, 8U, 125U, 115U, 210U, 171U, 60U, 214U, 185U, 86U, 151U, 75U, 120U, 52U, 231U, 126U, 176U, 172U, 217U, 26U, 77U, 146U, 229U, 205U, 104U, 214U, 188U, 113U, 56U, 25U, 73U, 4U, 239U, 168U, 65U, 198U, 191U, 232U, 141U, 245U, 89U, 189U, 243U, 119U, 126U, 165U, 190U, 11U, 26U, 3U, 154U, 133U, 143U, 74U, 92U, 221U, 5U, 79U, 181U, 236U, 126U, 0U, 230U, 170U, 37U, 109U, 45U, 168U, 238U, 163U, 215U, 10U, 158U, 195U, 163U, 153U, 32U, 57U, 188U, 81U, 120U, 112U, 53U, 55U, 97U, 202U, 104U, 88U, 237U, 239U } ; p7.Instructions(src), false;);
            }

            void on_InstructionsPack(const org::company::InstructionsPack & p7)
            {
                assert(p7.Length() == 86U);
                {
                    const uint8_t exemplary[] = { 191U, 233U, 78U, 96U, 49U, 190U, 196U, 226U, 229U, 150U, 145U, 246U, 188U, 238U, 87U, 15U, 143U, 139U, 242U, 134U, 202U, 44U, 166U, 173U, 47U, 20U, 3U, 208U, 71U, 89U, 50U, 132U, 37U, 47U, 9U, 176U, 89U, 184U, 108U, 80U, 80U, 109U, 59U, 190U, 155U, 191U, 63U, 228U, 30U, 109U, 9U, 81U, 252U, 205U, 144U, 171U, 157U, 203U, 116U, 135U, 188U, 73U, 174U, 155U, 225U, 51U, 47U, 60U, 13U, 132U, 98U, 47U, 39U, 90U, 84U, 115U, 159U, 41U, 45U, 190U, 172U, 74U, 205U, 136U, 150U, 79U, 241U, 168U, 80U, 46U, 46U, 97U, 185U, 214U, 225U, 75U, 100U, 167U, 220U, 148U, 200U, 110U, 83U, 200U, 155U, 229U, 144U, 143U, 166U, 17U, 44U, 160U, 212U, 11U, 109U, 106U, 14U, 134U, 255U, 50U, 162U, 147U, 165U, 131U, 175U, 145U, 70U, 110U, 62U, 250U, 234U, 149U, 30U, 74U, 21U, 248U, 25U, 150U, 149U, 73U, 56U, 87U, 113U, 158U, 99U, 222U, 106U, 132U, 55U, 160U, 88U, 238U, 116U, 184U, 20U, 39U, 6U, 139U, 122U, 151U, 5U, 201U, 234U, 67U, 236U, 108U, 60U, 8U, 125U, 115U, 210U, 171U, 60U, 214U, 185U, 86U, 151U, 75U, 120U, 52U, 231U, 126U, 176U, 172U, 217U, 26U, 77U, 146U, 229U, 205U, 104U, 214U, 188U, 113U, 56U, 25U, 73U, 4U, 239U, 168U, 65U, 198U, 191U, 232U, 141U, 245U, 89U, 189U, 243U, 119U, 126U, 165U, 190U, 11U, 26U, 3U, 154U, 133U, 143U, 74U, 92U, 221U, 5U, 79U, 181U, 236U, 126U, 0U, 230U, 170U, 37U, 109U, 45U, 168U, 238U, 163U, 215U, 10U, 158U, 195U, 163U, 153U, 32U, 57U, 188U, 81U, 120U, 112U, 53U, 55U, 97U, 202U, 104U, 88U, 237U, 239U }  ;
                    const auto sample = p7.Instructions();
                    for(size_t index = 0; index < org::company::InstructionsPack::Instructions_::len; index++)
                        assert(sample.get(index) == exemplary[index]);
                }
                printf("InstructionsPack \n");
            }

            void fill(const org::unirail::test::DeviceError & p8)
            {
                p8.param1(41440U) ;
                p8.error_id(Errors::SensorsDataOverflow) ;
            }

            void on_DeviceError(const org::company::DeviceError & p8)
            {
                assert(p8.param1() == 41440U);
                assert(p8.error_id() == Errors::SensorsDataOverflow);
                printf("DeviceError \n");
            }

            void fill(const org::unirail::test::SensorsData & p9)
            {
                for(const uint16_t src[] = { 49081U, 53509U, 11120U, 34500U, 45190U, 64201U, 50771U, 36794U, 51788U, 60966U, 52725U, 25826U, 62605U, 57835U, 56001U, 58368U, 8866U, 39624U, 18890U, 6116U, 17241U, 25552U, 44332U, 42298U, 47608U, 22618U, 54637U, 18944U, 58383U, 26423U, 14940U, 15909U, 65295U, 39374U, 43765U, 39223U, 64510U, 4182U, 60865U, 33756U, 30032U, 15087U, 18289U, 10328U, 61738U, 42542U, 36975U, 25849U, 45042U, 58602U, 5493U, 21094U, 13974U, 25009U, 395U, 48282U, 61349U, 23495U, 47786U, 2824U, 62594U, 11074U, 487U, 13964U, 17966U, 9287U, 32888U, 53252U, 43835U, 44525U, 56314U, 53476U, 33969U, 3244U, 61828U, 3971U, 3543U, 15790U, 3586U, 20991U, 8282U, 4447U, 4157U, 43605U, 29369U, 46578U, 268U, 38651U, 1233U, 62825U, 8570U, 24950U, 8500U, 37765U, 53143U, 60916U, 59298U, 37136U, 1967U, 41888U, 2410U, 27672U, 49324U, 9933U, 33501U, 4583U, 20463U, 56105U, 38568U, 7096U, 11861U, 8563U, 34787U, 16543U, 47261U, 22373U, 35342U, 22800U, 38676U, 36057U, 47267U, 38117U, 1072U, 28110U, 56706U, 10971U, 8321U, 64065U, 12355U, 386U, 26578U, 9689U, 35320U, 18695U, 54060U, 14291U, 47621U, 8970U, 11041U, 53695U, 41231U, 7382U, 17228U, 15891U, 43283U, 55602U, 4219U, 39074U, 63327U, 5936U, 30791U, 33248U, 41621U, 63430U, 41890U, 34896U, 8623U, 35490U, 6480U, 4994U, 36465U, 8372U, 10156U, 48775U, 12314U, 22210U, 44696U, 1554U, 32809U, 19315U, 36793U, 32786U, 65355U, 344U, 34583U, 21200U, 20245U, 36333U, 9918U, 4309U, 11584U, 62082U, 5883U, 48287U, 52613U, 56296U, 39597U, 10172U, 49508U, 41534U, 436U, 53155U, 26977U, 30535U, 2066U, 40588U, 24260U, 34190U, 38832U, 59283U, 18755U, 32614U, 65133U, 47408U, 49324U, 54341U, 56509U, 8822U, 19046U, 4488U, 2005U, 28449U, 22625U, 19779U, 31393U, 4013U, 54352U, 54225U, 28062U, 62343U, 51597U, 55870U, 64576U, 43366U, 56157U, 9884U, 18514U, 29266U, 36152U, 48164U, 61481U, 9671U, 41756U, 62671U, 40313U, 33557U, 10525U, 17980U, 5536U, 7228U, 33272U, 25253U, 12967U, 16362U, 43215U, 57605U, 10302U, 25466U, 51872U, 23781U, 49106U, 33479U, 7783U, 54955U, 53968U, 14120U, 52150U, 30917U, 50505U, 49132U, 11029U, 44206U, 47503U, 48056U, 36942U, 50941U, 51268U, 30682U, 50446U, 1006U, 701U, 54269U, 2960U, 5410U, 35857U, 14370U, 57001U, 26617U, 55946U, 15461U, 39588U, 54626U, 19846U, 31059U, 42234U, 57382U, 34521U, 44687U, 13010U, 15943U, 22503U, 8244U, 17171U, 5121U, 49390U, 36654U, 56729U, 15617U, 2382U, 6612U, 9205U, 57876U, 12551U, 36722U, 5318U, 54387U, 23041U, 18328U, 29424U, 20145U, 25449U, 37618U, 12678U, 38117U, 33211U, 23704U, 45207U, 36430U, 1027U, 47344U, 61955U, 30526U, 64315U, 46432U, 11024U, 6880U, 51544U, 29694U, 27953U, 39124U, 51201U, 31437U, 14986U, 34980U, 47412U, 59577U, 20660U, 14774U, 46260U, 8128U, 48884U, 16151U, 46470U, 46660U, 34052U, 62153U, 8050U, 22107U, 53858U, 2072U, 25701U, 7523U, 42759U, 36482U, 52277U, 10869U, 50630U, 40990U, 12430U, 44732U, 6233U, 47922U, 17767U, 59709U, 25050U, 8748U, 41513U, 1821U, 36331U, 63083U, 25503U, 24825U, 15073U, 23697U, 12916U, 39246U, 286U, 11162U, 33727U, 1743U, 24175U, 30934U, 58089U, 22371U, 22258U, 63709U, 17769U, 61661U, 44069U, 38864U, 5212U, 34772U, 41014U, 34550U, 24258U, 25371U, 36884U, 35113U, 18279U, 21132U, 34621U, 21311U, 20732U, 37147U, 65248U, 19175U, 4392U, 28177U, 24805U, 40859U, 46743U, 60630U, 22723U, 55039U, 23471U, 19814U, 64694U, 22265U, 44775U, 12604U, 45503U, 57905U, 31129U, 28772U, 42016U, 37828U, 45053U, 62287U, 7319U, 18702U, 9242U, 61204U, 9889U, 43244U, 62993U, 44024U, 20002U, 18185U, 47760U, 62229U, 29890U, 50150U, 47909U, 30475U, 22873U, 35042U, 18923U, 10143U, 59675U, 61188U, 20445U, 29590U, 3184U, 57241U, 41780U, 59292U, 23111U, 64855U, 20115U, 53819U, 60992U, 48862U, 5558U, 42398U, 27470U, 53684U, 24215U, 12982U, 63344U, 60762U, 55267U, 3744U, 57375U, 20312U, 32384U, 17474U, 39530U, 58405U, 65441U, 47471U, 42662U, 11340U, 16728U, 56130U, 60047U, 580U, 38551U, 30546U, 51854U, 2510U, 43918U, 28176U, 54179U, 50328U, 40335U, 30464U, 22461U, 34863U, 597U, 49282U, 59697U, 52004U, 44414U, 5814U, 18005U, 4572U, 62755U, 30314U, 36027U, 40663U, 21456U, 719U, 16857U, 26441U, 64397U, 26069U, 29137U, 9484U, 44499U, 29844U, 33807U, 63575U, 24039U, 53371U, 59883U, 50470U, 58198U, 3945U, 35740U, 49821U, 11117U, 61973U, 47084U, 30436U, 33375U, 3267U, 28906U, 8675U, 13328U, 51130U, 8862U, 1048U, 55822U, 11241U, 50313U, 19702U, 40964U, 2870U, 51315U, 57692U, 28514U, 17868U, 17169U, 53477U, 22131U, 5131U, 28709U, 50758U, 63847U, 50696U, 62156U, 36078U, 53515U, 49486U, 38868U, 48027U, 49410U, 43885U, 12150U, 12410U, 17283U, 23533U, 37152U, 25181U, 38031U, 22300U, 52365U, 36357U, 23045U, 4709U, 170U, 59898U, 34010U, 42552U, 26014U, 54789U, 13864U, 35031U, 2985U, 49151U, 9701U, 39908U, 61271U, 42539U, 45282U, 49277U, 45811U, 1259U, 44997U, 15283U, 7381U, 39303U, 64858U, 7959U, 6102U, 49535U, 3316U, 62222U, 22173U, 10972U, 7186U, 13878U, 54612U, 13031U, 15301U, 11533U, 10369U, 18413U, 48591U, 51282U, 55226U, 11439U, 14276U, 9071U, 55752U, 23906U, 59882U, 28253U, 49642U, 59988U, 10112U, 13768U, 52702U, 32979U, 14193U, 64161U, 46344U, 32211U, 49650U, 58624U, 57259U, 45547U, 47924U, 53090U, 11691U, 20762U, 35891U, 43486U, 33078U, 38272U, 7389U, 7819U, 23024U, 61572U, 61064U, 36052U, 5628U, 8583U, 36804U, 32351U, 35319U, 48010U, 48490U, 52704U, 7874U, 10597U, 40202U, 63154U, 40568U, 23181U, 60726U, 42609U, 52665U, 55172U, 58132U, 7652U, 37987U, 60043U, 36897U, 5667U, 19037U, 51986U, 13135U, 9526U, 4433U, 218U, 48195U, 48812U, 50097U, 1766U, 252U, 56774U, 28337U, 25147U, 57649U, 3621U, 21542U, 203U, 37294U, 31189U, 32148U, 5660U, 49089U, 58346U, 53832U, 12384U, 39683U, 15540U, 26688U, 2157U, 39915U, 22888U, 1604U, 33503U, 7635U, 17725U, 22848U, 40597U, 21010U, 56491U, 64505U, 13594U, 39059U, 27226U, 38212U, 11704U, 19147U, 57335U, 31267U, 38887U, 7255U, 50684U, 26245U, 24629U, 4180U, 47280U, 47556U, 8072U, 60383U, 62666U, 30030U, 38191U, 20054U, 61868U, 41669U, 37080U, 46185U, 54055U, 63420U, 50170U, 53920U, 18385U, 41255U, 35327U, 33668U, 20150U, 29646U, 65139U, 48934U, 63882U, 1257U, 30876U, 14305U, 3761U, 36644U, 13384U, 32972U, 35699U, 15661U, 22293U, 58654U, 10414U, 24689U, 46587U, 36649U, 57041U, 24813U, 37633U, 17252U, 55097U, 12588U, 5559U, 35630U, 412U, 23238U, 56994U, 8574U, 28006U, 43093U, 961U, 4793U, 48332U, 64295U, 37307U, 52553U, 6911U, 37373U, 59546U, 23999U, 30384U, 10223U, 17741U, 8667U, 23822U, 5277U, 65029U, 16026U, 17149U, 12213U, 47276U, 47143U, 62373U, 33354U, 30424U, 18313U, 34546U, 756U, 44898U, 38974U, 22182U, 49577U, 26103U, 34980U, 1016U, 56330U, 64895U, 11254U, 34552U, 56891U, 47745U, 35974U, 3828U, 15365U, 8217U, 52091U, 6218U, 32585U, 14305U, 27433U, 27754U, 9855U, 37281U, 24938U, 52215U, 53826U, 37913U, 22059U, 50667U, 51664U, 44468U, 38205U, 62312U, 38962U, 46603U, 18464U, 53332U, 33426U, 53012U, 43714U, 40347U, 37017U, 57139U, 5794U, 51450U, 26180U, 22383U, 6629U, 6969U, 21233U, 46109U, 16368U, 2049U, 8298U, 20616U, 19586U, 34717U, 1564U, 8192U, 12870U, 33353U, 54307U, 13275U, 60800U, 35027U, 44078U, 13526U, 58714U, 54348U, 24475U, 29803U, 38777U, 40038U, 23995U, 64659U, 60214U, 32002U, 18708U, 35063U, 41431U, 42502U, 35415U, 3322U, 42577U, 37642U, 23211U, 64804U, 56895U, 47594U, 6978U, 28379U, 40831U, 1288U, 26180U, 11042U, 44652U, 8018U, 41102U, 19484U, 53045U, 23603U, 47369U, 65510U, 45441U, 15204U, 4996U, 16502U, 63948U, 23171U, 57700U, 48266U, 19499U, 2284U, 60635U, 28595U, 16456U, 59123U, 30260U, 45021U, 52734U, 50756U, 45578U, 42926U, 20959U, 42407U, 14757U, 41080U, 35720U, 51556U, 25843U, 29678U, 33190U, 48160U, 55891U, 54533U, 62488U, 31141U, 64541U, 48994U, 38849U, 32094U, 24175U, 64805U, 17862U, 51488U, 29120U, 37840U, 39782U, 47969U, 45228U, 36535U, 960U, 15050U, 23848U, 57084U, 58285U, 40247U, 4479U, 19860U, 58998U, 40315U, 46961U, 46944U, 42560U, 13202U, 23710U, 50716U, 61748U, 56716U, 13920U, 64784U, 201U, 38072U, 62207U, 63425U, 37794U, 64925U, 3824U, 6983U, 59545U, 54365U } ; p9.values(src), false;);
            }

            void on_SensorsData(const org::company::SensorsData & p9)
            {
                {
                    const uint16_t exemplary[] = { 49081U, 53509U, 11120U, 34500U, 45190U, 64201U, 50771U, 36794U, 51788U, 60966U, 52725U, 25826U, 62605U, 57835U, 56001U, 58368U, 8866U, 39624U, 18890U, 6116U, 17241U, 25552U, 44332U, 42298U, 47608U, 22618U, 54637U, 18944U, 58383U, 26423U, 14940U, 15909U, 65295U, 39374U, 43765U, 39223U, 64510U, 4182U, 60865U, 33756U, 30032U, 15087U, 18289U, 10328U, 61738U, 42542U, 36975U, 25849U, 45042U, 58602U, 5493U, 21094U, 13974U, 25009U, 395U, 48282U, 61349U, 23495U, 47786U, 2824U, 62594U, 11074U, 487U, 13964U, 17966U, 9287U, 32888U, 53252U, 43835U, 44525U, 56314U, 53476U, 33969U, 3244U, 61828U, 3971U, 3543U, 15790U, 3586U, 20991U, 8282U, 4447U, 4157U, 43605U, 29369U, 46578U, 268U, 38651U, 1233U, 62825U, 8570U, 24950U, 8500U, 37765U, 53143U, 60916U, 59298U, 37136U, 1967U, 41888U, 2410U, 27672U, 49324U, 9933U, 33501U, 4583U, 20463U, 56105U, 38568U, 7096U, 11861U, 8563U, 34787U, 16543U, 47261U, 22373U, 35342U, 22800U, 38676U, 36057U, 47267U, 38117U, 1072U, 28110U, 56706U, 10971U, 8321U, 64065U, 12355U, 386U, 26578U, 9689U, 35320U, 18695U, 54060U, 14291U, 47621U, 8970U, 11041U, 53695U, 41231U, 7382U, 17228U, 15891U, 43283U, 55602U, 4219U, 39074U, 63327U, 5936U, 30791U, 33248U, 41621U, 63430U, 41890U, 34896U, 8623U, 35490U, 6480U, 4994U, 36465U, 8372U, 10156U, 48775U, 12314U, 22210U, 44696U, 1554U, 32809U, 19315U, 36793U, 32786U, 65355U, 344U, 34583U, 21200U, 20245U, 36333U, 9918U, 4309U, 11584U, 62082U, 5883U, 48287U, 52613U, 56296U, 39597U, 10172U, 49508U, 41534U, 436U, 53155U, 26977U, 30535U, 2066U, 40588U, 24260U, 34190U, 38832U, 59283U, 18755U, 32614U, 65133U, 47408U, 49324U, 54341U, 56509U, 8822U, 19046U, 4488U, 2005U, 28449U, 22625U, 19779U, 31393U, 4013U, 54352U, 54225U, 28062U, 62343U, 51597U, 55870U, 64576U, 43366U, 56157U, 9884U, 18514U, 29266U, 36152U, 48164U, 61481U, 9671U, 41756U, 62671U, 40313U, 33557U, 10525U, 17980U, 5536U, 7228U, 33272U, 25253U, 12967U, 16362U, 43215U, 57605U, 10302U, 25466U, 51872U, 23781U, 49106U, 33479U, 7783U, 54955U, 53968U, 14120U, 52150U, 30917U, 50505U, 49132U, 11029U, 44206U, 47503U, 48056U, 36942U, 50941U, 51268U, 30682U, 50446U, 1006U, 701U, 54269U, 2960U, 5410U, 35857U, 14370U, 57001U, 26617U, 55946U, 15461U, 39588U, 54626U, 19846U, 31059U, 42234U, 57382U, 34521U, 44687U, 13010U, 15943U, 22503U, 8244U, 17171U, 5121U, 49390U, 36654U, 56729U, 15617U, 2382U, 6612U, 9205U, 57876U, 12551U, 36722U, 5318U, 54387U, 23041U, 18328U, 29424U, 20145U, 25449U, 37618U, 12678U, 38117U, 33211U, 23704U, 45207U, 36430U, 1027U, 47344U, 61955U, 30526U, 64315U, 46432U, 11024U, 6880U, 51544U, 29694U, 27953U, 39124U, 51201U, 31437U, 14986U, 34980U, 47412U, 59577U, 20660U, 14774U, 46260U, 8128U, 48884U, 16151U, 46470U, 46660U, 34052U, 62153U, 8050U, 22107U, 53858U, 2072U, 25701U, 7523U, 42759U, 36482U, 52277U, 10869U, 50630U, 40990U, 12430U, 44732U, 6233U, 47922U, 17767U, 59709U, 25050U, 8748U, 41513U, 1821U, 36331U, 63083U, 25503U, 24825U, 15073U, 23697U, 12916U, 39246U, 286U, 11162U, 33727U, 1743U, 24175U, 30934U, 58089U, 22371U, 22258U, 63709U, 17769U, 61661U, 44069U, 38864U, 5212U, 34772U, 41014U, 34550U, 24258U, 25371U, 36884U, 35113U, 18279U, 21132U, 34621U, 21311U, 20732U, 37147U, 65248U, 19175U, 4392U, 28177U, 24805U, 40859U, 46743U, 60630U, 22723U, 55039U, 23471U, 19814U, 64694U, 22265U, 44775U, 12604U, 45503U, 57905U, 31129U, 28772U, 42016U, 37828U, 45053U, 62287U, 7319U, 18702U, 9242U, 61204U, 9889U, 43244U, 62993U, 44024U, 20002U, 18185U, 47760U, 62229U, 29890U, 50150U, 47909U, 30475U, 22873U, 35042U, 18923U, 10143U, 59675U, 61188U, 20445U, 29590U, 3184U, 57241U, 41780U, 59292U, 23111U, 64855U, 20115U, 53819U, 60992U, 48862U, 5558U, 42398U, 27470U, 53684U, 24215U, 12982U, 63344U, 60762U, 55267U, 3744U, 57375U, 20312U, 32384U, 17474U, 39530U, 58405U, 65441U, 47471U, 42662U, 11340U, 16728U, 56130U, 60047U, 580U, 38551U, 30546U, 51854U, 2510U, 43918U, 28176U, 54179U, 50328U, 40335U, 30464U, 22461U, 34863U, 597U, 49282U, 59697U, 52004U, 44414U, 5814U, 18005U, 4572U, 62755U, 30314U, 36027U, 40663U, 21456U, 719U, 16857U, 26441U, 64397U, 26069U, 29137U, 9484U, 44499U, 29844U, 33807U, 63575U, 24039U, 53371U, 59883U, 50470U, 58198U, 3945U, 35740U, 49821U, 11117U, 61973U, 47084U, 30436U, 33375U, 3267U, 28906U, 8675U, 13328U, 51130U, 8862U, 1048U, 55822U, 11241U, 50313U, 19702U, 40964U, 2870U, 51315U, 57692U, 28514U, 17868U, 17169U, 53477U, 22131U, 5131U, 28709U, 50758U, 63847U, 50696U, 62156U, 36078U, 53515U, 49486U, 38868U, 48027U, 49410U, 43885U, 12150U, 12410U, 17283U, 23533U, 37152U, 25181U, 38031U, 22300U, 52365U, 36357U, 23045U, 4709U, 170U, 59898U, 34010U, 42552U, 26014U, 54789U, 13864U, 35031U, 2985U, 49151U, 9701U, 39908U, 61271U, 42539U, 45282U, 49277U, 45811U, 1259U, 44997U, 15283U, 7381U, 39303U, 64858U, 7959U, 6102U, 49535U, 3316U, 62222U, 22173U, 10972U, 7186U, 13878U, 54612U, 13031U, 15301U, 11533U, 10369U, 18413U, 48591U, 51282U, 55226U, 11439U, 14276U, 9071U, 55752U, 23906U, 59882U, 28253U, 49642U, 59988U, 10112U, 13768U, 52702U, 32979U, 14193U, 64161U, 46344U, 32211U, 49650U, 58624U, 57259U, 45547U, 47924U, 53090U, 11691U, 20762U, 35891U, 43486U, 33078U, 38272U, 7389U, 7819U, 23024U, 61572U, 61064U, 36052U, 5628U, 8583U, 36804U, 32351U, 35319U, 48010U, 48490U, 52704U, 7874U, 10597U, 40202U, 63154U, 40568U, 23181U, 60726U, 42609U, 52665U, 55172U, 58132U, 7652U, 37987U, 60043U, 36897U, 5667U, 19037U, 51986U, 13135U, 9526U, 4433U, 218U, 48195U, 48812U, 50097U, 1766U, 252U, 56774U, 28337U, 25147U, 57649U, 3621U, 21542U, 203U, 37294U, 31189U, 32148U, 5660U, 49089U, 58346U, 53832U, 12384U, 39683U, 15540U, 26688U, 2157U, 39915U, 22888U, 1604U, 33503U, 7635U, 17725U, 22848U, 40597U, 21010U, 56491U, 64505U, 13594U, 39059U, 27226U, 38212U, 11704U, 19147U, 57335U, 31267U, 38887U, 7255U, 50684U, 26245U, 24629U, 4180U, 47280U, 47556U, 8072U, 60383U, 62666U, 30030U, 38191U, 20054U, 61868U, 41669U, 37080U, 46185U, 54055U, 63420U, 50170U, 53920U, 18385U, 41255U, 35327U, 33668U, 20150U, 29646U, 65139U, 48934U, 63882U, 1257U, 30876U, 14305U, 3761U, 36644U, 13384U, 32972U, 35699U, 15661U, 22293U, 58654U, 10414U, 24689U, 46587U, 36649U, 57041U, 24813U, 37633U, 17252U, 55097U, 12588U, 5559U, 35630U, 412U, 23238U, 56994U, 8574U, 28006U, 43093U, 961U, 4793U, 48332U, 64295U, 37307U, 52553U, 6911U, 37373U, 59546U, 23999U, 30384U, 10223U, 17741U, 8667U, 23822U, 5277U, 65029U, 16026U, 17149U, 12213U, 47276U, 47143U, 62373U, 33354U, 30424U, 18313U, 34546U, 756U, 44898U, 38974U, 22182U, 49577U, 26103U, 34980U, 1016U, 56330U, 64895U, 11254U, 34552U, 56891U, 47745U, 35974U, 3828U, 15365U, 8217U, 52091U, 6218U, 32585U, 14305U, 27433U, 27754U, 9855U, 37281U, 24938U, 52215U, 53826U, 37913U, 22059U, 50667U, 51664U, 44468U, 38205U, 62312U, 38962U, 46603U, 18464U, 53332U, 33426U, 53012U, 43714U, 40347U, 37017U, 57139U, 5794U, 51450U, 26180U, 22383U, 6629U, 6969U, 21233U, 46109U, 16368U, 2049U, 8298U, 20616U, 19586U, 34717U, 1564U, 8192U, 12870U, 33353U, 54307U, 13275U, 60800U, 35027U, 44078U, 13526U, 58714U, 54348U, 24475U, 29803U, 38777U, 40038U, 23995U, 64659U, 60214U, 32002U, 18708U, 35063U, 41431U, 42502U, 35415U, 3322U, 42577U, 37642U, 23211U, 64804U, 56895U, 47594U, 6978U, 28379U, 40831U, 1288U, 26180U, 11042U, 44652U, 8018U, 41102U, 19484U, 53045U, 23603U, 47369U, 65510U, 45441U, 15204U, 4996U, 16502U, 63948U, 23171U, 57700U, 48266U, 19499U, 2284U, 60635U, 28595U, 16456U, 59123U, 30260U, 45021U, 52734U, 50756U, 45578U, 42926U, 20959U, 42407U, 14757U, 41080U, 35720U, 51556U, 25843U, 29678U, 33190U, 48160U, 55891U, 54533U, 62488U, 31141U, 64541U, 48994U, 38849U, 32094U, 24175U, 64805U, 17862U, 51488U, 29120U, 37840U, 39782U, 47969U, 45228U, 36535U, 960U, 15050U, 23848U, 57084U, 58285U, 40247U, 4479U, 19860U, 58998U, 40315U, 46961U, 46944U, 42560U, 13202U, 23710U, 50716U, 61748U, 56716U, 13920U, 64784U, 201U, 38072U, 62207U, 63425U, 37794U, 64925U, 3824U, 6983U, 59545U, 54365U }  ;
                    const auto sample = p9.values();
                    for(size_t index = 0; index < org::company::SensorsData::values_::len; index++)
                        assert(sample.get(index) == exemplary[index]);
                }
                printf("SensorsData \n");
            }



            struct Communication_test
            {
                Communication<Communication_test> channel;
                void on_DeviceVersion(Pack * pack)
                {
                    ::org::unirail::test::on_DeviceVersion(pack);
                    printf("DeviceVersion pass\n");
                }
                void on_BusConfiguration(Pack * pack)
                {
                    ::org::unirail::test::on_BusConfiguration(pack);
                    printf("BusConfiguration pass\n");
                }
                void on_InstructionsPack(Pack * pack)
                {
                    ::org::unirail::test::on_InstructionsPack(pack);
                    printf("InstructionsPack pass\n");
                }
                void on_DeviceError(Pack * pack)
                {
                    ::org::unirail::test::on_DeviceError(pack);
                    printf("DeviceError pass\n");
                }
                void on_SensorsData(Pack * pack)
                {
                    ::org::unirail::test::on_SensorsData(pack);
                    printf("SensorsData pass\n");
                }
                static  Communication_test * from(Receiver * receiver)
                {
                    return reinterpret_cast<Communication_test*>(reinterpret_cast<uint8_t*>(receiver) - offsetof(Communication_test, channel) - offsetof(Communication<Communication_test>, receiver));
                }

                Pack* test_pack = nullptr;
                static const Pack* pull_sending_pack(Transmitter* transmitter)
                {
                    Pack * pack = nullptr;
                    std::swap(pack, reinterpret_cast<Communication_test*>(reinterpret_cast<uint8_t*>(transmitter) - offsetof(Communication_test, channel) - offsetof(Communication<Communication_test>, transmitter))->test_pack);
                    return pack;
                }
                static bool push_sending_pack(Transmitter* transmitter, Pack* pack)
                {
                    Communication_test* ch = reinterpret_cast<Communication_test*>(reinterpret_cast<uint8_t*>(transmitter) - offsetof(Communication_test, channel) - offsetof(Communication<Communication_test>, transmitter));
                    return ch->test_pack ? false : (ch->test_pack = pack, true);
                }

            };

        }

        namespace adapter
        {

            struct SensorsData
            {
                struct Push_dst_;

            };


            struct DeviceError
            {
                struct Push_dst_;

            };


            struct InstructionsPack
            {
                struct Push_dst_;
                struct Pull_src_;
            };


            struct BusConfiguration
            {
                struct Push_dst_;
                struct Pull_src_;
            };


            struct DeviceVersion
            {
                struct Push_dst_;

            };



            struct DeviceVersion::Push_dst_
            {
                const org::unirail::test::DeviceVersion & dst;
                Push_dst_(const org::unirail::test::DeviceVersion& dst) : dst(dst) {}

                void DevID(uint16_t src) const {dst.DevID(src);}
                void Ver(uint16_t src) const {dst.Ver(src);}

            };


            struct BusConfiguration::Push_dst_
            {
                const org::company::BusConfiguration & dst;
                Push_dst_(const org::company::BusConfiguration& dst) : dst(dst) {}

                void multiplier(uint8_t src) const {dst.multiplier(src);}
                void time(uint16_t src) const {dst.time(src);}
                void clk_khz(uint16_t src) const {dst.clk_khz(src);}

            };

            struct BusConfiguration::Pull_src_
            {
                const org::company:: BusConfiguration & src;
                Pull_src_(const org::company:: BusConfiguration& src) : src(src) {}

                uint8_t multiplier() const { return src.multiplier(); }
                uint16_t time() const { return src.time(); }
                uint16_t clk_khz() const { return src.clk_khz(); }

            };


            struct InstructionsPack::Push_dst_
            {
                const org::company::InstructionsPack & dst;
                Push_dst_(const org::company::InstructionsPack& dst) : dst(dst) {}

                void Length(uint8_t src) const {dst.Length(src);}
                void Instructions(const struct org::company::InstructionsPack :: Instructions_  & src) const
                {
                    dst.Instructions(src);
                }

            };

            struct InstructionsPack::Pull_src_
            {
                const org::company:: InstructionsPack & src;
                Pull_src_(const org::company:: InstructionsPack& src) : src(src) {}

                uint8_t Length() const { return src.Length(); }
                struct org::company:: InstructionsPack::Instructions_  Instructions()
                {
                    return { src.Instructions() };
                }

            };


            struct DeviceError::Push_dst_
            {
                const org::unirail::test::DeviceError & dst;
                Push_dst_(const org::unirail::test::DeviceError& dst) : dst(dst) {}

                void error_id(enum org::company:: Errors src) const {dst.error_id(src);}
                void param1(uint16_t src) const {dst.param1(src);}

            };


            struct SensorsData::Push_dst_
            {
                const org::unirail::test::SensorsData & dst;
                Push_dst_(const org::unirail::test::SensorsData& dst) : dst(dst) {}

                void values(const struct org::company::SensorsData :: values_  & src) const
                {
                    dst.values(src);
                }

            };


        }

    }
}

int main()
{
    using namespace org::unirail::test;
    static uint8_t buff[512];
    CURSORS(cur);
    CURSORS(cur_dst);
    CURSORS(cur_src);
    Communication_test ch_Communication{};
    TEST_Channel ch_TEST_Channel{};
    {
        auto p0 = ch_TEST_Channel.NEW.DeviceVersion() ;
        fill(p0);
        {
            auto dst = ch_TEST_Channel.NEW.DeviceVersion();
            org::company::DeviceVersion  src{ nullptr, p0.data_ };
            src.push_data_(org::unirail::adapter::DeviceVersion::Push_dst_(dst));
            assert(memcmp(&src.data_, &dst.data_, 4) == 0);
            free_pack(dst.unwrap_());
        }
        if(ch_TEST_Channel.send(p0))
            for(size_t len; (len = ch_TEST_Channel.packs_into_bytes(buff, sizeof buff));)
                ch_Communication.channel.bytes_into_packs(buff,  len);
        else
        {
            free_pack(p0.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    {
        if(ch_Communication.channel.send_Stop())
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff,  sizeof buff));)
                ch_TEST_Channel.bytes_into_packs(buff, len);
        else  assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
    }
    {
        if(ch_Communication.channel.send_Start())
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff,  sizeof buff));)
                ch_TEST_Channel.bytes_into_packs(buff, len);
        else  assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
    }
    {
        if(ch_Communication.channel.send_GetDeviceVersion())
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff,  sizeof buff));)
                ch_TEST_Channel.bytes_into_packs(buff, len);
        else  assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
    }
    {
        if(ch_Communication.channel.send_GetConfiguration())
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff,  sizeof buff));)
                ch_TEST_Channel.bytes_into_packs(buff, len);
        else  assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
    }
    {
        if(ch_Communication.channel.send_SetConfiguration())
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff,  sizeof buff));)
                ch_TEST_Channel.bytes_into_packs(buff, len);
        else  assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
    }
    {
        auto p6 = ch_Communication.channel.NEW.BusConfiguration() ;
        fill(p6);
        {
            {
                auto dst = ch_Communication.channel.NEW.BusConfiguration();
                p6.push_data_(org::unirail::adapter::BusConfiguration::Push_dst_(dst));
                assert(memcmp(&p6.data_, &dst.data_, 5) == 0);
                free_pack(dst.unwrap_());
            }
            auto dst = ch_Communication.channel.NEW.BusConfiguration();
            dst.pull_data_(org::unirail::adapter::BusConfiguration::Pull_src_(p6));
            assert(memcmp(&p6.data_, &dst.data_, 5) == 0);
            free_pack(dst.unwrap_());
        }
        if(ch_Communication.channel.send(p6))
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff, sizeof buff));)
                ch_Communication.channel.bytes_into_packs(buff,  len);
        else
        {
            free_pack(p6.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    {
        auto p7 = ch_Communication.channel.NEW.InstructionsPack() ;
        fill(p7);
        {
            {
                auto dst = ch_Communication.channel.NEW.InstructionsPack();
                p7.push_data_(org::unirail::adapter::InstructionsPack::Push_dst_(dst));
                assert(memcmp(&p7.data_, &dst.data_, 257) == 0);
                free_pack(dst.unwrap_());
            }
            auto dst = ch_Communication.channel.NEW.InstructionsPack();
            dst.pull_data_(org::unirail::adapter::InstructionsPack::Pull_src_(p7));
            assert(memcmp(&p7.data_, &dst.data_, 257) == 0);
            free_pack(dst.unwrap_());
        }
        if(ch_Communication.channel.send(p7))
            for(size_t len; (len = ch_Communication.channel.packs_into_bytes(buff, sizeof buff));)
                ch_Communication.channel.bytes_into_packs(buff,  len);
        else
        {
            free_pack(p7.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    {
        auto p8 = ch_TEST_Channel.NEW.DeviceError() ;
        fill(p8);
        {
            auto dst = ch_TEST_Channel.NEW.DeviceError();
            org::company::DeviceError  src{ nullptr, p8.data_ };
            src.push_data_(org::unirail::adapter::DeviceError::Push_dst_(dst));
            assert(memcmp(&src.data_, &dst.data_, 3) == 0);
            free_pack(dst.unwrap_());
        }
        if(ch_TEST_Channel.send(p8))
            for(size_t len; (len = ch_TEST_Channel.packs_into_bytes(buff, sizeof buff));)
                ch_Communication.channel.bytes_into_packs(buff,  len);
        else
        {
            free_pack(p8.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
    {
        auto p9 = ch_TEST_Channel.NEW.SensorsData() ;
        fill(p9);
        {
            auto dst = ch_TEST_Channel.NEW.SensorsData();
            org::company::SensorsData  src{ nullptr, p9.data_ };
            src.push_data_(org::unirail::adapter::SensorsData::Push_dst_(dst));
            assert(memcmp(&src.data_, &dst.data_, 2000) == 0);
            free_pack(dst.unwrap_());
        }
        if(ch_TEST_Channel.send(p9))
            for(size_t len; (len = ch_TEST_Channel.packs_into_bytes(buff, sizeof buff));)
                ch_Communication.channel.bytes_into_packs(buff,  len);
        else
        {
            free_pack(p9.unwrap_());
            assert("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
}

