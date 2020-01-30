
#include "Device.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>


void fill_DeviceVersion(p0_DeviceVersion * const p0);
void check_DeviceVersion(p0_DeviceVersion * const p0);

void fill_BusConfiguration(p6_BusConfiguration * const p6);
void check_BusConfiguration(p6_BusConfiguration * const p6);

void fill_InstructionsPack(p7_InstructionsPack * const p7);
void check_InstructionsPack(p7_InstructionsPack * const p7);

void fill_DeviceError(p8_DeviceError * const p8);
void check_DeviceError(p8_DeviceError * const p8);

void fill_SensorsData(p9_SensorsData * const p9);
void check_SensorsData(p9_SensorsData * const p9);

typedef struct
{
    const Pack * test_pack;
    c_Communication channel;
} c_Communication_TEST;

bool c_Communication_send(c_Communication * dst, const Pack * pack)
{
    c_Communication_TEST * ch = (c_Communication_TEST *)((uint8_t*)dst - offsetof(c_Communication_TEST, channel));
    return  ch->test_pack ? false : (ch->test_pack = pack), true;
}

const Pack * Communication_TEST_pull(Transmitter * transmitter)
{
    c_Communication_TEST * ch = (c_Communication_TEST *)((uint8_t*)transmitter - offsetof(c_Communication_TEST, channel) - offsetof(c_Communication, transmitter));
    const Pack * pack = ch->test_pack;
    ch->test_pack = NULL;
    return pack;
}






/**
 * send Stop
 * param c_TEST_Channel * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_TEST_Channel_send_Stop( c_TEST_Channel) ( c_TEST_Channel_send ( c_TEST_Channel , m1_Stop.pack_alloc(NULL) ) )

/**
 * send Start
 * param c_TEST_Channel * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_TEST_Channel_send_Start( c_TEST_Channel) ( c_TEST_Channel_send ( c_TEST_Channel , m2_Start.pack_alloc(NULL) ) )

/**
 * send GetDeviceVersion
 * param c_TEST_Channel * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_TEST_Channel_send_GetDeviceVersion( c_TEST_Channel) ( c_TEST_Channel_send ( c_TEST_Channel , m3_GetDeviceVersion.pack_alloc(NULL) ) )

/**
 * send GetConfiguration
 * param c_TEST_Channel * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_TEST_Channel_send_GetConfiguration( c_TEST_Channel) ( c_TEST_Channel_send ( c_TEST_Channel , m4_GetConfiguration.pack_alloc(NULL) ) )

/**
 * send SetConfiguration
 * param c_TEST_Channel * pointer to channel that contains transmitter
 * return true if add pack to the sending queue succeed
 */
#define c_TEST_Channel_send_SetConfiguration( c_TEST_Channel) ( c_TEST_Channel_send ( c_TEST_Channel , m5_SetConfiguration.pack_alloc(NULL) ) )


typedef struct
{
    Pack * test_pack;
    Transmitter transmitter;
    Receiver receiver;
} c_TEST_Channel;
bool c_TEST_Channel_send(c_TEST_Channel * dst, Pack  * const pack) { return dst->test_pack ? false : (dst->test_pack = pack), true;}

const Pack * TEST_Channel_pull(Transmitter * transmitter)
{
    c_TEST_Channel * ch = (c_TEST_Channel *)((uint8_t*)transmitter - offsetof(c_TEST_Channel, transmitter));
    Pack * pack = ch->test_pack;
    ch->test_pack = NULL;
    return pack;
}

static inline void TEST_Channel_on_DeviceVersion(c_TEST_Channel * channel,  DeviceVersion_0 * p0)
{
    check_DeviceVersion(p0->bytes);
    printf("DeviceVersion pass \n");
}
static inline void TEST_Channel_on_DeviceError(c_TEST_Channel * channel,  DeviceError_8 * p8)
{
    check_DeviceError(p8->bytes);
    printf("DeviceError pass \n");
}
static inline void TEST_Channel_on_SensorsData(c_TEST_Channel * channel,  SensorsData_9 * p9)
{
    check_SensorsData(p9->bytes);
    printf("SensorsData pass \n");
}



#define c_TEST_Channel_DISPATCHER(CHANNEL)\
		switch (id)\
		{\
			default:    return NULL;\
			case 0:	return pack ? CHANNEL##_on_DeviceVersion( channel, pack ), (Meta const*)NULL: &m0_DeviceVersion;\
			case 8:	return pack ? CHANNEL##_on_DeviceError( channel, pack ), (Meta const*)NULL: &m8_DeviceError;\
			case 9:	return pack ? CHANNEL##_on_SensorsData( channel, pack ), (Meta const*)NULL: &m9_SensorsData;\
		}


static inline Meta const * TEST_Channel_dispatcher(Receiver* receiver, size_t id, Pack* pack)
{
    c_TEST_Channel * channel  = (c_TEST_Channel *)((uint8_t*)receiver - offsetof(c_TEST_Channel, receiver));
    c_TEST_Channel_DISPATCHER(TEST_Channel)
}





void fill_DeviceVersion(p0_DeviceVersion * const p0)
{
    p0_DevID_SET(30176U, p0);
    p0_Ver_SET(43899U, p0);
}

void check_DeviceVersion(p0_DeviceVersion * const p0)
{
    assert(p0_DevID_GET(p0) == 30176U);
    assert(p0_Ver_GET(p0) == 43899U);
}




void fill_BusConfiguration(p6_BusConfiguration * const p6)
{
    p6_multiplier_SET(140U, p6);
    p6_time_SET(6426U, p6);
    p6_clk_khz_SET(40696U, p6);
}

void check_BusConfiguration(p6_BusConfiguration * const p6)
{
    assert(p6_multiplier_GET(p6) == 140U);
    assert(p6_time_GET(p6) == 6426U);
    assert(p6_clk_khz_GET(p6) == 40696U);
}




void fill_InstructionsPack(p7_InstructionsPack * const p7)
{
    p7_Length_SET(151U, p7);
    {
        const uint8_t src[] = { 152U, 95U, 210U, 43U, 125U, 132U, 25U, 38U, 70U, 73U, 208U, 149U, 159U, 10U, 87U, 130U, 7U, 225U, 54U, 113U, 226U, 72U, 231U, 4U, 252U, 138U, 140U, 227U, 253U, 233U, 179U, 102U, 129U, 41U, 74U, 75U, 242U, 237U, 225U, 17U, 225U, 164U, 84U, 242U, 238U, 84U, 48U, 208U, 134U, 41U, 243U, 59U, 4U, 245U, 45U, 212U, 223U, 162U, 205U, 242U, 204U, 56U, 248U, 18U, 154U, 111U, 141U, 83U, 183U, 17U, 114U, 74U, 136U, 113U, 198U, 170U, 145U, 157U, 49U, 237U, 174U, 206U, 0U, 76U, 57U, 135U, 138U, 242U, 110U, 58U, 182U, 253U, 233U, 109U, 76U, 53U, 73U, 233U, 21U, 26U, 90U, 33U, 120U, 255U, 212U, 178U, 48U, 79U, 254U, 202U, 117U, 165U, 104U, 254U, 197U, 192U, 65U, 93U, 100U, 152U, 83U, 79U, 25U, 33U, 202U, 230U, 45U, 163U, 59U, 203U, 29U, 185U, 180U, 198U, 107U, 132U, 250U, 16U, 68U, 246U, 253U, 44U, 32U, 13U, 44U, 184U, 144U, 114U, 189U, 126U, 194U, 181U, 42U, 84U, 253U, 26U, 6U, 53U, 144U, 49U, 147U, 236U, 218U, 201U, 197U, 190U, 249U, 19U, 193U, 99U, 70U, 184U, 167U, 227U, 218U, 157U, 178U, 177U, 34U, 71U, 219U, 52U, 14U, 5U, 131U, 131U, 28U, 193U, 118U, 243U, 91U, 218U, 30U, 24U, 78U, 240U, 61U, 242U, 148U, 42U, 70U, 131U, 252U, 121U, 200U, 25U, 74U, 134U, 31U, 98U, 158U, 1U, 119U, 249U, 178U, 86U, 187U, 192U, 22U, 157U, 34U, 198U, 211U, 124U, 207U, 142U, 99U, 202U, 74U, 159U, 99U, 223U, 250U, 77U, 126U, 125U, 194U, 114U, 243U, 147U, 124U, 161U, 241U, 6U, 195U, 37U, 182U, 85U, 233U, 140U, 165U, 178U, 231U, 62U, 251U, 208U } ;
        p7_Instructions_SET(src, p7);
    }
}

void check_InstructionsPack(p7_InstructionsPack * const p7)
{
    assert(p7_Length_GET(p7) == 151U);
    {
        const uint8_t exemplary[] = { 152U, 95U, 210U, 43U, 125U, 132U, 25U, 38U, 70U, 73U, 208U, 149U, 159U, 10U, 87U, 130U, 7U, 225U, 54U, 113U, 226U, 72U, 231U, 4U, 252U, 138U, 140U, 227U, 253U, 233U, 179U, 102U, 129U, 41U, 74U, 75U, 242U, 237U, 225U, 17U, 225U, 164U, 84U, 242U, 238U, 84U, 48U, 208U, 134U, 41U, 243U, 59U, 4U, 245U, 45U, 212U, 223U, 162U, 205U, 242U, 204U, 56U, 248U, 18U, 154U, 111U, 141U, 83U, 183U, 17U, 114U, 74U, 136U, 113U, 198U, 170U, 145U, 157U, 49U, 237U, 174U, 206U, 0U, 76U, 57U, 135U, 138U, 242U, 110U, 58U, 182U, 253U, 233U, 109U, 76U, 53U, 73U, 233U, 21U, 26U, 90U, 33U, 120U, 255U, 212U, 178U, 48U, 79U, 254U, 202U, 117U, 165U, 104U, 254U, 197U, 192U, 65U, 93U, 100U, 152U, 83U, 79U, 25U, 33U, 202U, 230U, 45U, 163U, 59U, 203U, 29U, 185U, 180U, 198U, 107U, 132U, 250U, 16U, 68U, 246U, 253U, 44U, 32U, 13U, 44U, 184U, 144U, 114U, 189U, 126U, 194U, 181U, 42U, 84U, 253U, 26U, 6U, 53U, 144U, 49U, 147U, 236U, 218U, 201U, 197U, 190U, 249U, 19U, 193U, 99U, 70U, 184U, 167U, 227U, 218U, 157U, 178U, 177U, 34U, 71U, 219U, 52U, 14U, 5U, 131U, 131U, 28U, 193U, 118U, 243U, 91U, 218U, 30U, 24U, 78U, 240U, 61U, 242U, 148U, 42U, 70U, 131U, 252U, 121U, 200U, 25U, 74U, 134U, 31U, 98U, 158U, 1U, 119U, 249U, 178U, 86U, 187U, 192U, 22U, 157U, 34U, 198U, 211U, 124U, 207U, 142U, 99U, 202U, 74U, 159U, 99U, 223U, 250U, 77U, 126U, 125U, 194U, 114U, 243U, 147U, 124U, 161U, 241U, 6U, 195U, 37U, 182U, 85U, 233U, 140U, 165U, 178U, 231U, 62U, 251U, 208U }  ;
        V7_Instructions item_Instructions = p7_Instructions_GET(p7);
        assert(item_Instructions.len == 256);
        for(size_t index = 0; index < item_Instructions.len; index++)
            assert(v7_Instructions_GET(&item_Instructions, index) == exemplary[index]);
    }
}




void fill_DeviceError(p8_DeviceError * const p8)
{
    p8_param1_SET(35339U, p8);
    p8_error_id_SET(e_Errors_CannotReadSensor, p8);
}

void check_DeviceError(p8_DeviceError * const p8)
{
    assert(p8_param1_GET(p8) == 35339U);
    assert(p8_error_id_GET(p8) == e_Errors_CannotReadSensor);
}




void fill_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t src[] = { 52275U, 53998U, 55795U, 26724U, 29469U, 41771U, 14964U, 35760U, 33762U, 13184U, 49145U, 13857U, 18814U, 31330U, 52132U, 27473U, 59675U, 39662U, 24930U, 64497U, 50577U, 5818U, 291U, 41844U, 28406U, 50603U, 28622U, 35733U, 60981U, 11688U, 29549U, 26582U, 6231U, 12817U, 60329U, 54350U, 9558U, 7279U, 24517U, 20272U, 34332U, 52631U, 52477U, 38458U, 55078U, 26957U, 58505U, 24859U, 45278U, 38841U, 42183U, 19542U, 57932U, 52665U, 755U, 57676U, 55751U, 24202U, 56869U, 44597U, 33011U, 30254U, 42356U, 57773U, 19712U, 2549U, 46427U, 1410U, 16046U, 52497U, 41337U, 63311U, 19995U, 19009U, 52038U, 9688U, 14809U, 4241U, 11433U, 14242U, 5287U, 9380U, 41587U, 63048U, 3113U, 37854U, 54844U, 11234U, 20946U, 32429U, 45247U, 16912U, 49900U, 31630U, 12911U, 33392U, 20140U, 34951U, 31000U, 43250U, 19003U, 65417U, 34800U, 6660U, 57835U, 3134U, 49270U, 5344U, 37064U, 14415U, 61621U, 21156U, 36962U, 61210U, 53933U, 53818U, 23008U, 53772U, 56363U, 50533U, 54843U, 33441U, 40774U, 54184U, 54257U, 20821U, 28267U, 15794U, 20775U, 10645U, 35034U, 51780U, 65513U, 31713U, 33985U, 860U, 50305U, 22730U, 52476U, 3697U, 31472U, 38982U, 54497U, 60743U, 64890U, 44165U, 45087U, 29456U, 12585U, 34158U, 5202U, 34534U, 42904U, 23421U, 46787U, 59382U, 64633U, 7144U, 2728U, 60598U, 4314U, 14250U, 8244U, 63600U, 46278U, 60895U, 29336U, 40938U, 33466U, 44228U, 50080U, 59745U, 8350U, 1235U, 33285U, 43763U, 32733U, 53633U, 27752U, 28363U, 16349U, 47617U, 25428U, 31761U, 36412U, 10395U, 15226U, 10259U, 22103U, 61254U, 22792U, 51792U, 43161U, 9099U, 24631U, 4982U, 11611U, 12921U, 4009U, 36376U, 11663U, 34004U, 11497U, 37225U, 54858U, 43567U, 35308U, 18157U, 54796U, 49542U, 11573U, 50200U, 36029U, 23982U, 53599U, 64386U, 27627U, 31489U, 64901U, 3750U, 20083U, 497U, 35392U, 39622U, 38407U, 21501U, 34847U, 43146U, 10187U, 43325U, 47867U, 33130U, 29054U, 57726U, 57940U, 7560U, 4883U, 33548U, 56077U, 22903U, 63564U, 61027U, 45933U, 54641U, 32881U, 52432U, 31698U, 32382U, 59498U, 40828U, 36375U, 22968U, 49670U, 9887U, 34645U, 60274U, 36029U, 28788U, 18214U, 45173U, 10964U, 11077U, 38822U, 5311U, 59338U, 32472U, 35675U, 6279U, 5351U, 13456U, 4188U, 38928U, 31079U, 18492U, 43834U, 9933U, 1506U, 710U, 50886U, 24582U, 23645U, 27963U, 61037U, 32306U, 39246U, 36213U, 43756U, 26749U, 35716U, 11239U, 48834U, 43879U, 53783U, 35599U, 5660U, 37769U, 14253U, 50252U, 55125U, 19991U, 43449U, 22145U, 13094U, 2741U, 7596U, 34871U, 56496U, 14974U, 55132U, 23001U, 21791U, 22882U, 48730U, 41510U, 5526U, 9015U, 25778U, 20162U, 23719U, 33204U, 54589U, 43027U, 53342U, 25535U, 12880U, 43553U, 16616U, 56258U, 28009U, 35272U, 17975U, 53021U, 8785U, 9168U, 37051U, 13730U, 21565U, 944U, 34230U, 33469U, 31050U, 41768U, 35871U, 59319U, 64643U, 27948U, 3492U, 49279U, 38448U, 47665U, 51170U, 15101U, 64099U, 45069U, 62042U, 29153U, 38220U, 52539U, 8682U, 15612U, 18744U, 64890U, 21625U, 29805U, 13162U, 12326U, 5752U, 29680U, 33422U, 56890U, 30601U, 40121U, 53631U, 18592U, 59192U, 53410U, 1958U, 44928U, 42399U, 8959U, 46969U, 31152U, 31069U, 62568U, 8212U, 55199U, 48011U, 60538U, 10282U, 39882U, 30611U, 49497U, 41489U, 36155U, 47619U, 43052U, 24933U, 19623U, 41104U, 35395U, 32725U, 48158U, 14118U, 63906U, 2373U, 62443U, 12257U, 63597U, 5494U, 29080U, 40460U, 31618U, 33652U, 36473U, 2132U, 29816U, 41411U, 31934U, 65369U, 11893U, 55758U, 51959U, 18554U, 60544U, 21401U, 26316U, 4908U, 44027U, 22368U, 9310U, 53524U, 61396U, 53605U, 47314U, 36471U, 18772U, 14885U, 30725U, 59182U, 58422U, 13359U, 22029U, 63580U, 50716U, 4317U, 14448U, 62536U, 24631U, 56487U, 1950U, 36248U, 39452U, 26293U, 41051U, 9694U, 43416U, 52962U, 10431U, 61455U, 57044U, 64983U, 54636U, 64229U, 28582U, 4292U, 27073U, 21549U, 40003U, 40902U, 42790U, 49858U, 48811U, 46777U, 7963U, 36169U, 18726U, 59222U, 63704U, 43730U, 43741U, 31934U, 621U, 53628U, 19135U, 35925U, 35023U, 18366U, 29630U, 9635U, 54752U, 42651U, 13469U, 29869U, 25854U, 34888U, 13481U, 27853U, 55552U, 58353U, 35279U, 24703U, 11140U, 52393U, 59305U, 45647U, 12926U, 31391U, 1522U, 49690U, 14294U, 65171U, 65534U, 39852U, 17191U, 29403U, 15250U, 658U, 44076U, 63020U, 41885U, 2417U, 38287U, 47982U, 32308U, 32919U, 27067U, 64336U, 58895U, 61923U, 50230U, 31126U, 15785U, 9124U, 37301U, 29012U, 29729U, 37475U, 14536U, 25703U, 5347U, 49004U, 58660U, 29188U, 48293U, 36716U, 33922U, 55914U, 14227U, 60133U, 65180U, 6261U, 2330U, 22607U, 63460U, 18533U, 4694U, 58528U, 41275U, 10420U, 57371U, 13324U, 38422U, 35817U, 33669U, 23757U, 57332U, 7539U, 35730U, 9567U, 26038U, 37598U, 7973U, 14339U, 42592U, 39410U, 32679U, 6838U, 22124U, 45253U, 6250U, 42038U, 53330U, 7825U, 35425U, 24241U, 46198U, 31928U, 50646U, 15847U, 49347U, 53000U, 47946U, 59107U, 4736U, 3184U, 48548U, 22816U, 28353U, 35800U, 10056U, 46919U, 9498U, 53870U, 59284U, 57957U, 33238U, 60884U, 7428U, 45762U, 487U, 25979U, 1625U, 39228U, 46031U, 33268U, 4670U, 23747U, 30121U, 2090U, 805U, 45681U, 49260U, 34245U, 51068U, 893U, 34341U, 5533U, 9657U, 32198U, 50551U, 49705U, 21721U, 2890U, 41170U, 2875U, 59843U, 37877U, 39896U, 63395U, 17078U, 27581U, 26431U, 62083U, 60171U, 39235U, 14404U, 12133U, 48992U, 11378U, 9968U, 26717U, 36862U, 35439U, 48551U, 28230U, 10063U, 2712U, 27787U, 1807U, 775U, 55455U, 4485U, 13109U, 45061U, 50202U, 57087U, 10367U, 19571U, 28099U, 34673U, 57861U, 31267U, 42336U, 7323U, 22413U, 15708U, 4778U, 32655U, 46897U, 53181U, 55828U, 20603U, 5182U, 46180U, 50232U, 1494U, 64086U, 18717U, 58304U, 56958U, 22552U, 51957U, 57322U, 23100U, 15572U, 9622U, 37523U, 41189U, 44769U, 7990U, 17722U, 49083U, 49998U, 35073U, 64786U, 22367U, 43912U, 12335U, 46462U, 46663U, 24791U, 42259U, 48438U, 26806U, 16547U, 19458U, 41527U, 59096U, 30405U, 53173U, 968U, 55557U, 40416U, 29708U, 14401U, 2144U, 52339U, 64317U, 5934U, 33503U, 22879U, 18153U, 45738U, 18068U, 32504U, 27696U, 60605U, 54490U, 47231U, 40467U, 5529U, 29302U, 26662U, 18959U, 40090U, 46076U, 21925U, 7133U, 64904U, 65031U, 39241U, 35771U, 63569U, 64436U, 1087U, 63828U, 29535U, 47565U, 52505U, 37237U, 11398U, 17408U, 63243U, 22928U, 20430U, 55172U, 8619U, 53438U, 19136U, 33485U, 60079U, 49127U, 45435U, 33696U, 8253U, 3702U, 7090U, 26547U, 43508U, 7077U, 58056U, 55009U, 49740U, 38841U, 38961U, 55282U, 34284U, 40853U, 42696U, 17495U, 35027U, 16291U, 22604U, 53724U, 14948U, 39879U, 3788U, 60221U, 132U, 43929U, 59508U, 27542U, 3274U, 2302U, 22414U, 3140U, 33215U, 28354U, 50884U, 3641U, 60958U, 29386U, 15376U, 28621U, 5543U, 29259U, 60770U, 48816U, 46517U, 20398U, 21257U, 26403U, 24123U, 23141U, 10910U, 64373U, 22386U, 18176U, 31481U, 59485U, 32261U, 53107U, 4591U, 9345U, 52186U, 12747U, 25963U, 62199U, 5254U, 63979U, 29288U, 13578U, 24015U, 63971U, 58208U, 28040U, 42520U, 5578U, 61823U, 43709U, 47037U, 52346U, 23900U, 638U, 29275U, 48674U, 50604U, 63563U, 38957U, 4377U, 14052U, 52396U, 520U, 22074U, 58917U, 44721U, 41969U, 33532U, 60375U, 17168U, 36845U, 53944U, 14632U, 23972U, 30750U, 21132U, 2316U, 31108U, 2350U, 20747U, 34660U, 54493U, 5945U, 35152U, 60580U, 25366U, 21851U, 2992U, 42193U, 59379U, 19220U, 20670U, 45112U, 46493U, 33903U, 15728U, 32694U, 51731U, 33074U, 10897U, 40136U, 2028U, 25854U, 36062U, 61621U, 60844U, 31126U, 42685U, 64006U, 32618U, 56563U, 24698U, 25519U, 54315U, 37643U, 4168U, 56767U, 13915U, 22918U, 35861U, 34638U, 65371U, 60854U, 39174U, 62500U, 32856U, 1551U, 41886U, 44491U, 65075U, 58063U, 36342U, 51411U, 65091U, 32109U, 38445U, 50988U, 9883U, 47052U, 43907U, 3057U, 28173U, 174U, 33174U, 16126U, 35668U, 52402U, 44354U, 54075U, 40081U, 2788U, 22074U, 52646U, 50204U, 46100U, 29925U, 43630U, 23255U, 34022U, 34589U, 17750U, 2940U, 1308U, 30071U, 37714U, 44574U, 20588U, 64166U, 31784U, 24090U, 34114U, 34262U, 40310U, 42758U, 28334U, 8801U, 22014U, 64538U, 17421U, 58978U, 52473U, 33641U, 44268U, 11090U, 45971U, 25182U, 37265U, 29167U, 718U, 60516U, 21074U, 27371U, 10298U, 25870U, 22008U, 47394U, 20274U, 405U, 56504U, 64954U } ;
        p9_values_SET(src, p9);
    }
}

void check_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t exemplary[] = { 52275U, 53998U, 55795U, 26724U, 29469U, 41771U, 14964U, 35760U, 33762U, 13184U, 49145U, 13857U, 18814U, 31330U, 52132U, 27473U, 59675U, 39662U, 24930U, 64497U, 50577U, 5818U, 291U, 41844U, 28406U, 50603U, 28622U, 35733U, 60981U, 11688U, 29549U, 26582U, 6231U, 12817U, 60329U, 54350U, 9558U, 7279U, 24517U, 20272U, 34332U, 52631U, 52477U, 38458U, 55078U, 26957U, 58505U, 24859U, 45278U, 38841U, 42183U, 19542U, 57932U, 52665U, 755U, 57676U, 55751U, 24202U, 56869U, 44597U, 33011U, 30254U, 42356U, 57773U, 19712U, 2549U, 46427U, 1410U, 16046U, 52497U, 41337U, 63311U, 19995U, 19009U, 52038U, 9688U, 14809U, 4241U, 11433U, 14242U, 5287U, 9380U, 41587U, 63048U, 3113U, 37854U, 54844U, 11234U, 20946U, 32429U, 45247U, 16912U, 49900U, 31630U, 12911U, 33392U, 20140U, 34951U, 31000U, 43250U, 19003U, 65417U, 34800U, 6660U, 57835U, 3134U, 49270U, 5344U, 37064U, 14415U, 61621U, 21156U, 36962U, 61210U, 53933U, 53818U, 23008U, 53772U, 56363U, 50533U, 54843U, 33441U, 40774U, 54184U, 54257U, 20821U, 28267U, 15794U, 20775U, 10645U, 35034U, 51780U, 65513U, 31713U, 33985U, 860U, 50305U, 22730U, 52476U, 3697U, 31472U, 38982U, 54497U, 60743U, 64890U, 44165U, 45087U, 29456U, 12585U, 34158U, 5202U, 34534U, 42904U, 23421U, 46787U, 59382U, 64633U, 7144U, 2728U, 60598U, 4314U, 14250U, 8244U, 63600U, 46278U, 60895U, 29336U, 40938U, 33466U, 44228U, 50080U, 59745U, 8350U, 1235U, 33285U, 43763U, 32733U, 53633U, 27752U, 28363U, 16349U, 47617U, 25428U, 31761U, 36412U, 10395U, 15226U, 10259U, 22103U, 61254U, 22792U, 51792U, 43161U, 9099U, 24631U, 4982U, 11611U, 12921U, 4009U, 36376U, 11663U, 34004U, 11497U, 37225U, 54858U, 43567U, 35308U, 18157U, 54796U, 49542U, 11573U, 50200U, 36029U, 23982U, 53599U, 64386U, 27627U, 31489U, 64901U, 3750U, 20083U, 497U, 35392U, 39622U, 38407U, 21501U, 34847U, 43146U, 10187U, 43325U, 47867U, 33130U, 29054U, 57726U, 57940U, 7560U, 4883U, 33548U, 56077U, 22903U, 63564U, 61027U, 45933U, 54641U, 32881U, 52432U, 31698U, 32382U, 59498U, 40828U, 36375U, 22968U, 49670U, 9887U, 34645U, 60274U, 36029U, 28788U, 18214U, 45173U, 10964U, 11077U, 38822U, 5311U, 59338U, 32472U, 35675U, 6279U, 5351U, 13456U, 4188U, 38928U, 31079U, 18492U, 43834U, 9933U, 1506U, 710U, 50886U, 24582U, 23645U, 27963U, 61037U, 32306U, 39246U, 36213U, 43756U, 26749U, 35716U, 11239U, 48834U, 43879U, 53783U, 35599U, 5660U, 37769U, 14253U, 50252U, 55125U, 19991U, 43449U, 22145U, 13094U, 2741U, 7596U, 34871U, 56496U, 14974U, 55132U, 23001U, 21791U, 22882U, 48730U, 41510U, 5526U, 9015U, 25778U, 20162U, 23719U, 33204U, 54589U, 43027U, 53342U, 25535U, 12880U, 43553U, 16616U, 56258U, 28009U, 35272U, 17975U, 53021U, 8785U, 9168U, 37051U, 13730U, 21565U, 944U, 34230U, 33469U, 31050U, 41768U, 35871U, 59319U, 64643U, 27948U, 3492U, 49279U, 38448U, 47665U, 51170U, 15101U, 64099U, 45069U, 62042U, 29153U, 38220U, 52539U, 8682U, 15612U, 18744U, 64890U, 21625U, 29805U, 13162U, 12326U, 5752U, 29680U, 33422U, 56890U, 30601U, 40121U, 53631U, 18592U, 59192U, 53410U, 1958U, 44928U, 42399U, 8959U, 46969U, 31152U, 31069U, 62568U, 8212U, 55199U, 48011U, 60538U, 10282U, 39882U, 30611U, 49497U, 41489U, 36155U, 47619U, 43052U, 24933U, 19623U, 41104U, 35395U, 32725U, 48158U, 14118U, 63906U, 2373U, 62443U, 12257U, 63597U, 5494U, 29080U, 40460U, 31618U, 33652U, 36473U, 2132U, 29816U, 41411U, 31934U, 65369U, 11893U, 55758U, 51959U, 18554U, 60544U, 21401U, 26316U, 4908U, 44027U, 22368U, 9310U, 53524U, 61396U, 53605U, 47314U, 36471U, 18772U, 14885U, 30725U, 59182U, 58422U, 13359U, 22029U, 63580U, 50716U, 4317U, 14448U, 62536U, 24631U, 56487U, 1950U, 36248U, 39452U, 26293U, 41051U, 9694U, 43416U, 52962U, 10431U, 61455U, 57044U, 64983U, 54636U, 64229U, 28582U, 4292U, 27073U, 21549U, 40003U, 40902U, 42790U, 49858U, 48811U, 46777U, 7963U, 36169U, 18726U, 59222U, 63704U, 43730U, 43741U, 31934U, 621U, 53628U, 19135U, 35925U, 35023U, 18366U, 29630U, 9635U, 54752U, 42651U, 13469U, 29869U, 25854U, 34888U, 13481U, 27853U, 55552U, 58353U, 35279U, 24703U, 11140U, 52393U, 59305U, 45647U, 12926U, 31391U, 1522U, 49690U, 14294U, 65171U, 65534U, 39852U, 17191U, 29403U, 15250U, 658U, 44076U, 63020U, 41885U, 2417U, 38287U, 47982U, 32308U, 32919U, 27067U, 64336U, 58895U, 61923U, 50230U, 31126U, 15785U, 9124U, 37301U, 29012U, 29729U, 37475U, 14536U, 25703U, 5347U, 49004U, 58660U, 29188U, 48293U, 36716U, 33922U, 55914U, 14227U, 60133U, 65180U, 6261U, 2330U, 22607U, 63460U, 18533U, 4694U, 58528U, 41275U, 10420U, 57371U, 13324U, 38422U, 35817U, 33669U, 23757U, 57332U, 7539U, 35730U, 9567U, 26038U, 37598U, 7973U, 14339U, 42592U, 39410U, 32679U, 6838U, 22124U, 45253U, 6250U, 42038U, 53330U, 7825U, 35425U, 24241U, 46198U, 31928U, 50646U, 15847U, 49347U, 53000U, 47946U, 59107U, 4736U, 3184U, 48548U, 22816U, 28353U, 35800U, 10056U, 46919U, 9498U, 53870U, 59284U, 57957U, 33238U, 60884U, 7428U, 45762U, 487U, 25979U, 1625U, 39228U, 46031U, 33268U, 4670U, 23747U, 30121U, 2090U, 805U, 45681U, 49260U, 34245U, 51068U, 893U, 34341U, 5533U, 9657U, 32198U, 50551U, 49705U, 21721U, 2890U, 41170U, 2875U, 59843U, 37877U, 39896U, 63395U, 17078U, 27581U, 26431U, 62083U, 60171U, 39235U, 14404U, 12133U, 48992U, 11378U, 9968U, 26717U, 36862U, 35439U, 48551U, 28230U, 10063U, 2712U, 27787U, 1807U, 775U, 55455U, 4485U, 13109U, 45061U, 50202U, 57087U, 10367U, 19571U, 28099U, 34673U, 57861U, 31267U, 42336U, 7323U, 22413U, 15708U, 4778U, 32655U, 46897U, 53181U, 55828U, 20603U, 5182U, 46180U, 50232U, 1494U, 64086U, 18717U, 58304U, 56958U, 22552U, 51957U, 57322U, 23100U, 15572U, 9622U, 37523U, 41189U, 44769U, 7990U, 17722U, 49083U, 49998U, 35073U, 64786U, 22367U, 43912U, 12335U, 46462U, 46663U, 24791U, 42259U, 48438U, 26806U, 16547U, 19458U, 41527U, 59096U, 30405U, 53173U, 968U, 55557U, 40416U, 29708U, 14401U, 2144U, 52339U, 64317U, 5934U, 33503U, 22879U, 18153U, 45738U, 18068U, 32504U, 27696U, 60605U, 54490U, 47231U, 40467U, 5529U, 29302U, 26662U, 18959U, 40090U, 46076U, 21925U, 7133U, 64904U, 65031U, 39241U, 35771U, 63569U, 64436U, 1087U, 63828U, 29535U, 47565U, 52505U, 37237U, 11398U, 17408U, 63243U, 22928U, 20430U, 55172U, 8619U, 53438U, 19136U, 33485U, 60079U, 49127U, 45435U, 33696U, 8253U, 3702U, 7090U, 26547U, 43508U, 7077U, 58056U, 55009U, 49740U, 38841U, 38961U, 55282U, 34284U, 40853U, 42696U, 17495U, 35027U, 16291U, 22604U, 53724U, 14948U, 39879U, 3788U, 60221U, 132U, 43929U, 59508U, 27542U, 3274U, 2302U, 22414U, 3140U, 33215U, 28354U, 50884U, 3641U, 60958U, 29386U, 15376U, 28621U, 5543U, 29259U, 60770U, 48816U, 46517U, 20398U, 21257U, 26403U, 24123U, 23141U, 10910U, 64373U, 22386U, 18176U, 31481U, 59485U, 32261U, 53107U, 4591U, 9345U, 52186U, 12747U, 25963U, 62199U, 5254U, 63979U, 29288U, 13578U, 24015U, 63971U, 58208U, 28040U, 42520U, 5578U, 61823U, 43709U, 47037U, 52346U, 23900U, 638U, 29275U, 48674U, 50604U, 63563U, 38957U, 4377U, 14052U, 52396U, 520U, 22074U, 58917U, 44721U, 41969U, 33532U, 60375U, 17168U, 36845U, 53944U, 14632U, 23972U, 30750U, 21132U, 2316U, 31108U, 2350U, 20747U, 34660U, 54493U, 5945U, 35152U, 60580U, 25366U, 21851U, 2992U, 42193U, 59379U, 19220U, 20670U, 45112U, 46493U, 33903U, 15728U, 32694U, 51731U, 33074U, 10897U, 40136U, 2028U, 25854U, 36062U, 61621U, 60844U, 31126U, 42685U, 64006U, 32618U, 56563U, 24698U, 25519U, 54315U, 37643U, 4168U, 56767U, 13915U, 22918U, 35861U, 34638U, 65371U, 60854U, 39174U, 62500U, 32856U, 1551U, 41886U, 44491U, 65075U, 58063U, 36342U, 51411U, 65091U, 32109U, 38445U, 50988U, 9883U, 47052U, 43907U, 3057U, 28173U, 174U, 33174U, 16126U, 35668U, 52402U, 44354U, 54075U, 40081U, 2788U, 22074U, 52646U, 50204U, 46100U, 29925U, 43630U, 23255U, 34022U, 34589U, 17750U, 2940U, 1308U, 30071U, 37714U, 44574U, 20588U, 64166U, 31784U, 24090U, 34114U, 34262U, 40310U, 42758U, 28334U, 8801U, 22014U, 64538U, 17421U, 58978U, 52473U, 33641U, 44268U, 11090U, 45971U, 25182U, 37265U, 29167U, 718U, 60516U, 21074U, 27371U, 10298U, 25870U, 22008U, 47394U, 20274U, 405U, 56504U, 64954U }  ;
        V9_values item_values = p9_values_GET(p9);
        assert(item_values.len == 1000);
        for(size_t index = 0; index < item_values.len; index++)
            assert(v9_values_GET(&item_values, index) == exemplary[index]);
    }
}













static inline void p0_DeviceVersion_DevID_SET(uint16_t src, p0_DeviceVersion * dst)
{
    p0_DevID_SET(src,  dst);
}
static inline void p0_DeviceVersion_Ver_SET(uint16_t src, p0_DeviceVersion * dst)
{
    p0_Ver_SET(src,  dst);
}
p0_DeviceVersion_PUSH_INTO(p0_DeviceVersion)

static inline uint16_t p0_DeviceVersion_DevID_GET(p0_DeviceVersion * src)
{
    return p0_DevID_GET(src);
}
static inline uint16_t p0_DeviceVersion_Ver_GET(p0_DeviceVersion * src)
{
    return p0_Ver_GET(src);
}

p0_DeviceVersion_PULL_FROM(p0_DeviceVersion)



static inline void p6_BusConfiguration_multiplier_SET(uint8_t src, p6_BusConfiguration * dst)
{
    p6_multiplier_SET(src,  dst);
}
static inline void p6_BusConfiguration_time_SET(uint16_t src, p6_BusConfiguration * dst)
{
    p6_time_SET(src,  dst);
}
static inline void p6_BusConfiguration_clk_khz_SET(uint16_t src, p6_BusConfiguration * dst)
{
    p6_clk_khz_SET(src,  dst);
}
p6_BusConfiguration_PUSH_INTO(p6_BusConfiguration)

static inline uint8_t p6_BusConfiguration_multiplier_GET(p6_BusConfiguration * src)
{
    return p6_multiplier_GET(src);
}
static inline uint16_t p6_BusConfiguration_time_GET(p6_BusConfiguration * src)
{
    return p6_time_GET(src);
}
static inline uint16_t p6_BusConfiguration_clk_khz_GET(p6_BusConfiguration * src)
{
    return p6_clk_khz_GET(src);
}

p6_BusConfiguration_PULL_FROM(p6_BusConfiguration)



static inline void p7_InstructionsPack_Length_SET(uint8_t src, p7_InstructionsPack * dst)
{
    p7_Length_SET(src,  dst);
}
static inline void p7_InstructionsPack_Instructions_SET(V7_Instructions * src, p7_InstructionsPack * dst)
{
    V7_Instructions item_Instructions = p7_Instructions_SET(NULL,  dst);
    for(size_t index = 0; index < src->len; index++)
        v7_Instructions_SET(v7_Instructions_GET(src, index), index, &item_Instructions);
}

p7_InstructionsPack_PUSH_INTO(p7_InstructionsPack)

static inline uint8_t p7_InstructionsPack_Length_GET(p7_InstructionsPack * src)
{
    return p7_Length_GET(src);
}

static inline void p7_InstructionsPack_Instructions_GET(p7_InstructionsPack * src, V7_Instructions * dst)
{
    V7_Instructions item_Instructions = p7_Instructions_GET(src);
    for(size_t index = 0; index < item_Instructions.len; index++)
        v7_Instructions_SET(v7_Instructions_GET(&item_Instructions, index), index, dst);
}

p7_InstructionsPack_PULL_FROM(p7_InstructionsPack)



static inline void p8_DeviceError_param1_SET(uint16_t src, p8_DeviceError * dst)
{
    p8_param1_SET(src,  dst);
}
static inline void p8_DeviceError_error_id_SET(e_Errors src, p8_DeviceError * dst)
{
    p8_error_id_SET(src,  dst);
}
p8_DeviceError_PUSH_INTO(p8_DeviceError)

static inline uint16_t p8_DeviceError_param1_GET(p8_DeviceError * src)
{
    return p8_param1_GET(src);
}
static inline e_Errors p8_DeviceError_error_id_GET(p8_DeviceError * src)
{
    return p8_error_id_GET(src);
}

p8_DeviceError_PULL_FROM(p8_DeviceError)



static inline void p9_SensorsData_values_SET(V9_values * src, p9_SensorsData * dst)
{
    V9_values item_values = p9_values_SET(NULL,  dst);
    for(size_t index = 0; index < src->len; index++)
        v9_values_SET(v9_values_GET(src, index), index, &item_values);
}

p9_SensorsData_PUSH_INTO(p9_SensorsData)


static inline void p9_SensorsData_values_GET(p9_SensorsData * src, V9_values * dst)
{
    V9_values item_values = p9_values_GET(src);
    for(size_t index = 0; index < item_values.len; index++)
        v9_values_SET(v9_values_GET(&item_values, index), index, dst);
}

p9_SensorsData_PULL_FROM(p9_SensorsData)



static inline void  Communication_TEST_on_Stop(c_Communication_TEST * channel)
{
    printf("on_Stop \n");
}
static inline void  Communication_TEST_on_Start(c_Communication_TEST * channel)
{
    printf("on_Start \n");
}
static inline void  Communication_TEST_on_GetDeviceVersion(c_Communication_TEST * channel)
{
    printf("on_GetDeviceVersion \n");
}
static inline void  Communication_TEST_on_GetConfiguration(c_Communication_TEST * channel)
{
    printf("on_GetConfiguration \n");
}
static inline void  Communication_TEST_on_SetConfiguration(c_Communication_TEST * channel)
{
    printf("on_SetConfiguration \n");
}
static inline void Communication_TEST_on_BusConfiguration(c_Communication_TEST * channel,  BusConfiguration_6 * p6)
{
    check_BusConfiguration(p6->bytes);
    printf("BusConfiguration pass \n");
}
static inline void Communication_TEST_on_InstructionsPack(c_Communication_TEST * channel,  InstructionsPack_7 * p7)
{
    check_InstructionsPack(p7->bytes);
    printf("InstructionsPack pass \n");
}
static inline Meta const * Communication_TEST_dispatcher(Receiver* receiver, size_t id, Pack* pack)
{
    c_Communication_TEST * channel = (c_Communication_TEST *)((uint8_t*)receiver - offsetof(c_Communication_TEST, channel) - offsetof(c_Communication, receiver));
    c_Communication_DISPATCHER(Communication_TEST)
}



int main()
{
    static uint8_t buff[512];
    c_TEST_Channel TEST_Channel_instance = { .receiver.dispatch = TEST_Channel_dispatcher, .transmitter.pull = TEST_Channel_pull  };
    c_Communication_TEST Communication_instance = { .channel.receiver.dispatch = Communication_TEST_dispatcher, .channel.transmitter.pull = Communication_TEST_pull  };
    {
        DeviceVersion_0 * p0 = c_Communication_new_DeviceVersion() ;
        {
            fill_DeviceVersion(p0->bytes);
            check_DeviceVersion(p0->bytes);  //direct test.
        }
        if(! c_Communication_send_DeviceVersion(&Communication_instance.channel, p0))       //put pack into the channel send-buffer
        {
            free_pack(p0);
            p0 = NULL;
            assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        for(size_t len; (len = c_Communication_packs_into_bytes(&Communication_instance.channel, buff,  sizeof buff));)
            receive(buff, len, &TEST_Channel_instance.receiver);
    }
    {
        if(! c_TEST_Channel_send_Stop(&TEST_Channel_instance)) assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        for(size_t len; (len = transmit(&TEST_Channel_instance.transmitter, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        if(! c_TEST_Channel_send_Start(&TEST_Channel_instance)) assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        for(size_t len; (len = transmit(&TEST_Channel_instance.transmitter, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        if(! c_TEST_Channel_send_GetDeviceVersion(&TEST_Channel_instance)) assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        for(size_t len; (len = transmit(&TEST_Channel_instance.transmitter, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        if(! c_TEST_Channel_send_GetConfiguration(&TEST_Channel_instance)) assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        for(size_t len; (len = transmit(&TEST_Channel_instance.transmitter, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        if(! c_TEST_Channel_send_SetConfiguration(&TEST_Channel_instance)) assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        for(size_t len; (len = transmit(&TEST_Channel_instance.transmitter, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        BusConfiguration_6 * p6 = c_Communication_new_BusConfiguration() ;
        {
            fill_BusConfiguration(p6->bytes);
            check_BusConfiguration(p6->bytes);  //direct test.
        }
        if(! c_Communication_send_BusConfiguration(&Communication_instance.channel, p6))       //put pack into the channel send-buffer
        {
            free_pack(p6);
            p6 = NULL;
            assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        for(size_t len; (len = c_Communication_packs_into_bytes(&Communication_instance.channel, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        InstructionsPack_7 * p7 = c_Communication_new_InstructionsPack() ;
        {
            fill_InstructionsPack(p7->bytes);
            check_InstructionsPack(p7->bytes);  //direct test.
        }
        if(! c_Communication_send_InstructionsPack(&Communication_instance.channel, p7))       //put pack into the channel send-buffer
        {
            free_pack(p7);
            p7 = NULL;
            assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        for(size_t len; (len = c_Communication_packs_into_bytes(&Communication_instance.channel, buff, sizeof buff));)
            c_Communication_bytes_into_packs(buff,  len, &Communication_instance.channel);
    }
    {
        DeviceError_8 * p8 = c_Communication_new_DeviceError() ;
        {
            fill_DeviceError(p8->bytes);
            check_DeviceError(p8->bytes);  //direct test.
        }
        if(! c_Communication_send_DeviceError(&Communication_instance.channel, p8))       //put pack into the channel send-buffer
        {
            free_pack(p8);
            p8 = NULL;
            assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        for(size_t len; (len = c_Communication_packs_into_bytes(&Communication_instance.channel, buff,  sizeof buff));)
            receive(buff, len, &TEST_Channel_instance.receiver);
    }
    {
        SensorsData_9 * p9 = c_Communication_new_SensorsData() ;
        {
            fill_SensorsData(p9->bytes);
            check_SensorsData(p9->bytes);  //direct test.
        }
        if(! c_Communication_send_SensorsData(&Communication_instance.channel, p9))       //put pack into the channel send-buffer
        {
            free_pack(p9);
            p9 = NULL;
            assert("AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
        for(size_t len; (len = c_Communication_packs_into_bytes(&Communication_instance.channel, buff,  sizeof buff));)
            receive(buff, len, &TEST_Channel_instance.receiver);
    }
    return 0;
}

