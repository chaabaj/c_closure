#include "iterable.h"

bool	cl_compare_iterator(CL_Iterator *self, CL_Iterator *it)
{
  if (self->pos != it->pos)
    return FALSE;
  return TRUE;
}
