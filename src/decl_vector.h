#ifndef DECL_VECTOR_H
 # define DECL_VECTOR_H
 
 extern void perror(char const*);
 
 # define DECL_VECTOR(NAME,TYPE,SIZE)                     \
    PUN_STRUCT(NAME,TYPE,SIZE);                           \
    INIT_STRUCT(NAME,TYPE,SIZE) 
 
 # define PUN_STRUCT(NAME,TYPE,SIZE)                      \
    struct {                                              \
       TYPE (*p)[(SIZE)];                                 \
       int size;                                          \
       int capacity;                                      \
    } *NAME = malloc(sizeof(TYPE(*)) + sizeof(int)*2)                        
 
 # define INIT_STRUCT(NAME,TYPE,SIZE)  do {               \
       if (!NAME) {                                       \
            perror("malloc fail");                        \
            abort();                                      \
       }else {                                            \
            NAME->size = (SIZE);                          \
            NAME->capacity = 0;                           \
            if (!(NAME->p = malloc(sizeof(*NAME->p)))) {  \
                perror("malloc fail");                    \
                abort();                                  \
            }                                             \
            NAME->p = (TYPE(*)[(SIZE)])(NAME->p);         \
       }                                                  \
    }while(0)

#endif  /* DECL_VECTOR_H */

/*
**
**  int main(int argc, char *argv[]) 
**  {
**     
**    DECL_VECTOR(vec1,int,8);
**     
**     
**     printf("size of handle struct:  %zu,\n\
**  ize of vector array:   %zu,\n\
**  ize of vector element: %zu\n\
**  ddress of handle struct: %p,\n\
**  ddress of vector array:  %p\n",  sizeof(*vec1),       \
**                                   sizeof(*vec1->p),    \
**                                   sizeof(*vec1->p[0]), \
**                                   vec1,                \
**                                   vec1->p);
**    free(vec1->p);
**    free(vec1);
**   
**    return 0;
**  }
**
*/
