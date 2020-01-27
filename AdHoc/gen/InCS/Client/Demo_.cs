
using System;

using org.company;
using org.company.Client;
using System.Collections.Generic;
using System.Text;
using org.unirail;
using Cursor = org.unirail.AdHoc.Pack.Cursor;
using Pack = org.unirail.AdHoc.Pack;
using Lib = org.unirail.AdHoc;

namespace org.unirail
{
    public struct Demo
    {

        static   int  some_int    = (int)0 ;
        static   ushort  some_ushort    = (ushort)0 ;
        static   byte  some_byte    = (byte)0 ;

        public static void onDeviceVersion(DeviceVersion p0)
        {
            var some_DevID = p0.DevID() ;
            var some_Ver = p0.Ver() ;
        }
        public static void onStop() {}
        public static void onStart() {}
        public static void onGetDeviceVersion() {}
        public static void onGetConfiguration() {}
        public static void onSetConfiguration() {}
        public static void onBusConfiguration(BusConfiguration p6)
        {
            var some_multiplier = p6.multiplier() ;
            var some_time = p6.time() ;
            var some_clk_khz = p6.clk_khz() ;
        }
        public static void fill(BusConfiguration p6)
        {
            p6.multiplier(some_byte);
            p6.time(some_ushort);
            p6.clk_khz(some_ushort);
        }
        public static void onInstructionsPack(InstructionsPack p7)
        {
            var some_Length = p7.Length() ;
            {
                var item = p7.Instructions();
                for(int i = 0; i < item.len(); i++)
                    some_byte = item.get(i);
            }
        }
        public static void fill(InstructionsPack p7)
        {
            p7.Length(some_byte);
            {
                var item = p7.Instructions();
                for(int i = 0; i < item.len(); i++)
                    item.set(some_byte, i);
            }
        }
        public static void onDeviceError(DeviceError p8)
        {
            var some_error_id = p8.error_id() ;
            var some_param1 = p8.param1() ;
        }
        public static void onSensorsData(SensorsData p9)
        {
            {
                var item = p9.values();
                for(int i = 0; i < item.len(); i++)
                    some_ushort = item.get(i);
            }
        }


        public class Communication_demo: Communication
        {
            protected override void onDeviceVersion(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                on_DeviceVersion(this,  new org.company.Client.DeviceVersion(data));
            }
            protected override void onBusConfiguration(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                on_BusConfiguration(this,  new org.company.Client.BusConfiguration(data));
            }
            protected override void onInstructionsPack(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                on_InstructionsPack(this,  new org.company.Client.InstructionsPack(data));
            }
            protected override void onDeviceError(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                on_DeviceError(this,  new org.company.Client.DeviceError(data));
            }
            protected override void onSensorsData(Pack pack)
            {
                var data = new Pack.Bytes();
                data.wrap(pack);
                on_SensorsData(this,  new org.company.Client.SensorsData(data));
            }

            readonly Queue<Pack> sendingPacks = new Queue<Pack>();
            protected internal override Pack pullSendingPack() { return sendingPacks.Dequeue(); }
            protected internal override bool pushSendingPack(Pack pack) {  sendingPacks.Enqueue(pack); return true;}
            public  event Pack.Handler< Communication_demo, DeviceVersion > on_DeviceVersion;
            public  event Pack.Handler< Communication_demo, BusConfiguration > on_BusConfiguration;
            public  event Pack.Handler< Communication_demo, InstructionsPack > on_InstructionsPack;
            public  event Pack.Handler< Communication_demo, DeviceError > on_DeviceError;
            public  event Pack.Handler< Communication_demo, SensorsData > on_SensorsData;

        }

        public static void Main2(string[] args)
        {
            var cur = Config_.Cursor();
            var buff = new byte[512];
            int bytes_out = 0;
            var Communication_instance = new  Communication_demo();
            Communication_instance.on_DeviceVersion += (ch, pack) =>  onDeviceVersion(pack);
            Communication_instance.on_BusConfiguration += (ch, pack) =>  onBusConfiguration(pack);
            Communication_instance.on_InstructionsPack += (ch, pack) =>  onInstructionsPack(pack);
            Communication_instance.on_DeviceError += (ch, pack) =>  onDeviceError(pack);
            Communication_instance.on_SensorsData += (ch, pack) =>  onSensorsData(pack);
            if(! Communication_instance.sendStop())
                throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            if(! Communication_instance.sendStart())
                throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            if(! Communication_instance.sendGetDeviceVersion())
                throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            if(! Communication_instance.sendGetConfiguration())
                throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            if(! Communication_instance.sendSetConfiguration())
                throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            {
                var p6 = Communication.NEW.BusConfiguration(cur);
                fill(p6);
                if(!Communication_instance.send(p6))    //put pack into the channel send-buffer
                    throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            }
            {
                var p7 = Communication.NEW.InstructionsPack(cur);
                fill(p7);
                if(!Communication_instance.send(p7))    //put pack into the channel send-buffer
                    throw new SystemException("error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW");
            }
            bytes_out = Communication_instance.Read(buff, 0, buff.Length);// sending packs
            Communication_instance.Write(buff, 0, buff.Length);// receiving packs
        }
    }
}
