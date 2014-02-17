#ifndef COLLECTION_H_
# define COLLECTION_H_

# include <stddef.h>
# include "iterable.h"
# define BLOCK_SIZE 8


typedef char	bool;

struct s_cl_collection
{
  CL_Iterable			parent;
  struct s_cl_collection	*(*create)(size_t, void (*)(void*));
  CL_Iterator			(*get_iterator)(struct s_cl_collection*, unsigned int);
  void				*(*get_data)(struct s_cl_collection*, unsigned int);
  void				(*set_data)(struct s_cl_collection*, unsigned int, void*);
  int				(*push_back)(struct s_cl_collection*, void*);
  int				(*pop_back)(struct s_cl_collection*);
  void				(*free_value)(void*);
  size_t			size;
  size_t			allocated_size;
};


typedef struct s_cl_collection	CL_Collection;

#endif