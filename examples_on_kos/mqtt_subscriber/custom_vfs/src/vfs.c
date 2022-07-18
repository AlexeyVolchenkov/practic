#include <coresrv/fs/fs_api.h>
#include <coresrv/handle/handle_api.h>
#include <services/fs/FS.idl.h>

#define _VFS_SERVER_INTERFACE
#include <vfs/basic.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>

static const char LogPrefix[] = "[VFS]";

static _vfs_server_thread_routine_t threadRoutine;
static void *threadRoutineArg;

void _vfs_server_set_main_thread_routine(
    _vfs_server_thread_routine_t func, void *arg)
{
    threadRoutine = func;
    threadRoutineArg = arg;
}

static bool processCfgLine(char *line)
{
    char *devName = strtok(line, " ");

    if (devName == NULL || strlen(devName) == 0)
    {
        fprintf(stderr, "%s Failed to read devName\n", LogPrefix);
        return false;
    }

    char *dirName = strtok(NULL, " ");

    if (dirName == NULL || strlen(dirName) == 0)
    {
        fprintf(stderr, "%s Failed to read dirName\n", LogPrefix);
        return false;
    }

    char *fsName = strtok(NULL, " ");

    if (fsName == NULL || strlen(fsName) == 0)
    {
        fprintf(stderr, "%s Failed to read fstype\n", LogPrefix);
        return false;
    }

    char *beginFlags = strtok(NULL, " ");

    if (beginFlags == NULL || strlen(beginFlags) == 0)
    {
        fprintf(stderr, "%s Failed to read flags\n", LogPrefix);
        return false;
    }

#if defined(__arm__)
    long unsigned int flags = (long unsigned int)strtol(beginFlags, NULL, 10);
#else
    int flags = (int)strtol(beginFlags, NULL, 10);
#endif
    char *options = strtok(NULL, " ");

    if (options == NULL)
    {
        options = "";
    }

    int err = mkdir(dirName, S_IRWXU | S_IRWXG | S_IRWXO);
    if (err != 0 && errno != EEXIST)
    {
        fprintf(stderr, "%s Failed to mkdir %s\n", LogPrefix, dirName);
        return false;
    }

    if (err == 0)
    {
        sync();
    }

    if (mount(devName, dirName, fsName, flags, options) != 0)
    {
        fprintf(
            stderr,
            "%s Failed to mount %s, %s, %s\n",
            LogPrefix,
            devName,
            dirName,
            fsName);
        perror("ERROR mount");
        fflush(stderr);
        return false;
    }

    return true;
}

static void parseArgs(int argc, char *const argv[])
{
    int ch = 0;

    while ((ch = getopt(argc, argv, "l:")) != -1)
    {
        switch (ch)
        {
        case 'l':
            processCfgLine(optarg);
            break;
        }
    }
}

int main(int argc, char *const argv[])
{
    fprintf(stderr, "%s %s started\n", LogPrefix, argv[0]);

    fflush(stderr);

    parseArgs(argc, argv);

    if (threadRoutine == NULL)
    {
        fprintf(stderr, "%s Failed to initialize thread routine\n", LogPrefix);
        return EXIT_FAILURE;
    }

    (*threadRoutine)(threadRoutineArg);

    return EXIT_SUCCESS;
}
