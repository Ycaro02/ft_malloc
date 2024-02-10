#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <unistd.h>
#include <stddef.h> 
#include <sys/mman.h> //mmap
#include "../libft/libft.h"
#include "../libft/list/linked_list.h"



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
# define DATA_SIZE          sizeof(t_data)
/* Aligne value for large block to check */
# define ALIGN_VALUE        64

/** e_type enum to represent different block with power of 2 */
enum e__type {
    TINY=1,
    SMALL=2,
    LARGE=4,
    PRE_ALLOCATE=8,
};

/** e_event enum used in free function to know what to do ? */
enum e__event {
    NONE,
    FREE_ALL,
    PARTIAL_FREE,
};

/*
    block->size :ALIGNED(size of block require by user + sizeof(t_block))
    block->next : pointer to next block struct
    when free block dont't remove node, just block->size = -1 and check it for refill
*/
typedef struct s_block {
    size_t          size;
    struct s_block *next;
}   t_block;


/*
    data->type : type of page allocate TINY, SMALL, LARGE
    data->size : ALIGNED(size of total page)
    data->size_free : size of data free in bytes
    data->block : pointer of linked list block of same type 
    data->next : pointer to next data struct
*/

typedef struct s_data {
    char            type;
    size_t          size;
    size_t          size_free;
    struct s_block  *block;
    struct s_data   *next;
}   t_data;

typedef enum e__type    e_type;
typedef enum e__bool    e_bool;
typedef enum e__event   e_event;

/* Global pointer on linked list of page*/
extern t_data *g_data;

//show_alloc_mem.c
void    show_alloc_mem();

// malloc.c
void    *malloc(size_t size);

// free.c
void    free(void *ptr);
void    free_meta_block(t_block* block, t_data *data);
int8_t  page_empty(t_data *block);
void    free_page(t_data *data);
//
void    free_meta_data();

//realloc.c
void    *realloc(void *ptr, size_t size);

//page_gestion.c
t_block *init_data(e_type type, size_t size);
e_type  detect_type(size_t size);
size_t  get_page_size(e_type type, size_t size);
t_data *alloc_first_page(e_type type, size_t block_size, size_t page_size);
void    data_add_back(t_data **lst, t_data *data);

//utils.c
void    display_line(char *str, char to_display);
size_t  get_align_by_type(e_type type);
int     get_lst_block_len(t_block *lst);
// void    free_block();
void    print_define(void);

//block.c
t_block	*init_block(t_block *block, size_t size, e_type type, int pos, t_data *data);
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


# endif /* FT_MALLOC_H */
