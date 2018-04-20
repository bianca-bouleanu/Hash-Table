/* Functie preluata din sectiunea de Resurse a
	paginii https://ocw.cs.pub.ro/courses/so/laboratoare/
	2018
 */

#include <errno.h>

#define DIE(assertion, call_description)		\
	do {							\
		if (assertion) {				\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(errno);				\
		}							\
	} while (0)
