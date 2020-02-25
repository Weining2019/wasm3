//
//  Wasm3 - high performance WebAssembly interpreter written in C.
//
//  Copyright © 2019 Steven Massey, Volodymyr Shymanskyy.
//  All rights reserved.
//

#include <zephyr.h>
#include <sys/printk.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "wasm3.h"
#include "m3_api_wasi.h"
#include "m3_api_libc.h"
#include "m3_env.h"

#include "extra/fib32.wasm.h"

#define FATAL(msg, ...) { printk("Fatal: " msg "\n", ##__VA_ARGS__); return; }

void run_wasm()
{
    M3Result result = m3Err_none;

    uint8_t* wasm = (uint8_t*)fib32_wasm;
    size_t fsize = fib32_wasm_len-1;

    printk("## a\n");
    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment failed");

    printk("## b\n");
    IM3Runtime runtime = m3_NewRuntime (env, 64*1024, NULL);
    if (!runtime) FATAL("m3_NewRuntime failed");

    printk("## c\n");
    IM3Module module;
    result = m3_ParseModule (env, &module, wasm, fsize);
    if (result) FATAL("m3_ParseModule: %s", result);

    printk("## d\n");
    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule: %s", result);

    /*
    result = m3_LinkWASI (runtime->modules);
    if (result) FATAL("m3_LinkWASI: %s", result);

    result = m3_LinkLibC (runtime->modules);
    if (result) FATAL("m3_LinkLibC: %s", result);
    */

    printk("## find function\n");
    IM3Function f;
    result = m3_FindFunction (&f, runtime, "app_main");
    if (result) FATAL("m3_FindFunction: %s", result);

    printk("## 1\n");
    const char* i_argv[1] = {  NULL };
    result = m3_CallWithArgs (f, 0, i_argv);

    if (result) FATAL("Call: %s", result);
    printk("## 2\n");

    uint64_t value = *(uint64_t*)(runtime->stack);
    printk("Result: 0x%llx\n", value);
}

int  main  (int i_argc, const char * i_argv [])
{
    printk("Wasm3 v" M3_VERSION " on WASM, build " __DATE__ " " __TIME__ "\n");

    int start = k_uptime_get_32();
    run_wasm();
    int end = k_uptime_get_32();

    //uint32_t elapsed_time = (end - start)*1000 / CLOCKS_PER_SEC ;
    printk("Elapsed: %d \n", (end - start));
    printk("### done\n");

    return 0;
}
