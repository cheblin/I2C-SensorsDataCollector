
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
    p0.DevID((13028U).toUShort())
    p0.Ver((33357U).toUShort())
}

fun onDeviceVersion(p0: org.company.Client.DeviceVersion) {
    assert(p0.DevID() == (13028U).toUShort())
    assert(p0.Ver() == (33357U).toUShort())
    println("DeviceVersion \n")
}
fun onStop() { println("Stop \n") }
fun onStart() { println("Start \n") }
fun onGetDeviceVersion() { println("GetDeviceVersion \n") }
fun onGetConfiguration() { println("GetConfiguration \n") }
fun onSetConfiguration() { println("SetConfiguration \n") }

fun fill(p6: org.company.Client.BusConfiguration) {
    p6.multiplier((43U).toUByte())
    p6.time((5646U).toUShort())
    p6.clk_khz((28959U).toUShort())
}

fun onBusConfiguration(p6: org.company.Client.BusConfiguration) {
    assert(p6.multiplier() == (43U).toUByte())
    assert(p6.time() == (5646U).toUShort())
    assert(p6.clk_khz() == (28959U).toUShort())
    println("BusConfiguration \n")
}

fun fill(p7: org.company.Client.InstructionsPack) {
    p7.Length((33U).toUByte())
    p7.Instructions(ubyteArrayOf(184U, 98U, 130U, 209U, 147U, 135U, 193U, 19U, 159U, 2U, 112U, 164U, 223U, 194U, 84U, 254U, 13U, 202U, 204U, 233U, 209U, 103U, 194U, 31U, 146U, 240U, 187U, 103U, 166U, 59U, 197U, 212U, 200U, 145U, 244U, 23U, 87U, 210U, 126U, 106U, 101U, 174U, 3U, 114U, 232U, 80U, 21U, 188U, 147U, 238U, 239U, 206U, 114U, 45U, 102U, 127U, 41U, 34U, 110U, 209U, 63U, 122U, 212U, 37U, 212U, 53U, 248U, 39U, 240U, 144U, 175U, 144U, 255U, 29U, 84U, 60U, 198U, 87U, 206U, 44U, 182U, 113U, 160U, 2U, 47U, 11U, 43U, 51U, 218U, 114U, 104U, 238U, 235U, 160U, 205U, 7U, 71U, 134U, 59U, 40U, 93U, 30U, 98U, 167U, 205U, 65U, 26U, 59U, 14U, 246U, 60U, 140U, 218U, 58U, 29U, 241U, 77U, 47U, 68U, 213U, 200U, 173U, 159U, 223U, 124U, 226U, 93U, 69U, 124U, 217U, 113U, 123U, 239U, 254U, 191U, 33U, 127U, 171U, 134U, 5U, 64U, 91U, 104U, 253U, 28U, 70U, 253U, 124U, 139U, 94U, 26U, 200U, 15U, 146U, 99U, 194U, 125U, 200U, 251U, 95U, 252U, 162U, 27U, 81U, 92U, 133U, 105U, 48U, 153U, 66U, 48U, 164U, 137U, 102U, 123U, 189U, 146U, 180U, 120U, 45U, 179U, 19U, 58U, 25U, 103U, 23U, 67U, 242U, 41U, 230U, 123U, 253U, 196U, 242U, 106U, 60U, 196U, 70U, 87U, 119U, 22U, 142U, 130U, 208U, 192U, 236U, 155U, 203U, 186U, 140U, 134U, 7U, 19U, 176U, 27U, 13U, 65U, 188U, 149U, 188U, 203U, 161U, 209U, 141U, 188U, 255U, 185U, 107U, 35U, 110U, 145U, 249U, 100U, 19U, 0U, 183U, 69U, 51U, 208U, 170U, 90U, 148U, 221U, 148U, 54U, 235U, 79U, 151U, 113U, 210U, 170U, 148U, 62U, 4U, 100U, 26U))
}

fun onInstructionsPack(p7: org.company.Client.InstructionsPack) {
    assert(p7.Length() == (33U).toUByte())

    assert(p7.Instructions().same(ubyteArrayOf(184U, 98U, 130U, 209U, 147U, 135U, 193U, 19U, 159U, 2U, 112U, 164U, 223U, 194U, 84U, 254U, 13U, 202U, 204U, 233U, 209U, 103U, 194U, 31U, 146U, 240U, 187U, 103U, 166U, 59U, 197U, 212U, 200U, 145U, 244U, 23U, 87U, 210U, 126U, 106U, 101U, 174U, 3U, 114U, 232U, 80U, 21U, 188U, 147U, 238U, 239U, 206U, 114U, 45U, 102U, 127U, 41U, 34U, 110U, 209U, 63U, 122U, 212U, 37U, 212U, 53U, 248U, 39U, 240U, 144U, 175U, 144U, 255U, 29U, 84U, 60U, 198U, 87U, 206U, 44U, 182U, 113U, 160U, 2U, 47U, 11U, 43U, 51U, 218U, 114U, 104U, 238U, 235U, 160U, 205U, 7U, 71U, 134U, 59U, 40U, 93U, 30U, 98U, 167U, 205U, 65U, 26U, 59U, 14U, 246U, 60U, 140U, 218U, 58U, 29U, 241U, 77U, 47U, 68U, 213U, 200U, 173U, 159U, 223U, 124U, 226U, 93U, 69U, 124U, 217U, 113U, 123U, 239U, 254U, 191U, 33U, 127U, 171U, 134U, 5U, 64U, 91U, 104U, 253U, 28U, 70U, 253U, 124U, 139U, 94U, 26U, 200U, 15U, 146U, 99U, 194U, 125U, 200U, 251U, 95U, 252U, 162U, 27U, 81U, 92U, 133U, 105U, 48U, 153U, 66U, 48U, 164U, 137U, 102U, 123U, 189U, 146U, 180U, 120U, 45U, 179U, 19U, 58U, 25U, 103U, 23U, 67U, 242U, 41U, 230U, 123U, 253U, 196U, 242U, 106U, 60U, 196U, 70U, 87U, 119U, 22U, 142U, 130U, 208U, 192U, 236U, 155U, 203U, 186U, 140U, 134U, 7U, 19U, 176U, 27U, 13U, 65U, 188U, 149U, 188U, 203U, 161U, 209U, 141U, 188U, 255U, 185U, 107U, 35U, 110U, 145U, 249U, 100U, 19U, 0U, 183U, 69U, 51U, 208U, 170U, 90U, 148U, 221U, 148U, 54U, 235U, 79U, 151U, 113U, 210U, 170U, 148U, 62U, 4U, 100U, 26U)))

    println("InstructionsPack \n")
}

fun fill(p8: test_.DeviceError) {
    p8.param1((5041U).toUShort())
    p8.error_id(Errors.CannotReadSensor)
}

fun onDeviceError(p8: org.company.Client.DeviceError) {
    assert(p8.param1() == (5041U).toUShort())
    assert(p8.error_id() == Errors.CannotReadSensor)
    println("DeviceError \n")
}

fun fill(p9: test_.SensorsData) {
    p9.values(ushortArrayOf(2998U, 31673U, 19893U, 8089U, 23213U, 51676U, 18008U, 52147U, 19088U, 41803U, 65507U, 28694U, 51159U, 52701U, 53415U, 38420U, 64275U, 29305U, 9312U, 37626U, 36413U, 35896U, 44752U, 27262U, 48558U, 51902U, 57328U, 37091U, 17044U, 52340U, 12786U, 49269U, 18609U, 40465U, 44419U, 27268U, 40935U, 61019U, 27946U, 54343U, 55131U, 15301U, 12547U, 8983U, 1740U, 55733U, 11466U, 59591U, 29320U, 63264U, 44842U, 10984U, 37580U, 19979U, 19121U, 64459U, 43515U, 22974U, 46129U, 50629U, 50728U, 16749U, 1984U, 36593U, 23506U, 32533U, 15518U, 24376U, 44185U, 13966U, 1329U, 33053U, 32408U, 44819U, 6039U, 18814U, 55261U, 59866U, 26909U, 23217U, 1076U, 45388U, 7019U, 13178U, 22731U, 26633U, 33765U, 13409U, 9576U, 40764U, 55157U, 48979U, 43310U, 33423U, 48773U, 39577U, 28683U, 8579U, 13058U, 42510U, 26854U, 4668U, 7655U, 51745U, 8260U, 33845U, 24846U, 11576U, 16141U, 9885U, 45284U, 10708U, 6039U, 46779U, 2642U, 38016U, 44609U, 6539U, 2697U, 64998U, 24463U, 41143U, 49235U, 29615U, 38261U, 23869U, 46694U, 4145U, 59833U, 47981U, 30908U, 45577U, 40413U, 19978U, 38225U, 65402U, 19771U, 61052U, 17629U, 16202U, 43956U, 57716U, 45896U, 41800U, 49843U, 23736U, 19483U, 41697U, 34570U, 37539U, 21106U, 50486U, 28308U, 51775U, 4702U, 15211U, 53920U, 64002U, 38737U, 62205U, 54779U, 61905U, 556U, 51712U, 20575U, 49265U, 56481U, 55309U, 49151U, 47829U, 38096U, 9914U, 42633U, 51409U, 3630U, 22188U, 29538U, 10883U, 53798U, 6907U, 22059U, 42055U, 29752U, 44851U, 23812U, 9606U, 5964U, 2022U, 26371U, 14645U, 1151U, 63373U, 40610U, 60033U, 63934U, 55485U, 33788U, 43792U, 24170U, 38220U, 17767U, 20469U, 40261U, 14416U, 59977U, 17920U, 4716U, 28243U, 41224U, 31972U, 54547U, 6818U, 40094U, 5102U, 430U, 34878U, 62416U, 20084U, 13177U, 60718U, 33005U, 3521U, 9054U, 32100U, 21504U, 43679U, 31852U, 9857U, 19721U, 36451U, 55730U, 52333U, 11272U, 37892U, 3379U, 51144U, 479U, 41384U, 47542U, 32558U, 31196U, 34410U, 49056U, 1047U, 20935U, 41894U, 49808U, 64495U, 36913U, 18805U, 57374U, 59008U, 3677U, 23160U, 37497U, 19885U, 35503U, 4233U, 38501U, 991U, 1574U, 45113U, 11943U, 51473U, 3682U, 19354U, 34572U, 40587U, 44007U, 40502U, 29652U, 28054U, 45030U, 23771U, 15226U, 26336U, 5168U, 57285U, 34879U, 39081U, 26103U, 15004U, 48766U, 23828U, 16205U, 25101U, 18616U, 37156U, 34674U, 42001U, 20104U, 23991U, 2328U, 51857U, 55692U, 38595U, 7837U, 7860U, 57170U, 3115U, 34413U, 36358U, 7413U, 9539U, 9045U, 34293U, 43494U, 27472U, 13493U, 55037U, 65251U, 24683U, 54828U, 38516U, 6532U, 45763U, 16049U, 33579U, 47857U, 33030U, 36505U, 18795U, 30736U, 36516U, 5103U, 20478U, 35721U, 58877U, 39991U, 22267U, 48184U, 41166U, 63912U, 21342U, 58195U, 10869U, 64453U, 61677U, 25143U, 2960U, 13498U, 38130U, 11031U, 55364U, 18390U, 45786U, 1168U, 9773U, 59789U, 42048U, 35190U, 61698U, 40656U, 39295U, 39932U, 8135U, 16185U, 28144U, 29791U, 64615U, 47315U, 27695U, 12374U, 47676U, 15740U, 27525U, 3067U, 47375U, 57333U, 44640U, 56473U, 60103U, 3385U, 32749U, 15000U, 1632U, 4833U, 4470U, 2593U, 55291U, 51472U, 13302U, 42861U, 19975U, 25026U, 60645U, 53124U, 52462U, 6192U, 6850U, 21328U, 26719U, 42016U, 22179U, 49629U, 30581U, 33197U, 1931U, 18536U, 37920U, 43888U, 3222U, 31770U, 49132U, 56596U, 59781U, 25617U, 46183U, 687U, 21725U, 38795U, 5026U, 56415U, 25361U, 44390U, 44949U, 26223U, 31146U, 11153U, 6521U, 40475U, 47143U, 43863U, 65307U, 54419U, 15069U, 53554U, 47509U, 48880U, 57559U, 44075U, 25327U, 37417U, 27704U, 64474U, 20926U, 65213U, 37650U, 32538U, 53606U, 39286U, 44940U, 55153U, 47069U, 36864U, 13444U, 18994U, 63021U, 52146U, 38296U, 25072U, 7509U, 47715U, 4453U, 38986U, 11444U, 709U, 63052U, 4594U, 53014U, 38210U, 6267U, 53281U, 34847U, 37576U, 24322U, 24767U, 36901U, 25939U, 29123U, 31203U, 62087U, 64684U, 19001U, 52218U, 43719U, 48539U, 1537U, 49502U, 1871U, 55147U, 37426U, 33138U, 39547U, 666U, 6940U, 40061U, 19787U, 46302U, 21137U, 54440U, 26718U, 24174U, 6348U, 50083U, 34998U, 31709U, 6439U, 31743U, 4446U, 15869U, 35354U, 12911U, 62278U, 41255U, 53416U, 44048U, 27517U, 49143U, 43322U, 17222U, 62075U, 16740U, 41247U, 23610U, 17770U, 16981U, 14697U, 28984U, 41504U, 23943U, 44309U, 24178U, 19674U, 48966U, 25245U, 12029U, 20608U, 40464U, 44128U, 54153U, 8891U, 27643U, 28674U, 54930U, 739U, 17849U, 54101U, 60088U, 3100U, 25817U, 51161U, 13646U, 59864U, 31178U, 65140U, 25330U, 7332U, 34299U, 61356U, 16562U, 62024U, 53533U, 42700U, 12886U, 27080U, 54515U, 63309U, 12148U, 49272U, 9335U, 265U, 11679U, 23895U, 60300U, 39845U, 9866U, 32805U, 32020U, 42168U, 4905U, 65355U, 4998U, 27724U, 23548U, 61229U, 55788U, 53399U, 40391U, 28349U, 62469U, 53609U, 11610U, 25221U, 35413U, 38293U, 53205U, 9116U, 52987U, 38819U, 56834U, 47571U, 28537U, 39321U, 19173U, 35678U, 32132U, 44898U, 26876U, 32473U, 31961U, 32795U, 19675U, 45989U, 10116U, 61297U, 38804U, 59051U, 20708U, 62255U, 25322U, 19477U, 58474U, 18056U, 40009U, 43370U, 64405U, 28707U, 6860U, 5199U, 55587U, 56237U, 46885U, 20313U, 7428U, 63529U, 52064U, 32708U, 65241U, 36705U, 13836U, 52878U, 20477U, 1235U, 24152U, 12606U, 59203U, 53647U, 36512U, 27587U, 27215U, 16622U, 19060U, 29388U, 22160U, 15983U, 14837U, 53728U, 5014U, 23820U, 38214U, 20577U, 7804U, 38861U, 48222U, 13528U, 61488U, 54868U, 26335U, 23426U, 59603U, 56802U, 60434U, 31643U, 19859U, 29316U, 4278U, 52737U, 7609U, 17881U, 20495U, 61352U, 26379U, 29538U, 27113U, 29210U, 17128U, 63990U, 2001U, 28382U, 15021U, 50417U, 21659U, 50337U, 17459U, 8520U, 55296U, 19431U, 47071U, 16007U, 16161U, 39894U, 64975U, 18788U, 57886U, 17608U, 36063U, 40442U, 47768U, 57700U, 65019U, 51259U, 51368U, 46354U, 34026U, 11884U, 53008U, 26029U, 41126U, 286U, 43203U, 2449U, 35183U, 42576U, 20210U, 41388U, 53360U, 6827U, 17256U, 50346U, 42036U, 53317U, 38405U, 62174U, 8352U, 40245U, 38215U, 9877U, 7175U, 45508U, 9623U, 3251U, 50929U, 3253U, 11931U, 40441U, 9043U, 30193U, 8370U, 34244U, 38417U, 28142U, 26937U, 60940U, 15296U, 23860U, 15114U, 44405U, 33764U, 11014U, 28825U, 14991U, 61787U, 37269U, 15979U, 49762U, 19045U, 60979U, 62533U, 58696U, 55923U, 6753U, 50257U, 27570U, 21014U, 2339U, 10215U, 5828U, 25841U, 29304U, 44818U, 55566U, 42500U, 53491U, 1366U, 31644U, 8152U, 18503U, 28472U, 63278U, 58052U, 32109U, 18182U, 37164U, 32448U, 13920U, 31663U, 56735U, 26691U, 47731U, 57680U, 14495U, 38961U, 52859U, 12108U, 22832U, 21709U, 38031U, 1265U, 29939U, 27284U, 49620U, 21236U, 54671U, 36659U, 45625U, 37072U, 8094U, 865U, 57073U, 62387U, 30165U, 50027U, 61199U, 62737U, 53631U, 64172U, 25740U, 829U, 25796U, 34305U, 4169U, 64823U, 25129U, 9951U, 30047U, 44793U, 64302U, 4234U, 63120U, 54767U, 55073U, 11453U, 52627U, 23719U, 21650U, 9970U, 19873U, 23065U, 65521U, 44865U, 61892U, 20096U, 3625U, 64909U, 21530U, 20484U, 8533U, 13202U, 4128U, 38406U, 64543U, 22458U, 38781U, 13453U, 8604U, 11727U, 61072U, 25502U, 63957U, 46451U, 14517U, 34640U, 40236U, 46169U, 55222U, 14875U, 10760U, 10818U, 10674U, 25016U, 5306U, 18089U, 53633U, 35119U, 841U, 7557U, 29472U, 33121U, 63121U, 50604U, 42632U, 33129U, 30855U, 7363U, 37539U, 13255U, 23880U, 55622U, 32590U, 22986U, 29864U, 1299U, 57544U, 1041U, 57283U, 63773U, 26804U, 6075U, 31894U, 63114U, 35669U, 20465U, 28511U, 56546U, 22901U, 6419U, 57756U, 55835U, 36648U, 45722U, 25741U, 1572U, 17320U, 4587U, 52091U, 50032U, 64467U, 12476U, 11588U, 7817U, 65246U, 58543U, 57067U, 17172U, 49308U, 13198U, 49989U, 60971U, 26134U, 51977U, 37783U, 10714U, 51957U, 1990U, 58708U, 12593U, 23797U, 55700U, 57607U, 15733U, 35652U, 17206U, 38382U, 58197U, 46522U, 22639U, 46709U, 17338U, 29900U, 45305U, 51361U, 16461U, 38879U, 38274U, 34572U, 9558U, 41880U, 26615U, 3404U, 54527U, 59927U, 8381U, 63344U, 46772U, 13907U, 51307U, 4030U, 16535U, 5644U, 602U, 19631U, 16087U, 7954U, 64282U, 2608U, 40657U, 4531U, 27069U, 52312U, 58972U, 14924U, 8432U, 65499U, 60724U, 36658U, 51641U, 31697U, 45047U, 7385U, 16815U, 27129U, 43354U, 42990U, 16135U, 31890U, 12858U, 16596U, 15495U, 24636U))
}

fun onSensorsData(p9: org.company.Client.SensorsData) {
    assert(p9.values().same(ushortArrayOf(2998U, 31673U, 19893U, 8089U, 23213U, 51676U, 18008U, 52147U, 19088U, 41803U, 65507U, 28694U, 51159U, 52701U, 53415U, 38420U, 64275U, 29305U, 9312U, 37626U, 36413U, 35896U, 44752U, 27262U, 48558U, 51902U, 57328U, 37091U, 17044U, 52340U, 12786U, 49269U, 18609U, 40465U, 44419U, 27268U, 40935U, 61019U, 27946U, 54343U, 55131U, 15301U, 12547U, 8983U, 1740U, 55733U, 11466U, 59591U, 29320U, 63264U, 44842U, 10984U, 37580U, 19979U, 19121U, 64459U, 43515U, 22974U, 46129U, 50629U, 50728U, 16749U, 1984U, 36593U, 23506U, 32533U, 15518U, 24376U, 44185U, 13966U, 1329U, 33053U, 32408U, 44819U, 6039U, 18814U, 55261U, 59866U, 26909U, 23217U, 1076U, 45388U, 7019U, 13178U, 22731U, 26633U, 33765U, 13409U, 9576U, 40764U, 55157U, 48979U, 43310U, 33423U, 48773U, 39577U, 28683U, 8579U, 13058U, 42510U, 26854U, 4668U, 7655U, 51745U, 8260U, 33845U, 24846U, 11576U, 16141U, 9885U, 45284U, 10708U, 6039U, 46779U, 2642U, 38016U, 44609U, 6539U, 2697U, 64998U, 24463U, 41143U, 49235U, 29615U, 38261U, 23869U, 46694U, 4145U, 59833U, 47981U, 30908U, 45577U, 40413U, 19978U, 38225U, 65402U, 19771U, 61052U, 17629U, 16202U, 43956U, 57716U, 45896U, 41800U, 49843U, 23736U, 19483U, 41697U, 34570U, 37539U, 21106U, 50486U, 28308U, 51775U, 4702U, 15211U, 53920U, 64002U, 38737U, 62205U, 54779U, 61905U, 556U, 51712U, 20575U, 49265U, 56481U, 55309U, 49151U, 47829U, 38096U, 9914U, 42633U, 51409U, 3630U, 22188U, 29538U, 10883U, 53798U, 6907U, 22059U, 42055U, 29752U, 44851U, 23812U, 9606U, 5964U, 2022U, 26371U, 14645U, 1151U, 63373U, 40610U, 60033U, 63934U, 55485U, 33788U, 43792U, 24170U, 38220U, 17767U, 20469U, 40261U, 14416U, 59977U, 17920U, 4716U, 28243U, 41224U, 31972U, 54547U, 6818U, 40094U, 5102U, 430U, 34878U, 62416U, 20084U, 13177U, 60718U, 33005U, 3521U, 9054U, 32100U, 21504U, 43679U, 31852U, 9857U, 19721U, 36451U, 55730U, 52333U, 11272U, 37892U, 3379U, 51144U, 479U, 41384U, 47542U, 32558U, 31196U, 34410U, 49056U, 1047U, 20935U, 41894U, 49808U, 64495U, 36913U, 18805U, 57374U, 59008U, 3677U, 23160U, 37497U, 19885U, 35503U, 4233U, 38501U, 991U, 1574U, 45113U, 11943U, 51473U, 3682U, 19354U, 34572U, 40587U, 44007U, 40502U, 29652U, 28054U, 45030U, 23771U, 15226U, 26336U, 5168U, 57285U, 34879U, 39081U, 26103U, 15004U, 48766U, 23828U, 16205U, 25101U, 18616U, 37156U, 34674U, 42001U, 20104U, 23991U, 2328U, 51857U, 55692U, 38595U, 7837U, 7860U, 57170U, 3115U, 34413U, 36358U, 7413U, 9539U, 9045U, 34293U, 43494U, 27472U, 13493U, 55037U, 65251U, 24683U, 54828U, 38516U, 6532U, 45763U, 16049U, 33579U, 47857U, 33030U, 36505U, 18795U, 30736U, 36516U, 5103U, 20478U, 35721U, 58877U, 39991U, 22267U, 48184U, 41166U, 63912U, 21342U, 58195U, 10869U, 64453U, 61677U, 25143U, 2960U, 13498U, 38130U, 11031U, 55364U, 18390U, 45786U, 1168U, 9773U, 59789U, 42048U, 35190U, 61698U, 40656U, 39295U, 39932U, 8135U, 16185U, 28144U, 29791U, 64615U, 47315U, 27695U, 12374U, 47676U, 15740U, 27525U, 3067U, 47375U, 57333U, 44640U, 56473U, 60103U, 3385U, 32749U, 15000U, 1632U, 4833U, 4470U, 2593U, 55291U, 51472U, 13302U, 42861U, 19975U, 25026U, 60645U, 53124U, 52462U, 6192U, 6850U, 21328U, 26719U, 42016U, 22179U, 49629U, 30581U, 33197U, 1931U, 18536U, 37920U, 43888U, 3222U, 31770U, 49132U, 56596U, 59781U, 25617U, 46183U, 687U, 21725U, 38795U, 5026U, 56415U, 25361U, 44390U, 44949U, 26223U, 31146U, 11153U, 6521U, 40475U, 47143U, 43863U, 65307U, 54419U, 15069U, 53554U, 47509U, 48880U, 57559U, 44075U, 25327U, 37417U, 27704U, 64474U, 20926U, 65213U, 37650U, 32538U, 53606U, 39286U, 44940U, 55153U, 47069U, 36864U, 13444U, 18994U, 63021U, 52146U, 38296U, 25072U, 7509U, 47715U, 4453U, 38986U, 11444U, 709U, 63052U, 4594U, 53014U, 38210U, 6267U, 53281U, 34847U, 37576U, 24322U, 24767U, 36901U, 25939U, 29123U, 31203U, 62087U, 64684U, 19001U, 52218U, 43719U, 48539U, 1537U, 49502U, 1871U, 55147U, 37426U, 33138U, 39547U, 666U, 6940U, 40061U, 19787U, 46302U, 21137U, 54440U, 26718U, 24174U, 6348U, 50083U, 34998U, 31709U, 6439U, 31743U, 4446U, 15869U, 35354U, 12911U, 62278U, 41255U, 53416U, 44048U, 27517U, 49143U, 43322U, 17222U, 62075U, 16740U, 41247U, 23610U, 17770U, 16981U, 14697U, 28984U, 41504U, 23943U, 44309U, 24178U, 19674U, 48966U, 25245U, 12029U, 20608U, 40464U, 44128U, 54153U, 8891U, 27643U, 28674U, 54930U, 739U, 17849U, 54101U, 60088U, 3100U, 25817U, 51161U, 13646U, 59864U, 31178U, 65140U, 25330U, 7332U, 34299U, 61356U, 16562U, 62024U, 53533U, 42700U, 12886U, 27080U, 54515U, 63309U, 12148U, 49272U, 9335U, 265U, 11679U, 23895U, 60300U, 39845U, 9866U, 32805U, 32020U, 42168U, 4905U, 65355U, 4998U, 27724U, 23548U, 61229U, 55788U, 53399U, 40391U, 28349U, 62469U, 53609U, 11610U, 25221U, 35413U, 38293U, 53205U, 9116U, 52987U, 38819U, 56834U, 47571U, 28537U, 39321U, 19173U, 35678U, 32132U, 44898U, 26876U, 32473U, 31961U, 32795U, 19675U, 45989U, 10116U, 61297U, 38804U, 59051U, 20708U, 62255U, 25322U, 19477U, 58474U, 18056U, 40009U, 43370U, 64405U, 28707U, 6860U, 5199U, 55587U, 56237U, 46885U, 20313U, 7428U, 63529U, 52064U, 32708U, 65241U, 36705U, 13836U, 52878U, 20477U, 1235U, 24152U, 12606U, 59203U, 53647U, 36512U, 27587U, 27215U, 16622U, 19060U, 29388U, 22160U, 15983U, 14837U, 53728U, 5014U, 23820U, 38214U, 20577U, 7804U, 38861U, 48222U, 13528U, 61488U, 54868U, 26335U, 23426U, 59603U, 56802U, 60434U, 31643U, 19859U, 29316U, 4278U, 52737U, 7609U, 17881U, 20495U, 61352U, 26379U, 29538U, 27113U, 29210U, 17128U, 63990U, 2001U, 28382U, 15021U, 50417U, 21659U, 50337U, 17459U, 8520U, 55296U, 19431U, 47071U, 16007U, 16161U, 39894U, 64975U, 18788U, 57886U, 17608U, 36063U, 40442U, 47768U, 57700U, 65019U, 51259U, 51368U, 46354U, 34026U, 11884U, 53008U, 26029U, 41126U, 286U, 43203U, 2449U, 35183U, 42576U, 20210U, 41388U, 53360U, 6827U, 17256U, 50346U, 42036U, 53317U, 38405U, 62174U, 8352U, 40245U, 38215U, 9877U, 7175U, 45508U, 9623U, 3251U, 50929U, 3253U, 11931U, 40441U, 9043U, 30193U, 8370U, 34244U, 38417U, 28142U, 26937U, 60940U, 15296U, 23860U, 15114U, 44405U, 33764U, 11014U, 28825U, 14991U, 61787U, 37269U, 15979U, 49762U, 19045U, 60979U, 62533U, 58696U, 55923U, 6753U, 50257U, 27570U, 21014U, 2339U, 10215U, 5828U, 25841U, 29304U, 44818U, 55566U, 42500U, 53491U, 1366U, 31644U, 8152U, 18503U, 28472U, 63278U, 58052U, 32109U, 18182U, 37164U, 32448U, 13920U, 31663U, 56735U, 26691U, 47731U, 57680U, 14495U, 38961U, 52859U, 12108U, 22832U, 21709U, 38031U, 1265U, 29939U, 27284U, 49620U, 21236U, 54671U, 36659U, 45625U, 37072U, 8094U, 865U, 57073U, 62387U, 30165U, 50027U, 61199U, 62737U, 53631U, 64172U, 25740U, 829U, 25796U, 34305U, 4169U, 64823U, 25129U, 9951U, 30047U, 44793U, 64302U, 4234U, 63120U, 54767U, 55073U, 11453U, 52627U, 23719U, 21650U, 9970U, 19873U, 23065U, 65521U, 44865U, 61892U, 20096U, 3625U, 64909U, 21530U, 20484U, 8533U, 13202U, 4128U, 38406U, 64543U, 22458U, 38781U, 13453U, 8604U, 11727U, 61072U, 25502U, 63957U, 46451U, 14517U, 34640U, 40236U, 46169U, 55222U, 14875U, 10760U, 10818U, 10674U, 25016U, 5306U, 18089U, 53633U, 35119U, 841U, 7557U, 29472U, 33121U, 63121U, 50604U, 42632U, 33129U, 30855U, 7363U, 37539U, 13255U, 23880U, 55622U, 32590U, 22986U, 29864U, 1299U, 57544U, 1041U, 57283U, 63773U, 26804U, 6075U, 31894U, 63114U, 35669U, 20465U, 28511U, 56546U, 22901U, 6419U, 57756U, 55835U, 36648U, 45722U, 25741U, 1572U, 17320U, 4587U, 52091U, 50032U, 64467U, 12476U, 11588U, 7817U, 65246U, 58543U, 57067U, 17172U, 49308U, 13198U, 49989U, 60971U, 26134U, 51977U, 37783U, 10714U, 51957U, 1990U, 58708U, 12593U, 23797U, 55700U, 57607U, 15733U, 35652U, 17206U, 38382U, 58197U, 46522U, 22639U, 46709U, 17338U, 29900U, 45305U, 51361U, 16461U, 38879U, 38274U, 34572U, 9558U, 41880U, 26615U, 3404U, 54527U, 59927U, 8381U, 63344U, 46772U, 13907U, 51307U, 4030U, 16535U, 5644U, 602U, 19631U, 16087U, 7954U, 64282U, 2608U, 40657U, 4531U, 27069U, 52312U, 58972U, 14924U, 8432U, 65499U, 60724U, 36658U, 51641U, 31697U, 45047U, 7385U, 16815U, 27129U, 43354U, 42990U, 16135U, 31890U, 12858U, 16596U, 15495U, 24636U)))

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
