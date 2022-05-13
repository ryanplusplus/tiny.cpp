__lib_tiny_cpp_path := $(call current_path)

$(call create_lib_with_defaults,tiny_cpp)

tiny_cpp_exported_INC_DIRS += \
  $(__lib_tiny_cpp_path)include \

tiny_cpp_INC_DIRS := \
  $(tiny_cpp_EXTERNAL_INC_DIRS) \
  $(__lib_tiny_cpp_path)include \

tiny_cpp_SYS_INC_DIRS := \
  $(tiny_cpp_EXTERNAL_SYS_INC_DIRS) \

tiny_cpp_SRC_FILES := \

tiny_cpp_SRC_DIRS := \
  $(__lib_tiny_cpp_path)src \

INC_DIRS += \
  $(tiny_cpp_exported_INC_DIRS) \
