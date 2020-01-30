
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
    p0.DevID((43375U).toUShort())
    p0.Ver((62686U).toUShort())
}

fun onDeviceVersion(p0: org.company.Client.DeviceVersion) {
    assert(p0.DevID() == (43375U).toUShort())
    assert(p0.Ver() == (62686U).toUShort())
    println("DeviceVersion \n")
}
fun onStop() { println("Stop \n") }
fun onStart() { println("Start \n") }
fun onGetDeviceVersion() { println("GetDeviceVersion \n") }
fun onGetConfiguration() { println("GetConfiguration \n") }
fun onSetConfiguration() { println("SetConfiguration \n") }

fun fill(p6: org.company.Client.BusConfiguration) {
    p6.multiplier((64U).toUByte())
    p6.time((28983U).toUShort())
    p6.clk_khz((45544U).toUShort())
}

fun onBusConfiguration(p6: org.company.Client.BusConfiguration) {
    assert(p6.multiplier() == (64U).toUByte())
    assert(p6.time() == (28983U).toUShort())
    assert(p6.clk_khz() == (45544U).toUShort())
    println("BusConfiguration \n")
}

fun fill(p7: org.company.Client.InstructionsPack) {
    p7.Length((108U).toUByte())
    p7.Instructions(ubyteArrayOf(54U, 172U, 50U, 243U, 185U, 44U, 107U, 241U, 168U, 250U, 83U, 153U, 5U, 15U, 25U, 179U, 238U, 19U, 41U, 64U, 182U, 88U, 60U, 70U, 225U, 156U, 90U, 219U, 78U, 145U, 62U, 14U, 140U, 41U, 115U, 126U, 155U, 245U, 16U, 77U, 250U, 79U, 98U, 180U, 227U, 208U, 134U, 92U, 172U, 137U, 115U, 96U, 253U, 123U, 98U, 172U, 72U, 130U, 219U, 57U, 11U, 245U, 113U, 56U, 194U, 155U, 72U, 224U, 55U, 150U, 209U, 249U, 233U, 82U, 25U, 84U, 234U, 246U, 165U, 246U, 134U, 82U, 100U, 108U, 155U, 205U, 242U, 249U, 243U, 152U, 246U, 75U, 127U, 219U, 193U, 243U, 146U, 158U, 145U, 240U, 146U, 211U, 235U, 94U, 242U, 253U, 128U, 97U, 177U, 187U, 190U, 149U, 83U, 173U, 238U, 62U, 18U, 143U, 185U, 58U, 176U, 51U, 110U, 171U, 245U, 207U, 70U, 166U, 13U, 101U, 57U, 50U, 177U, 105U, 111U, 153U, 202U, 98U, 15U, 214U, 244U, 42U, 176U, 250U, 148U, 18U, 23U, 39U, 243U, 124U, 35U, 14U, 5U, 86U, 191U, 64U, 73U, 197U, 223U, 173U, 43U, 91U, 127U, 207U, 26U, 129U, 30U, 135U, 197U, 242U, 80U, 224U, 107U, 152U, 200U, 115U, 197U, 60U, 35U, 198U, 124U, 150U, 196U, 68U, 197U, 18U, 225U, 116U, 77U, 139U, 225U, 243U, 87U, 66U, 188U, 103U, 44U, 2U, 186U, 28U, 203U, 85U, 172U, 202U, 1U, 33U, 149U, 182U, 212U, 205U, 94U, 63U, 202U, 69U, 140U, 82U, 172U, 251U, 120U, 32U, 54U, 162U, 249U, 77U, 170U, 46U, 215U, 248U, 147U, 167U, 160U, 9U, 34U, 113U, 49U, 26U, 230U, 64U, 55U, 185U, 101U, 66U, 111U, 203U, 204U, 62U, 184U, 1U, 143U, 187U, 61U, 155U, 7U, 48U, 95U, 13U))
}

fun onInstructionsPack(p7: org.company.Client.InstructionsPack) {
    assert(p7.Length() == (108U).toUByte())

    assert(p7.Instructions().same(ubyteArrayOf(54U, 172U, 50U, 243U, 185U, 44U, 107U, 241U, 168U, 250U, 83U, 153U, 5U, 15U, 25U, 179U, 238U, 19U, 41U, 64U, 182U, 88U, 60U, 70U, 225U, 156U, 90U, 219U, 78U, 145U, 62U, 14U, 140U, 41U, 115U, 126U, 155U, 245U, 16U, 77U, 250U, 79U, 98U, 180U, 227U, 208U, 134U, 92U, 172U, 137U, 115U, 96U, 253U, 123U, 98U, 172U, 72U, 130U, 219U, 57U, 11U, 245U, 113U, 56U, 194U, 155U, 72U, 224U, 55U, 150U, 209U, 249U, 233U, 82U, 25U, 84U, 234U, 246U, 165U, 246U, 134U, 82U, 100U, 108U, 155U, 205U, 242U, 249U, 243U, 152U, 246U, 75U, 127U, 219U, 193U, 243U, 146U, 158U, 145U, 240U, 146U, 211U, 235U, 94U, 242U, 253U, 128U, 97U, 177U, 187U, 190U, 149U, 83U, 173U, 238U, 62U, 18U, 143U, 185U, 58U, 176U, 51U, 110U, 171U, 245U, 207U, 70U, 166U, 13U, 101U, 57U, 50U, 177U, 105U, 111U, 153U, 202U, 98U, 15U, 214U, 244U, 42U, 176U, 250U, 148U, 18U, 23U, 39U, 243U, 124U, 35U, 14U, 5U, 86U, 191U, 64U, 73U, 197U, 223U, 173U, 43U, 91U, 127U, 207U, 26U, 129U, 30U, 135U, 197U, 242U, 80U, 224U, 107U, 152U, 200U, 115U, 197U, 60U, 35U, 198U, 124U, 150U, 196U, 68U, 197U, 18U, 225U, 116U, 77U, 139U, 225U, 243U, 87U, 66U, 188U, 103U, 44U, 2U, 186U, 28U, 203U, 85U, 172U, 202U, 1U, 33U, 149U, 182U, 212U, 205U, 94U, 63U, 202U, 69U, 140U, 82U, 172U, 251U, 120U, 32U, 54U, 162U, 249U, 77U, 170U, 46U, 215U, 248U, 147U, 167U, 160U, 9U, 34U, 113U, 49U, 26U, 230U, 64U, 55U, 185U, 101U, 66U, 111U, 203U, 204U, 62U, 184U, 1U, 143U, 187U, 61U, 155U, 7U, 48U, 95U, 13U)))

    println("InstructionsPack \n")
}

fun fill(p8: test_.DeviceError) {
    p8.param1((64929U).toUShort())
    p8.error_id(Errors.CannotReadSensor)
}

fun onDeviceError(p8: org.company.Client.DeviceError) {
    assert(p8.param1() == (64929U).toUShort())
    assert(p8.error_id() == Errors.CannotReadSensor)
    println("DeviceError \n")
}

fun fill(p9: test_.SensorsData) {
    p9.values(ushortArrayOf(26079U, 54262U, 28485U, 60668U, 56244U, 9996U, 2178U, 50726U, 60381U, 38777U, 50684U, 20843U, 60170U, 57999U, 2940U, 13311U, 773U, 22546U, 32435U, 18916U, 53993U, 62104U, 4151U, 14298U, 15310U, 23231U, 7435U, 27327U, 58487U, 36031U, 64726U, 54977U, 16270U, 51553U, 2152U, 39523U, 40632U, 9323U, 13699U, 23934U, 30667U, 38645U, 35985U, 17987U, 8081U, 15755U, 38400U, 3327U, 24362U, 10734U, 18266U, 7315U, 44941U, 37487U, 11866U, 22085U, 4597U, 28744U, 48019U, 44064U, 23392U, 2350U, 5135U, 7208U, 64452U, 24399U, 18149U, 51771U, 16952U, 55274U, 32343U, 11607U, 45030U, 55191U, 52791U, 44201U, 64596U, 55481U, 54435U, 4187U, 6977U, 60314U, 55897U, 34698U, 18072U, 50852U, 9934U, 39116U, 27134U, 32433U, 47446U, 49878U, 63295U, 65358U, 56596U, 40409U, 58634U, 2139U, 6833U, 25960U, 61851U, 43498U, 50479U, 51776U, 30753U, 45397U, 13990U, 22085U, 64757U, 5905U, 33176U, 27359U, 27245U, 46851U, 5751U, 279U, 23387U, 6555U, 568U, 52412U, 26537U, 53524U, 25083U, 24745U, 54232U, 27605U, 57756U, 42488U, 31586U, 29043U, 50345U, 20075U, 34035U, 46656U, 30493U, 32281U, 19763U, 7194U, 59410U, 25955U, 50633U, 49269U, 56629U, 63574U, 55486U, 9683U, 10582U, 64996U, 3897U, 27985U, 46901U, 50292U, 49407U, 27548U, 65192U, 11211U, 55351U, 2286U, 57480U, 1093U, 47683U, 57456U, 12859U, 41742U, 42715U, 8539U, 54005U, 62400U, 32307U, 59951U, 16451U, 50224U, 41594U, 43070U, 33948U, 62926U, 39643U, 51688U, 12508U, 12078U, 26601U, 22053U, 19108U, 50776U, 23704U, 55216U, 61840U, 2546U, 29155U, 12265U, 42439U, 47773U, 27528U, 5080U, 30936U, 26743U, 65337U, 3469U, 9318U, 62657U, 55969U, 4277U, 21183U, 27829U, 41039U, 39285U, 65298U, 41970U, 14951U, 16541U, 47327U, 54874U, 27829U, 62983U, 53900U, 20586U, 59322U, 27324U, 59577U, 27802U, 32135U, 21205U, 62333U, 60965U, 39328U, 32029U, 39987U, 25622U, 23366U, 30118U, 8348U, 64396U, 48375U, 36788U, 50098U, 12371U, 33573U, 11795U, 21584U, 15454U, 40256U, 30236U, 33125U, 18285U, 38560U, 49595U, 39552U, 43300U, 48340U, 49093U, 52695U, 12192U, 42868U, 9497U, 53284U, 52207U, 5874U, 50065U, 52345U, 1106U, 41691U, 18996U, 4203U, 30480U, 52564U, 23457U, 46765U, 30105U, 63338U, 63619U, 58100U, 51484U, 34778U, 46041U, 50574U, 33518U, 24443U, 9616U, 51833U, 27756U, 42133U, 26062U, 43948U, 17822U, 41075U, 25856U, 5188U, 25434U, 47863U, 14537U, 20806U, 51245U, 64406U, 10582U, 54708U, 4353U, 47877U, 10370U, 17676U, 6195U, 11103U, 40806U, 8180U, 36990U, 20291U, 1690U, 11357U, 44622U, 24682U, 3734U, 35008U, 62488U, 58564U, 8315U, 64989U, 46260U, 2412U, 61719U, 34333U, 42558U, 16188U, 47617U, 35833U, 16460U, 13759U, 48669U, 36549U, 26556U, 824U, 3383U, 45263U, 26250U, 48315U, 53309U, 23681U, 5145U, 33609U, 24745U, 54243U, 43086U, 61671U, 45881U, 5567U, 8943U, 21582U, 50505U, 19313U, 63391U, 26676U, 8005U, 2574U, 31709U, 6412U, 377U, 13668U, 22936U, 4388U, 19241U, 7154U, 32044U, 38393U, 39253U, 38562U, 15287U, 50175U, 6545U, 16055U, 7734U, 2355U, 24924U, 22826U, 8793U, 45928U, 35991U, 4348U, 3356U, 28432U, 4608U, 46943U, 26108U, 39991U, 49559U, 38335U, 14857U, 32467U, 54927U, 42813U, 31389U, 56879U, 55398U, 6323U, 33879U, 11491U, 36006U, 28959U, 9497U, 2986U, 58271U, 31591U, 60719U, 42228U, 5361U, 54534U, 58362U, 57711U, 20940U, 30623U, 60543U, 46877U, 9814U, 56103U, 4707U, 23790U, 2105U, 32381U, 14417U, 60635U, 57336U, 48800U, 33257U, 56730U, 25185U, 57450U, 17886U, 6899U, 21446U, 55710U, 3458U, 40256U, 45553U, 5333U, 4842U, 32235U, 21818U, 51512U, 44233U, 35542U, 54778U, 37476U, 9339U, 64618U, 2915U, 9533U, 34432U, 51827U, 23439U, 64439U, 23947U, 60803U, 63781U, 64134U, 19224U, 10160U, 31490U, 31738U, 52646U, 18111U, 65436U, 57218U, 40998U, 8093U, 1295U, 53983U, 20975U, 61881U, 9039U, 27416U, 53101U, 13729U, 48750U, 36984U, 38183U, 58076U, 53326U, 46763U, 28706U, 33407U, 32238U, 40918U, 65340U, 34409U, 56467U, 43989U, 54061U, 13577U, 6860U, 18323U, 33551U, 51985U, 21636U, 34277U, 14471U, 65204U, 31492U, 6083U, 37234U, 13061U, 61390U, 23715U, 24539U, 33884U, 35656U, 53072U, 9981U, 7368U, 41000U, 35379U, 53084U, 47653U, 43863U, 15347U, 17470U, 38205U, 60836U, 48145U, 517U, 35267U, 11429U, 1986U, 38298U, 64822U, 5202U, 6290U, 18192U, 12829U, 48028U, 58523U, 64158U, 4119U, 52625U, 12840U, 61155U, 14996U, 41702U, 33794U, 50911U, 53955U, 48109U, 51115U, 51597U, 17826U, 38098U, 47731U, 16893U, 13467U, 56081U, 14787U, 10200U, 48240U, 17971U, 52957U, 60553U, 12971U, 63278U, 40577U, 57308U, 22828U, 42232U, 1488U, 41116U, 32827U, 36155U, 24958U, 52447U, 18868U, 38826U, 43683U, 20093U, 25832U, 44341U, 28692U, 22783U, 11766U, 7697U, 54438U, 51986U, 50474U, 40104U, 58091U, 31529U, 8053U, 4773U, 56457U, 14930U, 24451U, 4747U, 43730U, 28671U, 17752U, 63184U, 21309U, 8481U, 4657U, 11893U, 56933U, 18252U, 47654U, 52866U, 22550U, 59850U, 34503U, 39025U, 4298U, 2639U, 37275U, 12172U, 34688U, 41948U, 43848U, 27098U, 39607U, 44784U, 40240U, 45108U, 16464U, 17195U, 36044U, 42432U, 2854U, 42663U, 15889U, 2277U, 22459U, 26136U, 34434U, 32855U, 4138U, 40586U, 16270U, 11647U, 41144U, 414U, 35318U, 15037U, 13471U, 23633U, 54977U, 5028U, 43340U, 12842U, 2711U, 45103U, 34936U, 51183U, 26850U, 55935U, 15321U, 3888U, 22245U, 25766U, 27357U, 3861U, 25942U, 52348U, 33783U, 19627U, 31109U, 33469U, 59498U, 54189U, 56610U, 57148U, 61279U, 38545U, 43727U, 34170U, 51017U, 16604U, 56121U, 525U, 49322U, 11514U, 14647U, 16874U, 60873U, 1667U, 31300U, 59073U, 11146U, 6972U, 9262U, 4868U, 38034U, 37237U, 51456U, 14847U, 34139U, 51821U, 53492U, 30818U, 11778U, 29949U, 63355U, 56151U, 10941U, 60331U, 18805U, 41577U, 64423U, 4849U, 4487U, 58083U, 33607U, 57815U, 46742U, 41124U, 8955U, 23441U, 42900U, 50944U, 15361U, 1556U, 23890U, 51269U, 54330U, 36893U, 53132U, 61583U, 41936U, 45179U, 18676U, 31132U, 40200U, 35705U, 22497U, 23470U, 56804U, 7194U, 42577U, 59212U, 6505U, 31445U, 43488U, 57105U, 53929U, 39665U, 60220U, 16614U, 28839U, 63450U, 32148U, 18360U, 4805U, 29231U, 52886U, 62429U, 40688U, 63720U, 30282U, 28696U, 41737U, 35012U, 51160U, 32537U, 41159U, 13241U, 55682U, 50406U, 49455U, 43124U, 33819U, 33688U, 36956U, 11571U, 54793U, 39112U, 56271U, 8468U, 45113U, 29880U, 37582U, 62455U, 64864U, 15728U, 7645U, 47531U, 35978U, 24387U, 29671U, 23187U, 52824U, 28794U, 6999U, 13944U, 40973U, 20614U, 15513U, 41382U, 47697U, 47921U, 61377U, 39088U, 4685U, 1017U, 7885U, 53810U, 64800U, 29852U, 25671U, 14432U, 42916U, 61203U, 50040U, 59957U, 44796U, 62736U, 56246U, 27008U, 12539U, 40122U, 52082U, 33466U, 53638U, 61622U, 1960U, 53437U, 35152U, 42622U, 62483U, 13658U, 59714U, 41617U, 52665U, 4351U, 7480U, 49464U, 5345U, 35224U, 29312U, 17576U, 33090U, 26919U, 19342U, 15899U, 17126U, 10246U, 52376U, 35928U, 24254U, 59779U, 3586U, 52355U, 3904U, 54125U, 31601U, 20057U, 26985U, 49674U, 23079U, 35457U, 31776U, 49133U, 25410U, 62426U, 38587U, 34220U, 64134U, 26638U, 54130U, 33467U, 64432U, 60248U, 51087U, 26196U, 33619U, 63597U, 58540U, 22750U, 58716U, 18315U, 8397U, 31661U, 52515U, 3101U, 36798U, 23933U, 10538U, 29249U, 9826U, 12235U, 11483U, 36873U, 63610U, 64706U, 22598U, 1496U, 59905U, 3007U, 22343U, 52225U, 25588U, 29010U, 58624U, 59681U, 11506U, 7894U, 19470U, 53975U, 34001U, 64895U, 36305U, 12938U, 16781U, 46555U, 51882U, 21673U, 60052U, 65528U, 50894U, 12766U, 19396U, 21511U, 41942U, 22894U, 3261U, 45851U, 28175U, 53076U, 53979U, 51182U, 17299U, 55047U, 10668U, 58696U, 33502U, 17496U, 9298U, 45431U, 35757U, 41261U, 35055U, 38557U, 52546U, 25263U, 6440U, 39661U, 32717U, 28680U, 65067U, 17988U, 63414U, 65458U, 37564U, 62287U, 17189U, 49521U, 56662U, 57126U, 14462U, 1765U, 65210U, 13451U, 50166U, 9505U, 8773U, 60803U, 6860U, 43488U, 26189U, 29458U, 16002U, 14351U, 31447U, 56528U, 6970U, 22023U, 14692U, 56219U, 30083U, 21574U, 55740U, 18260U, 26337U, 59311U, 48533U, 61191U, 29945U, 52267U, 62597U, 36499U, 44064U, 55313U, 62846U, 59959U, 2731U, 52321U, 48687U, 44833U, 4007U, 39969U, 62848U, 4199U, 1866U, 53957U, 7363U, 16130U, 53228U, 22044U, 15036U, 1107U))
}

fun onSensorsData(p9: org.company.Client.SensorsData) {
    assert(p9.values().same(ushortArrayOf(26079U, 54262U, 28485U, 60668U, 56244U, 9996U, 2178U, 50726U, 60381U, 38777U, 50684U, 20843U, 60170U, 57999U, 2940U, 13311U, 773U, 22546U, 32435U, 18916U, 53993U, 62104U, 4151U, 14298U, 15310U, 23231U, 7435U, 27327U, 58487U, 36031U, 64726U, 54977U, 16270U, 51553U, 2152U, 39523U, 40632U, 9323U, 13699U, 23934U, 30667U, 38645U, 35985U, 17987U, 8081U, 15755U, 38400U, 3327U, 24362U, 10734U, 18266U, 7315U, 44941U, 37487U, 11866U, 22085U, 4597U, 28744U, 48019U, 44064U, 23392U, 2350U, 5135U, 7208U, 64452U, 24399U, 18149U, 51771U, 16952U, 55274U, 32343U, 11607U, 45030U, 55191U, 52791U, 44201U, 64596U, 55481U, 54435U, 4187U, 6977U, 60314U, 55897U, 34698U, 18072U, 50852U, 9934U, 39116U, 27134U, 32433U, 47446U, 49878U, 63295U, 65358U, 56596U, 40409U, 58634U, 2139U, 6833U, 25960U, 61851U, 43498U, 50479U, 51776U, 30753U, 45397U, 13990U, 22085U, 64757U, 5905U, 33176U, 27359U, 27245U, 46851U, 5751U, 279U, 23387U, 6555U, 568U, 52412U, 26537U, 53524U, 25083U, 24745U, 54232U, 27605U, 57756U, 42488U, 31586U, 29043U, 50345U, 20075U, 34035U, 46656U, 30493U, 32281U, 19763U, 7194U, 59410U, 25955U, 50633U, 49269U, 56629U, 63574U, 55486U, 9683U, 10582U, 64996U, 3897U, 27985U, 46901U, 50292U, 49407U, 27548U, 65192U, 11211U, 55351U, 2286U, 57480U, 1093U, 47683U, 57456U, 12859U, 41742U, 42715U, 8539U, 54005U, 62400U, 32307U, 59951U, 16451U, 50224U, 41594U, 43070U, 33948U, 62926U, 39643U, 51688U, 12508U, 12078U, 26601U, 22053U, 19108U, 50776U, 23704U, 55216U, 61840U, 2546U, 29155U, 12265U, 42439U, 47773U, 27528U, 5080U, 30936U, 26743U, 65337U, 3469U, 9318U, 62657U, 55969U, 4277U, 21183U, 27829U, 41039U, 39285U, 65298U, 41970U, 14951U, 16541U, 47327U, 54874U, 27829U, 62983U, 53900U, 20586U, 59322U, 27324U, 59577U, 27802U, 32135U, 21205U, 62333U, 60965U, 39328U, 32029U, 39987U, 25622U, 23366U, 30118U, 8348U, 64396U, 48375U, 36788U, 50098U, 12371U, 33573U, 11795U, 21584U, 15454U, 40256U, 30236U, 33125U, 18285U, 38560U, 49595U, 39552U, 43300U, 48340U, 49093U, 52695U, 12192U, 42868U, 9497U, 53284U, 52207U, 5874U, 50065U, 52345U, 1106U, 41691U, 18996U, 4203U, 30480U, 52564U, 23457U, 46765U, 30105U, 63338U, 63619U, 58100U, 51484U, 34778U, 46041U, 50574U, 33518U, 24443U, 9616U, 51833U, 27756U, 42133U, 26062U, 43948U, 17822U, 41075U, 25856U, 5188U, 25434U, 47863U, 14537U, 20806U, 51245U, 64406U, 10582U, 54708U, 4353U, 47877U, 10370U, 17676U, 6195U, 11103U, 40806U, 8180U, 36990U, 20291U, 1690U, 11357U, 44622U, 24682U, 3734U, 35008U, 62488U, 58564U, 8315U, 64989U, 46260U, 2412U, 61719U, 34333U, 42558U, 16188U, 47617U, 35833U, 16460U, 13759U, 48669U, 36549U, 26556U, 824U, 3383U, 45263U, 26250U, 48315U, 53309U, 23681U, 5145U, 33609U, 24745U, 54243U, 43086U, 61671U, 45881U, 5567U, 8943U, 21582U, 50505U, 19313U, 63391U, 26676U, 8005U, 2574U, 31709U, 6412U, 377U, 13668U, 22936U, 4388U, 19241U, 7154U, 32044U, 38393U, 39253U, 38562U, 15287U, 50175U, 6545U, 16055U, 7734U, 2355U, 24924U, 22826U, 8793U, 45928U, 35991U, 4348U, 3356U, 28432U, 4608U, 46943U, 26108U, 39991U, 49559U, 38335U, 14857U, 32467U, 54927U, 42813U, 31389U, 56879U, 55398U, 6323U, 33879U, 11491U, 36006U, 28959U, 9497U, 2986U, 58271U, 31591U, 60719U, 42228U, 5361U, 54534U, 58362U, 57711U, 20940U, 30623U, 60543U, 46877U, 9814U, 56103U, 4707U, 23790U, 2105U, 32381U, 14417U, 60635U, 57336U, 48800U, 33257U, 56730U, 25185U, 57450U, 17886U, 6899U, 21446U, 55710U, 3458U, 40256U, 45553U, 5333U, 4842U, 32235U, 21818U, 51512U, 44233U, 35542U, 54778U, 37476U, 9339U, 64618U, 2915U, 9533U, 34432U, 51827U, 23439U, 64439U, 23947U, 60803U, 63781U, 64134U, 19224U, 10160U, 31490U, 31738U, 52646U, 18111U, 65436U, 57218U, 40998U, 8093U, 1295U, 53983U, 20975U, 61881U, 9039U, 27416U, 53101U, 13729U, 48750U, 36984U, 38183U, 58076U, 53326U, 46763U, 28706U, 33407U, 32238U, 40918U, 65340U, 34409U, 56467U, 43989U, 54061U, 13577U, 6860U, 18323U, 33551U, 51985U, 21636U, 34277U, 14471U, 65204U, 31492U, 6083U, 37234U, 13061U, 61390U, 23715U, 24539U, 33884U, 35656U, 53072U, 9981U, 7368U, 41000U, 35379U, 53084U, 47653U, 43863U, 15347U, 17470U, 38205U, 60836U, 48145U, 517U, 35267U, 11429U, 1986U, 38298U, 64822U, 5202U, 6290U, 18192U, 12829U, 48028U, 58523U, 64158U, 4119U, 52625U, 12840U, 61155U, 14996U, 41702U, 33794U, 50911U, 53955U, 48109U, 51115U, 51597U, 17826U, 38098U, 47731U, 16893U, 13467U, 56081U, 14787U, 10200U, 48240U, 17971U, 52957U, 60553U, 12971U, 63278U, 40577U, 57308U, 22828U, 42232U, 1488U, 41116U, 32827U, 36155U, 24958U, 52447U, 18868U, 38826U, 43683U, 20093U, 25832U, 44341U, 28692U, 22783U, 11766U, 7697U, 54438U, 51986U, 50474U, 40104U, 58091U, 31529U, 8053U, 4773U, 56457U, 14930U, 24451U, 4747U, 43730U, 28671U, 17752U, 63184U, 21309U, 8481U, 4657U, 11893U, 56933U, 18252U, 47654U, 52866U, 22550U, 59850U, 34503U, 39025U, 4298U, 2639U, 37275U, 12172U, 34688U, 41948U, 43848U, 27098U, 39607U, 44784U, 40240U, 45108U, 16464U, 17195U, 36044U, 42432U, 2854U, 42663U, 15889U, 2277U, 22459U, 26136U, 34434U, 32855U, 4138U, 40586U, 16270U, 11647U, 41144U, 414U, 35318U, 15037U, 13471U, 23633U, 54977U, 5028U, 43340U, 12842U, 2711U, 45103U, 34936U, 51183U, 26850U, 55935U, 15321U, 3888U, 22245U, 25766U, 27357U, 3861U, 25942U, 52348U, 33783U, 19627U, 31109U, 33469U, 59498U, 54189U, 56610U, 57148U, 61279U, 38545U, 43727U, 34170U, 51017U, 16604U, 56121U, 525U, 49322U, 11514U, 14647U, 16874U, 60873U, 1667U, 31300U, 59073U, 11146U, 6972U, 9262U, 4868U, 38034U, 37237U, 51456U, 14847U, 34139U, 51821U, 53492U, 30818U, 11778U, 29949U, 63355U, 56151U, 10941U, 60331U, 18805U, 41577U, 64423U, 4849U, 4487U, 58083U, 33607U, 57815U, 46742U, 41124U, 8955U, 23441U, 42900U, 50944U, 15361U, 1556U, 23890U, 51269U, 54330U, 36893U, 53132U, 61583U, 41936U, 45179U, 18676U, 31132U, 40200U, 35705U, 22497U, 23470U, 56804U, 7194U, 42577U, 59212U, 6505U, 31445U, 43488U, 57105U, 53929U, 39665U, 60220U, 16614U, 28839U, 63450U, 32148U, 18360U, 4805U, 29231U, 52886U, 62429U, 40688U, 63720U, 30282U, 28696U, 41737U, 35012U, 51160U, 32537U, 41159U, 13241U, 55682U, 50406U, 49455U, 43124U, 33819U, 33688U, 36956U, 11571U, 54793U, 39112U, 56271U, 8468U, 45113U, 29880U, 37582U, 62455U, 64864U, 15728U, 7645U, 47531U, 35978U, 24387U, 29671U, 23187U, 52824U, 28794U, 6999U, 13944U, 40973U, 20614U, 15513U, 41382U, 47697U, 47921U, 61377U, 39088U, 4685U, 1017U, 7885U, 53810U, 64800U, 29852U, 25671U, 14432U, 42916U, 61203U, 50040U, 59957U, 44796U, 62736U, 56246U, 27008U, 12539U, 40122U, 52082U, 33466U, 53638U, 61622U, 1960U, 53437U, 35152U, 42622U, 62483U, 13658U, 59714U, 41617U, 52665U, 4351U, 7480U, 49464U, 5345U, 35224U, 29312U, 17576U, 33090U, 26919U, 19342U, 15899U, 17126U, 10246U, 52376U, 35928U, 24254U, 59779U, 3586U, 52355U, 3904U, 54125U, 31601U, 20057U, 26985U, 49674U, 23079U, 35457U, 31776U, 49133U, 25410U, 62426U, 38587U, 34220U, 64134U, 26638U, 54130U, 33467U, 64432U, 60248U, 51087U, 26196U, 33619U, 63597U, 58540U, 22750U, 58716U, 18315U, 8397U, 31661U, 52515U, 3101U, 36798U, 23933U, 10538U, 29249U, 9826U, 12235U, 11483U, 36873U, 63610U, 64706U, 22598U, 1496U, 59905U, 3007U, 22343U, 52225U, 25588U, 29010U, 58624U, 59681U, 11506U, 7894U, 19470U, 53975U, 34001U, 64895U, 36305U, 12938U, 16781U, 46555U, 51882U, 21673U, 60052U, 65528U, 50894U, 12766U, 19396U, 21511U, 41942U, 22894U, 3261U, 45851U, 28175U, 53076U, 53979U, 51182U, 17299U, 55047U, 10668U, 58696U, 33502U, 17496U, 9298U, 45431U, 35757U, 41261U, 35055U, 38557U, 52546U, 25263U, 6440U, 39661U, 32717U, 28680U, 65067U, 17988U, 63414U, 65458U, 37564U, 62287U, 17189U, 49521U, 56662U, 57126U, 14462U, 1765U, 65210U, 13451U, 50166U, 9505U, 8773U, 60803U, 6860U, 43488U, 26189U, 29458U, 16002U, 14351U, 31447U, 56528U, 6970U, 22023U, 14692U, 56219U, 30083U, 21574U, 55740U, 18260U, 26337U, 59311U, 48533U, 61191U, 29945U, 52267U, 62597U, 36499U, 44064U, 55313U, 62846U, 59959U, 2731U, 52321U, 48687U, 44833U, 4007U, 39969U, 62848U, 4199U, 1866U, 53957U, 7363U, 16130U, 53228U, 22044U, 15036U, 1107U)))

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
