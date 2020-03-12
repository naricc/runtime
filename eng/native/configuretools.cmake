if(NOT CLR_CMAKE_CONFIGURE_PLATFORM_INCLUDED)
  message(FATAL_ERROR "configuretools.cmake needs to be included after configureplatform.cmake")
endif()

# Get the version of the compiler that is in the file name for tool location.
set (CLR_CMAKE_COMPILER_FILE_NAME_VERSION "")
if (CMAKE_C_COMPILER MATCHES "-?[0-9]+(\.[0-9]+)?$")
  set(CLR_CMAKE_COMPILER_FILE_NAME_VERSION "${CMAKE_MATCH_0}")
endif()

if(NOT WIN32)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(APPLE)
      set(TOOLSET_PREFIX "")
    else()
      set(TOOLSET_PREFIX "llvm-")
    endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CROSSCOMPILING)
      set(TOOLSET_PREFIX "${CMAKE_CXX_COMPILER_TARGET}-")
    else()
      set(TOOLSET_PREFIX "")
    endif()
  endif()

  function(locate_toolchain_exec exec var)
    string(TOUPPER ${exec} EXEC_UPPERCASE)
    if(NOT "$ENV{CLR_${EXEC_UPPERCASE}}" STREQUAL "")
      set(${var} "$ENV{CLR_${EXEC_UPPERCASE}}" PARENT_SCOPE)
      return()
    endif()

    find_program(EXEC_LOCATION_${exec}
      NAMES
      "${TOOLSET_PREFIX}${exec}${CLR_CMAKE_COMPILER_FILE_NAME_VERSION}"
      "${TOOLSET_PREFIX}${exec}")

    if (EXEC_LOCATION_${exec} STREQUAL "EXEC_LOCATION_${exec}-NOTFOUND")
      message(FATAL_ERROR "Unable to find toolchain executable for: ${exec}.")
    endif()
    set(${var} ${EXEC_LOCATION_${exec}} PARENT_SCOPE)
  endfunction()

  locate_toolchain_exec(ar CMAKE_AR)
  locate_toolchain_exec(link CMAKE_LINKER)
  locate_toolchain_exec(nm CMAKE_NM)

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    locate_toolchain_exec(ranlib CMAKE_RANLIB)
  endif()

  if(NOT CLR_CMAKE_TARGET_DARWIN AND NOT CLR_CMAKE_TARGET_IOS)
    locate_toolchain_exec(objdump CMAKE_OBJDUMP)

    if(CMAKE_CROSSCOMPILING AND NOT DEFINED CLR_CROSS_COMPONENTS_BUILD AND (CMAKE_SYSTEM_PROCESSOR STREQUAL armv7l OR
       CMAKE_SYSTEM_PROCESSOR STREQUAL aarch64 OR CMAKE_SYSTEM_PROCESSOR STREQUAL arm))
      set(TOOLSET_PREFIX "${TOOLCHAIN}-")
    else()
      set(TOOLSET_PREFIX "")
    endif()

    locate_toolchain_exec(objcopy CMAKE_OBJCOPY)
  endif()
endif()
