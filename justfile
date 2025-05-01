outdir := env("BUILD_OUT", "build")

release: (_build "Release" "NONE" "native")

asan: (_build "Debug" "MEMORY" "native")

_build type sanitizer target:
    cmake -S . -B {{ outdir }}-{{ target }} -DCMAKE_BUILD_TYPE={{ type }} -DSANITIZER={{ sanitizer }}
    cmake --build {{ outdir }}-{{ target }} -j {{ num_cpus() }}

clean:
    rm -rf {{ outdir }}-native
    rm -rf {{ outdir }}-web
