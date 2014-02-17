#ifndef ITERABLE_H
#define ITERABLE_H

# include "types.h"
#include <stddef.h>

typedef struct s_cl_iterable*	_CL_Iterable;

struct s_cl_iterator
{
  unsigned int		pos;
  void			*value;
  struct s_cl_iterator	(*next)(struct s_cl_iterator*, _CL_Iterable);
  struct s_cl_iterator	(*prev)(struct s_cl_iterator*, _CL_Iterable);
  bool			(*compare)(struct s_cl_iterator*, 
				   struct s_cl_iterator*);
};

struct s_cl_iterable
{
  struct s_cl_iterator	(*begin)(struct s_cl_iterable*);
  struct s_cl_iterator	(*end)(struct s_cl_iterable*);
  size_t		type_size;
};

typedef struct s_cl_iterator	CL_Iterator;
typedef struct s_cl_iterable	CL_Iterable;

bool	cl_compare_iterator(struct s_cl_iterator*, struct s_cl_iterator*);

#endif
