## Build for stm32 board (wasm binary embed in a header file) and runing on Zephyr OS

1. Create a soft link in src folder to wasm3
   cd src
   ln -s <wasm3_root> wasm3

2. Build

   source <zephyr_root>/zephyr-env.sh
   mkdir build && cd build

   cmake -GNinja -DBOARD=nucleo_f767zi -DCMAKE_BUILD_TYPE=Debug ..
   or:
   cmake -GNinja -DBOARD=nucleo_f767zi -DCMAKE_BUILD_TYPE=Release ..
   or:
   cmake -GNinja -DBOARD=nucleo_f767zi ..

3. Run
   ninja flash

build a wasm file and replace the one in wasm3/source/extra/fib32.wasm.h:
/opt/wasi-sdk/bin/clang      \
        -O3 --target=wasm32  \
        -Wl,--export=fib     \
        -Wl,--export=test    \
        -nostdlib            \
        -Wl,--no-entry       \
        -Wl,--strip-all      \
        -Wl,--allow-undefined \
        -o test-wasm3.wasm *.c
wasm2wat -o  test-wasm3.wat test-wasm3.wasm


