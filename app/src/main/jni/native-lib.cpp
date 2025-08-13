#include <jni.h>
#include <string>
#include <thread>
#include <dlfcn.h>
#include <unistd.h>
#include "Includes/log.h"
#include "Includes/il2cpp_dump.h"

uintptr_t il2cpp_base = 0x0;
uintptr_t unity_base = 0x0;

static int callback_z(struct dl_phdr_info *info, size_t size, void *arg) {
  (void)size, (void)arg;
  if (strstr(info->dlpi_name, "libil2cpp.so")) il2cpp_base = (uintptr_t)info->dlpi_addr;
  return 0;
}

#define libTarget "libil2cpp.so"

void dump_thread() {
    do {
        xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
        usleep(1000);
    } while (!il2cpp_base);
    sleep(1);
    auto il2cpp_handle = xdl_open(libTarget, XDL_DEFAULT);
    LOGI("il2cpp_base: %p", il2cpp_base);
    il2cpp_dump(il2cpp_handle, "/data/user/0/com.nekki.shadowfight");
}

__attribute__((constructor))
void init() {
    std::thread(dump_thread).detach();
}
