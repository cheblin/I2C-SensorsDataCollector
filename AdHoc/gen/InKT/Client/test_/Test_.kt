
package test_
import demo_.*
import java.util.*
import kotlin.experimental.*
import org.company.Client.*
import org.unirail.AdHoc
import org.unirail.AdHoc.*
import org.unirail.AdHoc.Pack.Cursor
import test_.util_.*

inline class DeviceVersion(val data: Pack.Bytes) {

    inline fun DevID(): UShort { return (get_bytes(data.bytes, data.origin + 0, 2)).toUShort() }
    inline fun DevID(src: UShort) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 0) }

    inline fun Ver(): UShort { return (get_bytes(data.bytes, data.origin + 2, 2)).toUShort() }
    inline fun Ver(src: UShort) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 2) }
}
inline class DeviceError(val data: Pack.Bytes) {

    inline fun param1(): UShort { return (get_bytes(data.bytes, data.origin + 0, 2)).toUShort() }
    inline fun param1(src: UShort) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 0) }

    inline fun error_id(): Errors { return Errors((get_bits(data.bytes, data.origin * 8 + 16, 1)).toByte()) }
    inline fun error_id(src: Errors) { set_bits((src.value).toULong().toLong(), 1, data.bytes, data.origin * 8 + 16) }
}
inline class SensorsData(val data: Pack.Bytes) {

    inline fun values(): SensorsData_values {
        return SensorsData_values(data)
    }

    inline fun values(src: UShortArray) {
        val len = minOf(src.size, 1000)

        for (index in 0 until len)
            set_bytes((src[index]).toULong().toLong(), 2, data.bytes, data.origin + 0 + index * 2)
    }
    object values {
        const val item_len = 1000
    }
}

class TestChannel : AdHoc.Channel() {

    var sendingPack: AdHoc.Pack? = null
    val transmitter: java.io.InputStream = object : Transmitter(1, 1) {
        override fun pullSendingPack(): Pack? {
            val ret = sendingPack
            sendingPack = null
            return ret
        }
    }
    val transmitterAdv: java.io.InputStream = object : Transmitter.Advanced(1, 1) {
        override fun pullSendingPack(): Pack? {
            val ret = sendingPack
            sendingPack = null
            return ret
        }
    }

    val receiver: java.io.OutputStream = object : Receiver(1, 1) {
        override fun dispatch(id: Int, pack: Pack?): Pack.Meta? { return test_dispatch(id, pack) }
    }
    val receiverAdv: java.io.OutputStream = object : Receiver.Advanced(1, 1) {
        override fun dispatch(id: Int, pack: Pack?): Pack.Meta? { return test_dispatch(id, pack) }
    }

    fun test_dispatch(id: Int, pack: Pack?): Pack.Meta? {
        when (id) {
            1 -> pack?.let { onStop() } ?: return Stop.pack.meta
            2 -> pack?.let { onStart() } ?: return Start.pack.meta
            3 -> pack?.let { onGetDeviceVersion() } ?: return GetDeviceVersion.pack.meta
            4 -> pack?.let { onGetConfiguration() } ?: return GetConfiguration.pack.meta
            5 -> pack?.let { onSetConfiguration() } ?: return SetConfiguration.pack.meta
        }
        return null
    }

    fun send(src: org.company.Client.DeviceVersion): Boolean {
        if (sendingPack != null) return false
        sendingPack = src.data.unwrap()
        return true
    }
    fun send(src: org.company.Client.DeviceError): Boolean {
        if (sendingPack != null) return false
        sendingPack = src.data.unwrap()
        return true
    }
    fun send(src: org.company.Client.SensorsData): Boolean {
        if (sendingPack != null) return false
        sendingPack = src.data.unwrap()
        return true
    }

    object NEW {
        fun DeviceVersion(cur: Pack.Bytes): test_.DeviceVersion {
            cur.init(org.company.Client.DeviceVersion.meta)
            return test_.DeviceVersion(cur)
        }
        fun DeviceError(cur: Pack.Bytes): test_.DeviceError {
            cur.init(org.company.Client.DeviceError.meta)
            return test_.DeviceError(cur)
        }
        fun SensorsData(cur: Pack.Bytes): test_.SensorsData {
            cur.init(org.company.Client.SensorsData.meta)
            return test_.SensorsData(cur)
        }
    }
}

fun fill(p0: test_.DeviceVersion) {
    p0.DevID((63026U).toUShort())
    p0.Ver((23611U).toUShort())
}

fun onDeviceVersion(p0: org.company.Client.DeviceVersion) {
    assert(p0.DevID() == (63026U).toUShort())
    assert(p0.Ver() == (23611U).toUShort())
    println("DeviceVersion \n")
}
fun onStop() { println("Stop \n") }
fun onStart() { println("Start \n") }
fun onGetDeviceVersion() { println("GetDeviceVersion \n") }
fun onGetConfiguration() { println("GetConfiguration \n") }
fun onSetConfiguration() { println("SetConfiguration \n") }

fun fill(p6: org.company.Client.BusConfiguration) {
    p6.multiplier((18U).toUByte())
    p6.time((10618U).toUShort())
    p6.clk_khz((21138U).toUShort())
}

fun onBusConfiguration(p6: org.company.Client.BusConfiguration) {
    assert(p6.multiplier() == (18U).toUByte())
    assert(p6.time() == (10618U).toUShort())
    assert(p6.clk_khz() == (21138U).toUShort())
    println("BusConfiguration \n")
}

fun fill(p7: org.company.Client.InstructionsPack) {
    p7.Length((47U).toUByte())
    p7.Instructions(ubyteArrayOf(108U, 200U, 157U, 20U, 57U, 104U, 176U, 52U, 142U, 8U, 95U, 87U, 208U, 242U, 198U, 95U, 140U, 193U, 136U, 16U, 201U, 0U, 36U, 219U, 59U, 58U, 168U, 228U, 59U, 161U, 141U, 147U, 33U, 130U, 58U, 196U, 47U, 129U, 242U, 153U, 204U, 20U, 221U, 211U, 32U, 24U, 56U, 122U, 190U, 115U, 158U, 40U, 73U, 79U, 238U, 254U, 164U, 222U, 232U, 194U, 208U, 208U, 249U, 138U, 216U, 121U, 86U, 164U, 73U, 194U, 227U, 131U, 249U, 221U, 71U, 197U, 145U, 32U, 105U, 66U, 184U, 132U, 4U, 247U, 171U, 2U, 111U, 141U, 11U, 209U, 137U, 132U, 252U, 29U, 251U, 174U, 144U, 52U, 210U, 60U, 71U, 12U, 0U, 245U, 28U, 118U, 1U, 118U, 204U, 25U, 76U, 67U, 47U, 92U, 11U, 29U, 125U, 4U, 173U, 183U, 114U, 64U, 10U, 156U, 122U, 147U, 164U, 98U, 200U, 23U, 97U, 90U, 12U, 241U, 126U, 52U, 55U, 175U, 169U, 36U, 212U, 25U, 188U, 39U, 128U, 139U, 211U, 154U, 89U, 31U, 0U, 47U, 234U, 219U, 148U, 43U, 203U, 24U, 18U, 41U, 110U, 242U, 222U, 114U, 196U, 188U, 80U, 128U, 171U, 239U, 238U, 28U, 104U, 235U, 180U, 178U, 222U, 98U, 184U, 14U, 166U, 130U, 109U, 144U, 205U, 134U, 13U, 36U, 126U, 195U, 10U, 179U, 48U, 41U, 100U, 164U, 231U, 60U, 196U, 212U, 255U, 196U, 164U, 2U, 155U, 100U, 218U, 222U, 232U, 110U, 9U, 33U, 183U, 26U, 54U, 195U, 84U, 197U, 111U, 88U, 206U, 130U, 117U, 137U, 186U, 88U, 133U, 22U, 226U, 237U, 207U, 9U, 94U, 129U, 98U, 244U, 31U, 98U, 200U, 199U, 226U, 28U, 188U, 142U, 205U, 66U, 132U, 197U, 140U, 183U, 228U, 189U, 55U, 235U, 238U, 240U))
}

fun onInstructionsPack(p7: org.company.Client.InstructionsPack) {
    assert(p7.Length() == (47U).toUByte())

    assert(p7.Instructions().same(ubyteArrayOf(108U, 200U, 157U, 20U, 57U, 104U, 176U, 52U, 142U, 8U, 95U, 87U, 208U, 242U, 198U, 95U, 140U, 193U, 136U, 16U, 201U, 0U, 36U, 219U, 59U, 58U, 168U, 228U, 59U, 161U, 141U, 147U, 33U, 130U, 58U, 196U, 47U, 129U, 242U, 153U, 204U, 20U, 221U, 211U, 32U, 24U, 56U, 122U, 190U, 115U, 158U, 40U, 73U, 79U, 238U, 254U, 164U, 222U, 232U, 194U, 208U, 208U, 249U, 138U, 216U, 121U, 86U, 164U, 73U, 194U, 227U, 131U, 249U, 221U, 71U, 197U, 145U, 32U, 105U, 66U, 184U, 132U, 4U, 247U, 171U, 2U, 111U, 141U, 11U, 209U, 137U, 132U, 252U, 29U, 251U, 174U, 144U, 52U, 210U, 60U, 71U, 12U, 0U, 245U, 28U, 118U, 1U, 118U, 204U, 25U, 76U, 67U, 47U, 92U, 11U, 29U, 125U, 4U, 173U, 183U, 114U, 64U, 10U, 156U, 122U, 147U, 164U, 98U, 200U, 23U, 97U, 90U, 12U, 241U, 126U, 52U, 55U, 175U, 169U, 36U, 212U, 25U, 188U, 39U, 128U, 139U, 211U, 154U, 89U, 31U, 0U, 47U, 234U, 219U, 148U, 43U, 203U, 24U, 18U, 41U, 110U, 242U, 222U, 114U, 196U, 188U, 80U, 128U, 171U, 239U, 238U, 28U, 104U, 235U, 180U, 178U, 222U, 98U, 184U, 14U, 166U, 130U, 109U, 144U, 205U, 134U, 13U, 36U, 126U, 195U, 10U, 179U, 48U, 41U, 100U, 164U, 231U, 60U, 196U, 212U, 255U, 196U, 164U, 2U, 155U, 100U, 218U, 222U, 232U, 110U, 9U, 33U, 183U, 26U, 54U, 195U, 84U, 197U, 111U, 88U, 206U, 130U, 117U, 137U, 186U, 88U, 133U, 22U, 226U, 237U, 207U, 9U, 94U, 129U, 98U, 244U, 31U, 98U, 200U, 199U, 226U, 28U, 188U, 142U, 205U, 66U, 132U, 197U, 140U, 183U, 228U, 189U, 55U, 235U, 238U, 240U)))

    println("InstructionsPack \n")
}

fun fill(p8: test_.DeviceError) {
    p8.param1((24967U).toUShort())
    p8.error_id(Errors.CannotReadSensor)
}

fun onDeviceError(p8: org.company.Client.DeviceError) {
    assert(p8.param1() == (24967U).toUShort())
    assert(p8.error_id() == Errors.CannotReadSensor)
    println("DeviceError \n")
}

fun fill(p9: test_.SensorsData) {
    p9.values(ushortArrayOf(44474U, 26062U, 21235U, 7374U, 15721U, 19317U, 45583U, 47438U, 62159U, 8591U, 8361U, 45579U, 11080U, 24658U, 57903U, 41066U, 64948U, 43172U, 21108U, 52919U, 35856U, 16041U, 54556U, 51840U, 3501U, 60116U, 50341U, 16061U, 39923U, 58508U, 33988U, 12499U, 40115U, 2681U, 19935U, 7265U, 33903U, 41302U, 5553U, 17189U, 26721U, 5621U, 47100U, 44162U, 54895U, 16215U, 23834U, 15741U, 34567U, 35153U, 51287U, 46645U, 64472U, 63833U, 38440U, 20588U, 21239U, 27050U, 3341U, 3027U, 28888U, 56644U, 349U, 41978U, 41865U, 40598U, 24584U, 40151U, 58542U, 60819U, 54910U, 33081U, 40553U, 2323U, 8935U, 6567U, 64597U, 53938U, 38949U, 9715U, 21744U, 31283U, 14435U, 55870U, 14365U, 34567U, 47342U, 20866U, 51704U, 60729U, 40626U, 50019U, 55727U, 54204U, 40439U, 9447U, 19997U, 59188U, 19793U, 16243U, 2780U, 52383U, 8876U, 52895U, 13116U, 8553U, 13127U, 12577U, 14280U, 56202U, 62607U, 39784U, 50866U, 19211U, 29698U, 18577U, 64087U, 45624U, 2379U, 35247U, 13287U, 53450U, 36798U, 64638U, 50516U, 10303U, 53840U, 44823U, 42082U, 24426U, 54832U, 51728U, 59391U, 43548U, 36228U, 27488U, 37963U, 47568U, 40563U, 48696U, 42188U, 56411U, 29895U, 39333U, 64911U, 17666U, 14861U, 37446U, 19906U, 58948U, 30290U, 9492U, 44939U, 60077U, 55405U, 6289U, 17649U, 21622U, 31546U, 33685U, 59911U, 47463U, 6415U, 6620U, 22502U, 31097U, 729U, 40866U, 15773U, 57815U, 54919U, 15884U, 52145U, 15451U, 37730U, 3150U, 48132U, 50294U, 9915U, 43374U, 48610U, 9733U, 1339U, 47885U, 5929U, 11811U, 55944U, 398U, 2314U, 29165U, 2447U, 30133U, 4176U, 43084U, 6565U, 54870U, 37981U, 23327U, 56326U, 29635U, 61383U, 52088U, 62364U, 32380U, 61195U, 2648U, 775U, 47723U, 35215U, 61722U, 51333U, 54820U, 15357U, 43875U, 52038U, 9514U, 64567U, 32995U, 23038U, 32465U, 45465U, 48123U, 13531U, 28788U, 17239U, 8289U, 14093U, 19660U, 50533U, 56095U, 46525U, 21326U, 48138U, 19614U, 33955U, 24725U, 32563U, 56412U, 65401U, 13760U, 36432U, 63730U, 20616U, 56138U, 55512U, 30179U, 41866U, 56518U, 34874U, 15827U, 57023U, 4630U, 22203U, 44835U, 46973U, 54536U, 50569U, 13402U, 21655U, 41362U, 15145U, 57535U, 62672U, 40053U, 3607U, 65342U, 35109U, 10470U, 2606U, 9530U, 28545U, 8531U, 55380U, 13375U, 4404U, 9750U, 43041U, 55427U, 63541U, 61717U, 55811U, 3080U, 63511U, 33706U, 9034U, 17919U, 59324U, 41524U, 36416U, 61445U, 46382U, 40544U, 61258U, 10803U, 22172U, 58112U, 57545U, 43388U, 43597U, 18935U, 25970U, 31569U, 20025U, 38963U, 9138U, 4952U, 7837U, 27419U, 37893U, 45776U, 52736U, 37541U, 55049U, 6465U, 8056U, 54883U, 17532U, 2051U, 49938U, 9733U, 45673U, 12995U, 12529U, 60526U, 21704U, 24054U, 19294U, 22261U, 42294U, 46295U, 11845U, 20141U, 44768U, 6292U, 53251U, 17516U, 21551U, 12546U, 27881U, 60626U, 35993U, 23258U, 52312U, 54660U, 4402U, 24471U, 21799U, 8182U, 7390U, 53618U, 60742U, 2153U, 9658U, 28726U, 46856U, 19716U, 43165U, 23491U, 37046U, 64957U, 34570U, 49605U, 25208U, 17260U, 30487U, 44513U, 53115U, 4450U, 58977U, 6162U, 54613U, 10717U, 63505U, 59824U, 26149U, 28977U, 6378U, 20358U, 54072U, 38883U, 34511U, 7254U, 18826U, 34588U, 55072U, 577U, 26740U, 25072U, 61226U, 5723U, 46005U, 34429U, 49209U, 11783U, 4589U, 36423U, 18418U, 39539U, 39594U, 62396U, 60744U, 25954U, 17665U, 62832U, 31797U, 9884U, 4737U, 20829U, 61470U, 43256U, 17622U, 48303U, 28544U, 2088U, 58436U, 22637U, 1075U, 63162U, 32347U, 29658U, 27017U, 52792U, 14188U, 44414U, 19778U, 60281U, 65304U, 2666U, 16585U, 16559U, 49781U, 4190U, 47753U, 55269U, 51225U, 29377U, 43458U, 35987U, 24916U, 5232U, 45251U, 51798U, 55173U, 15048U, 59105U, 49880U, 29016U, 3631U, 5397U, 38000U, 51453U, 51625U, 54602U, 22239U, 19511U, 39362U, 25033U, 12048U, 17820U, 65387U, 12566U, 39127U, 34216U, 34512U, 37982U, 62978U, 25345U, 23912U, 1559U, 30270U, 64590U, 33293U, 30886U, 36314U, 11735U, 61904U, 13426U, 36628U, 6025U, 53240U, 8683U, 50156U, 48367U, 23464U, 6255U, 33871U, 43812U, 49022U, 7277U, 22881U, 14155U, 42038U, 51554U, 48428U, 6233U, 8968U, 38109U, 27467U, 22859U, 17035U, 34520U, 9152U, 60574U, 48740U, 25631U, 53022U, 61978U, 5807U, 22492U, 15208U, 6212U, 52374U, 63704U, 39259U, 27835U, 64737U, 22040U, 18352U, 13948U, 23015U, 20941U, 53492U, 63045U, 35034U, 400U, 64929U, 64411U, 45230U, 50135U, 64716U, 20597U, 38059U, 37443U, 53767U, 22107U, 22923U, 44091U, 61540U, 61332U, 4430U, 8838U, 57735U, 51188U, 11874U, 28103U, 25652U, 61504U, 60831U, 36264U, 59356U, 23197U, 24911U, 4516U, 148U, 60908U, 53555U, 29206U, 18544U, 12842U, 21845U, 10525U, 45232U, 20263U, 39119U, 35440U, 26818U, 8245U, 57994U, 16427U, 34629U, 10068U, 46168U, 22609U, 51572U, 49880U, 44527U, 8779U, 64816U, 37068U, 54419U, 32650U, 32694U, 8952U, 15992U, 30941U, 29357U, 2035U, 1500U, 41923U, 61584U, 23324U, 56824U, 36090U, 33187U, 36773U, 52705U, 16572U, 27211U, 21054U, 16400U, 14084U, 47130U, 50610U, 55388U, 55068U, 7714U, 1425U, 28454U, 2859U, 6106U, 18546U, 60187U, 53091U, 30885U, 37816U, 54290U, 37470U, 56481U, 24721U, 64955U, 33138U, 62788U, 40023U, 46306U, 7994U, 321U, 20192U, 58107U, 26952U, 56076U, 21387U, 41362U, 25240U, 54369U, 51713U, 1721U, 36614U, 40038U, 4275U, 8278U, 22928U, 13241U, 40960U, 35733U, 16328U, 59020U, 39698U, 54967U, 29938U, 45597U, 65312U, 16858U, 50491U, 4542U, 44386U, 22931U, 5820U, 63525U, 56501U, 56387U, 20288U, 13566U, 42935U, 26284U, 13394U, 60264U, 47362U, 29516U, 24080U, 28273U, 9510U, 14906U, 45732U, 20588U, 33821U, 15423U, 42025U, 29864U, 21771U, 53215U, 1531U, 29860U, 51657U, 29527U, 22615U, 15517U, 20113U, 40844U, 59338U, 6473U, 38205U, 28384U, 62499U, 57266U, 2287U, 41938U, 36680U, 52606U, 32027U, 10438U, 54154U, 25528U, 14467U, 13242U, 43563U, 30243U, 8843U, 20146U, 25249U, 60599U, 6332U, 20336U, 63313U, 45017U, 55060U, 16373U, 31064U, 55995U, 9613U, 53662U, 64605U, 37687U, 7556U, 40288U, 50539U, 2232U, 39196U, 58604U, 35015U, 38736U, 55352U, 1981U, 32622U, 56620U, 5904U, 15941U, 46517U, 61657U, 57146U, 39697U, 53102U, 8366U, 49758U, 29727U, 59044U, 59367U, 29808U, 12739U, 10930U, 33833U, 24097U, 58479U, 53306U, 57452U, 1941U, 17185U, 15983U, 1819U, 23548U, 10656U, 56195U, 60999U, 58104U, 20102U, 45597U, 56366U, 3321U, 23195U, 16785U, 5322U, 23041U, 14015U, 39905U, 24233U, 53464U, 36923U, 43618U, 54879U, 60874U, 18947U, 24609U, 11855U, 9961U, 47131U, 50261U, 62910U, 45419U, 16745U, 28384U, 20229U, 32041U, 8070U, 15760U, 10061U, 20144U, 6106U, 11074U, 24671U, 45393U, 26198U, 21889U, 50841U, 5093U, 53352U, 18003U, 19996U, 13001U, 45861U, 37802U, 13692U, 6280U, 44477U, 6461U, 16586U, 63139U, 5005U, 62231U, 50379U, 42472U, 49779U, 38118U, 45917U, 5962U, 52657U, 24066U, 9036U, 15902U, 56681U, 1027U, 20300U, 33109U, 64726U, 52727U, 51938U, 58201U, 54040U, 64182U, 15931U, 51194U, 23452U, 43275U, 63705U, 40772U, 836U, 17022U, 22316U, 44080U, 37125U, 16904U, 32487U, 26933U, 32852U, 16808U, 52066U, 28164U, 9552U, 4875U, 57041U, 26345U, 17590U, 1785U, 56542U, 20898U, 30114U, 64446U, 22733U, 7459U, 29582U, 22199U, 27398U, 58459U, 9343U, 25091U, 59905U, 28423U, 5044U, 5687U, 17455U, 60488U, 40563U, 6592U, 45074U, 55535U, 5732U, 55295U, 32402U, 50661U, 5117U, 4406U, 53988U, 6804U, 35961U, 15208U, 56683U, 13685U, 39219U, 37305U, 28866U, 60268U, 16648U, 39713U, 31340U, 43224U, 14357U, 57694U, 17433U, 20904U, 58023U, 34889U, 12997U, 13084U, 22014U, 55295U, 52813U, 34926U, 52180U, 32451U, 29556U, 2585U, 63574U, 56235U, 13720U, 2329U, 51282U, 30438U, 27124U, 13530U, 43644U, 51850U, 36112U, 697U, 36458U, 34498U, 26430U, 62638U, 34706U, 58963U, 841U, 30749U, 64992U, 38164U, 16552U, 39611U, 4131U, 50791U, 26523U, 16300U, 25691U, 41580U, 20202U, 29263U, 57985U, 3075U, 26108U, 58248U, 26352U, 33766U, 7084U, 29562U, 11154U, 36798U, 51797U, 25508U, 26631U, 21315U, 28228U, 43593U, 8403U, 45135U, 18680U, 8985U, 40970U, 51343U, 63148U, 34465U, 23594U, 13983U, 37878U, 50630U, 15538U, 9603U, 48727U, 17696U, 53042U, 38555U, 22133U, 15934U, 44262U, 6801U, 2637U, 51547U, 49197U, 51273U, 26777U, 23012U, 17050U, 28644U, 36516U, 61880U, 45341U))
}

fun onSensorsData(p9: org.company.Client.SensorsData) {
    assert(p9.values().same(ushortArrayOf(44474U, 26062U, 21235U, 7374U, 15721U, 19317U, 45583U, 47438U, 62159U, 8591U, 8361U, 45579U, 11080U, 24658U, 57903U, 41066U, 64948U, 43172U, 21108U, 52919U, 35856U, 16041U, 54556U, 51840U, 3501U, 60116U, 50341U, 16061U, 39923U, 58508U, 33988U, 12499U, 40115U, 2681U, 19935U, 7265U, 33903U, 41302U, 5553U, 17189U, 26721U, 5621U, 47100U, 44162U, 54895U, 16215U, 23834U, 15741U, 34567U, 35153U, 51287U, 46645U, 64472U, 63833U, 38440U, 20588U, 21239U, 27050U, 3341U, 3027U, 28888U, 56644U, 349U, 41978U, 41865U, 40598U, 24584U, 40151U, 58542U, 60819U, 54910U, 33081U, 40553U, 2323U, 8935U, 6567U, 64597U, 53938U, 38949U, 9715U, 21744U, 31283U, 14435U, 55870U, 14365U, 34567U, 47342U, 20866U, 51704U, 60729U, 40626U, 50019U, 55727U, 54204U, 40439U, 9447U, 19997U, 59188U, 19793U, 16243U, 2780U, 52383U, 8876U, 52895U, 13116U, 8553U, 13127U, 12577U, 14280U, 56202U, 62607U, 39784U, 50866U, 19211U, 29698U, 18577U, 64087U, 45624U, 2379U, 35247U, 13287U, 53450U, 36798U, 64638U, 50516U, 10303U, 53840U, 44823U, 42082U, 24426U, 54832U, 51728U, 59391U, 43548U, 36228U, 27488U, 37963U, 47568U, 40563U, 48696U, 42188U, 56411U, 29895U, 39333U, 64911U, 17666U, 14861U, 37446U, 19906U, 58948U, 30290U, 9492U, 44939U, 60077U, 55405U, 6289U, 17649U, 21622U, 31546U, 33685U, 59911U, 47463U, 6415U, 6620U, 22502U, 31097U, 729U, 40866U, 15773U, 57815U, 54919U, 15884U, 52145U, 15451U, 37730U, 3150U, 48132U, 50294U, 9915U, 43374U, 48610U, 9733U, 1339U, 47885U, 5929U, 11811U, 55944U, 398U, 2314U, 29165U, 2447U, 30133U, 4176U, 43084U, 6565U, 54870U, 37981U, 23327U, 56326U, 29635U, 61383U, 52088U, 62364U, 32380U, 61195U, 2648U, 775U, 47723U, 35215U, 61722U, 51333U, 54820U, 15357U, 43875U, 52038U, 9514U, 64567U, 32995U, 23038U, 32465U, 45465U, 48123U, 13531U, 28788U, 17239U, 8289U, 14093U, 19660U, 50533U, 56095U, 46525U, 21326U, 48138U, 19614U, 33955U, 24725U, 32563U, 56412U, 65401U, 13760U, 36432U, 63730U, 20616U, 56138U, 55512U, 30179U, 41866U, 56518U, 34874U, 15827U, 57023U, 4630U, 22203U, 44835U, 46973U, 54536U, 50569U, 13402U, 21655U, 41362U, 15145U, 57535U, 62672U, 40053U, 3607U, 65342U, 35109U, 10470U, 2606U, 9530U, 28545U, 8531U, 55380U, 13375U, 4404U, 9750U, 43041U, 55427U, 63541U, 61717U, 55811U, 3080U, 63511U, 33706U, 9034U, 17919U, 59324U, 41524U, 36416U, 61445U, 46382U, 40544U, 61258U, 10803U, 22172U, 58112U, 57545U, 43388U, 43597U, 18935U, 25970U, 31569U, 20025U, 38963U, 9138U, 4952U, 7837U, 27419U, 37893U, 45776U, 52736U, 37541U, 55049U, 6465U, 8056U, 54883U, 17532U, 2051U, 49938U, 9733U, 45673U, 12995U, 12529U, 60526U, 21704U, 24054U, 19294U, 22261U, 42294U, 46295U, 11845U, 20141U, 44768U, 6292U, 53251U, 17516U, 21551U, 12546U, 27881U, 60626U, 35993U, 23258U, 52312U, 54660U, 4402U, 24471U, 21799U, 8182U, 7390U, 53618U, 60742U, 2153U, 9658U, 28726U, 46856U, 19716U, 43165U, 23491U, 37046U, 64957U, 34570U, 49605U, 25208U, 17260U, 30487U, 44513U, 53115U, 4450U, 58977U, 6162U, 54613U, 10717U, 63505U, 59824U, 26149U, 28977U, 6378U, 20358U, 54072U, 38883U, 34511U, 7254U, 18826U, 34588U, 55072U, 577U, 26740U, 25072U, 61226U, 5723U, 46005U, 34429U, 49209U, 11783U, 4589U, 36423U, 18418U, 39539U, 39594U, 62396U, 60744U, 25954U, 17665U, 62832U, 31797U, 9884U, 4737U, 20829U, 61470U, 43256U, 17622U, 48303U, 28544U, 2088U, 58436U, 22637U, 1075U, 63162U, 32347U, 29658U, 27017U, 52792U, 14188U, 44414U, 19778U, 60281U, 65304U, 2666U, 16585U, 16559U, 49781U, 4190U, 47753U, 55269U, 51225U, 29377U, 43458U, 35987U, 24916U, 5232U, 45251U, 51798U, 55173U, 15048U, 59105U, 49880U, 29016U, 3631U, 5397U, 38000U, 51453U, 51625U, 54602U, 22239U, 19511U, 39362U, 25033U, 12048U, 17820U, 65387U, 12566U, 39127U, 34216U, 34512U, 37982U, 62978U, 25345U, 23912U, 1559U, 30270U, 64590U, 33293U, 30886U, 36314U, 11735U, 61904U, 13426U, 36628U, 6025U, 53240U, 8683U, 50156U, 48367U, 23464U, 6255U, 33871U, 43812U, 49022U, 7277U, 22881U, 14155U, 42038U, 51554U, 48428U, 6233U, 8968U, 38109U, 27467U, 22859U, 17035U, 34520U, 9152U, 60574U, 48740U, 25631U, 53022U, 61978U, 5807U, 22492U, 15208U, 6212U, 52374U, 63704U, 39259U, 27835U, 64737U, 22040U, 18352U, 13948U, 23015U, 20941U, 53492U, 63045U, 35034U, 400U, 64929U, 64411U, 45230U, 50135U, 64716U, 20597U, 38059U, 37443U, 53767U, 22107U, 22923U, 44091U, 61540U, 61332U, 4430U, 8838U, 57735U, 51188U, 11874U, 28103U, 25652U, 61504U, 60831U, 36264U, 59356U, 23197U, 24911U, 4516U, 148U, 60908U, 53555U, 29206U, 18544U, 12842U, 21845U, 10525U, 45232U, 20263U, 39119U, 35440U, 26818U, 8245U, 57994U, 16427U, 34629U, 10068U, 46168U, 22609U, 51572U, 49880U, 44527U, 8779U, 64816U, 37068U, 54419U, 32650U, 32694U, 8952U, 15992U, 30941U, 29357U, 2035U, 1500U, 41923U, 61584U, 23324U, 56824U, 36090U, 33187U, 36773U, 52705U, 16572U, 27211U, 21054U, 16400U, 14084U, 47130U, 50610U, 55388U, 55068U, 7714U, 1425U, 28454U, 2859U, 6106U, 18546U, 60187U, 53091U, 30885U, 37816U, 54290U, 37470U, 56481U, 24721U, 64955U, 33138U, 62788U, 40023U, 46306U, 7994U, 321U, 20192U, 58107U, 26952U, 56076U, 21387U, 41362U, 25240U, 54369U, 51713U, 1721U, 36614U, 40038U, 4275U, 8278U, 22928U, 13241U, 40960U, 35733U, 16328U, 59020U, 39698U, 54967U, 29938U, 45597U, 65312U, 16858U, 50491U, 4542U, 44386U, 22931U, 5820U, 63525U, 56501U, 56387U, 20288U, 13566U, 42935U, 26284U, 13394U, 60264U, 47362U, 29516U, 24080U, 28273U, 9510U, 14906U, 45732U, 20588U, 33821U, 15423U, 42025U, 29864U, 21771U, 53215U, 1531U, 29860U, 51657U, 29527U, 22615U, 15517U, 20113U, 40844U, 59338U, 6473U, 38205U, 28384U, 62499U, 57266U, 2287U, 41938U, 36680U, 52606U, 32027U, 10438U, 54154U, 25528U, 14467U, 13242U, 43563U, 30243U, 8843U, 20146U, 25249U, 60599U, 6332U, 20336U, 63313U, 45017U, 55060U, 16373U, 31064U, 55995U, 9613U, 53662U, 64605U, 37687U, 7556U, 40288U, 50539U, 2232U, 39196U, 58604U, 35015U, 38736U, 55352U, 1981U, 32622U, 56620U, 5904U, 15941U, 46517U, 61657U, 57146U, 39697U, 53102U, 8366U, 49758U, 29727U, 59044U, 59367U, 29808U, 12739U, 10930U, 33833U, 24097U, 58479U, 53306U, 57452U, 1941U, 17185U, 15983U, 1819U, 23548U, 10656U, 56195U, 60999U, 58104U, 20102U, 45597U, 56366U, 3321U, 23195U, 16785U, 5322U, 23041U, 14015U, 39905U, 24233U, 53464U, 36923U, 43618U, 54879U, 60874U, 18947U, 24609U, 11855U, 9961U, 47131U, 50261U, 62910U, 45419U, 16745U, 28384U, 20229U, 32041U, 8070U, 15760U, 10061U, 20144U, 6106U, 11074U, 24671U, 45393U, 26198U, 21889U, 50841U, 5093U, 53352U, 18003U, 19996U, 13001U, 45861U, 37802U, 13692U, 6280U, 44477U, 6461U, 16586U, 63139U, 5005U, 62231U, 50379U, 42472U, 49779U, 38118U, 45917U, 5962U, 52657U, 24066U, 9036U, 15902U, 56681U, 1027U, 20300U, 33109U, 64726U, 52727U, 51938U, 58201U, 54040U, 64182U, 15931U, 51194U, 23452U, 43275U, 63705U, 40772U, 836U, 17022U, 22316U, 44080U, 37125U, 16904U, 32487U, 26933U, 32852U, 16808U, 52066U, 28164U, 9552U, 4875U, 57041U, 26345U, 17590U, 1785U, 56542U, 20898U, 30114U, 64446U, 22733U, 7459U, 29582U, 22199U, 27398U, 58459U, 9343U, 25091U, 59905U, 28423U, 5044U, 5687U, 17455U, 60488U, 40563U, 6592U, 45074U, 55535U, 5732U, 55295U, 32402U, 50661U, 5117U, 4406U, 53988U, 6804U, 35961U, 15208U, 56683U, 13685U, 39219U, 37305U, 28866U, 60268U, 16648U, 39713U, 31340U, 43224U, 14357U, 57694U, 17433U, 20904U, 58023U, 34889U, 12997U, 13084U, 22014U, 55295U, 52813U, 34926U, 52180U, 32451U, 29556U, 2585U, 63574U, 56235U, 13720U, 2329U, 51282U, 30438U, 27124U, 13530U, 43644U, 51850U, 36112U, 697U, 36458U, 34498U, 26430U, 62638U, 34706U, 58963U, 841U, 30749U, 64992U, 38164U, 16552U, 39611U, 4131U, 50791U, 26523U, 16300U, 25691U, 41580U, 20202U, 29263U, 57985U, 3075U, 26108U, 58248U, 26352U, 33766U, 7084U, 29562U, 11154U, 36798U, 51797U, 25508U, 26631U, 21315U, 28228U, 43593U, 8403U, 45135U, 18680U, 8985U, 40970U, 51343U, 63148U, 34465U, 23594U, 13983U, 37878U, 50630U, 15538U, 9603U, 48727U, 17696U, 53042U, 38555U, 22133U, 15934U, 44262U, 6801U, 2637U, 51547U, 49197U, 51273U, 26777U, 23012U, 17050U, 28644U, 36516U, 61880U, 45341U)))

    println("SensorsData \n")
}

class Communication_test : Communication() {
    override fun on_DeviceVersion(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p0 = org.company.Client.DeviceVersion(bytes)
        onDeviceVersion(p0)
    }
    override fun on_BusConfiguration(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p6 = org.company.Client.BusConfiguration(bytes)
        onBusConfiguration(p6)
    }
    override fun on_InstructionsPack(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p7 = org.company.Client.InstructionsPack(bytes)
        onInstructionsPack(p7)
    }
    override fun on_DeviceError(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p8 = org.company.Client.DeviceError(bytes)
        onDeviceError(p8)
    }
    override fun on_SensorsData(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p9 = org.company.Client.SensorsData(bytes)
        onSensorsData(p9)
    }
    var sendingPack: Pack? = null
    override fun pullSendingPack(): Pack? {
        val ret = sendingPack
        sendingPack = null
        return ret
    }
    override fun pushSendingPack(pack: AdHoc.Pack): Boolean {
        if (sendingPack != null) return false

        sendingPack = pack
        return true
    }
}

// By default, assertions are disabled at runtime. Two command-line switches allow you to selectively enable or disable assertions.
// To enable assertions at various granularities, use the -enableassertions, or -ea, switch.
// To disable assertions at various granularities, use the -disableassertions, or -da,

fun main() {
    val cur = Cursor()
    val cur_dst = Cursor()

    val buff = ByteArray(1024)
    fun transmission(src: java.io.InputStream, dst: java.io.OutputStream) {
        try {
            while (true) {
                val bytes = src.read(buff, 0, buff.size)
                if (bytes < 1) break
                dst.write(buff, 0, bytes)
            }
        } catch (e: java.io.IOException) {
            e.printStackTrace()
            assert(false)
        }
    }
    val TestChannel_instance = TestChannel()
    val Communication_instance = Communication_test()

    println("-------------------- DeviceVersion -------------------------")

    fill(TestChannel.NEW.DeviceVersion(cur))
    val p0 = org.company.Client.DeviceVersion(cur)
    onDeviceVersion(p0)
    run {
        cur_dst.init(org.company.Client.DeviceVersion.meta)
        val dst = test_.DeviceVersion(cur_dst)
        dst.copyFrom(p0)
        assert(Arrays.equals(p0.data.bytes, dst.data.bytes))
    }

    if (TestChannel_instance.send(p0)) transmission(TestChannel_instance.transmitter, Communication_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- Stop -------------------------")

    if (Communication_instance.sendStop()) transmission(Communication_instance.transmitter, TestChannel_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- Start -------------------------")

    if (Communication_instance.sendStart()) transmission(Communication_instance.transmitter, TestChannel_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- GetDeviceVersion -------------------------")

    if (Communication_instance.sendGetDeviceVersion()) transmission(Communication_instance.transmitter, TestChannel_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- GetConfiguration -------------------------")

    if (Communication_instance.sendGetConfiguration()) transmission(Communication_instance.transmitter, TestChannel_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- SetConfiguration -------------------------")

    if (Communication_instance.sendSetConfiguration()) transmission(Communication_instance.transmitter, TestChannel_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- BusConfiguration -------------------------")

    val p6 = Communication.NEW.BusConfiguration(cur)
    fill(p6)
    onBusConfiguration(p6)
    run {
        var dst = Communication.NEW.BusConfiguration(cur_dst)
        p6.copyInto(dst)
        assert(Arrays.equals(p6.data.bytes, dst.data.bytes))

        dst = Communication.NEW.BusConfiguration(cur_dst)
        dst.copyFrom(p6)
        assert(Arrays.equals(p6.data.bytes, dst.data.bytes))
    }

    if (Communication_instance.send(p6)) transmission(Communication_instance.transmitter, Communication_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- InstructionsPack -------------------------")

    val p7 = Communication.NEW.InstructionsPack(cur)
    fill(p7)
    onInstructionsPack(p7)
    run {
        var dst = Communication.NEW.InstructionsPack(cur_dst)
        p7.copyInto(dst)
        assert(Arrays.equals(p7.data.bytes, dst.data.bytes))

        dst = Communication.NEW.InstructionsPack(cur_dst)
        dst.copyFrom(p7)
        assert(Arrays.equals(p7.data.bytes, dst.data.bytes))
    }

    if (Communication_instance.send(p7)) transmission(Communication_instance.transmitter, Communication_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- DeviceError -------------------------")

    fill(TestChannel.NEW.DeviceError(cur))
    val p8 = org.company.Client.DeviceError(cur)
    onDeviceError(p8)
    run {
        cur_dst.init(org.company.Client.DeviceError.meta)
        val dst = test_.DeviceError(cur_dst)
        dst.copyFrom(p8)
        assert(Arrays.equals(p8.data.bytes, dst.data.bytes))
    }

    if (TestChannel_instance.send(p8)) transmission(TestChannel_instance.transmitter, Communication_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    println("-------------------- SensorsData -------------------------")

    fill(TestChannel.NEW.SensorsData(cur))
    val p9 = org.company.Client.SensorsData(cur)
    onSensorsData(p9)
    run {
        cur_dst.init(org.company.Client.SensorsData.meta)
        val dst = test_.SensorsData(cur_dst)
        dst.copyFrom(p9)
        assert(Arrays.equals(p9.data.bytes, dst.data.bytes))
    }

    if (TestChannel_instance.send(p9)) transmission(TestChannel_instance.transmitter, Communication_instance.receiver)
    else throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
}
