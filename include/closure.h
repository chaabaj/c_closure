#ifndef CLOSURE_H
#define CLOSURE_H

# include <stddef.h>
# include <stdarg.h>

typedef struct s_cl_closure*	_CL_Closure;

typedef void	*(*CL_Function)(_CL_Closure, va_list*);

struct s_cl_value
{
  void	*value;
  int	is_copy;
};

typedef struct s_cl_value	CL_Value;

struct s_cl_closure
{
  CL_Value	*captured_vars;
  size_t	nb_captured_vars;
  size_t	return_type;
  CL_Function	function;
  CL_Value	return_value;
};

typedef struct s_cl_closure	CL_Closure;


CL_Closure	*cl_make_closure(CL_Function, size_t return_type, 
			      size_t nb_captured_vars, ...);
void		*cl_closure_return_value(CL_Closure*, void *);
void		*cl_closure_return_pointer(CL_Closure*, void*);
CL_Value 	cl_closure_capture(void*, char);
void		*cl_closure_call(CL_Closure*, size_t nb_args, ...);
void		cl_destroy_closure(CL_Closure*);
void		*cl_closure_get_arg(CL_Closure*, unsigned int nb);

/* shortcut closure return value for scalar type */

char		*cl_closure_return_byte(CL_Closure*, char);
short int	*cl_closure_return_short(CL_Closure*, short int);
int		*cl_closure_return_integer(CL_Closure*, int);
long int	*cl_closure_return_long(CL_Closure*, long int);
long long int	*cl_closure_return_long_long(CL_Closure*, long long int);
float		*cl_closure_return_float(CL_Closure*, float);
double		*cl_closure_return_double(CL_Closure*, double);
long double	*cl_closure_return_long_double(CL_Closure*, long double);

#endif