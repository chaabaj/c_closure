#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "array.h"
#include "make_copy.h"
#include "types.h"
#include "linked_list.h"
#include "algorithm.h"
#include "closure_algorithm.h"
#include "closure.h"

static void	*filter_predicate_test(CL_Closure *self, va_list *args)
{
  int		*value;

  value = va_arg(*args, int*);
  if (*value % 2 == 0)
    return cl_closure_return_byte(self, FALSE);
  return cl_closure_return_byte(self, TRUE);
}

static void	show_each_value(void *value)
{
  printf("%d\n", *(int*)value);
}

static void	*transform_test(CL_Closure *self, va_list *args)
{
  int		*value;
  int		index;
  int		result;
  
  value = va_arg(*args, int*);
  index = va_arg(*args, int);
  result = *value + index;
  return cl_closure_return_value(self, &result);
}

static void	test_list()
{
  CL_Collection	*list;

  list = cl_make_linked_list(sizeof(int), free);
  list->push_back(list, cl_copy_integer(5));
  list->push_back(list, cl_copy_integer(10));
  list->push_back(list, cl_copy_integer(60));
  list->pop_back(list);

  puts("============PRINT COLLECTION=============");
  cl_for_each((CL_Iterable*)list, show_each_value);
  puts("=========================================");
  printf("%d\n", *(int*)list->get_data(list, 1));
  cl_destroy_linked_list((CL_List*)list);
}

static void	test_array()
{
  CL_Closure	*filter_closure;
  CL_Closure	*transform_closure;
  CL_Collection	*transformed_collection;
  CL_Collection	*collection;
  CL_Collection	*filtered_collection;
  
  filter_closure = cl_make_closure(filter_predicate_test, sizeof(bool), 0);
  transform_closure = cl_make_closure(transform_test, sizeof(char*), 0);
  collection = cl_closure_range(0, 10, 1, cl_make_array);

  puts("===============PRINT COLLECION=====================");
  cl_for_each((CL_Iterable*)collection, show_each_value);
  puts("===================================================");

  filtered_collection = cl_closure_filter(collection,filter_closure);
  puts("===============PRINT FILTERED COLLECTION===========");
  cl_for_each((CL_Iterable*)filtered_collection, show_each_value);
  puts("===================================================");

  transformed_collection = cl_closure_map(collection,
				       cl_make_array(sizeof(int), free),
				       transform_closure);
  puts("===============PRINT TRANSFORM COLLECTION==========");
  cl_for_each((CL_Iterable*)transformed_collection, show_each_value);
  puts("===================================================");

  cl_destroy_closure(filter_closure);
  cl_destroy_closure(transform_closure);
  cl_destroy_array((CL_Array*)collection);
  cl_destroy_array((CL_Array*)filtered_collection);
  cl_destroy_array((CL_Array*)transformed_collection);
}

int main()
{
  puts("=============TEST LINKED LIST==============");
  test_list();
  puts("===========================================");

  puts("=============TEST ARRAY====================");
  test_array();
  puts("===========================================");
  return 0;
}
