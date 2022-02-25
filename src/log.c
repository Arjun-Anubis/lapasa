#define _GNU_SOURCE
#include <stdarg.h>
#include <stdlib.h>

#include "colors.h"
#include "log.h"


/* Defaults */
int LOGLEVEL = LOG_MAX;
int LOG_NEWLINE = 1;
int LOG_FILE = 1;

void log_setlevel( int loglevel )
{
	LOGLEVEL = loglevel;
	return;
}

int vlog_stdc( const char * prefix, const char * format, va_list args )
	/*
	 * Valist LOG STanDard Custom
	 */
{
	int result;

	/*
	 * Needed for it to find where the arguments went, so format is the last named argument
	 */

	printf( "%s", prefix );
	result = vprintf( format, args );

	if (LOG_NEWLINE)
		printf( "\n" );

	return result;
}

int log_stdc( const char * prefix, const char * format, ...)
	/* 
	 * Log STanDard Custom Prefix
	 */
{
	int result;
	va_list args;
	va_start( args, format ); 

	/*
	 * Needed for it to find where the arguments went, so format is the last named argument
	 */

	printf( "%s", prefix );
	result = vprintf( format, args );

	if (LOG_NEWLINE)
		printf( "\n" );

	return result;
}

int log_std
(
	const char * file,
   	int line_number,
   	int loglevel,
   	const char * format,
   	... 
)
{
	char * prefix;
	int return_code;
	va_list args;
	va_start( args, format );

	char time_s[TIME_LEN];
	time_t now = time(0);
	struct tm  tstruct;

	tstruct = *localtime(&now);

	strftime( time_s, TIME_LEN, "%H:%M:%S", &tstruct );

	char prefix_fmt[] = COLOR_GREEN "%s" COLOR_RESET " " COLOR_BOLD COLOR_YELLOW "%s" COLOR_RESET ":" COLOR_CYAN "%d" COLOR_RESET "\t%s\t";

	switch (loglevel)
	{
		case LOG_PANIC:
			if ( LOGLEVEL >= LOG_PANIC )
				asprintf
				(
					 &prefix,
					 prefix_fmt, time_s,
					 file, line_number,
					 COLOR_RED COLOR_BLINK LOG_S_PANIC COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_ERR:
			if ( LOGLEVEL >= LOG_ERR )
				asprintf
				(
					 &prefix,
					 prefix_fmt, time_s,
					 file, line_number,
					 COLOR_RED LOG_S_ERR COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_WARN:
			if ( LOGLEVEL >= LOG_WARN )
				asprintf
				(
					 &prefix,
					 prefix_fmt, time_s,
					 file, line_number,
					 COLOR_YELLOW LOG_S_WARN COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_MSG:
			if ( LOGLEVEL >= LOG_MSG )
				asprintf
				(
					&prefix,
					prefix_fmt, time_s,
					file, line_number,
					COLOR_GREEN LOG_S_MSG COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_MTC:
			if ( LOGLEVEL >= LOG_MTC )
				asprintf
				(
				   	&prefix,
					prefix_fmt, time_s,
					file, line_number,
					COLOR_PURPLE LOG_S_MTC COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_DEBUG:
			if ( LOGLEVEL >= LOG_DEBUG )
				asprintf
				(
					&prefix,
					prefix_fmt, time_s,
					file, line_number,
					COLOR_CYAN LOG_S_DEBUG COLOR_RESET 
				);
			else return -1;
			break;
		case LOG_SL:
			if ( LOGLEVEL >= LOG_SL )
				asprintf
				(
					   	&prefix,
						prefix_fmt, time_s,
						file, line_number,
						COLOR_BLUE LOG_S_SL COLOR_RESET 
				);
			else return -1;
			break;
	}
	return_code = vlog_stdc( prefix, format, args );
	free(prefix);
	return return_code;
}

int log_test( void )
{
	for ( LOGLEVEL = 0; LOGLEVEL <= LOG_MAX; LOGLEVEL++ )
	{
		printf( "Set Log Level to %d\n", LOGLEVEL );
		log_panic( "Panic" );
		log_err( "Error" );
		log_warn( "Warning" );
		log_msg( "Message" );
		log_mtc( "mtc" );
		log_debug( "Debug" );
		log_sl( "sl" );
	}

	printf( "\n\n\n" );

	printf( "Automated Testing\n" );

	printf( "\n\n\n" );

	for ( LOGLEVEL = 0; LOGLEVEL <= LOG_MAX; LOGLEVEL++ )
		for ( int i = 0; i <= LOGLEVEL; i++ )
			if (log_wl( i, "..." ) < 0)
				printf( "Log errored\n" );

	return 0;
}
