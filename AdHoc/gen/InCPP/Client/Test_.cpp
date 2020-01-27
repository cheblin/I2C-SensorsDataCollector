
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
                p0.DevID(603U) ;
                p0.Ver(26305U) ;
            }

            void on_DeviceVersion(const org::company::DeviceVersion & p0)
            {
                assert(p0.DevID() == 603U);
                assert(p0.Ver() == 26305U);
                printf("DeviceVersion \n");
            }

            void fill(const org::company::BusConfiguration & p6)
            {
                p6.multiplier(217U) ;
                p6.time(44298U) ;
                p6.clk_khz(55283U) ;
            }

            void on_BusConfiguration(const org::company::BusConfiguration & p6)
            {
                assert(p6.multiplier() == 217U);
                assert(p6.time() == 44298U);
                assert(p6.clk_khz() == 55283U);
                printf("BusConfiguration \n");
            }

            void fill(const org::company::InstructionsPack & p7)
            {
                p7.Length(66U) ;
                for(const uint8_t src[] = { 157U, 42U, 140U, 215U, 111U, 170U, 68U, 186U, 241U, 252U, 24U, 250U, 221U, 169U, 189U, 104U, 208U, 224U, 247U, 242U, 2U, 187U, 88U, 187U, 245U, 50U, 111U, 250U, 43U, 73U, 180U, 89U, 42U, 197U, 93U, 15U, 140U, 214U, 234U, 201U, 94U, 216U, 128U, 84U, 64U, 58U, 252U, 220U, 186U, 97U, 183U, 186U, 75U, 5U, 244U, 98U, 116U, 150U, 204U, 85U, 17U, 48U, 162U, 53U, 99U, 208U, 84U, 218U, 26U, 47U, 250U, 115U, 208U, 146U, 83U, 212U, 250U, 40U, 39U, 238U, 236U, 40U, 98U, 53U, 206U, 229U, 26U, 50U, 58U, 105U, 27U, 38U, 123U, 69U, 232U, 4U, 5U, 207U, 118U, 150U, 190U, 101U, 72U, 130U, 109U, 154U, 131U, 212U, 181U, 176U, 130U, 196U, 151U, 194U, 84U, 150U, 164U, 39U, 251U, 170U, 72U, 112U, 122U, 186U, 17U, 209U, 133U, 250U, 146U, 129U, 80U, 178U, 14U, 104U, 196U, 68U, 116U, 152U, 6U, 142U, 82U, 232U, 209U, 100U, 167U, 113U, 122U, 27U, 203U, 214U, 184U, 250U, 130U, 46U, 222U, 119U, 119U, 60U, 181U, 81U, 0U, 78U, 22U, 254U, 196U, 9U, 223U, 72U, 101U, 111U, 197U, 114U, 48U, 144U, 31U, 39U, 244U, 45U, 33U, 79U, 169U, 154U, 76U, 74U, 15U, 74U, 254U, 127U, 189U, 130U, 243U, 250U, 150U, 92U, 90U, 100U, 27U, 107U, 197U, 219U, 229U, 197U, 216U, 168U, 122U, 146U, 248U, 127U, 224U, 153U, 153U, 36U, 80U, 136U, 198U, 44U, 128U, 102U, 162U, 147U, 37U, 175U, 253U, 190U, 44U, 194U, 230U, 195U, 15U, 187U, 41U, 145U, 136U, 156U, 227U, 143U, 35U, 122U, 22U, 73U, 91U, 127U, 178U, 150U, 229U, 71U, 135U, 95U, 160U, 180U, 68U, 62U, 145U, 69U, 150U, 27U } ; p7.Instructions(src), false;);
            }

            void on_InstructionsPack(const org::company::InstructionsPack & p7)
            {
                assert(p7.Length() == 66U);
                {
                    const uint8_t exemplary[] = { 157U, 42U, 140U, 215U, 111U, 170U, 68U, 186U, 241U, 252U, 24U, 250U, 221U, 169U, 189U, 104U, 208U, 224U, 247U, 242U, 2U, 187U, 88U, 187U, 245U, 50U, 111U, 250U, 43U, 73U, 180U, 89U, 42U, 197U, 93U, 15U, 140U, 214U, 234U, 201U, 94U, 216U, 128U, 84U, 64U, 58U, 252U, 220U, 186U, 97U, 183U, 186U, 75U, 5U, 244U, 98U, 116U, 150U, 204U, 85U, 17U, 48U, 162U, 53U, 99U, 208U, 84U, 218U, 26U, 47U, 250U, 115U, 208U, 146U, 83U, 212U, 250U, 40U, 39U, 238U, 236U, 40U, 98U, 53U, 206U, 229U, 26U, 50U, 58U, 105U, 27U, 38U, 123U, 69U, 232U, 4U, 5U, 207U, 118U, 150U, 190U, 101U, 72U, 130U, 109U, 154U, 131U, 212U, 181U, 176U, 130U, 196U, 151U, 194U, 84U, 150U, 164U, 39U, 251U, 170U, 72U, 112U, 122U, 186U, 17U, 209U, 133U, 250U, 146U, 129U, 80U, 178U, 14U, 104U, 196U, 68U, 116U, 152U, 6U, 142U, 82U, 232U, 209U, 100U, 167U, 113U, 122U, 27U, 203U, 214U, 184U, 250U, 130U, 46U, 222U, 119U, 119U, 60U, 181U, 81U, 0U, 78U, 22U, 254U, 196U, 9U, 223U, 72U, 101U, 111U, 197U, 114U, 48U, 144U, 31U, 39U, 244U, 45U, 33U, 79U, 169U, 154U, 76U, 74U, 15U, 74U, 254U, 127U, 189U, 130U, 243U, 250U, 150U, 92U, 90U, 100U, 27U, 107U, 197U, 219U, 229U, 197U, 216U, 168U, 122U, 146U, 248U, 127U, 224U, 153U, 153U, 36U, 80U, 136U, 198U, 44U, 128U, 102U, 162U, 147U, 37U, 175U, 253U, 190U, 44U, 194U, 230U, 195U, 15U, 187U, 41U, 145U, 136U, 156U, 227U, 143U, 35U, 122U, 22U, 73U, 91U, 127U, 178U, 150U, 229U, 71U, 135U, 95U, 160U, 180U, 68U, 62U, 145U, 69U, 150U, 27U }  ;
                    const auto sample = p7.Instructions();
                    for(size_t index = 0; index < org::company::InstructionsPack::Instructions_::len; index++)
                        assert(sample.get(index) == exemplary[index]);
                }
                printf("InstructionsPack \n");
            }

            void fill(const org::unirail::test::DeviceError & p8)
            {
                p8.param1(37758U) ;
                p8.error_id(Errors::SensorsDataOverflow) ;
            }

            void on_DeviceError(const org::company::DeviceError & p8)
            {
                assert(p8.param1() == 37758U);
                assert(p8.error_id() == Errors::SensorsDataOverflow);
                printf("DeviceError \n");
            }

            void fill(const org::unirail::test::SensorsData & p9)
            {
                for(const uint16_t src[] = { 15894U, 1448U, 17301U, 26841U, 62260U, 30716U, 43021U, 3360U, 56644U, 21114U, 50075U, 41691U, 28175U, 47295U, 10191U, 58412U, 15880U, 9076U, 46742U, 38465U, 9078U, 52229U, 27895U, 61214U, 25661U, 27441U, 38334U, 18228U, 61423U, 46818U, 57508U, 33758U, 32811U, 27487U, 60756U, 256U, 41212U, 37033U, 23443U, 20773U, 19816U, 45077U, 30332U, 58687U, 19910U, 747U, 6381U, 52025U, 63441U, 33272U, 344U, 5795U, 45888U, 36760U, 27766U, 40013U, 58839U, 685U, 31873U, 7060U, 15331U, 3313U, 62670U, 25237U, 44257U, 38845U, 37351U, 60509U, 15828U, 56175U, 41533U, 15264U, 45825U, 17009U, 22054U, 33530U, 61905U, 24960U, 29258U, 41677U, 35407U, 65486U, 32643U, 24143U, 63775U, 44864U, 42499U, 62843U, 37872U, 62010U, 39415U, 43950U, 52895U, 20568U, 45836U, 34370U, 24321U, 60519U, 37569U, 65321U, 25031U, 26664U, 10074U, 21924U, 48159U, 50638U, 27570U, 47692U, 64370U, 27856U, 55563U, 16167U, 29353U, 25404U, 64801U, 57135U, 56525U, 44817U, 3974U, 2849U, 22116U, 47135U, 6398U, 16501U, 16091U, 38102U, 28456U, 56301U, 42248U, 7657U, 27809U, 39673U, 25784U, 20704U, 48674U, 65509U, 51227U, 175U, 45054U, 8577U, 57322U, 40060U, 52900U, 20889U, 21806U, 20599U, 65152U, 34546U, 46501U, 32305U, 9916U, 58624U, 13397U, 53641U, 7981U, 16002U, 55941U, 50921U, 15934U, 31458U, 39348U, 65015U, 17394U, 26104U, 62407U, 22840U, 14257U, 63064U, 24843U, 46701U, 53845U, 56582U, 10300U, 63427U, 40774U, 47830U, 27474U, 21304U, 43477U, 21968U, 65365U, 25911U, 55489U, 35045U, 54954U, 3403U, 21880U, 62458U, 50724U, 43728U, 21736U, 51104U, 14736U, 13767U, 53857U, 8247U, 3952U, 33935U, 1876U, 25308U, 36250U, 841U, 53762U, 55997U, 44452U, 14061U, 64539U, 56213U, 48034U, 33279U, 48001U, 57657U, 15597U, 20900U, 42211U, 18984U, 57777U, 1610U, 20281U, 24174U, 7236U, 14034U, 53239U, 42216U, 49599U, 58972U, 22261U, 50174U, 31343U, 16896U, 44763U, 30283U, 19957U, 49438U, 11488U, 12100U, 18239U, 55743U, 52007U, 41676U, 25587U, 21098U, 45568U, 12582U, 10394U, 781U, 21353U, 65284U, 57078U, 18752U, 35653U, 41629U, 15034U, 64357U, 24320U, 36745U, 2846U, 63429U, 53116U, 56510U, 22313U, 61558U, 56831U, 15445U, 11827U, 57645U, 52002U, 44033U, 22344U, 52649U, 11432U, 43231U, 25856U, 2761U, 8966U, 5204U, 23142U, 19846U, 5061U, 62469U, 40119U, 277U, 42863U, 4747U, 5041U, 26437U, 11311U, 44925U, 128U, 28561U, 26726U, 49056U, 36485U, 63691U, 40585U, 60942U, 61979U, 65280U, 743U, 6045U, 31756U, 54061U, 16986U, 62213U, 5288U, 1449U, 64261U, 14043U, 49350U, 60095U, 7140U, 55631U, 33462U, 32296U, 36595U, 56999U, 10991U, 7541U, 32126U, 32039U, 41800U, 49833U, 10277U, 45455U, 41063U, 24863U, 18498U, 42020U, 22390U, 46062U, 36259U, 11453U, 64327U, 62669U, 50899U, 11779U, 36322U, 35267U, 9957U, 47236U, 45189U, 31962U, 59149U, 33199U, 15991U, 9744U, 1647U, 30931U, 23298U, 63348U, 13606U, 29531U, 7034U, 47581U, 35313U, 11500U, 3418U, 46991U, 13714U, 61690U, 61428U, 50959U, 38091U, 50679U, 38987U, 43694U, 38230U, 39973U, 60006U, 37496U, 19043U, 46041U, 62501U, 31879U, 29312U, 21420U, 58147U, 2143U, 63107U, 9227U, 38434U, 56542U, 19011U, 41821U, 65054U, 25458U, 27439U, 15677U, 37119U, 11766U, 1679U, 61801U, 43712U, 37552U, 8509U, 44472U, 15636U, 12904U, 57723U, 24288U, 54516U, 54924U, 56971U, 52101U, 5194U, 59849U, 29835U, 3568U, 1453U, 49465U, 32802U, 55882U, 28445U, 10969U, 14970U, 22869U, 35040U, 57307U, 50055U, 11332U, 33631U, 24357U, 61435U, 2060U, 63279U, 57803U, 39902U, 11153U, 12876U, 33761U, 50175U, 20881U, 12914U, 19705U, 34726U, 26422U, 54456U, 19356U, 46237U, 5924U, 29276U, 50377U, 61227U, 12962U, 54773U, 61048U, 25895U, 25829U, 60389U, 4796U, 47077U, 24334U, 28616U, 33681U, 45692U, 1795U, 31209U, 53494U, 36028U, 64042U, 53564U, 55552U, 33234U, 55195U, 37195U, 36704U, 57504U, 11538U, 55669U, 56201U, 33473U, 43041U, 10095U, 58041U, 7373U, 64402U, 31639U, 11414U, 53680U, 48189U, 35907U, 62877U, 46924U, 27650U, 4348U, 16917U, 51524U, 62876U, 1799U, 53915U, 45890U, 8709U, 63495U, 6544U, 7480U, 22382U, 64195U, 52521U, 40260U, 17182U, 56832U, 11632U, 50830U, 27892U, 48556U, 48251U, 51574U, 1297U, 27539U, 5723U, 31206U, 4285U, 9887U, 2109U, 61827U, 21438U, 18207U, 42914U, 37577U, 39U, 57277U, 31019U, 42653U, 58029U, 7791U, 46379U, 13326U, 24120U, 1791U, 36090U, 60533U, 17685U, 62992U, 64878U, 55279U, 53873U, 16083U, 45705U, 47692U, 32269U, 63558U, 24032U, 65099U, 54247U, 62051U, 59964U, 50107U, 62736U, 37674U, 33725U, 18444U, 1182U, 40795U, 391U, 54723U, 33770U, 37287U, 4513U, 50420U, 21081U, 8722U, 17299U, 63313U, 28853U, 39596U, 32761U, 7432U, 785U, 37935U, 18261U, 61754U, 35602U, 20587U, 65331U, 39062U, 5472U, 30206U, 46838U, 38179U, 59490U, 20138U, 42506U, 5261U, 20993U, 56347U, 43461U, 40943U, 33880U, 17293U, 55615U, 41677U, 54048U, 21178U, 8131U, 26017U, 53974U, 20084U, 32923U, 62473U, 39237U, 10628U, 13276U, 34915U, 25281U, 37419U, 63634U, 23626U, 55116U, 4488U, 9742U, 52163U, 57547U, 33647U, 58771U, 2481U, 60230U, 40242U, 44299U, 15441U, 36810U, 61524U, 39333U, 46277U, 12661U, 13037U, 30250U, 29291U, 7470U, 50268U, 30770U, 54426U, 30146U, 2476U, 57885U, 61205U, 2762U, 64065U, 2665U, 48903U, 40158U, 16137U, 17029U, 17107U, 31265U, 36224U, 65076U, 51521U, 48740U, 74U, 7283U, 63229U, 9094U, 19533U, 42386U, 39102U, 11274U, 46240U, 55161U, 37217U, 39501U, 8286U, 53964U, 53501U, 5462U, 30831U, 45830U, 59136U, 10360U, 28199U, 19235U, 31221U, 1524U, 32134U, 43702U, 54111U, 40298U, 30685U, 38437U, 3678U, 27059U, 9678U, 21131U, 26891U, 50909U, 32983U, 17526U, 43542U, 7727U, 43017U, 62414U, 9835U, 56806U, 48857U, 18944U, 5845U, 18771U, 3628U, 25818U, 40425U, 17710U, 11560U, 6357U, 18716U, 53121U, 53240U, 14610U, 30936U, 62715U, 64451U, 44186U, 8840U, 33360U, 46850U, 49814U, 19207U, 3978U, 23049U, 52069U, 10821U, 36433U, 58946U, 36024U, 7731U, 1724U, 55785U, 43816U, 64383U, 46377U, 42435U, 64496U, 43042U, 35114U, 114U, 35791U, 65222U, 42698U, 1776U, 1969U, 11003U, 10567U, 49130U, 12112U, 46836U, 4324U, 45321U, 50149U, 2747U, 22030U, 37360U, 494U, 16395U, 58104U, 58030U, 36244U, 2383U, 33625U, 15725U, 25080U, 40310U, 64892U, 7415U, 20511U, 25022U, 1591U, 45018U, 50891U, 9518U, 47214U, 33630U, 33141U, 45885U, 21896U, 21024U, 33702U, 57892U, 49510U, 3598U, 59137U, 60223U, 12014U, 14160U, 19300U, 572U, 46089U, 42525U, 12627U, 19675U, 13000U, 49962U, 28230U, 40094U, 43733U, 25398U, 4156U, 64065U, 51955U, 6072U, 39970U, 41355U, 17055U, 58273U, 7440U, 56356U, 5105U, 32806U, 12497U, 29740U, 965U, 16424U, 59926U, 7520U, 38612U, 16982U, 21719U, 49826U, 10005U, 43494U, 40827U, 451U, 64751U, 54718U, 3292U, 10786U, 40206U, 3410U, 65187U, 54014U, 22386U, 37022U, 44170U, 22158U, 37291U, 28229U, 64784U, 1380U, 38124U, 3057U, 11669U, 27266U, 40356U, 54295U, 30997U, 57376U, 13843U, 40152U, 15156U, 63500U, 60241U, 60729U, 21843U, 57331U, 58167U, 3808U, 49166U, 8208U, 50976U, 54941U, 14999U, 11353U, 37524U, 19605U, 50976U, 26654U, 52638U, 57938U, 27434U, 7515U, 11633U, 35153U, 2651U, 20364U, 6495U, 46205U, 64970U, 51947U, 33035U, 43520U, 12449U, 20651U, 13802U, 52984U, 57604U, 64332U, 9853U, 55404U, 7096U, 52627U, 42445U, 39215U, 46494U, 38123U, 33731U, 61849U, 10730U, 15350U, 50400U, 48214U, 29501U, 27270U, 26656U, 63188U, 30455U, 2899U, 22201U, 1771U, 40668U, 58385U, 39542U, 36385U, 42421U, 15512U, 7140U, 25032U, 65470U, 45244U, 2702U, 53737U, 31397U, 29688U, 3156U, 1430U, 39560U, 12061U, 18695U, 1030U, 1681U, 6164U, 60414U, 16153U, 55398U, 3156U, 15885U, 26198U, 56445U, 49785U, 1385U, 8693U, 39307U, 19105U, 49439U, 34689U, 13020U, 41291U, 33637U, 59753U, 42463U, 45218U, 37291U, 32633U, 19080U, 38593U, 32452U, 53748U, 2095U, 1813U, 13708U, 48325U, 12507U, 43337U, 61111U, 64446U, 42514U, 26995U, 10020U, 19478U, 24914U, 37730U, 61690U, 10928U, 50440U, 41551U, 32619U, 21657U, 24659U, 50388U, 49024U, 45606U, 2218U, 11847U, 35441U, 55310U, 44752U, 1748U, 20407U, 28785U, 11632U, 38283U, 3465U, 16552U, 12029U, 64466U, 44696U, 37881U, 61065U, 30910U, 23345U, 26943U, 26272U, 23985U, 10653U } ; p9.values(src), false;);
            }

            void on_SensorsData(const org::company::SensorsData & p9)
            {
                {
                    const uint16_t exemplary[] = { 15894U, 1448U, 17301U, 26841U, 62260U, 30716U, 43021U, 3360U, 56644U, 21114U, 50075U, 41691U, 28175U, 47295U, 10191U, 58412U, 15880U, 9076U, 46742U, 38465U, 9078U, 52229U, 27895U, 61214U, 25661U, 27441U, 38334U, 18228U, 61423U, 46818U, 57508U, 33758U, 32811U, 27487U, 60756U, 256U, 41212U, 37033U, 23443U, 20773U, 19816U, 45077U, 30332U, 58687U, 19910U, 747U, 6381U, 52025U, 63441U, 33272U, 344U, 5795U, 45888U, 36760U, 27766U, 40013U, 58839U, 685U, 31873U, 7060U, 15331U, 3313U, 62670U, 25237U, 44257U, 38845U, 37351U, 60509U, 15828U, 56175U, 41533U, 15264U, 45825U, 17009U, 22054U, 33530U, 61905U, 24960U, 29258U, 41677U, 35407U, 65486U, 32643U, 24143U, 63775U, 44864U, 42499U, 62843U, 37872U, 62010U, 39415U, 43950U, 52895U, 20568U, 45836U, 34370U, 24321U, 60519U, 37569U, 65321U, 25031U, 26664U, 10074U, 21924U, 48159U, 50638U, 27570U, 47692U, 64370U, 27856U, 55563U, 16167U, 29353U, 25404U, 64801U, 57135U, 56525U, 44817U, 3974U, 2849U, 22116U, 47135U, 6398U, 16501U, 16091U, 38102U, 28456U, 56301U, 42248U, 7657U, 27809U, 39673U, 25784U, 20704U, 48674U, 65509U, 51227U, 175U, 45054U, 8577U, 57322U, 40060U, 52900U, 20889U, 21806U, 20599U, 65152U, 34546U, 46501U, 32305U, 9916U, 58624U, 13397U, 53641U, 7981U, 16002U, 55941U, 50921U, 15934U, 31458U, 39348U, 65015U, 17394U, 26104U, 62407U, 22840U, 14257U, 63064U, 24843U, 46701U, 53845U, 56582U, 10300U, 63427U, 40774U, 47830U, 27474U, 21304U, 43477U, 21968U, 65365U, 25911U, 55489U, 35045U, 54954U, 3403U, 21880U, 62458U, 50724U, 43728U, 21736U, 51104U, 14736U, 13767U, 53857U, 8247U, 3952U, 33935U, 1876U, 25308U, 36250U, 841U, 53762U, 55997U, 44452U, 14061U, 64539U, 56213U, 48034U, 33279U, 48001U, 57657U, 15597U, 20900U, 42211U, 18984U, 57777U, 1610U, 20281U, 24174U, 7236U, 14034U, 53239U, 42216U, 49599U, 58972U, 22261U, 50174U, 31343U, 16896U, 44763U, 30283U, 19957U, 49438U, 11488U, 12100U, 18239U, 55743U, 52007U, 41676U, 25587U, 21098U, 45568U, 12582U, 10394U, 781U, 21353U, 65284U, 57078U, 18752U, 35653U, 41629U, 15034U, 64357U, 24320U, 36745U, 2846U, 63429U, 53116U, 56510U, 22313U, 61558U, 56831U, 15445U, 11827U, 57645U, 52002U, 44033U, 22344U, 52649U, 11432U, 43231U, 25856U, 2761U, 8966U, 5204U, 23142U, 19846U, 5061U, 62469U, 40119U, 277U, 42863U, 4747U, 5041U, 26437U, 11311U, 44925U, 128U, 28561U, 26726U, 49056U, 36485U, 63691U, 40585U, 60942U, 61979U, 65280U, 743U, 6045U, 31756U, 54061U, 16986U, 62213U, 5288U, 1449U, 64261U, 14043U, 49350U, 60095U, 7140U, 55631U, 33462U, 32296U, 36595U, 56999U, 10991U, 7541U, 32126U, 32039U, 41800U, 49833U, 10277U, 45455U, 41063U, 24863U, 18498U, 42020U, 22390U, 46062U, 36259U, 11453U, 64327U, 62669U, 50899U, 11779U, 36322U, 35267U, 9957U, 47236U, 45189U, 31962U, 59149U, 33199U, 15991U, 9744U, 1647U, 30931U, 23298U, 63348U, 13606U, 29531U, 7034U, 47581U, 35313U, 11500U, 3418U, 46991U, 13714U, 61690U, 61428U, 50959U, 38091U, 50679U, 38987U, 43694U, 38230U, 39973U, 60006U, 37496U, 19043U, 46041U, 62501U, 31879U, 29312U, 21420U, 58147U, 2143U, 63107U, 9227U, 38434U, 56542U, 19011U, 41821U, 65054U, 25458U, 27439U, 15677U, 37119U, 11766U, 1679U, 61801U, 43712U, 37552U, 8509U, 44472U, 15636U, 12904U, 57723U, 24288U, 54516U, 54924U, 56971U, 52101U, 5194U, 59849U, 29835U, 3568U, 1453U, 49465U, 32802U, 55882U, 28445U, 10969U, 14970U, 22869U, 35040U, 57307U, 50055U, 11332U, 33631U, 24357U, 61435U, 2060U, 63279U, 57803U, 39902U, 11153U, 12876U, 33761U, 50175U, 20881U, 12914U, 19705U, 34726U, 26422U, 54456U, 19356U, 46237U, 5924U, 29276U, 50377U, 61227U, 12962U, 54773U, 61048U, 25895U, 25829U, 60389U, 4796U, 47077U, 24334U, 28616U, 33681U, 45692U, 1795U, 31209U, 53494U, 36028U, 64042U, 53564U, 55552U, 33234U, 55195U, 37195U, 36704U, 57504U, 11538U, 55669U, 56201U, 33473U, 43041U, 10095U, 58041U, 7373U, 64402U, 31639U, 11414U, 53680U, 48189U, 35907U, 62877U, 46924U, 27650U, 4348U, 16917U, 51524U, 62876U, 1799U, 53915U, 45890U, 8709U, 63495U, 6544U, 7480U, 22382U, 64195U, 52521U, 40260U, 17182U, 56832U, 11632U, 50830U, 27892U, 48556U, 48251U, 51574U, 1297U, 27539U, 5723U, 31206U, 4285U, 9887U, 2109U, 61827U, 21438U, 18207U, 42914U, 37577U, 39U, 57277U, 31019U, 42653U, 58029U, 7791U, 46379U, 13326U, 24120U, 1791U, 36090U, 60533U, 17685U, 62992U, 64878U, 55279U, 53873U, 16083U, 45705U, 47692U, 32269U, 63558U, 24032U, 65099U, 54247U, 62051U, 59964U, 50107U, 62736U, 37674U, 33725U, 18444U, 1182U, 40795U, 391U, 54723U, 33770U, 37287U, 4513U, 50420U, 21081U, 8722U, 17299U, 63313U, 28853U, 39596U, 32761U, 7432U, 785U, 37935U, 18261U, 61754U, 35602U, 20587U, 65331U, 39062U, 5472U, 30206U, 46838U, 38179U, 59490U, 20138U, 42506U, 5261U, 20993U, 56347U, 43461U, 40943U, 33880U, 17293U, 55615U, 41677U, 54048U, 21178U, 8131U, 26017U, 53974U, 20084U, 32923U, 62473U, 39237U, 10628U, 13276U, 34915U, 25281U, 37419U, 63634U, 23626U, 55116U, 4488U, 9742U, 52163U, 57547U, 33647U, 58771U, 2481U, 60230U, 40242U, 44299U, 15441U, 36810U, 61524U, 39333U, 46277U, 12661U, 13037U, 30250U, 29291U, 7470U, 50268U, 30770U, 54426U, 30146U, 2476U, 57885U, 61205U, 2762U, 64065U, 2665U, 48903U, 40158U, 16137U, 17029U, 17107U, 31265U, 36224U, 65076U, 51521U, 48740U, 74U, 7283U, 63229U, 9094U, 19533U, 42386U, 39102U, 11274U, 46240U, 55161U, 37217U, 39501U, 8286U, 53964U, 53501U, 5462U, 30831U, 45830U, 59136U, 10360U, 28199U, 19235U, 31221U, 1524U, 32134U, 43702U, 54111U, 40298U, 30685U, 38437U, 3678U, 27059U, 9678U, 21131U, 26891U, 50909U, 32983U, 17526U, 43542U, 7727U, 43017U, 62414U, 9835U, 56806U, 48857U, 18944U, 5845U, 18771U, 3628U, 25818U, 40425U, 17710U, 11560U, 6357U, 18716U, 53121U, 53240U, 14610U, 30936U, 62715U, 64451U, 44186U, 8840U, 33360U, 46850U, 49814U, 19207U, 3978U, 23049U, 52069U, 10821U, 36433U, 58946U, 36024U, 7731U, 1724U, 55785U, 43816U, 64383U, 46377U, 42435U, 64496U, 43042U, 35114U, 114U, 35791U, 65222U, 42698U, 1776U, 1969U, 11003U, 10567U, 49130U, 12112U, 46836U, 4324U, 45321U, 50149U, 2747U, 22030U, 37360U, 494U, 16395U, 58104U, 58030U, 36244U, 2383U, 33625U, 15725U, 25080U, 40310U, 64892U, 7415U, 20511U, 25022U, 1591U, 45018U, 50891U, 9518U, 47214U, 33630U, 33141U, 45885U, 21896U, 21024U, 33702U, 57892U, 49510U, 3598U, 59137U, 60223U, 12014U, 14160U, 19300U, 572U, 46089U, 42525U, 12627U, 19675U, 13000U, 49962U, 28230U, 40094U, 43733U, 25398U, 4156U, 64065U, 51955U, 6072U, 39970U, 41355U, 17055U, 58273U, 7440U, 56356U, 5105U, 32806U, 12497U, 29740U, 965U, 16424U, 59926U, 7520U, 38612U, 16982U, 21719U, 49826U, 10005U, 43494U, 40827U, 451U, 64751U, 54718U, 3292U, 10786U, 40206U, 3410U, 65187U, 54014U, 22386U, 37022U, 44170U, 22158U, 37291U, 28229U, 64784U, 1380U, 38124U, 3057U, 11669U, 27266U, 40356U, 54295U, 30997U, 57376U, 13843U, 40152U, 15156U, 63500U, 60241U, 60729U, 21843U, 57331U, 58167U, 3808U, 49166U, 8208U, 50976U, 54941U, 14999U, 11353U, 37524U, 19605U, 50976U, 26654U, 52638U, 57938U, 27434U, 7515U, 11633U, 35153U, 2651U, 20364U, 6495U, 46205U, 64970U, 51947U, 33035U, 43520U, 12449U, 20651U, 13802U, 52984U, 57604U, 64332U, 9853U, 55404U, 7096U, 52627U, 42445U, 39215U, 46494U, 38123U, 33731U, 61849U, 10730U, 15350U, 50400U, 48214U, 29501U, 27270U, 26656U, 63188U, 30455U, 2899U, 22201U, 1771U, 40668U, 58385U, 39542U, 36385U, 42421U, 15512U, 7140U, 25032U, 65470U, 45244U, 2702U, 53737U, 31397U, 29688U, 3156U, 1430U, 39560U, 12061U, 18695U, 1030U, 1681U, 6164U, 60414U, 16153U, 55398U, 3156U, 15885U, 26198U, 56445U, 49785U, 1385U, 8693U, 39307U, 19105U, 49439U, 34689U, 13020U, 41291U, 33637U, 59753U, 42463U, 45218U, 37291U, 32633U, 19080U, 38593U, 32452U, 53748U, 2095U, 1813U, 13708U, 48325U, 12507U, 43337U, 61111U, 64446U, 42514U, 26995U, 10020U, 19478U, 24914U, 37730U, 61690U, 10928U, 50440U, 41551U, 32619U, 21657U, 24659U, 50388U, 49024U, 45606U, 2218U, 11847U, 35441U, 55310U, 44752U, 1748U, 20407U, 28785U, 11632U, 38283U, 3465U, 16552U, 12029U, 64466U, 44696U, 37881U, 61065U, 30910U, 23345U, 26943U, 26272U, 23985U, 10653U }  ;
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

