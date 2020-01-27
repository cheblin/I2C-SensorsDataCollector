import { org as _org } from './AdHoc';
import _Lib = _org.unirail.AdHoc;
import _Pack = _Lib.Pack;
import _Cursor = _Pack.Cursor;

export namespace org.company {
    export namespace Client {
        export const enum Errors {
            SensorsDataOverflow = 0,
            CannotReadSensor = 1
        }

        export interface DeviceVersion {
            DevID(): number;
            Ver(): number;
        }

        export namespace DeviceVersion {
            export const meta_ = new _Pack.Meta(0, 0, 0, 0, 4, 32);

            class Impl_ extends _Cursor.View implements DeviceVersion {
                public DevID(): number {
                    return this.data_.bytes.getUint16(this.data_.origin + 0, true);
                }

                public Ver(): number {
                    return this.data_.bytes.getUint16(this.data_.origin + 2, true);
                }
            }

            export const impl_ = Impl_.prototype;

            export interface DST_ {
                DevID_(src: number);
                Ver_(src: number);
            }

            export function push_<DST extends DST_>(src: DeviceVersion, dst: DST) {
                dst.DevID_(src.DevID());

                dst.Ver_(src.Ver());
            }
        }
        export namespace Stop {
            export const pack_ = new _Pack(new _Pack.Meta(1));
        }
        export namespace Start {
            export const pack_ = new _Pack(new _Pack.Meta(2));
        }
        export namespace GetDeviceVersion {
            export const pack_ = new _Pack(new _Pack.Meta(3));
        }
        export namespace GetConfiguration {
            export const pack_ = new _Pack(new _Pack.Meta(4));
        }
        export namespace SetConfiguration {
            export const pack_ = new _Pack(new _Pack.Meta(5));
        }
        export interface BusConfiguration {
            multiplier(): number;
            multiplier_(src: number);
            time(): number;
            time_(src: number);
            clk_khz(): number;
            clk_khz_(src: number);
        }

        export namespace BusConfiguration {
            export const meta_ = new _Pack.Meta(6, 0, 0, 0, 5, 40);

            class Impl_ extends _Cursor.View implements BusConfiguration {
                public multiplier(): number {
                    return this.data_.bytes.getUint8(this.data_.origin + 0);
                }
                public multiplier_(src: number) {
                    this.data_.bytes.setUint8(this.data_.origin + 0, src);
                }

                public time(): number {
                    return this.data_.bytes.getUint16(this.data_.origin + 1, true);
                }
                public time_(src: number) {
                    this.data_.bytes.setUint16(this.data_.origin + 1, src, true);
                }

                public clk_khz(): number {
                    return this.data_.bytes.getUint16(this.data_.origin + 3, true);
                }
                public clk_khz_(src: number) {
                    this.data_.bytes.setUint16(this.data_.origin + 3, src, true);
                }
            }

            export const impl_ = Impl_.prototype;

            export interface DST_ {
                multiplier_(src: number);
                time_(src: number);
                clk_khz_(src: number);
            }

            export function push_<DST extends DST_>(src: BusConfiguration, dst: DST) {
                dst.multiplier_(src.multiplier());

                dst.time_(src.time());

                dst.clk_khz_(src.clk_khz());
            }

            export interface SRC_ {
                multiplier(): number;
                time(): number;
                clk_khz(): number;
            }

            export function pull_<SRC extends SRC_>(src: SRC, dst: BusConfiguration) {
                dst.multiplier_(src.multiplier());

                dst.time_(src.time());

                dst.clk_khz_(src.clk_khz());
            }
        }
        export interface InstructionsPack {
            Length(): number;
            Length_(src: number);
            Instructions(): InstructionsPack.Instructions;
            Instructions_(src: InstructionsPack.Instructions | number[]);
            Instructions$(src: Iterator<number>, len: number);
        }

        export namespace InstructionsPack {
            export const meta_ = new _Pack.Meta(7, 0, 0, 0, 257, 2056);
            export const RW_mode: number = 128; //128

            export interface Instructions extends Iterable<number> {
                get(index: number);
                equals(other: this): boolean;
                equals(other: Iterator<number>): boolean;
                set(src: number, index: number);
            }
            export namespace Instructions {
                export class Field extends _Cursor.View implements Instructions, Iterable<number> {
                    public get(index: number): number {
                        return this.data_.bytes.getUint8(this.data_.origin + 1 + index);
                    }

                    equals(other: Iterator<number>): boolean;
                    public equals(other: this): boolean;

                    equals(other): boolean {
                        if (other instanceof Field) return _Lib.ArrayEquals(this.data_.bytes.buffer, this.data_.origin + 1, other.data_.bytes.buffer, other.data_.origin + 1, _Host.InstructionsPack.Instructions.item_len) === 0;
                        for (let i = 0; i < _Host.InstructionsPack.Instructions.item_len; i++) {
                            let v = other.next();
                            if (v.done || v.value !== this.get(i)) return false;
                        }
                        return true;
                    }
                    [Symbol.iterator](): Iterator<number> {
                        const outer = this;
                        let index = 0;
                        const max = _Host.InstructionsPack.Instructions.item_len;
                        return new (class implements Iterator<number> {
                            value: number;
                            done: boolean;

                            next(value?: any): IteratorResult<number> {
                                if ((this.done = index === max)) index = 0;
                                else this.value = outer.get(index++);
                                return this;
                            }
                        })();
                    }

                    public set(src: number, index: number) {
                        this.data_.bytes.setUint8(this.data_.origin + 1 + index, src);
                    }
                }
                export const field = Field.prototype;

                export const item_len = 256;
            }

            class Impl_ extends _Cursor.View implements InstructionsPack {
                public Length(): number {
                    return this.data_.bytes.getUint8(this.data_.origin + 0);
                }
                public Length_(src: number) {
                    this.data_.bytes.setUint8(this.data_.origin + 0, src);
                }

                public Instructions(): InstructionsPack.Instructions {
                    return this.data_.as_field(InstructionsPack.Instructions.field);
                }
                public Instructions_(src: InstructionsPack.Instructions | number[]): Instructions {
                    if (src instanceof Array) {
                        let len = Math.min(_Host.InstructionsPack.Instructions.item_len, src.length);
                        for (let index = 0; index < len; index++) this.data_.bytes.setUint8(this.data_.origin + 1 + index, src[index]);
                        return this.data_.as_field(Instructions.field);
                    }
                    for (let index = 0; index < _Host.InstructionsPack.Instructions.item_len; index++) this.data_.bytes.setUint8(this.data_.origin + 1 + index, src.get(index));

                    return this.data_.as_field(Instructions.field);
                }

                public Instructions$(src: Iterator<number>, len: number): Instructions {
                    let len_ = Math.min(len, _Host.InstructionsPack.Instructions.item_len);

                    len = Math.min(len, _Host.InstructionsPack.Instructions.item_len);
                    if (src)
                        for (let index = 0; index < len; index++) {
                            let v = src.next();
                            if (v.done) break;
                            this.data_.bytes.setUint8(this.data_.origin + 1 + index, v.value);
                        }
                    return this.data_.as_field(Instructions.field);
                }
            }

            export const impl_ = Impl_.prototype;

            export interface DST_ {
                Length_(src: number);
                Instructions_(src: _Host.InstructionsPack.Instructions);
            }

            export function push_<DST extends DST_>(src: InstructionsPack, dst: DST) {
                dst.Length_(src.Length());
                dst.Instructions_(src.Instructions());
            }

            export interface SRC_ {
                Length(): number;
                Instructions(dst: _Host.InstructionsPack.Instructions);
            }

            export function pull_<SRC extends SRC_>(src: SRC, dst: InstructionsPack) {
                dst.Length_(src.Length());
                src.Instructions(dst.Instructions());
            }
        }
        export interface DeviceError {
            param1(): number;
            error_id(): Errors;
        }

        export namespace DeviceError {
            export const meta_ = new _Pack.Meta(8, 0, 0, 0, 3, 17);

            class Impl_ extends _Cursor.View implements DeviceError {
                public param1(): number {
                    return this.data_.bytes.getUint16(this.data_.origin + 0, true);
                }

                public error_id(): Errors {
                    return _Lib.get_bits(this.data_.bytes, this.data_.origin * 8 + 16, 1);
                }
            }

            export const impl_ = Impl_.prototype;

            export interface DST_ {
                error_id_(src: _Host.Errors);
                param1_(src: number);
            }

            export function push_<DST extends DST_>(src: DeviceError, dst: DST) {
                dst.error_id_(src.error_id());

                dst.param1_(src.param1());
            }
        }
        export interface SensorsData {
            values(): SensorsData.values;
        }

        export namespace SensorsData {
            export const meta_ = new _Pack.Meta(9, 0, 0, 0, 2000, 16000);

            export interface values extends Iterable<number> {
                get(index: number);
                equals(other: this): boolean;
                equals(other: Iterator<number>): boolean;
            }
            export namespace values {
                export class Field extends _Cursor.View implements values, Iterable<number> {
                    public get(index: number): number {
                        return this.data_.bytes.getUint16(this.data_.origin + 0 + index * 2, true);
                    }

                    equals(other: Iterator<number>): boolean;
                    public equals(other: this): boolean;

                    equals(other): boolean {
                        if (other instanceof Field) return _Lib.ArrayEquals(this.data_.bytes.buffer, this.data_.origin + 0, other.data_.bytes.buffer, other.data_.origin + 0, _Host.SensorsData.values.item_len) === 0;
                        for (let i = 0; i < _Host.SensorsData.values.item_len; i++) {
                            let v = other.next();
                            if (v.done || v.value !== this.get(i)) return false;
                        }
                        return true;
                    }
                    [Symbol.iterator](): Iterator<number> {
                        const outer = this;
                        let index = 0;
                        const max = _Host.SensorsData.values.item_len;
                        return new (class implements Iterator<number> {
                            value: number;
                            done: boolean;

                            next(value?: any): IteratorResult<number> {
                                if ((this.done = index === max)) index = 0;
                                else this.value = outer.get(index++);
                                return this;
                            }
                        })();
                    }
                }
                export const field = Field.prototype;

                export const item_len = 1000;
            }

            class Impl_ extends _Cursor.View implements SensorsData {
                public values(): SensorsData.values {
                    return this.data_.as_field(SensorsData.values.field);
                }
            }

            export const impl_ = Impl_.prototype;

            export interface DST_ {
                values_(src: _Host.SensorsData.values);
            }

            export function push_<DST extends DST_>(src: SensorsData, dst: DST) {
                dst.values_(src.values());
            }
        }
    }
    import _Host = org.company.Client;

    export abstract class Communication {
        public readonly receiver = new Communication.Receiver(this);

        public readonly transmitter = new Communication.Transmitter(this);
        public abstract pullSendingPack(): _Pack | null;
        public abstract pushSendingPack(pack: _Pack): boolean;
        public sendSetConfiguration(): boolean {
            return this.pushSendingPack(_Host.SetConfiguration.pack_);
        }
        public sendGetConfiguration(): boolean {
            return this.pushSendingPack(_Host.GetConfiguration.pack_);
        }
        public sendGetDeviceVersion(): boolean {
            return this.pushSendingPack(_Host.GetDeviceVersion.pack_);
        }
        public sendStart(): boolean {
            return this.pushSendingPack(_Host.Start.pack_);
        }
        public sendStop(): boolean {
            return this.pushSendingPack(_Host.Stop.pack_);
        }
        public send(src: _Host.BusConfiguration): boolean;
        public send(src: _Host.InstructionsPack): boolean;

        public send(src): boolean {
            if (!src.data_.bytes) return false;
            let pack = src.data_.unwrap();
            if (this.pushSendingPack(pack)) return true;
            src.data_.wrap(pack);
            return false;
        }

        abstract onDeviceVersion(pack: _Pack);
        abstract onBusConfiguration(pack: _Pack);
        abstract onInstructionsPack(pack: _Pack);
        abstract onDeviceError(pack: _Pack);
        abstract onSensorsData(pack: _Pack);
    }
    export namespace Communication {
        export class Receiver extends _Lib.Channel.Receiver {
            constructor(public readonly ch: Communication) {
                super(1, 1);
            }

            dispatch(id: number, pack: _Pack): _Pack.Meta | null {
                switch (id) {
                    case 0:
                        if (!pack) return _Host.DeviceVersion.meta_;
                        this.ch.onDeviceVersion(pack);

                        break;

                    case 6:
                        if (!pack) return _Host.BusConfiguration.meta_;
                        this.ch.onBusConfiguration(pack);

                        break;

                    case 7:
                        if (!pack) return _Host.InstructionsPack.meta_;
                        this.ch.onInstructionsPack(pack);

                        break;

                    case 8:
                        if (!pack) return _Host.DeviceError.meta_;
                        this.ch.onDeviceError(pack);

                        break;

                    case 9:
                        if (!pack) return _Host.SensorsData.meta_;
                        this.ch.onSensorsData(pack);

                        break;
                }
                return null;
            }
        }

        export class Transmitter extends _Lib.Channel.Transmitter {
            constructor(public readonly ch: Communication) {
                super(1, 1);
            }
            public pullSendingPack(): _Pack | null {
                return this.ch.pullSendingPack();
            }
        }

        export namespace NEW {
            /**
             *Device configuration. Sending from device as reply on PC request, sending from PC as new device configuration */
            export function BusConfiguration(cur: _Cursor): _Host.BusConfiguration {
                cur.wrap(Client.BusConfiguration.meta_);
                return cur.as_pack(_Host.BusConfiguration.impl_);
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
            export function InstructionsPack(cur: _Cursor): _Host.InstructionsPack {
                cur.wrap(Client.InstructionsPack.meta_);
                return cur.as_pack(_Host.InstructionsPack.impl_);
            }
        }
    }
} // namespace  org.company
export namespace org.unirail.AdHoc {
    import _Host = org.company.Client;

    export namespace Config_ {
        export function Cursor(): _Cursor {
            return new _Cursor(1, 2);
        }

        export const RECEIVE_REQ_MAX_BYTES = 2000;
        export const RECEIVE_FULL_MAX_BYTES = 2000;

        export const SEND_REQ_MAX_BYTES = 257;
        export const SEND_FULL_MAX_BYTES = 257;
    }
}
