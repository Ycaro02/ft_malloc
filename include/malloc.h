#ifndef MALLOC_H
# define MALLOC_H

#include <unistd.h>
#include <stddef.h> 
#include "ft_printf.h"
#include <sys/mman.h> //mmap
//
#include <stdio.h>
#include <stdlib.h>
//
// 4096 / 256 = 16
// AKA PAGE_SIZE / TINY_SIZE
// 16 * 8 > 100;
// AKA (PAGE_SIZE / TINY_SIZE) * x > 100

// 4096 / 1024 = 4

#define TINY_MULT 8
#define SMALL_MULT 32

# define PAGE_SIZE              (size_t)getpagesize()
# define TINY_SIZE              (size_t)256
# define SMALL_SIZE             (size_t)1024
# define TINY_PAGE_SIZE         (size_t)(PAGE_SIZE * 8)
# define SMALL_PAGE_SIZE        (size_t)(PAGE_SIZE * 32)

//
# define TINY_BLOCK_PER_PAGE    (PAGE_SIZE / TINY_SIZE * TINY_MULT) ///128
# define SMALL_BLOCK_PER_PAGE   (PAGE_SIZE / SMALL_SIZE * SMALL_MULT)

// see print_define in utils
// 100 * (TINY_BLOCK_SIZE + DATA_SIZE) / TINY_SIZE = 6, 1 for small 

# define BLOCK_SIZE          sizeof(t_block)
# define DATA_SIZE           sizeof(t_data)
# define ALIGN_VALUE            64

enum e__type {
    TINY=1,
    SMALL=2,
    LARGE=4,
};

enum e__bool {
    TRUE,
    FALSE,
};

// block->size :ALIGNED(size of block require by user + sizeof(t_block))
// block->next : pointer to next block struct

// when free block dont't remove node, just block->size = -1 and check it for refill

typedef struct s_block {
    size_t          size;
    struct s_block *next;
}   t_block;

// data->type : type of page allocate TINY, SMALL, LARGE
// data->size : ALIGNED(size of total page)
// data->size_free : size of data free in bytes
// data->block : pointer of linked list block of same type 
// data->next : pointer to next data struct

typedef struct s_data {
    char            type;
    size_t          size;
    size_t          size_free;
    struct s_block  *block;
    struct s_data   *next;
}   t_data;

typedef enum e__type e_type;
typedef enum e__bool e_bool;

extern t_data *g_data;

// void    ffree(void *ptr);
// void    *fmalloc(size_t size);
// void    *ft_realloc(void *ptr, size_t size);
void    show_alloc_mem();

//page_gestion.c
t_block *init_data(e_type type, size_t size);
e_type  detect_type(size_t size);
size_t  get_page_size(e_type type, size_t size);

// t_block *check_add_page(e_type type, size_t size);
size_t  get_page_size(e_type type, size_t size);


//utils.c
size_t get_align_by_type(e_type type);
int     get_lst_block_len(t_block *lst);
void    free_block();
void    data_add_back(t_data **lst, t_data *data);
void    block_add_back(t_block **lst, t_block *block);
void    print_define(void);
void    display_line(char *str, char to_display);
size_t  ft_strlen(char *str);

//block.c
t_block	*init_block(t_block *block, size_t size, e_type type, int pos, t_data *data);
size_t align_mem_block(size_t m_size, size_t size);
t_block *try_add_block(char type, size_t size);

//show_alloc_mem.c
void show_alloc_mem();

// malloc.c
void    *malloc(size_t size);
void    free(void* ptr);
void    free_meta_data();

// ft_printf.c
int	ft_printf_fd(int fd, const char *s, ...);


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


# endif /* MALLOC_H */
