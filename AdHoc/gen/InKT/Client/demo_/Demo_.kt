
package demo_

import org.company.Client.*
import java.util.concurrent.ConcurrentLinkedQueue
import kotlin.experimental.*
import org.unirail.AdHoc

var some_Int = 0.toInt()
var some_UShort = 0.toUShort()
var some_UByte = 0.toUByte()

fun onDeviceVersion(p0: DeviceVersion) {
    val some_DevID = p0.DevID()
    val some_Ver = p0.Ver()
}
fun test_.DeviceVersion.copyFrom(SRC: DeviceVersion) {
    DeviceVersion.push(
        SRC, { src -> DevID(src) }, { src -> Ver(src) }

    )
}

fun onStop() {}
fun onStart() {}
fun onGetDeviceVersion() {}
fun onGetConfiguration() {}
fun onSetConfiguration() {}
fun onBusConfiguration(p6: BusConfiguration) {
    val some_multiplier = p6.multiplier()
    val some_time = p6.time()
    val some_clk_khz = p6.clk_khz()
}
fun BusConfiguration.copyFrom(SRC: BusConfiguration) {
    BusConfiguration.push(
        SRC, { src -> multiplier(src) }, { src -> time(src) }, { src -> clk_khz(src) }

    )
}
fun fill(p6: BusConfiguration) {
    p6.multiplier(some_UByte)
    p6.time(some_UShort)
    p6.clk_khz(some_UShort)
}
fun BusConfiguration.copyInto(DST: BusConfiguration) {
    BusConfiguration.pull(
        DST, { -> multiplier() }, { -> time() }, { -> clk_khz() }

    )
}

fun onInstructionsPack(p7: InstructionsPack) {
    val some_Length = p7.Length()
    p7.Instructions()?.let { item ->
        for (index in 0 until item.len())
            some_UByte = item.get(index)
    }
}
fun InstructionsPack.copyFrom(SRC: InstructionsPack) {
    InstructionsPack.push(
        SRC, { src -> Length(src) },
        { src ->
            val item = Instructions()
            for (i in 0 until 256)
                item.set(src.get(i), i)
        }

    )
}
fun fill(p7: InstructionsPack) {
    p7.Length(some_UByte)
    p7.Instructions()?.let { item ->
        p7.Instructions().let { item ->
            for (index in 0 until item.len())
                item.set(some_UByte, index)
        }
    }
}
fun InstructionsPack.copyInto(DST: InstructionsPack) {
    InstructionsPack.pull(
        DST, { -> Length() },
        { dst ->
            var src = this.Instructions()
            for (i in 0 until 256)
                dst.set(src.get(i), i)
        }

    )
}

fun onDeviceError(p8: DeviceError) {
    val some_error_id = p8.error_id()
    val some_param1 = p8.param1()
}
fun test_.DeviceError.copyFrom(SRC: DeviceError) {
    DeviceError.push(
        SRC, { src -> error_id(src) }, { src -> param1(src) }

    )
}

fun onSensorsData(p9: SensorsData) {
    p9.values()?.let { item ->
        for (index in 0 until item.len())
            some_UShort = item.get(index)
    }
}
fun test_.SensorsData.copyFrom(SRC: SensorsData) {
    SensorsData.push(
        SRC,
        { src ->
            val item = values()
            for (i in 0 until 1000)
                item.set(src.get(i), i)
        }

    )
}

class Communication_demo : Communication() {
    val sendingPacks = ConcurrentLinkedQueue< AdHoc.Pack >()
    override fun pushSendingPack(pack: AdHoc.Pack): Boolean { return sendingPacks.add(pack) }
    override fun pullSendingPack(): AdHoc.Pack? { return sendingPacks.poll() }
    override fun on_DeviceVersion(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p0 = DeviceVersion(bytes)
        onDeviceVersion.forEach { handler -> handler(p0) }
    }
    override fun on_BusConfiguration(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p6 = BusConfiguration(bytes)
        onBusConfiguration.forEach { handler -> handler(p6) }
    }
    override fun on_InstructionsPack(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p7 = InstructionsPack(bytes)
        onInstructionsPack.forEach { handler -> handler(p7) }
    }
    override fun on_DeviceError(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p8 = DeviceError(bytes)
        onDeviceError.forEach { handler -> handler(p8) }
    }
    override fun on_SensorsData(pack: AdHoc.Pack) {
        val bytes = AdHoc.Pack.Bytes()
        bytes.wrap(pack)
        val p9 = SensorsData(bytes)
        onSensorsData.forEach { handler -> handler(p9) }
    }

    val onDeviceVersion: MutableCollection< (DeviceVersion) -> Unit > = ArrayList()
    val onBusConfiguration: MutableCollection< (BusConfiguration) -> Unit > = ArrayList()
    val onInstructionsPack: MutableCollection< (InstructionsPack) -> Unit > = ArrayList()
    val onDeviceError: MutableCollection< (DeviceError) -> Unit > = ArrayList()
    val onSensorsData: MutableCollection< (SensorsData) -> Unit > = ArrayList()
}

fun main() {
    val buff = ByteArray(1024)
    var bytes_out = 0
    val cur = Cursor()

    val Communication_instance = Communication_demo()

    Communication_instance.onDeviceVersion.add { pack -> onDeviceVersion(pack) }
    Communication_instance.onBusConfiguration.add { pack -> onBusConfiguration(pack) }
    Communication_instance.onInstructionsPack.add { pack -> onInstructionsPack(pack) }
    Communication_instance.onDeviceError.add { pack -> onDeviceError(pack) }
    Communication_instance.onSensorsData.add { pack -> onSensorsData(pack) }
    if (! Communication_instance.sendStop())
        throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    if (! Communication_instance.sendStart())
        throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    if (! Communication_instance.sendGetDeviceVersion())
        throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    if (! Communication_instance.sendGetConfiguration())
        throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    if (! Communication_instance.sendSetConfiguration())
        throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")

    Communication.NEW.BusConfiguration(cur).let { p6 ->
        fill(p6)
        if (!Communication_instance.send(p6)) // push p6 pack into the channel send out buffer
            throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    }

    Communication.NEW.InstructionsPack(cur).let { p7 ->
        fill(p7)
        if (!Communication_instance.send(p7)) // push p7 pack into the channel send out buffer
            throw RuntimeException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW")
    }

    bytes_out = Communication_instance.transmitter.read(buff) // sending out packs
    Communication_instance.receiver.write(buff) // receiving packs
}
