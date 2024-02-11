#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <pthread.h>   /* mutex Need to be include first ? */

# include <mcheck.h>

# include <unistd.h>    /* write */
# include <stddef.h>    /* NULL */
# include <sys/mman.h>  /* mmap */
# include "../libft/libft.h"
# include "../libft/list/linked_list.h" /* maybe not mandatory, to check */

/* Return of getpagesize function casted in size_t */
# define PAGE_SIZE              (size_t)getpagesize()
/* TINY block size */
# define TINY_SIZE              (size_t)128
/* SMALL block size */
# define SMALL_SIZE             (size_t)1024

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
    100 * (TINY_BLOCK_SIZE + DATA_SIZE) / TINY_SIZE = 6, 1 for small
    We can use ~128 - (6 * 0.28) block in TINY page, 128 - 9 = 119;
    102 block in SMALL, keep 2 for metadata
    OLD 126 block in SMALL, just keep 2 for metadata
*/
# define BLOCK_SIZE         sizeof(t_block)
# define DATA_SIZE          sizeof(t_page)
/* Aligne value for large block to check */
# define ALIGN_VALUE        64

/** e_type enum to represent different block with power of 2, store status of env var or special page too  */
enum e__type {
    TINY=1,
    SMALL=2,
    LARGE=4,
    PRE_ALLOCATE=8,         /*  Pre allocated page for tiny and small block */
    SAVE_MALLOC_CALL=16,    /*  Save malloc call if env variable is enable */
    DEBUG_ALLOC_FLAG=32,    /*  Create output file with alloc data: size, trace leaks, unitialised data  ... */
};

enum e_free_status {
    BLOCK_ALREADY_FREE=-1,
    BLOCK_FREE_SUCCESS,
    BLOCK_NOT_FOUND,
};

typedef enum e__type    e_type;

/*
    block->size :ALIGNED(size of block require by user + sizeof(t_block))
    block->next : pointer to next block struct
    when free block dont't remove node, just block->size = -1 and check it for refill
*/
typedef struct s_block {
    size_t          size;   /* size of desired/wanted data by user */
    struct s_block *next;   /* pointer on next block */
}   t_block;

/*
    
    page->size :
    page->size_free : size of page free in bytes
    page->block : pointer of linked list block of same type 
    page->next : pointer to next page struct
*/

typedef struct s_page {
    e_type          type;       /* type of page allocate TINY, SMALL, LARGE, debug storing info */
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
void    show_alloc_mem();
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    free(void *ptr);

//free.c
void    free_meta_block(t_block* block, t_page *data);
int8_t  page_empty(t_page *block);
void    free_page(t_page *data);
void    free_meta_data();

//page_gestion.c
t_block *init_data(e_type type, size_t size);
t_page  *init_page(e_type type, size_t size, e_type pre_aloc);
e_type  detect_type(size_t size);
size_t  get_page_size(e_type type, size_t size);
void    page_add_back(t_page **lst, t_page *data);

//utils.c
void    display_line(char *str, char to_display);
size_t  get_align_by_type(e_type type);
int     get_lst_block_len(t_block *lst);
// void    free_block();
void    print_define(void);

//block.c
t_block	*init_block(t_block *block, size_t size, int pos, t_page *data);
size_t align_mem_block(size_t m_size, size_t size);
t_block *try_add_block(char type, size_t size);
void    block_add_back(t_block **lst, t_block *block);

// ft_printf.c
// int	ft_printf_fd(int fd, const char *s, ...);


/* show mem format
    TINY : 0xA0000
    0xA0020 - 0xA004A : 42 bytes
    0xA006A - 0xA00BE : 84 bytes
    SMALL : 0xAD000
    0xAD020 - 0xADEAD : 3725 bytes
    LARGE : 0xB0000
    0xB0020 - 0xBBEEF : 48847 bytes
    Total : 52698 bytes
*/  

/*
Debugging
Exemple de bonus:
- Lors d'un free, le projet "défragmente" la mémoire libre en regroupant les blocs libres concomitants en un seul
- Malloc possède des variables d'environnement de debug
- Une fonction permet de faire un dump hexa des zones allouées
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
