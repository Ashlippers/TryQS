#include "env.h"
#include "prime_gen.h"
#include "relation_collect.h"
#include "large_integer.h"


long f;	

int main(int argc, char const *argv[])
{
	//scanf("%ld", f);
	f = 4033;
	prime_gen();
	//prime_print();

	collect(f, 8, 8);
	print_relation(8, 8);
	
	return 0;
}
