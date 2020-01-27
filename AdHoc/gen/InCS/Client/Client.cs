
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Text;
using org.unirail;
using Cursor = org.unirail.AdHoc.Pack.Cursor;
using Field = org.unirail.AdHoc.Pack.Meta.Field;
using Pack = org.unirail.AdHoc.Pack;
using Lib  = org.unirail.AdHoc;
using System.IO;
using org.company.Client;




namespace  org.company
{
    namespace Client
    {



        public enum Errors : sbyte
        {

            SensorsDataOverflow = 0,
            CannotReadSensor = 1
        }

        public struct DeviceVersion
        {
            internal Pack.Bytes  data_;
            internal DeviceVersion(Pack.Bytes  data) { this.data_ = data; }

            public ushort DevID() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 0, 2)); ;}


            public ushort Ver() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 2, 2)); ;}




            public interface DST_
            {
                void DevID(ushort src);
                void Ver(ushort src);

            }


            internal static readonly Pack.Meta meta_ =  new Pack.Meta(0, 0, 0, 0, 4, 32);


            public  void push_< DST >(DST dst) where DST : struct, DST_
            {
                dst.DevID(DevID()) ;
                dst.Ver(Ver()) ;
            }



        }
        public struct Stop
        {
            internal static readonly Pack pack_ = new Pack(new Pack.Meta(1));
        }
        public struct Start
        {
            internal static readonly Pack pack_ = new Pack(new Pack.Meta(2));
        }
        public struct GetDeviceVersion
        {
            internal static readonly Pack pack_ = new Pack(new Pack.Meta(3));
        }
        public struct GetConfiguration
        {
            internal static readonly Pack pack_ = new Pack(new Pack.Meta(4));
        }
        public struct SetConfiguration
        {
            internal static readonly Pack pack_ = new Pack(new Pack.Meta(5));
        }
        public struct BusConfiguration
        {
            internal Pack.Bytes  data_;
            internal BusConfiguration(Pack.Bytes  data) { this.data_ = data; }

            public byte multiplier() { return (byte)(data_.bytes[data_.origin + 0]); ;}
            public void multiplier(byte src) {    data_.bytes[data_.origin + 0] = (byte)(src); ; }

            public ushort time() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 1, 2)); ;}
            public void time(ushort src) {   Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 1); ; }

            public ushort clk_khz() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 3, 2)); ;}
            public void clk_khz(ushort src) {   Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 3); ; }



            public interface DST_
            {
                void multiplier(byte src);
                void time(ushort src);
                void clk_khz(ushort src);

            }


            internal static readonly Pack.Meta meta_ =  new Pack.Meta(6, 0, 0, 0, 5, 40);


            public  void push_< DST >(DST dst) where DST : struct, DST_
            {
                dst.multiplier(multiplier()) ;
                dst.time(time()) ;
                dst.clk_khz(clk_khz()) ;
            }

            public interface SRC_
            {
                byte multiplier();
                ushort time();
                ushort clk_khz();

            }

            public void pull_<SRC>(SRC src) where SRC: struct, SRC_
            {
                multiplier(src.multiplier());
                time(src.time());
                clk_khz(src.clk_khz());
            }



        }
        public struct InstructionsPack
        {
            internal Pack.Bytes  data_;
            internal InstructionsPack(Pack.Bytes  data) { this.data_ = data; }

            public byte Length() { return (byte)(data_.bytes[data_.origin + 0]); ;}
            public void Length(byte src) {    data_.bytes[data_.origin + 0] = (byte)(src); ; }

            public Instructions_ Instructions()
            {
                return new  Instructions_(data_);
            }

            public struct Instructions_
            {
                internal Pack.Bytes data_;
                public Instructions_(Pack.Bytes data) { this.data_ = data; }
                public int len() { return InstructionsPack.Instructions_.item_len  ;}
                public byte get(int index) { return (byte)(data_.bytes[data_.origin + 1 + index]); ;}

                public bool same(byte[] other)
                {
                    if(other.Length != InstructionsPack.Instructions_.item_len) return false;
                    for(int i = 0; i < InstructionsPack.Instructions_.item_len  ; i++) if(other[i] != get(i)) return false;
                    return true;
                }

                public bool same(Instructions_ other)
                {
                    if(other.len() != InstructionsPack.Instructions_.item_len) return false;
                    for(int i = 0; i < InstructionsPack.Instructions_.item_len ; i++)	if(other.get(i) != get(i)) return false ;
                    return true;
                }

                public void set(byte src, int index) {  data_.bytes[data_.origin + 1 + index] = (byte)(src); ;}

                public const int item_len = 256 ;


            }


            public void Instructions(byte[] src)
            {
                var len =  Math.Min(src.Length, InstructionsPack.Instructions_.item_len);
                for(int index = 0; index < len; index++)
                    data_.bytes[data_.origin + 1 + index] = (byte)(src[index]);;
            }



            public interface DST_
            {
                void Length(byte src);
                void Instructions(InstructionsPack.Instructions_ src);

            }


            internal static readonly Pack.Meta meta_ =  new Pack.Meta(7, 0, 0, 0, 257, 2056);
            const  short RW_mode = (short) 128;  //128


            public  void push_< DST >(DST dst) where DST : struct, DST_
            {
                dst.Length(Length()) ;
                dst.Instructions(Instructions());
            }

            public interface SRC_
            {
                byte Length();
                void Instructions(InstructionsPack.Instructions_ dst) ;

            }

            public void pull_<SRC>(SRC src) where SRC: struct, SRC_
            {
                Length(src.Length());
                src.Instructions(Instructions());
            }



        }
        public struct DeviceError
        {
            internal Pack.Bytes  data_;
            internal DeviceError(Pack.Bytes  data) { this.data_ = data; }

            public ushort param1() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 0, 2)); ;}


            public  Errors error_id() { return (Errors)(((sbyte)Lib.get_bits(data_.bytes, data_.origin * 8 + 16, 1))); ;}




            public interface DST_
            {
                void error_id(org.company.Client. Errors src);
                void param1(ushort src);

            }


            internal static readonly Pack.Meta meta_ =  new Pack.Meta(8, 0, 0, 0, 3, 17);


            public  void push_< DST >(DST dst) where DST : struct, DST_
            {
                dst.error_id(error_id()) ;
                dst.param1(param1()) ;
            }



        }
        public struct SensorsData
        {
            internal Pack.Bytes  data_;
            internal SensorsData(Pack.Bytes  data) { this.data_ = data; }

            public values_ values()
            {
                return new  values_(data_);
            }

            public struct values_
            {
                internal Pack.Bytes data_;
                public values_(Pack.Bytes data) { this.data_ = data; }
                public int len() { return SensorsData.values_.item_len  ;}
                public ushort get(int index) { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 0 + index * 2, 2)); ;}

                public bool same(ushort[] other)
                {
                    if(other.Length != SensorsData.values_.item_len) return false;
                    for(int i = 0; i < SensorsData.values_.item_len  ; i++) if(other[i] != get(i)) return false;
                    return true;
                }

                public bool same(values_ other)
                {
                    if(other.len() != SensorsData.values_.item_len) return false;
                    for(int i = 0; i < SensorsData.values_.item_len ; i++)	if(other.get(i) != get(i)) return false ;
                    return true;
                }



                public const int item_len = 1000 ;


            }





            public interface DST_
            {
                void values(SensorsData.values_ src);

            }


            internal static readonly Pack.Meta meta_ =  new Pack.Meta(9, 0, 0, 0, 2000, 16000);


            public  void push_< DST >(DST dst) where DST : struct, DST_
            {
                dst.values(values());
            }



        }

    }

    public abstract class Communication : Stream
    {
        public override void Flush() { }
        public override long Seek(long offset, SeekOrigin origin) { throw new NotSupportedException(); }
        public override void SetLength(long value) { throw new NotSupportedException(); }
        public override bool CanRead { get { return true  ; } }
        public override bool CanWrite { get { return true; } }
        public override bool CanSeek => false;
        public override long Length => throw new NotSupportedException();

        public override long Position
        {
            get => throw new NotSupportedException();
            set => throw new NotSupportedException();
        }

        public Communication()
        {
            receiver = new Receiver(this);
            transmitter = new Transmitter(this);
        }


        public override void Write(byte[] src, int offset, int bytes) { receiver.BytesIntoPacks(src, offset, bytes); }
        class Receiver : Lib.Channel.Receiver
        {
            readonly Communication ch;
            public Receiver(Communication ch): base(1, 1) { this.ch = ch; }

            protected internal override Pack.Meta dispatch(int id, Pack pack)
            {
                switch(id)
                {
                    case 0 :
                        if(pack == null)return DeviceVersion.meta_ ;
                        ch.onDeviceVersion(pack);
                        break;
                    case 6 :
                        if(pack == null)return BusConfiguration.meta_ ;
                        ch.onBusConfiguration(pack);
                        break;
                    case 7 :
                        if(pack == null)return InstructionsPack.meta_ ;
                        ch.onInstructionsPack(pack);
                        break;
                    case 8 :
                        if(pack == null)return DeviceError.meta_ ;
                        ch.onDeviceError(pack);
                        break;
                    case 9 :
                        if(pack == null)return SensorsData.meta_ ;
                        ch.onSensorsData(pack);
                        break;
                }
                return null;
            }
        }
        readonly Receiver receiver;
        protected abstract void onDeviceVersion(Pack pack);
        protected abstract void onBusConfiguration(Pack pack);
        protected abstract void onInstructionsPack(Pack pack);
        protected abstract void onDeviceError(Pack pack);
        protected abstract void onSensorsData(Pack pack);



        protected internal abstract Pack pullSendingPack();
        protected internal abstract bool pushSendingPack(Pack pack);
        public override int Read(byte[] dst, int offset, int bytes) { return transmitter.PacksIntoBytes(dst, offset, bytes); }
        class Transmitter : Lib.Channel.Transmitter
        {
            readonly Communication ch;
            public  Transmitter(Communication ch): base(1, 1) { this.ch = ch; }
            protected internal override Pack pullSendingPack() { return ch.pullSendingPack(); }
        }
        readonly Transmitter transmitter;

        public bool sendStop() { return  pushSendingPack(Stop.pack_); }
        public bool sendStart() { return  pushSendingPack(Start.pack_); }
        public bool sendGetDeviceVersion() { return  pushSendingPack(GetDeviceVersion.pack_); }
        public bool sendGetConfiguration() { return  pushSendingPack(GetConfiguration.pack_); }
        public bool sendSetConfiguration() { return  pushSendingPack(SetConfiguration.pack_); }

        public bool send(BusConfiguration src)
        {
            if(src.data_.bytes == null) return false;
            var pack = src.data_.unwrap();
            if(pushSendingPack(pack)) return true;
            src.data_.wrap(pack);
            return false;
        }

        public bool send(InstructionsPack src)
        {
            if(src.data_.bytes == null) return false;
            var pack = src.data_.unwrap();
            if(pushSendingPack(pack)) return true;
            src.data_.wrap(pack);
            return false;
        }

        public struct NEW
        {
            /**
            *Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */
            public static BusConfiguration BusConfiguration(Pack.Bytes bytes)

            {
                bytes.wrap(Client.BusConfiguration.meta_);
                return new BusConfiguration(bytes);
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
            public static InstructionsPack InstructionsPack(Pack.Bytes bytes)

            {
                bytes.wrap(Client.InstructionsPack.meta_);
                return new InstructionsPack(bytes);
            }

        }

    }

}
namespace org.unirail
{
    using org.company.Client;

    public struct Config_
    {
        public static Cursor Cursor() { return new Cursor(null, 1, 2);}

        public const int RECEIVE_REQ_MAX_BYTES = 2000;
        public const int RECEIVE_FULL_MAX_BYTES = 2000;



        public const int SEND_REQ_MAX_BYTES = 257;
        public const int SEND_FULL_MAX_BYTES = 257;



    }
}
