#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct s_block* t_block;
struct s_block { 
    int magic;
    int size;
    t_block next;
    int free;
};

extern void *malloc_simple(int size) ;
extern void *malloc_ffit(int size) ; //first fit malloc implementation
extern void free(void* blk) ;
extern void* init_new_block( int size );
extern t_block get_tail();

int main () { 

    int* val = malloc_ffit(sizeof(int));
    *val = 100;
    printf("val: %d <-- %p\n", *val, val);
    //free(val);
    int* val_2 = malloc_ffit(16);
    *val_2 = 200;
    printf("val: %d <-- %p\n", *val_2, val_2);
    free(val_2);
    int* val_3 = malloc_ffit(sizeof(int));
    *val_3 = 300;
    printf("val: %d <-- %p\n", *val_3, val_3);
   //free(val_2);
}

void *malloc_simple(int size) { 
    void* current = sbrk(0);// current position
    sbrk(size);
    return current;
}

t_block start = NULL;
int allocated = 0;
void *malloc_ffit(int size) { 
    if( allocated == 0 ) {
        void* current = init_new_block(size);
        start = (t_block) current;
        allocated ++;
        return current+(sizeof(struct s_block));
    }
    t_block cur = start;
    int allc = 0;
    do { 
        printf("%d --- %d -- %d\n", cur->free, cur->size, size);
        if(cur->free && cur->size >= size) { 
            allc = 1;
            break;
        }
    }while((cur=cur->next) != NULL);

    if(allc) { 
        cur->free = 0; // unfreeing allocated page
        void* current =  ((void*)cur)+(sizeof(struct s_block));
        return current;
    }

    printf(" no luck finding free blocks in %d allocated...creating new block\n", allocated);
    t_block tail = get_tail();
    tail->next = init_new_block(size);
    allocated ++;
    return ((void*)tail->next)+(sizeof(struct s_block));;
}

void free(void* blk) { 
    t_block bk = blk-(sizeof(struct s_block));
    if(bk->magic == 0xaabb) { 
        bk->free = 1;
    }
}
void* init_new_block( int size );
t_block get_tail();
t_block get_tail() { 
    if(start == NULL || start->next == NULL){ 
        return start;
    }
    t_block cur = start;
    while((cur = cur->next)->next != NULL);
    return cur;
}

void* init_new_block( int size ) { 
    void* current = sbrk(0);
    sbrk(sizeof(struct s_block) + size);
    ((t_block)current)->magic = 0xaabb;
    ((t_block)current)->size = size;
    ((t_block)current)->next = NULL;
    ((t_block)current)->free = 0;
    return current;
}