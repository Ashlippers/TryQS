#ifndef ENV_H
#define ENV_H

#include "large_integer.h"
#include <time.h>
#include <set>
#include <stack>
#define D_SP 10000 // Dimension of small primes
#define D_P 1000   // Dimension of primes
#define D_R 1000   // Dimension of relations
#define D_PR 50000 // Dimension of partial relations
using namespace std;

extern long p[D_SP+10]; 			// Prime table
extern bool m[D_R+10][D_P+10]; 	// Relation table
extern BigInt y[D_R+10];			// Y table for each relation (y[i]^2 is factored in m[i])
extern BigInt r[D_R+10];          // Remain number table of factor
extern BigInt q[D_R+10];
extern BigInt f;					// Number to factor
extern time_t start_time;
extern time_t cur_time;
extern clock_t start_time_ms;
extern clock_t cur_time_ms;
extern long cursetsize;
extern float Alpha;
extern multiset<BigInt> rs;
extern stack<BigInt> ns;
#endif
