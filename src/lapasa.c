#include "lapasa.h"
#include "colors.h"

int lapasa_test()
/* 
 * LaPaSa tests
 */
{
	struct Tokens tokens;
	char test_json[] = "{ forty:  40       , fifty:50	   , 45: { 34 : 45 }}";
	log_info(" Doing " COLOR_ITALIC "lapasa's" COLOR_RESET " tests" );
	log_info(" Testing tokenizer" );

	tokens = atokenize( test_json );

	for ( int i = 0; i < tokens.length; i++ )
	{
		struct Kvpair pair = lapasa_splits( tokens.tokens[i] );
		printf( "%s is %s\n", pair.key, pair.value.string);
	}

	free( tokens.tokens );
	
	return 0;
}

struct Tokens atokenize( char * obj )
{
	int array_len = 0;
	int obj_depth = 0;
	char ** table;
	table = malloc( sizeof( char * ) * 0 );
	for ( int i = 0; obj[i] != '\0'; i++ )
	{
		switch ( obj[i] )
		{

			case '{':
				/*
				 * Seperate if its first {
				 */
				if (obj_depth == 0)
				{
					obj[i] = '\0';
					array_len++;

					/* table = realloc( ( sizeof( char * ) * table, array_len ); */
					table = realloc( table, sizeof( char * ) * array_len );

					/* 
					 * Set  last element to address of next char
					 */

					table[ array_len - 1] = &obj[i+1];
				}

				obj_depth++;

				break;
			case '}':

				obj_depth--;

				/*
				 * Seperate if its last }
				 */
				if (obj_depth == 0)
					obj[i] = '\0'; 

				break;
			case ',':
				if (obj_depth == 1)
					obj[i] = '\0';
				array_len++;

				table = realloc( table, sizeof( char * ) * array_len );

				/* 
				 * Set  last element to address of next char
				 */

				table[ array_len - 1] = &obj[i+1];
				break;
			case ' ':
				break;
			case '\n':
				break;
			case '\t':
				break;
		}
	}
	return (struct Tokens) { .tokens = table, .length = array_len };
};

struct Kvpair lapasa_splits( char * str )
{
	char * key;
	char * val;
	union lapasa_json_object obj;
	enum lapasa_type type;
	int in_key = 0;
	int in_val = 0;

	int seen_key = 0;
	int seen_val = 0;

	for ( int i = 0; str[i] != '\0'; i++ )
	{
		switch ( str[i] )
		{
			case ':':
				str[i] = '\0';
				break;
			case ' ':
				str[i] = '\0';

				if (!(seen_key) && !(seen_val))
				{
					if (!(in_key))
					/*
					 * Before key
					 */
					{
						break;
					}

					if (in_key == 1)
					/*
					 * First space between key and val
					 */
					{
						in_key = 0;
						seen_key = 1;
					}
				}
				
					
				if ((seen_key) && !(seen_val))
				{
					if (!(in_val))
					/*
					 * Between key and val
					 */
					{
						break;
					}

					if (in_val)
					/* 
					 * First space after val
					 */

					{
						in_val = 0;
					}
				}

			default:

				if (!(in_key) && !(in_val))
				{
					if (!(seen_key))
					{
						key = &(str[i]);
						in_key = 1;
					}
					if (seen_key)
					{
						val = &(str[i]);
						in_val = 1;
					}
				}
		}
	}
	obj.string = val;
	type = LAPASA_STRING;
	return (struct Kvpair) { .key = key, .value = obj, .value_type = type };
}

void lapasa_init_json( char * str_json, lapasa_json_object * json )
/*
 * Initializes a json from a json string, into the given pointer
 */
{
	json->json.length = 5;
	json->json.keys = malloc( sizeof( char * ) * json->json.length );
	json->json.values = malloc( sizeof( lapasa_json_object * ) * json->json.length );
	json->json.types = malloc( sizeof( enum lapasa_type ) * json->json.length );

	for ( int i = 0; i < json->json.length; i++ )
	{
		int keylen = 6;
		json->json.keys[i] = malloc( sizeof( char ) * keylen );
		// add type switch here
	}
}

struct lapasa_json lapasa_json_get( lapasa_json_object json, char * key )
/* 
 * Fetches a value from a json given a key, returns a struct lapasa_json which contains the json as well as the type
 * If no key is found returns a struct with json as null pointer and type as LAPASA_NOT_FOUND
 */
{
	for ( int i = 0; i < json.json.length; i++ )
	{
		log_sl( "Checking %s against %s", json.json.keys[i], key );
		if ( strcmp( key, json.json.keys[i] ) == 0 )
		{
			return (struct lapasa_json){ .json = &(json.json.values[i]), .type = json.json.types[i] };
		}
	}
	return (struct lapasa_json){ .json = NULL, .type = LAPASA_NOT_FOUND };
}
