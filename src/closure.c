#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "types.h"
#include "closure.h"

static void	make_vars_capture(CL_Closure *self,  va_list *list)
{
  unsigned int	i;
  CL_Value	capture;
  
  
  for (i = 0; i < self->nb_captured_vars; ++i)
  {
    capture = va_arg(*list, CL_Value);
    self->captured_vars[i] = capture;
  }
}

CL_Closure	*cl_make_closure(CL_Function function,
			      size_t return_type,
			      size_t nb_captured_vars, ...)
{
  va_list	list;
  CL_Closure	*closure;
  
  va_start(list, nb_captured_vars);
  closure = malloc(sizeof(*closure));
  if (closure == NULL)
    return NULL;
  closure->return_type = return_type;
  closure->return_value.is_copy = FALSE;
  closure->nb_captured_vars = nb_captured_vars;
  closure->captured_vars = NULL;
  closure->function = function;
  if (nb_captured_vars > 0)
    {
      closure->captured_vars = malloc(sizeof(CL_Value) * nb_captured_vars);
      if (closure->captured_vars == NULL)
	{
	  free(closure);
	}
      make_vars_capture(closure, &list);
      va_end(list);
    }
  return closure;
}

void	*cl_closure_return_value(CL_Closure *self, void *return_value)
{
  if (self->return_value.value == NULL)
    self->return_value.value = malloc(self->return_type);
  memcpy(self->return_value.value, return_value, self->return_type);
  self->return_value.is_copy = TRUE;
  return self->return_value.value;
}

void	*cl_closure_return_pointer(CL_Closure *self, void *return_ptr)
{
  if (self->return_value.is_copy && self->return_value.value != NULL)
    {
      free(self->return_value.value);
    }
  self->return_value.value = return_ptr;
  self->return_value.is_copy = FALSE;
  return self->return_value.value;
}

void	*cl_closure_get_arg(CL_Closure *self, unsigned int nb)
{
  return self->captured_vars[nb].value;
}

void	*cl_closure_call(CL_Closure *self, size_t nb_args, ...)
{
  void		*return_value;
  va_list	list;
  
  va_start(list, nb_args);
  return_value = (self->function(self, &list));
  va_end(list);
  return return_value;
}

CL_Value	cl_closure_capture(void *value, char is_copy)
{
  CL_Value	capture;
  
  capture.value = value;
  capture.is_copy = is_copy;
  return capture;
}

void	cl_destroy_closure(CL_Closure *self)
{
  unsigned int	i;

  for (i = 0; i < self->nb_captured_vars; ++i)
    {
      if (self->captured_vars[i].is_copy)
	{
	  free(self->captured_vars[i].value);
	}
    }
  if (self->captured_vars != NULL)
    free(self->captured_vars);
  if (self->return_value.is_copy)
    free(self->return_value.value);
  free(self);
}

/* Shortcut return value call */


char	*cl_closure_return_byte(CL_Closure *self, char c)
{
  return cl_closure_return_value(self, &c);
}

short int	*cl_closure_return_short(CL_Closure *self, short int c)
{
  return cl_closure_return_value(self, &c);
}

int		*cl_closure_return_integer(CL_Closure *self, int c)
{
  return cl_closure_return_value(self, &c);
}

long int	*cl_closure_return_long(CL_Closure *self, long int c)
{
  return cl_closure_return_value(self, &c);
}

long long int	*cl_closure_return_long_long(CL_Closure *self, long long int c)
{
  return cl_closure_return_value(self, &c);
}

float		*cl_closure_return_float(CL_Closure *self, float c)
{
  return cl_closure_return_value(self, &c);
}

double		*cl_closure_return_double(CL_Closure *self, double c)
{
  return cl_closure_return_value(self, &c);
}

long double	*cl_closure_return_long_double(CL_Closure *self, long double c)
{
  return cl_closure_return_value(self, &c);
}

