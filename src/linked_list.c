#include <stdlib.h>
#include "linked_list.h"
#include "assert.h"

static CL_Iterator next_it_linked_list(CL_Iterator*, CL_Iterable*);
static CL_Iterator prev_it_linked_list(CL_Iterator*, CL_Iterable*);

static struct s_cl_linked_list_node	*get_nearest_linked_list_node(CL_List *self, unsigned int pos, int *way)
{
  if (pos < self->parent.size / 2)
    {
      *way = 0;
      return (struct s_cl_linked_list_node*)self->first;
    }
  *way = 1;
  return (struct s_cl_linked_list_node*)self->last;
}

static struct s_cl_linked_list_node	*search_linked_list_node(CL_List *self, unsigned int pos)
{
  CL_List				*list;
  struct s_cl_linked_list_node		*node;
  int					way;
  
  assert(pos < self->parent.size);
  list = (CL_List*)self;
  if (list->current == NULL)
    node = get_nearest_linked_list_node(list, pos, &way);
  else
    {
      node = (struct s_cl_linked_list_node*)list->current;
      if ((unsigned)abs(node->pos - pos) < (self->parent.size - pos))
	{
	  node = (struct s_cl_linked_list_node*)list->current;
	  if ((signed)node->pos - (signed)pos < 0)
	    way = 0;
	  else
	    way = 1;
	}
      else
	{
	  way = 1;
	  node = (struct s_cl_linked_list_node*)list->last;
	}
    }
  
  while (node != NULL && node->pos != pos)
    {
      if (way == 0)
	node = (struct s_cl_linked_list_node*)node->parent.next;
      else
	node = (struct s_cl_linked_list_node*)node->parent.prev;
    }
  if (node != NULL)
    list->current = (struct s_cl_list_node*)node;
  return node;
}

static CL_Iterator prev_it_linked_list(CL_Iterator *self ,
				       CL_Iterable *iterable)
{
  CL_List	*list;
  CL_Iterator	it;
  
  assert(self->pos > 0);
  list = (CL_List*)iterable;
  it.compare = cl_compare_iterator;
  it.next = next_it_linked_list;
  it.prev = prev_it_linked_list;
  it.pos = self->pos - 1;
  it.value = search_linked_list_node(list, it.pos)->parent.data;
  return it;
}

static CL_Iterator	next_it_linked_list(CL_Iterator *self,
					    CL_Iterable *iterable)
{
  CL_List		*list;
  CL_Iterator		it;
  
  assert(self->pos < ((CL_Collection*)iterable)->size);
  list = (CL_List*)iterable;
  it.compare = cl_compare_iterator;
  it.next = next_it_linked_list;
  it.prev = prev_it_linked_list;
  it.pos = self->pos + 1;
  if (it.pos < list->parent.size)
    it.value = search_linked_list_node(list, it.pos)->parent.data;
  return it;
}

static CL_Iterator	begin_linked_list_iterator(CL_Iterable *self)
{
  CL_Iterator	it;
  CL_List		*list;
  
  list = (CL_List*)self;
  it.compare = cl_compare_iterator;
  it.next = next_it_linked_list;
  it.prev = prev_it_linked_list;
  it.pos = 0;
  if (list->first)
    it.value = list->first->data;
  return it;
}

static CL_Iterator	end_linked_list_iterator(CL_Iterable *self)
{
  CL_Iterator		it;
  CL_List		*list;
  
  list = (CL_List*)self;
  it.compare = cl_compare_iterator;
  it.next = next_it_linked_list;
  it.prev = prev_it_linked_list;
  it.pos = list->parent.size;
  it.value = NULL;
  return it;
}

static CL_Iterator	get_linked_list_iterator(CL_Collection *self,
						 unsigned int pos)
{
  CL_Iterator		it;
  CL_List		*list;
  
  list = (CL_List*)self;
  assert(list->parent.size > 0);
  assert(pos < list->parent.size - 1);
  it.compare = cl_compare_iterator;
  it.next = next_it_linked_list;
  it.prev = prev_it_linked_list;
  it.pos = pos;
  it.value = search_linked_list_node(list, pos)->parent.data;
  return it;
}

static struct s_cl_linked_list_node	*make_linked_list_node(void *data)
{
  struct s_cl_linked_list_node		*node;
  
  node = malloc(sizeof(*node));
  if (node == NULL)
    return NULL;
  node->parent.data = data;
  node->parent.next = NULL;
  node->parent.prev = NULL;
  node->pos = 0;
  return node;
}

static int			push_back_linked_list(CL_Collection *self,
						      void *data)
{
  CL_List			*list;
  struct s_cl_linked_list_node	*newNode;
  
  newNode = make_linked_list_node(data);
  if (newNode == NULL)
    return FALSE;
  list = (CL_List*)self;
  if (list->last == NULL)
  {
    list->first = (struct s_cl_list_node*)newNode;
    list->last = (struct s_cl_list_node*)newNode;
  }
  else
  {
    list->last->next = (struct s_cl_list_node*)newNode;
    newNode->parent.prev = list->last;
    list->last = (struct s_cl_list_node*)newNode;
  }
  ++list->parent.size;
  newNode->pos = list->parent.size - 1;
  return TRUE;
}

static int		pop_back_linked_list(CL_Collection *self)
{
  CL_List		*list;
  struct s_cl_list_node	*node;
  
  list = (CL_List*)self;
  if (list->last == NULL)
    return FALSE;
  node = list->last;
  list->last = list->last->prev;
  if (self->free_value != NULL)
    self->free_value(node->data);
  free(node);
  --self->size;
  return TRUE;
}


static void			*get_data_linked_list(CL_Collection *self,
						      unsigned int pos)
{
  struct s_cl_linked_list_node	*node;
  
  node = search_linked_list_node((CL_List*)self, pos);
  if (node == NULL)
    return NULL;
  return node->parent.data;
}

static void			set_data_linked_list(CL_Collection *self,
						     unsigned int pos,
						     void *data)
{
  struct s_cl_linked_list_node	*node;
  
  node = search_linked_list_node((CL_List*)self, pos);
  assert(node != NULL);
  node->parent.data = data;
}

CL_Collection	*cl_make_linked_list(size_t size, void (*freeData)(void*))
{
  CL_List	*list;
  
  list = calloc(1, sizeof(*list));
  list->parent.free_value = freeData;
  list->parent.parent.type_size = size;
  list->parent.create = cl_make_linked_list;
  list->parent.get_data = get_data_linked_list;
  list->parent.pop_back = pop_back_linked_list;
  list->parent.push_back = push_back_linked_list;
  list->parent.set_data = set_data_linked_list;
  list->parent.get_iterator = get_linked_list_iterator;
  list->parent.parent.begin = begin_linked_list_iterator;
  list->parent.parent.end = end_linked_list_iterator;
  return (CL_Collection*)(list);
}

void		cl_destroy_linked_list(CL_List *self)
{
  while (self->parent.size > 0)
    self->parent.pop_back((CL_Collection*)self);
  free(self);
}
