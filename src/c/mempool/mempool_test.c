#include <mempool.h>
#include <stdio.h>
#include <string.h>

void test_mempool_available ( mempool_t *p, size_t expected );
void test_mempool_add ( mempool_t *p, const char *str );
void test_mempool_reset ( mempool_t *p );

int main ( int argc, char *argv[] ) {
	mempool_t *pool = NULL;

	test_mempool_available( pool, 0 );

	pool = mempool_create( 128 );

	test_mempool_available( pool, 128 );
	test_mempool_add( pool, "hello world" );
	test_mempool_add( pool, "goodbye world" );
	test_mempool_reset( pool );
	test_mempool_add( pool, "let's try this again" );
	test_mempool_available( pool, 128 - strlen("let's try this again") - 2 );

	mempool_destroy( pool );

	return 0;
}

void test_mempool_available ( mempool_t *p, size_t expected ) {
	size_t retval;

	printf( "test mempool_available(): " );
	retval = mempool_available( p );
	if ( retval == expected )
		printf("ok.\n");
	else
		printf("failed - mempool_available() returned %u instead of %u\n",
				retval, expected );
}

void test_mempool_add ( mempool_t *p, const char *str ) {
	/* make sure the null terminator is copied */
	size_t sz = strlen(str) + 1;
	size_t avail = mempool_available(p);
	size_t retval;
	void *pool_str;

	printf( "test mempool_add(): " );

	pool_str = mempool_add ( p, str, sz );

	if ( pool_str == NULL ) {
		printf("failed - mempool_add() returned NULL.\n");
		return;
	}

	if ( (retval = mempool_available(p)) != avail - sz - 1 ) {
		printf("failed - mempool_available() returned %u instead of %u.\n",
				retval, avail - sz - 1);
		return;
	}

	if ( strcmp( (char *) pool_str, str ) != 0 ) {
		printf("failed - string in pool is \"%s\" instead of \"%s\"\n",
				(char *) pool_str, str);
		return;
	}

	printf("ok.\n");
}

void test_mempool_reset ( mempool_t *p ) {
	size_t avail, capacity;
	printf( "test mempool_reset(): " );

	mempool_reset( p );
	avail = mempool_available(p);
	capacity = mempool_capacity(p);

	if ( avail != capacity ) {
		printf("failed - available: %u, capacity: %u\n",
				avail, capacity);
	}
	else {
		printf("ok.\n");
	}
}
