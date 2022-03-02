cmake_minimum_required(VERSION 3.0.0)
project(homegear_knx_baos)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/PhysicalInterfaces/Serial.cpp
        src/PhysicalInterfaces/Serial.h
        src/Factory.cpp
        src/Factory.h
        src/GD.cpp
        src/GD.h
        src/Interfaces.cpp
        src/Interfaces.h
        src/Baos.cpp
        src/Baos.h
        src/BaosCentral.cpp
        src/BaosCentral.h
        src/BaosPacket.cpp
        src/BaosPacket.h
        src/BaosPeer.cpp
        src/BaosPeer.h
        src/MakeDeviceDescriptionFile.cpp
        src/MakeDeviceDescriptionFile.h)


# These are the corresponding output paths
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

set(CMAKE_SHARED_LIBRARY_PREFIX "mod_")

include_directories(src/PhysicalInterfaces/baos/kdrive/include)
include_directories(src/PhysicalInterfaces/baos/build_unix/sdk/include)

add_custom_target(homegear COMMAND ../../makeAll.sh SOURCES ${SOURCE_FILES})
add_library(knx_baos SHARED ${SOURCE_FILES})

target_link_libraries (knx_baos "-lkdriveBAOS")

install(TARGETS knx_baos PERMISSIONS OWNER_EXECUTE OWNER_READ GROUP_READ GROUP_EXECUTE DESTINATION /var/lib/homegear/modules)