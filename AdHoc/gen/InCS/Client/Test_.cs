
using System;
using org.company;
using org.company.Client;
using org.unirail;
using System.Diagnostics;
using System.Linq;
using Cursor = org.unirail.AdHoc.Pack.Cursor;
using Pack = org.unirail.AdHoc.Pack;
using Lib = org.unirail.AdHoc;
using System.IO;
using System.Text;

namespace org.unirail
{
    public struct Test
    {

        public struct DeviceVersion
        {
            internal Pack.Bytes  data_;
            internal DeviceVersion(Pack.Bytes  data) { this.data_ = data; }

            public ushort DevID() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 0, 2)); ;}
            public void DevID(ushort src) {   Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 0); ; }

            public ushort Ver() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 2, 2)); ;}
            public void Ver(ushort src) {   Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 2); ; }

        }
        public struct DeviceVersion_dst_adapter : org.company.Client. DeviceVersion.DST_
        {
            DeviceVersion dst;
            public DeviceVersion_dst_adapter(DeviceVersion dst) { this.dst = dst; }
            public void DevID(ushort src) { dst.DevID(src); }
            public void Ver(ushort src) { dst.Ver(src); }

        }

        public struct BusConfiguration_dst_adapter : BusConfiguration.DST_
        {
            BusConfiguration dst;
            public BusConfiguration_dst_adapter(BusConfiguration dst) { this.dst = dst; }
            public void multiplier(byte src) { dst.multiplier(src); }
            public void time(ushort src) { dst.time(src); }
            public void clk_khz(ushort src) { dst.clk_khz(src); }

        }

        public struct BusConfiguration_src_adapter : BusConfiguration.SRC_
        {
            BusConfiguration src;
            public BusConfiguration_src_adapter(BusConfiguration src) { this.src = src; }
            public byte multiplier() { return src.multiplier();}
            public ushort time() { return src.time();}
            public ushort clk_khz() { return src.clk_khz();}

        }

        public struct InstructionsPack_dst_adapter : InstructionsPack.DST_
        {
            InstructionsPack dst;
            public InstructionsPack_dst_adapter(InstructionsPack dst) { this.dst = dst; }
            public void Length(byte src) { dst.Length(src); }
            public void Instructions(InstructionsPack.Instructions_ src)
            {
                var item = dst.Instructions();
                for(int i = 0; i < InstructionsPack.Instructions_.item_len ; i++)
                    item.set(src.get(i), i);
            }

        }

        public struct InstructionsPack_src_adapter : InstructionsPack.SRC_
        {
            InstructionsPack src;
            public InstructionsPack_src_adapter(InstructionsPack src) { this.src = src; }
            public byte Length() { return src.Length();}
            public void Instructions(InstructionsPack.Instructions_ dst)
            {
                var item = src.Instructions();
                for(int i = 0; i < InstructionsPack.Instructions_.item_len ; i++)
                    dst.set(item.get(i), i);
            }

        }
        public struct DeviceError
        {
            internal Pack.Bytes  data_;
            internal DeviceError(Pack.Bytes  data) { this.data_ = data; }

            public ushort param1() { return (ushort)((ushort)(ushort)Lib.get_bytes(data_.bytes,  data_.origin + 0, 2)); ;}
            public void param1(ushort src) {   Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 0); ; }

            public  Errors error_id() { return (Errors)(((sbyte)Lib.get_bits(data_.bytes, data_.origin * 8 + 16, 1))); ;}
            public void error_id(Errors src) {   Lib.set_bits((ulong)(src), 1, data_.bytes, data_.origin * 8 + 16); ; }

        }
        public struct DeviceError_dst_adapter : org.company.Client. DeviceError.DST_
        {
            DeviceError dst;
            public DeviceError_dst_adapter(DeviceError dst) { this.dst = dst; }
            public void error_id(org.company.Client. Errors src) { dst.error_id(src); }
            public void param1(ushort src) { dst.param1(src); }

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

                public void set(ushort src, int index) { Lib.set_bytes((ulong)(src), 2, data_.bytes,  data_.origin + 0 + index * 2); ;}

                public const int item_len = 1000 ;


            }


            public void values(ushort[] src)
            {
                var len =  Math.Min(src.Length, SensorsData.values_.item_len);
                for(int index = 0; index < len; index++)
                    Lib.set_bytes((ulong)(src[index]), 2, data_.bytes,  data_.origin + 0 + index * 2);;
            }

        }
        public struct SensorsData_dst_adapter : org.company.Client. SensorsData.DST_
        {
            SensorsData dst;
            public SensorsData_dst_adapter(SensorsData dst) { this.dst = dst; }
            public void values(org.company.Client.SensorsData.values_ src)
            {
                var item = dst.values();
                for(int i = 0; i < org.company.Client.SensorsData.values_.item_len ; i++)
                    item.set(src.get(i), i);
            }

        }




        class TestChannel : Stream
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

            private bool is_advanced = false;

            public TestChannel Adv
            {
                get
                {
                    is_advanced = true;
                    return this;
                }
            }

            public TestChannel Std
            {
                get
                {
                    is_advanced = false;
                    return this;
                }
            }

            public TestChannel()
            {
                receiver = new Receiver(this);
                receiverAdv = new ReceiverAdv(this);
                transmitter = new Transmitter(this);
                transmitterAdv = new TransmitterAdv(this);
            }

            protected internal  Pack.Meta dispatch(int id, Pack pack)
            {
                switch(id)
                {
                    case 1:
                        if(pack == null) return org.company.Client.Stop.pack_.meta;
                        onStop();
                        return null;
                    case 2:
                        if(pack == null) return org.company.Client.Start.pack_.meta;
                        onStart();
                        return null;
                    case 3:
                        if(pack == null) return org.company.Client.GetDeviceVersion.pack_.meta;
                        onGetDeviceVersion();
                        return null;
                    case 4:
                        if(pack == null) return org.company.Client.GetConfiguration.pack_.meta;
                        onGetConfiguration();
                        return null;
                    case 5:
                        if(pack == null) return org.company.Client.SetConfiguration.pack_.meta;
                        onSetConfiguration();
                        return null;
                }
                return null;
            }

            public override void Write(byte[] src, int offset, int bytes)
            {
                if(is_advanced)
                    receiverAdv.BytesIntoPacks(src, offset, bytes);
                else
                    receiver.BytesIntoPacks(src, offset, bytes);
            }

            class Receiver : Lib.Channel.Receiver
            {
                readonly TestChannel ch;
                public Receiver(TestChannel ch): base(1, 1) { this.ch = ch; }
                protected internal override Pack.Meta dispatch(int id, Pack pack)  { return ch.dispatch(id, pack) ;}
            }
            readonly Receiver receiver;

            class ReceiverAdv : Lib.Channel.Receiver.Advanced
            {
                readonly TestChannel ch;
                public ReceiverAdv(TestChannel ch): base(1, 1) { this.ch = ch; }
                protected internal override Pack.Meta dispatch(int id, Pack pack)  { return ch.dispatch(id, pack); }
            }
            readonly ReceiverAdv receiverAdv;


            Pack sendingPack = null;

            public override int Read(byte[] dst, int offset, int bytes)
            {
                return is_advanced ? transmitterAdv.PacksIntoBytes(dst, offset, bytes) : transmitter.PacksIntoBytes(dst, offset, bytes);
            }

            class Transmitter : Lib.Channel.Transmitter
            {
                readonly TestChannel ch;
                public  Transmitter(TestChannel ch): base(1, 1) { this.ch = ch; }
                protected internal override Pack pullSendingPack()
                {
                    var ret = ch.sendingPack;
                    ch.sendingPack = null;
                    return ret;
                }
            }
            readonly Transmitter transmitter;

            class TransmitterAdv : Lib.Channel.Transmitter.Advanced
            {
                readonly TestChannel ch;
                public  TransmitterAdv(TestChannel ch): base(1, 1) { this.ch = ch; }
                protected internal override Pack pullSendingPack()
                {
                    var ret = ch.sendingPack;
                    ch.sendingPack = null;
                    return ret;
                }
            }
            readonly TransmitterAdv transmitterAdv;


            public bool  send(org.company.Client.DeviceVersion src)
            {
                if(sendingPack != null) return false;
                sendingPack = src.data_.unwrap();
                return  true;
            }

            public bool  send(org.company.Client.DeviceError src)
            {
                if(sendingPack != null) return false;
                sendingPack = src.data_.unwrap();
                return  true;
            }

            public bool  send(org.company.Client.SensorsData src)
            {
                if(sendingPack != null) return false;
                sendingPack = src.data_.unwrap();
                return  true;
            }


            public struct NEW
            {
                public static org.unirail.Test.DeviceVersion DeviceVersion(Pack.Bytes bytes)
                {
                    bytes.wrap(org.company.Client. DeviceVersion.meta_);
                    return new org.unirail.Test.DeviceVersion(bytes);
                }
                public static org.unirail.Test.DeviceError DeviceError(Pack.Bytes bytes)
                {
                    bytes.wrap(org.company.Client. DeviceError.meta_);
                    return new org.unirail.Test.DeviceError(bytes);
                }
                public static org.unirail.Test.SensorsData SensorsData(Pack.Bytes bytes)
                {
                    bytes.wrap(org.company.Client. SensorsData.meta_);
                    return new org.unirail.Test.SensorsData(bytes);
                }

            }

        }



        public static void fill(org.unirail.Test. DeviceVersion  p0)
        {
            p0.DevID(15212)  ;
            p0.Ver(4469)  ;
        }

        public static void onDeviceVersion(org.company.Client.DeviceVersion p0)
        {
            Debug.Assert(p0.DevID() == 15212) ;
            Debug.Assert(p0.Ver() == 4469) ;
            Debug.Print("DeviceVersion \n");
        }
        public static void onStop() {Debug.Print("Stop \n"); }
        public static void onStart() {Debug.Print("Start \n"); }
        public static void onGetDeviceVersion() {Debug.Print("GetDeviceVersion \n"); }
        public static void onGetConfiguration() {Debug.Print("GetConfiguration \n"); }
        public static void onSetConfiguration() {Debug.Print("SetConfiguration \n"); }

        public static void fill(org.company.Client. BusConfiguration  p6)
        {
            p6.multiplier(204)  ;
            p6.time(61623)  ;
            p6.clk_khz(62428)  ;
        }

        public static void onBusConfiguration(org.company.Client.BusConfiguration p6)
        {
            Debug.Assert(p6.multiplier() == 204) ;
            Debug.Assert(p6.time() == 61623) ;
            Debug.Assert(p6.clk_khz() == 62428) ;
            Debug.Print("BusConfiguration \n");
        }

        public static void fill(org.company.Client. InstructionsPack  p7)
        {
            p7.Length(145)  ;
            p7.Instructions(new byte [] { 218, 120, 49, 29, 133, 54, 164, 209, 187, 212, 28, 28, 67, 221, 110, 235, 220, 100, 138, 12, 120, 74, 35, 9, 181, 160, 24, 179, 37, 175, 27, 14, 156, 110, 55, 131, 60, 203, 124, 24, 46, 187, 146, 149, 139, 151, 178, 231, 234, 130, 111, 51, 237, 185, 150, 16, 142, 179, 60, 211, 164, 156, 79, 238, 180, 176, 238, 172, 80, 198, 248, 102, 170, 250, 45, 2, 63, 185, 248, 4, 130, 237, 184, 185, 104, 27, 94, 200, 242, 243, 222, 167, 35, 233, 230, 177, 202, 14, 94, 246, 167, 83, 73, 196, 20, 253, 70, 40, 157, 13, 185, 150, 37, 186, 106, 35, 192, 225, 42, 132, 85, 144, 120, 235, 228, 131, 223, 145, 254, 23, 141, 223, 238, 221, 35, 21, 67, 85, 195, 34, 52, 85, 77, 9, 105, 169, 220, 146, 100, 41, 45, 21, 132, 226, 49, 51, 172, 15, 122, 68, 128, 197, 125, 40, 98, 114, 196, 107, 126, 208, 90, 235, 22, 194, 22, 73, 45, 142, 5, 76, 100, 167, 91, 121, 104, 177, 243, 65, 184, 7, 112, 219, 89, 75, 214, 73, 154, 85, 253, 180, 63, 248, 127, 96, 244, 202, 16, 108, 228, 125, 114, 255, 189, 78, 212, 17, 201, 24, 95, 184, 207, 13, 100, 199, 141, 177, 255, 71, 185, 2, 43, 165, 102, 27, 128, 38, 12, 46, 103, 202, 218, 16, 17, 56, 194, 51, 173, 195, 132, 193, 95, 49, 94, 26, 185, 89 })  ;
        }

        public static void onInstructionsPack(org.company.Client.InstructionsPack p7)
        {
            Debug.Assert(p7.Length() == 145) ;
            Debug.Assert(p7.Instructions().same(new byte [] { 218, 120, 49, 29, 133, 54, 164, 209, 187, 212, 28, 28, 67, 221, 110, 235, 220, 100, 138, 12, 120, 74, 35, 9, 181, 160, 24, 179, 37, 175, 27, 14, 156, 110, 55, 131, 60, 203, 124, 24, 46, 187, 146, 149, 139, 151, 178, 231, 234, 130, 111, 51, 237, 185, 150, 16, 142, 179, 60, 211, 164, 156, 79, 238, 180, 176, 238, 172, 80, 198, 248, 102, 170, 250, 45, 2, 63, 185, 248, 4, 130, 237, 184, 185, 104, 27, 94, 200, 242, 243, 222, 167, 35, 233, 230, 177, 202, 14, 94, 246, 167, 83, 73, 196, 20, 253, 70, 40, 157, 13, 185, 150, 37, 186, 106, 35, 192, 225, 42, 132, 85, 144, 120, 235, 228, 131, 223, 145, 254, 23, 141, 223, 238, 221, 35, 21, 67, 85, 195, 34, 52, 85, 77, 9, 105, 169, 220, 146, 100, 41, 45, 21, 132, 226, 49, 51, 172, 15, 122, 68, 128, 197, 125, 40, 98, 114, 196, 107, 126, 208, 90, 235, 22, 194, 22, 73, 45, 142, 5, 76, 100, 167, 91, 121, 104, 177, 243, 65, 184, 7, 112, 219, 89, 75, 214, 73, 154, 85, 253, 180, 63, 248, 127, 96, 244, 202, 16, 108, 228, 125, 114, 255, 189, 78, 212, 17, 201, 24, 95, 184, 207, 13, 100, 199, 141, 177, 255, 71, 185, 2, 43, 165, 102, 27, 128, 38, 12, 46, 103, 202, 218, 16, 17, 56, 194, 51, 173, 195, 132, 193, 95, 49, 94, 26, 185, 89 }));
            Debug.Print("InstructionsPack \n");
        }

        public static void fill(org.unirail.Test. DeviceError  p8)
        {
            p8.param1(41224)  ;
            p8.error_id(Errors.CannotReadSensor)  ;
        }

        public static void onDeviceError(org.company.Client.DeviceError p8)
        {
            Debug.Assert(p8.param1() == 41224) ;
            Debug.Assert(p8.error_id() == Errors.CannotReadSensor) ;
            Debug.Print("DeviceError \n");
        }

        public static void fill(org.unirail.Test. SensorsData  p9)
        {
            p9.values(new ushort [] { 55746, 23315, 37349, 25727, 64778, 40908, 29937, 4217, 42025, 12462, 1630, 34828, 33363, 5264, 27914, 28045, 53480, 52617, 20420, 57828, 50846, 26898, 13215, 55023, 49437, 32957, 17174, 39468, 4813, 59669, 9026, 7942, 35658, 62465, 16772, 35618, 34873, 21317, 43406, 55499, 43989, 61483, 57180, 28568, 48394, 25527, 48912, 5257, 41169, 58676, 38198, 44615, 23558, 41052, 18000, 64729, 25091, 41364, 25287, 7172, 22980, 32746, 55816, 16427, 28889, 31664, 56233, 33886, 12692, 28234, 36197, 43455, 34938, 48512, 33813, 45154, 31683, 12136, 837, 29930, 8932, 58610, 6665, 1768, 42450, 17673, 61932, 30108, 11488, 62614, 20919, 27036, 44723, 32804, 41181, 37493, 21208, 5279, 38738, 55295, 19719, 29230, 5310, 40000, 17678, 10113, 29162, 19521, 37, 63643, 59198, 56546, 20683, 48661, 13435, 42698, 1046, 17906, 35214, 23205, 29158, 49584, 31457, 40960, 45906, 23459, 45408, 26579, 7643, 47404, 41974, 53702, 30741, 49880, 22718, 59992, 51116, 41416, 16487, 12878, 53479, 42325, 36507, 12466, 30353, 1847, 32263, 45421, 20866, 21022, 35562, 4739, 41067, 29276, 12578, 33354, 27912, 53293, 40604, 41370, 49345, 31272, 28174, 23186, 18861, 4220, 863, 23928, 23316, 6270, 23492, 64085, 19598, 14196, 37448, 3752, 15034, 5331, 2728, 31319, 31219, 58823, 27246, 12117, 5361, 55001, 29159, 40842, 15901, 35601, 41171, 25963, 30247, 44457, 2228, 15071, 14955, 25246, 45644, 47427, 33311, 33192, 61898, 29731, 5840, 57932, 2042, 33329, 15699, 8970, 57419, 54518, 60373, 24521, 31998, 1769, 50451, 23101, 64878, 53475, 40244, 58045, 19688, 16305, 59034, 2048, 14349, 47528, 36134, 31285, 58223, 41066, 21742, 32976, 5505, 51063, 51778, 26684, 36956, 17892, 39073, 2070, 51461, 23281, 63738, 40454, 43412, 31796, 8713, 23681, 55459, 24688, 62244, 45789, 33288, 42081, 20748, 49737, 48112, 56954, 63146, 56288, 29740, 59851, 32039, 25648, 40994, 6143, 42975, 51431, 18419, 17065, 30411, 54818, 5759, 19411, 31035, 9028, 5979, 63660, 58014, 31206, 45696, 20060, 51780, 43691, 54267, 41074, 24212, 11980, 28252, 10950, 40270, 49065, 47559, 31602, 38678, 31230, 39184, 49268, 65223, 45373, 22064, 58486, 54652, 40268, 16373, 62342, 16334, 45460, 44826, 23009, 60459, 38602, 31706, 12656, 33271, 54069, 12798, 56334, 46195, 37244, 6946, 35711, 61309, 22960, 10050, 53317, 21327, 48127, 1280, 7174, 32866, 30155, 20617, 16075, 24411, 57869, 58467, 6629, 5484, 16530, 15928, 65522, 23817, 50778, 46633, 11095, 56806, 26537, 48869, 60043, 27084, 41502, 40509, 60764, 15636, 46306, 20847, 50026, 50560, 43264, 51731, 34181, 22254, 14219, 30288, 46072, 32833, 27074, 14173, 7451, 60817, 29836, 58785, 52466, 7042, 19759, 23423, 19902, 12417, 3445, 13925, 50710, 37672, 4722, 52407, 29288, 22061, 15840, 64865, 45367, 32523, 16598, 4182, 8690, 23783, 61000, 17102, 2127, 3833, 33127, 60342, 12465, 55296, 30146, 32213, 7815, 6206, 61576, 6096, 2595, 22913, 31894, 40038, 40642, 59619, 45629, 53609, 34386, 57087, 136, 17398, 63523, 4196, 3971, 21405, 63190, 32641, 4617, 34007, 24971, 28391, 52403, 28473, 49419, 27081, 17360, 38857, 44995, 45953, 22823, 5639, 9047, 4244, 44237, 40532, 15558, 50429, 51260, 52383, 15600, 47192, 42144, 15064, 16442, 41500, 24343, 13741, 64000, 27955, 26791, 61465, 6236, 63650, 54650, 2327, 6174, 18711, 59061, 41164, 65101, 62058, 35414, 59908, 31616, 61715, 18273, 51833, 41183, 24594, 4674, 18791, 5790, 34602, 59734, 61591, 42245, 64946, 34808, 11592, 26101, 56983, 9228, 11291, 17750, 32572, 34329, 4840, 35137, 11016, 18456, 10195, 16056, 7681, 65376, 63064, 30179, 49071, 18574, 35549, 27719, 48024, 42744, 28029, 20366, 2972, 16880, 11215, 46279, 53229, 9593, 61421, 39422, 43607, 49799, 17837, 8912, 56337, 11007, 49777, 878, 24813, 35026, 38038, 30069, 33026, 58709, 46605, 36511, 5038, 21552, 7070, 37089, 8445, 21513, 36512, 64005, 30914, 20423, 58837, 57307, 15671, 53659, 38433, 40920, 63083, 63565, 41993, 4106, 7768, 5313, 62483, 13272, 61329, 48874, 37205, 55763, 51066, 4899, 3764, 9230, 36372, 51958, 56037, 14037, 55738, 26397, 1612, 51421, 25742, 43841, 15749, 16314, 13154, 20950, 52514, 403, 55835, 55788, 13223, 7790, 38681, 40122, 49936, 41319, 34657, 41041, 48199, 59733, 24821, 32371, 20310, 22741, 37522, 61937, 3362, 39669, 16036, 59856, 60567, 64866, 29059, 24578, 23543, 40169, 7560, 1398, 41971, 49660, 37037, 27600, 37990, 44152, 30025, 9299, 4714, 37591, 17864, 34610, 45784, 10296, 5301, 29732, 8144, 30239, 14231, 26557, 14333, 20333, 49742, 61954, 53463, 45239, 7643, 9317, 12285, 27717, 18884, 28596, 34938, 6866, 62481, 53607, 572, 35485, 13362, 56406, 35664, 64659, 57410, 35651, 2460, 15608, 29437, 58537, 34556, 45561, 5799, 1954, 46483, 41131, 60923, 15393, 57447, 23064, 37382, 58904, 41960, 37853, 61631, 57167, 51846, 64636, 2898, 40304, 53243, 50193, 58454, 29563, 978, 52550, 12490, 39390, 4625, 50815, 10340, 1009, 9462, 52148, 37962, 15597, 43996, 44902, 56063, 36291, 63772, 3294, 60295, 59815, 31414, 49325, 49458, 61039, 17338, 54117, 44584, 1363, 27071, 13963, 58459, 50506, 23344, 41545, 50109, 58863, 4585, 6060, 10391, 57631, 53477, 18466, 57998, 30712, 43741, 46991, 47060, 25924, 61370, 25268, 25520, 404, 16984, 6619, 23240, 27534, 19233, 57181, 3048, 20108, 9159, 6759, 7472, 40730, 45230, 33267, 6855, 1881, 63829, 10577, 27331, 50126, 22584, 12858, 28232, 33342, 46138, 58118, 9643, 40973, 20197, 58820, 55187, 23834, 12274, 11093, 62209, 28785, 20354, 22702, 40478, 18895, 60889, 2493, 62217, 30105, 41951, 25727, 63120, 51920, 3455, 50926, 14870, 52524, 17142, 18422, 57608, 54893, 19881, 49181, 62778, 53897, 33044, 8360, 21201, 43269, 33412, 28885, 60350, 26859, 35827, 26593, 57629, 2034, 32250, 30488, 34208, 9806, 53756, 49233, 49337, 28677, 55914, 43475, 50110, 49508, 43670, 58067, 16273, 35885, 7051, 61434, 41814, 34702, 2332, 49831, 42420, 26689, 64413, 47836, 56274, 4377, 22499, 28177, 16603, 6943, 15768, 11137, 14081, 53781, 1392, 41121, 20464, 51074, 52449, 11826, 34781, 45370, 42446, 25434, 32628, 21319, 62487, 61367, 5563, 34057, 63872, 57002, 64864, 24710, 10949, 40080, 7771, 18053, 55843, 37078, 21086, 32700, 58308, 48204, 1728, 7929, 32205, 38004, 43825, 25623, 52680, 44872, 54583, 62597, 37087, 46082, 8088, 21172, 9015, 43394, 24318, 61613, 35469, 23212, 58359, 23365, 40157, 15660, 45961, 46195, 20020, 59782, 16822, 3328, 50231, 64448, 31531, 10953, 29144, 45865, 34851, 52632, 3464, 3826, 24450, 65518, 26756, 37009, 11298, 13704, 55207, 47363, 54691, 59067, 22711, 41013, 56149, 59151, 46537, 10366, 33504, 23629, 34070, 21563, 38244, 53690, 54060, 12986, 60880, 45201, 4188, 50436, 55756, 38884, 17559, 61192, 27612, 10473, 51046, 28850, 12794, 59250, 45268, 12926, 61015, 7021, 21250, 44332, 50668, 55907, 44456, 58998, 38432, 6243, 47691, 19156, 34900, 59935, 9792, 7685, 37019, 21313, 59914, 21264, 613, 19833, 28920, 50973, 25266, 177, 57475, 34867, 59979, 58983, 13122, 15785, 11934, 64142, 62419, 1668, 255, 51965, 24681, 57474 })  ;
        }

        public static void onSensorsData(org.company.Client.SensorsData p9)
        {
            Debug.Assert(p9.values().same(new ushort [] { 55746, 23315, 37349, 25727, 64778, 40908, 29937, 4217, 42025, 12462, 1630, 34828, 33363, 5264, 27914, 28045, 53480, 52617, 20420, 57828, 50846, 26898, 13215, 55023, 49437, 32957, 17174, 39468, 4813, 59669, 9026, 7942, 35658, 62465, 16772, 35618, 34873, 21317, 43406, 55499, 43989, 61483, 57180, 28568, 48394, 25527, 48912, 5257, 41169, 58676, 38198, 44615, 23558, 41052, 18000, 64729, 25091, 41364, 25287, 7172, 22980, 32746, 55816, 16427, 28889, 31664, 56233, 33886, 12692, 28234, 36197, 43455, 34938, 48512, 33813, 45154, 31683, 12136, 837, 29930, 8932, 58610, 6665, 1768, 42450, 17673, 61932, 30108, 11488, 62614, 20919, 27036, 44723, 32804, 41181, 37493, 21208, 5279, 38738, 55295, 19719, 29230, 5310, 40000, 17678, 10113, 29162, 19521, 37, 63643, 59198, 56546, 20683, 48661, 13435, 42698, 1046, 17906, 35214, 23205, 29158, 49584, 31457, 40960, 45906, 23459, 45408, 26579, 7643, 47404, 41974, 53702, 30741, 49880, 22718, 59992, 51116, 41416, 16487, 12878, 53479, 42325, 36507, 12466, 30353, 1847, 32263, 45421, 20866, 21022, 35562, 4739, 41067, 29276, 12578, 33354, 27912, 53293, 40604, 41370, 49345, 31272, 28174, 23186, 18861, 4220, 863, 23928, 23316, 6270, 23492, 64085, 19598, 14196, 37448, 3752, 15034, 5331, 2728, 31319, 31219, 58823, 27246, 12117, 5361, 55001, 29159, 40842, 15901, 35601, 41171, 25963, 30247, 44457, 2228, 15071, 14955, 25246, 45644, 47427, 33311, 33192, 61898, 29731, 5840, 57932, 2042, 33329, 15699, 8970, 57419, 54518, 60373, 24521, 31998, 1769, 50451, 23101, 64878, 53475, 40244, 58045, 19688, 16305, 59034, 2048, 14349, 47528, 36134, 31285, 58223, 41066, 21742, 32976, 5505, 51063, 51778, 26684, 36956, 17892, 39073, 2070, 51461, 23281, 63738, 40454, 43412, 31796, 8713, 23681, 55459, 24688, 62244, 45789, 33288, 42081, 20748, 49737, 48112, 56954, 63146, 56288, 29740, 59851, 32039, 25648, 40994, 6143, 42975, 51431, 18419, 17065, 30411, 54818, 5759, 19411, 31035, 9028, 5979, 63660, 58014, 31206, 45696, 20060, 51780, 43691, 54267, 41074, 24212, 11980, 28252, 10950, 40270, 49065, 47559, 31602, 38678, 31230, 39184, 49268, 65223, 45373, 22064, 58486, 54652, 40268, 16373, 62342, 16334, 45460, 44826, 23009, 60459, 38602, 31706, 12656, 33271, 54069, 12798, 56334, 46195, 37244, 6946, 35711, 61309, 22960, 10050, 53317, 21327, 48127, 1280, 7174, 32866, 30155, 20617, 16075, 24411, 57869, 58467, 6629, 5484, 16530, 15928, 65522, 23817, 50778, 46633, 11095, 56806, 26537, 48869, 60043, 27084, 41502, 40509, 60764, 15636, 46306, 20847, 50026, 50560, 43264, 51731, 34181, 22254, 14219, 30288, 46072, 32833, 27074, 14173, 7451, 60817, 29836, 58785, 52466, 7042, 19759, 23423, 19902, 12417, 3445, 13925, 50710, 37672, 4722, 52407, 29288, 22061, 15840, 64865, 45367, 32523, 16598, 4182, 8690, 23783, 61000, 17102, 2127, 3833, 33127, 60342, 12465, 55296, 30146, 32213, 7815, 6206, 61576, 6096, 2595, 22913, 31894, 40038, 40642, 59619, 45629, 53609, 34386, 57087, 136, 17398, 63523, 4196, 3971, 21405, 63190, 32641, 4617, 34007, 24971, 28391, 52403, 28473, 49419, 27081, 17360, 38857, 44995, 45953, 22823, 5639, 9047, 4244, 44237, 40532, 15558, 50429, 51260, 52383, 15600, 47192, 42144, 15064, 16442, 41500, 24343, 13741, 64000, 27955, 26791, 61465, 6236, 63650, 54650, 2327, 6174, 18711, 59061, 41164, 65101, 62058, 35414, 59908, 31616, 61715, 18273, 51833, 41183, 24594, 4674, 18791, 5790, 34602, 59734, 61591, 42245, 64946, 34808, 11592, 26101, 56983, 9228, 11291, 17750, 32572, 34329, 4840, 35137, 11016, 18456, 10195, 16056, 7681, 65376, 63064, 30179, 49071, 18574, 35549, 27719, 48024, 42744, 28029, 20366, 2972, 16880, 11215, 46279, 53229, 9593, 61421, 39422, 43607, 49799, 17837, 8912, 56337, 11007, 49777, 878, 24813, 35026, 38038, 30069, 33026, 58709, 46605, 36511, 5038, 21552, 7070, 37089, 8445, 21513, 36512, 64005, 30914, 20423, 58837, 57307, 15671, 53659, 38433, 40920, 63083, 63565, 41993, 4106, 7768, 5313, 62483, 13272, 61329, 48874, 37205, 55763, 51066, 4899, 3764, 9230, 36372, 51958, 56037, 14037, 55738, 26397, 1612, 51421, 25742, 43841, 15749, 16314, 13154, 20950, 52514, 403, 55835, 55788, 13223, 7790, 38681, 40122, 49936, 41319, 34657, 41041, 48199, 59733, 24821, 32371, 20310, 22741, 37522, 61937, 3362, 39669, 16036, 59856, 60567, 64866, 29059, 24578, 23543, 40169, 7560, 1398, 41971, 49660, 37037, 27600, 37990, 44152, 30025, 9299, 4714, 37591, 17864, 34610, 45784, 10296, 5301, 29732, 8144, 30239, 14231, 26557, 14333, 20333, 49742, 61954, 53463, 45239, 7643, 9317, 12285, 27717, 18884, 28596, 34938, 6866, 62481, 53607, 572, 35485, 13362, 56406, 35664, 64659, 57410, 35651, 2460, 15608, 29437, 58537, 34556, 45561, 5799, 1954, 46483, 41131, 60923, 15393, 57447, 23064, 37382, 58904, 41960, 37853, 61631, 57167, 51846, 64636, 2898, 40304, 53243, 50193, 58454, 29563, 978, 52550, 12490, 39390, 4625, 50815, 10340, 1009, 9462, 52148, 37962, 15597, 43996, 44902, 56063, 36291, 63772, 3294, 60295, 59815, 31414, 49325, 49458, 61039, 17338, 54117, 44584, 1363, 27071, 13963, 58459, 50506, 23344, 41545, 50109, 58863, 4585, 6060, 10391, 57631, 53477, 18466, 57998, 30712, 43741, 46991, 47060, 25924, 61370, 25268, 25520, 404, 16984, 6619, 23240, 27534, 19233, 57181, 3048, 20108, 9159, 6759, 7472, 40730, 45230, 33267, 6855, 1881, 63829, 10577, 27331, 50126, 22584, 12858, 28232, 33342, 46138, 58118, 9643, 40973, 20197, 58820, 55187, 23834, 12274, 11093, 62209, 28785, 20354, 22702, 40478, 18895, 60889, 2493, 62217, 30105, 41951, 25727, 63120, 51920, 3455, 50926, 14870, 52524, 17142, 18422, 57608, 54893, 19881, 49181, 62778, 53897, 33044, 8360, 21201, 43269, 33412, 28885, 60350, 26859, 35827, 26593, 57629, 2034, 32250, 30488, 34208, 9806, 53756, 49233, 49337, 28677, 55914, 43475, 50110, 49508, 43670, 58067, 16273, 35885, 7051, 61434, 41814, 34702, 2332, 49831, 42420, 26689, 64413, 47836, 56274, 4377, 22499, 28177, 16603, 6943, 15768, 11137, 14081, 53781, 1392, 41121, 20464, 51074, 52449, 11826, 34781, 45370, 42446, 25434, 32628, 21319, 62487, 61367, 5563, 34057, 63872, 57002, 64864, 24710, 10949, 40080, 7771, 18053, 55843, 37078, 21086, 32700, 58308, 48204, 1728, 7929, 32205, 38004, 43825, 25623, 52680, 44872, 54583, 62597, 37087, 46082, 8088, 21172, 9015, 43394, 24318, 61613, 35469, 23212, 58359, 23365, 40157, 15660, 45961, 46195, 20020, 59782, 16822, 3328, 50231, 64448, 31531, 10953, 29144, 45865, 34851, 52632, 3464, 3826, 24450, 65518, 26756, 37009, 11298, 13704, 55207, 47363, 54691, 59067, 22711, 41013, 56149, 59151, 46537, 10366, 33504, 23629, 34070, 21563, 38244, 53690, 54060, 12986, 60880, 45201, 4188, 50436, 55756, 38884, 17559, 61192, 27612, 10473, 51046, 28850, 12794, 59250, 45268, 12926, 61015, 7021, 21250, 44332, 50668, 55907, 44456, 58998, 38432, 6243, 47691, 19156, 34900, 59935, 9792, 7685, 37019, 21313, 59914, 21264, 613, 19833, 28920, 50973, 25266, 177, 57475, 34867, 59979, 58983, 13122, 15785, 11934, 64142, 62419, 1668, 255, 51965, 24681, 57474 }));
            Debug.Print("SensorsData \n");
        }


        public class Communication_test: Communication
        {
            protected override void onDeviceVersion(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                org.unirail.Test. onDeviceVersion(new org.company.Client.DeviceVersion(data));
            }
            protected override void onBusConfiguration(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                org.unirail.Test. onBusConfiguration(new org.company.Client.BusConfiguration(data));
            }
            protected override void onInstructionsPack(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                org.unirail.Test. onInstructionsPack(new org.company.Client.InstructionsPack(data));
            }
            protected override void onDeviceError(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                org.unirail.Test. onDeviceError(new org.company.Client.DeviceError(data));
            }
            protected override void onSensorsData(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                org.unirail.Test. onSensorsData(new org.company.Client.SensorsData(data));
            }
            Pack sendingPack = null;
            protected internal override Pack pullSendingPack()
            {
                var pack = sendingPack;
                sendingPack = null;
                return pack;
            }
            protected internal override bool pushSendingPack(Pack pack)
            {
                if(sendingPack != null) return false;
                sendingPack = pack;
                return true;
            }

        }

        public static void Main(string[] args)
        {
            var cur = Config_.Cursor();
            var cur_dst = Config_.Cursor();
            var TestChannel_instance = new TestChannel();
            var Communication_instance = new Communication_test();
            Debug.Print("-------------------- DeviceVersion -------------------------");
            fill(TestChannel.NEW.DeviceVersion(cur));
            var p0 = new  org.company.Client.DeviceVersion(cur);
            onDeviceVersion(p0);
            {
                cur_dst.wrap(org.company.Client.DeviceVersion.meta_);
                var dst = new org.unirail.Test. DeviceVersion(cur_dst);
                p0.push_(new DeviceVersion_dst_adapter(dst));
                Debug.Assert(p0.data_.bytes.SequenceEqual(dst.data_.bytes));
            }
            if(TestChannel_instance.send(p0)) TestChannel_instance.Std.CopyTo(Communication_instance);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- Stop -------------------------");
            if(Communication_instance.sendStop()) Communication_instance.CopyTo(TestChannel_instance.Std);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- Start -------------------------");
            if(Communication_instance.sendStart()) Communication_instance.CopyTo(TestChannel_instance.Std);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- GetDeviceVersion -------------------------");
            if(Communication_instance.sendGetDeviceVersion()) Communication_instance.CopyTo(TestChannel_instance.Std);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- GetConfiguration -------------------------");
            if(Communication_instance.sendGetConfiguration()) Communication_instance.CopyTo(TestChannel_instance.Std);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- SetConfiguration -------------------------");
            if(Communication_instance.sendSetConfiguration()) Communication_instance.CopyTo(TestChannel_instance.Std);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- BusConfiguration -------------------------");
            var p6 = Communication.NEW.BusConfiguration(cur);
            fill(p6);
            onBusConfiguration(p6);
            {
                var dst = Communication.NEW.BusConfiguration(cur_dst);
                p6.push_(new BusConfiguration_dst_adapter(dst));
                Debug.Assert(p6.data_.bytes.SequenceEqual(dst.data_.bytes));
                dst = Communication.NEW.BusConfiguration(cur_dst);
                dst.pull_(new BusConfiguration_src_adapter(p6));
                Debug.Assert(p6.data_.bytes.SequenceEqual(dst.data_.bytes));
            }
            if(Communication_instance.send(p6)) Communication_instance.CopyTo(Communication_instance);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- InstructionsPack -------------------------");
            var p7 = Communication.NEW.InstructionsPack(cur);
            fill(p7);
            onInstructionsPack(p7);
            {
                var dst = Communication.NEW.InstructionsPack(cur_dst);
                p7.push_(new InstructionsPack_dst_adapter(dst));
                Debug.Assert(p7.data_.bytes.SequenceEqual(dst.data_.bytes));
                dst = Communication.NEW.InstructionsPack(cur_dst);
                dst.pull_(new InstructionsPack_src_adapter(p7));
                Debug.Assert(p7.data_.bytes.SequenceEqual(dst.data_.bytes));
            }
            if(Communication_instance.send(p7)) Communication_instance.CopyTo(Communication_instance);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- DeviceError -------------------------");
            fill(TestChannel.NEW.DeviceError(cur));
            var p8 = new  org.company.Client.DeviceError(cur);
            onDeviceError(p8);
            {
                cur_dst.wrap(org.company.Client.DeviceError.meta_);
                var dst = new org.unirail.Test. DeviceError(cur_dst);
                p8.push_(new DeviceError_dst_adapter(dst));
                Debug.Assert(p8.data_.bytes.SequenceEqual(dst.data_.bytes));
            }
            if(TestChannel_instance.send(p8)) TestChannel_instance.Std.CopyTo(Communication_instance);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            Debug.Print("-------------------- SensorsData -------------------------");
            fill(TestChannel.NEW.SensorsData(cur));
            var p9 = new  org.company.Client.SensorsData(cur);
            onSensorsData(p9);
            {
                cur_dst.wrap(org.company.Client.SensorsData.meta_);
                var dst = new org.unirail.Test. SensorsData(cur_dst);
                p9.push_(new SensorsData_dst_adapter(dst));
                Debug.Assert(p9.data_.bytes.SequenceEqual(dst.data_.bytes));
            }
            if(TestChannel_instance.send(p9)) TestChannel_instance.Std.CopyTo(Communication_instance);
            else throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
        }
    }
}
