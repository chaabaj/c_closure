#ifndef LIST_H
# define LIST_H

# include "collection.h"

struct s_cl_list_node
{
  void			*data;
  struct s_cl_list_node	*next;
  struct s_cl_list_node	*prev;
};

struct s_cl_list
{
  CL_Collection		parent;
  struct s_cl_list_node	*first;
  struct s_cl_list_node	*current;
  struct s_cl_list_node	*last;
};

typedef struct s_cl_list	CL_List;


#endif