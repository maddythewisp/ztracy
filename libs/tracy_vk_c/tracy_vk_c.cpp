#include <new>

#include <vulkan/vulkan.h>

#include "tracy/TracyVulkan.hpp"
#include "tracy_vk_c.h"

extern "C" ztracy_vk_context ztracy_vk_context_calibrated(
    uintptr_t instance,
    uintptr_t physical_device,
    uintptr_t device,
    uintptr_t queue,
    uintptr_t command_buffer,
    uintptr_t get_instance_proc_addr,
    uintptr_t get_device_proc_addr)
{
    return TracyVkContextCalibrated(
        reinterpret_cast<VkInstance>(instance),
        reinterpret_cast<VkPhysicalDevice>(physical_device),
        reinterpret_cast<VkDevice>(device),
        reinterpret_cast<VkQueue>(queue),
        reinterpret_cast<VkCommandBuffer>(command_buffer),
        reinterpret_cast<PFN_vkGetInstanceProcAddr>(get_instance_proc_addr),
        reinterpret_cast<PFN_vkGetDeviceProcAddr>(get_device_proc_addr));
}

extern "C" ztracy_vk_context ztracy_vk_context_host_calibrated(
    uintptr_t instance,
    uintptr_t physical_device,
    uintptr_t device,
    uintptr_t get_instance_proc_addr,
    uintptr_t get_device_proc_addr)
{
    return TracyVkContextHostCalibrated(
        reinterpret_cast<VkInstance>(instance),
        reinterpret_cast<VkPhysicalDevice>(physical_device),
        reinterpret_cast<VkDevice>(device),
        reinterpret_cast<PFN_vkGetInstanceProcAddr>(get_instance_proc_addr),
        reinterpret_cast<PFN_vkGetDeviceProcAddr>(get_device_proc_addr));
}

extern "C" void ztracy_vk_context_destroy(ztracy_vk_context context)
{
    if (!context) return;
    TracyVkDestroy(static_cast<TracyVkCtx>(context));
}

extern "C" void ztracy_vk_context_name(ztracy_vk_context context, const char* name, size_t name_len)
{
    if (!context || !name) return;
    TracyVkContextName(static_cast<TracyVkCtx>(context), name, static_cast<uint16_t>(name_len));
}

extern "C" void ztracy_vk_context_collect(ztracy_vk_context context, uintptr_t command_buffer)
{
    if (!context) return;
    TracyVkCollect(
        static_cast<TracyVkCtx>(context),
        reinterpret_cast<VkCommandBuffer>(command_buffer));
}

extern "C" ztracy_vk_zone ztracy_vk_zone_begin(
    ztracy_vk_context context,
    uintptr_t command_buffer,
    uint32_t line,
    const char* file,
    size_t file_len,
    const char* function,
    size_t function_len,
    const char* name,
    size_t name_len)
{
    if (!context) return nullptr;
    return new tracy::VkCtxScope(
        static_cast<tracy::VkCtx*>(context),
        line,
        file,
        file_len,
        function,
        function_len,
        name,
        name_len,
        reinterpret_cast<VkCommandBuffer>(command_buffer),
        true);
}

extern "C" void ztracy_vk_zone_end(ztracy_vk_zone zone)
{
    if (!zone) return;
    delete static_cast<tracy::VkCtxScope*>(zone);
}
