
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
    p0_DevID_SET(63816U, p0);
    p0_Ver_SET(47408U, p0);
}

void check_DeviceVersion(p0_DeviceVersion * const p0)
{
    assert(p0_DevID_GET(p0) == 63816U);
    assert(p0_Ver_GET(p0) == 47408U);
}




void fill_BusConfiguration(p6_BusConfiguration * const p6)
{
    p6_multiplier_SET(134U, p6);
    p6_time_SET(28197U, p6);
    p6_clk_khz_SET(24167U, p6);
}

void check_BusConfiguration(p6_BusConfiguration * const p6)
{
    assert(p6_multiplier_GET(p6) == 134U);
    assert(p6_time_GET(p6) == 28197U);
    assert(p6_clk_khz_GET(p6) == 24167U);
}




void fill_InstructionsPack(p7_InstructionsPack * const p7)
{
    p7_Length_SET(201U, p7);
    {
        const uint8_t src[] = { 112U, 161U, 246U, 129U, 159U, 108U, 193U, 218U, 207U, 253U, 220U, 58U, 58U, 169U, 36U, 243U, 114U, 141U, 32U, 43U, 177U, 254U, 13U, 138U, 134U, 225U, 126U, 160U, 3U, 244U, 230U, 125U, 92U, 215U, 166U, 228U, 139U, 136U, 158U, 192U, 36U, 8U, 25U, 182U, 119U, 146U, 184U, 130U, 113U, 22U, 26U, 106U, 85U, 148U, 139U, 49U, 152U, 12U, 121U, 182U, 221U, 12U, 106U, 101U, 10U, 134U, 246U, 189U, 17U, 9U, 179U, 107U, 128U, 171U, 87U, 40U, 21U, 33U, 79U, 38U, 97U, 206U, 35U, 15U, 204U, 202U, 59U, 244U, 77U, 15U, 249U, 33U, 109U, 213U, 118U, 82U, 124U, 202U, 44U, 127U, 71U, 127U, 254U, 82U, 228U, 114U, 79U, 147U, 129U, 175U, 1U, 130U, 149U, 14U, 180U, 240U, 140U, 53U, 203U, 61U, 70U, 172U, 100U, 130U, 24U, 146U, 187U, 32U, 46U, 99U, 154U, 187U, 8U, 188U, 40U, 186U, 6U, 182U, 54U, 183U, 101U, 143U, 220U, 216U, 162U, 41U, 16U, 130U, 89U, 139U, 10U, 120U, 234U, 205U, 186U, 149U, 250U, 75U, 25U, 10U, 142U, 94U, 6U, 154U, 52U, 240U, 28U, 172U, 226U, 168U, 81U, 164U, 178U, 171U, 220U, 197U, 150U, 168U, 9U, 203U, 158U, 212U, 107U, 79U, 211U, 86U, 80U, 209U, 238U, 170U, 169U, 212U, 161U, 174U, 174U, 52U, 3U, 147U, 52U, 144U, 72U, 38U, 188U, 123U, 39U, 225U, 110U, 109U, 148U, 102U, 175U, 61U, 11U, 37U, 137U, 28U, 72U, 203U, 208U, 225U, 22U, 209U, 40U, 190U, 179U, 150U, 208U, 3U, 136U, 137U, 221U, 124U, 31U, 4U, 122U, 232U, 159U, 85U, 13U, 47U, 134U, 24U, 110U, 125U, 25U, 142U, 161U, 254U, 125U, 242U, 30U, 180U, 16U, 81U, 54U, 229U } ;
        p7_Instructions_SET(src, p7);
    }
}

void check_InstructionsPack(p7_InstructionsPack * const p7)
{
    assert(p7_Length_GET(p7) == 201U);
    {
        const uint8_t exemplary[] = { 112U, 161U, 246U, 129U, 159U, 108U, 193U, 218U, 207U, 253U, 220U, 58U, 58U, 169U, 36U, 243U, 114U, 141U, 32U, 43U, 177U, 254U, 13U, 138U, 134U, 225U, 126U, 160U, 3U, 244U, 230U, 125U, 92U, 215U, 166U, 228U, 139U, 136U, 158U, 192U, 36U, 8U, 25U, 182U, 119U, 146U, 184U, 130U, 113U, 22U, 26U, 106U, 85U, 148U, 139U, 49U, 152U, 12U, 121U, 182U, 221U, 12U, 106U, 101U, 10U, 134U, 246U, 189U, 17U, 9U, 179U, 107U, 128U, 171U, 87U, 40U, 21U, 33U, 79U, 38U, 97U, 206U, 35U, 15U, 204U, 202U, 59U, 244U, 77U, 15U, 249U, 33U, 109U, 213U, 118U, 82U, 124U, 202U, 44U, 127U, 71U, 127U, 254U, 82U, 228U, 114U, 79U, 147U, 129U, 175U, 1U, 130U, 149U, 14U, 180U, 240U, 140U, 53U, 203U, 61U, 70U, 172U, 100U, 130U, 24U, 146U, 187U, 32U, 46U, 99U, 154U, 187U, 8U, 188U, 40U, 186U, 6U, 182U, 54U, 183U, 101U, 143U, 220U, 216U, 162U, 41U, 16U, 130U, 89U, 139U, 10U, 120U, 234U, 205U, 186U, 149U, 250U, 75U, 25U, 10U, 142U, 94U, 6U, 154U, 52U, 240U, 28U, 172U, 226U, 168U, 81U, 164U, 178U, 171U, 220U, 197U, 150U, 168U, 9U, 203U, 158U, 212U, 107U, 79U, 211U, 86U, 80U, 209U, 238U, 170U, 169U, 212U, 161U, 174U, 174U, 52U, 3U, 147U, 52U, 144U, 72U, 38U, 188U, 123U, 39U, 225U, 110U, 109U, 148U, 102U, 175U, 61U, 11U, 37U, 137U, 28U, 72U, 203U, 208U, 225U, 22U, 209U, 40U, 190U, 179U, 150U, 208U, 3U, 136U, 137U, 221U, 124U, 31U, 4U, 122U, 232U, 159U, 85U, 13U, 47U, 134U, 24U, 110U, 125U, 25U, 142U, 161U, 254U, 125U, 242U, 30U, 180U, 16U, 81U, 54U, 229U }  ;
        V7_Instructions item_Instructions = p7_Instructions_GET(p7);
        assert(item_Instructions.len == 256);
        for(size_t index = 0; index < item_Instructions.len; index++)
            assert(v7_Instructions_GET(&item_Instructions, index) == exemplary[index]);
    }
}




void fill_DeviceError(p8_DeviceError * const p8)
{
    p8_param1_SET(38836U, p8);
    p8_error_id_SET(e_Errors_SensorsDataOverflow, p8);
}

void check_DeviceError(p8_DeviceError * const p8)
{
    assert(p8_param1_GET(p8) == 38836U);
    assert(p8_error_id_GET(p8) == e_Errors_SensorsDataOverflow);
}




void fill_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t src[] = { 51659U, 56213U, 16465U, 16423U, 28482U, 47483U, 22473U, 28312U, 42934U, 13625U, 64021U, 16110U, 8870U, 43124U, 50850U, 60153U, 18515U, 10220U, 60954U, 11097U, 47939U, 2065U, 50914U, 45657U, 59559U, 35282U, 30481U, 46002U, 25098U, 54815U, 41850U, 34921U, 40465U, 16470U, 20121U, 42556U, 13309U, 50687U, 9526U, 10801U, 795U, 35837U, 7973U, 26657U, 39815U, 2392U, 7638U, 2529U, 25226U, 20458U, 23861U, 48258U, 19522U, 38941U, 64285U, 2895U, 30048U, 9020U, 15458U, 43321U, 47403U, 37878U, 16638U, 61112U, 42004U, 13553U, 11440U, 16655U, 57351U, 6476U, 2953U, 41274U, 25425U, 1501U, 39419U, 25164U, 11950U, 19868U, 13190U, 65314U, 25349U, 64108U, 15342U, 34413U, 43033U, 64169U, 190U, 9779U, 52284U, 44354U, 53005U, 58277U, 24944U, 46423U, 20116U, 54828U, 24239U, 59327U, 5707U, 8740U, 26898U, 35729U, 52724U, 15079U, 64646U, 59984U, 17863U, 18142U, 19481U, 16956U, 12283U, 9963U, 5465U, 11768U, 22972U, 39696U, 62194U, 15365U, 28177U, 30140U, 13390U, 46742U, 15047U, 27852U, 38783U, 52081U, 36662U, 8844U, 63839U, 15338U, 2886U, 47043U, 17470U, 53074U, 44126U, 15001U, 44130U, 47748U, 10222U, 54699U, 30784U, 9829U, 32720U, 48953U, 22500U, 61857U, 45302U, 31900U, 24213U, 37626U, 60550U, 16899U, 55262U, 22028U, 39150U, 5497U, 13076U, 37773U, 55601U, 58492U, 50139U, 13085U, 34727U, 8377U, 17838U, 42128U, 59793U, 19192U, 40169U, 14384U, 21899U, 4530U, 3553U, 62924U, 33849U, 43540U, 8075U, 24790U, 8681U, 40137U, 16819U, 17965U, 19156U, 786U, 1079U, 17183U, 33269U, 10344U, 51990U, 27041U, 58315U, 5601U, 25903U, 12738U, 2777U, 52925U, 61352U, 28116U, 34943U, 36842U, 41432U, 16908U, 13534U, 24454U, 58958U, 62836U, 42005U, 7943U, 30938U, 19563U, 26242U, 36066U, 54584U, 48648U, 2808U, 43170U, 12178U, 57828U, 38092U, 18380U, 49931U, 4322U, 11835U, 11944U, 57847U, 41638U, 16008U, 37275U, 13862U, 32823U, 38412U, 14989U, 6951U, 49775U, 60365U, 35278U, 43251U, 22038U, 51452U, 41686U, 46545U, 41765U, 32568U, 34399U, 19076U, 39819U, 46157U, 8120U, 42333U, 63612U, 41083U, 10419U, 14105U, 187U, 57089U, 27893U, 50347U, 54591U, 50794U, 60566U, 34088U, 22588U, 50197U, 47561U, 58900U, 22797U, 15127U, 19198U, 54514U, 48075U, 15616U, 52552U, 3358U, 14810U, 21915U, 22772U, 13058U, 27431U, 30988U, 7285U, 27120U, 44118U, 6713U, 13167U, 13489U, 41579U, 9814U, 1305U, 5863U, 36897U, 49756U, 63351U, 33586U, 35692U, 65282U, 35561U, 34350U, 44116U, 15087U, 29662U, 43371U, 23199U, 33455U, 62617U, 51233U, 59497U, 12860U, 57699U, 64695U, 10490U, 46621U, 61041U, 2736U, 11671U, 25749U, 60172U, 34235U, 3445U, 55761U, 9764U, 16849U, 35650U, 34807U, 18197U, 6885U, 2003U, 57136U, 56989U, 50872U, 40338U, 10792U, 38417U, 47751U, 39246U, 15032U, 17685U, 1823U, 1404U, 23720U, 49986U, 31228U, 24760U, 34509U, 59263U, 42497U, 31796U, 39629U, 14879U, 11531U, 20312U, 61119U, 35595U, 8607U, 5483U, 13252U, 52959U, 6708U, 36198U, 60159U, 27284U, 58273U, 60510U, 48183U, 13861U, 56668U, 35355U, 24629U, 6309U, 19754U, 59155U, 19398U, 1251U, 54726U, 22083U, 38556U, 12548U, 58193U, 35644U, 20065U, 60311U, 4755U, 65390U, 60106U, 61574U, 43017U, 57990U, 20093U, 23583U, 40823U, 20380U, 50464U, 20005U, 45290U, 18959U, 7326U, 27595U, 25623U, 50145U, 23159U, 13970U, 35994U, 39292U, 55169U, 60628U, 12882U, 33466U, 43762U, 62548U, 64940U, 49589U, 12306U, 47771U, 2987U, 26225U, 33198U, 2488U, 64087U, 56033U, 44854U, 33383U, 30312U, 36050U, 52680U, 22742U, 5510U, 51720U, 65214U, 56689U, 16011U, 16748U, 12804U, 22806U, 42320U, 19914U, 26756U, 25470U, 26049U, 49606U, 58401U, 1211U, 15629U, 30555U, 41811U, 31227U, 48008U, 34083U, 10577U, 46039U, 16445U, 29995U, 33795U, 31657U, 1885U, 20790U, 41454U, 52879U, 18331U, 10438U, 29287U, 13517U, 12919U, 52650U, 24078U, 45465U, 1365U, 43740U, 14484U, 7191U, 35641U, 59747U, 33766U, 56624U, 47273U, 4650U, 29675U, 34517U, 2762U, 16602U, 40220U, 55467U, 56503U, 29510U, 13784U, 27442U, 7141U, 18960U, 43556U, 44140U, 38036U, 43156U, 33891U, 7854U, 57848U, 24795U, 24542U, 43417U, 50657U, 51244U, 56774U, 5868U, 61408U, 54026U, 37565U, 30592U, 5783U, 36257U, 7725U, 4081U, 23368U, 37845U, 47962U, 47896U, 4056U, 63040U, 17449U, 8026U, 43121U, 14017U, 41265U, 25441U, 50601U, 9686U, 32406U, 1130U, 16026U, 35260U, 36755U, 21394U, 20772U, 29554U, 62381U, 15327U, 38248U, 6880U, 59113U, 32702U, 8235U, 31839U, 26569U, 41272U, 18202U, 64794U, 40862U, 8852U, 64398U, 36860U, 59925U, 47020U, 60558U, 64524U, 59956U, 39028U, 3555U, 278U, 59132U, 15201U, 45575U, 53809U, 37562U, 33474U, 11507U, 54741U, 7995U, 39151U, 61253U, 11242U, 36216U, 16647U, 59026U, 7573U, 10615U, 42851U, 37884U, 55610U, 3201U, 22021U, 60023U, 4619U, 3350U, 20866U, 2605U, 5539U, 20962U, 33553U, 23176U, 65376U, 52965U, 59914U, 3300U, 32623U, 738U, 20620U, 40493U, 40477U, 48635U, 32827U, 12271U, 43384U, 40039U, 49411U, 34185U, 58056U, 3883U, 5507U, 46936U, 57341U, 22472U, 11824U, 47530U, 5909U, 33035U, 17410U, 11427U, 37768U, 65418U, 24605U, 32712U, 10262U, 39297U, 3264U, 21396U, 7262U, 36116U, 60698U, 29435U, 47168U, 60953U, 60272U, 48723U, 63874U, 18936U, 60526U, 24988U, 21225U, 37198U, 43101U, 18758U, 7760U, 46255U, 39254U, 49862U, 24098U, 44943U, 60694U, 29573U, 13146U, 24821U, 38511U, 58891U, 19663U, 29856U, 11050U, 36357U, 12150U, 14843U, 5862U, 5801U, 31101U, 59156U, 22456U, 37231U, 44827U, 6701U, 29767U, 20512U, 24766U, 64624U, 50507U, 53972U, 65094U, 37930U, 37010U, 32275U, 8042U, 34198U, 40632U, 4175U, 22978U, 18085U, 9936U, 42826U, 47337U, 5728U, 22276U, 12581U, 65398U, 43101U, 3420U, 34431U, 25493U, 9520U, 64612U, 18272U, 36531U, 27857U, 58118U, 20609U, 29520U, 17209U, 16955U, 8621U, 56508U, 51470U, 12085U, 16662U, 6730U, 39436U, 12938U, 46675U, 15905U, 4727U, 40263U, 1151U, 64534U, 62541U, 40754U, 45213U, 54898U, 40968U, 41733U, 8630U, 8823U, 58122U, 22814U, 63204U, 57390U, 9997U, 60267U, 43607U, 12030U, 11615U, 15659U, 53326U, 14531U, 15600U, 25289U, 62451U, 44382U, 16109U, 15801U, 12153U, 41487U, 31921U, 62982U, 4035U, 61256U, 43538U, 28687U, 20597U, 23929U, 13435U, 31271U, 13772U, 21483U, 24193U, 56373U, 63313U, 40035U, 32422U, 28057U, 184U, 49524U, 2944U, 21549U, 31420U, 25264U, 58994U, 49101U, 52852U, 4607U, 61530U, 49647U, 18531U, 5106U, 27195U, 50861U, 9456U, 26205U, 15182U, 21580U, 61011U, 23668U, 38695U, 8186U, 37042U, 19698U, 16349U, 35609U, 21757U, 25270U, 54450U, 9250U, 37845U, 29169U, 44497U, 55653U, 12538U, 36064U, 55646U, 3420U, 62152U, 41061U, 19418U, 50621U, 2843U, 7938U, 27230U, 63123U, 7650U, 20005U, 31550U, 30368U, 43924U, 5011U, 11597U, 44642U, 39451U, 43504U, 9394U, 20914U, 17923U, 55987U, 22426U, 5520U, 61756U, 57221U, 17830U, 57331U, 47620U, 60750U, 23166U, 16252U, 13942U, 29288U, 50481U, 39776U, 27008U, 13315U, 2713U, 52349U, 15497U, 15006U, 62499U, 7216U, 60348U, 2299U, 33912U, 18473U, 20130U, 16414U, 14560U, 34105U, 45787U, 10383U, 5798U, 20604U, 29195U, 65230U, 36423U, 13469U, 23259U, 56116U, 23536U, 57850U, 14429U, 43636U, 46613U, 44714U, 50811U, 41190U, 47043U, 45841U, 9433U, 27959U, 58640U, 16677U, 18994U, 50826U, 3814U, 31030U, 10512U, 34562U, 25809U, 35375U, 40851U, 50680U, 58451U, 860U, 10014U, 230U, 23527U, 38370U, 30241U, 31933U, 7729U, 44086U, 7547U, 22320U, 11367U, 64795U, 57344U, 63656U, 42362U, 12863U, 16725U, 55630U, 61568U, 16363U, 20434U, 52441U, 62520U, 15063U, 36266U, 794U, 16299U, 12981U, 52277U, 17846U, 6318U, 22992U, 62264U, 45157U, 21325U, 52249U, 24190U, 41796U, 61923U, 63776U, 60402U, 25024U, 18973U, 41955U, 304U, 45713U, 54382U, 11487U, 22741U, 35713U, 48821U, 35635U, 42049U, 2850U, 21427U, 41762U, 62809U, 25049U, 33082U, 49649U, 48460U, 11171U, 16077U, 9557U, 13455U, 18000U, 53455U, 13714U, 39069U, 40122U, 34198U, 26512U, 38972U, 26893U, 43582U, 25918U, 18870U, 58669U, 55876U, 56697U, 38864U, 7317U, 22719U, 46402U, 24475U, 28226U, 9885U, 27484U, 52561U, 65468U, 22979U, 8750U, 51258U, 49023U, 50225U, 51600U, 10125U, 16642U, 55621U, 33336U, 29961U, 22311U, 17304U, 33114U, 48069U, 26187U, 4910U, 24597U, 12817U, 38696U, 47961U, 49658U } ;
        p9_values_SET(src, p9);
    }
}

void check_SensorsData(p9_SensorsData * const p9)
{
    {
        const uint16_t exemplary[] = { 51659U, 56213U, 16465U, 16423U, 28482U, 47483U, 22473U, 28312U, 42934U, 13625U, 64021U, 16110U, 8870U, 43124U, 50850U, 60153U, 18515U, 10220U, 60954U, 11097U, 47939U, 2065U, 50914U, 45657U, 59559U, 35282U, 30481U, 46002U, 25098U, 54815U, 41850U, 34921U, 40465U, 16470U, 20121U, 42556U, 13309U, 50687U, 9526U, 10801U, 795U, 35837U, 7973U, 26657U, 39815U, 2392U, 7638U, 2529U, 25226U, 20458U, 23861U, 48258U, 19522U, 38941U, 64285U, 2895U, 30048U, 9020U, 15458U, 43321U, 47403U, 37878U, 16638U, 61112U, 42004U, 13553U, 11440U, 16655U, 57351U, 6476U, 2953U, 41274U, 25425U, 1501U, 39419U, 25164U, 11950U, 19868U, 13190U, 65314U, 25349U, 64108U, 15342U, 34413U, 43033U, 64169U, 190U, 9779U, 52284U, 44354U, 53005U, 58277U, 24944U, 46423U, 20116U, 54828U, 24239U, 59327U, 5707U, 8740U, 26898U, 35729U, 52724U, 15079U, 64646U, 59984U, 17863U, 18142U, 19481U, 16956U, 12283U, 9963U, 5465U, 11768U, 22972U, 39696U, 62194U, 15365U, 28177U, 30140U, 13390U, 46742U, 15047U, 27852U, 38783U, 52081U, 36662U, 8844U, 63839U, 15338U, 2886U, 47043U, 17470U, 53074U, 44126U, 15001U, 44130U, 47748U, 10222U, 54699U, 30784U, 9829U, 32720U, 48953U, 22500U, 61857U, 45302U, 31900U, 24213U, 37626U, 60550U, 16899U, 55262U, 22028U, 39150U, 5497U, 13076U, 37773U, 55601U, 58492U, 50139U, 13085U, 34727U, 8377U, 17838U, 42128U, 59793U, 19192U, 40169U, 14384U, 21899U, 4530U, 3553U, 62924U, 33849U, 43540U, 8075U, 24790U, 8681U, 40137U, 16819U, 17965U, 19156U, 786U, 1079U, 17183U, 33269U, 10344U, 51990U, 27041U, 58315U, 5601U, 25903U, 12738U, 2777U, 52925U, 61352U, 28116U, 34943U, 36842U, 41432U, 16908U, 13534U, 24454U, 58958U, 62836U, 42005U, 7943U, 30938U, 19563U, 26242U, 36066U, 54584U, 48648U, 2808U, 43170U, 12178U, 57828U, 38092U, 18380U, 49931U, 4322U, 11835U, 11944U, 57847U, 41638U, 16008U, 37275U, 13862U, 32823U, 38412U, 14989U, 6951U, 49775U, 60365U, 35278U, 43251U, 22038U, 51452U, 41686U, 46545U, 41765U, 32568U, 34399U, 19076U, 39819U, 46157U, 8120U, 42333U, 63612U, 41083U, 10419U, 14105U, 187U, 57089U, 27893U, 50347U, 54591U, 50794U, 60566U, 34088U, 22588U, 50197U, 47561U, 58900U, 22797U, 15127U, 19198U, 54514U, 48075U, 15616U, 52552U, 3358U, 14810U, 21915U, 22772U, 13058U, 27431U, 30988U, 7285U, 27120U, 44118U, 6713U, 13167U, 13489U, 41579U, 9814U, 1305U, 5863U, 36897U, 49756U, 63351U, 33586U, 35692U, 65282U, 35561U, 34350U, 44116U, 15087U, 29662U, 43371U, 23199U, 33455U, 62617U, 51233U, 59497U, 12860U, 57699U, 64695U, 10490U, 46621U, 61041U, 2736U, 11671U, 25749U, 60172U, 34235U, 3445U, 55761U, 9764U, 16849U, 35650U, 34807U, 18197U, 6885U, 2003U, 57136U, 56989U, 50872U, 40338U, 10792U, 38417U, 47751U, 39246U, 15032U, 17685U, 1823U, 1404U, 23720U, 49986U, 31228U, 24760U, 34509U, 59263U, 42497U, 31796U, 39629U, 14879U, 11531U, 20312U, 61119U, 35595U, 8607U, 5483U, 13252U, 52959U, 6708U, 36198U, 60159U, 27284U, 58273U, 60510U, 48183U, 13861U, 56668U, 35355U, 24629U, 6309U, 19754U, 59155U, 19398U, 1251U, 54726U, 22083U, 38556U, 12548U, 58193U, 35644U, 20065U, 60311U, 4755U, 65390U, 60106U, 61574U, 43017U, 57990U, 20093U, 23583U, 40823U, 20380U, 50464U, 20005U, 45290U, 18959U, 7326U, 27595U, 25623U, 50145U, 23159U, 13970U, 35994U, 39292U, 55169U, 60628U, 12882U, 33466U, 43762U, 62548U, 64940U, 49589U, 12306U, 47771U, 2987U, 26225U, 33198U, 2488U, 64087U, 56033U, 44854U, 33383U, 30312U, 36050U, 52680U, 22742U, 5510U, 51720U, 65214U, 56689U, 16011U, 16748U, 12804U, 22806U, 42320U, 19914U, 26756U, 25470U, 26049U, 49606U, 58401U, 1211U, 15629U, 30555U, 41811U, 31227U, 48008U, 34083U, 10577U, 46039U, 16445U, 29995U, 33795U, 31657U, 1885U, 20790U, 41454U, 52879U, 18331U, 10438U, 29287U, 13517U, 12919U, 52650U, 24078U, 45465U, 1365U, 43740U, 14484U, 7191U, 35641U, 59747U, 33766U, 56624U, 47273U, 4650U, 29675U, 34517U, 2762U, 16602U, 40220U, 55467U, 56503U, 29510U, 13784U, 27442U, 7141U, 18960U, 43556U, 44140U, 38036U, 43156U, 33891U, 7854U, 57848U, 24795U, 24542U, 43417U, 50657U, 51244U, 56774U, 5868U, 61408U, 54026U, 37565U, 30592U, 5783U, 36257U, 7725U, 4081U, 23368U, 37845U, 47962U, 47896U, 4056U, 63040U, 17449U, 8026U, 43121U, 14017U, 41265U, 25441U, 50601U, 9686U, 32406U, 1130U, 16026U, 35260U, 36755U, 21394U, 20772U, 29554U, 62381U, 15327U, 38248U, 6880U, 59113U, 32702U, 8235U, 31839U, 26569U, 41272U, 18202U, 64794U, 40862U, 8852U, 64398U, 36860U, 59925U, 47020U, 60558U, 64524U, 59956U, 39028U, 3555U, 278U, 59132U, 15201U, 45575U, 53809U, 37562U, 33474U, 11507U, 54741U, 7995U, 39151U, 61253U, 11242U, 36216U, 16647U, 59026U, 7573U, 10615U, 42851U, 37884U, 55610U, 3201U, 22021U, 60023U, 4619U, 3350U, 20866U, 2605U, 5539U, 20962U, 33553U, 23176U, 65376U, 52965U, 59914U, 3300U, 32623U, 738U, 20620U, 40493U, 40477U, 48635U, 32827U, 12271U, 43384U, 40039U, 49411U, 34185U, 58056U, 3883U, 5507U, 46936U, 57341U, 22472U, 11824U, 47530U, 5909U, 33035U, 17410U, 11427U, 37768U, 65418U, 24605U, 32712U, 10262U, 39297U, 3264U, 21396U, 7262U, 36116U, 60698U, 29435U, 47168U, 60953U, 60272U, 48723U, 63874U, 18936U, 60526U, 24988U, 21225U, 37198U, 43101U, 18758U, 7760U, 46255U, 39254U, 49862U, 24098U, 44943U, 60694U, 29573U, 13146U, 24821U, 38511U, 58891U, 19663U, 29856U, 11050U, 36357U, 12150U, 14843U, 5862U, 5801U, 31101U, 59156U, 22456U, 37231U, 44827U, 6701U, 29767U, 20512U, 24766U, 64624U, 50507U, 53972U, 65094U, 37930U, 37010U, 32275U, 8042U, 34198U, 40632U, 4175U, 22978U, 18085U, 9936U, 42826U, 47337U, 5728U, 22276U, 12581U, 65398U, 43101U, 3420U, 34431U, 25493U, 9520U, 64612U, 18272U, 36531U, 27857U, 58118U, 20609U, 29520U, 17209U, 16955U, 8621U, 56508U, 51470U, 12085U, 16662U, 6730U, 39436U, 12938U, 46675U, 15905U, 4727U, 40263U, 1151U, 64534U, 62541U, 40754U, 45213U, 54898U, 40968U, 41733U, 8630U, 8823U, 58122U, 22814U, 63204U, 57390U, 9997U, 60267U, 43607U, 12030U, 11615U, 15659U, 53326U, 14531U, 15600U, 25289U, 62451U, 44382U, 16109U, 15801U, 12153U, 41487U, 31921U, 62982U, 4035U, 61256U, 43538U, 28687U, 20597U, 23929U, 13435U, 31271U, 13772U, 21483U, 24193U, 56373U, 63313U, 40035U, 32422U, 28057U, 184U, 49524U, 2944U, 21549U, 31420U, 25264U, 58994U, 49101U, 52852U, 4607U, 61530U, 49647U, 18531U, 5106U, 27195U, 50861U, 9456U, 26205U, 15182U, 21580U, 61011U, 23668U, 38695U, 8186U, 37042U, 19698U, 16349U, 35609U, 21757U, 25270U, 54450U, 9250U, 37845U, 29169U, 44497U, 55653U, 12538U, 36064U, 55646U, 3420U, 62152U, 41061U, 19418U, 50621U, 2843U, 7938U, 27230U, 63123U, 7650U, 20005U, 31550U, 30368U, 43924U, 5011U, 11597U, 44642U, 39451U, 43504U, 9394U, 20914U, 17923U, 55987U, 22426U, 5520U, 61756U, 57221U, 17830U, 57331U, 47620U, 60750U, 23166U, 16252U, 13942U, 29288U, 50481U, 39776U, 27008U, 13315U, 2713U, 52349U, 15497U, 15006U, 62499U, 7216U, 60348U, 2299U, 33912U, 18473U, 20130U, 16414U, 14560U, 34105U, 45787U, 10383U, 5798U, 20604U, 29195U, 65230U, 36423U, 13469U, 23259U, 56116U, 23536U, 57850U, 14429U, 43636U, 46613U, 44714U, 50811U, 41190U, 47043U, 45841U, 9433U, 27959U, 58640U, 16677U, 18994U, 50826U, 3814U, 31030U, 10512U, 34562U, 25809U, 35375U, 40851U, 50680U, 58451U, 860U, 10014U, 230U, 23527U, 38370U, 30241U, 31933U, 7729U, 44086U, 7547U, 22320U, 11367U, 64795U, 57344U, 63656U, 42362U, 12863U, 16725U, 55630U, 61568U, 16363U, 20434U, 52441U, 62520U, 15063U, 36266U, 794U, 16299U, 12981U, 52277U, 17846U, 6318U, 22992U, 62264U, 45157U, 21325U, 52249U, 24190U, 41796U, 61923U, 63776U, 60402U, 25024U, 18973U, 41955U, 304U, 45713U, 54382U, 11487U, 22741U, 35713U, 48821U, 35635U, 42049U, 2850U, 21427U, 41762U, 62809U, 25049U, 33082U, 49649U, 48460U, 11171U, 16077U, 9557U, 13455U, 18000U, 53455U, 13714U, 39069U, 40122U, 34198U, 26512U, 38972U, 26893U, 43582U, 25918U, 18870U, 58669U, 55876U, 56697U, 38864U, 7317U, 22719U, 46402U, 24475U, 28226U, 9885U, 27484U, 52561U, 65468U, 22979U, 8750U, 51258U, 49023U, 50225U, 51600U, 10125U, 16642U, 55621U, 33336U, 29961U, 22311U, 17304U, 33114U, 48069U, 26187U, 4910U, 24597U, 12817U, 38696U, 47961U, 49658U }  ;
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

