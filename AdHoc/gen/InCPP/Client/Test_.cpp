
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
                p0.DevID(42210U) ;
                p0.Ver(10480U) ;
            }

            void on_DeviceVersion(const org::company::DeviceVersion & p0)
            {
                assert(p0.DevID() == 42210U);
                assert(p0.Ver() == 10480U);
                printf("DeviceVersion \n");
            }

            void fill(const org::company::BusConfiguration & p6)
            {
                p6.multiplier(164U) ;
                p6.time(47380U) ;
                p6.clk_khz(40043U) ;
            }

            void on_BusConfiguration(const org::company::BusConfiguration & p6)
            {
                assert(p6.multiplier() == 164U);
                assert(p6.time() == 47380U);
                assert(p6.clk_khz() == 40043U);
                printf("BusConfiguration \n");
            }

            void fill(const org::company::InstructionsPack & p7)
            {
                p7.Length(202U) ;
                for(const uint8_t src[] = { 53U, 82U, 120U, 63U, 187U, 235U, 224U, 245U, 234U, 132U, 201U, 56U, 22U, 13U, 206U, 11U, 132U, 197U, 61U, 200U, 106U, 74U, 218U, 103U, 172U, 62U, 154U, 176U, 216U, 166U, 201U, 218U, 102U, 66U, 184U, 179U, 223U, 169U, 108U, 58U, 125U, 66U, 202U, 56U, 16U, 4U, 122U, 139U, 0U, 229U, 195U, 22U, 89U, 57U, 158U, 14U, 223U, 32U, 189U, 203U, 163U, 30U, 106U, 52U, 79U, 168U, 174U, 41U, 237U, 59U, 188U, 126U, 61U, 175U, 94U, 128U, 121U, 115U, 91U, 14U, 121U, 249U, 116U, 23U, 137U, 251U, 1U, 48U, 211U, 92U, 24U, 187U, 229U, 76U, 202U, 46U, 33U, 80U, 3U, 152U, 242U, 37U, 45U, 31U, 42U, 93U, 7U, 217U, 77U, 153U, 65U, 57U, 87U, 176U, 144U, 123U, 154U, 111U, 50U, 187U, 17U, 245U, 183U, 193U, 188U, 141U, 70U, 57U, 220U, 142U, 163U, 189U, 226U, 220U, 58U, 125U, 79U, 68U, 168U, 4U, 214U, 68U, 165U, 135U, 148U, 49U, 78U, 104U, 58U, 63U, 154U, 110U, 219U, 46U, 81U, 190U, 184U, 144U, 98U, 178U, 47U, 230U, 151U, 130U, 233U, 52U, 183U, 181U, 155U, 171U, 56U, 39U, 112U, 213U, 24U, 166U, 151U, 164U, 106U, 70U, 171U, 118U, 205U, 203U, 72U, 156U, 100U, 26U, 29U, 30U, 246U, 31U, 206U, 239U, 197U, 154U, 234U, 133U, 4U, 83U, 237U, 183U, 247U, 137U, 238U, 87U, 255U, 29U, 138U, 15U, 28U, 5U, 10U, 110U, 249U, 81U, 125U, 142U, 18U, 57U, 203U, 240U, 243U, 227U, 229U, 90U, 203U, 83U, 251U, 71U, 73U, 104U, 76U, 87U, 198U, 156U, 162U, 194U, 226U, 95U, 249U, 81U, 184U, 0U, 254U, 95U, 163U, 210U, 203U, 132U, 189U, 64U, 130U, 226U, 52U, 137U } ; p7.Instructions(src), false;);
            }

            void on_InstructionsPack(const org::company::InstructionsPack & p7)
            {
                assert(p7.Length() == 202U);
                {
                    const uint8_t exemplary[] = { 53U, 82U, 120U, 63U, 187U, 235U, 224U, 245U, 234U, 132U, 201U, 56U, 22U, 13U, 206U, 11U, 132U, 197U, 61U, 200U, 106U, 74U, 218U, 103U, 172U, 62U, 154U, 176U, 216U, 166U, 201U, 218U, 102U, 66U, 184U, 179U, 223U, 169U, 108U, 58U, 125U, 66U, 202U, 56U, 16U, 4U, 122U, 139U, 0U, 229U, 195U, 22U, 89U, 57U, 158U, 14U, 223U, 32U, 189U, 203U, 163U, 30U, 106U, 52U, 79U, 168U, 174U, 41U, 237U, 59U, 188U, 126U, 61U, 175U, 94U, 128U, 121U, 115U, 91U, 14U, 121U, 249U, 116U, 23U, 137U, 251U, 1U, 48U, 211U, 92U, 24U, 187U, 229U, 76U, 202U, 46U, 33U, 80U, 3U, 152U, 242U, 37U, 45U, 31U, 42U, 93U, 7U, 217U, 77U, 153U, 65U, 57U, 87U, 176U, 144U, 123U, 154U, 111U, 50U, 187U, 17U, 245U, 183U, 193U, 188U, 141U, 70U, 57U, 220U, 142U, 163U, 189U, 226U, 220U, 58U, 125U, 79U, 68U, 168U, 4U, 214U, 68U, 165U, 135U, 148U, 49U, 78U, 104U, 58U, 63U, 154U, 110U, 219U, 46U, 81U, 190U, 184U, 144U, 98U, 178U, 47U, 230U, 151U, 130U, 233U, 52U, 183U, 181U, 155U, 171U, 56U, 39U, 112U, 213U, 24U, 166U, 151U, 164U, 106U, 70U, 171U, 118U, 205U, 203U, 72U, 156U, 100U, 26U, 29U, 30U, 246U, 31U, 206U, 239U, 197U, 154U, 234U, 133U, 4U, 83U, 237U, 183U, 247U, 137U, 238U, 87U, 255U, 29U, 138U, 15U, 28U, 5U, 10U, 110U, 249U, 81U, 125U, 142U, 18U, 57U, 203U, 240U, 243U, 227U, 229U, 90U, 203U, 83U, 251U, 71U, 73U, 104U, 76U, 87U, 198U, 156U, 162U, 194U, 226U, 95U, 249U, 81U, 184U, 0U, 254U, 95U, 163U, 210U, 203U, 132U, 189U, 64U, 130U, 226U, 52U, 137U }  ;
                    const auto sample = p7.Instructions();
                    for(size_t index = 0; index < org::company::InstructionsPack::Instructions_::len; index++)
                        assert(sample.get(index) == exemplary[index]);
                }
                printf("InstructionsPack \n");
            }

            void fill(const org::unirail::test::DeviceError & p8)
            {
                p8.param1(13770U) ;
                p8.error_id(Errors::CannotReadSensor) ;
            }

            void on_DeviceError(const org::company::DeviceError & p8)
            {
                assert(p8.param1() == 13770U);
                assert(p8.error_id() == Errors::CannotReadSensor);
                printf("DeviceError \n");
            }

            void fill(const org::unirail::test::SensorsData & p9)
            {
                for(const uint16_t src[] = { 49749U, 54692U, 29585U, 28587U, 15835U, 58740U, 57691U, 2241U, 30225U, 16927U, 12242U, 53252U, 41894U, 30443U, 34413U, 29212U, 39405U, 5933U, 55649U, 58017U, 60590U, 639U, 43464U, 29117U, 50459U, 65114U, 45966U, 5734U, 34286U, 28237U, 26617U, 28620U, 45168U, 50310U, 18712U, 56670U, 2298U, 45406U, 50552U, 20593U, 50731U, 46201U, 58508U, 43359U, 43673U, 20096U, 27208U, 64834U, 50579U, 28603U, 39039U, 32144U, 28011U, 21489U, 22749U, 27790U, 2999U, 60218U, 17866U, 15748U, 35435U, 50686U, 11653U, 39364U, 28902U, 63068U, 13312U, 27457U, 1199U, 10482U, 47669U, 50919U, 65002U, 54964U, 34266U, 7103U, 43162U, 57183U, 25906U, 14079U, 5368U, 39665U, 39190U, 16453U, 20701U, 60199U, 5608U, 25413U, 16111U, 41764U, 168U, 4323U, 19567U, 41007U, 52249U, 33270U, 20694U, 3547U, 28172U, 29763U, 36923U, 37899U, 43454U, 23080U, 41584U, 59264U, 26898U, 55359U, 50168U, 36554U, 64357U, 18335U, 60729U, 18408U, 63350U, 47548U, 60130U, 20544U, 34136U, 8170U, 16329U, 20157U, 44787U, 59428U, 48076U, 40820U, 12520U, 32479U, 47106U, 34416U, 61677U, 21863U, 8278U, 13350U, 25551U, 19639U, 8583U, 11731U, 40589U, 59127U, 43775U, 12379U, 34215U, 54712U, 59535U, 45158U, 40815U, 3050U, 1575U, 8276U, 40049U, 38249U, 40146U, 6930U, 43213U, 10904U, 10353U, 44685U, 43615U, 64685U, 55666U, 52318U, 60108U, 47017U, 34720U, 14979U, 58136U, 30015U, 4210U, 5640U, 43420U, 59400U, 59516U, 6994U, 63895U, 28389U, 34489U, 55653U, 25907U, 54795U, 9602U, 25917U, 29968U, 27180U, 33849U, 42684U, 65415U, 10706U, 24565U, 44350U, 10765U, 58425U, 28728U, 57523U, 41324U, 63370U, 52705U, 1832U, 21000U, 61855U, 61382U, 60469U, 42330U, 60823U, 20364U, 62561U, 53086U, 40975U, 598U, 13422U, 26237U, 59874U, 28183U, 21619U, 10923U, 36984U, 15630U, 34637U, 37340U, 55974U, 1128U, 32949U, 16771U, 7209U, 61152U, 19287U, 250U, 32680U, 28390U, 29132U, 63920U, 24062U, 34912U, 9241U, 31973U, 27310U, 46319U, 25735U, 40069U, 60051U, 40707U, 47248U, 1877U, 24056U, 36984U, 424U, 6839U, 20497U, 5774U, 59345U, 27324U, 53667U, 53253U, 58747U, 47076U, 1661U, 3473U, 27747U, 102U, 62616U, 53594U, 27501U, 65128U, 11431U, 44871U, 48452U, 60095U, 63592U, 57954U, 64950U, 34620U, 27582U, 23167U, 14787U, 62691U, 12232U, 46675U, 60892U, 19187U, 5990U, 65253U, 50524U, 56567U, 53095U, 3650U, 46828U, 40667U, 38861U, 3070U, 51049U, 49820U, 6452U, 27321U, 46711U, 52655U, 19652U, 39197U, 18675U, 53917U, 50630U, 55203U, 40891U, 20205U, 29247U, 2599U, 16148U, 38680U, 10802U, 1689U, 63802U, 18206U, 20588U, 15094U, 28982U, 16686U, 5099U, 63414U, 50153U, 25005U, 27963U, 58447U, 16518U, 18517U, 28764U, 31527U, 34326U, 39060U, 30538U, 49812U, 5598U, 30863U, 52931U, 49338U, 56443U, 25018U, 26311U, 55351U, 29583U, 31432U, 60757U, 50645U, 29096U, 28942U, 44464U, 37819U, 17222U, 45576U, 34219U, 27424U, 37990U, 33857U, 32287U, 25684U, 54719U, 47855U, 58233U, 12626U, 31508U, 54477U, 52954U, 64788U, 24433U, 19017U, 19475U, 15542U, 30354U, 55963U, 64111U, 60336U, 10938U, 26622U, 31457U, 2506U, 53814U, 52775U, 35007U, 14556U, 10372U, 17418U, 33952U, 16249U, 51928U, 48936U, 32366U, 24586U, 22778U, 11845U, 31729U, 6726U, 15214U, 18523U, 23814U, 59583U, 57650U, 38840U, 24291U, 22620U, 63968U, 56244U, 50467U, 52964U, 15156U, 42020U, 61651U, 13714U, 60572U, 61676U, 65099U, 58037U, 44977U, 9676U, 41990U, 17905U, 17934U, 28900U, 513U, 51766U, 59170U, 12116U, 17948U, 53257U, 18417U, 56390U, 35904U, 1753U, 56U, 2518U, 7965U, 57424U, 31948U, 7787U, 8934U, 18544U, 56625U, 63695U, 18627U, 19663U, 1913U, 24742U, 55024U, 44032U, 36379U, 4226U, 1618U, 58772U, 1799U, 30345U, 54409U, 25065U, 22820U, 29795U, 55549U, 31188U, 55644U, 30333U, 56110U, 30737U, 33518U, 62087U, 49811U, 46882U, 16865U, 32191U, 63225U, 2354U, 22513U, 41949U, 29563U, 48768U, 38358U, 32211U, 65300U, 5516U, 2802U, 22026U, 44868U, 25641U, 19065U, 12438U, 55854U, 17229U, 43975U, 44354U, 43422U, 48665U, 43864U, 5917U, 42999U, 48022U, 26057U, 35827U, 40401U, 39869U, 16613U, 60197U, 38812U, 6388U, 38650U, 59017U, 49522U, 11468U, 57387U, 3951U, 4716U, 55401U, 41317U, 48111U, 23591U, 26189U, 33004U, 10763U, 3936U, 65463U, 37565U, 46252U, 48384U, 23713U, 58095U, 16439U, 7225U, 47584U, 9393U, 49847U, 12845U, 12926U, 52415U, 41945U, 12842U, 36287U, 16846U, 14984U, 4368U, 9330U, 11130U, 23485U, 52289U, 40482U, 55518U, 34858U, 5511U, 45650U, 21259U, 41094U, 58277U, 33125U, 49642U, 17730U, 11619U, 52071U, 11137U, 16461U, 16318U, 53804U, 62725U, 65010U, 26442U, 41245U, 18996U, 63139U, 60157U, 25638U, 40885U, 1796U, 5987U, 782U, 29410U, 5418U, 57587U, 50219U, 32705U, 49997U, 43373U, 4211U, 13935U, 56700U, 48282U, 3759U, 65398U, 7736U, 32865U, 4528U, 49093U, 20779U, 60340U, 36599U, 13939U, 29553U, 47719U, 54966U, 9491U, 51689U, 53838U, 46209U, 62006U, 24494U, 5221U, 57178U, 40411U, 59993U, 32495U, 59106U, 58606U, 21196U, 36010U, 29111U, 21113U, 65462U, 7746U, 26062U, 22137U, 56832U, 52455U, 5037U, 63286U, 55644U, 52438U, 751U, 60981U, 19552U, 27328U, 21622U, 37974U, 16879U, 47738U, 49004U, 28135U, 52195U, 38035U, 19148U, 12295U, 32484U, 58400U, 54936U, 44838U, 60072U, 11948U, 63392U, 18004U, 54777U, 14022U, 6570U, 4490U, 45810U, 40580U, 21348U, 43396U, 52468U, 55779U, 18392U, 24733U, 65488U, 45167U, 1308U, 31772U, 31852U, 39878U, 9513U, 36424U, 10457U, 18441U, 10174U, 48750U, 7049U, 53911U, 20514U, 14029U, 41567U, 23585U, 60658U, 39905U, 3862U, 62800U, 36147U, 48171U, 6485U, 21638U, 20362U, 12987U, 62962U, 55003U, 57118U, 20803U, 64991U, 27958U, 584U, 44741U, 38985U, 40632U, 2988U, 40618U, 16372U, 20243U, 19069U, 30521U, 40903U, 35762U, 57768U, 13079U, 60216U, 54351U, 29280U, 40210U, 40705U, 61788U, 48006U, 2659U, 4464U, 64938U, 48869U, 270U, 13636U, 4588U, 53735U, 3552U, 32870U, 7618U, 26193U, 27599U, 61223U, 22757U, 30164U, 38075U, 9696U, 53831U, 35553U, 21221U, 40587U, 40228U, 49983U, 56904U, 25484U, 27487U, 39301U, 47518U, 40916U, 3335U, 24621U, 38484U, 29009U, 50689U, 4425U, 19934U, 24906U, 9675U, 8711U, 34201U, 38498U, 19857U, 34087U, 29844U, 58399U, 58769U, 26279U, 53851U, 15459U, 13890U, 13106U, 24843U, 63568U, 52502U, 62943U, 55370U, 14689U, 32278U, 37464U, 40476U, 45427U, 17365U, 15635U, 61192U, 14568U, 23866U, 6886U, 51038U, 30792U, 45152U, 36193U, 29873U, 19638U, 17409U, 4259U, 49503U, 2919U, 46146U, 7695U, 55318U, 55365U, 29609U, 34502U, 12937U, 26852U, 59238U, 20213U, 58368U, 25983U, 53150U, 37279U, 4677U, 54535U, 34722U, 58266U, 42991U, 27087U, 23378U, 152U, 57784U, 21668U, 40710U, 35355U, 56294U, 59036U, 39190U, 3833U, 50167U, 54946U, 11126U, 50807U, 61414U, 33608U, 47726U, 59387U, 62699U, 37608U, 44366U, 63280U, 40747U, 29671U, 7148U, 24467U, 60426U, 18052U, 6900U, 2031U, 35156U, 44503U, 38871U, 16857U, 17885U, 46957U, 18169U, 1258U, 34648U, 61544U, 56843U, 55711U, 43702U, 45680U, 36631U, 32254U, 27653U, 24570U, 16354U, 42141U, 24605U, 30710U, 20629U, 58887U, 53263U, 14201U, 21603U, 38757U, 17375U, 17643U, 58994U, 15369U, 63U, 29545U, 4627U, 59384U, 30371U, 18257U, 33858U, 11556U, 63854U, 8960U, 10255U, 58004U, 52691U, 9787U, 23102U, 16999U, 43202U, 64848U, 58323U, 30861U, 14508U, 52895U, 42509U, 58880U, 45961U, 58489U, 58142U, 54112U, 22993U, 64719U, 22034U, 64015U, 49802U, 58769U, 42715U, 6785U, 50377U, 53427U, 55989U, 48687U, 22017U, 10569U, 379U, 37676U, 10830U, 1246U, 40126U, 48396U, 58962U, 594U, 46486U, 30050U, 18822U, 46192U, 28866U, 30064U, 14341U, 31881U, 20987U, 39253U, 22220U, 3306U, 55845U, 48470U, 13637U, 18273U, 2663U, 61155U, 4612U, 10522U, 40320U, 64518U, 9677U, 30413U, 65279U, 4832U, 44372U, 23311U, 26943U, 5417U, 9336U, 48856U, 7474U, 46142U, 31178U, 21367U, 15119U, 31497U, 21982U, 15643U, 52306U, 2060U, 16231U, 23923U, 42450U, 30997U, 31584U, 58332U, 65287U, 40530U, 30282U, 32112U, 28661U, 49795U, 56680U, 3081U, 56836U, 25446U, 943U, 60129U, 51776U, 21231U, 49976U, 7297U, 26778U, 6073U, 26982U, 52628U, 5411U, 61503U, 60474U, 56321U, 9778U, 23744U, 62321U, 40760U, 38136U, 15155U, 60073U, 34215U, 32142U, 52650U, 52079U, 18510U, 62332U } ; p9.values(src), false;);
            }

            void on_SensorsData(const org::company::SensorsData & p9)
            {
                {
                    const uint16_t exemplary[] = { 49749U, 54692U, 29585U, 28587U, 15835U, 58740U, 57691U, 2241U, 30225U, 16927U, 12242U, 53252U, 41894U, 30443U, 34413U, 29212U, 39405U, 5933U, 55649U, 58017U, 60590U, 639U, 43464U, 29117U, 50459U, 65114U, 45966U, 5734U, 34286U, 28237U, 26617U, 28620U, 45168U, 50310U, 18712U, 56670U, 2298U, 45406U, 50552U, 20593U, 50731U, 46201U, 58508U, 43359U, 43673U, 20096U, 27208U, 64834U, 50579U, 28603U, 39039U, 32144U, 28011U, 21489U, 22749U, 27790U, 2999U, 60218U, 17866U, 15748U, 35435U, 50686U, 11653U, 39364U, 28902U, 63068U, 13312U, 27457U, 1199U, 10482U, 47669U, 50919U, 65002U, 54964U, 34266U, 7103U, 43162U, 57183U, 25906U, 14079U, 5368U, 39665U, 39190U, 16453U, 20701U, 60199U, 5608U, 25413U, 16111U, 41764U, 168U, 4323U, 19567U, 41007U, 52249U, 33270U, 20694U, 3547U, 28172U, 29763U, 36923U, 37899U, 43454U, 23080U, 41584U, 59264U, 26898U, 55359U, 50168U, 36554U, 64357U, 18335U, 60729U, 18408U, 63350U, 47548U, 60130U, 20544U, 34136U, 8170U, 16329U, 20157U, 44787U, 59428U, 48076U, 40820U, 12520U, 32479U, 47106U, 34416U, 61677U, 21863U, 8278U, 13350U, 25551U, 19639U, 8583U, 11731U, 40589U, 59127U, 43775U, 12379U, 34215U, 54712U, 59535U, 45158U, 40815U, 3050U, 1575U, 8276U, 40049U, 38249U, 40146U, 6930U, 43213U, 10904U, 10353U, 44685U, 43615U, 64685U, 55666U, 52318U, 60108U, 47017U, 34720U, 14979U, 58136U, 30015U, 4210U, 5640U, 43420U, 59400U, 59516U, 6994U, 63895U, 28389U, 34489U, 55653U, 25907U, 54795U, 9602U, 25917U, 29968U, 27180U, 33849U, 42684U, 65415U, 10706U, 24565U, 44350U, 10765U, 58425U, 28728U, 57523U, 41324U, 63370U, 52705U, 1832U, 21000U, 61855U, 61382U, 60469U, 42330U, 60823U, 20364U, 62561U, 53086U, 40975U, 598U, 13422U, 26237U, 59874U, 28183U, 21619U, 10923U, 36984U, 15630U, 34637U, 37340U, 55974U, 1128U, 32949U, 16771U, 7209U, 61152U, 19287U, 250U, 32680U, 28390U, 29132U, 63920U, 24062U, 34912U, 9241U, 31973U, 27310U, 46319U, 25735U, 40069U, 60051U, 40707U, 47248U, 1877U, 24056U, 36984U, 424U, 6839U, 20497U, 5774U, 59345U, 27324U, 53667U, 53253U, 58747U, 47076U, 1661U, 3473U, 27747U, 102U, 62616U, 53594U, 27501U, 65128U, 11431U, 44871U, 48452U, 60095U, 63592U, 57954U, 64950U, 34620U, 27582U, 23167U, 14787U, 62691U, 12232U, 46675U, 60892U, 19187U, 5990U, 65253U, 50524U, 56567U, 53095U, 3650U, 46828U, 40667U, 38861U, 3070U, 51049U, 49820U, 6452U, 27321U, 46711U, 52655U, 19652U, 39197U, 18675U, 53917U, 50630U, 55203U, 40891U, 20205U, 29247U, 2599U, 16148U, 38680U, 10802U, 1689U, 63802U, 18206U, 20588U, 15094U, 28982U, 16686U, 5099U, 63414U, 50153U, 25005U, 27963U, 58447U, 16518U, 18517U, 28764U, 31527U, 34326U, 39060U, 30538U, 49812U, 5598U, 30863U, 52931U, 49338U, 56443U, 25018U, 26311U, 55351U, 29583U, 31432U, 60757U, 50645U, 29096U, 28942U, 44464U, 37819U, 17222U, 45576U, 34219U, 27424U, 37990U, 33857U, 32287U, 25684U, 54719U, 47855U, 58233U, 12626U, 31508U, 54477U, 52954U, 64788U, 24433U, 19017U, 19475U, 15542U, 30354U, 55963U, 64111U, 60336U, 10938U, 26622U, 31457U, 2506U, 53814U, 52775U, 35007U, 14556U, 10372U, 17418U, 33952U, 16249U, 51928U, 48936U, 32366U, 24586U, 22778U, 11845U, 31729U, 6726U, 15214U, 18523U, 23814U, 59583U, 57650U, 38840U, 24291U, 22620U, 63968U, 56244U, 50467U, 52964U, 15156U, 42020U, 61651U, 13714U, 60572U, 61676U, 65099U, 58037U, 44977U, 9676U, 41990U, 17905U, 17934U, 28900U, 513U, 51766U, 59170U, 12116U, 17948U, 53257U, 18417U, 56390U, 35904U, 1753U, 56U, 2518U, 7965U, 57424U, 31948U, 7787U, 8934U, 18544U, 56625U, 63695U, 18627U, 19663U, 1913U, 24742U, 55024U, 44032U, 36379U, 4226U, 1618U, 58772U, 1799U, 30345U, 54409U, 25065U, 22820U, 29795U, 55549U, 31188U, 55644U, 30333U, 56110U, 30737U, 33518U, 62087U, 49811U, 46882U, 16865U, 32191U, 63225U, 2354U, 22513U, 41949U, 29563U, 48768U, 38358U, 32211U, 65300U, 5516U, 2802U, 22026U, 44868U, 25641U, 19065U, 12438U, 55854U, 17229U, 43975U, 44354U, 43422U, 48665U, 43864U, 5917U, 42999U, 48022U, 26057U, 35827U, 40401U, 39869U, 16613U, 60197U, 38812U, 6388U, 38650U, 59017U, 49522U, 11468U, 57387U, 3951U, 4716U, 55401U, 41317U, 48111U, 23591U, 26189U, 33004U, 10763U, 3936U, 65463U, 37565U, 46252U, 48384U, 23713U, 58095U, 16439U, 7225U, 47584U, 9393U, 49847U, 12845U, 12926U, 52415U, 41945U, 12842U, 36287U, 16846U, 14984U, 4368U, 9330U, 11130U, 23485U, 52289U, 40482U, 55518U, 34858U, 5511U, 45650U, 21259U, 41094U, 58277U, 33125U, 49642U, 17730U, 11619U, 52071U, 11137U, 16461U, 16318U, 53804U, 62725U, 65010U, 26442U, 41245U, 18996U, 63139U, 60157U, 25638U, 40885U, 1796U, 5987U, 782U, 29410U, 5418U, 57587U, 50219U, 32705U, 49997U, 43373U, 4211U, 13935U, 56700U, 48282U, 3759U, 65398U, 7736U, 32865U, 4528U, 49093U, 20779U, 60340U, 36599U, 13939U, 29553U, 47719U, 54966U, 9491U, 51689U, 53838U, 46209U, 62006U, 24494U, 5221U, 57178U, 40411U, 59993U, 32495U, 59106U, 58606U, 21196U, 36010U, 29111U, 21113U, 65462U, 7746U, 26062U, 22137U, 56832U, 52455U, 5037U, 63286U, 55644U, 52438U, 751U, 60981U, 19552U, 27328U, 21622U, 37974U, 16879U, 47738U, 49004U, 28135U, 52195U, 38035U, 19148U, 12295U, 32484U, 58400U, 54936U, 44838U, 60072U, 11948U, 63392U, 18004U, 54777U, 14022U, 6570U, 4490U, 45810U, 40580U, 21348U, 43396U, 52468U, 55779U, 18392U, 24733U, 65488U, 45167U, 1308U, 31772U, 31852U, 39878U, 9513U, 36424U, 10457U, 18441U, 10174U, 48750U, 7049U, 53911U, 20514U, 14029U, 41567U, 23585U, 60658U, 39905U, 3862U, 62800U, 36147U, 48171U, 6485U, 21638U, 20362U, 12987U, 62962U, 55003U, 57118U, 20803U, 64991U, 27958U, 584U, 44741U, 38985U, 40632U, 2988U, 40618U, 16372U, 20243U, 19069U, 30521U, 40903U, 35762U, 57768U, 13079U, 60216U, 54351U, 29280U, 40210U, 40705U, 61788U, 48006U, 2659U, 4464U, 64938U, 48869U, 270U, 13636U, 4588U, 53735U, 3552U, 32870U, 7618U, 26193U, 27599U, 61223U, 22757U, 30164U, 38075U, 9696U, 53831U, 35553U, 21221U, 40587U, 40228U, 49983U, 56904U, 25484U, 27487U, 39301U, 47518U, 40916U, 3335U, 24621U, 38484U, 29009U, 50689U, 4425U, 19934U, 24906U, 9675U, 8711U, 34201U, 38498U, 19857U, 34087U, 29844U, 58399U, 58769U, 26279U, 53851U, 15459U, 13890U, 13106U, 24843U, 63568U, 52502U, 62943U, 55370U, 14689U, 32278U, 37464U, 40476U, 45427U, 17365U, 15635U, 61192U, 14568U, 23866U, 6886U, 51038U, 30792U, 45152U, 36193U, 29873U, 19638U, 17409U, 4259U, 49503U, 2919U, 46146U, 7695U, 55318U, 55365U, 29609U, 34502U, 12937U, 26852U, 59238U, 20213U, 58368U, 25983U, 53150U, 37279U, 4677U, 54535U, 34722U, 58266U, 42991U, 27087U, 23378U, 152U, 57784U, 21668U, 40710U, 35355U, 56294U, 59036U, 39190U, 3833U, 50167U, 54946U, 11126U, 50807U, 61414U, 33608U, 47726U, 59387U, 62699U, 37608U, 44366U, 63280U, 40747U, 29671U, 7148U, 24467U, 60426U, 18052U, 6900U, 2031U, 35156U, 44503U, 38871U, 16857U, 17885U, 46957U, 18169U, 1258U, 34648U, 61544U, 56843U, 55711U, 43702U, 45680U, 36631U, 32254U, 27653U, 24570U, 16354U, 42141U, 24605U, 30710U, 20629U, 58887U, 53263U, 14201U, 21603U, 38757U, 17375U, 17643U, 58994U, 15369U, 63U, 29545U, 4627U, 59384U, 30371U, 18257U, 33858U, 11556U, 63854U, 8960U, 10255U, 58004U, 52691U, 9787U, 23102U, 16999U, 43202U, 64848U, 58323U, 30861U, 14508U, 52895U, 42509U, 58880U, 45961U, 58489U, 58142U, 54112U, 22993U, 64719U, 22034U, 64015U, 49802U, 58769U, 42715U, 6785U, 50377U, 53427U, 55989U, 48687U, 22017U, 10569U, 379U, 37676U, 10830U, 1246U, 40126U, 48396U, 58962U, 594U, 46486U, 30050U, 18822U, 46192U, 28866U, 30064U, 14341U, 31881U, 20987U, 39253U, 22220U, 3306U, 55845U, 48470U, 13637U, 18273U, 2663U, 61155U, 4612U, 10522U, 40320U, 64518U, 9677U, 30413U, 65279U, 4832U, 44372U, 23311U, 26943U, 5417U, 9336U, 48856U, 7474U, 46142U, 31178U, 21367U, 15119U, 31497U, 21982U, 15643U, 52306U, 2060U, 16231U, 23923U, 42450U, 30997U, 31584U, 58332U, 65287U, 40530U, 30282U, 32112U, 28661U, 49795U, 56680U, 3081U, 56836U, 25446U, 943U, 60129U, 51776U, 21231U, 49976U, 7297U, 26778U, 6073U, 26982U, 52628U, 5411U, 61503U, 60474U, 56321U, 9778U, 23744U, 62321U, 40760U, 38136U, 15155U, 60073U, 34215U, 32142U, 52650U, 52079U, 18510U, 62332U }  ;
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
    return 0;
}

