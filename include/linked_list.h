#ifndef LINKED_LIST_H
# define LINKED_LIST_H

# include "list.h"

struct s_cl_linked_list_node
{
  struct s_cl_list_node	parent;
  unsigned int 		pos;
};

CL_Collection	*cl_make_linked_list(size_t, void (*)(void*));
void		cl_destroy_linked_list(CL_List *list);

#endif