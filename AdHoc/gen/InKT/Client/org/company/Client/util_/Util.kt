
package org.company.Client.util_

import org.company.Client.*
import org.unirail.AdHoc.*
import org.unirail.AdHoc.Pack

inline class InstructionsPack_Instructions(val data: Pack.Bytes) {
    inline fun len(): Int { return 256 }
    inline fun get(index: Int): UByte { return (data.bytes[data.origin + 1 + index]).toUByte() }

    fun same(other: UByteArray): Boolean {
        if (other.size != 256) return false
        other.forEachIndexed { i, v -> if (v != get(i)) return false }
        return true
    }

    fun same(other: InstructionsPack_Instructions): Boolean {
        if (other.len() != 256) return false
        for (i in 0 until 256) { if (other.get(i) != get(i)) return false }
        return true
    }

    inline fun set(src: UByte, index: Int) { data.bytes[data.origin + 1 + index] = (src).toByte() }
}

inline class SensorsData_values(val data: Pack.Bytes) {
    inline fun len(): Int { return 1000 }
    inline fun get(index: Int): UShort { return (get_bytes(data.bytes, data.origin + 0 + index * 2, 2)).toUShort() }

    fun same(other: UShortArray): Boolean {
        if (other.size != 1000) return false
        other.forEachIndexed { i, v -> if (v != get(i)) return false }
        return true
    }

    fun same(other: SensorsData_values): Boolean {
        if (other.len() != 1000) return false
        for (i in 0 until 1000) { if (other.get(i) != get(i)) return false }
        return true
    }
}

const val RECEIVE_REQ_MAX_BYTES = 2000
const val RECEIVE_FULL_MAX_BYTES = 2000

const val SEND_REQ_MAX_BYTES = 257
const val SEND_FULL_MAX_BYTES = 257
