#ifndef BASIC_DEF_TYPE_HEADER
# define BASIC_DEF_TYPE_HEADER

# include <sys/types.h>

/********************************************************************
 *                        TYPEDEF		                            *
 ********************************************************************/
 /**
  * Basic scalar type typedef
 */
typedef signed char 		t_int8;
typedef signed short		t_int16;
typedef signed int 			t_int32;
typedef signed long         t_int64;
typedef unsigned char 		t_uint8;
typedef unsigned short		t_uint16; 
typedef unsigned int 		t_uint32;
typedef unsigned long 		t_uint64;

typedef struct              timespec t_timespec;

/********************************************************************
 *                        DEFINE		                            *
 ********************************************************************/
/* bool */
# define    FALSE 		    0
# define    TRUE 		    1
/* pointer size */
# define    PTR_SIZE	    sizeof(void *)
/* LS linux color */
# define    RED             "\033[1;31m"
# define    GREEN           "\033[1;32m"
# define    YELLOW          "\033[1;33m"
# define    BLUE            "\033[1;34m"
# define    PURPLE          "\033[1;35m"
# define    CYAN            "\033[1;36m"
# define    RESET           "\033[0m"
# define    FILL_GREEN      "\033[7;32m"
# define    FILL_YELLOW     "\033[7;33m"
# define    FILL_RED        "\033[7;31m"
# define    FILL_YELBLACK   "\033[40;33m"


 # endif /* BASIC_DEF_TYPE_HEADER */