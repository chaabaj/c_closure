#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "make_copy.h"
#include "closure_algorithm.h"


void		cl_closure_for_each(CL_Iterable *self, CL_Closure *closure)
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      cl_closure_call(closure, 3, it.value, it.pos, self);
    }
}

void		*cl_closure_find(CL_Iterable *self, 
				 void *valueToFind, CL_Closure *closure)
{
  bool		*value;
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(closure, 2, it.value, valueToFind);
      if (*value)
	{
	  return it.value;
	}
    }
  return NULL;
}

void		cl_closure_apply_if(CL_Iterable *self, 
				    CL_Closure *predicate, CL_Closure *apply)
{
  bool		*value;
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(predicate, 1, it.value);
      if (*value)
	{
	  cl_closure_call(apply, 1, it.value);
	}
    }
}

size_t		cl_closure_count(CL_Iterable *self, 
				 void *valueToFind, CL_Closure *predicate)
{
  size_t	nbOccured;
  CL_Iterator	end;
  bool		*value;
  CL_Iterator	it;
  
  nbOccured = 0;
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(predicate, 2, it.value, valueToFind);
      if (*value)
	{
	  ++nbOccured;
	}
    }
  return nbOccured;
}

void		cl_closure_replace_if(CL_Iterable *self,
				      void *newValue, CL_Closure *predicate)
{
  bool		*value;
  CL_Iterator	it;
  CL_Iterator	end;

  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(predicate, 1, it.value);
      if (*value)
	memcpy(it.value, newValue, self->type_size);
    }
}

bool		cl_closure_every(CL_Iterable *self, CL_Closure *predicate)
{
  bool		*value;
  CL_Iterator	it;
  CL_Iterator	end;

  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(predicate, 1, it.value);
      if (!*value)
	return FALSE;
    }
  return TRUE;
}

bool		cl_closure_some(CL_Iterable *self, CL_Closure *predicate)
{
  bool		*value;
  CL_Iterator	it;
  CL_Iterator	end;

  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      value = cl_closure_call(predicate, 1, it.value);
      if (*value)
	{
	  return TRUE;
	}
    }
  return FALSE;
}

CL_Iterator	cl_closure_index_of(CL_Collection *self,
				    void *value, CL_Closure *predicate)
{
  bool		*return_value;
  CL_Iterator	it;
  CL_Iterator	end;

  end = self->parent.end((CL_Iterable*)self);
  for (it = self->parent.begin((CL_Iterable*)self);
       !it.compare(&it, &end); it = it.next(&it, (CL_Iterable*)self))
    {
      return_value = cl_closure_call(predicate, 2, it.value, value);
      if (*return_value)
	{
	  return it;
	}
    }
  return it;
}

void		cl_closure_for_range(int start, int end, int step,
				     CL_Closure *closure)
{
  int i;
  
  if (end < start)
    {
      for (i = start; i > end; i -= step)
	{
	  cl_closure_call(closure, i);
	}
    }
  else
    {
      for (i = start; i < end; i += step)
	{
	  cl_closure_call(closure, i);
	}
    }
}

static void 	*range_closure(CL_Closure *self, va_list *args)
{
  unsigned int		*index;
  CL_Collection		*collection;
  
  index = malloc(sizeof(int));
  *index = va_arg(*args, unsigned int);
  collection = cl_closure_get_arg(self, 0);
  collection->push_back(collection, index);
  return NULL;
}

CL_Collection	*cl_closure_range(int start, int end, int step,
				  CL_Collection *(*create)(size_t, void (*)(void*)))
{
  CL_Collection	*collection;
  CL_Closure	*closure;

  collection = (*create)(sizeof(int), free);
  closure = cl_make_closure(range_closure, 0, 1,
			    cl_closure_capture(collection, FALSE));
  cl_closure_for_range(start, end, step, closure);
  cl_destroy_closure(closure);
  return collection;
}

static void	*each_filter(CL_Closure *self, va_list *args)
{
  CL_Collection	*collection;
  CL_Closure	*predicate;
  void		*value;
  bool		*return_value;
  
  collection = cl_closure_get_arg(self, 0);
  predicate = cl_closure_get_arg(self, 1);
  value = va_arg(*args, void*);
  return_value = cl_closure_call(predicate, 1, value);
  if (*return_value)
    {
      collection->push_back(collection,
			    cl_copy_value(value, collection->parent.type_size));
    }
  return NULL;
}

CL_Collection	*cl_closure_filter(CL_Collection *self, CL_Closure *predicate)
{
  CL_Closure	*closure;
  CL_Collection	*collection;

  collection = self->create(self->parent.type_size, self->free_value);
  closure = cl_make_closure(each_filter, 0, 2, 
			    cl_closure_capture(collection, FALSE), 
			    cl_closure_capture(predicate, FALSE));
  cl_closure_for_each((CL_Iterable*)self, closure);
  cl_destroy_closure(closure);
  return collection;
}

CL_Collection	*cl_closure_map(CL_Collection *self, 
				CL_Collection *newCollection,
				CL_Closure *transform)
{
  CL_Iterator	it;
  CL_Iterator	end;
  void		*transformedValue;
  
  end = self->parent.end((CL_Iterable*)self);
  for (it = self->parent.begin((CL_Iterable*)self);
       !it.compare(&it, &end); it = it.next(&it, (CL_Iterable*)self))
    {
      transformedValue = cl_closure_call(transform, 3, it.value, it.pos, self);
      newCollection->push_back(newCollection, cl_copy_value(transformedValue, newCollection->parent.type_size));
    }
  return newCollection;
}

void		*cl_closure_reduce(CL_Collection *self, CL_Closure *closure)
{
  CL_Iterator	it;
  CL_Iterator	end;
  void		*value;
  
  end = self->parent.end((CL_Iterable*)self);
  for (it = self->parent.begin((CL_Iterable*)self);
       !it.compare(&it, &end); it = it.next(&it, (CL_Iterable*)self))
    {
      value = cl_closure_call(closure, 4, value, it.value, it.pos, self);
    }
  return value;
}
