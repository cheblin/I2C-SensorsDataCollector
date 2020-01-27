extern crate cc;
extern crate bindgen;

use std::ffi::OsStr;

fn main() {
    use std::env;
    use std::path::Path;
    use std::fs;
    use std::process::Command;
    
    
    let dst_dir = env::var("OUT_DIR").unwrap();
    let dst_dir = Path::new(&dst_dir);
    
    if !dst_dir.join("config_.h").exists() {
        let mut check_now = false;
        println!("cargo:warning=Searching config_.h in....");
        'search: loop {
            for par in dst_dir.ancestors() {
                println!("cargo:warning={}", par.to_str().unwrap());
                if check_now
                {
                    check_now = false;
                    if par.join("Cargo.toml").exists() {
                        let mut cmd = Command::new(env::var("CARGO").unwrap_or(String::from("cargo")));
                        cmd.arg("metadata");
                        cmd.arg("--format-version");
                        cmd.arg("1");
                        cmd.arg("--manifest-path");
                        cmd.arg(par.join("Cargo.toml").to_str().unwrap());
                        
                        let output = if let Ok(output) = cmd.output() { output } else {
                            println!("cargo:warning=Cannot execute cargo metadata --offline --manifest-path command");
                            continue;
                        };
                        
                        if !output.status.success() {
                            println!("cargo:warning=Cannot execute cargo error{}", std::str::from_utf8(&output.stderr).unwrap());
                            continue;
                        }
                        
                        let stdout = std::str::from_utf8(&output.stdout).unwrap();
                        
                        let search = "\"id\":\"ad_hoc-gen ";
                        let start = if let Some(start) = stdout.find(search) { start } else {
                            println!("cargo:warning=Cannot find reference to generated ad_hoc-gen project");
                            continue;
                        };
                        
                        let (_, substr) = stdout.split_at(start);
                        let start = substr.find("file:///").unwrap();
                        let finish = substr.find(")\"").unwrap();
                        
                        #[cfg(target_os = "windows")]
                            let gen_dir = &substr[start + 8..finish];
                        
                        #[cfg(not(target_os = "windows"))]
                            let gen_dir = &substr[start + 7..finish];
                        
                        println!("cargo:warning=<<< Find and use generated config_.h in {}/{}/ >>>", gen_dir, "src");
                        let _ = fs::copy(Path::new(gen_dir).join("src").join("config_.h"), dst_dir.join("config_.h"));
                        
                        let src_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
                        let src_dir = Path::new(&src_dir);
                        let src_dir = Path::new(src_dir).join("src");
                        
                        let _ = fs::copy(src_dir.join("AdHoc.h"), dst_dir.join("AdHoc.h"));
                        let _ = fs::copy(src_dir.join("AdHoc.c"), dst_dir.join("AdHoc.c"));
                        
                        break 'search;
                    }
                } else { check_now = par.file_name() == Some(OsStr::new("target")); }
            }
            return;
        }
    }
    
    
    /*
    SET CPATH !!!
    SET CPATH !!!
    SET CPATH !!!
    SET CPATH !!!
    .....mingw\tools\install\mingw64\x86_64-w64-mingw32\include

    ENVIRONMENT
        TMPDIR, TEMP, TMP
        These environment variables are checked, in order, for the location to write temporary files used during the compilation process.
        !!!!!!!!!!!!!!!
        CPATH
        If this environment variable is present, it is treated as a delimited list of paths to be added to the default system include path list.
        The delimiter is the platform dependent delimiter, as used in the PATH environment variable.
        !!!!!!!!!!!!!!!!
        Empty components in the environment variable are ignored.
        
        C_INCLUDE_PATH, OBJC_INCLUDE_PATH, CPLUS_INCLUDE_PATH, OBJCPLUS_INCLUDE_PATH
        These environment variables specify additional paths, as for CPATH, which are only used when processing the appropriate language.
        
        MACOSX_DEPLOYMENT_TARGET
        If -mmacosx-version-min is unspecified, the default deployment target is read from this environment variable. This option only affects Darwin targets.
    */
    
    
    cc::Build::new()
        .extra_warnings(false)
        .warnings_into_errors(false)
        .warnings(false)
        .file(dst_dir.join("AdHoc.c").as_path())
        .out_dir(dst_dir)
        .compile("ad_hoc");
    
    println!(r"cargo:rustc-link-lib=static=ad_hoc");
    
    bindgen::Builder::default()
        .rustfmt_bindings(true) //install rustfmt with command:>>>>>>>>>>  rustup component add rustfmt-preview
        
        .header(dst_dir.join("AdHoc.h").to_str().unwrap())
        .header(dst_dir.join("config_.h").to_str().unwrap())
        
        .whitelist_function("pack_alloc")
        .whitelist_function("set_field")
        .whitelist_function("set_item")
        .whitelist_function("reset_cursor")
        .whitelist_function("pack_size")
        
        
        .whitelist_function("transmit")
        .whitelist_function("transmit_adv")
        .whitelist_function("receive")
        .whitelist_function("receive_adv")
        
        .whitelist_type("TRANSMITTER_EVENTS")
        .whitelist_type("RECEIVER_EVENTS")
        .whitelist_type("STATE")
        .whitelist_type("MODE")
        .whitelist_type("UMAX")
        .whitelist_type("CursorBase")
        .whitelist_type("Cursor")
        .whitelist_type("Meta")
        .whitelist_type("Field")
        .whitelist_type("Pack")
        .whitelist_type("Flow")
        .whitelist_type("Transmitter")
        .whitelist_type("Receiver")
        
        .whitelist_var("AD_HOC_NESTED_MAX")
        
        .layout_tests(false)
        
        .whitelist_recursively(false)
        .generate()
        
        .expect("Unable to generate bindings")
        
        .write_to_file(dst_dir.join("adapter.rs"))
        .expect("Couldn't write bindings!");
}

