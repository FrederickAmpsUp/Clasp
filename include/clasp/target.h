/**
 * Clasp Build Target loader declaration
 * Authored 1/3/2024-present
 * 
 * This program is part of the Clasp Header Libraries
 * 
 * Copyright (c) 2023, Frederick Ziola
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

#ifndef TARGET_H
#define TARGET_H

#include <stdbool.h>
#include <stdarg.h>
#include <clasp/ast.h>

#ifdef _WIN32

#include <windows.h>
/**
 * Dynamic Linked Library struct (win32)
*/
typedef struct _DLL {
    HMODULE handle;
}; _DLL;

#else

#include <dlfcn.h>
/**
 * Dynamic Linked Library struct (posix)
*/
typedef struct _DLL {
    void *handle;
} _DLL;

#endif

typedef enum {
    TARGET_VISITOR,
    TARGET_TRANSPILER,
} ClaspTargetType;

/**
 * Struct to store a build target's info.
*/
typedef struct ClaspTarget {
    _DLL *library;
    ClaspTargetType type;
    union {
        ClaspASTVisitor visitor;
        // TOOD: bytecode transpiler
    };
} ClaspTarget;

/**
 * Open a DLL from a file.
 * @param filename The filename of the DLL.
*/
_DLL *_DLL_open(char *filename);

/**
 * Load a function from a DLL.
 * Arguments are arbitrary, use the correct signature!
 * @param lib The DLL to load the function from
 * @param ret The return type of the function.
 * @param name The name of the function.
*/
#define _DLL_load(lib, ret, name) ((ret (*) ())(_DLL_load_impl(name, lib)))
void (*_DLL_load_impl(char *name, _DLL *lib))();

#endif // TARGET_H