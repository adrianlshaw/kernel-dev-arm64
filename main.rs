#![feature(asm)]
#![feature(core_intrinsics, lang_items)]
#![crate_type = "staticlib"]
#![no_std]

use core::intrinsics::volatile_load;
use core::intrinsics::volatile_store;

const UART0_BASE: u32 = 0x09000000;
const UART0_OFFSET_FULLQ: u32 = 0x18;


fn mmio_write(reg: u32, val: u32) {
    unsafe { volatile_store(reg as *mut u32, val) }
}

fn mmio_read(reg: u32) -> u32 {
    unsafe { volatile_load(reg as *const u32) }
}

fn tx_fifo_full() -> bool {
    mmio_read(UART0_BASE + UART0_OFFSET_FULLQ) & (1 << 5) > 0
}

fn writec(c: u8) {

    while tx_fifo_full() {
        // wait until queue isn't full
    }
    mmio_write(UART0_BASE, c as u32);
}

fn print(msg: &str) {
    for c in msg.chars() {
        writec(c as u8)
    }
}

fn println(msg: &str) {
    print(msg);
    writec('\n' as u8)
}

#[no_mangle]
pub extern fn main() {
    let test = b"TEST";

    print("Hello from adrianlshaw\n");
    println("SUCCESS");

    loop {

    }
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[lang = "panic_fmt"]
extern fn panic_fmt() {}
