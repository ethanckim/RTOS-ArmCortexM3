#ifndef __LLST_H__
#define __LLST_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct osThreadNode_t {
	struct osThread_t *data;
	int id;
	struct osThreadNode_t *next;
}osThreadNode_t;

typedef struct osThreadLLst_t {
	struct osThreadNode_t *head;
	int size;
} osThreadLLst_t;

void llst_init( osThreadLLst_t * );
void llst_destroy( osThreadLLst_t * );
uint8_t llst_size( osThreadLLst_t * );

bool llst_insertFirst( osThreadLLst_t *, struct osThread_t *thread_data);
bool llst_insertLast( osThreadLLst_t * , struct osThread_t *thread_data);
bool llst_insert_node( osThreadLLst_t *, struct osThread_t *thread_data, int id);
bool list_deleteFirst( osThreadLLst_t * );
bool list_deleteLast( osThreadLLst_t *);
bool llst_delete_node( osThreadLLst_t *, int id);
struct osThread_t* llst_get_node( osThreadLLst_t *, int id);

#endif
