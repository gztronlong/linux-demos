
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>


int test_list() {
    LIST_HEAD(listhead, entry) head;
    //struct listhead *headp;                 /* List head. */
    struct entry {
        char name[64];
        char sexy[8];
        int age;
        LIST_ENTRY(entry) entries;          /* List. */
    } *n1, *n2, *np;


    LIST_INIT(&head);                       /* Initialize the list. */


    n1 = (struct entry *)malloc(sizeof(struct entry));      /* Insert at the head. */
    strcpy(n1->name,"Tom");
    strcpy(n1->sexy,"boy");
    n1->age = 25;
    LIST_INSERT_HEAD(&head, n1, entries);


    n2 = (struct entry *)malloc(sizeof(struct entry));      /* Insert after. */
    strcpy(n2->name,"Hameimei");
    strcpy(n2->sexy,"girl");
    n2->age = 23;
    LIST_INSERT_AFTER(n1, n2, entries);
    /* Forward traversal. */
    for (np = head.lh_first; np != NULL; np = np->entries.le_next)
    {
        printf("%s %s %d\n",np->name,np->sexy,np->age);
    }


    while (head.lh_first != NULL)           /* Delete. */
        LIST_REMOVE(head.lh_first, entries);
    return 0;
}

int test_queue() {
    TAILQ_HEAD(tailhead, entry) head;
    //struct tailhead *headp;                 /* Tail queue head. */
    struct entry {
        char name[64];
        char sexy[8];
        int age;
        TAILQ_ENTRY(entry) entries;         /* Tail queue. */
    } *n1, *n2, *n3,*np;


    TAILQ_INIT(&head);                      /* Initialize the queue. */


    n1 = (struct entry *)malloc(sizeof(struct entry));      /* Insert at the head. */
    strcpy(n1->name,"Tom");
    strcpy(n1->sexy,"boy");
    n1->age = 25;
    TAILQ_INSERT_HEAD(&head, n1, entries);


    n2 = (struct entry *)malloc(sizeof(struct entry));      /* Insert at the tail. */
    strcpy(n2->name,"Jim");
    strcpy(n2->sexy,"boy");
    n2->age = 24;
    TAILQ_INSERT_TAIL(&head, n2, entries);


    n3 = (struct entry *)malloc(sizeof(struct entry));      /* Insert after. */
    strcpy(n3->name,"Hameimei");
    strcpy(n3->sexy,"girl");
    n3->age = 22;
    TAILQ_INSERT_AFTER(&head, n1, n3, entries);


    /* Forward traversal. */
    for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next)
        printf("%s %s %d\n",np->name,np->sexy,np->age);
    /* Delete. */
    while (head.tqh_first != NULL)
        TAILQ_REMOVE(&head, head.tqh_first, entries);


    return 0;
}

int test_circular() {
    CIRCLEQ_HEAD(circleq, entry) head;
    //struct circleq *headp;              /* Circular queue head. */
    struct entry {
        char name[64];
        char sexy[8];
        int age;
        CIRCLEQ_ENTRY(entry) entries;   /* Circular queue. */
    } *n1, *n2, *n3,*n4,*np;

    CIRCLEQ_INIT(&head);                /* Initialize the circular queue. */

    n1 = (struct entry *)malloc(sizeof(struct entry));  /* Insert at the head. */
    strcpy(n1->name,"Tom");
    strcpy(n1->sexy,"boy");
    n1->age = 25;
    CIRCLEQ_INSERT_HEAD(&head, n1, entries);

    n2 = (struct entry *)malloc(sizeof(struct entry));  /* Insert at the tail. */
    strcpy(n2->name,"Jim");
    strcpy(n2->sexy,"boy");
    n2->age = 24;
    CIRCLEQ_INSERT_TAIL(&head, n2, entries);

    n3 = (struct entry *)malloc(sizeof(struct entry));  /* Insert after. */
    strcpy(n3->name,"Hameimei");
    strcpy(n3->sexy,"girl");
    n3->age = 22;
    CIRCLEQ_INSERT_AFTER(&head, n1, n3, entries);

    n4 = (struct entry *)malloc(sizeof(struct entry));  /* Insert before. */
    strcpy(n4->name,"Lilei");
    strcpy(n4->sexy,"boy");
    n4->age = 30;
    CIRCLEQ_INSERT_BEFORE(&head, n1, n4, entries);

    /* Forward traversal. */
    printf("Forward traversal.\n");
    for (np = head.cqh_first; np != (void *)&head; np = np->entries.cqe_next)
        printf("%s %s %d\n",np->name,np->sexy,np->age);

    /* Reverse traversal. */
    printf("Reverse traversal.\n");
    for (np = head.cqh_last; np != (void *)&head; np = np->entries.cqe_prev)
        printf("%s %s %d\n",np->name,np->sexy,np->age);

    /* Delete. */
    while (head.cqh_first != (void *)&head)
        CIRCLEQ_REMOVE(&head, head.cqh_first, entries);

    return 0;
}

int main(int argc, char *argv[]) {
    printf("Test list...\n");
    test_list();
    printf("________________________\n");

    printf("Test queue...\n");
    test_queue();
    printf("________________________\n");

    printf("Test circular...\n");
    test_circular();
    printf("________________________\n");

    return 0;
}
