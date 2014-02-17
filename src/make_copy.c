#include <stdlib.h>
#include <string.h>
#include "make_copy.h"

char	*cl_copy_byte(char c)
{
  return cl_copy_value(&c, sizeof(c));
}

short int	*cl_copy_short(short int c)
{
  return cl_copy_value(&c, sizeof(c));
}

int		*cl_copy_integer(int c)
{
  return cl_copy_value(&c, sizeof(c));
}

long int	*cl_copy_long(long int c)
{
  return cl_copy_value(&c, sizeof(c));
}

long long int	*cl_copy_long_long(long long int c)
{
  return cl_copy_value(&c, sizeof(c));
}

float		*cl_copy_float(float c)
{
  return cl_copy_value(&c, sizeof(c));
}

double		*cl_copy_double(double c)
{
  return cl_copy_value(&c, sizeof(c));
}

long double	*cl_copy_long_double(long double c)
{
  return cl_copy_value(&c, sizeof(c));
}

void		*cl_copy_value(void *value, size_t type)
{
  void		*copy;
  
  copy = malloc(type);
  if (copy == NULL)
    return NULL;
  memcpy(copy, value, type);
  return copy;
}
