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
#include <stdint.h>

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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

static char* get_temp_directory() {
    char* temp_dir = NULL;

#ifdef _WIN32
    DWORD path_size = GetTempPath(0, NULL);
    temp_dir = (char*)malloc(path_size + 1);

    if (temp_dir) {
        GetTempPath(path_size, temp_dir);
    }
#else
    temp_dir = getenv("TMPDIR");

    if (!temp_dir) {
        temp_dir = "/tmp";
    }
#endif

    return temp_dir;
}

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(directory, mode) _mkdir(directory)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define mkdir(directory, mode) mkdir(directory, mode)
#endif

ClaspTarget *new_target(char *fname) {
    FILE *f = fopen(fname, "rb");
    char head[3];
    if (fread(head, 1, sizeof(head), f) != sizeof(head)) {
        printf("Invalid header in build target file: %s\n", fname);
        fclose(f);
        return NULL;
    }
    if (strncmp(head, "CBT", sizeof(head))) {
        printf("Invalid header in build target file: %s\n", fname);
        fclose(f);
        return NULL;
    }

    uint8_t type;
    if (fread(&type, 1, sizeof(uint8_t), f) != sizeof(uint8_t)) {
        printf("Failed to read target type in build target file: %s\n", fname);
        fclose(f);
        return NULL;
    }

    size_t len;
    if (fread(&len, 1, sizeof(size_t), f) != sizeof(size_t)) {
        printf("Failed to read binary size in build target file: %s\n", fname);
        fclose(f);
        return NULL;
    }

    char *buf = malloc(len);
    if (fread(buf, 1, len, f) != len) {
        printf("Binary data in build target file %s is of incorrect length.\n", fname);
        free(buf);
        fclose(f);
        return NULL;
    }

    char *tmp = get_temp_directory();
    size_t tmp_len = strlen(tmp);
    const char *const extension = "/CBT/";
#ifdef _WIN32
    const char *const file = "TARGET.dll";
#else
    const char *const file = "TARGET.so";
#endif
    char *dir = malloc(tmp_len + strlen(extension) + strlen(file) + 1);
    strncpy(dir, tmp, tmp_len);
    strncpy(dir + tmp_len, extension, strlen(extension) + 1);
    mkdir(dir, 0775);

    char *dll_fname = dir;
    strncpy(dll_fname + tmp_len + strlen(extension), file, strlen(file) + 1);

    FILE *dll_outf = fopen(dll_fname, "wb");
    fwrite(buf, 1, len, dll_outf);
    fclose(dll_outf);
    _DLL *dll_f = _DLL_open(dll_fname);

    ClaspTarget *out = malloc(sizeof(ClaspTarget));

    if (type == TARGET_VISITOR) {
        ClaspASTVisitor visitor;
/* Visitor Types:
    AST_EXPR_BINOP
    AST_EXPR_UNOP
    AST_EXPR_POSTFIX
    AST_EXPR_LIT_NUMBER
    AST_EXPR_VAR_REF
    AST_EXPR_FN_CALL

    AST_EXPR_STMT
    AST_BLOCK_STMT
    AST_VAR_DECL_STMT
    AST_LET_DECL_STMT
    AST_CONST_DECL_STMT
    AST_FN_DECL_STMT

    AST_IF_STMT
    AST_WHILE_STMT
*/
        out->run = _DLL_load(dll_f, void, "target_run");

        out->library = dll_f;
        out->type = TARGET_VISITOR;
    }

    free(dll_fname); // also frees dir
    return out;
}