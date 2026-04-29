#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* ztracy_vk_context;
typedef void* ztracy_vk_zone;

ztracy_vk_context ztracy_vk_context_calibrated(
    uintptr_t instance,
    uintptr_t physical_device,
    uintptr_t device,
    uintptr_t queue,
    uintptr_t command_buffer,
    uintptr_t get_instance_proc_addr,
    uintptr_t get_device_proc_addr);

ztracy_vk_context ztracy_vk_context_host_calibrated(
    uintptr_t instance,
    uintptr_t physical_device,
    uintptr_t device,
    uintptr_t get_instance_proc_addr,
    uintptr_t get_device_proc_addr);

void ztracy_vk_context_destroy(ztracy_vk_context context);
void ztracy_vk_context_name(ztracy_vk_context context, const char* name, size_t name_len);
void ztracy_vk_context_collect(ztracy_vk_context context, uintptr_t command_buffer);
void ztracy_vk_context_collect_host_limit(ztracy_vk_context context, uint32_t max_count);

ztracy_vk_zone ztracy_vk_zone_begin(
    ztracy_vk_context context,
    uintptr_t command_buffer,
    uint32_t line,
    const char* file,
    size_t file_len,
    const char* function,
    size_t function_len,
    const char* name,
    size_t name_len);

void ztracy_vk_zone_end(ztracy_vk_zone zone);

#ifdef __cplusplus
}
#endif
