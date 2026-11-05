#include "cube.h"
#include "debuglog.h"
#include <cstdio>
#include <cstdarg>

static FILE *dbgfile = NULL;

void debuglog(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    conoutf("LOG: %s", buf);
    if (!dbgfile) dbgfile = fopen("profile/debuglog.txt", "a");
    if (dbgfile) { fprintf(dbgfile, "%s\n", buf); fflush(dbgfile); }
}