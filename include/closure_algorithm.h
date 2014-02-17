#ifndef CLOSURE_ALGORITHM_H
#define CLOSURE_ALGORITHM_H

# include "collection.h"
# include "closure.h"

CL_Collection	*cl_closure_range(int, int, int, CL_Collection *(*)(size_t, void (*)(void*)));
void		cl_closure_for_each(CL_Iterable*, CL_Closure *closure);
void		cl_closure_for_range(int, int, int, CL_Closure *closure);
void		*cl_closure_find(CL_Iterable*, void*, CL_Closure *predicate);
void		cl_closure_apply_if(CL_Iterable*, CL_Closure *predicate, CL_Closure *apply);
size_t		cl_closure_count(CL_Iterable*, void*, CL_Closure *predicate);
void		cl_closure_replace_if(CL_Iterable*, void*, CL_Closure *predicate);
bool		cl_closure_every(CL_Iterable*, CL_Closure *predicate);
bool		cl_closure_some(CL_Iterable*, CL_Closure *predicate);
CL_Iterator	cl_closure_index_of(CL_Collection*, void*, CL_Closure *predicate);
CL_Collection	*cl_closure_filter(CL_Collection*, CL_Closure *predicate);
CL_Collection	*cl_closure_map(CL_Collection*,  CL_Collection*, CL_Closure *transform);
void		*cl_closure_reduce(CL_Collection*, CL_Closure *closure);

#endif
