
package org.company.Client

import java.util.*
import kotlin.experimental.*
import org.company.Client.util_.*
import org.unirail.AdHoc.*
import org.unirail.AdHoc.Pack.Cursor
import org.unirail.AdHoc.Pack.Meta
fun Cursor(): Cursor {
    return Cursor(null, 1, 2)
}

inline class Errors(val value: Byte) {

    operator fun not() = Errors(value.inv())
    operator fun contains(other: Errors) = (value and other.value) != 0.toByte()
    operator fun contains(other: ULong) = (value and other.toByte()) != 0.toByte()
    infix fun or(other: Errors) = Errors(value or (other.value))
    infix fun and(other: Errors) = Errors(value and (other.value))
    infix fun xor(other: Errors) = Errors(value xor (other.value))
    companion object {
        val SensorsDataOverflow = Errors(0)
        val CannotReadSensor = Errors(1)
    } 
}

inline class DeviceVersion(val data: Pack.Bytes) {

    inline fun DevID(): UShort { return (get_bytes(data.bytes, data.origin + 0, 2)).toUShort() }

    inline fun Ver(): UShort { return (get_bytes(data.bytes, data.origin + 2, 2)).toUShort() }

    companion object {

        val meta = Meta(0, 0, 0, 0, 4, 32)

        inline fun push(
            src: DeviceVersion,
            DevID: (src: UShort) -> Unit,
            Ver: (src: UShort) -> Unit
        ) {
            DevID(src.DevID())

            Ver(src.Ver())
        }
    }
}
object Stop {
    val pack = Pack(Meta(1))
}
object Start {
    val pack = Pack(Meta(2))
}
object GetDeviceVersion {
    val pack = Pack(Meta(3))
}
object GetConfiguration {
    val pack = Pack(Meta(4))
}
object SetConfiguration {
    val pack = Pack(Meta(5))
}
inline class BusConfiguration(val data: Pack.Bytes) {

    inline fun multiplier(): UByte { return (data.bytes[data.origin + 0]).toUByte() }
    inline fun multiplier(src: UByte) { data.bytes[data.origin + 0] = (src).toByte() }

    inline fun time(): UShort { return (get_bytes(data.bytes, data.origin + 1, 2)).toUShort() }
    inline fun time(src: UShort) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 1) }

    inline fun clk_khz(): UShort { return (get_bytes(data.bytes, data.origin + 3, 2)).toUShort() }
    inline fun clk_khz(src: UShort) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 3) }

    companion object {

        val meta = Meta(6, 0, 0, 0, 5, 40)

        inline fun push(
            src: BusConfiguration,
            multiplier: (src: UByte) -> Unit,
            time: (src: UShort) -> Unit,
            clk_khz: (src: UShort) -> Unit
        ) {
            multiplier(src.multiplier())

            time(src.time())

            clk_khz(src.clk_khz())
        }

        inline fun pull(
            dst: BusConfiguration,
            multiplier: () -> UByte,
            time: () -> UShort,
            clk_khz: () -> UShort
        ) {
            dst.multiplier(multiplier())

            dst.time(time())

            dst.clk_khz(clk_khz())
        }
    }
}
inline class InstructionsPack(val data: Pack.Bytes) {

    inline fun Length(): UByte { return (data.bytes[data.origin + 0]).toUByte() }
    inline fun Length(src: UByte) { data.bytes[data.origin + 0] = (src).toByte() }

    inline fun Instructions(): InstructionsPack_Instructions {
        return InstructionsPack_Instructions(data)
    }

    inline fun Instructions(src: UByteArray) {
        val len = minOf(src.size, 256)

        for (index in 0 until len)
            data.bytes[data.origin + 1 + index] = (src[index]).toByte()
    }
    object Instructions {
        const val item_len = 256
    }

    companion object {

        val meta = Meta(7, 0, 0, 0, 257, 2056)
        @JvmField val RW_mode: Short = (128).toShort() // 128

        inline fun push(
            src: InstructionsPack,
            Length: (src: UByte) -> Unit,
            Instructions: (src: InstructionsPack_Instructions) -> Unit
        ) {
            Length(src.Length())
            src.Instructions()?.let { item ->
                Instructions(item)
            }
        }

        inline fun pull(
            dst: InstructionsPack,
            Length: () -> UByte,
            Instructions: (dst: InstructionsPack_Instructions) -> Unit
        ) {
            dst.Length(Length())
            Instructions(dst.Instructions())
        }
    }
}
inline class DeviceError(val data: Pack.Bytes) {

    inline fun param1(): UShort { return (get_bytes(data.bytes, data.origin + 0, 2)).toUShort() }

    inline fun error_id(): Errors { return Errors((get_bits(data.bytes, data.origin * 8 + 16, 1)).toByte()) }

    companion object {

        val meta = Meta(8, 0, 0, 0, 3, 17)

        inline fun push(
            src: DeviceError,
            error_id: (src: org.company.Client.Errors) -> Unit,
            param1: (src: UShort) -> Unit
        ) {
            error_id(src.error_id())

            param1(src.param1())
        }
    }
}
inline class SensorsData(val data: Pack.Bytes) {

    inline fun values(): SensorsData_values {
        return SensorsData_values(data)
    }

    object values {
        const val item_len = 1000
    }

    companion object {

        val meta = Meta(9, 0, 0, 0, 2000, 16000)

        inline fun push(
            src: SensorsData,
            values: (src: SensorsData_values) -> Unit
        ) {
            src.values()?.let { item ->
                values(item)
            }
        }
    }
}

abstract class Communication {

    abstract fun pushSendingPack(pack: Pack): Boolean
    abstract fun pullSendingPack(): Pack?

    val transmitter: java.io.InputStream = object : Channel.Transmitter(1, 1) {
        override fun pullSendingPack(): Pack? { return this@Communication.pullSendingPack() }
    }
    fun sendStop(): Boolean {
        return pushSendingPack(Stop.pack)
    } fun sendStart(): Boolean {
        return pushSendingPack(Start.pack)
    } fun sendGetDeviceVersion(): Boolean {
        return pushSendingPack(GetDeviceVersion.pack)
    } fun sendGetConfiguration(): Boolean {
        return pushSendingPack(GetConfiguration.pack)
    } fun sendSetConfiguration(): Boolean {
        return pushSendingPack(SetConfiguration.pack)
    }
    fun send(src: BusConfiguration): Boolean {
        if (src.data.bytes == null) return false
        val pack = src.data.unwrap()
        if (pushSendingPack(pack)) return true
        src.data.wrap(pack)
        return false
    }

    fun send(src: InstructionsPack): Boolean {
        if (src.data.bytes == null) return false
        val pack = src.data.unwrap()
        if (pushSendingPack(pack)) return true
        src.data.wrap(pack)
        return false
    }

    object NEW { /**
*Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */
        fun BusConfiguration(bytes: Pack.Bytes): BusConfiguration {
            bytes.init(BusConfiguration.meta)
            return org.company.Client.BusConfiguration(bytes)
        }
        /**
*Mini assembler instructions buffer starts with a
*				 RW-instruction byte, it switch device in the Read/Write mode and hold the number of Readings/Writings I2C Bus.
*				 .
*				 every Read instruction consume 2 bytes
*				 address
*				 register
*				 .
*				 every Write instruction consume 4 bytes
*				 address
*				 writing_value - 2 bytes
*				 register
*				 .
*				 so InstructionsPack can hold maximum 127 Readings or 64 Writings
*				 .
*				 if RW- < 128 - it contains number of rest Readings  from I2C
*				 if 128 < RW - it contains (RW - 128) number of rest Writings  into I2C
*				 .
*				 If this pack is not following after RequestConfiguration:
*				 .
*				 if it contains read Sensor data instructions
*				 it will be proceeded many times until fill all SensorsData, and then SensorsData send
*				 .
*				 if it contains any write Sensors registers instructions
*				 it will be proceeded only once */
        fun InstructionsPack(bytes: Pack.Bytes): InstructionsPack {
            bytes.init(InstructionsPack.meta)
            return org.company.Client.InstructionsPack(bytes)
        }
    }

    val receiver: java.io.OutputStream = object : Channel.Receiver(1, 1) {
        override fun dispatch(id: Int, pack: Pack?): Meta? {
            when (id) {
                0 -> on_DeviceVersion(pack ?: return DeviceVersion.meta)
                6 -> on_BusConfiguration(pack ?: return BusConfiguration.meta)
                7 -> on_InstructionsPack(pack ?: return InstructionsPack.meta)
                8 -> on_DeviceError(pack ?: return DeviceError.meta)
                9 -> on_SensorsData(pack ?: return SensorsData.meta)

                else -> return null
            }
            return null
        }
    }
    abstract fun on_DeviceVersion(pack: Pack)
    abstract fun on_BusConfiguration(pack: Pack)
    abstract fun on_InstructionsPack(pack: Pack)
    abstract fun on_DeviceError(pack: Pack)
    abstract fun on_SensorsData(pack: Pack)
}
