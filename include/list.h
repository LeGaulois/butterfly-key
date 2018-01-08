#ifndef LIST
#define LIST


typedef struct element element;
typedef struct list list;


struct element{
    element   *previous;
    element   *next;
    void      *data;    
};

struct list {
    int     size;
    element *first;
    element *last;
    int     (*free_data)(void **data);
    int     (*compare_data)(void *d1, void *d2);
};


list * list_init(int(*free_data)(void **data),int(*compare_data)(void *d1, void *d2));
int list_destroy(list **l);
int list_element_exist(list *l,void *data);
int list_uniq_lpush(list *l, void *data);
int list_uniq_rpush(list *l, void *data);
int list_lpush(list *l, void *data);
int list_rpush(list *l, void *data);
int list_lpop(list *l);
int list_rpop(list *l);
element* element_init();
int element_destroy(element **el, int(*fct)());
element* list_get_element_by_data(list *l, void *data);
int element_get_position_inlist(list *l, void *data);
int list_remove_element_byposition(list *l, int position);

#endif
