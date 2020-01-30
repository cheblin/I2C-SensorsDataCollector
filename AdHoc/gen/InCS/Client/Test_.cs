
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
            p0.DevID(11457)  ;
            p0.Ver(32401)  ;
        }

        public static void onDeviceVersion(org.company.Client.DeviceVersion p0)
        {
            Debug.Assert(p0.DevID() == 11457) ;
            Debug.Assert(p0.Ver() == 32401) ;
            Debug.Print("DeviceVersion \n");
        }
        public static void onStop() {Debug.Print("Stop \n"); }
        public static void onStart() {Debug.Print("Start \n"); }
        public static void onGetDeviceVersion() {Debug.Print("GetDeviceVersion \n"); }
        public static void onGetConfiguration() {Debug.Print("GetConfiguration \n"); }
        public static void onSetConfiguration() {Debug.Print("SetConfiguration \n"); }

        public static void fill(org.company.Client. BusConfiguration  p6)
        {
            p6.multiplier(1)  ;
            p6.time(24120)  ;
            p6.clk_khz(61286)  ;
        }

        public static void onBusConfiguration(org.company.Client.BusConfiguration p6)
        {
            Debug.Assert(p6.multiplier() == 1) ;
            Debug.Assert(p6.time() == 24120) ;
            Debug.Assert(p6.clk_khz() == 61286) ;
            Debug.Print("BusConfiguration \n");
        }

        public static void fill(org.company.Client. InstructionsPack  p7)
        {
            p7.Length(200)  ;
            p7.Instructions(new byte [] { 31, 214, 97, 121, 158, 95, 10, 89, 136, 85, 49, 171, 178, 5, 196, 131, 199, 14, 254, 212, 25, 205, 188, 129, 224, 206, 165, 149, 142, 99, 166, 130, 179, 141, 173, 171, 53, 142, 225, 127, 198, 244, 4, 13, 196, 20, 215, 35, 228, 138, 10, 180, 199, 157, 161, 216, 197, 197, 157, 228, 184, 92, 46, 25, 49, 189, 32, 10, 127, 0, 224, 69, 72, 241, 186, 5, 107, 102, 130, 175, 108, 194, 63, 237, 78, 187, 203, 99, 94, 71, 112, 7, 187, 133, 192, 233, 189, 62, 239, 65, 55, 14, 0, 242, 236, 189, 247, 1, 248, 144, 184, 63, 227, 225, 84, 85, 220, 3, 148, 25, 134, 199, 156, 25, 188, 125, 204, 133, 28, 238, 107, 23, 229, 65, 165, 72, 13, 212, 6, 208, 227, 104, 250, 95, 176, 32, 228, 91, 131, 66, 72, 37, 146, 45, 53, 198, 201, 169, 30, 253, 179, 74, 107, 52, 242, 242, 229, 3, 99, 141, 239, 143, 61, 29, 163, 1, 63, 219, 154, 180, 124, 62, 161, 96, 136, 38, 138, 8, 54, 229, 242, 110, 55, 246, 82, 232, 16, 233, 159, 25, 39, 168, 112, 254, 200, 197, 161, 136, 149, 40, 149, 25, 103, 153, 88, 3, 227, 197, 141, 116, 94, 188, 236, 92, 34, 20, 5, 99, 53, 239, 246, 44, 152, 73, 38, 134, 6, 244, 119, 73, 159, 252, 129, 237, 28, 36, 248, 209, 153, 64, 212, 66, 23, 61, 175, 33 })  ;
        }

        public static void onInstructionsPack(org.company.Client.InstructionsPack p7)
        {
            Debug.Assert(p7.Length() == 200) ;
            Debug.Assert(p7.Instructions().same(new byte [] { 31, 214, 97, 121, 158, 95, 10, 89, 136, 85, 49, 171, 178, 5, 196, 131, 199, 14, 254, 212, 25, 205, 188, 129, 224, 206, 165, 149, 142, 99, 166, 130, 179, 141, 173, 171, 53, 142, 225, 127, 198, 244, 4, 13, 196, 20, 215, 35, 228, 138, 10, 180, 199, 157, 161, 216, 197, 197, 157, 228, 184, 92, 46, 25, 49, 189, 32, 10, 127, 0, 224, 69, 72, 241, 186, 5, 107, 102, 130, 175, 108, 194, 63, 237, 78, 187, 203, 99, 94, 71, 112, 7, 187, 133, 192, 233, 189, 62, 239, 65, 55, 14, 0, 242, 236, 189, 247, 1, 248, 144, 184, 63, 227, 225, 84, 85, 220, 3, 148, 25, 134, 199, 156, 25, 188, 125, 204, 133, 28, 238, 107, 23, 229, 65, 165, 72, 13, 212, 6, 208, 227, 104, 250, 95, 176, 32, 228, 91, 131, 66, 72, 37, 146, 45, 53, 198, 201, 169, 30, 253, 179, 74, 107, 52, 242, 242, 229, 3, 99, 141, 239, 143, 61, 29, 163, 1, 63, 219, 154, 180, 124, 62, 161, 96, 136, 38, 138, 8, 54, 229, 242, 110, 55, 246, 82, 232, 16, 233, 159, 25, 39, 168, 112, 254, 200, 197, 161, 136, 149, 40, 149, 25, 103, 153, 88, 3, 227, 197, 141, 116, 94, 188, 236, 92, 34, 20, 5, 99, 53, 239, 246, 44, 152, 73, 38, 134, 6, 244, 119, 73, 159, 252, 129, 237, 28, 36, 248, 209, 153, 64, 212, 66, 23, 61, 175, 33 }));
            Debug.Print("InstructionsPack \n");
        }

        public static void fill(org.unirail.Test. DeviceError  p8)
        {
            p8.param1(56724)  ;
            p8.error_id(Errors.SensorsDataOverflow)  ;
        }

        public static void onDeviceError(org.company.Client.DeviceError p8)
        {
            Debug.Assert(p8.param1() == 56724) ;
            Debug.Assert(p8.error_id() == Errors.SensorsDataOverflow) ;
            Debug.Print("DeviceError \n");
        }

        public static void fill(org.unirail.Test. SensorsData  p9)
        {
            p9.values(new ushort [] { 31270, 29554, 43809, 16883, 21088, 7857, 57697, 17176, 51072, 64521, 27686, 53384, 22776, 26696, 4445, 49614, 18366, 36291, 4559, 47967, 27146, 28532, 31655, 53775, 55174, 38737, 18941, 39017, 40691, 24020, 6190, 26439, 30448, 55762, 54864, 49908, 42541, 37478, 36837, 25086, 46348, 37775, 63580, 50418, 49776, 8102, 34639, 9142, 39461, 40346, 11818, 34280, 35171, 58874, 56537, 4941, 10607, 35880, 52515, 63395, 44848, 26415, 3215, 27076, 10839, 27548, 53869, 18327, 13185, 64394, 35858, 52711, 44749, 57996, 49651, 21672, 15345, 58344, 40712, 193, 35441, 38140, 22829, 25538, 8792, 46148, 28394, 32899, 43535, 37503, 54043, 39147, 64250, 910, 30540, 24648, 26032, 37402, 52692, 11391, 27359, 20307, 19249, 25306, 63668, 32082, 13699, 14223, 65505, 60716, 1263, 36836, 61349, 54667, 62013, 42226, 45168, 15697, 9568, 44923, 46756, 55068, 5017, 59804, 27146, 55386, 39909, 10113, 48792, 31737, 33657, 42746, 14883, 46251, 43711, 20102, 21147, 53941, 28666, 47677, 39477, 19221, 56813, 44266, 58233, 59830, 19950, 25086, 43040, 29900, 12987, 54152, 43376, 42838, 47, 34338, 14887, 31108, 25548, 13111, 12748, 20779, 21464, 52213, 13545, 26110, 62148, 1722, 5570, 52216, 26422, 14176, 1008, 43341, 16764, 26625, 22400, 38310, 11822, 15994, 42448, 22432, 18783, 64667, 54270, 43301, 63325, 43694, 56528, 25995, 55502, 64340, 58579, 63116, 20778, 64369, 8048, 65405, 65046, 45725, 60529, 2146, 9805, 9481, 59183, 19976, 15681, 43488, 19832, 58717, 25095, 3438, 20243, 27813, 33243, 35114, 32426, 24208, 10450, 47074, 64876, 1449, 57323, 39439, 51202, 44211, 36451, 21145, 51121, 12298, 59607, 63999, 45276, 21647, 15350, 35548, 7505, 24959, 65143, 12739, 50855, 8313, 54779, 46598, 3093, 50721, 55846, 866, 10273, 64589, 49282, 48107, 32107, 50242, 59890, 24070, 44579, 9326, 31871, 43051, 21899, 56130, 39694, 23658, 19544, 24189, 18845, 44791, 31785, 35137, 27246, 36801, 26324, 34920, 42176, 21163, 33662, 15696, 12105, 3066, 18507, 58119, 61418, 58334, 38455, 57175, 14937, 42009, 37639, 22806, 60601, 3196, 48029, 22916, 42717, 26857, 29183, 42782, 18318, 60119, 24888, 58150, 40743, 35301, 13377, 21828, 14627, 38146, 10287, 35347, 7990, 30150, 9951, 10816, 39014, 18030, 53939, 16378, 43068, 54079, 12142, 63568, 47128, 1346, 6803, 23657, 9805, 64339, 42139, 53641, 16341, 23146, 26682, 4633, 52274, 12367, 9529, 20542, 3006, 42276, 6864, 22562, 43442, 50984, 9122, 20772, 64036, 41549, 34831, 42592, 4604, 40481, 13210, 64549, 60299, 63726, 52209, 51322, 13867, 13807, 18124, 35355, 782, 2814, 27168, 46572, 7984, 10859, 36053, 42679, 3611, 47234, 50388, 28412, 7494, 9253, 4999, 60081, 62204, 36951, 142, 55980, 35246, 41405, 17406, 54641, 24068, 22648, 39813, 60329, 59793, 11018, 20743, 48263, 60262, 15868, 8138, 63131, 11043, 11408, 46943, 63876, 19525, 45057, 46625, 55791, 59906, 52253, 17615, 33743, 61434, 49737, 45061, 28933, 41985, 63366, 29069, 4001, 56168, 7963, 17330, 16372, 21575, 33668, 40281, 51114, 35717, 29398, 34616, 39872, 27425, 29098, 22643, 11758, 50533, 45886, 23594, 17438, 1386, 2527, 42777, 45313, 51729, 14582, 48104, 40864, 19122, 15912, 64128, 56113, 1030, 54765, 50373, 61077, 36084, 34892, 58345, 55764, 10610, 15803, 34725, 42588, 2609, 16095, 46891, 20378, 38900, 28986, 48357, 3807, 5434, 52927, 28570, 65246, 34792, 11723, 64626, 15063, 56003, 39147, 20185, 26102, 7876, 27184, 9528, 11008, 16552, 7928, 52590, 36549, 6571, 6454, 39508, 11273, 17488, 32054, 42763, 36141, 13035, 2409, 32176, 36421, 31333, 29252, 52625, 13436, 59446, 43378, 39261, 10891, 47917, 47503, 12166, 16918, 60459, 910, 58757, 46832, 14986, 62073, 380, 6175, 23421, 26545, 55096, 59372, 39216, 50176, 12541, 51589, 1520, 43765, 23130, 47354, 29493, 61835, 24892, 56292, 27220, 11058, 48565, 53619, 14082, 8910, 40283, 17137, 34046, 30623, 10951, 48662, 20791, 8918, 46398, 9970, 47871, 44517, 37118, 40222, 23446, 3664, 21361, 40819, 57392, 18574, 6300, 11558, 57704, 48377, 18526, 61497, 29951, 35831, 21381, 6780, 14694, 54722, 58981, 53226, 14084, 14266, 59528, 36935, 45472, 40666, 63224, 26593, 23355, 37739, 1895, 24630, 38793, 35695, 2245, 7325, 61463, 53197, 10310, 8037, 41251, 56430, 15250, 44593, 28932, 11012, 59384, 28132, 23718, 51783, 40166, 64173, 39750, 15464, 18674, 12725, 60359, 20495, 63923, 37998, 13991, 23567, 53217, 53238, 11042, 35818, 33887, 49416, 52350, 24622, 55786, 56047, 44710, 38102, 50228, 10708, 4429, 14476, 7431, 34416, 55350, 1771, 62132, 37768, 3103, 12585, 45202, 56199, 17482, 38096, 63841, 52674, 43836, 1432, 26942, 6238, 62544, 52101, 2628, 53989, 26629, 11642, 59262, 27882, 64378, 15946, 65, 63670, 60871, 25111, 9191, 49562, 25880, 7762, 65534, 17083, 41717, 14843, 32288, 55827, 1610, 6954, 64341, 21057, 20886, 13604, 26789, 41573, 52482, 19118, 36320, 48218, 19288, 50879, 30351, 24636, 60833, 10526, 5017, 36782, 21327, 22753, 3360, 59500, 46645, 44401, 56965, 35517, 47049, 11118, 4939, 57160, 8560, 8259, 30772, 52052, 55040, 38245, 42272, 60850, 26172, 16636, 34909, 55969, 33883, 2170, 35240, 62830, 8371, 37359, 36719, 56240, 32732, 41029, 47585, 9801, 25840, 30855, 5469, 24645, 24596, 47865, 25048, 45671, 37949, 45639, 6239, 27218, 43699, 29840, 22483, 47782, 62574, 33618, 53305, 571, 28339, 62191, 1695, 58156, 52727, 4824, 48746, 46327, 4783, 63268, 34335, 21645, 40898, 19240, 50881, 49117, 6204, 7243, 38902, 50209, 555, 60033, 45206, 26727, 22537, 54930, 50715, 9216, 42015, 14872, 21968, 8672, 1555, 37805, 17648, 9702, 39181, 33857, 7500, 31254, 4757, 34037, 55837, 17582, 30375, 986, 54818, 26705, 35370, 64539, 56285, 15697, 53635, 57384, 56393, 33157, 24081, 42958, 64609, 46991, 21491, 20602, 36332, 57559, 45103, 2959, 39117, 58866, 4684, 59349, 28362, 46934, 58828, 52621, 27704, 14535, 46256, 14810, 49434, 53651, 47933, 28124, 20520, 62783, 24658, 1759, 55156, 3317, 9692, 39102, 61541, 1319, 41399, 27049, 50732, 44836, 58600, 61695, 8748, 13255, 47394, 25837, 14995, 60605, 14364, 16246, 42864, 50709, 42871, 3567, 30342, 52881, 34048, 59080, 35860, 33206, 34446, 58830, 926, 1110, 35375, 27854, 30073, 1267, 2911, 2472, 40034, 36434, 9472, 40774, 56466, 20876, 35520, 48110, 23939, 59721, 34096, 60630, 2530, 51796, 31572, 13826, 23699, 24704, 18719, 55743, 23390, 62469, 6744, 54235, 56275, 64889, 13456, 51109, 9347, 22108, 25536, 60102, 44575, 38406, 40049, 33069, 46135, 15980, 30690, 57097, 13942, 61902, 15293, 33484, 65042, 34837, 27879, 45027, 53512, 60224, 57449, 3484, 63581, 55364, 36543, 12665, 47084, 36667, 4879, 1606, 46043, 61157, 34142, 28533, 45806, 55932, 14638, 6922, 38038, 46852, 49400, 4506, 38357, 60487, 29631, 29705, 4264, 7872, 64737, 45079, 9363, 37874, 439, 35150, 13479, 11943, 58632, 36230, 57796, 49153, 30398, 58502, 57591, 37681, 13387, 43179, 56767, 53995, 26991, 13038, 63911, 63792, 63873, 49541, 8896, 49319, 33883, 45479, 14446, 13931, 19905, 1757, 29817, 7113, 59298, 53842, 45924, 52824, 53124, 44156 })  ;
        }

        public static void onSensorsData(org.company.Client.SensorsData p9)
        {
            Debug.Assert(p9.values().same(new ushort [] { 31270, 29554, 43809, 16883, 21088, 7857, 57697, 17176, 51072, 64521, 27686, 53384, 22776, 26696, 4445, 49614, 18366, 36291, 4559, 47967, 27146, 28532, 31655, 53775, 55174, 38737, 18941, 39017, 40691, 24020, 6190, 26439, 30448, 55762, 54864, 49908, 42541, 37478, 36837, 25086, 46348, 37775, 63580, 50418, 49776, 8102, 34639, 9142, 39461, 40346, 11818, 34280, 35171, 58874, 56537, 4941, 10607, 35880, 52515, 63395, 44848, 26415, 3215, 27076, 10839, 27548, 53869, 18327, 13185, 64394, 35858, 52711, 44749, 57996, 49651, 21672, 15345, 58344, 40712, 193, 35441, 38140, 22829, 25538, 8792, 46148, 28394, 32899, 43535, 37503, 54043, 39147, 64250, 910, 30540, 24648, 26032, 37402, 52692, 11391, 27359, 20307, 19249, 25306, 63668, 32082, 13699, 14223, 65505, 60716, 1263, 36836, 61349, 54667, 62013, 42226, 45168, 15697, 9568, 44923, 46756, 55068, 5017, 59804, 27146, 55386, 39909, 10113, 48792, 31737, 33657, 42746, 14883, 46251, 43711, 20102, 21147, 53941, 28666, 47677, 39477, 19221, 56813, 44266, 58233, 59830, 19950, 25086, 43040, 29900, 12987, 54152, 43376, 42838, 47, 34338, 14887, 31108, 25548, 13111, 12748, 20779, 21464, 52213, 13545, 26110, 62148, 1722, 5570, 52216, 26422, 14176, 1008, 43341, 16764, 26625, 22400, 38310, 11822, 15994, 42448, 22432, 18783, 64667, 54270, 43301, 63325, 43694, 56528, 25995, 55502, 64340, 58579, 63116, 20778, 64369, 8048, 65405, 65046, 45725, 60529, 2146, 9805, 9481, 59183, 19976, 15681, 43488, 19832, 58717, 25095, 3438, 20243, 27813, 33243, 35114, 32426, 24208, 10450, 47074, 64876, 1449, 57323, 39439, 51202, 44211, 36451, 21145, 51121, 12298, 59607, 63999, 45276, 21647, 15350, 35548, 7505, 24959, 65143, 12739, 50855, 8313, 54779, 46598, 3093, 50721, 55846, 866, 10273, 64589, 49282, 48107, 32107, 50242, 59890, 24070, 44579, 9326, 31871, 43051, 21899, 56130, 39694, 23658, 19544, 24189, 18845, 44791, 31785, 35137, 27246, 36801, 26324, 34920, 42176, 21163, 33662, 15696, 12105, 3066, 18507, 58119, 61418, 58334, 38455, 57175, 14937, 42009, 37639, 22806, 60601, 3196, 48029, 22916, 42717, 26857, 29183, 42782, 18318, 60119, 24888, 58150, 40743, 35301, 13377, 21828, 14627, 38146, 10287, 35347, 7990, 30150, 9951, 10816, 39014, 18030, 53939, 16378, 43068, 54079, 12142, 63568, 47128, 1346, 6803, 23657, 9805, 64339, 42139, 53641, 16341, 23146, 26682, 4633, 52274, 12367, 9529, 20542, 3006, 42276, 6864, 22562, 43442, 50984, 9122, 20772, 64036, 41549, 34831, 42592, 4604, 40481, 13210, 64549, 60299, 63726, 52209, 51322, 13867, 13807, 18124, 35355, 782, 2814, 27168, 46572, 7984, 10859, 36053, 42679, 3611, 47234, 50388, 28412, 7494, 9253, 4999, 60081, 62204, 36951, 142, 55980, 35246, 41405, 17406, 54641, 24068, 22648, 39813, 60329, 59793, 11018, 20743, 48263, 60262, 15868, 8138, 63131, 11043, 11408, 46943, 63876, 19525, 45057, 46625, 55791, 59906, 52253, 17615, 33743, 61434, 49737, 45061, 28933, 41985, 63366, 29069, 4001, 56168, 7963, 17330, 16372, 21575, 33668, 40281, 51114, 35717, 29398, 34616, 39872, 27425, 29098, 22643, 11758, 50533, 45886, 23594, 17438, 1386, 2527, 42777, 45313, 51729, 14582, 48104, 40864, 19122, 15912, 64128, 56113, 1030, 54765, 50373, 61077, 36084, 34892, 58345, 55764, 10610, 15803, 34725, 42588, 2609, 16095, 46891, 20378, 38900, 28986, 48357, 3807, 5434, 52927, 28570, 65246, 34792, 11723, 64626, 15063, 56003, 39147, 20185, 26102, 7876, 27184, 9528, 11008, 16552, 7928, 52590, 36549, 6571, 6454, 39508, 11273, 17488, 32054, 42763, 36141, 13035, 2409, 32176, 36421, 31333, 29252, 52625, 13436, 59446, 43378, 39261, 10891, 47917, 47503, 12166, 16918, 60459, 910, 58757, 46832, 14986, 62073, 380, 6175, 23421, 26545, 55096, 59372, 39216, 50176, 12541, 51589, 1520, 43765, 23130, 47354, 29493, 61835, 24892, 56292, 27220, 11058, 48565, 53619, 14082, 8910, 40283, 17137, 34046, 30623, 10951, 48662, 20791, 8918, 46398, 9970, 47871, 44517, 37118, 40222, 23446, 3664, 21361, 40819, 57392, 18574, 6300, 11558, 57704, 48377, 18526, 61497, 29951, 35831, 21381, 6780, 14694, 54722, 58981, 53226, 14084, 14266, 59528, 36935, 45472, 40666, 63224, 26593, 23355, 37739, 1895, 24630, 38793, 35695, 2245, 7325, 61463, 53197, 10310, 8037, 41251, 56430, 15250, 44593, 28932, 11012, 59384, 28132, 23718, 51783, 40166, 64173, 39750, 15464, 18674, 12725, 60359, 20495, 63923, 37998, 13991, 23567, 53217, 53238, 11042, 35818, 33887, 49416, 52350, 24622, 55786, 56047, 44710, 38102, 50228, 10708, 4429, 14476, 7431, 34416, 55350, 1771, 62132, 37768, 3103, 12585, 45202, 56199, 17482, 38096, 63841, 52674, 43836, 1432, 26942, 6238, 62544, 52101, 2628, 53989, 26629, 11642, 59262, 27882, 64378, 15946, 65, 63670, 60871, 25111, 9191, 49562, 25880, 7762, 65534, 17083, 41717, 14843, 32288, 55827, 1610, 6954, 64341, 21057, 20886, 13604, 26789, 41573, 52482, 19118, 36320, 48218, 19288, 50879, 30351, 24636, 60833, 10526, 5017, 36782, 21327, 22753, 3360, 59500, 46645, 44401, 56965, 35517, 47049, 11118, 4939, 57160, 8560, 8259, 30772, 52052, 55040, 38245, 42272, 60850, 26172, 16636, 34909, 55969, 33883, 2170, 35240, 62830, 8371, 37359, 36719, 56240, 32732, 41029, 47585, 9801, 25840, 30855, 5469, 24645, 24596, 47865, 25048, 45671, 37949, 45639, 6239, 27218, 43699, 29840, 22483, 47782, 62574, 33618, 53305, 571, 28339, 62191, 1695, 58156, 52727, 4824, 48746, 46327, 4783, 63268, 34335, 21645, 40898, 19240, 50881, 49117, 6204, 7243, 38902, 50209, 555, 60033, 45206, 26727, 22537, 54930, 50715, 9216, 42015, 14872, 21968, 8672, 1555, 37805, 17648, 9702, 39181, 33857, 7500, 31254, 4757, 34037, 55837, 17582, 30375, 986, 54818, 26705, 35370, 64539, 56285, 15697, 53635, 57384, 56393, 33157, 24081, 42958, 64609, 46991, 21491, 20602, 36332, 57559, 45103, 2959, 39117, 58866, 4684, 59349, 28362, 46934, 58828, 52621, 27704, 14535, 46256, 14810, 49434, 53651, 47933, 28124, 20520, 62783, 24658, 1759, 55156, 3317, 9692, 39102, 61541, 1319, 41399, 27049, 50732, 44836, 58600, 61695, 8748, 13255, 47394, 25837, 14995, 60605, 14364, 16246, 42864, 50709, 42871, 3567, 30342, 52881, 34048, 59080, 35860, 33206, 34446, 58830, 926, 1110, 35375, 27854, 30073, 1267, 2911, 2472, 40034, 36434, 9472, 40774, 56466, 20876, 35520, 48110, 23939, 59721, 34096, 60630, 2530, 51796, 31572, 13826, 23699, 24704, 18719, 55743, 23390, 62469, 6744, 54235, 56275, 64889, 13456, 51109, 9347, 22108, 25536, 60102, 44575, 38406, 40049, 33069, 46135, 15980, 30690, 57097, 13942, 61902, 15293, 33484, 65042, 34837, 27879, 45027, 53512, 60224, 57449, 3484, 63581, 55364, 36543, 12665, 47084, 36667, 4879, 1606, 46043, 61157, 34142, 28533, 45806, 55932, 14638, 6922, 38038, 46852, 49400, 4506, 38357, 60487, 29631, 29705, 4264, 7872, 64737, 45079, 9363, 37874, 439, 35150, 13479, 11943, 58632, 36230, 57796, 49153, 30398, 58502, 57591, 37681, 13387, 43179, 56767, 53995, 26991, 13038, 63911, 63792, 63873, 49541, 8896, 49319, 33883, 45479, 14446, 13931, 19905, 1757, 29817, 7113, 59298, 53842, 45924, 52824, 53124, 44156 }));
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
