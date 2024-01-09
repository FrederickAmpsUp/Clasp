/**
 * Clasp Build target implementations
 * Authored 1/4/2024-present
 * 
 * This program is part of the Clasp Source Libraries
 * 
 * Copyright (c) 2024, Frederick Ziola
 *                      frederick.ziola@gmail.com
 * 
 * SPDX-License-Identifier: GPL-3.0
 * 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <clasp/target.h>
#include <stdio.h>
#include <sys/stat.h>

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

ClaspTarget *new_target(char *fname) {

}