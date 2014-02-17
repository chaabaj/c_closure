#ifndef ALGORITHM_H_
# define ALGORITHM_H_

# include "iterable.h"


void		cl_for_each(CL_Iterable*, void (*)(void*));
void		*cl_find(CL_Iterable*, void*, bool (*)(void*, void*));
void		cl_apply_if(CL_Iterable*, void (*)(void*), bool (*)(void*));
unsigned int	cl_count(CL_Iterable*, void*, bool(*)(void*, void*));
void		cl_replace(CL_Iterable*, void*, void*);
void		cl_replace_if(CL_Iterable*, void*, bool (*)(void*));

#endif
