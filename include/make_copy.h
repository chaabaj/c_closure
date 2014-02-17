#ifndef MAKE_COPY_H
# define MAKE_COPY_H

char		*cl_copy_byte(char);
short int	*cl_copy_short(short int);
int		*cl_copy_integer(int);
long int	*cl_copy_long(long int);
long long int	*cl_copy_long_long(long long int);
float		*cl_copy_float(float);
double		*cl_copy_double(double);
long double	*cl_copy_long_double(long double);
void		*cl_copy_value(void*, size_t type);

#endif