#include "DllMain.h"
#pragma comment( lib, "Shlwapi.lib")

#pragma comment(linker, "/EXPORT:CloseZStream=AheadLib_CloseZStream,@1")
#pragma comment(linker, "/EXPORT:CreateZStream=AheadLib_CreateZStream,@2")
#pragma comment(linker, "/EXPORT:Flush=AheadLib_Flush,@3")
#pragma comment(linker, "/EXPORT:ReadZStream=AheadLib_ReadZStream,@4")
#pragma comment(linker, "/EXPORT:UnityPalGetLocalTimeZoneData=AheadLib_UnityPalGetLocalTimeZoneData,@5")
#pragma comment(linker, "/EXPORT:UnityPalGetTimeZoneDataForID=AheadLib_UnityPalGetTimeZoneDataForID,@6")
#pragma comment(linker, "/EXPORT:UnityPalTimeZoneInfoGetTimeZoneIDs=AheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs,@7")
#pragma comment(linker, "/EXPORT:UseUnityPalForTimeZoneInformation=AheadLib_UseUnityPalForTimeZoneInformation,@8")
#pragma comment(linker, "/EXPORT:WriteZStream=AheadLib_WriteZStream,@9")
#pragma comment(linker, "/EXPORT:il2cpp_add_internal_call=AheadLib_il2cpp_add_internal_call,@10")
#pragma comment(linker, "/EXPORT:il2cpp_alloc=AheadLib_il2cpp_alloc,@11")
#pragma comment(linker, "/EXPORT:il2cpp_array_class_get=AheadLib_il2cpp_array_class_get,@12")
#pragma comment(linker, "/EXPORT:il2cpp_array_element_size=AheadLib_il2cpp_array_element_size,@13")
#pragma comment(linker, "/EXPORT:il2cpp_array_get_byte_length=AheadLib_il2cpp_array_get_byte_length,@14")
#pragma comment(linker, "/EXPORT:il2cpp_array_length=AheadLib_il2cpp_array_length,@15")
#pragma comment(linker, "/EXPORT:il2cpp_array_new=AheadLib_il2cpp_array_new,@16")
#pragma comment(linker, "/EXPORT:il2cpp_array_new_full=AheadLib_il2cpp_array_new_full,@17")
#pragma comment(linker, "/EXPORT:il2cpp_array_new_specific=AheadLib_il2cpp_array_new_specific,@18")
#pragma comment(linker, "/EXPORT:il2cpp_assembly_get_image=AheadLib_il2cpp_assembly_get_image,@19")
#pragma comment(linker, "/EXPORT:il2cpp_bounded_array_class_get=AheadLib_il2cpp_bounded_array_class_get,@20")
#pragma comment(linker, "/EXPORT:il2cpp_capture_memory_snapshot=AheadLib_il2cpp_capture_memory_snapshot,@21")
#pragma comment(linker, "/EXPORT:il2cpp_class_array_element_size=AheadLib_il2cpp_class_array_element_size,@22")
#pragma comment(linker, "/EXPORT:il2cpp_class_enum_basetype=AheadLib_il2cpp_class_enum_basetype,@23")
#pragma comment(linker, "/EXPORT:il2cpp_class_from_il2cpp_type=AheadLib_il2cpp_class_from_il2cpp_type,@24")
#pragma comment(linker, "/EXPORT:il2cpp_class_from_name=AheadLib_il2cpp_class_from_name,@25")
#pragma comment(linker, "/EXPORT:il2cpp_class_from_system_type=AheadLib_il2cpp_class_from_system_type,@26")
#pragma comment(linker, "/EXPORT:il2cpp_class_from_type=AheadLib_il2cpp_class_from_type,@27")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_assemblyname=AheadLib_il2cpp_class_get_assemblyname,@28")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_bitmap=AheadLib_il2cpp_class_get_bitmap,@29")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_bitmap_size=AheadLib_il2cpp_class_get_bitmap_size,@30")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_declaring_type=AheadLib_il2cpp_class_get_declaring_type,@31")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_element_class=AheadLib_il2cpp_class_get_element_class,@32")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_events=AheadLib_il2cpp_class_get_events,@33")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_field_from_name=AheadLib_il2cpp_class_get_field_from_name,@34")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_fields=AheadLib_il2cpp_class_get_fields,@35")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_flags=AheadLib_il2cpp_class_get_flags,@36")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_image=AheadLib_il2cpp_class_get_image,@37")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_interfaces=AheadLib_il2cpp_class_get_interfaces,@38")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_method_from_name=AheadLib_il2cpp_class_get_method_from_name,@39")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_methods=AheadLib_il2cpp_class_get_methods,@40")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_name=AheadLib_il2cpp_class_get_name,@41")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_namespace=AheadLib_il2cpp_class_get_namespace,@42")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_nested_types=AheadLib_il2cpp_class_get_nested_types,@43")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_parent=AheadLib_il2cpp_class_get_parent,@44")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_properties=AheadLib_il2cpp_class_get_properties,@45")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_property_from_name=AheadLib_il2cpp_class_get_property_from_name,@46")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_rank=AheadLib_il2cpp_class_get_rank,@47")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_type=AheadLib_il2cpp_class_get_type,@48")
#pragma comment(linker, "/EXPORT:il2cpp_class_get_type_token=AheadLib_il2cpp_class_get_type_token,@49")
#pragma comment(linker, "/EXPORT:il2cpp_class_has_attribute=AheadLib_il2cpp_class_has_attribute,@50")
#pragma comment(linker, "/EXPORT:il2cpp_class_has_parent=AheadLib_il2cpp_class_has_parent,@51")
#pragma comment(linker, "/EXPORT:il2cpp_class_has_references=AheadLib_il2cpp_class_has_references,@52")
#pragma comment(linker, "/EXPORT:il2cpp_class_instance_size=AheadLib_il2cpp_class_instance_size,@53")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_abstract=AheadLib_il2cpp_class_is_abstract,@54")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_assignable_from=AheadLib_il2cpp_class_is_assignable_from,@55")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_blittable=AheadLib_il2cpp_class_is_blittable,@56")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_enum=AheadLib_il2cpp_class_is_enum,@57")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_generic=AheadLib_il2cpp_class_is_generic,@58")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_inflated=AheadLib_il2cpp_class_is_inflated,@59")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_interface=AheadLib_il2cpp_class_is_interface,@60")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_subclass_of=AheadLib_il2cpp_class_is_subclass_of,@61")
#pragma comment(linker, "/EXPORT:il2cpp_class_is_valuetype=AheadLib_il2cpp_class_is_valuetype,@62")
#pragma comment(linker, "/EXPORT:il2cpp_class_num_fields=AheadLib_il2cpp_class_num_fields,@63")
#pragma comment(linker, "/EXPORT:il2cpp_class_value_size=AheadLib_il2cpp_class_value_size,@64")
#pragma comment(linker, "/EXPORT:il2cpp_current_thread_get_frame_at=AheadLib_il2cpp_current_thread_get_frame_at,@65")
#pragma comment(linker, "/EXPORT:il2cpp_current_thread_get_stack_depth=AheadLib_il2cpp_current_thread_get_stack_depth,@66")
#pragma comment(linker, "/EXPORT:il2cpp_current_thread_get_top_frame=AheadLib_il2cpp_current_thread_get_top_frame,@67")
#pragma comment(linker, "/EXPORT:il2cpp_current_thread_walk_frame_stack=AheadLib_il2cpp_current_thread_walk_frame_stack,@68")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_construct=AheadLib_il2cpp_custom_attrs_construct,@69")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_free=AheadLib_il2cpp_custom_attrs_free,@70")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_from_class=AheadLib_il2cpp_custom_attrs_from_class,@71")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_from_method=AheadLib_il2cpp_custom_attrs_from_method,@72")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_get_attr=AheadLib_il2cpp_custom_attrs_get_attr,@73")
#pragma comment(linker, "/EXPORT:il2cpp_custom_attrs_has_attr=AheadLib_il2cpp_custom_attrs_has_attr,@74")
#pragma comment(linker, "/EXPORT:il2cpp_debugger_set_agent_options=AheadLib_il2cpp_debugger_set_agent_options,@75")
#pragma comment(linker, "/EXPORT:il2cpp_domain_assembly_open=AheadLib_il2cpp_domain_assembly_open,@76")
#pragma comment(linker, "/EXPORT:il2cpp_domain_get=AheadLib_il2cpp_domain_get,@77")
#pragma comment(linker, "/EXPORT:il2cpp_domain_get_assemblies=AheadLib_il2cpp_domain_get_assemblies,@78")
#pragma comment(linker, "/EXPORT:il2cpp_exception_from_name_msg=AheadLib_il2cpp_exception_from_name_msg,@79")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_flags=AheadLib_il2cpp_field_get_flags,@80")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_name=AheadLib_il2cpp_field_get_name,@81")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_offset=AheadLib_il2cpp_field_get_offset,@82")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_parent=AheadLib_il2cpp_field_get_parent,@83")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_type=AheadLib_il2cpp_field_get_type,@84")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_value=AheadLib_il2cpp_field_get_value,@85")
#pragma comment(linker, "/EXPORT:il2cpp_field_get_value_object=AheadLib_il2cpp_field_get_value_object,@86")
#pragma comment(linker, "/EXPORT:il2cpp_field_has_attribute=AheadLib_il2cpp_field_has_attribute,@87")
#pragma comment(linker, "/EXPORT:il2cpp_field_set_value=AheadLib_il2cpp_field_set_value,@88")
#pragma comment(linker, "/EXPORT:il2cpp_field_set_value_object=AheadLib_il2cpp_field_set_value_object,@89")
#pragma comment(linker, "/EXPORT:il2cpp_field_static_get_value=AheadLib_il2cpp_field_static_get_value,@90")
#pragma comment(linker, "/EXPORT:il2cpp_field_static_set_value=AheadLib_il2cpp_field_static_set_value,@91")
#pragma comment(linker, "/EXPORT:il2cpp_format_exception=AheadLib_il2cpp_format_exception,@92")
#pragma comment(linker, "/EXPORT:il2cpp_format_stack_trace=AheadLib_il2cpp_format_stack_trace,@93")
#pragma comment(linker, "/EXPORT:il2cpp_free=AheadLib_il2cpp_free,@94")
#pragma comment(linker, "/EXPORT:il2cpp_free_captured_memory_snapshot=AheadLib_il2cpp_free_captured_memory_snapshot,@95")
#pragma comment(linker, "/EXPORT:il2cpp_gc_collect=AheadLib_il2cpp_gc_collect,@96")
#pragma comment(linker, "/EXPORT:il2cpp_gc_collect_a_little=AheadLib_il2cpp_gc_collect_a_little,@97")
#pragma comment(linker, "/EXPORT:il2cpp_gc_disable=AheadLib_il2cpp_gc_disable,@98")
#pragma comment(linker, "/EXPORT:il2cpp_gc_enable=AheadLib_il2cpp_gc_enable,@99")
#pragma comment(linker, "/EXPORT:il2cpp_gc_get_heap_size=AheadLib_il2cpp_gc_get_heap_size,@100")
#pragma comment(linker, "/EXPORT:il2cpp_gc_get_used_size=AheadLib_il2cpp_gc_get_used_size,@101")
#pragma comment(linker, "/EXPORT:il2cpp_gc_is_disabled=AheadLib_il2cpp_gc_is_disabled,@102")
#pragma comment(linker, "/EXPORT:il2cpp_gc_wbarrier_set_field=AheadLib_il2cpp_gc_wbarrier_set_field,@103")
#pragma comment(linker, "/EXPORT:il2cpp_gchandle_free=AheadLib_il2cpp_gchandle_free,@104")
#pragma comment(linker, "/EXPORT:il2cpp_gchandle_get_target=AheadLib_il2cpp_gchandle_get_target,@105")
#pragma comment(linker, "/EXPORT:il2cpp_gchandle_new=AheadLib_il2cpp_gchandle_new,@106")
#pragma comment(linker, "/EXPORT:il2cpp_gchandle_new_weakref=AheadLib_il2cpp_gchandle_new_weakref,@107")
#pragma comment(linker, "/EXPORT:il2cpp_get_corlib=AheadLib_il2cpp_get_corlib,@108")
#pragma comment(linker, "/EXPORT:il2cpp_get_exception_argument_null=AheadLib_il2cpp_get_exception_argument_null,@109")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_assembly=AheadLib_il2cpp_image_get_assembly,@110")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_class=AheadLib_il2cpp_image_get_class,@111")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_class_count=AheadLib_il2cpp_image_get_class_count,@112")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_entry_point=AheadLib_il2cpp_image_get_entry_point,@113")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_filename=AheadLib_il2cpp_image_get_filename,@114")
#pragma comment(linker, "/EXPORT:il2cpp_image_get_name=AheadLib_il2cpp_image_get_name,@115")
#pragma comment(linker, "/EXPORT:il2cpp_init=AheadLib_il2cpp_init,@116")
#pragma comment(linker, "/EXPORT:il2cpp_init_utf16=AheadLib_il2cpp_init_utf16,@117")
#pragma comment(linker, "/EXPORT:il2cpp_is_debugger_attached=AheadLib_il2cpp_is_debugger_attached,@118")
#pragma comment(linker, "/EXPORT:il2cpp_is_vm_thread=AheadLib_il2cpp_is_vm_thread,@119")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_class=AheadLib_il2cpp_method_get_class,@120")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_declaring_type=AheadLib_il2cpp_method_get_declaring_type,@121")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_flags=AheadLib_il2cpp_method_get_flags,@122")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_from_reflection=AheadLib_il2cpp_method_get_from_reflection,@123")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_name=AheadLib_il2cpp_method_get_name,@124")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_object=AheadLib_il2cpp_method_get_object,@125")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_param=AheadLib_il2cpp_method_get_param,@126")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_param_count=AheadLib_il2cpp_method_get_param_count,@127")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_param_name=AheadLib_il2cpp_method_get_param_name,@128")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_return_type=AheadLib_il2cpp_method_get_return_type,@129")
#pragma comment(linker, "/EXPORT:il2cpp_method_get_token=AheadLib_il2cpp_method_get_token,@130")
#pragma comment(linker, "/EXPORT:il2cpp_method_has_attribute=AheadLib_il2cpp_method_has_attribute,@131")
#pragma comment(linker, "/EXPORT:il2cpp_method_is_generic=AheadLib_il2cpp_method_is_generic,@132")
#pragma comment(linker, "/EXPORT:il2cpp_method_is_inflated=AheadLib_il2cpp_method_is_inflated,@133")
#pragma comment(linker, "/EXPORT:il2cpp_method_is_instance=AheadLib_il2cpp_method_is_instance,@134")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_enter=AheadLib_il2cpp_monitor_enter,@135")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_exit=AheadLib_il2cpp_monitor_exit,@136")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_pulse=AheadLib_il2cpp_monitor_pulse,@137")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_pulse_all=AheadLib_il2cpp_monitor_pulse_all,@138")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_try_enter=AheadLib_il2cpp_monitor_try_enter,@139")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_try_wait=AheadLib_il2cpp_monitor_try_wait,@140")
#pragma comment(linker, "/EXPORT:il2cpp_monitor_wait=AheadLib_il2cpp_monitor_wait,@141")
#pragma comment(linker, "/EXPORT:il2cpp_object_get_class=AheadLib_il2cpp_object_get_class,@142")
#pragma comment(linker, "/EXPORT:il2cpp_object_get_size=AheadLib_il2cpp_object_get_size,@143")
#pragma comment(linker, "/EXPORT:il2cpp_object_get_virtual_method=AheadLib_il2cpp_object_get_virtual_method,@144")
#pragma comment(linker, "/EXPORT:il2cpp_object_new=AheadLib_il2cpp_object_new,@145")
#pragma comment(linker, "/EXPORT:il2cpp_object_unbox=AheadLib_il2cpp_object_unbox,@146")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install=AheadLib_il2cpp_profiler_install,@147")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install_allocation=AheadLib_il2cpp_profiler_install_allocation,@148")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install_enter_leave=AheadLib_il2cpp_profiler_install_enter_leave,@149")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install_fileio=AheadLib_il2cpp_profiler_install_fileio,@150")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install_gc=AheadLib_il2cpp_profiler_install_gc,@151")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_install_thread=AheadLib_il2cpp_profiler_install_thread,@152")
#pragma comment(linker, "/EXPORT:il2cpp_profiler_set_events=AheadLib_il2cpp_profiler_set_events,@153")
#pragma comment(linker, "/EXPORT:il2cpp_property_get_flags=AheadLib_il2cpp_property_get_flags,@154")
#pragma comment(linker, "/EXPORT:il2cpp_property_get_get_method=AheadLib_il2cpp_property_get_get_method,@155")
#pragma comment(linker, "/EXPORT:il2cpp_property_get_name=AheadLib_il2cpp_property_get_name,@156")
#pragma comment(linker, "/EXPORT:il2cpp_property_get_parent=AheadLib_il2cpp_property_get_parent,@157")
#pragma comment(linker, "/EXPORT:il2cpp_property_get_set_method=AheadLib_il2cpp_property_get_set_method,@158")
#pragma comment(linker, "/EXPORT:il2cpp_raise_exception=AheadLib_il2cpp_raise_exception,@159")
#pragma comment(linker, "/EXPORT:il2cpp_register_log_callback=AheadLib_il2cpp_register_log_callback,@160")
#pragma comment(linker, "/EXPORT:il2cpp_resolve_icall=AheadLib_il2cpp_resolve_icall,@161")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_class_init=AheadLib_il2cpp_runtime_class_init,@162")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_invoke=AheadLib_il2cpp_runtime_invoke,@163")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_invoke_convert_args=AheadLib_il2cpp_runtime_invoke_convert_args,@164")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_object_init=AheadLib_il2cpp_runtime_object_init,@165")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_object_init_exception=AheadLib_il2cpp_runtime_object_init_exception,@166")
#pragma comment(linker, "/EXPORT:il2cpp_runtime_unhandled_exception_policy_set=AheadLib_il2cpp_runtime_unhandled_exception_policy_set,@167")
#pragma comment(linker, "/EXPORT:il2cpp_set_commandline_arguments=AheadLib_il2cpp_set_commandline_arguments,@168")
#pragma comment(linker, "/EXPORT:il2cpp_set_commandline_arguments_utf16=AheadLib_il2cpp_set_commandline_arguments_utf16,@169")
#pragma comment(linker, "/EXPORT:il2cpp_set_config=AheadLib_il2cpp_set_config,@170")
#pragma comment(linker, "/EXPORT:il2cpp_set_config_dir=AheadLib_il2cpp_set_config_dir,@171")
#pragma comment(linker, "/EXPORT:il2cpp_set_config_utf16=AheadLib_il2cpp_set_config_utf16,@172")
#pragma comment(linker, "/EXPORT:il2cpp_set_data_dir=AheadLib_il2cpp_set_data_dir,@173")
#pragma comment(linker, "/EXPORT:il2cpp_set_find_plugin_callback=AheadLib_il2cpp_set_find_plugin_callback,@174")
#pragma comment(linker, "/EXPORT:il2cpp_set_memory_callbacks=AheadLib_il2cpp_set_memory_callbacks,@175")
#pragma comment(linker, "/EXPORT:il2cpp_set_temp_dir=AheadLib_il2cpp_set_temp_dir,@176")
#pragma comment(linker, "/EXPORT:il2cpp_shutdown=AheadLib_il2cpp_shutdown,@177")
#pragma comment(linker, "/EXPORT:il2cpp_stats_dump_to_file=AheadLib_il2cpp_stats_dump_to_file,@178")
#pragma comment(linker, "/EXPORT:il2cpp_stats_get_value=AheadLib_il2cpp_stats_get_value,@179")
#pragma comment(linker, "/EXPORT:il2cpp_string_chars=AheadLib_il2cpp_string_chars,@180")
#pragma comment(linker, "/EXPORT:il2cpp_string_intern=AheadLib_il2cpp_string_intern,@181")
#pragma comment(linker, "/EXPORT:il2cpp_string_is_interned=AheadLib_il2cpp_string_is_interned,@182")
#pragma comment(linker, "/EXPORT:il2cpp_string_length=AheadLib_il2cpp_string_length,@183")
#pragma comment(linker, "/EXPORT:il2cpp_string_new=AheadLib_il2cpp_string_new,@184")
#pragma comment(linker, "/EXPORT:il2cpp_string_new_len=AheadLib_il2cpp_string_new_len,@185")
#pragma comment(linker, "/EXPORT:il2cpp_string_new_utf16=AheadLib_il2cpp_string_new_utf16,@186")
#pragma comment(linker, "/EXPORT:il2cpp_string_new_wrapper=AheadLib_il2cpp_string_new_wrapper,@187")
#pragma comment(linker, "/EXPORT:il2cpp_thread_attach=AheadLib_il2cpp_thread_attach,@188")
#pragma comment(linker, "/EXPORT:il2cpp_thread_current=AheadLib_il2cpp_thread_current,@189")
#pragma comment(linker, "/EXPORT:il2cpp_thread_detach=AheadLib_il2cpp_thread_detach,@190")
#pragma comment(linker, "/EXPORT:il2cpp_thread_get_all_attached_threads=AheadLib_il2cpp_thread_get_all_attached_threads,@191")
#pragma comment(linker, "/EXPORT:il2cpp_thread_get_frame_at=AheadLib_il2cpp_thread_get_frame_at,@192")
#pragma comment(linker, "/EXPORT:il2cpp_thread_get_stack_depth=AheadLib_il2cpp_thread_get_stack_depth,@193")
#pragma comment(linker, "/EXPORT:il2cpp_thread_get_top_frame=AheadLib_il2cpp_thread_get_top_frame,@194")
#pragma comment(linker, "/EXPORT:il2cpp_thread_walk_frame_stack=AheadLib_il2cpp_thread_walk_frame_stack,@195")
#pragma comment(linker, "/EXPORT:il2cpp_type_equals=AheadLib_il2cpp_type_equals,@196")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_assembly_qualified_name=AheadLib_il2cpp_type_get_assembly_qualified_name,@197")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_attrs=AheadLib_il2cpp_type_get_attrs,@198")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_class_or_element_class=AheadLib_il2cpp_type_get_class_or_element_class,@199")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_name=AheadLib_il2cpp_type_get_name,@200")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_object=AheadLib_il2cpp_type_get_object,@201")
#pragma comment(linker, "/EXPORT:il2cpp_type_get_type=AheadLib_il2cpp_type_get_type,@202")
#pragma comment(linker, "/EXPORT:il2cpp_type_is_byref=AheadLib_il2cpp_type_is_byref,@203")
#pragma comment(linker, "/EXPORT:il2cpp_unhandled_exception=AheadLib_il2cpp_unhandled_exception,@204")
#pragma comment(linker, "/EXPORT:il2cpp_unity_install_unitytls_interface=AheadLib_il2cpp_unity_install_unitytls_interface,@205")
#pragma comment(linker, "/EXPORT:il2cpp_unity_liveness_calculation_begin=AheadLib_il2cpp_unity_liveness_calculation_begin,@206")
#pragma comment(linker, "/EXPORT:il2cpp_unity_liveness_calculation_end=AheadLib_il2cpp_unity_liveness_calculation_end,@207")
#pragma comment(linker, "/EXPORT:il2cpp_unity_liveness_calculation_from_root=AheadLib_il2cpp_unity_liveness_calculation_from_root,@208")
#pragma comment(linker, "/EXPORT:il2cpp_unity_liveness_calculation_from_statics=AheadLib_il2cpp_unity_liveness_calculation_from_statics,@209")
#pragma comment(linker, "/EXPORT:il2cpp_value_box=AheadLib_il2cpp_value_box,@210")


// 导出的函数
extern "C" 
{
	PVOID pfnAheadLib_CloseZStream;
	PVOID pfnAheadLib_CreateZStream;
	PVOID pfnAheadLib_Flush;
	PVOID pfnAheadLib_ReadZStream;
	PVOID pfnAheadLib_UnityPalGetLocalTimeZoneData;
	PVOID pfnAheadLib_UnityPalGetTimeZoneDataForID;
	PVOID pfnAheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs;
	PVOID pfnAheadLib_UseUnityPalForTimeZoneInformation;
	PVOID pfnAheadLib_WriteZStream;
	PVOID pfnAheadLib_il2cpp_add_internal_call;
	PVOID pfnAheadLib_il2cpp_alloc;
	PVOID pfnAheadLib_il2cpp_array_class_get;
	PVOID pfnAheadLib_il2cpp_array_element_size;
	PVOID pfnAheadLib_il2cpp_array_get_byte_length;
	PVOID pfnAheadLib_il2cpp_array_length;
	PVOID pfnAheadLib_il2cpp_array_new;
	PVOID pfnAheadLib_il2cpp_array_new_full;
	PVOID pfnAheadLib_il2cpp_array_new_specific;
	PVOID pfnAheadLib_il2cpp_assembly_get_image;
	PVOID pfnAheadLib_il2cpp_bounded_array_class_get;
	PVOID pfnAheadLib_il2cpp_capture_memory_snapshot;
	PVOID pfnAheadLib_il2cpp_class_array_element_size;
	PVOID pfnAheadLib_il2cpp_class_enum_basetype;
	PVOID pfnAheadLib_il2cpp_class_from_il2cpp_type;
	PVOID pfnAheadLib_il2cpp_class_from_name;
	PVOID pfnAheadLib_il2cpp_class_from_system_type;
	PVOID pfnAheadLib_il2cpp_class_from_type;
	PVOID pfnAheadLib_il2cpp_class_get_assemblyname;
	PVOID pfnAheadLib_il2cpp_class_get_bitmap;
	PVOID pfnAheadLib_il2cpp_class_get_bitmap_size;
	PVOID pfnAheadLib_il2cpp_class_get_declaring_type;
	PVOID pfnAheadLib_il2cpp_class_get_element_class;
	PVOID pfnAheadLib_il2cpp_class_get_events;
	PVOID pfnAheadLib_il2cpp_class_get_field_from_name;
	PVOID pfnAheadLib_il2cpp_class_get_fields;
	PVOID pfnAheadLib_il2cpp_class_get_flags;
	PVOID pfnAheadLib_il2cpp_class_get_image;
	PVOID pfnAheadLib_il2cpp_class_get_interfaces;
	PVOID pfnAheadLib_il2cpp_class_get_method_from_name;
	PVOID pfnAheadLib_il2cpp_class_get_methods;
	PVOID pfnAheadLib_il2cpp_class_get_name;
	PVOID pfnAheadLib_il2cpp_class_get_namespace;
	PVOID pfnAheadLib_il2cpp_class_get_nested_types;
	PVOID pfnAheadLib_il2cpp_class_get_parent;
	PVOID pfnAheadLib_il2cpp_class_get_properties;
	PVOID pfnAheadLib_il2cpp_class_get_property_from_name;
	PVOID pfnAheadLib_il2cpp_class_get_rank;
	PVOID pfnAheadLib_il2cpp_class_get_type;
	PVOID pfnAheadLib_il2cpp_class_get_type_token;
	PVOID pfnAheadLib_il2cpp_class_has_attribute;
	PVOID pfnAheadLib_il2cpp_class_has_parent;
	PVOID pfnAheadLib_il2cpp_class_has_references;
	PVOID pfnAheadLib_il2cpp_class_instance_size;
	PVOID pfnAheadLib_il2cpp_class_is_abstract;
	PVOID pfnAheadLib_il2cpp_class_is_assignable_from;
	PVOID pfnAheadLib_il2cpp_class_is_blittable;
	PVOID pfnAheadLib_il2cpp_class_is_enum;
	PVOID pfnAheadLib_il2cpp_class_is_generic;
	PVOID pfnAheadLib_il2cpp_class_is_inflated;
	PVOID pfnAheadLib_il2cpp_class_is_interface;
	PVOID pfnAheadLib_il2cpp_class_is_subclass_of;
	PVOID pfnAheadLib_il2cpp_class_is_valuetype;
	PVOID pfnAheadLib_il2cpp_class_num_fields;
	PVOID pfnAheadLib_il2cpp_class_value_size;
	PVOID pfnAheadLib_il2cpp_current_thread_get_frame_at;
	PVOID pfnAheadLib_il2cpp_current_thread_get_stack_depth;
	PVOID pfnAheadLib_il2cpp_current_thread_get_top_frame;
	PVOID pfnAheadLib_il2cpp_current_thread_walk_frame_stack;
	PVOID pfnAheadLib_il2cpp_custom_attrs_construct;
	PVOID pfnAheadLib_il2cpp_custom_attrs_free;
	PVOID pfnAheadLib_il2cpp_custom_attrs_from_class;
	PVOID pfnAheadLib_il2cpp_custom_attrs_from_method;
	PVOID pfnAheadLib_il2cpp_custom_attrs_get_attr;
	PVOID pfnAheadLib_il2cpp_custom_attrs_has_attr;
	PVOID pfnAheadLib_il2cpp_debugger_set_agent_options;
	PVOID pfnAheadLib_il2cpp_domain_assembly_open;
	PVOID pfnAheadLib_il2cpp_domain_get;
	PVOID pfnAheadLib_il2cpp_domain_get_assemblies;
	PVOID pfnAheadLib_il2cpp_exception_from_name_msg;
	PVOID pfnAheadLib_il2cpp_field_get_flags;
	PVOID pfnAheadLib_il2cpp_field_get_name;
	PVOID pfnAheadLib_il2cpp_field_get_offset;
	PVOID pfnAheadLib_il2cpp_field_get_parent;
	PVOID pfnAheadLib_il2cpp_field_get_type;
	PVOID pfnAheadLib_il2cpp_field_get_value;
	PVOID pfnAheadLib_il2cpp_field_get_value_object;
	PVOID pfnAheadLib_il2cpp_field_has_attribute;
	PVOID pfnAheadLib_il2cpp_field_set_value;
	PVOID pfnAheadLib_il2cpp_field_set_value_object;
	PVOID pfnAheadLib_il2cpp_field_static_get_value;
	PVOID pfnAheadLib_il2cpp_field_static_set_value;
	PVOID pfnAheadLib_il2cpp_format_exception;
	PVOID pfnAheadLib_il2cpp_format_stack_trace;
	PVOID pfnAheadLib_il2cpp_free;
	PVOID pfnAheadLib_il2cpp_free_captured_memory_snapshot;
	PVOID pfnAheadLib_il2cpp_gc_collect;
	PVOID pfnAheadLib_il2cpp_gc_collect_a_little;
	PVOID pfnAheadLib_il2cpp_gc_disable;
	PVOID pfnAheadLib_il2cpp_gc_enable;
	PVOID pfnAheadLib_il2cpp_gc_get_heap_size;
	PVOID pfnAheadLib_il2cpp_gc_get_used_size;
	PVOID pfnAheadLib_il2cpp_gc_is_disabled;
	PVOID pfnAheadLib_il2cpp_gc_wbarrier_set_field;
	PVOID pfnAheadLib_il2cpp_gchandle_free;
	PVOID pfnAheadLib_il2cpp_gchandle_get_target;
	PVOID pfnAheadLib_il2cpp_gchandle_new;
	PVOID pfnAheadLib_il2cpp_gchandle_new_weakref;
	PVOID pfnAheadLib_il2cpp_get_corlib;
	PVOID pfnAheadLib_il2cpp_get_exception_argument_null;
	PVOID pfnAheadLib_il2cpp_image_get_assembly;
	PVOID pfnAheadLib_il2cpp_image_get_class;
	PVOID pfnAheadLib_il2cpp_image_get_class_count;
	PVOID pfnAheadLib_il2cpp_image_get_entry_point;
	PVOID pfnAheadLib_il2cpp_image_get_filename;
	PVOID pfnAheadLib_il2cpp_image_get_name;
	PVOID pfnAheadLib_il2cpp_init;
	PVOID pfnAheadLib_il2cpp_init_utf16;
	PVOID pfnAheadLib_il2cpp_is_debugger_attached;
	PVOID pfnAheadLib_il2cpp_is_vm_thread;
	PVOID pfnAheadLib_il2cpp_method_get_class;
	PVOID pfnAheadLib_il2cpp_method_get_declaring_type;
	PVOID pfnAheadLib_il2cpp_method_get_flags;
	PVOID pfnAheadLib_il2cpp_method_get_from_reflection;
	PVOID pfnAheadLib_il2cpp_method_get_name;
	PVOID pfnAheadLib_il2cpp_method_get_object;
	PVOID pfnAheadLib_il2cpp_method_get_param;
	PVOID pfnAheadLib_il2cpp_method_get_param_count;
	PVOID pfnAheadLib_il2cpp_method_get_param_name;
	PVOID pfnAheadLib_il2cpp_method_get_return_type;
	PVOID pfnAheadLib_il2cpp_method_get_token;
	PVOID pfnAheadLib_il2cpp_method_has_attribute;
	PVOID pfnAheadLib_il2cpp_method_is_generic;
	PVOID pfnAheadLib_il2cpp_method_is_inflated;
	PVOID pfnAheadLib_il2cpp_method_is_instance;
	PVOID pfnAheadLib_il2cpp_monitor_enter;
	PVOID pfnAheadLib_il2cpp_monitor_exit;
	PVOID pfnAheadLib_il2cpp_monitor_pulse;
	PVOID pfnAheadLib_il2cpp_monitor_pulse_all;
	PVOID pfnAheadLib_il2cpp_monitor_try_enter;
	PVOID pfnAheadLib_il2cpp_monitor_try_wait;
	PVOID pfnAheadLib_il2cpp_monitor_wait;
	PVOID pfnAheadLib_il2cpp_object_get_class;
	PVOID pfnAheadLib_il2cpp_object_get_size;
	PVOID pfnAheadLib_il2cpp_object_get_virtual_method;
	PVOID pfnAheadLib_il2cpp_object_new;
	PVOID pfnAheadLib_il2cpp_object_unbox;
	PVOID pfnAheadLib_il2cpp_profiler_install;
	PVOID pfnAheadLib_il2cpp_profiler_install_allocation;
	PVOID pfnAheadLib_il2cpp_profiler_install_enter_leave;
	PVOID pfnAheadLib_il2cpp_profiler_install_fileio;
	PVOID pfnAheadLib_il2cpp_profiler_install_gc;
	PVOID pfnAheadLib_il2cpp_profiler_install_thread;
	PVOID pfnAheadLib_il2cpp_profiler_set_events;
	PVOID pfnAheadLib_il2cpp_property_get_flags;
	PVOID pfnAheadLib_il2cpp_property_get_get_method;
	PVOID pfnAheadLib_il2cpp_property_get_name;
	PVOID pfnAheadLib_il2cpp_property_get_parent;
	PVOID pfnAheadLib_il2cpp_property_get_set_method;
	PVOID pfnAheadLib_il2cpp_raise_exception;
	PVOID pfnAheadLib_il2cpp_register_log_callback;
	PVOID pfnAheadLib_il2cpp_resolve_icall;
	PVOID pfnAheadLib_il2cpp_runtime_class_init;
	PVOID pfnAheadLib_il2cpp_runtime_invoke;
	PVOID pfnAheadLib_il2cpp_runtime_invoke_convert_args;
	PVOID pfnAheadLib_il2cpp_runtime_object_init;
	PVOID pfnAheadLib_il2cpp_runtime_object_init_exception;
	PVOID pfnAheadLib_il2cpp_runtime_unhandled_exception_policy_set;
	PVOID pfnAheadLib_il2cpp_set_commandline_arguments;
	PVOID pfnAheadLib_il2cpp_set_commandline_arguments_utf16;
	PVOID pfnAheadLib_il2cpp_set_config;
	PVOID pfnAheadLib_il2cpp_set_config_dir;
	PVOID pfnAheadLib_il2cpp_set_config_utf16;
	PVOID pfnAheadLib_il2cpp_set_data_dir;
	PVOID pfnAheadLib_il2cpp_set_find_plugin_callback;
	PVOID pfnAheadLib_il2cpp_set_memory_callbacks;
	PVOID pfnAheadLib_il2cpp_set_temp_dir;
	PVOID pfnAheadLib_il2cpp_shutdown;
	PVOID pfnAheadLib_il2cpp_stats_dump_to_file;
	PVOID pfnAheadLib_il2cpp_stats_get_value;
	PVOID pfnAheadLib_il2cpp_string_chars;
	PVOID pfnAheadLib_il2cpp_string_intern;
	PVOID pfnAheadLib_il2cpp_string_is_interned;
	PVOID pfnAheadLib_il2cpp_string_length;
	PVOID pfnAheadLib_il2cpp_string_new;
	PVOID pfnAheadLib_il2cpp_string_new_len;
	PVOID pfnAheadLib_il2cpp_string_new_utf16;
	PVOID pfnAheadLib_il2cpp_string_new_wrapper;
	PVOID pfnAheadLib_il2cpp_thread_attach;
	PVOID pfnAheadLib_il2cpp_thread_current;
	PVOID pfnAheadLib_il2cpp_thread_detach;
	PVOID pfnAheadLib_il2cpp_thread_get_all_attached_threads;
	PVOID pfnAheadLib_il2cpp_thread_get_frame_at;
	PVOID pfnAheadLib_il2cpp_thread_get_stack_depth;
	PVOID pfnAheadLib_il2cpp_thread_get_top_frame;
	PVOID pfnAheadLib_il2cpp_thread_walk_frame_stack;
	PVOID pfnAheadLib_il2cpp_type_equals;
	PVOID pfnAheadLib_il2cpp_type_get_assembly_qualified_name;
	PVOID pfnAheadLib_il2cpp_type_get_attrs;
	PVOID pfnAheadLib_il2cpp_type_get_class_or_element_class;
	PVOID pfnAheadLib_il2cpp_type_get_name;
	PVOID pfnAheadLib_il2cpp_type_get_object;
	PVOID pfnAheadLib_il2cpp_type_get_type;
	PVOID pfnAheadLib_il2cpp_type_is_byref;
	PVOID pfnAheadLib_il2cpp_unhandled_exception;
	PVOID pfnAheadLib_il2cpp_unity_install_unitytls_interface;
	PVOID pfnAheadLib_il2cpp_unity_liveness_calculation_begin;
	PVOID pfnAheadLib_il2cpp_unity_liveness_calculation_end;
	PVOID pfnAheadLib_il2cpp_unity_liveness_calculation_from_root;
	PVOID pfnAheadLib_il2cpp_unity_liveness_calculation_from_statics;
	PVOID pfnAheadLib_il2cpp_value_box;
}

// 默认最大魔法值
int g_nMaxMana = 0x78;
// 魔法值不扣减的标志
int g_nManaHookFlag = FALSE;
// 魔法值不扣减的代码起始地址
ULONG64 ulHookBeginManaHold = NULL;
// 是否已经HOOK魔法值
int g_nAlreadyHookManaFlag = FALSE;
// 魔法值代码HOOK从哪里开始
ULONG64 g_ulManaHookBegin = NULL;
// 本次要修改的魔法值数值
DWORD g_dwManaNumber = NULL;
// 本次要修改的魔法值地址
extern "C" ULONG64 g_ulManaAddress = NULL;
// 魔法值修改完后应该返回的位置
extern "C" ULONG64 g_ulManaHookOver = NULL;
// 卸载魔法值钩子
extern "C" VOID WINAPI UnHookMana();
// 内联汇编
extern "C" VOID WINAPI FuncManaAsm();
// 原始模块基址
HMODULE g_OldModule = NULL;
// 当前DLL模块句柄
HINSTANCE g_hInstance = NULL;
// 原代码(魔法值数值 -> 修改魔法值)
UCHAR OldCodeArrayMana[] = {0x44, 0x8B, 0x73, 0x1C, 0x41, 0x8B, 0xCE};
// 原代码(减少魔法值 -> 锁定魔法值)
UCHAR OldCodeArrayManaHold[] = {0x2B, 0xCF};
// 原代码(基础魔法值 -> 修改基础魔法值)
UCHAR OldBaseMana[] = {0x89, 0x43, 0x58};
// 原代码(人物等级 -> 修改人物等级)
UCHAR OldCharacterLevel[] = {0x8B, 0x40, 0x44, 0x48, 0x83, 0xC4, 0x28};

// 卸载模块
VOID WINAPI Free()
{
	if (g_OldModule)
	{
		FreeLibrary(g_OldModule);
	}
}

// 加载模块
BOOL WINAPI Load()
{
	TCHAR tzPath[MAX_PATH];
	TCHAR tzTemp[MAX_PATH * 2];

	//
	// 这里是否从系统目录或当前目录加载原始DLL
	//
	GetCurrentDirectory(MAX_PATH, tzPath); // 获取本目录下的
	
	// GetSystemDirectory(tzPath, MAX_PATH); // 默认获取系统目录的
	lstrcat(tzPath, TEXT("\\OldGameAssembly.dll"));

	g_OldModule = LoadLibrary(tzPath);
	if (g_OldModule == NULL)
	{
		wsprintf(tzTemp, TEXT("无法找到模块 %s,程序无法正常运行"), tzPath);
		MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
	}
	return (g_OldModule != NULL);
}

// 得到原始模块中函数地址
FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress;
	CHAR szProcName[64];
	TCHAR tzTemp[MAX_PATH];

	fpAddress = GetProcAddress(g_OldModule, pszProcName);
	if (fpAddress == NULL)
	{
		if (HIWORD(pszProcName) == 0)
		{
			wsprintfA(szProcName, "#%d", pszProcName);
			pszProcName = szProcName;
		}
		wsprintf(tzTemp, TEXT("无法找到函数 %hs,程序无法正常运行"), pszProcName);
		MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		ExitProcess(-2);
	}
	return fpAddress;
}

// 初始化函数
BOOL WINAPI Init()
{
	pfnAheadLib_CloseZStream = GetAddress("CloseZStream");
	pfnAheadLib_CreateZStream = GetAddress("CreateZStream");
	pfnAheadLib_Flush = GetAddress("Flush");
	pfnAheadLib_ReadZStream = GetAddress("ReadZStream");
	pfnAheadLib_UnityPalGetLocalTimeZoneData = GetAddress("UnityPalGetLocalTimeZoneData");
	pfnAheadLib_UnityPalGetTimeZoneDataForID = GetAddress("UnityPalGetTimeZoneDataForID");
	pfnAheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs = GetAddress("UnityPalTimeZoneInfoGetTimeZoneIDs");
	pfnAheadLib_UseUnityPalForTimeZoneInformation = GetAddress("UseUnityPalForTimeZoneInformation");
	pfnAheadLib_WriteZStream = GetAddress("WriteZStream");
	pfnAheadLib_il2cpp_add_internal_call = GetAddress("il2cpp_add_internal_call");
	pfnAheadLib_il2cpp_alloc = GetAddress("il2cpp_alloc");
	pfnAheadLib_il2cpp_array_class_get = GetAddress("il2cpp_array_class_get");
	pfnAheadLib_il2cpp_array_element_size = GetAddress("il2cpp_array_element_size");
	pfnAheadLib_il2cpp_array_get_byte_length = GetAddress("il2cpp_array_get_byte_length");
	pfnAheadLib_il2cpp_array_length = GetAddress("il2cpp_array_length");
	pfnAheadLib_il2cpp_array_new = GetAddress("il2cpp_array_new");
	pfnAheadLib_il2cpp_array_new_full = GetAddress("il2cpp_array_new_full");
	pfnAheadLib_il2cpp_array_new_specific = GetAddress("il2cpp_array_new_specific");
	pfnAheadLib_il2cpp_assembly_get_image = GetAddress("il2cpp_assembly_get_image");
	pfnAheadLib_il2cpp_bounded_array_class_get = GetAddress("il2cpp_bounded_array_class_get");
	pfnAheadLib_il2cpp_capture_memory_snapshot = GetAddress("il2cpp_capture_memory_snapshot");
	pfnAheadLib_il2cpp_class_array_element_size = GetAddress("il2cpp_class_array_element_size");
	pfnAheadLib_il2cpp_class_enum_basetype = GetAddress("il2cpp_class_enum_basetype");
	pfnAheadLib_il2cpp_class_from_il2cpp_type = GetAddress("il2cpp_class_from_il2cpp_type");
	pfnAheadLib_il2cpp_class_from_name = GetAddress("il2cpp_class_from_name");
	pfnAheadLib_il2cpp_class_from_system_type = GetAddress("il2cpp_class_from_system_type");
	pfnAheadLib_il2cpp_class_from_type = GetAddress("il2cpp_class_from_type");
	pfnAheadLib_il2cpp_class_get_assemblyname = GetAddress("il2cpp_class_get_assemblyname");
	pfnAheadLib_il2cpp_class_get_bitmap = GetAddress("il2cpp_class_get_bitmap");
	pfnAheadLib_il2cpp_class_get_bitmap_size = GetAddress("il2cpp_class_get_bitmap_size");
	pfnAheadLib_il2cpp_class_get_declaring_type = GetAddress("il2cpp_class_get_declaring_type");
	pfnAheadLib_il2cpp_class_get_element_class = GetAddress("il2cpp_class_get_element_class");
	pfnAheadLib_il2cpp_class_get_events = GetAddress("il2cpp_class_get_events");
	pfnAheadLib_il2cpp_class_get_field_from_name = GetAddress("il2cpp_class_get_field_from_name");
	pfnAheadLib_il2cpp_class_get_fields = GetAddress("il2cpp_class_get_fields");
	pfnAheadLib_il2cpp_class_get_flags = GetAddress("il2cpp_class_get_flags");
	pfnAheadLib_il2cpp_class_get_image = GetAddress("il2cpp_class_get_image");
	pfnAheadLib_il2cpp_class_get_interfaces = GetAddress("il2cpp_class_get_interfaces");
	pfnAheadLib_il2cpp_class_get_method_from_name = GetAddress("il2cpp_class_get_method_from_name");
	pfnAheadLib_il2cpp_class_get_methods = GetAddress("il2cpp_class_get_methods");
	pfnAheadLib_il2cpp_class_get_name = GetAddress("il2cpp_class_get_name");
	pfnAheadLib_il2cpp_class_get_namespace = GetAddress("il2cpp_class_get_namespace");
	pfnAheadLib_il2cpp_class_get_nested_types = GetAddress("il2cpp_class_get_nested_types");
	pfnAheadLib_il2cpp_class_get_parent = GetAddress("il2cpp_class_get_parent");
	pfnAheadLib_il2cpp_class_get_properties = GetAddress("il2cpp_class_get_properties");
	pfnAheadLib_il2cpp_class_get_property_from_name = GetAddress("il2cpp_class_get_property_from_name");
	pfnAheadLib_il2cpp_class_get_rank = GetAddress("il2cpp_class_get_rank");
	pfnAheadLib_il2cpp_class_get_type = GetAddress("il2cpp_class_get_type");
	pfnAheadLib_il2cpp_class_get_type_token = GetAddress("il2cpp_class_get_type_token");
	pfnAheadLib_il2cpp_class_has_attribute = GetAddress("il2cpp_class_has_attribute");
	pfnAheadLib_il2cpp_class_has_parent = GetAddress("il2cpp_class_has_parent");
	pfnAheadLib_il2cpp_class_has_references = GetAddress("il2cpp_class_has_references");
	pfnAheadLib_il2cpp_class_instance_size = GetAddress("il2cpp_class_instance_size");
	pfnAheadLib_il2cpp_class_is_abstract = GetAddress("il2cpp_class_is_abstract");
	pfnAheadLib_il2cpp_class_is_assignable_from = GetAddress("il2cpp_class_is_assignable_from");
	pfnAheadLib_il2cpp_class_is_blittable = GetAddress("il2cpp_class_is_blittable");
	pfnAheadLib_il2cpp_class_is_enum = GetAddress("il2cpp_class_is_enum");
	pfnAheadLib_il2cpp_class_is_generic = GetAddress("il2cpp_class_is_generic");
	pfnAheadLib_il2cpp_class_is_inflated = GetAddress("il2cpp_class_is_inflated");
	pfnAheadLib_il2cpp_class_is_interface = GetAddress("il2cpp_class_is_interface");
	pfnAheadLib_il2cpp_class_is_subclass_of = GetAddress("il2cpp_class_is_subclass_of");
	pfnAheadLib_il2cpp_class_is_valuetype = GetAddress("il2cpp_class_is_valuetype");
	pfnAheadLib_il2cpp_class_num_fields = GetAddress("il2cpp_class_num_fields");
	pfnAheadLib_il2cpp_class_value_size = GetAddress("il2cpp_class_value_size");
	pfnAheadLib_il2cpp_current_thread_get_frame_at = GetAddress("il2cpp_current_thread_get_frame_at");
	pfnAheadLib_il2cpp_current_thread_get_stack_depth = GetAddress("il2cpp_current_thread_get_stack_depth");
	pfnAheadLib_il2cpp_current_thread_get_top_frame = GetAddress("il2cpp_current_thread_get_top_frame");
	pfnAheadLib_il2cpp_current_thread_walk_frame_stack = GetAddress("il2cpp_current_thread_walk_frame_stack");
	pfnAheadLib_il2cpp_custom_attrs_construct = GetAddress("il2cpp_custom_attrs_construct");
	pfnAheadLib_il2cpp_custom_attrs_free = GetAddress("il2cpp_custom_attrs_free");
	pfnAheadLib_il2cpp_custom_attrs_from_class = GetAddress("il2cpp_custom_attrs_from_class");
	pfnAheadLib_il2cpp_custom_attrs_from_method = GetAddress("il2cpp_custom_attrs_from_method");
	pfnAheadLib_il2cpp_custom_attrs_get_attr = GetAddress("il2cpp_custom_attrs_get_attr");
	pfnAheadLib_il2cpp_custom_attrs_has_attr = GetAddress("il2cpp_custom_attrs_has_attr");
	pfnAheadLib_il2cpp_debugger_set_agent_options = GetAddress("il2cpp_debugger_set_agent_options");
	pfnAheadLib_il2cpp_domain_assembly_open = GetAddress("il2cpp_domain_assembly_open");
	pfnAheadLib_il2cpp_domain_get = GetAddress("il2cpp_domain_get");
	pfnAheadLib_il2cpp_domain_get_assemblies = GetAddress("il2cpp_domain_get_assemblies");
	pfnAheadLib_il2cpp_exception_from_name_msg = GetAddress("il2cpp_exception_from_name_msg");
	pfnAheadLib_il2cpp_field_get_flags = GetAddress("il2cpp_field_get_flags");
	pfnAheadLib_il2cpp_field_get_name = GetAddress("il2cpp_field_get_name");
	pfnAheadLib_il2cpp_field_get_offset = GetAddress("il2cpp_field_get_offset");
	pfnAheadLib_il2cpp_field_get_parent = GetAddress("il2cpp_field_get_parent");
	pfnAheadLib_il2cpp_field_get_type = GetAddress("il2cpp_field_get_type");
	pfnAheadLib_il2cpp_field_get_value = GetAddress("il2cpp_field_get_value");
	pfnAheadLib_il2cpp_field_get_value_object = GetAddress("il2cpp_field_get_value_object");
	pfnAheadLib_il2cpp_field_has_attribute = GetAddress("il2cpp_field_has_attribute");
	pfnAheadLib_il2cpp_field_set_value = GetAddress("il2cpp_field_set_value");
	pfnAheadLib_il2cpp_field_set_value_object = GetAddress("il2cpp_field_set_value_object");
	pfnAheadLib_il2cpp_field_static_get_value = GetAddress("il2cpp_field_static_get_value");
	pfnAheadLib_il2cpp_field_static_set_value = GetAddress("il2cpp_field_static_set_value");
	pfnAheadLib_il2cpp_format_exception = GetAddress("il2cpp_format_exception");
	pfnAheadLib_il2cpp_format_stack_trace = GetAddress("il2cpp_format_stack_trace");
	pfnAheadLib_il2cpp_free = GetAddress("il2cpp_free");
	pfnAheadLib_il2cpp_free_captured_memory_snapshot = GetAddress("il2cpp_free_captured_memory_snapshot");
	pfnAheadLib_il2cpp_gc_collect = GetAddress("il2cpp_gc_collect");
	pfnAheadLib_il2cpp_gc_collect_a_little = GetAddress("il2cpp_gc_collect_a_little");
	pfnAheadLib_il2cpp_gc_disable = GetAddress("il2cpp_gc_disable");
	pfnAheadLib_il2cpp_gc_enable = GetAddress("il2cpp_gc_enable");
	pfnAheadLib_il2cpp_gc_get_heap_size = GetAddress("il2cpp_gc_get_heap_size");
	pfnAheadLib_il2cpp_gc_get_used_size = GetAddress("il2cpp_gc_get_used_size");
	pfnAheadLib_il2cpp_gc_is_disabled = GetAddress("il2cpp_gc_is_disabled");
	pfnAheadLib_il2cpp_gc_wbarrier_set_field = GetAddress("il2cpp_gc_wbarrier_set_field");
	pfnAheadLib_il2cpp_gchandle_free = GetAddress("il2cpp_gchandle_free");
	pfnAheadLib_il2cpp_gchandle_get_target = GetAddress("il2cpp_gchandle_get_target");
	pfnAheadLib_il2cpp_gchandle_new = GetAddress("il2cpp_gchandle_new");
	pfnAheadLib_il2cpp_gchandle_new_weakref = GetAddress("il2cpp_gchandle_new_weakref");
	pfnAheadLib_il2cpp_get_corlib = GetAddress("il2cpp_get_corlib");
	pfnAheadLib_il2cpp_get_exception_argument_null = GetAddress("il2cpp_get_exception_argument_null");
	pfnAheadLib_il2cpp_image_get_assembly = GetAddress("il2cpp_image_get_assembly");
	pfnAheadLib_il2cpp_image_get_class = GetAddress("il2cpp_image_get_class");
	pfnAheadLib_il2cpp_image_get_class_count = GetAddress("il2cpp_image_get_class_count");
	pfnAheadLib_il2cpp_image_get_entry_point = GetAddress("il2cpp_image_get_entry_point");
	pfnAheadLib_il2cpp_image_get_filename = GetAddress("il2cpp_image_get_filename");
	pfnAheadLib_il2cpp_image_get_name = GetAddress("il2cpp_image_get_name");
	pfnAheadLib_il2cpp_init = GetAddress("il2cpp_init");
	pfnAheadLib_il2cpp_init_utf16 = GetAddress("il2cpp_init_utf16");
	pfnAheadLib_il2cpp_is_debugger_attached = GetAddress("il2cpp_is_debugger_attached");
	pfnAheadLib_il2cpp_is_vm_thread = GetAddress("il2cpp_is_vm_thread");
	pfnAheadLib_il2cpp_method_get_class = GetAddress("il2cpp_method_get_class");
	pfnAheadLib_il2cpp_method_get_declaring_type = GetAddress("il2cpp_method_get_declaring_type");
	pfnAheadLib_il2cpp_method_get_flags = GetAddress("il2cpp_method_get_flags");
	pfnAheadLib_il2cpp_method_get_from_reflection = GetAddress("il2cpp_method_get_from_reflection");
	pfnAheadLib_il2cpp_method_get_name = GetAddress("il2cpp_method_get_name");
	pfnAheadLib_il2cpp_method_get_object = GetAddress("il2cpp_method_get_object");
	pfnAheadLib_il2cpp_method_get_param = GetAddress("il2cpp_method_get_param");
	pfnAheadLib_il2cpp_method_get_param_count = GetAddress("il2cpp_method_get_param_count");
	pfnAheadLib_il2cpp_method_get_param_name = GetAddress("il2cpp_method_get_param_name");
	pfnAheadLib_il2cpp_method_get_return_type = GetAddress("il2cpp_method_get_return_type");
	pfnAheadLib_il2cpp_method_get_token = GetAddress("il2cpp_method_get_token");
	pfnAheadLib_il2cpp_method_has_attribute = GetAddress("il2cpp_method_has_attribute");
	pfnAheadLib_il2cpp_method_is_generic = GetAddress("il2cpp_method_is_generic");
	pfnAheadLib_il2cpp_method_is_inflated = GetAddress("il2cpp_method_is_inflated");
	pfnAheadLib_il2cpp_method_is_instance = GetAddress("il2cpp_method_is_instance");
	pfnAheadLib_il2cpp_monitor_enter = GetAddress("il2cpp_monitor_enter");
	pfnAheadLib_il2cpp_monitor_exit = GetAddress("il2cpp_monitor_exit");
	pfnAheadLib_il2cpp_monitor_pulse = GetAddress("il2cpp_monitor_pulse");
	pfnAheadLib_il2cpp_monitor_pulse_all = GetAddress("il2cpp_monitor_pulse_all");
	pfnAheadLib_il2cpp_monitor_try_enter = GetAddress("il2cpp_monitor_try_enter");
	pfnAheadLib_il2cpp_monitor_try_wait = GetAddress("il2cpp_monitor_try_wait");
	pfnAheadLib_il2cpp_monitor_wait = GetAddress("il2cpp_monitor_wait");
	pfnAheadLib_il2cpp_object_get_class = GetAddress("il2cpp_object_get_class");
	pfnAheadLib_il2cpp_object_get_size = GetAddress("il2cpp_object_get_size");
	pfnAheadLib_il2cpp_object_get_virtual_method = GetAddress("il2cpp_object_get_virtual_method");
	pfnAheadLib_il2cpp_object_new = GetAddress("il2cpp_object_new");
	pfnAheadLib_il2cpp_object_unbox = GetAddress("il2cpp_object_unbox");
	pfnAheadLib_il2cpp_profiler_install = GetAddress("il2cpp_profiler_install");
	pfnAheadLib_il2cpp_profiler_install_allocation = GetAddress("il2cpp_profiler_install_allocation");
	pfnAheadLib_il2cpp_profiler_install_enter_leave = GetAddress("il2cpp_profiler_install_enter_leave");
	pfnAheadLib_il2cpp_profiler_install_fileio = GetAddress("il2cpp_profiler_install_fileio");
	pfnAheadLib_il2cpp_profiler_install_gc = GetAddress("il2cpp_profiler_install_gc");
	pfnAheadLib_il2cpp_profiler_install_thread = GetAddress("il2cpp_profiler_install_thread");
	pfnAheadLib_il2cpp_profiler_set_events = GetAddress("il2cpp_profiler_set_events");
	pfnAheadLib_il2cpp_property_get_flags = GetAddress("il2cpp_property_get_flags");
	pfnAheadLib_il2cpp_property_get_get_method = GetAddress("il2cpp_property_get_get_method");
	pfnAheadLib_il2cpp_property_get_name = GetAddress("il2cpp_property_get_name");
	pfnAheadLib_il2cpp_property_get_parent = GetAddress("il2cpp_property_get_parent");
	pfnAheadLib_il2cpp_property_get_set_method = GetAddress("il2cpp_property_get_set_method");
	pfnAheadLib_il2cpp_raise_exception = GetAddress("il2cpp_raise_exception");
	pfnAheadLib_il2cpp_register_log_callback = GetAddress("il2cpp_register_log_callback");
	pfnAheadLib_il2cpp_resolve_icall = GetAddress("il2cpp_resolve_icall");
	pfnAheadLib_il2cpp_runtime_class_init = GetAddress("il2cpp_runtime_class_init");
	pfnAheadLib_il2cpp_runtime_invoke = GetAddress("il2cpp_runtime_invoke");
	pfnAheadLib_il2cpp_runtime_invoke_convert_args = GetAddress("il2cpp_runtime_invoke_convert_args");
	pfnAheadLib_il2cpp_runtime_object_init = GetAddress("il2cpp_runtime_object_init");
	pfnAheadLib_il2cpp_runtime_object_init_exception = GetAddress("il2cpp_runtime_object_init_exception");
	pfnAheadLib_il2cpp_runtime_unhandled_exception_policy_set = GetAddress("il2cpp_runtime_unhandled_exception_policy_set");
	pfnAheadLib_il2cpp_set_commandline_arguments = GetAddress("il2cpp_set_commandline_arguments");
	pfnAheadLib_il2cpp_set_commandline_arguments_utf16 = GetAddress("il2cpp_set_commandline_arguments_utf16");
	pfnAheadLib_il2cpp_set_config = GetAddress("il2cpp_set_config");
	pfnAheadLib_il2cpp_set_config_dir = GetAddress("il2cpp_set_config_dir");
	pfnAheadLib_il2cpp_set_config_utf16 = GetAddress("il2cpp_set_config_utf16");
	pfnAheadLib_il2cpp_set_data_dir = GetAddress("il2cpp_set_data_dir");
	pfnAheadLib_il2cpp_set_find_plugin_callback = GetAddress("il2cpp_set_find_plugin_callback");
	pfnAheadLib_il2cpp_set_memory_callbacks = GetAddress("il2cpp_set_memory_callbacks");
	pfnAheadLib_il2cpp_set_temp_dir = GetAddress("il2cpp_set_temp_dir");
	pfnAheadLib_il2cpp_shutdown = GetAddress("il2cpp_shutdown");
	pfnAheadLib_il2cpp_stats_dump_to_file = GetAddress("il2cpp_stats_dump_to_file");
	pfnAheadLib_il2cpp_stats_get_value = GetAddress("il2cpp_stats_get_value");
	pfnAheadLib_il2cpp_string_chars = GetAddress("il2cpp_string_chars");
	pfnAheadLib_il2cpp_string_intern = GetAddress("il2cpp_string_intern");
	pfnAheadLib_il2cpp_string_is_interned = GetAddress("il2cpp_string_is_interned");
	pfnAheadLib_il2cpp_string_length = GetAddress("il2cpp_string_length");
	pfnAheadLib_il2cpp_string_new = GetAddress("il2cpp_string_new");
	pfnAheadLib_il2cpp_string_new_len = GetAddress("il2cpp_string_new_len");
	pfnAheadLib_il2cpp_string_new_utf16 = GetAddress("il2cpp_string_new_utf16");
	pfnAheadLib_il2cpp_string_new_wrapper = GetAddress("il2cpp_string_new_wrapper");
	pfnAheadLib_il2cpp_thread_attach = GetAddress("il2cpp_thread_attach");
	pfnAheadLib_il2cpp_thread_current = GetAddress("il2cpp_thread_current");
	pfnAheadLib_il2cpp_thread_detach = GetAddress("il2cpp_thread_detach");
	pfnAheadLib_il2cpp_thread_get_all_attached_threads = GetAddress("il2cpp_thread_get_all_attached_threads");
	pfnAheadLib_il2cpp_thread_get_frame_at = GetAddress("il2cpp_thread_get_frame_at");
	pfnAheadLib_il2cpp_thread_get_stack_depth = GetAddress("il2cpp_thread_get_stack_depth");
	pfnAheadLib_il2cpp_thread_get_top_frame = GetAddress("il2cpp_thread_get_top_frame");
	pfnAheadLib_il2cpp_thread_walk_frame_stack = GetAddress("il2cpp_thread_walk_frame_stack");
	pfnAheadLib_il2cpp_type_equals = GetAddress("il2cpp_type_equals");
	pfnAheadLib_il2cpp_type_get_assembly_qualified_name = GetAddress("il2cpp_type_get_assembly_qualified_name");
	pfnAheadLib_il2cpp_type_get_attrs = GetAddress("il2cpp_type_get_attrs");
	pfnAheadLib_il2cpp_type_get_class_or_element_class = GetAddress("il2cpp_type_get_class_or_element_class");
	pfnAheadLib_il2cpp_type_get_name = GetAddress("il2cpp_type_get_name");
	pfnAheadLib_il2cpp_type_get_object = GetAddress("il2cpp_type_get_object");
	pfnAheadLib_il2cpp_type_get_type = GetAddress("il2cpp_type_get_type");
	pfnAheadLib_il2cpp_type_is_byref = GetAddress("il2cpp_type_is_byref");
	pfnAheadLib_il2cpp_unhandled_exception = GetAddress("il2cpp_unhandled_exception");
	pfnAheadLib_il2cpp_unity_install_unitytls_interface = GetAddress("il2cpp_unity_install_unitytls_interface");
	pfnAheadLib_il2cpp_unity_liveness_calculation_begin = GetAddress("il2cpp_unity_liveness_calculation_begin");
	pfnAheadLib_il2cpp_unity_liveness_calculation_end = GetAddress("il2cpp_unity_liveness_calculation_end");
	pfnAheadLib_il2cpp_unity_liveness_calculation_from_root = GetAddress("il2cpp_unity_liveness_calculation_from_root");
	pfnAheadLib_il2cpp_unity_liveness_calculation_from_statics = GetAddress("il2cpp_unity_liveness_calculation_from_statics");
	pfnAheadLib_il2cpp_value_box = GetAddress("il2cpp_value_box");
	return TRUE;
}

// 多字节字符版本;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...)  
{  
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
	char* strBuffer = new char[nLen];
	_vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
	va_end(vlArgs);  
	OutputDebugStringA(strBuffer);  
	delete[] strBuffer;
	return;
}

// 入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	// 将模块句柄赋值给全局变量
	g_hInstance = (HINSTANCE)hModule;
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// 禁用DLL_THREAD_ATTACH和DLL_THREAD_DETACH通知
		DisableThreadLibraryCalls(hModule);
		// 初始化并加载原始模块
		if (Load() && Init())
		{
			// 宿主进程名
			TCHAR szAppName[MAX_PATH] = TEXT("RogueMage.exe"); 
			TCHAR szCurName[MAX_PATH];

			// 获取文件路径并分割取到文件名
			GetModuleFileName(NULL, szCurName, MAX_PATH);
			PathStripPath(szCurName);

			// 是否判断宿主进程名
			if (StrCmpI(szCurName, szAppName) == 0)
			{
				// 启动补丁线程或者其他操作
				HANDLE hThread = CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);
				if (hThread)
				{
					CloseHandle(hThread);
				}
			}
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

// 补丁线程及其它操作
DWORD WINAPI ThreadProc(LPVOID lpThreadParameter)
{
	HWND hRogueMageWindow = NULL;
	while(TRUE)
	{
		Sleep(1000);
		hRogueMageWindow = FindWindowA("UnityWndClass", "RogueMage");
		if(!hRogueMageWindow)
		{
			continue;
		}
		break;
	}
	// 判断DLL是否是相同版本
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_OldModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((ULONG64)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((ULONG64)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	if (pIMAGE_FILE_HEADER -> TimeDateStamp != 0x65646454)
	{
		int nRet = MessageBoxA(hRogueMageWindow, "当前游戏模块\"OldGameAssembly.dll\"版本非目标版本! 是否退出游戏?", \
			"可能无法正常运行! 需要将补丁更新至1.0.7版本!并且应该是GOG平台", MB_YESNO | MB_ICONEXCLAMATION);
		if (nRet == IDYES)
		{
			ExitProcess(-1);
		}
	}
	// 创建一个主对话框
	DialogBoxA(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DialogProcMain);
	return 0;
}

// 对话框回调
BOOL CALLBACK DialogProcMain(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	// 用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	switch(uMsg)								
	{
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 界面初始化
			// 初始化对话框图标
			HICON hIcon_SMALL = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON_GOG));
			HICON hIcon_BIG = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON_GOG));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);
			// 默认让RadioButton选择否(取消锁定当前法师能量)
			SendMessage(GetDlgItem(hwndDlg, IDC_RADIO_CurrentManaNo), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			// 将自身切换到最前面
			HMODULE hUser32 = GetModuleHandle("user32.dll");
			void (WINAPI* PSwitchToThisWindow)(HWND, BOOL) = \
				(void (WINAPI* )(HWND, BOOL))GetProcAddress(hUser32, "SwitchToThisWindow");
			PSwitchToThisWindow(hwndDlg, FALSE);
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_CurrentMana:
				{
					// 修改当前魔法值按钮
					int nNumber = 0;
					TCHAR szDATA[0x10] = {0};
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_CurrentMana), WM_GETTEXT, 0x10, (LPARAM)szDATA);
					if (strcmp(szDATA, "") == 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"您必须要输入一个值");
						break;
					}
					// 字符串转数字
					sscanf_s(szDATA, "%d", &nNumber);
					if (nNumber > 999)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"输入的数值不正确(只能是0-999之间)");
						break; 
					}
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_CurrentMana), WM_SETTEXT, 0, (LPARAM)"");
					// 检查是否已经HOOK
					if (g_nAlreadyHookManaFlag == TRUE)
					{
						g_dwManaNumber = nNumber;
						goto todo; 
					}
					g_dwManaNumber = nNumber;
					// 设置钩子
					SetHookMana();
todo:
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_CurrentManaDigit), WM_SETTEXT, 0, (LPARAM)szDATA);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(进入游戏或释放一次魔法生效)");
					break;
				}
			case IDC_BUTTON_MaxMana:
				{
					// 修改最大魔法值按钮
					int nNumber = 0;
					TCHAR szDATA[0x10] = {0};
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_MaxMana), WM_GETTEXT, 0x10, (LPARAM)szDATA);
					if (strcmp(szDATA, "") == 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"您必须要输入一个值");
						break;
					}
					// 字符串转数字
					sscanf_s(szDATA, "%d", &nNumber);
					if (nNumber > 999)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"输入的数值不正确(只能是0-999之间)");
						break; 
					}
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_MaxMana), WM_SETTEXT, 0, (LPARAM)"");
					// 检查是否已经HOOK
					if (g_nAlreadyHookManaFlag == TRUE)
					{
						// 设置最大魔法值
						g_nMaxMana = nNumber;
						goto end; 
					}
					g_nMaxMana = nNumber;
					// 设置钩子
					SetHookMana();
end:
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_MaxManaDigit), WM_SETTEXT, 0, (LPARAM)szDATA);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(进入游戏或释放一次魔法生效)");
					break;
				}
			case IDC_BUTTON_InterfaceMana:
				{
					// 修改基础魔法值
					// 获取要修改指令的地址
					ULONG64 hBaseManaHookAddr = (ULONG64)g_OldModule + 0x656369;
					// 检查是否已经HOOK
					if (*(PUCHAR)hBaseManaHookAddr != 0x89)
					{
						goto unhook;
					}
					// 基础法师能量
					int nNumber = 0;
					TCHAR szDATA[0x10] = {0};
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_InterfaceMana), WM_GETTEXT, 0x10, (LPARAM)szDATA);
					if (strcmp(szDATA, "") == 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"您必须要输入一个值");
						break;
					}
					// 字符串转数字
					sscanf_s(szDATA, "%d", &nNumber);
					if (nNumber > 999)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"输入的数值不正确(只能是0-999之间)");
						break; 
					}
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_InterfaceMana), WM_SETTEXT, 0, (LPARAM)"");
					// 设置钩子
					DWORD dwProtect;
					VirtualProtect((LPVOID)hBaseManaHookAddr, 0x3, PAGE_EXECUTE_READWRITE, &dwProtect);
					*(PUCHAR)hBaseManaHookAddr++ = 0xEB;
					*(PUCHAR)hBaseManaHookAddr++ = 0x59;
					*(PUCHAR)hBaseManaHookAddr = 0x90;
					VirtualProtect((LPVOID)hBaseManaHookAddr, 0x3, dwProtect, &dwProtect);
					// 找到代码偏移并设置代码
					ULONG64 hBaseManaHookCodeAddr = (ULONG64)g_OldModule + 0x6563C4;
					DWORD dwProtectCode;
					VirtualProtect((LPVOID)hBaseManaHookCodeAddr, 0x9, PAGE_EXECUTE_READWRITE, &dwProtectCode);
					*(PUCHAR)hBaseManaHookCodeAddr++ = 0xC7;
					*(PUCHAR)hBaseManaHookCodeAddr++ = 0x43;
					*(PUCHAR)hBaseManaHookCodeAddr++ = 0x58;
					*(LPDWORD)hBaseManaHookCodeAddr = nNumber;
					hBaseManaHookCodeAddr += 4;
					*(PUCHAR)hBaseManaHookCodeAddr++ = 0xEB;
					*(PUCHAR)hBaseManaHookCodeAddr = 0x9F;
					VirtualProtect((LPVOID)hBaseManaHookCodeAddr, 0x9, dwProtectCode, &dwProtectCode);
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_InterfaceManaDigit), WM_SETTEXT, 0, (LPARAM)szDATA);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(基础法师能量)");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_InterfaceMana), WM_SETTEXT, 0, (LPARAM)"关 闭");
					break;
unhook:
					// 取消钩子
					ULONG64 hBaseManaHookAddrRec = (ULONG64)g_OldModule + 0x656369;
					DWORD dwProtectCodeRec;
					VirtualProtect((LPVOID)hBaseManaHookAddrRec, 0x3, PAGE_EXECUTE_READWRITE, &dwProtectCodeRec);
					*(PUCHAR)hBaseManaHookAddrRec++ = OldBaseMana[0];
					*(PUCHAR)hBaseManaHookAddrRec++ = OldBaseMana[1];
					*(PUCHAR)hBaseManaHookAddrRec = OldBaseMana[2];
					VirtualProtect((LPVOID)hBaseManaHookAddrRec, 0x3, dwProtectCodeRec, &dwProtectCodeRec);
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_InterfaceManaDigit), WM_SETTEXT, 0, (LPARAM)"--");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"已关闭基础法师能量修改");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_InterfaceMana), WM_SETTEXT, 0, (LPARAM)"开 启");
					break;
				}
			case IDC_RADIO_CurrentManaYes:
				{
					// 线程安全
					// todo
					// ..
					// ..
					// 锁定当前法师能量
					if (g_nManaHookFlag)
					{
						break;
					}
					// 获取要修改指令的地址
					ulHookBeginManaHold = (ULONG64)g_OldModule + 0xD85EDD;
					DWORD dwProtect;
					// nop掉
					VirtualProtect((LPVOID)ulHookBeginManaHold, 0x2, PAGE_EXECUTE_READWRITE, &dwProtect);
					*(PUCHAR)ulHookBeginManaHold = 0x90;
					*(PUCHAR)(ulHookBeginManaHold + 1) = 0x90;
					VirtualProtect((LPVOID)ulHookBeginManaHold, 0x2, dwProtect, &dwProtect);
					g_nManaHookFlag = TRUE;
					break;
				}
			case IDC_RADIO_CurrentManaNo:
				{
					// 线程安全
					// todo
					// ..
					// ..
					// 取消锁定当前法师能量
					if (!g_nManaHookFlag)
					{
						break;
					}
					// 还原
					DWORD dwProtect;
					VirtualProtect((LPVOID)ulHookBeginManaHold, 0x2, PAGE_EXECUTE_READWRITE, &dwProtect);
					*(PCHAR)ulHookBeginManaHold = OldCodeArrayManaHold[0];
					*(PCHAR)(ulHookBeginManaHold + 1) = OldCodeArrayManaHold[1];
					VirtualProtect((LPVOID)ulHookBeginManaHold, 0x2, dwProtect, &dwProtect);
					g_nManaHookFlag = FALSE;
					break;
				}
			case IDC_BUTTON_Characterlevel:
				{
					// 人物等级
					// 获取要修改指令的地址
					ULONG64 hBaseCharacterLevelHookAddr = (ULONG64)g_OldModule + 0xA4656F;
					// 检查是否已经HOOK
					if (*(PUCHAR)hBaseCharacterLevelHookAddr == 0xE9)
					{
						goto unhookCharacterLevel;
					}
					int nNumber = 0;
					TCHAR szDATA[0x10] = {0};
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_Characterlevel), WM_GETTEXT, 0x10, (LPARAM)szDATA);
					if (strcmp(szDATA, "") == 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"您必须要输入一个值");
						break;
					}
					// 字符串转数字
					sscanf_s(szDATA, "%d", &nNumber);
					if (nNumber > 100 || nNumber < 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"输入的数值不正确(等级在0-100之间)");
						break; 
					}
					// 设置钩子
					DWORD dwProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, PAGE_EXECUTE_READWRITE, &dwProtect);
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0xE9;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x13;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0xC0;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x3D;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x01;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x90;
					*(PUCHAR)hBaseCharacterLevelHookAddr = 0x90;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, dwProtect, &dwProtect);
					// 找到代码偏移并设置代码
					ULONG64 hBaseCharacterLevelHookCodeAddr = (ULONG64)g_OldModule + 0x1E22587;
					DWORD dwCodeProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookCodeAddr, 0x18, PAGE_EXECUTE_READWRITE, &dwCodeProtect);
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x50;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x48;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x83;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC0;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x44;	// 经验值
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC7;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x00;
					*(LPDWORD)hBaseCharacterLevelHookCodeAddr = nNumber;
					hBaseCharacterLevelHookCodeAddr += 4;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x58;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x8B;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x40;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x44;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x48;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x83;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC4;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x28;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xE9;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xD5;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x3F;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC2;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr = 0xFE;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookCodeAddr, 0x18, dwCodeProtect, &dwCodeProtect);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_Characterlevel), WM_SETTEXT, 0, (LPARAM)"");
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_CharacterlevelDigit), WM_SETTEXT, 0, (LPARAM)szDATA);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(人物等级)");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_Characterlevel), WM_SETTEXT, 0, (LPARAM)"关 闭");
					break;
unhookCharacterLevel:
					// 卸载钩子
					DWORD dwUnHookProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, PAGE_EXECUTE_READWRITE, &dwUnHookProtect);
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[0];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[1];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[2];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[3];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[4];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[5];
					*(PUCHAR)hBaseCharacterLevelHookAddr = OldCharacterLevel[6];
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, dwUnHookProtect, &dwUnHookProtect);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_CharacterlevelDigit), WM_SETTEXT, 0, (LPARAM)"--");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(人物等级)");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_Characterlevel), WM_SETTEXT, 0, (LPARAM)"开 启");
					break;
				}
			case IDC_BUTTON_CharacterExperience:
				{
					// 人物经验
					// 获取要修改指令的地址
					ULONG64 hBaseCharacterLevelHookAddr = (ULONG64)g_OldModule + 0xA4656F;
					// 检查是否已经HOOK
					if (*(PUCHAR)hBaseCharacterLevelHookAddr == 0xE9)
					{
						goto unhookCharacterExperience;
					}
					int nNumber = 0;
					TCHAR szDATA[0x10] = {0};
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_CharacterExperience), WM_GETTEXT, 0x10, (LPARAM)szDATA);
					if (strcmp(szDATA, "") == 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"您必须要输入一个值");
						break;
					}
					// 字符串转数字
					sscanf_s(szDATA, "%d", &nNumber);
					if (nNumber > 2500 || nNumber < 0)
					{
						SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"输入的数值不正确(经验值在0-2500之间)");
						break; 
					}
					// 设置钩子
					DWORD dwProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, PAGE_EXECUTE_READWRITE, &dwProtect);
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0xE9;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x13;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0xC0;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x3D;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x01;
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = 0x90;
					*(PUCHAR)hBaseCharacterLevelHookAddr = 0x90;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, dwProtect, &dwProtect);
					// 找到代码偏移并设置代码
					ULONG64 hBaseCharacterLevelHookCodeAddr = (ULONG64)g_OldModule + 0x1E22587;
					DWORD dwCodeProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookCodeAddr, 0x18, PAGE_EXECUTE_READWRITE, &dwCodeProtect);
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x50;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x48;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x83;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC0;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x4C;  // 经验值
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC7;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x00;
					*(LPDWORD)hBaseCharacterLevelHookCodeAddr = nNumber;
					hBaseCharacterLevelHookCodeAddr += 4;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x58;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x8B;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x40;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x44;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x48;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x83;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC4;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x28;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xE9;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xD5;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0x3F;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr++ = 0xC2;
					*(PUCHAR)hBaseCharacterLevelHookCodeAddr = 0xFE;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookCodeAddr, 0x18, dwCodeProtect, &dwCodeProtect);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_CharacterExperience), WM_SETTEXT, 0, (LPARAM)"");
					// 界面提示
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_CharacterExperienceDigit), WM_SETTEXT, 0, (LPARAM)szDATA);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(人物等级)");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_CharacterExperience), WM_SETTEXT, 0, (LPARAM)"关 闭");
					break;
unhookCharacterExperience:
					// 卸载钩子
					DWORD dwUnHookProtect;
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, PAGE_EXECUTE_READWRITE, &dwUnHookProtect);
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[0];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[1];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[2];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[3];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[4];
					*(PUCHAR)hBaseCharacterLevelHookAddr++ = OldCharacterLevel[5];
					*(PUCHAR)hBaseCharacterLevelHookAddr = OldCharacterLevel[6];
					VirtualProtect((LPVOID)hBaseCharacterLevelHookAddr, 0x7, dwUnHookProtect, &dwUnHookProtect);
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_CharacterExperienceDigit), WM_SETTEXT, 0, (LPARAM)"--");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_STATIC_DLG), WM_SETTEXT, 0, (LPARAM)"修改成功(人物等级)");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_BUTTON_CharacterExperience), WM_SETTEXT, 0, (LPARAM)"开 启");
					break;
				}
			}
			return TRUE;
		} 
	}
	return FALSE;
}


// 设置魔法值钩子
VOID WINAPI SetHookMana()
{
	// 线程安全
	// todo
	// ..
	// ..
	// 更改HOOK标志
	g_nAlreadyHookManaFlag = TRUE;
	// 根据偏移得到要HOOK目标地址(E9的地址)
	ULONG64 ulHookBegin = (ULONG64)g_OldModule + 0xD85EC1;
	g_ulManaHookBegin = ulHookBegin;
	// 得到HOOK函数地址(要跳转的地址)
	ULONG64 ulHookFunc = (ULONG64)FuncManaAsm;
	OutputDebugStringFA("1------------------->%llx", ulHookFunc);
	OutputDebugStringFA("2------------------->%llx", ulHookBegin);
	// OutputDebugStringFA("%llx", hOldGameAssembly);
	// 构造硬编码
	// X = 要跳转的地址 - E9的下一行地址 - 5
	DWORD dwOffSet = (DWORD)(ulHookFunc - ulHookBegin - 5);
	// 修改内存属性
	DWORD dwProtect;
	VirtualProtect((LPVOID)ulHookBegin, 0x5, PAGE_EXECUTE_READWRITE, &dwProtect);
	// 设置跳转指令(JMP + 4字节)
	// -------------------------------------------<<指针不需要重新申请
	*(PUCHAR)ulHookBegin++ = 0xE9;
	*(LPDWORD)ulHookBegin = dwOffSet;
	ulHookBegin += 4;
	*(PUCHAR)ulHookBegin++ = 0x90;
	*(PUCHAR)ulHookBegin++ = 0x90;
	// 保存魔法值修改完后应该返回的位置
	g_ulManaHookOver = ulHookBegin;
	VirtualProtect((LPVOID)ulHookBegin, 0x5, dwProtect, &dwProtect);

	return;
}

// 卸载魔法值钩子
VOID WINAPI UnHookMana()
{
	// 线程安全
	// todo
	// ..
	// ..
	// 修改魔法当前值
	*(LPDWORD)(g_ulManaAddress + 0x1c) = g_dwManaNumber;
	// 修改魔法最大值
	*(LPDWORD)(g_ulManaAddress + 0x30) = g_nMaxMana;
	// 是否已经HOOK的标志
	g_nAlreadyHookManaFlag = FALSE;
	// 得到要卸载HOOK的代码起始位置
	ULONG64 ulManaHookBegin = g_ulManaHookBegin;
	DWORD dwProtect;
	VirtualProtect((LPVOID)ulManaHookBegin, 0x7, PAGE_EXECUTE_READWRITE, &dwProtect);
	for (int i = 0; i < 7; i++)
	{
		*(PCHAR)ulManaHookBegin++ = OldCodeArrayMana[i];
	}
	VirtualProtect((LPVOID)ulManaHookBegin, 0x7, dwProtect, &dwProtect);
	return;
}