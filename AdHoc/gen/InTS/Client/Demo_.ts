import { org as _org } from './AdHoc';
import _Lib = _org.unirail.AdHoc;
import _Pack = _Lib.Pack;
import _Cursor = _Pack.Cursor;

import { org as _ad_hoc } from './Client';
import _Config = _ad_hoc.unirail.AdHoc.Config_;

import { org as _gen } from './Client';
import _Host_root = _gen.company;
import _Host = _gen.company.Client;

export namespace org.unirail {
    export namespace Demo_ {
        let some_string = 'null';
        let some_boolean = true;
        let some_number = 0;
        let some_numbers: number[] = [];

        type Handler<Channel, Pack> = (src: Channel, pack: Pack) => void;

        export function onDeviceVersion(p0: _Host.DeviceVersion) {
            const some_DevID = p0.DevID();
            const some_Ver = p0.Ver();
        }
        export function onStop() {}
        export function onStart() {}
        export function onGetDeviceVersion() {}
        export function onGetConfiguration() {}
        export function onSetConfiguration() {}
        export function onBusConfiguration(p6: _Host.BusConfiguration) {
            const some_multiplier = p6.multiplier();
            const some_time = p6.time();
            const some_clk_khz = p6.clk_khz();
        }
        export function fill_BusConfiguration(p6: _Host.BusConfiguration) {
            p6.multiplier_(some_number);
            p6.time_(some_number);
            p6.clk_khz_(some_number);
        }

        export function onInstructionsPack(p7: _Host.InstructionsPack) {
            const some_Length = p7.Length();
            {
                const item = p7.Instructions();
                for (let value of item) some_number = value;
            }
        }
        export function fill_InstructionsPack(p7: _Host.InstructionsPack) {
            p7.Length_(some_number);
            {
                const item = p7.Instructions();

                for (let i = 0; i < _Host.InstructionsPack.Instructions.item_len; i++) item.set(some_number, i);
            }
        }

        export function onDeviceError(p8: _Host.DeviceError) {
            const some_error_id = p8.error_id();
            const some_param1 = p8.param1();
        }
        export function onSensorsData(p9: _Host.SensorsData) {
            {
                const item = p9.values();
                for (let value of item) some_number = value;
            }
        }

        class Communication_demo extends _Host_root.Communication {
            onDeviceVersion(pack: _Pack) {
                let cur = _Config.Cursor();
                cur.wrap(pack);
                this.on_DeviceVersion.forEach(h => h(this, cur.as_pack(_Host.DeviceVersion.impl_)));
            }
            onBusConfiguration(pack: _Pack) {
                let cur = _Config.Cursor();
                cur.wrap(pack);
                this.on_BusConfiguration.forEach(h => h(this, cur.as_pack(_Host.BusConfiguration.impl_)));
            }
            onInstructionsPack(pack: _Pack) {
                let cur = _Config.Cursor();
                cur.wrap(pack);
                this.on_InstructionsPack.forEach(h => h(this, cur.as_pack(_Host.InstructionsPack.impl_)));
            }
            onDeviceError(pack: _Pack) {
                let cur = _Config.Cursor();
                cur.wrap(pack);
                this.on_DeviceError.forEach(h => h(this, cur.as_pack(_Host.DeviceError.impl_)));
            }
            onSensorsData(pack: _Pack) {
                let cur = _Config.Cursor();
                cur.wrap(pack);
                this.on_SensorsData.forEach(h => h(this, cur.as_pack(_Host.SensorsData.impl_)));
            }

            readonly sendingPacks = new Array<_Pack>(5);
            public pullSendingPack(): _Pack | null {
                return this.sendingPacks.shift()!;
            }
            public pushSendingPack(pack: _Pack): boolean {
                this.sendingPacks.push(pack);
                return true;
            }
            public on_DeviceVersion: Handler<Communication_demo, _Host.DeviceVersion>[] = [];
            public on_BusConfiguration: Handler<Communication_demo, _Host.BusConfiguration>[] = [];
            public on_InstructionsPack: Handler<Communication_demo, _Host.InstructionsPack>[] = [];
            public on_DeviceError: Handler<Communication_demo, _Host.DeviceError>[] = [];
            public on_SensorsData: Handler<Communication_demo, _Host.SensorsData>[] = [];
        }

        export function Main() {
            let cur = _Config.Cursor();
            let buff = new Uint8Array(new ArrayBuffer(512));
            let bytes_out = 0;

            let Communication_instance = new Communication_demo();
            Communication_instance.on_DeviceVersion.push((ch, pack) => onDeviceVersion(pack));
            Communication_instance.on_BusConfiguration.push((ch, pack) => onBusConfiguration(pack));
            Communication_instance.on_InstructionsPack.push((ch, pack) => onInstructionsPack(pack));
            Communication_instance.on_DeviceError.push((ch, pack) => onDeviceError(pack));
            Communication_instance.on_SensorsData.push((ch, pack) => onSensorsData(pack));
            if (!Communication_instance.sendStop()) throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            if (!Communication_instance.sendStart()) throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            if (!Communication_instance.sendGetDeviceVersion()) throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            if (!Communication_instance.sendGetConfiguration()) throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            if (!Communication_instance.sendSetConfiguration()) throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            {
                let p6 = _Host_root.Communication.NEW.BusConfiguration(cur);
                fill_BusConfiguration(p6);
                if (!Communication_instance.send(p6))
                    //put pack into the channel send-buffer
                    throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            }
            {
                let p7 = _Host_root.Communication.NEW.InstructionsPack(cur);
                fill_InstructionsPack(p7);
                if (!Communication_instance.send(p7))
                    //put pack into the channel send-buffer
                    throw new Error('error AD_HOC_FAILURE_SENDING_QUEUE_OVERFLOW');
            }

            bytes_out = Communication_instance.transmitter.packs_into_bytes(buff, 0, buff.byteLength); // sending packs
            Communication_instance.receiver.bytes_into_packs(buff, 0, buff.byteLength); // receiving packs
        }
    }
}
