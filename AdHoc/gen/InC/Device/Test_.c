
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
    p0_DevID_SET(60475U, p0);
    p0_Ver_SET(41303U, p0);
}

void check_DeviceVersion(p0_DeviceVersion * const p0)
{
    assert(p0_DevID_GET(p0) == 60475U);
    assert(p0_Ver_GET(p0) == 41303U);
}




void fill_BusConfiguration(p6_BusConfiguration * const p6)
{
    p6_multiplier_SET(252U, p6);
    p6_time_SET(16083U, p6);
    p6_clk_khz_SET(54928U, p6);
}

void check_BusConfiguration(p6_BusConfiguration * const p6)
{
    assert(p6_multiplier_GET(p6) == 252U);
    assert(p6_time_GET(p6) == 16083U);
    assert(p6_clk_khz_GET(p6) == 54928U);
}




void fill_InstructionsPack(p7_InstructionsPack * const p7)
{
    p7_Length_SET(93U, p7);
    {
        const uint8_t src[] = { 246U, 68U, 59U, 226U, 36U, 86U, 192U, 82U, 224U, 146U, 250U, 229U, 199U, 47U, 243U, 246U, 221U, 63U, 144U, 193U, 149U, 185U, 5U, 172U, 195U, 150U, 158U, 33U, 196U, 111U, 233U, 200U, 28U, 92U, 239U, 198U, 246U, 239U, 125U, 188U, 119U, 25U, 37U, 219U, 60U, 29U, 164U, 65U, 184U, 93U, 104U, 161U, 13U, 235U, 136U, 201U, 201U, 97U, 5U, 118U, 213U, 69U, 61U, 157U, 64U, 192U, 71U, 146U, 81U, 199U, 67U, 234U, 238U, 116U, 229U, 122U, 114U, 140U, 235U, 118U, 46U, 50U, 57U, 49U, 254U, 9U, 123U, 118U, 137U, 36U, 111U, 74U, 33U, 51U, 122U, 100U, 97U, 172U, 122U, 45U, 68U, 231U, 41U, 155U, 185U, 166U, 212U, 192U, 245U, 167U, 223U, 80U, 198U, 182U, 124U, 12U, 204U, 222U, 14U, 10U, 70U, 216U, 42U, 204U, 19U, 249U, 134U, 137U, 53U, 0U, 211U, 241U, 236U, 1U, 189U, 95U, 145U, 236U, 218U, 29U, 192U, 212U, 94U, 119U, 255U, 78U, 229U, 247U, 246U, 78U, 121U, 184U, 230U, 91U, 43U, 81U, 208U, 243U, 44U, 169U, 169U, 57U, 100U, 16U, 222U, 225U, 226U, 106U, 87U, 95U, 254U, 227U, 46U, 27U, 25U, 188U, 51U, 144U, 174U, 19U, 244U, 239U, 138U, 84U, 151U, 168U, 112U, 146U, 30U, 142U, 200U, 146U, 24U, 29U, 110U, 173U, 64U, 63U, 23U, 248U, 188U, 225U, 2U, 178U, 68U, 190U, 25U, 114U, 175U, 40U, 72U, 85U, 24U, 245U, 111U, 163U, 124U, 34U, 213U, 131U, 61U, 121U, 255U, 134U, 178U, 246U, 108U, 62U, 97U, 217U, 151U, 79U, 200U, 196U, 84U, 243U, 49U, 171U, 252U, 124U, 244U, 203U, 72U, 118U, 84U, 206U, 126U, 8U, 112U, 5U, 116U, 235U, 25U, 166U, 87U, 240U } ;
        p7_Instructions_SET(src, p7);
    }
}

void check_InstructionsPack(p7_InstructionsPack * const p7)
{
    assert(p7_Length_GET(p7) == 93U);
    {
        const uint8_t exemplary[] = { 246U, 68U, 59U, 226U, 36U, 86U, 192U, 82U, 224U, 146U, 250U, 229U, 199U, 47U, 243U, 246U, 221U, 63U, 144U, 193U, 149U, 185U, 5U, 172U, 195U, 150U, 158U, 33U, 196U, 111U, 233U, 200U, 28U, 92U, 239U, 198U, 246U, 239U, 125U, 188U, 119U, 25U, 37U, 219U, 60U, 29U, 164U, 65U, 184U, 93U, 104U, 161U, 13U, 235U, 136U, 201U, 201U, 97U, 5U, 118U, 213U, 69U, 61U, 157U, 64U, 192U, 71U, 146U, 81U, 199U, 67U, 234U, 238U, 116U, 229U, 122U, 114U, 140U, 235U, 118U, 46U, 50U, 57U, 49U, 254U, 9U, 123U, 118U, 137U, 36U, 111U, 74U, 33U, 51U, 122U, 100U, 97U, 172U, 122U, 45U, 68U, 231U, 41U, 155U, 185U, 166U, 212U, 192U, 245U, 167U, 223U, 80U, 198U, 182U, 124U, 12U, 204U, 222U, 14U, 10U, 70U, 216U, 42U, 204U, 19U, 249U, 134U, 137U, 53U, 0U, 211U, 241U, 236U, 1U, 189U, 95U, 145U, 236U, 218U, 29U, 192U, 212U, 94U, 119U, 255U, 78U, 229U, 247U, 246U, 78U, 121U, 184U, 230U, 91U, 43U, 81U, 208U, 243U, 44U, 169U, 169U, 57U, 100U, 16U, 222U, 225U, 226U, 106U, 87U, 95U, 254U, 227U, 46U, 27U, 25U, 188U, 51U, 144U, 174U, 19U, 244U, 239U, 138U, 84U, 151U, 168U, 112U, 146U, 30U, 142U, 200U, 146U, 24U, 29U, 110U, 173U, 64U, 63U, 23U, 248U, 188U, 225U, 2U, 178U, 68U, 190U, 25U, 114U, 175U, 40U, 72U, 85U, 24U, 245U, 111U, 163U, 124U, 34U, 213U, 131U, 61U, 121U, 255U, 134U, 178U, 246U, 108U, 62U, 97U, 217U, 151U, 79U, 200U, 196U, 84U, 243U, 49U, 171U, 252U, 124U, 244U, 203U, 72U, 118U, 84U, 206U, 126U, 8U, 112U, 5U, 116U, 235U, 25U, 166U, 87U, 240U }  ;
        V7_Instructions item_Instructions = p7_Instructions_GET(p7);
        assert(item_Instructions.len == 256);
        for(size_t index = 0; index < item_Instructions.len; index++)
            assert(v7_Instructions_GET(&item_Instructions, index) == exemplary[index]);
    }
}




void fill_DeviceError(p8_DeviceError * const p8)
{
    p8_param1_SET(51937U, p8);
    p8_error_id_SET(e_Errors_SensorsDataOverflow, p8);
}

void check_DeviceError(p8_DeviceError * const p8)
{
    assert(p8_param1_GET(p8) == 51937U);
    assert(p8_error_id_GET(p8) == e_Errors_SensorsDataOverflow);
}




void fill_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t src[] = { 6415U, 62862U, 25512U, 7150U, 6162U, 7929U, 12528U, 14551U, 33747U, 33015U, 22571U, 9964U, 30323U, 63793U, 22964U, 8152U, 15239U, 14834U, 42498U, 3200U, 23196U, 6863U, 98U, 30612U, 42416U, 42672U, 15733U, 52528U, 44565U, 10392U, 22585U, 44519U, 20180U, 1585U, 50619U, 37885U, 63405U, 2710U, 18194U, 51343U, 15139U, 61055U, 54131U, 27325U, 13172U, 56602U, 44590U, 61111U, 63239U, 50514U, 630U, 62075U, 28129U, 60332U, 53479U, 54468U, 62730U, 65181U, 36953U, 23527U, 34030U, 41981U, 40181U, 43466U, 26910U, 15361U, 13289U, 22621U, 46551U, 3790U, 32192U, 35599U, 38396U, 57522U, 58873U, 63503U, 29754U, 53999U, 63713U, 65516U, 45920U, 45506U, 20023U, 52769U, 61229U, 43185U, 63393U, 37553U, 7455U, 56768U, 13635U, 42795U, 23320U, 44139U, 2178U, 62877U, 3929U, 25117U, 14751U, 6063U, 29992U, 50430U, 51140U, 36207U, 21878U, 5217U, 36501U, 8143U, 34779U, 49511U, 64891U, 58826U, 57590U, 44813U, 10293U, 60794U, 62274U, 10147U, 34105U, 22096U, 10678U, 33710U, 58160U, 4491U, 42663U, 16742U, 43863U, 28663U, 11217U, 8667U, 26894U, 39175U, 50996U, 41937U, 31390U, 30161U, 51855U, 51179U, 44016U, 50866U, 24339U, 13645U, 44542U, 19300U, 42783U, 61048U, 10081U, 38487U, 11143U, 27101U, 23709U, 52930U, 57686U, 49099U, 22204U, 46253U, 35930U, 35607U, 62330U, 12849U, 24990U, 47211U, 47195U, 24657U, 57083U, 28435U, 65373U, 53254U, 24706U, 52643U, 39336U, 35900U, 37057U, 29458U, 57063U, 15904U, 57580U, 16494U, 36379U, 29598U, 51449U, 39751U, 5901U, 34721U, 62765U, 64483U, 41692U, 17171U, 2200U, 61712U, 31693U, 17197U, 24362U, 31261U, 3156U, 8579U, 57528U, 18939U, 49217U, 23967U, 8043U, 55215U, 27000U, 46727U, 4466U, 32828U, 4471U, 36830U, 51699U, 62578U, 51820U, 20103U, 31966U, 24958U, 4839U, 49554U, 1399U, 22206U, 8808U, 659U, 56337U, 37443U, 45376U, 35784U, 27017U, 47122U, 28398U, 3897U, 5575U, 36097U, 56378U, 2368U, 20392U, 1863U, 59715U, 380U, 37492U, 51920U, 19366U, 36093U, 12863U, 9381U, 61214U, 36196U, 26987U, 58256U, 61925U, 59164U, 42500U, 24187U, 7589U, 25716U, 28310U, 24598U, 26646U, 51372U, 38253U, 33504U, 35769U, 58543U, 5759U, 52732U, 17281U, 50820U, 23708U, 17914U, 22718U, 20514U, 3160U, 21773U, 44067U, 42128U, 29350U, 17054U, 37147U, 17803U, 36627U, 28113U, 7991U, 58548U, 39482U, 2570U, 24260U, 18648U, 51576U, 55686U, 20002U, 25747U, 37705U, 50900U, 921U, 42302U, 43604U, 64925U, 18708U, 11622U, 31034U, 52530U, 9251U, 662U, 34620U, 38195U, 22667U, 54453U, 1525U, 52591U, 39356U, 11762U, 28997U, 6356U, 10547U, 2504U, 24547U, 20941U, 26426U, 46512U, 14744U, 37897U, 31102U, 17677U, 62074U, 22552U, 5588U, 27834U, 37822U, 39257U, 21144U, 5100U, 7273U, 21216U, 5618U, 11469U, 63448U, 5957U, 37636U, 58434U, 18044U, 1806U, 56428U, 51066U, 3014U, 5813U, 445U, 62517U, 40594U, 60154U, 41505U, 32888U, 51208U, 51128U, 14419U, 31722U, 9087U, 39679U, 62250U, 38881U, 22755U, 54295U, 3991U, 55892U, 19916U, 43598U, 41282U, 50279U, 28626U, 59460U, 14865U, 59264U, 27009U, 28511U, 12787U, 6842U, 10045U, 48559U, 17310U, 2055U, 20836U, 53371U, 30511U, 45018U, 25811U, 65366U, 39363U, 16745U, 51748U, 46889U, 4785U, 55565U, 53512U, 33565U, 17606U, 48664U, 53608U, 21606U, 33226U, 28034U, 60650U, 41160U, 32032U, 59545U, 33322U, 55996U, 6326U, 51685U, 17926U, 5661U, 17384U, 15917U, 43756U, 60259U, 22612U, 62106U, 1139U, 45649U, 19145U, 25873U, 48029U, 6264U, 58285U, 61157U, 58650U, 44560U, 32645U, 50528U, 55342U, 19135U, 36982U, 64251U, 4791U, 39439U, 32546U, 51907U, 34284U, 48275U, 6059U, 61135U, 59574U, 40347U, 28753U, 64801U, 60801U, 38104U, 14387U, 64708U, 4826U, 8811U, 4932U, 11607U, 13611U, 44661U, 27454U, 49764U, 26013U, 50389U, 25721U, 23730U, 23069U, 42829U, 45061U, 60609U, 8110U, 25748U, 47980U, 48928U, 43561U, 50979U, 51605U, 16276U, 13470U, 17830U, 38185U, 23883U, 4576U, 38890U, 29511U, 21200U, 1267U, 37654U, 65045U, 62415U, 54365U, 55554U, 29068U, 49186U, 7681U, 41412U, 4867U, 3201U, 49U, 50751U, 24656U, 27123U, 10472U, 19757U, 27111U, 19652U, 28712U, 33433U, 16272U, 19235U, 33308U, 21605U, 36624U, 18U, 17696U, 53986U, 32646U, 48376U, 13218U, 6328U, 56332U, 33699U, 39429U, 45785U, 64147U, 14765U, 25047U, 62624U, 38084U, 9060U, 13035U, 14826U, 29403U, 32090U, 36740U, 55477U, 14928U, 43865U, 30116U, 23497U, 2211U, 43924U, 59530U, 28368U, 36652U, 63673U, 34610U, 13752U, 47166U, 17207U, 23859U, 26882U, 5882U, 39999U, 35799U, 13822U, 17120U, 17887U, 61991U, 64948U, 15206U, 20414U, 28482U, 713U, 57989U, 2142U, 24903U, 37961U, 37239U, 25870U, 38446U, 60934U, 24855U, 19087U, 60988U, 30070U, 4811U, 23059U, 17315U, 47003U, 2539U, 46306U, 19978U, 31671U, 13109U, 61530U, 52399U, 37189U, 18565U, 27067U, 31733U, 14420U, 19788U, 58679U, 44531U, 22457U, 54416U, 43919U, 752U, 41595U, 32079U, 56793U, 12962U, 24511U, 23753U, 61598U, 62055U, 53387U, 45835U, 49758U, 64013U, 5296U, 53015U, 36120U, 12442U, 10011U, 10883U, 27507U, 41620U, 35024U, 42782U, 44874U, 9657U, 52865U, 60000U, 55162U, 65328U, 38813U, 44610U, 48192U, 27428U, 56993U, 27762U, 65474U, 2565U, 50733U, 30822U, 37849U, 24757U, 48774U, 45610U, 23834U, 38345U, 54926U, 8126U, 61435U, 19487U, 21120U, 36877U, 15137U, 38610U, 57186U, 1543U, 60292U, 39355U, 2804U, 27671U, 42992U, 19243U, 16447U, 3809U, 63900U, 3640U, 11130U, 25879U, 198U, 19539U, 8876U, 15722U, 15059U, 36978U, 41510U, 22799U, 11868U, 47590U, 20150U, 49568U, 16787U, 17642U, 27862U, 36631U, 64595U, 61263U, 39655U, 64140U, 30715U, 63404U, 51459U, 27843U, 60180U, 44070U, 9493U, 8232U, 26134U, 12993U, 56404U, 26094U, 48468U, 15903U, 13716U, 26574U, 51511U, 494U, 39423U, 58443U, 1564U, 30744U, 59923U, 28943U, 47293U, 44285U, 40399U, 58804U, 24370U, 51768U, 28571U, 62269U, 46200U, 9968U, 47361U, 32971U, 9836U, 28843U, 54876U, 33226U, 18403U, 29861U, 53741U, 5643U, 7709U, 21446U, 35081U, 64324U, 24915U, 64040U, 21847U, 43882U, 22161U, 53998U, 58216U, 112U, 47737U, 23146U, 6578U, 52222U, 43527U, 62362U, 6395U, 65142U, 60049U, 53329U, 16332U, 60296U, 57482U, 51522U, 25056U, 36427U, 44994U, 63649U, 9497U, 5133U, 2297U, 45693U, 26861U, 29531U, 13753U, 54699U, 36472U, 20743U, 64003U, 60290U, 39132U, 19271U, 9953U, 65218U, 11760U, 52125U, 27559U, 37777U, 50888U, 54695U, 61162U, 4112U, 9036U, 63405U, 580U, 9632U, 32558U, 43508U, 53569U, 28062U, 44708U, 27486U, 44367U, 33193U, 24340U, 23925U, 54309U, 13961U, 8059U, 23317U, 59127U, 624U, 9107U, 52177U, 11170U, 58672U, 33354U, 56988U, 48019U, 57896U, 21034U, 11810U, 36683U, 3216U, 5676U, 61724U, 4192U, 50975U, 60897U, 27593U, 41034U, 41958U, 424U, 8415U, 16279U, 57468U, 14864U, 28181U, 54626U, 46856U, 27476U, 45815U, 55302U, 21938U, 30888U, 33979U, 654U, 64884U, 52453U, 56554U, 36932U, 55242U, 5292U, 27528U, 28564U, 39572U, 13029U, 41195U, 37853U, 61601U, 48793U, 48408U, 63096U, 8193U, 7580U, 2613U, 5397U, 50852U, 6583U, 3708U, 40750U, 3671U, 56903U, 15172U, 7854U, 21482U, 15539U, 59281U, 43561U, 35819U, 41980U, 65164U, 2836U, 21091U, 55708U, 27226U, 50075U, 45304U, 61222U, 34040U, 16003U, 2794U, 51742U, 50809U, 17314U, 566U, 47629U, 44501U, 42149U, 59487U, 25026U, 23950U, 696U, 26682U, 35884U, 28577U, 51645U, 58153U, 3279U, 7025U, 29825U, 59093U, 31046U, 34770U, 8048U, 51106U, 48576U, 57265U, 16978U, 54414U, 64107U, 45372U, 43429U, 56607U, 1563U, 47932U, 25634U, 19911U, 53298U, 50032U, 60987U, 38177U, 32269U, 40129U, 42684U, 65031U, 8828U, 55277U, 25465U, 37485U, 29593U, 39864U, 12980U, 22204U, 8993U, 65018U, 60604U, 61789U, 2735U, 39294U, 4566U, 16680U, 62200U, 8240U, 39174U, 52608U, 49577U, 18930U, 39002U, 51304U, 29779U, 38453U, 10353U, 6417U, 36666U, 2036U, 52530U, 56673U, 48639U, 16459U, 36561U, 56297U, 2370U, 31998U, 54071U, 36250U, 45840U, 2620U, 49255U, 58124U, 5287U, 21042U, 44296U, 35381U, 50719U, 46403U, 48707U, 33728U, 34987U, 24850U, 13375U, 44651U, 57183U, 20757U, 54119U, 13969U, 55514U, 55132U, 62222U, 29139U, 14736U, 29658U, 25996U, 6701U, 49228U, 1774U, 19736U, 15147U, 65129U, 3516U, 58783U, 1805U, 60775U, 27317U, 36492U, 65065U, 53883U, 60710U, 33354U } ;
        p9_values_SET(src, p9);
    }
}

void check_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t exemplary[] = { 6415U, 62862U, 25512U, 7150U, 6162U, 7929U, 12528U, 14551U, 33747U, 33015U, 22571U, 9964U, 30323U, 63793U, 22964U, 8152U, 15239U, 14834U, 42498U, 3200U, 23196U, 6863U, 98U, 30612U, 42416U, 42672U, 15733U, 52528U, 44565U, 10392U, 22585U, 44519U, 20180U, 1585U, 50619U, 37885U, 63405U, 2710U, 18194U, 51343U, 15139U, 61055U, 54131U, 27325U, 13172U, 56602U, 44590U, 61111U, 63239U, 50514U, 630U, 62075U, 28129U, 60332U, 53479U, 54468U, 62730U, 65181U, 36953U, 23527U, 34030U, 41981U, 40181U, 43466U, 26910U, 15361U, 13289U, 22621U, 46551U, 3790U, 32192U, 35599U, 38396U, 57522U, 58873U, 63503U, 29754U, 53999U, 63713U, 65516U, 45920U, 45506U, 20023U, 52769U, 61229U, 43185U, 63393U, 37553U, 7455U, 56768U, 13635U, 42795U, 23320U, 44139U, 2178U, 62877U, 3929U, 25117U, 14751U, 6063U, 29992U, 50430U, 51140U, 36207U, 21878U, 5217U, 36501U, 8143U, 34779U, 49511U, 64891U, 58826U, 57590U, 44813U, 10293U, 60794U, 62274U, 10147U, 34105U, 22096U, 10678U, 33710U, 58160U, 4491U, 42663U, 16742U, 43863U, 28663U, 11217U, 8667U, 26894U, 39175U, 50996U, 41937U, 31390U, 30161U, 51855U, 51179U, 44016U, 50866U, 24339U, 13645U, 44542U, 19300U, 42783U, 61048U, 10081U, 38487U, 11143U, 27101U, 23709U, 52930U, 57686U, 49099U, 22204U, 46253U, 35930U, 35607U, 62330U, 12849U, 24990U, 47211U, 47195U, 24657U, 57083U, 28435U, 65373U, 53254U, 24706U, 52643U, 39336U, 35900U, 37057U, 29458U, 57063U, 15904U, 57580U, 16494U, 36379U, 29598U, 51449U, 39751U, 5901U, 34721U, 62765U, 64483U, 41692U, 17171U, 2200U, 61712U, 31693U, 17197U, 24362U, 31261U, 3156U, 8579U, 57528U, 18939U, 49217U, 23967U, 8043U, 55215U, 27000U, 46727U, 4466U, 32828U, 4471U, 36830U, 51699U, 62578U, 51820U, 20103U, 31966U, 24958U, 4839U, 49554U, 1399U, 22206U, 8808U, 659U, 56337U, 37443U, 45376U, 35784U, 27017U, 47122U, 28398U, 3897U, 5575U, 36097U, 56378U, 2368U, 20392U, 1863U, 59715U, 380U, 37492U, 51920U, 19366U, 36093U, 12863U, 9381U, 61214U, 36196U, 26987U, 58256U, 61925U, 59164U, 42500U, 24187U, 7589U, 25716U, 28310U, 24598U, 26646U, 51372U, 38253U, 33504U, 35769U, 58543U, 5759U, 52732U, 17281U, 50820U, 23708U, 17914U, 22718U, 20514U, 3160U, 21773U, 44067U, 42128U, 29350U, 17054U, 37147U, 17803U, 36627U, 28113U, 7991U, 58548U, 39482U, 2570U, 24260U, 18648U, 51576U, 55686U, 20002U, 25747U, 37705U, 50900U, 921U, 42302U, 43604U, 64925U, 18708U, 11622U, 31034U, 52530U, 9251U, 662U, 34620U, 38195U, 22667U, 54453U, 1525U, 52591U, 39356U, 11762U, 28997U, 6356U, 10547U, 2504U, 24547U, 20941U, 26426U, 46512U, 14744U, 37897U, 31102U, 17677U, 62074U, 22552U, 5588U, 27834U, 37822U, 39257U, 21144U, 5100U, 7273U, 21216U, 5618U, 11469U, 63448U, 5957U, 37636U, 58434U, 18044U, 1806U, 56428U, 51066U, 3014U, 5813U, 445U, 62517U, 40594U, 60154U, 41505U, 32888U, 51208U, 51128U, 14419U, 31722U, 9087U, 39679U, 62250U, 38881U, 22755U, 54295U, 3991U, 55892U, 19916U, 43598U, 41282U, 50279U, 28626U, 59460U, 14865U, 59264U, 27009U, 28511U, 12787U, 6842U, 10045U, 48559U, 17310U, 2055U, 20836U, 53371U, 30511U, 45018U, 25811U, 65366U, 39363U, 16745U, 51748U, 46889U, 4785U, 55565U, 53512U, 33565U, 17606U, 48664U, 53608U, 21606U, 33226U, 28034U, 60650U, 41160U, 32032U, 59545U, 33322U, 55996U, 6326U, 51685U, 17926U, 5661U, 17384U, 15917U, 43756U, 60259U, 22612U, 62106U, 1139U, 45649U, 19145U, 25873U, 48029U, 6264U, 58285U, 61157U, 58650U, 44560U, 32645U, 50528U, 55342U, 19135U, 36982U, 64251U, 4791U, 39439U, 32546U, 51907U, 34284U, 48275U, 6059U, 61135U, 59574U, 40347U, 28753U, 64801U, 60801U, 38104U, 14387U, 64708U, 4826U, 8811U, 4932U, 11607U, 13611U, 44661U, 27454U, 49764U, 26013U, 50389U, 25721U, 23730U, 23069U, 42829U, 45061U, 60609U, 8110U, 25748U, 47980U, 48928U, 43561U, 50979U, 51605U, 16276U, 13470U, 17830U, 38185U, 23883U, 4576U, 38890U, 29511U, 21200U, 1267U, 37654U, 65045U, 62415U, 54365U, 55554U, 29068U, 49186U, 7681U, 41412U, 4867U, 3201U, 49U, 50751U, 24656U, 27123U, 10472U, 19757U, 27111U, 19652U, 28712U, 33433U, 16272U, 19235U, 33308U, 21605U, 36624U, 18U, 17696U, 53986U, 32646U, 48376U, 13218U, 6328U, 56332U, 33699U, 39429U, 45785U, 64147U, 14765U, 25047U, 62624U, 38084U, 9060U, 13035U, 14826U, 29403U, 32090U, 36740U, 55477U, 14928U, 43865U, 30116U, 23497U, 2211U, 43924U, 59530U, 28368U, 36652U, 63673U, 34610U, 13752U, 47166U, 17207U, 23859U, 26882U, 5882U, 39999U, 35799U, 13822U, 17120U, 17887U, 61991U, 64948U, 15206U, 20414U, 28482U, 713U, 57989U, 2142U, 24903U, 37961U, 37239U, 25870U, 38446U, 60934U, 24855U, 19087U, 60988U, 30070U, 4811U, 23059U, 17315U, 47003U, 2539U, 46306U, 19978U, 31671U, 13109U, 61530U, 52399U, 37189U, 18565U, 27067U, 31733U, 14420U, 19788U, 58679U, 44531U, 22457U, 54416U, 43919U, 752U, 41595U, 32079U, 56793U, 12962U, 24511U, 23753U, 61598U, 62055U, 53387U, 45835U, 49758U, 64013U, 5296U, 53015U, 36120U, 12442U, 10011U, 10883U, 27507U, 41620U, 35024U, 42782U, 44874U, 9657U, 52865U, 60000U, 55162U, 65328U, 38813U, 44610U, 48192U, 27428U, 56993U, 27762U, 65474U, 2565U, 50733U, 30822U, 37849U, 24757U, 48774U, 45610U, 23834U, 38345U, 54926U, 8126U, 61435U, 19487U, 21120U, 36877U, 15137U, 38610U, 57186U, 1543U, 60292U, 39355U, 2804U, 27671U, 42992U, 19243U, 16447U, 3809U, 63900U, 3640U, 11130U, 25879U, 198U, 19539U, 8876U, 15722U, 15059U, 36978U, 41510U, 22799U, 11868U, 47590U, 20150U, 49568U, 16787U, 17642U, 27862U, 36631U, 64595U, 61263U, 39655U, 64140U, 30715U, 63404U, 51459U, 27843U, 60180U, 44070U, 9493U, 8232U, 26134U, 12993U, 56404U, 26094U, 48468U, 15903U, 13716U, 26574U, 51511U, 494U, 39423U, 58443U, 1564U, 30744U, 59923U, 28943U, 47293U, 44285U, 40399U, 58804U, 24370U, 51768U, 28571U, 62269U, 46200U, 9968U, 47361U, 32971U, 9836U, 28843U, 54876U, 33226U, 18403U, 29861U, 53741U, 5643U, 7709U, 21446U, 35081U, 64324U, 24915U, 64040U, 21847U, 43882U, 22161U, 53998U, 58216U, 112U, 47737U, 23146U, 6578U, 52222U, 43527U, 62362U, 6395U, 65142U, 60049U, 53329U, 16332U, 60296U, 57482U, 51522U, 25056U, 36427U, 44994U, 63649U, 9497U, 5133U, 2297U, 45693U, 26861U, 29531U, 13753U, 54699U, 36472U, 20743U, 64003U, 60290U, 39132U, 19271U, 9953U, 65218U, 11760U, 52125U, 27559U, 37777U, 50888U, 54695U, 61162U, 4112U, 9036U, 63405U, 580U, 9632U, 32558U, 43508U, 53569U, 28062U, 44708U, 27486U, 44367U, 33193U, 24340U, 23925U, 54309U, 13961U, 8059U, 23317U, 59127U, 624U, 9107U, 52177U, 11170U, 58672U, 33354U, 56988U, 48019U, 57896U, 21034U, 11810U, 36683U, 3216U, 5676U, 61724U, 4192U, 50975U, 60897U, 27593U, 41034U, 41958U, 424U, 8415U, 16279U, 57468U, 14864U, 28181U, 54626U, 46856U, 27476U, 45815U, 55302U, 21938U, 30888U, 33979U, 654U, 64884U, 52453U, 56554U, 36932U, 55242U, 5292U, 27528U, 28564U, 39572U, 13029U, 41195U, 37853U, 61601U, 48793U, 48408U, 63096U, 8193U, 7580U, 2613U, 5397U, 50852U, 6583U, 3708U, 40750U, 3671U, 56903U, 15172U, 7854U, 21482U, 15539U, 59281U, 43561U, 35819U, 41980U, 65164U, 2836U, 21091U, 55708U, 27226U, 50075U, 45304U, 61222U, 34040U, 16003U, 2794U, 51742U, 50809U, 17314U, 566U, 47629U, 44501U, 42149U, 59487U, 25026U, 23950U, 696U, 26682U, 35884U, 28577U, 51645U, 58153U, 3279U, 7025U, 29825U, 59093U, 31046U, 34770U, 8048U, 51106U, 48576U, 57265U, 16978U, 54414U, 64107U, 45372U, 43429U, 56607U, 1563U, 47932U, 25634U, 19911U, 53298U, 50032U, 60987U, 38177U, 32269U, 40129U, 42684U, 65031U, 8828U, 55277U, 25465U, 37485U, 29593U, 39864U, 12980U, 22204U, 8993U, 65018U, 60604U, 61789U, 2735U, 39294U, 4566U, 16680U, 62200U, 8240U, 39174U, 52608U, 49577U, 18930U, 39002U, 51304U, 29779U, 38453U, 10353U, 6417U, 36666U, 2036U, 52530U, 56673U, 48639U, 16459U, 36561U, 56297U, 2370U, 31998U, 54071U, 36250U, 45840U, 2620U, 49255U, 58124U, 5287U, 21042U, 44296U, 35381U, 50719U, 46403U, 48707U, 33728U, 34987U, 24850U, 13375U, 44651U, 57183U, 20757U, 54119U, 13969U, 55514U, 55132U, 62222U, 29139U, 14736U, 29658U, 25996U, 6701U, 49228U, 1774U, 19736U, 15147U, 65129U, 3516U, 58783U, 1805U, 60775U, 27317U, 36492U, 65065U, 53883U, 60710U, 33354U }  ;
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
}

