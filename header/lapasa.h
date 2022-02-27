#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


struct Tokens
{
	char ** tokens;
	int length;
};


enum lapasa_type
{
	LAPASA_NOT_FOUND,
	LAPASA_INT,
	LAPASA_FLOAT,
	LAPASA_STRING,
	LAPASA_JSON,
};

struct internal_lapasa_json
{
	char ** keys;
	union lapasa_json_object * values;
	enum lapasa_type * types;
	int length;
};


typedef union lapasa_json_object
{
	double d;
	int i;
	char * string;
	struct internal_lapasa_json json;
} lapasa_json_object;

struct lapasa_json
{
	lapasa_json_object * json;
	enum lapasa_type type;
};

struct Kvpair
{
	char * key;
	union lapasa_json_object value;
	enum lapasa_type value_type;
};

int lapasa_test();
void lapasa_init_json( char * str_json, lapasa_json_object * json );
struct lapasa_json lapasa_json_get( lapasa_json_object json, char * key );

struct Tokens atokenize( char * obj );
struct Kvpair lapasa_splits( char * str );
