#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/*
Initialise une liste
Les 2 fonctions correspondent aux fonctions de 
* destruction de la data contenue dans lelement de la liste, ex:free
* comparaison de la data contenue dans lelement de la liste
    cette fonction devra renvoyer 0 en cas degalite et 1 sinon
*/
list * list_init(int(*free_data)(void **data),
        int(*compare_data)(void *d1, void *d2)){
    list *l = NULL;
    
    l = calloc(1, sizeof(list));
    if(l==NULL) return l;
    
    l->size         = 0;
    l->first        = NULL;
    l->last         = NULL;
    l->free_data    = free_data;
    l->compare_data = compare_data;
    return l;
}



int list_destroy(list **l){
    int i=0, max;
    
    if (*l==NULL) return 0;
    
    max = (*l)->size;
    
    for(i=0;i<max;i++){
        if(list_lpop(*l)==-1){
            return -1;
        }
    }
    
    free(*l);
    
    return 0;
}


/*
* Verifie si un element existe deja dans une liste
* on utilise la fonction de comparaison fournie
* lors de l'initialisation de la liste
*/
int list_element_exist(list *l,void *data){
    /*
    * 0 -> data exist
    * 1 -> data not exist
    */
    element *actual = NULL;
    
    if ( l->size==0 ) return 1;
    actual = l->first;
    
    do{
        if(l->compare_data(data,actual->data)==0) return 0;
        actual = actual->next;
    }while(actual != NULL);
    
    return 1;
}

/*
* Pousse une nouvelle data en debut de liste
* Si la data existe deja, elle n'est pas ajoute
* Valeur de retour:
*    $ 0 -> OK
*    $ -1 -> erreur lors de l'ajout
*    $ 1 -> data deja presente
*/
int list_uniq_lpush(list *l, void *data){
    if(list_element_exist(l,data)==1){
        return list_lpush(l,data);        
    }
    return 1;
}

int list_uniq_rpush(list *l, void *data){
    if(list_element_exist(l,data)==1){
        return list_rpush(l,data);        
    }
    return 1;
}


int list_lpush(list *l, void *data){
    element *el;
    el = element_init();
    
    if(el==NULL) return -1;
    l->size += 1;
    el->next = l->first;
    
    if ( l->first != NULL){
        l->first->previous = el;    
    } 
    else{
        l->last = el;
    }
    l->first = el;
    el->data = data;
    
    return 0;
}

int list_rpush(list *l, void *data){
    element *el;
    el = element_init();
    
    if (el==NULL) return -1;
    l->size += 1;
    el->previous = l->last;
    
    if (l->last !=NULL){
        l->last->next = el;    
    }
    else{
        l->first = el;
    }
    
    l->last = el;
    el->data = data;
    
    return 0;
}


/*
* Supprime la data en debut de liste
*    * 0 -> OK
*    * -1 -> erreur lors de la suppresion
*    * 1 -> liste vide
*/
int list_lpop(list *l){
    element *tmp = NULL;
    
    if(l->size==0) return 1;
    
    tmp = l->first->next;
    if( element_destroy(&(l->first), l->free_data) ==-1) return -1;
    l->first = tmp;
    
    if(l->first != NULL) l->first->previous = NULL;
    return 0;
}

int list_rpop(list *l){
    element *tmp = NULL;
    
    if(l->size==0) return 1;
    
    tmp = l->last->previous;
    if( element_destroy(&(l->last), l->free_data) ==-1) return -1;
    l->last = tmp;
    l->last->next = NULL;
    return 0;
}


element* element_init(){
    element *el = NULL;
    
    el = calloc(1, sizeof(element));
    if(el==NULL) return el;
    
    el->data        = NULL;
    el->next        = NULL;
    el->previous    = NULL;
    return el;
}


int element_destroy(element **el, int(*fct)()){
    if (*el == NULL) return -1;
    
    if ( (*el)->data!=NULL ){
        if( fct(&((*el)->data)) < 1) return -1;
    }
    
    if ( (*el)->previous != NULL ){
        (*el)->previous->next = (*el)->next;
    }
    
    if( (*el)->next != NULL){
        (*el)->next->previous = (*el)->previous;
    }
    free(*el);
    
    return 0;   
}

element* list_get_element_by_data(list *l, void *data){
    element *actual = NULL;
    
    if((l==NULL)||(l->size==0)) return NULL;
    actual = l->first;
    
    do{
        if(l->compare_data(data,actual->data)==0) return actual;
        actual = actual->next;
    }while(actual != NULL);
    
    return NULL;
}

/*
Recupere la position de la data dans la liste
* -2  -> data non presente
* -1  -> liste vide
* >=0 -> position dans la liste
*/
int element_get_position_inlist(list *l, void *data){
    element *actual = NULL;
    int position = 0;
    
    if(l->size==0) return -1;
    actual = l->first;
    
    do{
        if(l->compare_data(data,actual->data)==0) return position;
        actual = actual->next;
        position++;
    }while(actual != NULL);
    
    return -2;
}

int list_remove_element_byposition(list *l, int position){
    element *actual = NULL;
    int current_position = 0, objectif=0;
    
    objectif = position % l->size;
    actual = l->first;
    
    while(current_position<objectif){
        actual = actual->next;
    }
    
    return element_destroy(&actual, l->free_data);
}

