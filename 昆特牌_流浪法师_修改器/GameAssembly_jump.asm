; 把 .asm 文件添加到工程一次
; 右键单击文件-属性-常规-
; 项类型:自定义生成工具
; 从生成中排除:否

; 然后复制下面命令填入
; 命令行: ml64 /Fo $(IntDir)%(fileName).obj /c /Cp %(fileName).asm
; 输出: $(IntDir)%(fileName).obj;%(Outputs)
; 链接对象: 是

.DATA
EXTERN pfnAheadLib_CloseZStream:dq;
EXTERN pfnAheadLib_CreateZStream:dq;
EXTERN pfnAheadLib_Flush:dq;
EXTERN pfnAheadLib_ReadZStream:dq;
EXTERN pfnAheadLib_UnityPalGetLocalTimeZoneData:dq;
EXTERN pfnAheadLib_UnityPalGetTimeZoneDataForID:dq;
EXTERN pfnAheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs:dq;
EXTERN pfnAheadLib_UseUnityPalForTimeZoneInformation:dq;
EXTERN pfnAheadLib_WriteZStream:dq;
EXTERN pfnAheadLib_il2cpp_add_internal_call:dq;
EXTERN pfnAheadLib_il2cpp_alloc:dq;
EXTERN pfnAheadLib_il2cpp_array_class_get:dq;
EXTERN pfnAheadLib_il2cpp_array_element_size:dq;
EXTERN pfnAheadLib_il2cpp_array_get_byte_length:dq;
EXTERN pfnAheadLib_il2cpp_array_length:dq;
EXTERN pfnAheadLib_il2cpp_array_new:dq;
EXTERN pfnAheadLib_il2cpp_array_new_full:dq;
EXTERN pfnAheadLib_il2cpp_array_new_specific:dq;
EXTERN pfnAheadLib_il2cpp_assembly_get_image:dq;
EXTERN pfnAheadLib_il2cpp_bounded_array_class_get:dq;
EXTERN pfnAheadLib_il2cpp_capture_memory_snapshot:dq;
EXTERN pfnAheadLib_il2cpp_class_array_element_size:dq;
EXTERN pfnAheadLib_il2cpp_class_enum_basetype:dq;
EXTERN pfnAheadLib_il2cpp_class_from_il2cpp_type:dq;
EXTERN pfnAheadLib_il2cpp_class_from_name:dq;
EXTERN pfnAheadLib_il2cpp_class_from_system_type:dq;
EXTERN pfnAheadLib_il2cpp_class_from_type:dq;
EXTERN pfnAheadLib_il2cpp_class_get_assemblyname:dq;
EXTERN pfnAheadLib_il2cpp_class_get_bitmap:dq;
EXTERN pfnAheadLib_il2cpp_class_get_bitmap_size:dq;
EXTERN pfnAheadLib_il2cpp_class_get_declaring_type:dq;
EXTERN pfnAheadLib_il2cpp_class_get_element_class:dq;
EXTERN pfnAheadLib_il2cpp_class_get_events:dq;
EXTERN pfnAheadLib_il2cpp_class_get_field_from_name:dq;
EXTERN pfnAheadLib_il2cpp_class_get_fields:dq;
EXTERN pfnAheadLib_il2cpp_class_get_flags:dq;
EXTERN pfnAheadLib_il2cpp_class_get_image:dq;
EXTERN pfnAheadLib_il2cpp_class_get_interfaces:dq;
EXTERN pfnAheadLib_il2cpp_class_get_method_from_name:dq;
EXTERN pfnAheadLib_il2cpp_class_get_methods:dq;
EXTERN pfnAheadLib_il2cpp_class_get_name:dq;
EXTERN pfnAheadLib_il2cpp_class_get_namespace:dq;
EXTERN pfnAheadLib_il2cpp_class_get_nested_types:dq;
EXTERN pfnAheadLib_il2cpp_class_get_parent:dq;
EXTERN pfnAheadLib_il2cpp_class_get_properties:dq;
EXTERN pfnAheadLib_il2cpp_class_get_property_from_name:dq;
EXTERN pfnAheadLib_il2cpp_class_get_rank:dq;
EXTERN pfnAheadLib_il2cpp_class_get_type:dq;
EXTERN pfnAheadLib_il2cpp_class_get_type_token:dq;
EXTERN pfnAheadLib_il2cpp_class_has_attribute:dq;
EXTERN pfnAheadLib_il2cpp_class_has_parent:dq;
EXTERN pfnAheadLib_il2cpp_class_has_references:dq;
EXTERN pfnAheadLib_il2cpp_class_instance_size:dq;
EXTERN pfnAheadLib_il2cpp_class_is_abstract:dq;
EXTERN pfnAheadLib_il2cpp_class_is_assignable_from:dq;
EXTERN pfnAheadLib_il2cpp_class_is_blittable:dq;
EXTERN pfnAheadLib_il2cpp_class_is_enum:dq;
EXTERN pfnAheadLib_il2cpp_class_is_generic:dq;
EXTERN pfnAheadLib_il2cpp_class_is_inflated:dq;
EXTERN pfnAheadLib_il2cpp_class_is_interface:dq;
EXTERN pfnAheadLib_il2cpp_class_is_subclass_of:dq;
EXTERN pfnAheadLib_il2cpp_class_is_valuetype:dq;
EXTERN pfnAheadLib_il2cpp_class_num_fields:dq;
EXTERN pfnAheadLib_il2cpp_class_value_size:dq;
EXTERN pfnAheadLib_il2cpp_current_thread_get_frame_at:dq;
EXTERN pfnAheadLib_il2cpp_current_thread_get_stack_depth:dq;
EXTERN pfnAheadLib_il2cpp_current_thread_get_top_frame:dq;
EXTERN pfnAheadLib_il2cpp_current_thread_walk_frame_stack:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_construct:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_free:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_from_class:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_from_method:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_get_attr:dq;
EXTERN pfnAheadLib_il2cpp_custom_attrs_has_attr:dq;
EXTERN pfnAheadLib_il2cpp_debugger_set_agent_options:dq;
EXTERN pfnAheadLib_il2cpp_domain_assembly_open:dq;
EXTERN pfnAheadLib_il2cpp_domain_get:dq;
EXTERN pfnAheadLib_il2cpp_domain_get_assemblies:dq;
EXTERN pfnAheadLib_il2cpp_exception_from_name_msg:dq;
EXTERN pfnAheadLib_il2cpp_field_get_flags:dq;
EXTERN pfnAheadLib_il2cpp_field_get_name:dq;
EXTERN pfnAheadLib_il2cpp_field_get_offset:dq;
EXTERN pfnAheadLib_il2cpp_field_get_parent:dq;
EXTERN pfnAheadLib_il2cpp_field_get_type:dq;
EXTERN pfnAheadLib_il2cpp_field_get_value:dq;
EXTERN pfnAheadLib_il2cpp_field_get_value_object:dq;
EXTERN pfnAheadLib_il2cpp_field_has_attribute:dq;
EXTERN pfnAheadLib_il2cpp_field_set_value:dq;
EXTERN pfnAheadLib_il2cpp_field_set_value_object:dq;
EXTERN pfnAheadLib_il2cpp_field_static_get_value:dq;
EXTERN pfnAheadLib_il2cpp_field_static_set_value:dq;
EXTERN pfnAheadLib_il2cpp_format_exception:dq;
EXTERN pfnAheadLib_il2cpp_format_stack_trace:dq;
EXTERN pfnAheadLib_il2cpp_free:dq;
EXTERN pfnAheadLib_il2cpp_free_captured_memory_snapshot:dq;
EXTERN pfnAheadLib_il2cpp_gc_collect:dq;
EXTERN pfnAheadLib_il2cpp_gc_collect_a_little:dq;
EXTERN pfnAheadLib_il2cpp_gc_disable:dq;
EXTERN pfnAheadLib_il2cpp_gc_enable:dq;
EXTERN pfnAheadLib_il2cpp_gc_get_heap_size:dq;
EXTERN pfnAheadLib_il2cpp_gc_get_used_size:dq;
EXTERN pfnAheadLib_il2cpp_gc_is_disabled:dq;
EXTERN pfnAheadLib_il2cpp_gc_wbarrier_set_field:dq;
EXTERN pfnAheadLib_il2cpp_gchandle_free:dq;
EXTERN pfnAheadLib_il2cpp_gchandle_get_target:dq;
EXTERN pfnAheadLib_il2cpp_gchandle_new:dq;
EXTERN pfnAheadLib_il2cpp_gchandle_new_weakref:dq;
EXTERN pfnAheadLib_il2cpp_get_corlib:dq;
EXTERN pfnAheadLib_il2cpp_get_exception_argument_null:dq;
EXTERN pfnAheadLib_il2cpp_image_get_assembly:dq;
EXTERN pfnAheadLib_il2cpp_image_get_class:dq;
EXTERN pfnAheadLib_il2cpp_image_get_class_count:dq;
EXTERN pfnAheadLib_il2cpp_image_get_entry_point:dq;
EXTERN pfnAheadLib_il2cpp_image_get_filename:dq;
EXTERN pfnAheadLib_il2cpp_image_get_name:dq;
EXTERN pfnAheadLib_il2cpp_init:dq;
EXTERN pfnAheadLib_il2cpp_init_utf16:dq;
EXTERN pfnAheadLib_il2cpp_is_debugger_attached:dq;
EXTERN pfnAheadLib_il2cpp_is_vm_thread:dq;
EXTERN pfnAheadLib_il2cpp_method_get_class:dq;
EXTERN pfnAheadLib_il2cpp_method_get_declaring_type:dq;
EXTERN pfnAheadLib_il2cpp_method_get_flags:dq;
EXTERN pfnAheadLib_il2cpp_method_get_from_reflection:dq;
EXTERN pfnAheadLib_il2cpp_method_get_name:dq;
EXTERN pfnAheadLib_il2cpp_method_get_object:dq;
EXTERN pfnAheadLib_il2cpp_method_get_param:dq;
EXTERN pfnAheadLib_il2cpp_method_get_param_count:dq;
EXTERN pfnAheadLib_il2cpp_method_get_param_name:dq;
EXTERN pfnAheadLib_il2cpp_method_get_return_type:dq;
EXTERN pfnAheadLib_il2cpp_method_get_token:dq;
EXTERN pfnAheadLib_il2cpp_method_has_attribute:dq;
EXTERN pfnAheadLib_il2cpp_method_is_generic:dq;
EXTERN pfnAheadLib_il2cpp_method_is_inflated:dq;
EXTERN pfnAheadLib_il2cpp_method_is_instance:dq;
EXTERN pfnAheadLib_il2cpp_monitor_enter:dq;
EXTERN pfnAheadLib_il2cpp_monitor_exit:dq;
EXTERN pfnAheadLib_il2cpp_monitor_pulse:dq;
EXTERN pfnAheadLib_il2cpp_monitor_pulse_all:dq;
EXTERN pfnAheadLib_il2cpp_monitor_try_enter:dq;
EXTERN pfnAheadLib_il2cpp_monitor_try_wait:dq;
EXTERN pfnAheadLib_il2cpp_monitor_wait:dq;
EXTERN pfnAheadLib_il2cpp_object_get_class:dq;
EXTERN pfnAheadLib_il2cpp_object_get_size:dq;
EXTERN pfnAheadLib_il2cpp_object_get_virtual_method:dq;
EXTERN pfnAheadLib_il2cpp_object_new:dq;
EXTERN pfnAheadLib_il2cpp_object_unbox:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install_allocation:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install_enter_leave:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install_fileio:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install_gc:dq;
EXTERN pfnAheadLib_il2cpp_profiler_install_thread:dq;
EXTERN pfnAheadLib_il2cpp_profiler_set_events:dq;
EXTERN pfnAheadLib_il2cpp_property_get_flags:dq;
EXTERN pfnAheadLib_il2cpp_property_get_get_method:dq;
EXTERN pfnAheadLib_il2cpp_property_get_name:dq;
EXTERN pfnAheadLib_il2cpp_property_get_parent:dq;
EXTERN pfnAheadLib_il2cpp_property_get_set_method:dq;
EXTERN pfnAheadLib_il2cpp_raise_exception:dq;
EXTERN pfnAheadLib_il2cpp_register_log_callback:dq;
EXTERN pfnAheadLib_il2cpp_resolve_icall:dq;
EXTERN pfnAheadLib_il2cpp_runtime_class_init:dq;
EXTERN pfnAheadLib_il2cpp_runtime_invoke:dq;
EXTERN pfnAheadLib_il2cpp_runtime_invoke_convert_args:dq;
EXTERN pfnAheadLib_il2cpp_runtime_object_init:dq;
EXTERN pfnAheadLib_il2cpp_runtime_object_init_exception:dq;
EXTERN pfnAheadLib_il2cpp_runtime_unhandled_exception_policy_set:dq;
EXTERN pfnAheadLib_il2cpp_set_commandline_arguments:dq;
EXTERN pfnAheadLib_il2cpp_set_commandline_arguments_utf16:dq;
EXTERN pfnAheadLib_il2cpp_set_config:dq;
EXTERN pfnAheadLib_il2cpp_set_config_dir:dq;
EXTERN pfnAheadLib_il2cpp_set_config_utf16:dq;
EXTERN pfnAheadLib_il2cpp_set_data_dir:dq;
EXTERN pfnAheadLib_il2cpp_set_find_plugin_callback:dq;
EXTERN pfnAheadLib_il2cpp_set_memory_callbacks:dq;
EXTERN pfnAheadLib_il2cpp_set_temp_dir:dq;
EXTERN pfnAheadLib_il2cpp_shutdown:dq;
EXTERN pfnAheadLib_il2cpp_stats_dump_to_file:dq;
EXTERN pfnAheadLib_il2cpp_stats_get_value:dq;
EXTERN pfnAheadLib_il2cpp_string_chars:dq;
EXTERN pfnAheadLib_il2cpp_string_intern:dq;
EXTERN pfnAheadLib_il2cpp_string_is_interned:dq;
EXTERN pfnAheadLib_il2cpp_string_length:dq;
EXTERN pfnAheadLib_il2cpp_string_new:dq;
EXTERN pfnAheadLib_il2cpp_string_new_len:dq;
EXTERN pfnAheadLib_il2cpp_string_new_utf16:dq;
EXTERN pfnAheadLib_il2cpp_string_new_wrapper:dq;
EXTERN pfnAheadLib_il2cpp_thread_attach:dq;
EXTERN pfnAheadLib_il2cpp_thread_current:dq;
EXTERN pfnAheadLib_il2cpp_thread_detach:dq;
EXTERN pfnAheadLib_il2cpp_thread_get_all_attached_threads:dq;
EXTERN pfnAheadLib_il2cpp_thread_get_frame_at:dq;
EXTERN pfnAheadLib_il2cpp_thread_get_stack_depth:dq;
EXTERN pfnAheadLib_il2cpp_thread_get_top_frame:dq;
EXTERN pfnAheadLib_il2cpp_thread_walk_frame_stack:dq;
EXTERN pfnAheadLib_il2cpp_type_equals:dq;
EXTERN pfnAheadLib_il2cpp_type_get_assembly_qualified_name:dq;
EXTERN pfnAheadLib_il2cpp_type_get_attrs:dq;
EXTERN pfnAheadLib_il2cpp_type_get_class_or_element_class:dq;
EXTERN pfnAheadLib_il2cpp_type_get_name:dq;
EXTERN pfnAheadLib_il2cpp_type_get_object:dq;
EXTERN pfnAheadLib_il2cpp_type_get_type:dq;
EXTERN pfnAheadLib_il2cpp_type_is_byref:dq;
EXTERN pfnAheadLib_il2cpp_unhandled_exception:dq;
EXTERN pfnAheadLib_il2cpp_unity_install_unitytls_interface:dq;
EXTERN pfnAheadLib_il2cpp_unity_liveness_calculation_begin:dq;
EXTERN pfnAheadLib_il2cpp_unity_liveness_calculation_end:dq;
EXTERN pfnAheadLib_il2cpp_unity_liveness_calculation_from_root:dq;
EXTERN pfnAheadLib_il2cpp_unity_liveness_calculation_from_statics:dq;
EXTERN pfnAheadLib_il2cpp_value_box:dq;

; 魔法值修改完后应该返回的位置
EXTERN g_ulManaHookOver:dq;
; 本次要修改的魔法值数值
EXTERN g_ulManaAddress:dq;
; 卸载钩子函数
EXTERN UnHookMana:proc



.CODE
AheadLib_CloseZStream PROC
	jmp pfnAheadLib_CloseZStream
AheadLib_CloseZStream ENDP

AheadLib_CreateZStream PROC
	jmp pfnAheadLib_CreateZStream
AheadLib_CreateZStream ENDP

AheadLib_Flush PROC
	jmp pfnAheadLib_Flush
AheadLib_Flush ENDP

AheadLib_ReadZStream PROC
	jmp pfnAheadLib_ReadZStream
AheadLib_ReadZStream ENDP

AheadLib_UnityPalGetLocalTimeZoneData PROC
	jmp pfnAheadLib_UnityPalGetLocalTimeZoneData
AheadLib_UnityPalGetLocalTimeZoneData ENDP

AheadLib_UnityPalGetTimeZoneDataForID PROC
	jmp pfnAheadLib_UnityPalGetTimeZoneDataForID
AheadLib_UnityPalGetTimeZoneDataForID ENDP

AheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs PROC
	jmp pfnAheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs
AheadLib_UnityPalTimeZoneInfoGetTimeZoneIDs ENDP

AheadLib_UseUnityPalForTimeZoneInformation PROC
	jmp pfnAheadLib_UseUnityPalForTimeZoneInformation
AheadLib_UseUnityPalForTimeZoneInformation ENDP

AheadLib_WriteZStream PROC
	jmp pfnAheadLib_WriteZStream
AheadLib_WriteZStream ENDP

AheadLib_il2cpp_add_internal_call PROC
	jmp pfnAheadLib_il2cpp_add_internal_call
AheadLib_il2cpp_add_internal_call ENDP

AheadLib_il2cpp_alloc PROC
	jmp pfnAheadLib_il2cpp_alloc
AheadLib_il2cpp_alloc ENDP

AheadLib_il2cpp_array_class_get PROC
	jmp pfnAheadLib_il2cpp_array_class_get
AheadLib_il2cpp_array_class_get ENDP

AheadLib_il2cpp_array_element_size PROC
	jmp pfnAheadLib_il2cpp_array_element_size
AheadLib_il2cpp_array_element_size ENDP

AheadLib_il2cpp_array_get_byte_length PROC
	jmp pfnAheadLib_il2cpp_array_get_byte_length
AheadLib_il2cpp_array_get_byte_length ENDP

AheadLib_il2cpp_array_length PROC
	jmp pfnAheadLib_il2cpp_array_length
AheadLib_il2cpp_array_length ENDP

AheadLib_il2cpp_array_new PROC
	jmp pfnAheadLib_il2cpp_array_new
AheadLib_il2cpp_array_new ENDP

AheadLib_il2cpp_array_new_full PROC
	jmp pfnAheadLib_il2cpp_array_new_full
AheadLib_il2cpp_array_new_full ENDP

AheadLib_il2cpp_array_new_specific PROC
	jmp pfnAheadLib_il2cpp_array_new_specific
AheadLib_il2cpp_array_new_specific ENDP

AheadLib_il2cpp_assembly_get_image PROC
	jmp pfnAheadLib_il2cpp_assembly_get_image
AheadLib_il2cpp_assembly_get_image ENDP

AheadLib_il2cpp_bounded_array_class_get PROC
	jmp pfnAheadLib_il2cpp_bounded_array_class_get
AheadLib_il2cpp_bounded_array_class_get ENDP

AheadLib_il2cpp_capture_memory_snapshot PROC
	jmp pfnAheadLib_il2cpp_capture_memory_snapshot
AheadLib_il2cpp_capture_memory_snapshot ENDP

AheadLib_il2cpp_class_array_element_size PROC
	jmp pfnAheadLib_il2cpp_class_array_element_size
AheadLib_il2cpp_class_array_element_size ENDP

AheadLib_il2cpp_class_enum_basetype PROC
	jmp pfnAheadLib_il2cpp_class_enum_basetype
AheadLib_il2cpp_class_enum_basetype ENDP

AheadLib_il2cpp_class_from_il2cpp_type PROC
	jmp pfnAheadLib_il2cpp_class_from_il2cpp_type
AheadLib_il2cpp_class_from_il2cpp_type ENDP

AheadLib_il2cpp_class_from_name PROC
	jmp pfnAheadLib_il2cpp_class_from_name
AheadLib_il2cpp_class_from_name ENDP

AheadLib_il2cpp_class_from_system_type PROC
	jmp pfnAheadLib_il2cpp_class_from_system_type
AheadLib_il2cpp_class_from_system_type ENDP

AheadLib_il2cpp_class_from_type PROC
	jmp pfnAheadLib_il2cpp_class_from_type
AheadLib_il2cpp_class_from_type ENDP

AheadLib_il2cpp_class_get_assemblyname PROC
	jmp pfnAheadLib_il2cpp_class_get_assemblyname
AheadLib_il2cpp_class_get_assemblyname ENDP

AheadLib_il2cpp_class_get_bitmap PROC
	jmp pfnAheadLib_il2cpp_class_get_bitmap
AheadLib_il2cpp_class_get_bitmap ENDP

AheadLib_il2cpp_class_get_bitmap_size PROC
	jmp pfnAheadLib_il2cpp_class_get_bitmap_size
AheadLib_il2cpp_class_get_bitmap_size ENDP

AheadLib_il2cpp_class_get_declaring_type PROC
	jmp pfnAheadLib_il2cpp_class_get_declaring_type
AheadLib_il2cpp_class_get_declaring_type ENDP

AheadLib_il2cpp_class_get_element_class PROC
	jmp pfnAheadLib_il2cpp_class_get_element_class
AheadLib_il2cpp_class_get_element_class ENDP

AheadLib_il2cpp_class_get_events PROC
	jmp pfnAheadLib_il2cpp_class_get_events
AheadLib_il2cpp_class_get_events ENDP

AheadLib_il2cpp_class_get_field_from_name PROC
	jmp pfnAheadLib_il2cpp_class_get_field_from_name
AheadLib_il2cpp_class_get_field_from_name ENDP

AheadLib_il2cpp_class_get_fields PROC
	jmp pfnAheadLib_il2cpp_class_get_fields
AheadLib_il2cpp_class_get_fields ENDP

AheadLib_il2cpp_class_get_flags PROC
	jmp pfnAheadLib_il2cpp_class_get_flags
AheadLib_il2cpp_class_get_flags ENDP

AheadLib_il2cpp_class_get_image PROC
	jmp pfnAheadLib_il2cpp_class_get_image
AheadLib_il2cpp_class_get_image ENDP

AheadLib_il2cpp_class_get_interfaces PROC
	jmp pfnAheadLib_il2cpp_class_get_interfaces
AheadLib_il2cpp_class_get_interfaces ENDP

AheadLib_il2cpp_class_get_method_from_name PROC
	jmp pfnAheadLib_il2cpp_class_get_method_from_name
AheadLib_il2cpp_class_get_method_from_name ENDP

AheadLib_il2cpp_class_get_methods PROC
	jmp pfnAheadLib_il2cpp_class_get_methods
AheadLib_il2cpp_class_get_methods ENDP

AheadLib_il2cpp_class_get_name PROC
	jmp pfnAheadLib_il2cpp_class_get_name
AheadLib_il2cpp_class_get_name ENDP

AheadLib_il2cpp_class_get_namespace PROC
	jmp pfnAheadLib_il2cpp_class_get_namespace
AheadLib_il2cpp_class_get_namespace ENDP

AheadLib_il2cpp_class_get_nested_types PROC
	jmp pfnAheadLib_il2cpp_class_get_nested_types
AheadLib_il2cpp_class_get_nested_types ENDP

AheadLib_il2cpp_class_get_parent PROC
	jmp pfnAheadLib_il2cpp_class_get_parent
AheadLib_il2cpp_class_get_parent ENDP

AheadLib_il2cpp_class_get_properties PROC
	jmp pfnAheadLib_il2cpp_class_get_properties
AheadLib_il2cpp_class_get_properties ENDP

AheadLib_il2cpp_class_get_property_from_name PROC
	jmp pfnAheadLib_il2cpp_class_get_property_from_name
AheadLib_il2cpp_class_get_property_from_name ENDP

AheadLib_il2cpp_class_get_rank PROC
	jmp pfnAheadLib_il2cpp_class_get_rank
AheadLib_il2cpp_class_get_rank ENDP

AheadLib_il2cpp_class_get_type PROC
	jmp pfnAheadLib_il2cpp_class_get_type
AheadLib_il2cpp_class_get_type ENDP

AheadLib_il2cpp_class_get_type_token PROC
	jmp pfnAheadLib_il2cpp_class_get_type_token
AheadLib_il2cpp_class_get_type_token ENDP

AheadLib_il2cpp_class_has_attribute PROC
	jmp pfnAheadLib_il2cpp_class_has_attribute
AheadLib_il2cpp_class_has_attribute ENDP

AheadLib_il2cpp_class_has_parent PROC
	jmp pfnAheadLib_il2cpp_class_has_parent
AheadLib_il2cpp_class_has_parent ENDP

AheadLib_il2cpp_class_has_references PROC
	jmp pfnAheadLib_il2cpp_class_has_references
AheadLib_il2cpp_class_has_references ENDP

AheadLib_il2cpp_class_instance_size PROC
	jmp pfnAheadLib_il2cpp_class_instance_size
AheadLib_il2cpp_class_instance_size ENDP

AheadLib_il2cpp_class_is_abstract PROC
	jmp pfnAheadLib_il2cpp_class_is_abstract
AheadLib_il2cpp_class_is_abstract ENDP

AheadLib_il2cpp_class_is_assignable_from PROC
	jmp pfnAheadLib_il2cpp_class_is_assignable_from
AheadLib_il2cpp_class_is_assignable_from ENDP

AheadLib_il2cpp_class_is_blittable PROC
	jmp pfnAheadLib_il2cpp_class_is_blittable
AheadLib_il2cpp_class_is_blittable ENDP

AheadLib_il2cpp_class_is_enum PROC
	jmp pfnAheadLib_il2cpp_class_is_enum
AheadLib_il2cpp_class_is_enum ENDP

AheadLib_il2cpp_class_is_generic PROC
	jmp pfnAheadLib_il2cpp_class_is_generic
AheadLib_il2cpp_class_is_generic ENDP

AheadLib_il2cpp_class_is_inflated PROC
	jmp pfnAheadLib_il2cpp_class_is_inflated
AheadLib_il2cpp_class_is_inflated ENDP

AheadLib_il2cpp_class_is_interface PROC
	jmp pfnAheadLib_il2cpp_class_is_interface
AheadLib_il2cpp_class_is_interface ENDP

AheadLib_il2cpp_class_is_subclass_of PROC
	jmp pfnAheadLib_il2cpp_class_is_subclass_of
AheadLib_il2cpp_class_is_subclass_of ENDP

AheadLib_il2cpp_class_is_valuetype PROC
	jmp pfnAheadLib_il2cpp_class_is_valuetype
AheadLib_il2cpp_class_is_valuetype ENDP

AheadLib_il2cpp_class_num_fields PROC
	jmp pfnAheadLib_il2cpp_class_num_fields
AheadLib_il2cpp_class_num_fields ENDP

AheadLib_il2cpp_class_value_size PROC
	jmp pfnAheadLib_il2cpp_class_value_size
AheadLib_il2cpp_class_value_size ENDP

AheadLib_il2cpp_current_thread_get_frame_at PROC
	jmp pfnAheadLib_il2cpp_current_thread_get_frame_at
AheadLib_il2cpp_current_thread_get_frame_at ENDP

AheadLib_il2cpp_current_thread_get_stack_depth PROC
	jmp pfnAheadLib_il2cpp_current_thread_get_stack_depth
AheadLib_il2cpp_current_thread_get_stack_depth ENDP

AheadLib_il2cpp_current_thread_get_top_frame PROC
	jmp pfnAheadLib_il2cpp_current_thread_get_top_frame
AheadLib_il2cpp_current_thread_get_top_frame ENDP

AheadLib_il2cpp_current_thread_walk_frame_stack PROC
	jmp pfnAheadLib_il2cpp_current_thread_walk_frame_stack
AheadLib_il2cpp_current_thread_walk_frame_stack ENDP

AheadLib_il2cpp_custom_attrs_construct PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_construct
AheadLib_il2cpp_custom_attrs_construct ENDP

AheadLib_il2cpp_custom_attrs_free PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_free
AheadLib_il2cpp_custom_attrs_free ENDP

AheadLib_il2cpp_custom_attrs_from_class PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_from_class
AheadLib_il2cpp_custom_attrs_from_class ENDP

AheadLib_il2cpp_custom_attrs_from_method PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_from_method
AheadLib_il2cpp_custom_attrs_from_method ENDP

AheadLib_il2cpp_custom_attrs_get_attr PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_get_attr
AheadLib_il2cpp_custom_attrs_get_attr ENDP

AheadLib_il2cpp_custom_attrs_has_attr PROC
	jmp pfnAheadLib_il2cpp_custom_attrs_has_attr
AheadLib_il2cpp_custom_attrs_has_attr ENDP

AheadLib_il2cpp_debugger_set_agent_options PROC
	jmp pfnAheadLib_il2cpp_debugger_set_agent_options
AheadLib_il2cpp_debugger_set_agent_options ENDP

AheadLib_il2cpp_domain_assembly_open PROC
	jmp pfnAheadLib_il2cpp_domain_assembly_open
AheadLib_il2cpp_domain_assembly_open ENDP

AheadLib_il2cpp_domain_get PROC
	jmp pfnAheadLib_il2cpp_domain_get
AheadLib_il2cpp_domain_get ENDP

AheadLib_il2cpp_domain_get_assemblies PROC
	jmp pfnAheadLib_il2cpp_domain_get_assemblies
AheadLib_il2cpp_domain_get_assemblies ENDP

AheadLib_il2cpp_exception_from_name_msg PROC
	jmp pfnAheadLib_il2cpp_exception_from_name_msg
AheadLib_il2cpp_exception_from_name_msg ENDP

AheadLib_il2cpp_field_get_flags PROC
	jmp pfnAheadLib_il2cpp_field_get_flags
AheadLib_il2cpp_field_get_flags ENDP

AheadLib_il2cpp_field_get_name PROC
	jmp pfnAheadLib_il2cpp_field_get_name
AheadLib_il2cpp_field_get_name ENDP

AheadLib_il2cpp_field_get_offset PROC
	jmp pfnAheadLib_il2cpp_field_get_offset
AheadLib_il2cpp_field_get_offset ENDP

AheadLib_il2cpp_field_get_parent PROC
	jmp pfnAheadLib_il2cpp_field_get_parent
AheadLib_il2cpp_field_get_parent ENDP

AheadLib_il2cpp_field_get_type PROC
	jmp pfnAheadLib_il2cpp_field_get_type
AheadLib_il2cpp_field_get_type ENDP

AheadLib_il2cpp_field_get_value PROC
	jmp pfnAheadLib_il2cpp_field_get_value
AheadLib_il2cpp_field_get_value ENDP

AheadLib_il2cpp_field_get_value_object PROC
	jmp pfnAheadLib_il2cpp_field_get_value_object
AheadLib_il2cpp_field_get_value_object ENDP

AheadLib_il2cpp_field_has_attribute PROC
	jmp pfnAheadLib_il2cpp_field_has_attribute
AheadLib_il2cpp_field_has_attribute ENDP

AheadLib_il2cpp_field_set_value PROC
	jmp pfnAheadLib_il2cpp_field_set_value
AheadLib_il2cpp_field_set_value ENDP

AheadLib_il2cpp_field_set_value_object PROC
	jmp pfnAheadLib_il2cpp_field_set_value_object
AheadLib_il2cpp_field_set_value_object ENDP

AheadLib_il2cpp_field_static_get_value PROC
	jmp pfnAheadLib_il2cpp_field_static_get_value
AheadLib_il2cpp_field_static_get_value ENDP

AheadLib_il2cpp_field_static_set_value PROC
	jmp pfnAheadLib_il2cpp_field_static_set_value
AheadLib_il2cpp_field_static_set_value ENDP

AheadLib_il2cpp_format_exception PROC
	jmp pfnAheadLib_il2cpp_format_exception
AheadLib_il2cpp_format_exception ENDP

AheadLib_il2cpp_format_stack_trace PROC
	jmp pfnAheadLib_il2cpp_format_stack_trace
AheadLib_il2cpp_format_stack_trace ENDP

AheadLib_il2cpp_free PROC
	jmp pfnAheadLib_il2cpp_free
AheadLib_il2cpp_free ENDP

AheadLib_il2cpp_free_captured_memory_snapshot PROC
	jmp pfnAheadLib_il2cpp_free_captured_memory_snapshot
AheadLib_il2cpp_free_captured_memory_snapshot ENDP

AheadLib_il2cpp_gc_collect PROC
	jmp pfnAheadLib_il2cpp_gc_collect
AheadLib_il2cpp_gc_collect ENDP

AheadLib_il2cpp_gc_collect_a_little PROC
	jmp pfnAheadLib_il2cpp_gc_collect_a_little
AheadLib_il2cpp_gc_collect_a_little ENDP

AheadLib_il2cpp_gc_disable PROC
	jmp pfnAheadLib_il2cpp_gc_disable
AheadLib_il2cpp_gc_disable ENDP

AheadLib_il2cpp_gc_enable PROC
	jmp pfnAheadLib_il2cpp_gc_enable
AheadLib_il2cpp_gc_enable ENDP

AheadLib_il2cpp_gc_get_heap_size PROC
	jmp pfnAheadLib_il2cpp_gc_get_heap_size
AheadLib_il2cpp_gc_get_heap_size ENDP

AheadLib_il2cpp_gc_get_used_size PROC
	jmp pfnAheadLib_il2cpp_gc_get_used_size
AheadLib_il2cpp_gc_get_used_size ENDP

AheadLib_il2cpp_gc_is_disabled PROC
	jmp pfnAheadLib_il2cpp_gc_is_disabled
AheadLib_il2cpp_gc_is_disabled ENDP

AheadLib_il2cpp_gc_wbarrier_set_field PROC
	jmp pfnAheadLib_il2cpp_gc_wbarrier_set_field
AheadLib_il2cpp_gc_wbarrier_set_field ENDP

AheadLib_il2cpp_gchandle_free PROC
	jmp pfnAheadLib_il2cpp_gchandle_free
AheadLib_il2cpp_gchandle_free ENDP

AheadLib_il2cpp_gchandle_get_target PROC
	jmp pfnAheadLib_il2cpp_gchandle_get_target
AheadLib_il2cpp_gchandle_get_target ENDP

AheadLib_il2cpp_gchandle_new PROC
	jmp pfnAheadLib_il2cpp_gchandle_new
AheadLib_il2cpp_gchandle_new ENDP

AheadLib_il2cpp_gchandle_new_weakref PROC
	jmp pfnAheadLib_il2cpp_gchandle_new_weakref
AheadLib_il2cpp_gchandle_new_weakref ENDP

AheadLib_il2cpp_get_corlib PROC
	jmp pfnAheadLib_il2cpp_get_corlib
AheadLib_il2cpp_get_corlib ENDP

AheadLib_il2cpp_get_exception_argument_null PROC
	jmp pfnAheadLib_il2cpp_get_exception_argument_null
AheadLib_il2cpp_get_exception_argument_null ENDP

AheadLib_il2cpp_image_get_assembly PROC
	jmp pfnAheadLib_il2cpp_image_get_assembly
AheadLib_il2cpp_image_get_assembly ENDP

AheadLib_il2cpp_image_get_class PROC
	jmp pfnAheadLib_il2cpp_image_get_class
AheadLib_il2cpp_image_get_class ENDP

AheadLib_il2cpp_image_get_class_count PROC
	jmp pfnAheadLib_il2cpp_image_get_class_count
AheadLib_il2cpp_image_get_class_count ENDP

AheadLib_il2cpp_image_get_entry_point PROC
	jmp pfnAheadLib_il2cpp_image_get_entry_point
AheadLib_il2cpp_image_get_entry_point ENDP

AheadLib_il2cpp_image_get_filename PROC
	jmp pfnAheadLib_il2cpp_image_get_filename
AheadLib_il2cpp_image_get_filename ENDP

AheadLib_il2cpp_image_get_name PROC
	jmp pfnAheadLib_il2cpp_image_get_name
AheadLib_il2cpp_image_get_name ENDP

AheadLib_il2cpp_init PROC
	jmp pfnAheadLib_il2cpp_init
AheadLib_il2cpp_init ENDP

AheadLib_il2cpp_init_utf16 PROC
	jmp pfnAheadLib_il2cpp_init_utf16
AheadLib_il2cpp_init_utf16 ENDP

AheadLib_il2cpp_is_debugger_attached PROC
	jmp pfnAheadLib_il2cpp_is_debugger_attached
AheadLib_il2cpp_is_debugger_attached ENDP

AheadLib_il2cpp_is_vm_thread PROC
	jmp pfnAheadLib_il2cpp_is_vm_thread
AheadLib_il2cpp_is_vm_thread ENDP

AheadLib_il2cpp_method_get_class PROC
	jmp pfnAheadLib_il2cpp_method_get_class
AheadLib_il2cpp_method_get_class ENDP

AheadLib_il2cpp_method_get_declaring_type PROC
	jmp pfnAheadLib_il2cpp_method_get_declaring_type
AheadLib_il2cpp_method_get_declaring_type ENDP

AheadLib_il2cpp_method_get_flags PROC
	jmp pfnAheadLib_il2cpp_method_get_flags
AheadLib_il2cpp_method_get_flags ENDP

AheadLib_il2cpp_method_get_from_reflection PROC
	jmp pfnAheadLib_il2cpp_method_get_from_reflection
AheadLib_il2cpp_method_get_from_reflection ENDP

AheadLib_il2cpp_method_get_name PROC
	jmp pfnAheadLib_il2cpp_method_get_name
AheadLib_il2cpp_method_get_name ENDP

AheadLib_il2cpp_method_get_object PROC
	jmp pfnAheadLib_il2cpp_method_get_object
AheadLib_il2cpp_method_get_object ENDP

AheadLib_il2cpp_method_get_param PROC
	jmp pfnAheadLib_il2cpp_method_get_param
AheadLib_il2cpp_method_get_param ENDP

AheadLib_il2cpp_method_get_param_count PROC
	jmp pfnAheadLib_il2cpp_method_get_param_count
AheadLib_il2cpp_method_get_param_count ENDP

AheadLib_il2cpp_method_get_param_name PROC
	jmp pfnAheadLib_il2cpp_method_get_param_name
AheadLib_il2cpp_method_get_param_name ENDP

AheadLib_il2cpp_method_get_return_type PROC
	jmp pfnAheadLib_il2cpp_method_get_return_type
AheadLib_il2cpp_method_get_return_type ENDP

AheadLib_il2cpp_method_get_token PROC
	jmp pfnAheadLib_il2cpp_method_get_token
AheadLib_il2cpp_method_get_token ENDP

AheadLib_il2cpp_method_has_attribute PROC
	jmp pfnAheadLib_il2cpp_method_has_attribute
AheadLib_il2cpp_method_has_attribute ENDP

AheadLib_il2cpp_method_is_generic PROC
	jmp pfnAheadLib_il2cpp_method_is_generic
AheadLib_il2cpp_method_is_generic ENDP

AheadLib_il2cpp_method_is_inflated PROC
	jmp pfnAheadLib_il2cpp_method_is_inflated
AheadLib_il2cpp_method_is_inflated ENDP

AheadLib_il2cpp_method_is_instance PROC
	jmp pfnAheadLib_il2cpp_method_is_instance
AheadLib_il2cpp_method_is_instance ENDP

AheadLib_il2cpp_monitor_enter PROC
	jmp pfnAheadLib_il2cpp_monitor_enter
AheadLib_il2cpp_monitor_enter ENDP

AheadLib_il2cpp_monitor_exit PROC
	jmp pfnAheadLib_il2cpp_monitor_exit
AheadLib_il2cpp_monitor_exit ENDP

AheadLib_il2cpp_monitor_pulse PROC
	jmp pfnAheadLib_il2cpp_monitor_pulse
AheadLib_il2cpp_monitor_pulse ENDP

AheadLib_il2cpp_monitor_pulse_all PROC
	jmp pfnAheadLib_il2cpp_monitor_pulse_all
AheadLib_il2cpp_monitor_pulse_all ENDP

AheadLib_il2cpp_monitor_try_enter PROC
	jmp pfnAheadLib_il2cpp_monitor_try_enter
AheadLib_il2cpp_monitor_try_enter ENDP

AheadLib_il2cpp_monitor_try_wait PROC
	jmp pfnAheadLib_il2cpp_monitor_try_wait
AheadLib_il2cpp_monitor_try_wait ENDP

AheadLib_il2cpp_monitor_wait PROC
	jmp pfnAheadLib_il2cpp_monitor_wait
AheadLib_il2cpp_monitor_wait ENDP

AheadLib_il2cpp_object_get_class PROC
	jmp pfnAheadLib_il2cpp_object_get_class
AheadLib_il2cpp_object_get_class ENDP

AheadLib_il2cpp_object_get_size PROC
	jmp pfnAheadLib_il2cpp_object_get_size
AheadLib_il2cpp_object_get_size ENDP

AheadLib_il2cpp_object_get_virtual_method PROC
	jmp pfnAheadLib_il2cpp_object_get_virtual_method
AheadLib_il2cpp_object_get_virtual_method ENDP

AheadLib_il2cpp_object_new PROC
	jmp pfnAheadLib_il2cpp_object_new
AheadLib_il2cpp_object_new ENDP

AheadLib_il2cpp_object_unbox PROC
	jmp pfnAheadLib_il2cpp_object_unbox
AheadLib_il2cpp_object_unbox ENDP

AheadLib_il2cpp_profiler_install PROC
	jmp pfnAheadLib_il2cpp_profiler_install
AheadLib_il2cpp_profiler_install ENDP

AheadLib_il2cpp_profiler_install_allocation PROC
	jmp pfnAheadLib_il2cpp_profiler_install_allocation
AheadLib_il2cpp_profiler_install_allocation ENDP

AheadLib_il2cpp_profiler_install_enter_leave PROC
	jmp pfnAheadLib_il2cpp_profiler_install_enter_leave
AheadLib_il2cpp_profiler_install_enter_leave ENDP

AheadLib_il2cpp_profiler_install_fileio PROC
	jmp pfnAheadLib_il2cpp_profiler_install_fileio
AheadLib_il2cpp_profiler_install_fileio ENDP

AheadLib_il2cpp_profiler_install_gc PROC
	jmp pfnAheadLib_il2cpp_profiler_install_gc
AheadLib_il2cpp_profiler_install_gc ENDP

AheadLib_il2cpp_profiler_install_thread PROC
	jmp pfnAheadLib_il2cpp_profiler_install_thread
AheadLib_il2cpp_profiler_install_thread ENDP

AheadLib_il2cpp_profiler_set_events PROC
	jmp pfnAheadLib_il2cpp_profiler_set_events
AheadLib_il2cpp_profiler_set_events ENDP

AheadLib_il2cpp_property_get_flags PROC
	jmp pfnAheadLib_il2cpp_property_get_flags
AheadLib_il2cpp_property_get_flags ENDP

AheadLib_il2cpp_property_get_get_method PROC
	jmp pfnAheadLib_il2cpp_property_get_get_method
AheadLib_il2cpp_property_get_get_method ENDP

AheadLib_il2cpp_property_get_name PROC
	jmp pfnAheadLib_il2cpp_property_get_name
AheadLib_il2cpp_property_get_name ENDP

AheadLib_il2cpp_property_get_parent PROC
	jmp pfnAheadLib_il2cpp_property_get_parent
AheadLib_il2cpp_property_get_parent ENDP

AheadLib_il2cpp_property_get_set_method PROC
	jmp pfnAheadLib_il2cpp_property_get_set_method
AheadLib_il2cpp_property_get_set_method ENDP

AheadLib_il2cpp_raise_exception PROC
	jmp pfnAheadLib_il2cpp_raise_exception
AheadLib_il2cpp_raise_exception ENDP

AheadLib_il2cpp_register_log_callback PROC
	jmp pfnAheadLib_il2cpp_register_log_callback
AheadLib_il2cpp_register_log_callback ENDP

AheadLib_il2cpp_resolve_icall PROC
	jmp pfnAheadLib_il2cpp_resolve_icall
AheadLib_il2cpp_resolve_icall ENDP

AheadLib_il2cpp_runtime_class_init PROC
	jmp pfnAheadLib_il2cpp_runtime_class_init
AheadLib_il2cpp_runtime_class_init ENDP

AheadLib_il2cpp_runtime_invoke PROC
	jmp pfnAheadLib_il2cpp_runtime_invoke
AheadLib_il2cpp_runtime_invoke ENDP

AheadLib_il2cpp_runtime_invoke_convert_args PROC
	jmp pfnAheadLib_il2cpp_runtime_invoke_convert_args
AheadLib_il2cpp_runtime_invoke_convert_args ENDP

AheadLib_il2cpp_runtime_object_init PROC
	jmp pfnAheadLib_il2cpp_runtime_object_init
AheadLib_il2cpp_runtime_object_init ENDP

AheadLib_il2cpp_runtime_object_init_exception PROC
	jmp pfnAheadLib_il2cpp_runtime_object_init_exception
AheadLib_il2cpp_runtime_object_init_exception ENDP

AheadLib_il2cpp_runtime_unhandled_exception_policy_set PROC
	jmp pfnAheadLib_il2cpp_runtime_unhandled_exception_policy_set
AheadLib_il2cpp_runtime_unhandled_exception_policy_set ENDP

AheadLib_il2cpp_set_commandline_arguments PROC
	jmp pfnAheadLib_il2cpp_set_commandline_arguments
AheadLib_il2cpp_set_commandline_arguments ENDP

AheadLib_il2cpp_set_commandline_arguments_utf16 PROC
	jmp pfnAheadLib_il2cpp_set_commandline_arguments_utf16
AheadLib_il2cpp_set_commandline_arguments_utf16 ENDP

AheadLib_il2cpp_set_config PROC
	jmp pfnAheadLib_il2cpp_set_config
AheadLib_il2cpp_set_config ENDP

AheadLib_il2cpp_set_config_dir PROC
	jmp pfnAheadLib_il2cpp_set_config_dir
AheadLib_il2cpp_set_config_dir ENDP

AheadLib_il2cpp_set_config_utf16 PROC
	jmp pfnAheadLib_il2cpp_set_config_utf16
AheadLib_il2cpp_set_config_utf16 ENDP

AheadLib_il2cpp_set_data_dir PROC
	jmp pfnAheadLib_il2cpp_set_data_dir
AheadLib_il2cpp_set_data_dir ENDP

AheadLib_il2cpp_set_find_plugin_callback PROC
	jmp pfnAheadLib_il2cpp_set_find_plugin_callback
AheadLib_il2cpp_set_find_plugin_callback ENDP

AheadLib_il2cpp_set_memory_callbacks PROC
	jmp pfnAheadLib_il2cpp_set_memory_callbacks
AheadLib_il2cpp_set_memory_callbacks ENDP

AheadLib_il2cpp_set_temp_dir PROC
	jmp pfnAheadLib_il2cpp_set_temp_dir
AheadLib_il2cpp_set_temp_dir ENDP

AheadLib_il2cpp_shutdown PROC
	jmp pfnAheadLib_il2cpp_shutdown
AheadLib_il2cpp_shutdown ENDP

AheadLib_il2cpp_stats_dump_to_file PROC
	jmp pfnAheadLib_il2cpp_stats_dump_to_file
AheadLib_il2cpp_stats_dump_to_file ENDP

AheadLib_il2cpp_stats_get_value PROC
	jmp pfnAheadLib_il2cpp_stats_get_value
AheadLib_il2cpp_stats_get_value ENDP

AheadLib_il2cpp_string_chars PROC
	jmp pfnAheadLib_il2cpp_string_chars
AheadLib_il2cpp_string_chars ENDP

AheadLib_il2cpp_string_intern PROC
	jmp pfnAheadLib_il2cpp_string_intern
AheadLib_il2cpp_string_intern ENDP

AheadLib_il2cpp_string_is_interned PROC
	jmp pfnAheadLib_il2cpp_string_is_interned
AheadLib_il2cpp_string_is_interned ENDP

AheadLib_il2cpp_string_length PROC
	jmp pfnAheadLib_il2cpp_string_length
AheadLib_il2cpp_string_length ENDP

AheadLib_il2cpp_string_new PROC
	jmp pfnAheadLib_il2cpp_string_new
AheadLib_il2cpp_string_new ENDP

AheadLib_il2cpp_string_new_len PROC
	jmp pfnAheadLib_il2cpp_string_new_len
AheadLib_il2cpp_string_new_len ENDP

AheadLib_il2cpp_string_new_utf16 PROC
	jmp pfnAheadLib_il2cpp_string_new_utf16
AheadLib_il2cpp_string_new_utf16 ENDP

AheadLib_il2cpp_string_new_wrapper PROC
	jmp pfnAheadLib_il2cpp_string_new_wrapper
AheadLib_il2cpp_string_new_wrapper ENDP

AheadLib_il2cpp_thread_attach PROC
	jmp pfnAheadLib_il2cpp_thread_attach
AheadLib_il2cpp_thread_attach ENDP

AheadLib_il2cpp_thread_current PROC
	jmp pfnAheadLib_il2cpp_thread_current
AheadLib_il2cpp_thread_current ENDP

AheadLib_il2cpp_thread_detach PROC
	jmp pfnAheadLib_il2cpp_thread_detach
AheadLib_il2cpp_thread_detach ENDP

AheadLib_il2cpp_thread_get_all_attached_threads PROC
	jmp pfnAheadLib_il2cpp_thread_get_all_attached_threads
AheadLib_il2cpp_thread_get_all_attached_threads ENDP

AheadLib_il2cpp_thread_get_frame_at PROC
	jmp pfnAheadLib_il2cpp_thread_get_frame_at
AheadLib_il2cpp_thread_get_frame_at ENDP

AheadLib_il2cpp_thread_get_stack_depth PROC
	jmp pfnAheadLib_il2cpp_thread_get_stack_depth
AheadLib_il2cpp_thread_get_stack_depth ENDP

AheadLib_il2cpp_thread_get_top_frame PROC
	jmp pfnAheadLib_il2cpp_thread_get_top_frame
AheadLib_il2cpp_thread_get_top_frame ENDP

AheadLib_il2cpp_thread_walk_frame_stack PROC
	jmp pfnAheadLib_il2cpp_thread_walk_frame_stack
AheadLib_il2cpp_thread_walk_frame_stack ENDP

AheadLib_il2cpp_type_equals PROC
	jmp pfnAheadLib_il2cpp_type_equals
AheadLib_il2cpp_type_equals ENDP

AheadLib_il2cpp_type_get_assembly_qualified_name PROC
	jmp pfnAheadLib_il2cpp_type_get_assembly_qualified_name
AheadLib_il2cpp_type_get_assembly_qualified_name ENDP

AheadLib_il2cpp_type_get_attrs PROC
	jmp pfnAheadLib_il2cpp_type_get_attrs
AheadLib_il2cpp_type_get_attrs ENDP

AheadLib_il2cpp_type_get_class_or_element_class PROC
	jmp pfnAheadLib_il2cpp_type_get_class_or_element_class
AheadLib_il2cpp_type_get_class_or_element_class ENDP

AheadLib_il2cpp_type_get_name PROC
	jmp pfnAheadLib_il2cpp_type_get_name
AheadLib_il2cpp_type_get_name ENDP

AheadLib_il2cpp_type_get_object PROC
	jmp pfnAheadLib_il2cpp_type_get_object
AheadLib_il2cpp_type_get_object ENDP

AheadLib_il2cpp_type_get_type PROC
	jmp pfnAheadLib_il2cpp_type_get_type
AheadLib_il2cpp_type_get_type ENDP

AheadLib_il2cpp_type_is_byref PROC
	jmp pfnAheadLib_il2cpp_type_is_byref
AheadLib_il2cpp_type_is_byref ENDP

AheadLib_il2cpp_unhandled_exception PROC
	jmp pfnAheadLib_il2cpp_unhandled_exception
AheadLib_il2cpp_unhandled_exception ENDP

AheadLib_il2cpp_unity_install_unitytls_interface PROC
	jmp pfnAheadLib_il2cpp_unity_install_unitytls_interface
AheadLib_il2cpp_unity_install_unitytls_interface ENDP

AheadLib_il2cpp_unity_liveness_calculation_begin PROC
	jmp pfnAheadLib_il2cpp_unity_liveness_calculation_begin
AheadLib_il2cpp_unity_liveness_calculation_begin ENDP

AheadLib_il2cpp_unity_liveness_calculation_end PROC
	jmp pfnAheadLib_il2cpp_unity_liveness_calculation_end
AheadLib_il2cpp_unity_liveness_calculation_end ENDP

AheadLib_il2cpp_unity_liveness_calculation_from_root PROC
	jmp pfnAheadLib_il2cpp_unity_liveness_calculation_from_root
AheadLib_il2cpp_unity_liveness_calculation_from_root ENDP

AheadLib_il2cpp_unity_liveness_calculation_from_statics PROC
	jmp pfnAheadLib_il2cpp_unity_liveness_calculation_from_statics
AheadLib_il2cpp_unity_liveness_calculation_from_statics ENDP

AheadLib_il2cpp_value_box PROC
	jmp pfnAheadLib_il2cpp_value_box
AheadLib_il2cpp_value_box ENDP

FuncManaAsm PROC
	; 压入要返回的地址
	PUSH g_ulManaHookOver
    ; 保存所有寄存器
    PUSH RAX
    PUSH RCX
    PUSH RDX
    PUSH RBX
    PUSH RSI
    PUSH RDI
    PUSH R8
    PUSH R9
    PUSH R10
    PUSH R11
    PUSH R12
    PUSH R13
    PUSH R14
    PUSH R15
	PUSHF
	;
	;MOV EAX, g_dwManaNumber
	;ADD RBX, 1CH
	;MOV DWORD PTR [RBX], EAX
	; 保存地址出去
	MOV g_ulManaAddress, RBX
	; CALL功能函数
	CALL UnHookMana
	; 恢复寄存器
	POPF 
    POP R15
    POP R14
    POP R13
    POP R12
    POP R11
    POP R10
    POP R9
    POP R8
    POP RDI
    POP RSI
    POP RBX
    POP RDX
    POP RCX
    POP RAX
	; 执行覆盖的代码
	MOV R14D, [RBX + 1CH]
	MOV ECX, R14D
    RET
FuncManaAsm ENDP

END