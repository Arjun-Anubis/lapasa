#ifndef LOGH
#define LOGH

#include <stdio.h>
#include <time.h>

#define TIME_LEN 12 

#define LOG_PANIC 0
#define LOG_ERR  1
#define LOG_WARN  2
#define LOG_MSG   3 
#define LOG_MTC   4 // Maintainence
#define LOG_DEBUG 5
#define LOG_SL    6 // Super Low
#define LOG_MAX   LOG_SL

#define LOG_S_PANIC  "Panic"
#define LOG_S_ERR  "Error"
#define LOG_S_WARN  "Warn"
#define LOG_S_MSG  "INFO"
#define LOG_S_MTC  "Mtc"
#define LOG_S_DEBUG  "Debug"
#define LOG_S_SL  "SL"

extern int LOGLEVEL;
extern int LOG_NEWLINE;
extern int LOG_FILE;

void log_setlevel( int loglevel );

int log_std( const char * file, int line, int loglevel, const char * fmt, ...);
int log_stdc( const char * prefix, const char * fmt, ... );

int log_test(void);

#define log_wl( loglevel, ...) ( log_std( __FILE__, __LINE__, loglevel, __VA_ARGS__ ) )

#define log_panic(...) ( log_std( __FILE__, __LINE__, LOG_PANIC, __VA_ARGS__ ) )
#define log_err(...) ( log_std( __FILE__, __LINE__, LOG_ERR, __VA_ARGS__ ) )
#define log_warn(...) ( log_std( __FILE__, __LINE__, LOG_WARN, __VA_ARGS__ ) )
#define log_msg(...) ( log_std( __FILE__, __LINE__, LOG_MSG, __VA_ARGS__ ) )
#define log_mtc(...) ( log_std( __FILE__, __LINE__, LOG_MTC, __VA_ARGS__ ) )
#define log_debug(...) ( log_std( __FILE__, __LINE__, LOG_DEBUG, __VA_ARGS__ ) )
#define log_sl(...) ( log_std( __FILE__, __LINE__, LOG_SL, __VA_ARGS__ ) )

/*
 * Aliases
 */

#define log_info(...) ( log_std( __FILE__, __LINE__, LOG_MSG, __VA_ARGS__ ) )

#endif // LOGH

