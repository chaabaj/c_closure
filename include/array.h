#ifndef ARRAY_H_
# define ARRAY_H_

# include "collection.h"

struct s_cl_array
{
  CL_Collection	parent;
  void		**values;
};

typedef struct s_cl_array	CL_Array;

CL_Collection	*cl_make_array(size_t, void (*)(void*));
void		cl_destroy_array(CL_Array*);

#endif