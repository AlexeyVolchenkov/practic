cmake_minimum_required (VERSION 3.12)
project(mosquitto)

# Initialize CMake library for Kaspersky OS SDK.
include (platform)
initialize_platform ()

# Connect artifact installation functions.
include (platform/install)

# Toolchain for work with NK-parser.
include (platform/nk)

# Build edl files
nk_build_edl_files (subscriber_edl_files
                    NK_MODULE "mosquitto" EDL
                    "resources/edl/Subscriber.edl")

nk_build_edl_files (multi_vfs_mosquitto_vfs_ram_fs
                    NK_MODULE "mosquitto"
                    EDL "resources/edl/VfsRamFs.edl")

nk_build_edl_files (multi_vfs_mosquitto_vfs_sdcard_fs
                    NK_MODULE "mosquitto"
                    EDL "resources/edl/VfsSdcardFs.edl")

nk_build_edl_files (multi_vfs_mosquitto_vfs_net
                    NK_MODULE "mosquitto"
                    EDL "resources/edl/VfsNet.edl")

nk_build_edl_files (multi_vfs_mosquitto_env
                    NK_MODULE "mosquitto"
                    EDL "resources/edl/Env.edl")

# Attach a package with mosquitto component
find_package (mosquitto REQUIRED COMPONENTS CXX_LIB)
include_directories (${mosquitto_INCLUDE})

# Attach a package with the virtual file system implementation.
find_package (vfs REQUIRED)
include_directories (${vfs_INCLUDE})

# Attach a package with the dhcpcd entity implementation.
find_package (rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories (${rump_INCLUDE})

# Attach a package with the ramdisk implementation.
find_package (ramdisk REQUIRED)
include_directories (${ramdisk_INCLUDE})

# Attach a package with the Ext4 file system implementation.
find_package (LWEXT4 REQUIRED)
include_directories (${LWEXT4_INCLUDE})

# Attach a package with the sdcard suppport implementation.
find_package (sdcard REQUIRED)
include_directories (${sdcard_INCLUDE})

# Attach a package with the FAT file system implementation.
find_package (VFAT REQUIRED)
include_directories (${VFAT_INCLUDE})

# Attach a package with the DNET file system implementation.
find_package (dnet REQUIRED COMPONENTS IMPLEMENTATION_LIB)

# Choose the storage devices for the configuration files.
# If none of the option is selected, the ramfs device is used.
option(ROOTFS_SDCARD "Use SD card to mount root filesystem" OFF)
message(STATUS "Use SD card to mount root filesystem - ${ROOTFS_SDCARD}")

# Build configuration header files from a configuration file template
configure_file(resources/include/config.h.in config.h)

# Attach the directory with the generated config header files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Attach the directories with entities.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/resources/include)

add_subdirectory (subscriber)
add_subdirectory (custom_vfs)
add_subdirectory (env)

# Attach the directory with the Einit entity to create a solution image
# based on Kaspersky OS.
add_subdirectory (einit)
