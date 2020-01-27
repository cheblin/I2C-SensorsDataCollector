
#include "Utils_.hpp"

namespace org
{
    namespace company
    {
        enum class Errors
        {
            SensorsDataOverflow = 0,
            CannotReadSensor = 1
        };

        struct DeviceVersion
        {
            static Meta meta ;

            Pack * pack_;
            uint8_t & data_;
            DeviceVersion(Pack* pack, uint8_t& data) : pack_(pack), data_(data) {}
            Pack* unwrap_()
            {
                Pack* ret = pack_;
                pack_ = nullptr;
                return ret;
            }
            bool operator==(const DeviceVersion &rhs)
            {
                return &data_ == &rhs.data_ || (memcmp(&data_, &rhs.data_, 4) == 0);
            }

            uint16_t DevID() const {return (static_cast< uint16_t >(get_bytes(&data_,  0, 2)))    ;}

            uint16_t Ver() const {return (static_cast< uint16_t >(get_bytes(&data_,  2, 2)))    ;}



            /*
            struct <DST> {
            	void DevID( uint16_t src  ) {}
            	void Ver( uint16_t src  ) {}
            }
            */
            template<typename DST>
            void push_data_(DST  dst) const;

        };
        struct Stop {static Meta meta ;};
        struct Start {static Meta meta ;};
        struct GetDeviceVersion {static Meta meta ;};
        struct GetConfiguration {static Meta meta ;};
        struct SetConfiguration {static Meta meta ;};
        struct BusConfiguration
        {
            static Meta meta ;

            Pack * pack_;
            uint8_t & data_;
            BusConfiguration(Pack* pack, uint8_t& data) : pack_(pack), data_(data) {}
            Pack* unwrap_()
            {
                Pack* ret = pack_;
                pack_ = nullptr;
                return ret;
            }
            bool operator==(const BusConfiguration &rhs)
            {
                return &data_ == &rhs.data_ || (memcmp(&data_, &rhs.data_, 5) == 0);
            }

            uint8_t multiplier() const {return        static_cast< uint8_t >((&data_)[0])    ;}
            void multiplier(uint8_t  src) const {    (&data_)[0] = (uint8_t)(src) ;  }
            uint16_t time() const {return (static_cast< uint16_t >(get_bytes(&data_,  1, 2)))    ;}
            void time(uint16_t  src) const {    set_bytes((src), 2, &data_,  1);  }
            uint16_t clk_khz() const {return (static_cast< uint16_t >(get_bytes(&data_,  3, 2)))    ;}
            void clk_khz(uint16_t  src) const {    set_bytes((src), 2, &data_,  3);  }


            /*
            struct <DST> {
            	void multiplier( uint8_t src  ) {}
            	void time( uint16_t src  ) {}
            	void clk_khz( uint16_t src  ) {}
            }
            */
            template<typename DST>
            void push_data_(DST  dst) const;

            /*
            struct <SRC> {
            	uint8_t multiplier(  ) {}
            	uint8_t multiplier(  ) {}
            	uint16_t time(  ) {}
            	uint16_t time(  ) {}
            	uint16_t clk_khz(  ) {}
            	uint16_t clk_khz(  ) {}
            }
            */
            template<typename SRC>
            void pull_data_(SRC  src);

        };
        struct InstructionsPack
        {
            static Meta meta ;

            Pack * pack_;
            uint8_t & data_;
            InstructionsPack(Pack* pack, uint8_t& data) : pack_(pack), data_(data) {}
            Pack* unwrap_()
            {
                Pack* ret = pack_;
                pack_ = nullptr;
                return ret;
            }
            bool operator==(const InstructionsPack &rhs)
            {
                return &data_ == &rhs.data_ || (memcmp(&data_, &rhs.data_, 257) == 0);
            }

            uint8_t Length() const {return        static_cast< uint8_t >((&data_)[0])    ;}
            void Length(uint8_t  src) const {    (&data_)[0] = (uint8_t)(src) ;  }

            struct Instructions_
            {
                uint8_t * bytes;
                size_t length;
                org::unirail::utils:: iterator<uint8_t, Instructions_> begin() const { return {*this}; }
                org::unirail::utils:: iterator<uint8_t, Instructions_> end() const { return {*this, 256 }; }

                uint8_t get(size_t index) const {return      static_cast< uint8_t >((bytes)[index]);}


                void set(uint8_t src, size_t index) const
                {
                    (bytes)[index] = (uint8_t)(src) ;
                }

                static constexpr size_t len = 256;

            };

            struct Instructions_ Instructions() const
            {
                struct Instructions_ ret {   &data_ + 1  };
                return ret;
            }

            template< typename uint8_src_container >
            Instructions_ Instructions(const uint8_src_container & src) const
            {
                auto first = src.begin();
                struct Instructions_ ret {   &data_ + 1  };
                for(size_t i = 0 ; i < 256 ; i++, ++first)
                    ret.set(*first, i);
                return ret;
            }

            Instructions_ Instructions(uint8_t  * const src)  const { return  Instructions(static_cast<uint8_t const *>(src)); }
            Instructions_ Instructions(uint8_t const * const src)  const
            {
                struct Instructions_ ret {   &data_ + 1  };
                if(src)
                    for(size_t i = 0 ; i < 256 ; i++)
                        ret.set(src[i], i);
                return ret;
            }

            static constexpr  int16_t  RW_mode =  128; //128

            /*
            struct <DST> {
            	void Length( uint8_t src  ) {}
            	void Instructions( const struct org::company::InstructionsPack :: Instructions_  & src  ) const {}
            }
            */
            template<typename DST>
            void push_data_(DST  dst) const;

            /*
            struct <SRC> {
            	uint8_t Length(  ) {}
            	uint8_t Length(  ) {}
            	std container of uint8  Instructions(  ) {}
            }
            */
            template<typename SRC>
            void pull_data_(SRC  src);

        };
        struct DeviceError
        {
            static Meta meta ;

            Pack * pack_;
            uint8_t & data_;
            DeviceError(Pack* pack, uint8_t& data) : pack_(pack), data_(data) {}
            Pack* unwrap_()
            {
                Pack* ret = pack_;
                pack_ = nullptr;
                return ret;
            }
            bool operator==(const DeviceError &rhs)
            {
                return &data_ == &rhs.data_ || (memcmp(&data_, &rhs.data_, 3) == 0);
            }

            uint16_t param1() const {return (static_cast< uint16_t >(get_bytes(&data_,  0, 2)))    ;}

            enum Errors error_id() const {return      static_cast< enum Errors >((get_bits(&data_, 16, 1)))    ;}



            /*
            struct <DST> {
            	void error_id( enum org::company:: Errors src  ) {}
            	void param1( uint16_t src  ) {}
            }
            */
            template<typename DST>
            void push_data_(DST  dst) const;

        };
        struct SensorsData
        {
            static Meta meta ;

            Pack * pack_;
            uint8_t & data_;
            SensorsData(Pack* pack, uint8_t& data) : pack_(pack), data_(data) {}
            Pack* unwrap_()
            {
                Pack* ret = pack_;
                pack_ = nullptr;
                return ret;
            }
            bool operator==(const SensorsData &rhs)
            {
                return &data_ == &rhs.data_ || (memcmp(&data_, &rhs.data_, 2000) == 0);
            }


            struct values_
            {
                uint8_t * bytes;
                size_t length;
                org::unirail::utils:: iterator<uint16_t, values_> begin() const { return {*this}; }
                org::unirail::utils:: iterator<uint16_t, values_> end() const { return {*this, 1000 }; }

                uint16_t get(size_t index) const {return (static_cast< uint16_t >(get_bytes(bytes,  index * 2, 2)));}


                static constexpr size_t len = 1000;

            };

            struct values_ values() const
            {
                struct values_ ret {   &data_ + 0  };
                return ret;
            }


            /*
            struct <DST> {
            	void values( const struct org::company::SensorsData :: values_  & src  ) const {}
            }
            */
            template<typename DST>
            void push_data_(DST  dst) const;

        };



        template<typename DST>
        void DeviceVersion::push_data_(DST dst) const
        {
            dst.DevID(DevID());
            dst.Ver(Ver());
        }

        template<typename SRC>
        void BusConfiguration::pull_data_(SRC  src)
        {
            multiplier(src.multiplier());
            time(src.time());
            clk_khz(src.clk_khz());
        }

        template<typename DST>
        void BusConfiguration::push_data_(DST dst) const
        {
            dst.multiplier(multiplier());
            dst.time(time());
            dst.clk_khz(clk_khz());
        }

        template<typename SRC>
        void InstructionsPack::pull_data_(SRC  src)
        {
            Length(src.Length());
            Instructions(src.Instructions());
        }

        template<typename DST>
        void InstructionsPack::push_data_(DST dst) const
        {
            dst.Length(Length());
            dst.Instructions(Instructions());
        }

        template<typename DST>
        void DeviceError::push_data_(DST dst) const
        {
            dst.error_id(error_id());
            dst.param1(param1());
        }

        template<typename DST>
        void SensorsData::push_data_(DST dst) const
        {
            dst.values(values());
        }


        //#######################                 Communication                       ##################################
        /*
        	static <CHANNEL> * from(Receiver * receiver){}
        	static const Pack* pull_sending_pack(Transmitter * transmitter){}
        	static bool push_sending_pack(Transmitter * transmitter, Pack * pack){}
        */
        template< typename CHANNEL >
        struct Communication
        {
            Communication()
            {
                receiver.dispatch = dispatch;
                transmitter.pull = CHANNEL::pull_sending_pack;
            }

            Receiver receiver{};
            Transmitter transmitter{};
            size_t  packs_into_bytes(uint8_t* dst, size_t dst_bytes) { return ::transmit(& transmitter, dst, dst_bytes);}
            /*
            void on_DeviceVersion(  Pack * pack ){}
            void on_BusConfiguration(  Pack * pack ){}
            void on_InstructionsPack(  Pack * pack ){}
            void on_DeviceError(  Pack * pack ){}
            void on_SensorsData(  Pack * pack ){}
            */
            static Meta const * dispatch(Receiver * receiver, size_t id, Pack * pack)
            {
                CHANNEL * channel = CHANNEL::from(receiver);
                using namespace org::unirail;
                switch(id)
                {
                    default:
                        return nullptr;
                    case 0:
                        return pack ? channel->on_DeviceVersion(pack), nullptr : &org::company::DeviceVersion::meta;
                    case 6:
                        return pack ? channel->on_BusConfiguration(pack), nullptr : &org::company::BusConfiguration::meta;
                    case 7:
                        return pack ? channel->on_InstructionsPack(pack), nullptr : &org::company::InstructionsPack::meta;
                    case 8:
                        return pack ? channel->on_DeviceError(pack), nullptr : &org::company::DeviceError::meta;
                    case 9:
                        return pack ? channel->on_SensorsData(pack), nullptr : &org::company::SensorsData::meta;
                }
            }
            /*Push received packs bytes to the channel*/
            void  bytes_into_packs(uint8_t* src, size_t src_bytes) {  ::receive(src, src_bytes, & receiver);}
            bool send_Stop() { return CHANNEL::push_sending_pack(&transmitter, new_pack(&org::company::Stop::meta));}
            bool send_Start() { return CHANNEL::push_sending_pack(&transmitter, new_pack(&org::company::Start::meta));}
            bool send_GetDeviceVersion() { return CHANNEL::push_sending_pack(&transmitter, new_pack(&org::company::GetDeviceVersion::meta));}
            bool send_GetConfiguration() { return CHANNEL::push_sending_pack(&transmitter, new_pack(&org::company::GetConfiguration::meta));}
            bool send_SetConfiguration() { return CHANNEL::push_sending_pack(&transmitter, new_pack(&org::company::SetConfiguration::meta));}

            bool send(BusConfiguration & src)
            {
                return src.pack_ && CHANNEL::push_sending_pack(&transmitter,  src.pack_) ?  src.pack_ = nullptr, true : false;
            }

            bool send(InstructionsPack & src)
            {
                return src.pack_ && CHANNEL::push_sending_pack(&transmitter,  src.pack_) ?  src.pack_ = nullptr, true : false;
            }

            struct NEW
            {
                /**
                *Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */
                static struct BusConfiguration BusConfiguration()

                {
                    auto pack = new_pack(&org::company::BusConfiguration::meta);
                    return {pack, *pack->bytes};
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
                static struct InstructionsPack InstructionsPack()

                {
                    auto pack = new_pack(&org::company::InstructionsPack::meta);
                    return {pack, *pack->bytes};
                }

            } NEW;

        };

    }
}
