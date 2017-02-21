#![feature(no_std, lang_items)]
#![crate_type = "staticlib"]
#![no_std]

#[no_mangle]
pub extern fn main() {
	let hello = b"QEMU";
	let uart0 = 0x09000000 as *mut u32;

	for byte in hello {
		unsafe {
			*uart0 = *byte as u32;
		}
	}


    unsafe {
	*uart0.offset(1)  = 'L' as u32;
    }
    loop {}
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[lang = "panic_fmt"]
extern fn panic_fmt() {} 
