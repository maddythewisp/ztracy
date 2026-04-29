const std = @import("std");

const Src = std.builtin.SourceLocation;

const ztracy_vk_context = ?*anyopaque;
const ztracy_vk_zone = ?*anyopaque;

extern fn ztracy_vk_context_calibrated(
    instance: usize,
    physical_device: usize,
    device: usize,
    queue: usize,
    command_buffer: usize,
    get_instance_proc_addr: usize,
    get_device_proc_addr: usize,
) ztracy_vk_context;

extern fn ztracy_vk_context_host_calibrated(
    instance: usize,
    physical_device: usize,
    device: usize,
    get_instance_proc_addr: usize,
    get_device_proc_addr: usize,
) ztracy_vk_context;

extern fn ztracy_vk_context_destroy(context: ztracy_vk_context) void;
extern fn ztracy_vk_context_name(context: ztracy_vk_context, name: [*]const u8, name_len: usize) void;
extern fn ztracy_vk_context_collect(context: ztracy_vk_context, command_buffer: usize) void;

extern fn ztracy_vk_zone_begin(
    context: ztracy_vk_context,
    command_buffer: usize,
    line: u32,
    file: [*]const u8,
    file_len: usize,
    function: [*]const u8,
    function_len: usize,
    name: [*]const u8,
    name_len: usize,
) ztracy_vk_zone;

extern fn ztracy_vk_zone_end(zone: ztracy_vk_zone) void;

pub const VkContext = struct {
    handle: ztracy_vk_context = null,

    pub inline fn initCalibrated(
        instance: usize,
        physical_device: usize,
        device: usize,
        queue: usize,
        command_buffer: usize,
        get_instance_proc_addr: usize,
        get_device_proc_addr: usize,
    ) VkContext {
        return .{
            .handle = ztracy_vk_context_calibrated(
                instance,
                physical_device,
                device,
                queue,
                command_buffer,
                get_instance_proc_addr,
                get_device_proc_addr,
            ),
        };
    }

    pub inline fn initHostCalibrated(
        instance: usize,
        physical_device: usize,
        device: usize,
        get_instance_proc_addr: usize,
        get_device_proc_addr: usize,
    ) VkContext {
        return .{
            .handle = ztracy_vk_context_host_calibrated(
                instance,
                physical_device,
                device,
                get_instance_proc_addr,
                get_device_proc_addr,
            ),
        };
    }

    pub inline fn deinit(self: *VkContext) void {
        ztracy_vk_context_destroy(self.handle);
        self.handle = null;
    }

    pub inline fn name(self: *VkContext, label: []const u8) void {
        ztracy_vk_context_name(self.handle, label.ptr, label.len);
    }

    pub inline fn collect(self: *VkContext, command_buffer: usize) void {
        ztracy_vk_context_collect(self.handle, command_buffer);
    }

    pub inline fn zone(self: *VkContext, comptime src: Src, command_buffer: usize, comptime label: [:0]const u8) VkZone {
        return VkZone.begin(self, src, command_buffer, label);
    }
};

pub const VkZone = struct {
    handle: ztracy_vk_zone = null,

    pub inline fn begin(
        context: *VkContext,
        comptime src: Src,
        command_buffer: usize,
        comptime label: [:0]const u8,
    ) VkZone {
        return .{
            .handle = ztracy_vk_zone_begin(
                context.handle,
                command_buffer,
                src.line,
                src.file.ptr,
                src.file.len,
                src.fn_name.ptr,
                src.fn_name.len,
                label.ptr,
                label.len,
            ),
        };
    }

    pub inline fn end(self: *VkZone) void {
        ztracy_vk_zone_end(self.handle);
        self.handle = null;
    }
};
