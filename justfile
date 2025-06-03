build:
    if [ ! -d ./build ]; then cmake -S . -B build; fi
    cmake --build ./build --parallel $(nproc)

clean:
    if [ -d ./build ]; then rm -fr ./build; fi
