#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <pthread.h>   /* mutex Need to be include first ? */

# include <unistd.h>    /* write */
# include <stddef.h>    /* NULL */
# include <sys/mman.h>  /* mmap */
# include <fcntl.h>     /* open */
# include "basic_define.h"
# include "../libft/libft.h"

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
    TINY:
    4096 / 128 = 32 -> 32 block in one PAGE_SIZE
    32 * 4 = 128 > 100
*/
# define TINY_MULT 4
# define TINY_BLOCK_PER_PAGE    (PAGE_SIZE / TINY_SIZE * TINY_MULT) ///128
/*
    SMALL:
    4096 / 1024 = 4 -> 4 blocks in on PAGE_SIZE
    4 * 26 = 104;
    OLD : 4 * 32 = 128 > 100
*/
// # define SMALL_MULT 32
# define SMALL_MULT 26
# define SMALL_BLOCK_PER_PAGE   (PAGE_SIZE / SMALL_SIZE * SMALL_MULT)
/*  TINY_PAGE_SIZE  = 4096 * 4 = 16324 bytes */
# define TINY_PAGE_SIZE         (size_t)(PAGE_SIZE * TINY_MULT)
/* SMALL_PAGE_SIZE  = 4096 * 32 = 131072 bytes */
# define SMALL_PAGE_SIZE        (size_t)(PAGE_SIZE * SMALL_MULT)

/*
    See print_define in utils
    DATA_SIZE + (100 * TINY_BLOCK_SIZE)
    40 - (16 * 100) = 1640 bytes for metadata:
    - represent 12 tiny block or one small
    On 128 tiny block we case use ~(128 - 12) = 116
    On 104 small block we can use 104 -1 = 103
*/
# define BLOCK_SIZE         sizeof(t_block)     /* 16 bytes struct */
# define DATA_SIZE          (sizeof(t_page) + 8) /* 40 bytes struct, 48 to align block */

/* Aligne value for large block TOCHECK */
// # define ALIGN_VALUE        64
# define ALIGN_VALUE        16

/**/

# define MALLOC_TRACE_ENV   "TRACE_MALLOC"          /* Environement variable for ALLOCATION TRACE */
# define MALLOC_COLOR_ENV   "COLOR_MALLOC"          /* Environement variable for ENABLE COLOR */
// # define MALLOC_LEAKS_ENV   "CHECK_LEAKS_MALLOC"    /* Environement variable for DETECT_LEAK */
/** e_type enum to represent different block with power of 2, store status of env var or special page  */
enum type_block_e {
    TINY=1,                 /*  Tiny size block */
    SMALL=2,                /*  Small size block */
    LARGE=4,                /*  Large size block */
    PRE_ALLOCATE=8,         /*  Pre allocated page for tiny and small block */
    ALLOCATION_TRACE=16,    /*  Enable tracing of all alloc calls, a trace of the various calls is store in the file named by this variable.  */
    ENABLE_COLOR=32,        /*  Enable color for allocation trace option */
    // DETECT_LEAK=64,         /*  Detect leak and store issue in file named by this var or default file/stderr */
};

enum e_free_status {
    BLOCK_ALREADY_FREE=-1,
    BLOCK_FREE_SUCCESS,
    BLOCK_NOT_FOUND,
};

enum e_function_call {
    MALLOC_CALL,
    REALLOC_CALL,
    FREE_CALL,
};

typedef enum type_block_e   e_type;

typedef struct s_block {
    size_t          size;   /* size of desired/wanted data by user */
    struct s_block *next;   /* pointer on next block */
}   t_block;

typedef struct s_page {
    int8_t          type;       /* type of page allocate TINY, SMALL, LARGE, debug storing info */
    int             fd;         /* fd deb file */
    size_t          size;       /* size of total page, multiple of get_page_size */
    size_t          size_free;  /* size free in bytes*/
    struct s_block  *block;     /* t_block linked list */
    struct s_page   *next;      /* pointer on next page */
}   t_page;

/* Global pointer on linked list of page*/
extern t_page *g_data;
/* Global mutex to be thread safe */
extern pthread_mutex_t g_libft_malloc_mutex;

/* Library function */
void        *malloc(size_t size);
void        *realloc(void *ptr, size_t size);
void        free(void *ptr);
void        show_alloc_mem();
/* BONUS */
void        show_alloc_mem_hex();
void        check_for_leak();


/* Free all page. close debug fd if in use and destroy mutex */
void        free_meta_data();

// free.c
void        free_meta_block(t_block* block, t_page *data);
int8_t      page_empty(t_page *block);
void        free_page(t_page *data);

//page_gestion.c
t_block     *init_data(e_type type, size_t size);
t_page      *init_page(e_type type, size_t size, e_type pre_aloc);
e_type      detect_type(size_t size);
size_t      get_page_size(e_type type, size_t size);
void        page_add_back(t_page **lst, t_page *data);

//utils.c
void        display_line(char *str, char to_display);
size_t      get_align_by_type(e_type type);
int         get_lst_block_len(t_block *lst);
// void    free_block();
void        print_define(void);

//block.c
t_block	    *init_block(t_block *block, size_t size, int pos, t_page *data);
size_t      align_mem_block(size_t m_size, size_t size);
t_block     *try_add_block(char type, size_t size);
void        block_add_back(t_block **lst, t_block *block);

/* trace_alloc.c */
void        write_block_info(t_block *block, size_t size, int8_t call, int fd);
void        write_function_name(int8_t call, int fd);

/* handle env.c */
int         handle_env_variable(int8_t *special_flag);
int8_t      check_debug_flag(int8_t flag);
int         get_debug_fd();

/* need to include getenv here for bonus, basicly this definition is in stdilib.h */
extern char *getenv (const char *__name) __THROW __nonnull ((1)) __wur;


/*
The debug malloc library also uses these environment variables:
 *  MALLOC_INITVERBOSE
    	Enable some initial verbose output regarding other variables that are enabled.
 *  MALLOC_BTDEPTH
        The depth of the backtrace for allocations (i.e. where the allocation occurred) 
        on CPUs that support deeper backtrace levels. Currently the builtin-return-address feature of gcc 
        is used to implement deeper backtraces for the debug malloc library. The default value is 0.
 *  MALLOC_TRACEBT
        Set the depth of the backtrace for errors and warnings on CPUs that support deeper backtrace levels.
        Currently the builtin-return-address feature of gcc is used to 
        implement deeper backtraces for the debug malloc library. The default value is 0.
 *  MALLOC_DUMP_LEAKS
        Trigger leak detection on exit of the program.
        The output of the leak detection is sent to the file named by this variable.
 *  MALLOC_TRACE
        Enable tracing of all calls to malloc(), free(), calloc(), realloc(), etc.
        A trace of the various calls is store in the file named by this variable.
 *  MALLOC_CKACCESS_LEVEL
        Specify the level of checking performed by the MALLOC_CKACCESS option to mallopt().
*/

/*
    RSC:

    https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_3.html            MALLOC ENV VAR
    https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_13.html#SEC246    MMAP
    'MAP_ANON:
This flag tells the system to create an anonymous mapping, not connected to a file. filedes and off are ignored, and the region is initialized with zeros.
Anonymous maps are used as the basic primitive to extend the heap on some systems. They are also useful to share data between multiple tasks without creating a file.
On some systems using private anonymous mmaps is more efficient than using malloc for large blocks.
This is not an issue with the GNU C library, as the included malloc automatically uses mmap where appropriate.'
*/

# endif /* FT_MALLOC_H */
