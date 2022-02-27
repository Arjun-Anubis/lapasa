#include <ctype.h>


#include "lapasa.h"


#include "log.h"
#include "colors.h"

int lapasa_test()
/* 
 * LaPaSa tests
 */
{
	struct Tokens tokens;
	char test_json[] = "{ \"forty two\"hundred:  \"42\"       ,\n fifty:50	   , \n 45: { 34 : 45 }}";
	log_info(" Doing " COLOR_ITALIC "lapasa's" COLOR_RESET " tests" );
	log_info(" Testing tokenizer" );

	tokens = atokenize( test_json );

	for ( int i = 0; i < tokens.length; i++ )
	{
		struct Kvpair pair = lapasa_splits( tokens.tokens[i] );
		printf( "%s -> %s\n", pair.key, pair.value.string);
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

char * splits( char * str, char seperator )
{
	int i;
	for ( i = 0; str[i] != '\0'; i++ )
	{
		if ( str[i] == seperator )
		{
			str[i] = '\0';
			return &(str[i+1]);
		}
	}
	return NULL;
}

char * shaves( char * str )
{
	bool seen_word = false;
	bool in_word = false;
	bool encountered_double_quotes = false;

	int bracket_depth = 0;

	enum lapasa_type type;

	char * return_value;

	for ( int i = 0; str[i] != '\0'; i++ )
	{
		if ( !(seen_word) && !(in_word) )
		{
			if (isspace( str[i] ))
			/*
			 * Blank before word
			 */
				continue;
			else
			/*
			 * First char
			 */
			{
				if (isdigit( str[i] ))
				{
					type = LAPASA_INT;
				}
				switch ( str[i] )
				{
					case '"':
						if (encountered_double_quotes == false)
						{
							encountered_double_quotes = true;
							type = LAPASA_STRING;
						}
						break;
					case '{':
						type = LAPASA_JSON;
						bracket_depth++;
						break;
				}
				return_value = &(str[i]);
				in_word = true;
			}
		}
		else if (!(seen_word) && (in_word))
		{
			if (isspace( str[i] ))
			/*
			 * First space after word
			 */
			{
				if (( encountered_double_quotes == 0 ) && (bracket_depth == 0))
				{
					in_word = false;
					seen_word = true;
				}
			}
			else
			/*
			 * All chars within the word
			 */
			{
				if ( type == LAPASA_INT )
					if ( str[i] == '.' )
						type = LAPASA_FLOAT;

				switch ( str[i] )
				{

					case '"':
						if (encountered_double_quotes == true)
						{
							encountered_double_quotes = false;
							seen_word = true;
							str[i + 1] = '\0';
						}
						else if (encountered_double_quotes == false)
							log_sl( "What is this even supposed to mean??" );
						break;
					case '{':
						bracket_depth++;
						break;
					case '}':
						bracket_depth--;
						break;
				}
			}
		}
		else if ( (seen_word) )
		/* 
		 * Seen word and now exiting
		 */
		{
			if ( isspace( str[i] ) )
			{
				str[i] = '\0';
				break;
			}

		}
	}
	return return_value;
}

struct Kvpair lapasa_splits( char * str )
{
	const char seperator = ':';
	char * key;
	char * val;

	union lapasa_json_object obj;
	enum lapasa_type type;

	key = &(str[0]);
	val = splits( str, seperator );

	log_sl( "Split key: %s", key );
	log_sl( "Split value: %s", val );

	key = shaves( key );
	val = shaves( val );

	log_sl( "Shaved key: %s", key );
	log_sl( "Shaved value: %s", val );


	
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
