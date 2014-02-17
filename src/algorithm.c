#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "algorithm.h"

void		cl_apply_if(CL_Iterable *self,
			    void (*applyFunc)(void*), bool (*predicate)(void*))
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      if (predicate(it.value))
	{
	  applyFunc(it.value);
	}
    }
}

void		cl_for_each(CL_Iterable *self, void (*eachFunc)(void*))
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      eachFunc(it.value);
    }
}

void		*cl_find(CL_Iterable *self,
			 void *valueToFind, bool (*equalFunc)(void*, void*))
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      if (equalFunc(it.value, valueToFind))
	{
	  return it.value;
	}
    }
  return NULL;
}

unsigned int	cl_count(CL_Iterable *self,
			 void *toFind, bool (*equalFunc)(void*, void*))
{
  unsigned int	nb;
  CL_Iterator	it;
  CL_Iterator	end;
  
  nb = 0;
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      if (equalFunc(it.value, toFind))
	{
	  ++nb;
	}
    }
  return nb;
}

void		cl_replace_if(CL_Iterable *self,
			      void *newValue, bool (*predicate)(void*))
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      if (predicate(it.value))
	{
	  memcpy(it.value, newValue, self->type_size);
	}
    }
}

void		cl_replace(CL_Iterable *self, void *value, void *newValue)
{
  CL_Iterator	it;
  CL_Iterator	end;
  
  end = self->end(self);
  for (it = self->begin(self); !it.compare(&it, &end); it = it.next(&it, self))
    {
      if (memcmp(it.value, value, self->type_size) == 0)
	{
	  memcpy(it.value, newValue, self->type_size);
	}
    }
}

