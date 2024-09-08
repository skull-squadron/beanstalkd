#include "ct/ct.h"
#include "dat.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void
cttest_allocf(void)
{
    char *got;

    got = fmtalloc("hello, %s %d", "world", 5);
    assertf(strcmp("hello, world 5", got) == 0, "got \"%s\"", got);
    free(got);
}

void
cttest_opt_none(void)
{
    char *args[] = {
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.port, Portdef) == 0);
    assert(srv.addr == NULL);
    assert(job_data_size_limit == JOB_DATA_SIZE_LIMIT_DEFAULT);
    assert(srv.wal.filesize == Filesizedef);
    assert(srv.wal.wantsync == 1);
    assert(srv.wal.syncrate == DEFAULT_FSYNC_MS*1000000);
    assert(srv.user == NULL);
    assert(srv.wal.dir == NULL);
    assert(srv.wal.use == 0);
    assert(verbose == 0);
}

static void
success(void)
{
    _exit(0);
}

void
cttest_optminus(void)
{
    char *args[] = {
        "-",
        NULL,
    };

    atexit(success);
    optparse(&srv, args);
    assertf(0, "optparse failed to call exit");
}

void
cttest_optp(void)
{
    char *args[] = {
        "-p1234",
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.port, "1234") == 0);
}

void
cttest_optl(void)
{
    char *args[] = {
        "-llocalhost",
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.addr, "localhost") == 0);
}

void
cttest_optlseparate(void)
{
    char *args[] = {
        "-l",
        "localhost",
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.addr, "localhost") == 0);
}

void
cttest_optz(void)
{
    char *args[] = {
        "-z1234",
        NULL,
    };

    optparse(&srv, args);
    assert(job_data_size_limit == 1234);
}

void
cttest_optz_more_than_max(void)
{
    char *args[] = {
        "-z1073741825",
        NULL,
    };

    optparse(&srv, args);
    assert(job_data_size_limit == 1073741824);
}

void
cttest_opts(void)
{
    char *args[] = {
        "-s1234",
        NULL,
    };

    optparse(&srv, args);
    assert(srv.wal.filesize == 1234);
}

void
cttest_optf(void)
{
    char *args[] = {
        "-f1234",
        NULL,
    };

    optparse(&srv, args);
    assert(srv.wal.syncrate == 1234000000);
    assert(srv.wal.wantsync == 1);
}

void
cttest_optF(void)
{
    char *args[] = {
        "-f1234",
        "-F",
        NULL,
    };

    optparse(&srv, args);
    assert(srv.wal.wantsync == 0);
}

void
cttest_optu(void)
{
    char *args[] = {
        "-ukr",
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.user, "kr") == 0);
}

void
cttest_optb(void)
{
    char *args[] = {
        "-bfoo",
        NULL,
    };

    optparse(&srv, args);
    assert(strcmp(srv.wal.dir, "foo") == 0);
    assert(srv.wal.use == 1);
}

void
cttest_optV(void)
{
    char *args[] = {
        "-V",
        NULL,
    };

    optparse(&srv, args);
    assert(verbose == 1);
}

void
cttest_optV_V(void)
{
    char *args[] = {
        "-V",
        "-V",
        NULL,
    };

    optparse(&srv, args);
    assert(verbose == 2);
}

void
cttest_optVVV(void)
{
    char *args[] = {
        "-VVV",
        NULL,
    };

    optparse(&srv, args);
    assert(verbose == 3);
}

void
cttest_optVFVu(void)
{
    char *args[] = {
        "-VFVukr",
        NULL,
    };

    optparse(&srv, args);
    assert(verbose == 2);
    assert(srv.wal.wantsync == 0);
    assert(strcmp(srv.user, "kr") == 0);
}
