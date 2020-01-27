
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
            p0.DevID(32530)  ;
            p0.Ver(17268)  ;
        }

        public static void onDeviceVersion(org.company.Client.DeviceVersion p0)
        {
            Debug.Assert(p0.DevID() == 32530) ;
            Debug.Assert(p0.Ver() == 17268) ;
            Debug.Print("DeviceVersion \n");
        }
        public static void onStop() {Debug.Print("Stop \n"); }
        public static void onStart() {Debug.Print("Start \n"); }
        public static void onGetDeviceVersion() {Debug.Print("GetDeviceVersion \n"); }
        public static void onGetConfiguration() {Debug.Print("GetConfiguration \n"); }
        public static void onSetConfiguration() {Debug.Print("SetConfiguration \n"); }

        public static void fill(org.company.Client. BusConfiguration  p6)
        {
            p6.multiplier(179)  ;
            p6.time(42334)  ;
            p6.clk_khz(14718)  ;
        }

        public static void onBusConfiguration(org.company.Client.BusConfiguration p6)
        {
            Debug.Assert(p6.multiplier() == 179) ;
            Debug.Assert(p6.time() == 42334) ;
            Debug.Assert(p6.clk_khz() == 14718) ;
            Debug.Print("BusConfiguration \n");
        }

        public static void fill(org.company.Client. InstructionsPack  p7)
        {
            p7.Length(168)  ;
            p7.Instructions(new byte [] { 86, 5, 24, 164, 167, 57, 132, 63, 77, 14, 126, 190, 149, 53, 246, 42, 144, 113, 202, 148, 4, 204, 238, 209, 145, 112, 90, 117, 13, 201, 17, 71, 51, 17, 167, 236, 122, 82, 127, 138, 33, 57, 179, 99, 205, 78, 89, 250, 8, 190, 237, 219, 15, 142, 115, 211, 121, 3, 30, 48, 58, 157, 31, 132, 24, 235, 10, 49, 88, 158, 173, 75, 239, 163, 140, 98, 183, 198, 220, 220, 9, 28, 4, 29, 173, 81, 232, 255, 71, 9, 167, 192, 161, 255, 197, 18, 210, 237, 77, 187, 185, 255, 38, 192, 12, 132, 216, 3, 125, 200, 230, 130, 171, 153, 42, 219, 52, 223, 95, 59, 65, 65, 211, 5, 164, 91, 239, 61, 201, 175, 188, 244, 80, 247, 239, 151, 86, 193, 208, 168, 56, 40, 169, 252, 81, 123, 215, 247, 186, 23, 215, 175, 44, 85, 43, 92, 191, 39, 244, 252, 78, 95, 179, 146, 180, 240, 153, 6, 26, 18, 238, 16, 15, 200, 26, 31, 230, 220, 133, 127, 67, 166, 8, 4, 212, 221, 31, 227, 135, 94, 92, 166, 18, 161, 185, 90, 20, 202, 11, 16, 222, 41, 255, 104, 187, 139, 25, 52, 36, 223, 136, 53, 2, 225, 234, 222, 203, 20, 116, 177, 182, 118, 151, 127, 35, 15, 136, 109, 176, 197, 162, 44, 250, 145, 254, 183, 245, 179, 31, 37, 105, 225, 149, 185, 147, 15, 44, 123, 156, 151, 225, 118, 176, 51, 239, 116 })  ;
        }

        public static void onInstructionsPack(org.company.Client.InstructionsPack p7)
        {
            Debug.Assert(p7.Length() == 168) ;
            Debug.Assert(p7.Instructions().same(new byte [] { 86, 5, 24, 164, 167, 57, 132, 63, 77, 14, 126, 190, 149, 53, 246, 42, 144, 113, 202, 148, 4, 204, 238, 209, 145, 112, 90, 117, 13, 201, 17, 71, 51, 17, 167, 236, 122, 82, 127, 138, 33, 57, 179, 99, 205, 78, 89, 250, 8, 190, 237, 219, 15, 142, 115, 211, 121, 3, 30, 48, 58, 157, 31, 132, 24, 235, 10, 49, 88, 158, 173, 75, 239, 163, 140, 98, 183, 198, 220, 220, 9, 28, 4, 29, 173, 81, 232, 255, 71, 9, 167, 192, 161, 255, 197, 18, 210, 237, 77, 187, 185, 255, 38, 192, 12, 132, 216, 3, 125, 200, 230, 130, 171, 153, 42, 219, 52, 223, 95, 59, 65, 65, 211, 5, 164, 91, 239, 61, 201, 175, 188, 244, 80, 247, 239, 151, 86, 193, 208, 168, 56, 40, 169, 252, 81, 123, 215, 247, 186, 23, 215, 175, 44, 85, 43, 92, 191, 39, 244, 252, 78, 95, 179, 146, 180, 240, 153, 6, 26, 18, 238, 16, 15, 200, 26, 31, 230, 220, 133, 127, 67, 166, 8, 4, 212, 221, 31, 227, 135, 94, 92, 166, 18, 161, 185, 90, 20, 202, 11, 16, 222, 41, 255, 104, 187, 139, 25, 52, 36, 223, 136, 53, 2, 225, 234, 222, 203, 20, 116, 177, 182, 118, 151, 127, 35, 15, 136, 109, 176, 197, 162, 44, 250, 145, 254, 183, 245, 179, 31, 37, 105, 225, 149, 185, 147, 15, 44, 123, 156, 151, 225, 118, 176, 51, 239, 116 }));
            Debug.Print("InstructionsPack \n");
        }

        public static void fill(org.unirail.Test. DeviceError  p8)
        {
            p8.param1(31521)  ;
            p8.error_id(Errors.SensorsDataOverflow)  ;
        }

        public static void onDeviceError(org.company.Client.DeviceError p8)
        {
            Debug.Assert(p8.param1() == 31521) ;
            Debug.Assert(p8.error_id() == Errors.SensorsDataOverflow) ;
            Debug.Print("DeviceError \n");
        }

        public static void fill(org.unirail.Test. SensorsData  p9)
        {
            p9.values(new ushort [] { 40739, 61944, 59986, 34752, 27893, 62384, 56358, 26524, 46782, 36190, 39632, 16950, 44062, 61416, 3812, 60984, 30338, 52316, 14495, 8002, 32495, 3695, 52690, 588, 34002, 28416, 27360, 44208, 29274, 64785, 49605, 53715, 23917, 47283, 41267, 46603, 45615, 15592, 32081, 57105, 22199, 48466, 34716, 25111, 49258, 34814, 58307, 50724, 4015, 46739, 55719, 54493, 51175, 62978, 26296, 39638, 51837, 21141, 2625, 54961, 18741, 64275, 53494, 15319, 17792, 12322, 17122, 49064, 23175, 36506, 18727, 13610, 38956, 16390, 8518, 8661, 59619, 33976, 6635, 56286, 1961, 60793, 53453, 61765, 38363, 286, 4824, 7693, 51152, 60499, 31995, 18507, 49821, 62247, 50800, 47429, 16268, 41176, 38171, 31292, 22047, 52279, 590, 33103, 34373, 6997, 40478, 58694, 31763, 7793, 31620, 4595, 57777, 31137, 54456, 21174, 2958, 12688, 32488, 49922, 44978, 5576, 62171, 29120, 10353, 33220, 25565, 39677, 2123, 31007, 24401, 25935, 520, 19689, 31445, 25254, 15323, 40402, 24843, 34408, 19033, 29373, 55737, 15121, 4857, 2908, 51091, 26528, 5578, 65399, 54722, 58336, 1959, 1726, 56136, 61290, 11337, 39039, 40420, 51962, 48692, 22396, 25792, 25332, 28074, 16708, 40037, 47696, 35229, 24849, 5738, 45138, 1967, 2754, 24868, 10327, 53129, 33509, 51517, 57720, 41727, 55084, 20377, 31934, 53708, 27020, 34195, 4170, 32117, 622, 42995, 53493, 61437, 37695, 32733, 63666, 55950, 25172, 27869, 3264, 59049, 5573, 55043, 3800, 55375, 19412, 8689, 20616, 24639, 54190, 26918, 63946, 24399, 7264, 10416, 32148, 37481, 33703, 3429, 56660, 61142, 10753, 38321, 39714, 27296, 30446, 11156, 16149, 63500, 12665, 44093, 11947, 33677, 46915, 38069, 18086, 25070, 22078, 63498, 20932, 61267, 16084, 53687, 23210, 25741, 59280, 33226, 24229, 16408, 54736, 36203, 31900, 25371, 27025, 14766, 32130, 22915, 7172, 57284, 26617, 57127, 43585, 25148, 64082, 30938, 57003, 32133, 5017, 22838, 64167, 31857, 50832, 54950, 1413, 19676, 5433, 15366, 56484, 54141, 29371, 60928, 21874, 7573, 57195, 30246, 12215, 45119, 11704, 2197, 38876, 11545, 9308, 47526, 12862, 23491, 20780, 27224, 333, 37804, 34753, 56781, 851, 38132, 26731, 42366, 59946, 10560, 62373, 14596, 1113, 9587, 52635, 16525, 38204, 54243, 32721, 28380, 10336, 54557, 47600, 52663, 40428, 1975, 8170, 44587, 8106, 55822, 35839, 38316, 16520, 9782, 35934, 24674, 42466, 19463, 64844, 62326, 13717, 32540, 39821, 57609, 61142, 9746, 7492, 2128, 14066, 26559, 19724, 41916, 62296, 8266, 6938, 50774, 24028, 58334, 23765, 5538, 46243, 21745, 22672, 4712, 58368, 34883, 7364, 53263, 11837, 17324, 7471, 40447, 29528, 16586, 53165, 38425, 35130, 22620, 16764, 10833, 63592, 43188, 42512, 34050, 23181, 8393, 39481, 55388, 9290, 9784, 11028, 24830, 23024, 1821, 3981, 15479, 34872, 11999, 12390, 28120, 1955, 22561, 15019, 64671, 61135, 57678, 23390, 48550, 27169, 4971, 47997, 10599, 46964, 48056, 54142, 30905, 18791, 41245, 9152, 51189, 59214, 54236, 49367, 48833, 38850, 12979, 36251, 8690, 29045, 37629, 26884, 54518, 45583, 39713, 30187, 3428, 33221, 36783, 26232, 40871, 56637, 13932, 33746, 8448, 56111, 16477, 22840, 57847, 43819, 22772, 19015, 13569, 15891, 14635, 41322, 62799, 46830, 13382, 30071, 15838, 10162, 8432, 36958, 30874, 1844, 23710, 63678, 64771, 36200, 63917, 19220, 4639, 13254, 21910, 63135, 13610, 25587, 63295, 60625, 4973, 15124, 43079, 49562, 21616, 34782, 55901, 3588, 7200, 28177, 48151, 27812, 24355, 40214, 51219, 49216, 21015, 38192, 10569, 20195, 13725, 48740, 60436, 26942, 24959, 33675, 34614, 50668, 54284, 53271, 33434, 281, 4426, 26173, 15385, 9257, 49094, 61719, 44061, 26975, 11324, 65263, 27377, 48734, 7703, 63968, 18027, 36555, 48250, 47848, 31573, 16117, 31170, 28507, 29029, 40469, 10325, 9272, 15396, 29143, 43250, 34712, 58890, 4915, 45098, 39424, 39344, 61870, 22397, 42638, 59495, 64799, 58374, 26926, 17712, 54122, 43651, 47144, 18220, 61865, 41985, 7858, 62932, 7076, 4839, 30128, 38630, 22708, 35559, 50421, 59142, 43195, 63430, 22105, 43404, 1502, 20867, 56609, 29925, 8554, 30831, 50963, 15396, 17605, 27150, 48679, 26525, 63147, 18339, 24941, 10219, 40190, 55679, 39896, 25335, 44659, 35801, 29986, 61805, 6343, 13397, 6042, 64790, 39294, 35011, 55988, 39302, 44685, 3144, 21759, 27222, 35481, 63651, 63705, 30871, 2805, 20429, 6643, 29628, 52500, 63031, 10287, 62251, 38640, 25222, 37595, 64786, 44943, 25510, 39648, 6943, 57163, 30429, 48425, 8363, 47464, 9863, 20371, 53399, 56072, 46340, 36697, 56063, 15982, 51984, 40205, 38185, 56624, 11589, 39949, 35864, 40550, 31374, 59573, 26818, 19969, 21675, 57239, 38676, 51627, 12634, 31218, 47938, 46204, 35704, 26740, 39, 35398, 40037, 20207, 46346, 22394, 10531, 25642, 32295, 59843, 15715, 40195, 62295, 32181, 43968, 25492, 33131, 40572, 25136, 5939, 11312, 27248, 65128, 15667, 58958, 45924, 55906, 55206, 34274, 57972, 28700, 9286, 31938, 40849, 24815, 47048, 57726, 20640, 59644, 63512, 35219, 15118, 38738, 5314, 34438, 27206, 35541, 56363, 24700, 50982, 9364, 52150, 36418, 25818, 37524, 45952, 10424, 47963, 1240, 17773, 52040, 60088, 45483, 1415, 57014, 6728, 33050, 34533, 47178, 65472, 42333, 3769, 29802, 50199, 22430, 14886, 31152, 59522, 52533, 55803, 19915, 62583, 32928, 1063, 9373, 55815, 13165, 64935, 63674, 20505, 31001, 39371, 26182, 45484, 64782, 52458, 64037, 58581, 40768, 30504, 7965, 59357, 53703, 43836, 11687, 47368, 36337, 43844, 42594, 4827, 49860, 23991, 58848, 311, 36337, 48411, 23657, 28474, 47113, 46393, 44135, 38112, 17608, 12599, 312, 39261, 34823, 37740, 2657, 47257, 63831, 370, 59055, 33943, 17310, 7824, 56384, 53397, 58234, 27439, 7539, 820, 23823, 35500, 42518, 32702, 36423, 388, 10165, 30204, 40451, 40942, 43360, 39248, 59420, 56618, 5608, 40784, 33071, 16148, 52788, 28454, 46852, 36148, 2054, 26466, 19188, 9583, 22011, 36807, 56575, 6467, 52395, 20301, 31609, 9909, 58857, 12420, 32992, 13895, 41626, 1569, 34630, 14452, 54225, 32518, 32188, 55283, 50007, 62555, 31298, 49439, 183, 34088, 5981, 11928, 18975, 12532, 49928, 1844, 63133, 142, 8718, 50971, 54744, 62912, 42423, 46615, 44535, 21711, 22045, 48806, 40550, 63957, 60865, 51861, 16236, 28, 23675, 6672, 27023, 56467, 33365, 6213, 36705, 45805, 20548, 21470, 51404, 18903, 51716, 50304, 40538, 46459, 33424, 42198, 38003, 13025, 39172, 55355, 53738, 49679, 48566, 62388, 32261, 39098, 23205, 19385, 46887, 5708, 52442, 20212, 14445, 55346, 46884, 60161, 56624, 32801, 64414, 36002, 33147, 44022, 27076, 38428, 9082, 49386, 24461, 51225, 55808, 57199, 48813, 4117, 35691, 49105, 32066, 57393, 2771, 49575, 60139, 24678, 16141, 55455, 18460, 37020, 6566, 546, 44496, 1952, 40453, 14994, 29016, 49165, 34185, 22234, 18825, 56561, 16129, 5817, 14375, 25594, 12150, 32941, 8116, 37470, 5755, 40852, 24085, 57949, 55101, 17788, 14818, 32432, 8404, 53668, 41377, 17570, 30459, 21849, 22973, 25128, 42184, 22143, 18937, 609, 44073, 19882, 41825, 60021, 62056, 392, 59278, 21650, 53099, 46674, 7421, 4312, 61179, 37140 })  ;
        }

        public static void onSensorsData(org.company.Client.SensorsData p9)
        {
            Debug.Assert(p9.values().same(new ushort [] { 40739, 61944, 59986, 34752, 27893, 62384, 56358, 26524, 46782, 36190, 39632, 16950, 44062, 61416, 3812, 60984, 30338, 52316, 14495, 8002, 32495, 3695, 52690, 588, 34002, 28416, 27360, 44208, 29274, 64785, 49605, 53715, 23917, 47283, 41267, 46603, 45615, 15592, 32081, 57105, 22199, 48466, 34716, 25111, 49258, 34814, 58307, 50724, 4015, 46739, 55719, 54493, 51175, 62978, 26296, 39638, 51837, 21141, 2625, 54961, 18741, 64275, 53494, 15319, 17792, 12322, 17122, 49064, 23175, 36506, 18727, 13610, 38956, 16390, 8518, 8661, 59619, 33976, 6635, 56286, 1961, 60793, 53453, 61765, 38363, 286, 4824, 7693, 51152, 60499, 31995, 18507, 49821, 62247, 50800, 47429, 16268, 41176, 38171, 31292, 22047, 52279, 590, 33103, 34373, 6997, 40478, 58694, 31763, 7793, 31620, 4595, 57777, 31137, 54456, 21174, 2958, 12688, 32488, 49922, 44978, 5576, 62171, 29120, 10353, 33220, 25565, 39677, 2123, 31007, 24401, 25935, 520, 19689, 31445, 25254, 15323, 40402, 24843, 34408, 19033, 29373, 55737, 15121, 4857, 2908, 51091, 26528, 5578, 65399, 54722, 58336, 1959, 1726, 56136, 61290, 11337, 39039, 40420, 51962, 48692, 22396, 25792, 25332, 28074, 16708, 40037, 47696, 35229, 24849, 5738, 45138, 1967, 2754, 24868, 10327, 53129, 33509, 51517, 57720, 41727, 55084, 20377, 31934, 53708, 27020, 34195, 4170, 32117, 622, 42995, 53493, 61437, 37695, 32733, 63666, 55950, 25172, 27869, 3264, 59049, 5573, 55043, 3800, 55375, 19412, 8689, 20616, 24639, 54190, 26918, 63946, 24399, 7264, 10416, 32148, 37481, 33703, 3429, 56660, 61142, 10753, 38321, 39714, 27296, 30446, 11156, 16149, 63500, 12665, 44093, 11947, 33677, 46915, 38069, 18086, 25070, 22078, 63498, 20932, 61267, 16084, 53687, 23210, 25741, 59280, 33226, 24229, 16408, 54736, 36203, 31900, 25371, 27025, 14766, 32130, 22915, 7172, 57284, 26617, 57127, 43585, 25148, 64082, 30938, 57003, 32133, 5017, 22838, 64167, 31857, 50832, 54950, 1413, 19676, 5433, 15366, 56484, 54141, 29371, 60928, 21874, 7573, 57195, 30246, 12215, 45119, 11704, 2197, 38876, 11545, 9308, 47526, 12862, 23491, 20780, 27224, 333, 37804, 34753, 56781, 851, 38132, 26731, 42366, 59946, 10560, 62373, 14596, 1113, 9587, 52635, 16525, 38204, 54243, 32721, 28380, 10336, 54557, 47600, 52663, 40428, 1975, 8170, 44587, 8106, 55822, 35839, 38316, 16520, 9782, 35934, 24674, 42466, 19463, 64844, 62326, 13717, 32540, 39821, 57609, 61142, 9746, 7492, 2128, 14066, 26559, 19724, 41916, 62296, 8266, 6938, 50774, 24028, 58334, 23765, 5538, 46243, 21745, 22672, 4712, 58368, 34883, 7364, 53263, 11837, 17324, 7471, 40447, 29528, 16586, 53165, 38425, 35130, 22620, 16764, 10833, 63592, 43188, 42512, 34050, 23181, 8393, 39481, 55388, 9290, 9784, 11028, 24830, 23024, 1821, 3981, 15479, 34872, 11999, 12390, 28120, 1955, 22561, 15019, 64671, 61135, 57678, 23390, 48550, 27169, 4971, 47997, 10599, 46964, 48056, 54142, 30905, 18791, 41245, 9152, 51189, 59214, 54236, 49367, 48833, 38850, 12979, 36251, 8690, 29045, 37629, 26884, 54518, 45583, 39713, 30187, 3428, 33221, 36783, 26232, 40871, 56637, 13932, 33746, 8448, 56111, 16477, 22840, 57847, 43819, 22772, 19015, 13569, 15891, 14635, 41322, 62799, 46830, 13382, 30071, 15838, 10162, 8432, 36958, 30874, 1844, 23710, 63678, 64771, 36200, 63917, 19220, 4639, 13254, 21910, 63135, 13610, 25587, 63295, 60625, 4973, 15124, 43079, 49562, 21616, 34782, 55901, 3588, 7200, 28177, 48151, 27812, 24355, 40214, 51219, 49216, 21015, 38192, 10569, 20195, 13725, 48740, 60436, 26942, 24959, 33675, 34614, 50668, 54284, 53271, 33434, 281, 4426, 26173, 15385, 9257, 49094, 61719, 44061, 26975, 11324, 65263, 27377, 48734, 7703, 63968, 18027, 36555, 48250, 47848, 31573, 16117, 31170, 28507, 29029, 40469, 10325, 9272, 15396, 29143, 43250, 34712, 58890, 4915, 45098, 39424, 39344, 61870, 22397, 42638, 59495, 64799, 58374, 26926, 17712, 54122, 43651, 47144, 18220, 61865, 41985, 7858, 62932, 7076, 4839, 30128, 38630, 22708, 35559, 50421, 59142, 43195, 63430, 22105, 43404, 1502, 20867, 56609, 29925, 8554, 30831, 50963, 15396, 17605, 27150, 48679, 26525, 63147, 18339, 24941, 10219, 40190, 55679, 39896, 25335, 44659, 35801, 29986, 61805, 6343, 13397, 6042, 64790, 39294, 35011, 55988, 39302, 44685, 3144, 21759, 27222, 35481, 63651, 63705, 30871, 2805, 20429, 6643, 29628, 52500, 63031, 10287, 62251, 38640, 25222, 37595, 64786, 44943, 25510, 39648, 6943, 57163, 30429, 48425, 8363, 47464, 9863, 20371, 53399, 56072, 46340, 36697, 56063, 15982, 51984, 40205, 38185, 56624, 11589, 39949, 35864, 40550, 31374, 59573, 26818, 19969, 21675, 57239, 38676, 51627, 12634, 31218, 47938, 46204, 35704, 26740, 39, 35398, 40037, 20207, 46346, 22394, 10531, 25642, 32295, 59843, 15715, 40195, 62295, 32181, 43968, 25492, 33131, 40572, 25136, 5939, 11312, 27248, 65128, 15667, 58958, 45924, 55906, 55206, 34274, 57972, 28700, 9286, 31938, 40849, 24815, 47048, 57726, 20640, 59644, 63512, 35219, 15118, 38738, 5314, 34438, 27206, 35541, 56363, 24700, 50982, 9364, 52150, 36418, 25818, 37524, 45952, 10424, 47963, 1240, 17773, 52040, 60088, 45483, 1415, 57014, 6728, 33050, 34533, 47178, 65472, 42333, 3769, 29802, 50199, 22430, 14886, 31152, 59522, 52533, 55803, 19915, 62583, 32928, 1063, 9373, 55815, 13165, 64935, 63674, 20505, 31001, 39371, 26182, 45484, 64782, 52458, 64037, 58581, 40768, 30504, 7965, 59357, 53703, 43836, 11687, 47368, 36337, 43844, 42594, 4827, 49860, 23991, 58848, 311, 36337, 48411, 23657, 28474, 47113, 46393, 44135, 38112, 17608, 12599, 312, 39261, 34823, 37740, 2657, 47257, 63831, 370, 59055, 33943, 17310, 7824, 56384, 53397, 58234, 27439, 7539, 820, 23823, 35500, 42518, 32702, 36423, 388, 10165, 30204, 40451, 40942, 43360, 39248, 59420, 56618, 5608, 40784, 33071, 16148, 52788, 28454, 46852, 36148, 2054, 26466, 19188, 9583, 22011, 36807, 56575, 6467, 52395, 20301, 31609, 9909, 58857, 12420, 32992, 13895, 41626, 1569, 34630, 14452, 54225, 32518, 32188, 55283, 50007, 62555, 31298, 49439, 183, 34088, 5981, 11928, 18975, 12532, 49928, 1844, 63133, 142, 8718, 50971, 54744, 62912, 42423, 46615, 44535, 21711, 22045, 48806, 40550, 63957, 60865, 51861, 16236, 28, 23675, 6672, 27023, 56467, 33365, 6213, 36705, 45805, 20548, 21470, 51404, 18903, 51716, 50304, 40538, 46459, 33424, 42198, 38003, 13025, 39172, 55355, 53738, 49679, 48566, 62388, 32261, 39098, 23205, 19385, 46887, 5708, 52442, 20212, 14445, 55346, 46884, 60161, 56624, 32801, 64414, 36002, 33147, 44022, 27076, 38428, 9082, 49386, 24461, 51225, 55808, 57199, 48813, 4117, 35691, 49105, 32066, 57393, 2771, 49575, 60139, 24678, 16141, 55455, 18460, 37020, 6566, 546, 44496, 1952, 40453, 14994, 29016, 49165, 34185, 22234, 18825, 56561, 16129, 5817, 14375, 25594, 12150, 32941, 8116, 37470, 5755, 40852, 24085, 57949, 55101, 17788, 14818, 32432, 8404, 53668, 41377, 17570, 30459, 21849, 22973, 25128, 42184, 22143, 18937, 609, 44073, 19882, 41825, 60021, 62056, 392, 59278, 21650, 53099, 46674, 7421, 4312, 61179, 37140 }));
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
