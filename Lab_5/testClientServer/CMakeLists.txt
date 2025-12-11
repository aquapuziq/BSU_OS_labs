cmake_minimum_required (VERSION 3.8)

project ("testClientServer")

include_directories("C:\\dev\\projects\\msVS\\libraryBoost\\boost_1_90_0")


add_executable(testClientServer "testEmployee.cpp" "testEmployee" "../ClientServerHead/employee.h" "../ClientServerHead/employee.cpp" "../Server/Server.cpp" "testClientServer.cpp")


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET testClientServer PROPERTY CXX_STANDARD 20)
endif()