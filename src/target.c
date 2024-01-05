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
#include <minizip/unzip.h>
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

#define CHUNK 16384

static int unzip(const char *zip_filename, const char *output_dir) {
    unzFile zip_file = unzOpen(zip_filename);
    if (!zip_file) {
        fprintf(stderr, "Error: Could not open zip file %s\n", zip_filename);
        return -1;
    }

    if (unzGoToFirstFile(zip_file) != UNZ_OK) {
        fprintf(stderr, "Error: Could not go to the first file in the zip archive\n");
        unzClose(zip_file);
        return -1;
    }

    int ret;
    unsigned char buffer[CHUNK];
    unz_file_info file_info;

    do {
        char file_name[256];
        if (unzGetCurrentFileInfo(zip_file, &file_info, file_name, sizeof(file_name), NULL, 0, NULL, 0) != UNZ_OK) {
            fprintf(stderr, "Error: Could not get file info\n");
            unzClose(zip_file);
            return -1;
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", output_dir, file_name);

        if (file_name[strlen(file_name) - 1] == '/') {
            // The file in the zip archive is a directory, create it
            mkdir(full_path, 0755);
        } else {
            // The file in the zip archive is a regular file, extract it
            FILE *output_file = fopen(full_path, "wb");
            if (!output_file) {
                fprintf(stderr, "Error: Could not open output file %s\n", full_path);
                unzClose(zip_file);
                return -1;
            }

            ret = UNZ_OK;
            int read_bytes;
            do {
                ret = unzReadCurrentFile(zip_file, buffer, CHUNK);
                if (ret < 0) {
                    fprintf(stderr, "Error: Could not read from zip file\n");
                    fclose(output_file);
                    unzClose(zip_file);
                    return -1;
                }
                read_bytes = ret;
                if (read_bytes > 0) {
                    fwrite(buffer, read_bytes, 1, output_file);
                }
            } while (ret > 0);

            fclose(output_file);
        }

        if (unzGoToNextFile(zip_file) != UNZ_OK) {
            break;  // No more files in the zip archive
        }

    } while (1);

    unzClose(zip_file);
    return 0;
}

ClaspTarget *new_target(char *fname) {

}