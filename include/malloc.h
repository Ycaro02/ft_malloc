#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <pthread.h>           /* mutex */
# include <unistd.h>            /* write */
# include <stddef.h>            /* NULL */
# include <sys/mman.h>          /* mmap */
# include <fcntl.h>             /* open */
# include <sys/types.h>         /* int16_t */
# include "basic_define.h"      /* Color ... */
# include "../libft/libft.h"    /* Libft without stdlib */



/********************************************************************
 *                        DEFINE		                            *
 ********************************************************************/

/* Return of getpagesize function casted in size_t 4096 */
# define PAGE_SIZE              (size_t)getpagesize()

/* TINY block size */
# define TINY_SIZE              (size_t)128

/* SMALL block size */
# define SMALL_SIZE             (size_t)1024

# define TINY_NAME              "TINY"
# define SMALL_NAME             "SMALL"
# define LARGE_NAME             "LARGE"

/*
 * TINY:
 * 4096 / 128 = 32 -> 32 block in one PAGE_SIZE
 * 32 * 4 = 128 > 100
*/
# define TINY_MULT 4
# define TINY_BLOCK_PER_PAGE    (PAGE_SIZE / TINY_SIZE * TINY_MULT) ///128

/*
 * SMALL:
 * 4096 / 1024 = 4 -> 4 blocks in on PAGE_SIZE
 * 4 * 26 = 104;
*/
# define SMALL_MULT 26
# define SMALL_BLOCK_PER_PAGE   (PAGE_SIZE / SMALL_SIZE * SMALL_MULT)

/*  TINY_PAGE_SIZE  = 4096 * 4 = 16324 bytes */
# define TINY_PAGE_SIZE         (size_t)(PAGE_SIZE * TINY_MULT)

/* SMALL_PAGE_SIZE  = 4096 * 32 = 131072 bytes */
# define SMALL_PAGE_SIZE        (size_t)(PAGE_SIZE * SMALL_MULT)

/*
 * See print_define in utils
 * DATA_SIZE + (100 * TINY_BLOCK_SIZE)
 * 40 - (16 * 100) = 1640 bytes for metadata:
 * Represent 12 tiny block or one small
 * On 128 tiny block we can use ~(128 - 12) = 116
 * On 104 small block we can use 104 -1 = 103
*/
# define BLOCK_SIZE         sizeof(t_block)         /* 16 bytes struct */
# define DATA_SIZE          (sizeof(t_page) + 8)    /* 40 bytes struct, 48 to align block */

/* Aligne value */
# define ALIGN_VALUE        16

/*  
    * This variable represent the path of the debug file, the option will not
    * be enable if the given path can't be open or create
 */
# define MALLOC_TRACE_ENV   "TRACE_MALLOC"          /* Environement variable for ALLOCATION TRACE */

/* Basic gestion just need to had to value te enable */
# define MALLOC_COLOR_ENV   "COLOR_MALLOC"          /* Environement variable for ENABLE COLOR */
# define MALLOC_LEAKS_ENV   "CHECK_LEAKS_MALLOC"    /* Environement variable for DETECT_LEAK */
# define MALLOC_GARBAGE_ENV "GARBAGE_FREE_MALLOC"   /* Environement variable for GARBAGE_COLLECTOR_FREE */


/********************************************************************
 *                        ENUM  		                            *
 ********************************************************************/
/** 
 * e_type enum to represent different block type with power of 2
 * store status of env var or special page  
*/
enum type_block_e {
    TINY=1,                     /*  Tiny size block */
    SMALL=2,                    /*  Small size block */
    LARGE=4,                    /*  Large size block */
    PRE_ALLOCATE=8,             /*  Pre allocated page for tiny and small block */
    ALLOCATION_TRACE=16,        /*  Enable tracing of all alloc/mmap/munmap calls  */
    ENABLE_COLOR=32,            /*  Enable color for allocation trace option */
    DETECT_LEAK=64,             /*  Detect leak output issue on stderr, or the given fd  */
    GARBAGE_COLLECTOR_FREE=128, /* Free all page at the end of program */
};


# define SANITIZE_TYPE          (PRE_ALLOCATE + ALLOCATION_TRACE + ENABLE_COLOR + DETECT_LEAK + GARBAGE_COLLECTOR_FREE)
typedef enum type_block_e   e_type;

/**
 * Free block status
*/
enum e_free_status {
    BLOCK_ALREADY_FREE=-1,
    BLOCK_FREE_SUCCESS,
    BLOCK_NOT_FOUND,
};

/**
 * Enum for display block
*/
enum e_function_call {
    MALLOC_CALL,
    REALLOC_CALL,
    FREE_CALL,
};

/********************************************************************
 *                        STRUCT		                            *
 ********************************************************************/
typedef struct s_block {
    size_t          size;   /* size of desired/wanted data by user */
    struct s_block *next;   /* pointer on next block */
}   t_block;

typedef struct s_page {
    int16_t          type;       /* type of page allocate TINY, SMALL, LARGE, debug storing info */
    int             fd;         /* fd deb file */
    size_t          size;       /* size of total page, multiple of get_page_size */
    size_t          size_free;  /* size free in bytes*/
    struct s_block  *block;     /* t_block linked list */
    struct s_page   *next;      /* pointer on next page */
}   t_page;

/********************************************************************
 *                        GLOBAL		                            *
 ********************************************************************/

/* Global pointer on linked list of page*/
extern t_page           *g_data;

/* Global mutex to be thread safe */
extern pthread_mutex_t  g_malloc_mutex;

/********************************************************************
 *                        Library function                          *
 ********************************************************************/
void        *malloc(size_t size);
void        *realloc(void *ptr, size_t size);
void        free(void *ptr);
void        show_alloc_mem();

/********************************************************************
 *                        Bonus function                            *
 ********************************************************************/

/* Hexa dump of memory allocated */
void        show_alloc_mem_hex();
/* Search for no free block */
void        check_for_leak();
/* Free all page function, close debug fd too */
void        free_meta_data();

/********************************************************************
 *                        Private lib function                      *
 ********************************************************************/

/* free.c */
void        free_meta_block(t_block* block, t_page *data);
int16_t      page_empty(t_page *block);
void        free_page(t_page *data);

/* page_gestion.c */
t_block     *init_data(e_type type, size_t size);
t_page      *init_page(e_type type, size_t size, e_type pre_aloc);
e_type      detect_type(size_t size);
size_t      get_page_size(e_type type, size_t size);
void        page_add_back(t_page **lst, t_page *data);

/* utils.c */
void        display_line(char *str, char to_display);
size_t      get_align_by_type(e_type type);
int         get_lst_block_len(t_block *lst);
void        print_define(void);

/* block.c */
t_block	    *init_block(t_block *block, size_t size, int pos, t_page *data);
size_t      align_mem_block(size_t m_size, size_t size);
t_block     *try_add_block(char type, size_t size);
void        block_add_back(t_block **lst, t_block *block);

/* trace_alloc.c */
void        write_block_info(t_block *block, size_t size, int16_t call, int fd);
void        write_function_name(int16_t call, int fd);

/* handle env.c */
int         handle_env_variable(int16_t *special_flag);
int16_t      check_debug_flag(int16_t flag);
int         get_debug_fd();

/* need to declare getenv for bonus, basicly this declaration is in stdlib.h */
extern char *getenv (const char *__name) __THROW __nonnull ((1)) __wur;
/* same for atexit */
extern int  atexit(void(*function)(void));

# endif /* FT_MALLOC_H */
