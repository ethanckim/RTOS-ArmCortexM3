#include <stdio.h>
#include "linkedlist.h"


void llst_init(osThreadLLst_t *llst) {
	llst->head = NULL;
	llst->size = 0;
	return;
}

void llst_destroy(osThreadLLst_t* llst) {}

uint8_t llst_size(osThreadLLst_t* llst) {
	return llst->size;
}

bool llst_insertFirst(osThreadLLst_t* llst, struct osThread_t *thread_data) {
  struct osThreadNode_t *newNode = (struct osThreadNode_t*) malloc(sizeof(struct osThreadNode_t));
  newNode->id = llst->size;
  newNode->data = thread_data;	
  newNode->next = llst->head; //point newNode to old first node
	
  llst->head = newNode; //point head to newNode
	llst->size++;
	
	return true;
}

bool llst_insertLast(osThreadLLst_t* llst, struct osThread_t *thread_data) {
  struct osThreadNode_t *newNode = (struct osThreadNode_t*) malloc(sizeof(struct osThreadNode_t));
  newNode->id = llst->size;
  newNode->data = thread_data;
	newNode->next = NULL;
	
	llst->size++;
	
	if (llst->head == NULL) {
		llst->head = newNode;
	}
	
  struct osThreadNode_t* curr = llst->head;
	while(curr->next != NULL) {  // walk to the end
    curr = curr->next;
	}	
	
	curr->next = newNode;
	return true;
}

bool llst_insert_node( osThreadLLst_t* llst, struct osThread_t *thread_data, int id) {return true;}

bool list_deleteFirst( osThreadLLst_t* llst) {
	struct osThreadNode_t* head = llst->head;
	struct osThreadNode_t* next = head->next;
	llst->head = next;
	free(head);
	return true;
}

bool list_deleteLast( osThreadLLst_t* llst) {
	struct osThreadNode_t* curr = llst->head;
	struct osThreadNode_t* next = curr->next;
	while(next->next != NULL) {  // walk to the end
    curr = next;
    next = curr->next;
	}	
	free(next);
	curr->next = NULL;
	return true;
}

bool llst_delete_node( osThreadLLst_t* llst, int id) {return true;}

struct osThread_t* llst_get_node(osThreadLLst_t* llst, int id) {
	struct osThreadNode_t* curr = llst->head;
	
	//if list is empty
  if(curr == NULL) {
     return NULL;
  }
	
	//navigate through list
  while(curr->id != id) {
		//if it is last node
    if(curr->next == NULL) {
			return NULL;
    } else {
			//go to next link
      curr = curr->next;
    }
  }      
   //if data found, return the current Link
   return curr->data;
}
