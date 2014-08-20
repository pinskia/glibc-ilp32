/* Determine the wordsize from the preprocessor defines.  */

#ifdef __LP64__
# define __WORDSIZE	64
#else
# define __WORDSIZE	32
#endif

/* Both ILP32 and LP64 have a 64bit time_t. */
#define __WORDSIZE_TIME64_COMPAT32	1
