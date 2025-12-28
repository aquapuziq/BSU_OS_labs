cmake_minimum_required (VERSION 3.8)

project ("testClientServer")

include_directories("C:\\dev\\projects\\msVS\\libraryBoost\\boost_1_90_0")


add_executable(testClientServer "testEmployee.cpp" "testReadWrite.cpp" "../ClientServerHead/employee.h" "../ClientServerHead/employee.cpp" "../Server/Server.cpp" "testClientServer.cpp")


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET testClientServer PROPERTY CXX_STANDARD 20)
endif()

set(BOOST_ROOT "C:/dev/projects/msVS/libraryBoost/boost_1_90_0")
set(Boost_INCLUDE_DIR "C:/dev/projects/msVS/libraryBoost/boost_1_90_0")

set(BOOST_LIBRARYDIR "C:/dev/projects/msVS/libraryBoost/boost_1_90_0/stage/lib")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

enable_testing()
add_test(NAME MyBoostTests COMMAND testClientServer)
