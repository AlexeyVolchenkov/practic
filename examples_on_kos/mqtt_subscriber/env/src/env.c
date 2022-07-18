#include <env/env.h>
#include <stdio.h>
#include <stdlib.h>
#include <vfs/rumpfs_client.h>
#include <vfs/vfs.h>

/* Add an automatically generated config file. */
#include "config.h"

#define NET_VFS_CONNECTION       "VfsNet"
#define RAM_FS_VFS_CONNECTION    "VfsRamFs"
#define SDCARD_FS_VFS_CONNECTION "VfsSdcardFs"

#define WHITE "\033[1;37m"
#define NC    "\033[0m"

int main(int argc, char** argv)
{
    /*FIXME: VFS with _VFS_NETWORK_BACKEND can't print any messages and the printf will not work!
     * And in the future, it needs to replace all printf to fprintf(stderr) and OutString/printk in rump and etc.
    */

#if defined(ROOTFS_SDCARD)
    const char* RootFsDevice = "sdcard";
#else
    const char* RootFsDevice = "ramfs";
#endif

    const char* VfsNetArgs[] = {
        "-l", "devfs /dev devfs 0"
    };
    const char* VfsNetEnvs[] = {
        _VFS_NETWORK_BACKEND"=server:"NET_VFS_CONNECTION,
        _VFS_FILESYSTEM_BACKEND"=server:"NET_VFS_CONNECTION,
    };
    ENV_REGISTER_PROGRAM_ENVIRONMENT("VfsNet", VfsNetArgs, VfsNetEnvs);

    const char* VfsRamFsArgs[] = {
    };
    const char* VfsRamFsEnvs[] = {
        "ROOTFS=ramdisk0,0 / ext4 0",
        _VFS_FILESYSTEM_BACKEND"=server:"RAM_FS_VFS_CONNECTION,
    };
    ENV_REGISTER_PROGRAM_ENVIRONMENT("VfsRamFs", VfsRamFsArgs, VfsRamFsEnvs);

    const char* VfsSdcardFsArgs[] = {
        "-l", "nodev /tmp ramfs 0",
        "-l", "nodev /var ramfs 0"
    };
    const char* VfsSdcardFsEnvs[] = {
        "ROOTFS=mmc0,0 / fat32 0",
        _VFS_FILESYSTEM_BACKEND"=server:"SDCARD_FS_VFS_CONNECTION,
    };
    ENV_REGISTER_PROGRAM_ENVIRONMENT("VfsSdcardFs", VfsSdcardFsArgs, VfsSdcardFsEnvs);

    const char *subscriberEnvs[] = {
        _VFS_FILESYSTEM_BACKEND "=client:" SDCARD_FS_VFS_CONNECTION,
        _VFS_NETWORK_BACKEND "=client:" NET_VFS_CONNECTION,
        "MQTT_BROKER_ADDRESS=10.0.2.2",
        "MQTT_BROKER_PORT=1883"};
    ENV_REGISTER_VARS("Subscriber", subscriberEnvs);

    const char* DhcpcdEnvs[] = {
#if defined(ROOTFS_SDCARD) && defined(__arm__)
        _VFS_FILESYSTEM_BACKEND"="RUMPFS_CLIENT_FS_BACKEND":"SDCARD_FS_VFS_CONNECTION,
#else
        _VFS_FILESYSTEM_BACKEND"="RUMPFS_CLIENT_FS_BACKEND":"RAM_FS_VFS_CONNECTION,
#endif
        _VFS_NETWORK_BACKEND"=client:"NET_VFS_CONNECTION,
    };
    const char* DhcpcdArgs[] = {
        "-4",
        "-f", "/etc/dhcpcd.conf"
    };
    ENV_REGISTER_PROGRAM_ENVIRONMENT("Dhcpcd", DhcpcdArgs, DhcpcdEnvs);

    const char* NtpdEnvs[] = {
#if defined(ROOTFS_SDCARD) && defined(__arm__)
        _VFS_FILESYSTEM_BACKEND"=client:"SDCARD_FS_VFS_CONNECTION,
#else
        _VFS_FILESYSTEM_BACKEND"=client:"RAM_FS_VFS_CONNECTION,
#endif
        _VFS_NETWORK_BACKEND"=client:"NET_VFS_CONNECTION,
    };
    const char* NtpdArgs[] = {
        "-4",
        "-c", "/etc/ntp.conf"
    };
    ENV_REGISTER_PROGRAM_ENVIRONMENT("Ntpd", NtpdArgs, NtpdEnvs);

    fprintf(stderr,
            "[ENV] Services use "
            WHITE "%s" NC " device\r\n",
            RootFsDevice);

    envServerRun();

    return EXIT_SUCCESS;
}
