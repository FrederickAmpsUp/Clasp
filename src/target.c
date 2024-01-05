#include <clasp/target.h>
#include <stdio.h>


    // platform specific code for dlls
_DLL *_DLL_open(char *fname) {
    _DLL *out = malloc(sizeof(_DLL));
#ifdef _WIN32

    out->handle = LoadLibrary(fname);
    if (out->handle == NULL) {
        fprintf(stderr, "LoadLibrary error while opening \"%s\": %lu\n", fname, GetLastError());
    }

#else

    out->handle = dlopen(fname, RTLD_LAZY);
    if (out->handle == NULL) {
        fprintf(stderr, "dlopen error while opening \"%s\": %s\n", fname, dlerror());
    }

#endif

    return out;
}

void (*_DLL_load_impl(char *name, _DLL *lib))() {
#ifdef _WIN32

    void (*fn)() = (void (*)()) GetProcAddress(lib->handle, name);
    if (fn == NULL) {
        fprintf(stderr, "GetProcAddress error while loading function \"%s\": %lu\n", name, GetLastError());
    }
    return fn;

#else

    void (*fn)() = dlsym(lib->handle, name);
    if (fn == NULL) {
        // Handle error
        fprintf(stderr, "dlsym error while loading function \"%s\": %s\n", dlerror());
    }
    return fn;

#endif
}