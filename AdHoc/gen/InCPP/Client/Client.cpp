
#include "Client.hpp"

namespace org
{
    namespace company
    {

        Meta DeviceVersion::meta = {0, 0, 0, 0, 4, nullptr, 0, 0, 0};
        static Pack* meta1alloc(Pack* pack,  size_t len)
        {
            static const Pack empty = { &Stop::meta};
            return (Pack*)&empty;
        }
        Meta Stop::meta = {1, 0, 0, 0, 0, meta1alloc, 0, 0, 0};
        static Pack* meta2alloc(Pack* pack,  size_t len)
        {
            static const Pack empty = { &Start::meta};
            return (Pack*)&empty;
        }
        Meta Start::meta = {2, 0, 0, 0, 0, meta2alloc, 0, 0, 0};
        static Pack* meta3alloc(Pack* pack,  size_t len)
        {
            static const Pack empty = { &GetDeviceVersion::meta};
            return (Pack*)&empty;
        }
        Meta GetDeviceVersion::meta = {3, 0, 0, 0, 0, meta3alloc, 0, 0, 0};
        static Pack* meta4alloc(Pack* pack,  size_t len)
        {
            static const Pack empty = { &GetConfiguration::meta};
            return (Pack*)&empty;
        }
        Meta GetConfiguration::meta = {4, 0, 0, 0, 0, meta4alloc, 0, 0, 0};
        static Pack* meta5alloc(Pack* pack,  size_t len)
        {
            static const Pack empty = { &SetConfiguration::meta};
            return (Pack*)&empty;
        }
        Meta SetConfiguration::meta = {5, 0, 0, 0, 0, meta5alloc, 0, 0, 0};
        Meta BusConfiguration::meta = {6, 0, 0, 0, 5, nullptr, 0, 0, 0};
        Meta InstructionsPack::meta = {7, 0, 0, 0, 257, nullptr, 0, 0, 0};
        Meta DeviceError::meta = {8, 0, 0, 0, 3, nullptr, 0, 0, 0};
        Meta SensorsData::meta = {9, 0, 0, 0, 2000, nullptr, 0, 0, 0};

    }
}
