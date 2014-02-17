#include <stdlib.h>
#include "types.h"
#include "array.h"
#include "algorithm.h"

static CL_Iterator next_it_array(CL_Iterator*, CL_Iterable*);
static CL_Iterator prev_it_array(CL_Iterator*, CL_Iterable*);

static CL_Iterator	begin_array(CL_Iterable *self)
{
  CL_Iterator	it;
  CL_Array	*array;
  
  array = (CL_Array*)(self);
  it.next = next_it_array;
  it.prev = prev_it_array;
  it.pos = 0;
  it.compare = cl_compare_iterator;
  if (array->parent.size > 0)
    it.value = array->values[0];
  return it;
}

static CL_Iterator end_array(CL_Iterable *self)
{
  CL_Iterator	it;
  CL_Array	*array;
  
  array = (CL_Array*)(self);
  it.next = next_it_array;
  it.prev = prev_it_array;
  it.compare = cl_compare_iterator;
  it.pos = array->parent.size;
  it.value = NULL;
  return it;
}

static CL_Iterator	next_it_array(CL_Iterator *self, CL_Iterable *iterable)
{
  CL_Iterator	it;
  CL_Array	*array;
  
  array = (CL_Array*)(iterable);
  it.next = next_it_array;
  it.prev = prev_it_array;
  it.pos = self->pos + 1;
  it.compare = cl_compare_iterator;
  if (it.pos < array->parent.size)
    it.value = array->values[it.pos];
  return it;
}

static CL_Iterator prev_it_array(CL_Iterator *self, CL_Iterable *iterable)
{
  CL_Iterator	it;
  CL_Array	*array;
  
  array = (CL_Array*)(iterable);
  it.next = next_it_array;
  it.prev = prev_it_array;
  it.pos = self->pos - 1;
  it.compare = cl_compare_iterator;
  it.value = array->values[it.pos];
  return it;
}

static CL_Iterator	get_iterator_array(CL_Collection *self,
					   unsigned int pos)
{
  CL_Iterator	it;
  CL_Array	*array;
  
  array = (CL_Array*)(self);
  it.next = next_it_array;
  it.prev = prev_it_array;
  it.pos = pos;
  it.compare = cl_compare_iterator;
  it.value = array->values[pos];
  return it;
}

static void	*get_data_array(CL_Collection *self, unsigned int pos)
{
  CL_Array	*array;
  
  array = (CL_Array*)(self);
  return array->values[pos];
}

static void	set_data_array(CL_Collection *self, 
			       unsigned int pos, void *value)
{
  CL_Array	*array;
  
  array = (CL_Array*)(self);
  array->values[pos] = value;
}

static int	push_back_array(CL_Collection *self, void *data)
{
  CL_Array	*array;
  
  array = (CL_Array*)self;
  if (self->size < self->allocated_size)
    {
      array->values[self->size] = data;
      ++self->size;
    }
  else
    {
      self->allocated_size += 8;
      array->values = realloc(array->values, 
			      sizeof(void*) * self->allocated_size);
      if (array->values == NULL)
	return FALSE;
      return push_back_array(self, data);
    }
  return TRUE;
}

static int	pop_back_array(CL_Collection *self)
{
  CL_Array	*array;
  
  array = (CL_Array*)self;
  if (self->size > 0)
    {
      if (array->parent.free_value != NULL)
	array->parent.free_value(array->values[self->size - 1]);
      array->values[self->size - 1] = NULL;
      --self->size;
      if (self->allocated_size - self->size > 8)
	{
	  self->allocated_size -= 8;
	  if (self->allocated_size == 0)
	    {
	      free(array->values);
	      array->values = NULL;
	      return TRUE;
	    }
	  else
	    {
	      array->values = realloc(array->values,
				      sizeof(void*) * self->allocated_size);
	    }
	  if (array->values == NULL)
	    return FALSE;
	  return TRUE;
	}
    }
  return TRUE;
}

CL_Collection	*cl_make_array(size_t type_size, void (*free_func)(void*))
{
  CL_Array	*array;
  
  array = calloc(1, sizeof(*array));
  if (!array)
    return NULL;
  array->parent.free_value = free_func;
  array->parent.size = 0;
  array->parent.create = cl_make_array;
  array->parent.parent.begin = begin_array;
  array->parent.parent.end = end_array;
  array->parent.get_data = get_data_array;
  array->parent.set_data = set_data_array;
  array->parent.get_iterator = get_iterator_array;
  array->parent.push_back = push_back_array;
  array->parent.pop_back = pop_back_array;
  array->parent.parent.type_size = type_size;
  return (CL_Collection*)array;
}

void	cl_destroy_array(CL_Array *self)
{
  if (self->parent.free_value != NULL)
    { 
      cl_for_each((CL_Iterable*)self, self->parent.free_value);
    }
  free(self->values);
  free(self);
}
