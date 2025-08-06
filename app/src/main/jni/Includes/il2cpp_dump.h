//
// Created by Perfare on 2020/7/4.
//

#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <thread>
#include <xdl/xdl.h>
#include <dobby.h>
#include <Includes/obfuscate.h>
#include <KittyMemory/MemoryPatch.h>

#ifndef RIRU_IL2CPPDUMPER_IL2CPP_H
#define RIRU_IL2CPPDUMPER_IL2CPP_H

#define UnityVersion xui

// 2018.3.0f2(v24.1) and up enable it
#define VersionAboveV24

// 2020.2.0f1 and up enable it
#define VersionAbove2020V2

#define STR(x) #x
#define STRINGIFY_MACRO(x) STR(x)
#define EXPAND(x) x
#define IL2CPPHEADER(a, b, c) STRINGIFY_MACRO(EXPAND(a)EXPAND(b)EXPAND(c))
#define IL2CPPAPIDIR il2cppapi/
#define IL2CPPCLASS IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-class.h)
#define IL2CPPAPI IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-api-functions.h)

void il2cpp_dump(void *handle, char *outDir);
bool equals(const std::string &s1, const std::string &s2);
std::string utf16le_to_utf8(const std::u16string &u16str);

#if defined (__aarch64__)

#define RETURN _("C0 03 5F D6")
#define NOP _("1F 20 03 D5")
#define TRUE _("20 00 80 D2 C0 03 5F D6")
#define FALSE _("00 00 80 D2 C0 03 5F D6")

#else

#define RETURN _("1E FF 2F E1")
#define TRUE _("01 00 A0 E3 1E FF 2F E1")
#define FALSE _("00 00 A0 E3 1E FF 2F E1")

#endif

#endif //RIRU_IL2CPPDUMPER_IL2CPP_H
