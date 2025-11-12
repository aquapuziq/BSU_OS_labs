cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("receiver")

add_executable (receiver "receiver.cpp" "receiver.h")

add_dependencies(receiver sender)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET receiver PROPERTY CXX_STANDARD 20)
endif()

