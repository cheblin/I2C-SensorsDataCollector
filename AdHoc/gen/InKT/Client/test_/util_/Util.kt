
package test_.util_
import org.unirail.AdHoc.*
import test_.*

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

    inline fun set(src: UShort, index: Int) { set_bytes((src).toULong().toLong(), 2, data.bytes, data.origin + 0 + index * 2) }
}
