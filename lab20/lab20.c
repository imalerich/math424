/* File:     linked_list.c
 *
 * Purpose:  Implement a sorted linked list of ints with ops insert,
 *           print, member, delete, free_list.
 * 
 * Input:    Single character lower case letters to indicate operators, 
 *           followed by arguments needed by operators.
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o linked_list linked_list.c
 * Run:      ./linked_list
 *
 * Notes:
 *    1.  Repeated values are not allowed in the list
 *    2.  DEBUG compile flag used.  To get debug output compile with
 *        -DDEBUG command line flag.
 *    3.  Int input isn't checked for errors.
 *
 * IPP:   Section 4.9.1 (pp. 181 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

struct list_node_s {
   int    data;
   struct list_node_s* next;
   omp_lock_t lock; // lock this object while we are modifying it
};

int Init(int value, struct list_node_s ** head_pp);
void Insert(int value, struct list_node_s ** head_p);
void Print(struct list_node_s* head_p);
int  Member(int value, struct list_node_s* head_p);
int  Delete(int value, struct list_node_s** head_p);
void Free_list(struct list_node_s** head_p);
int  Is_empty(struct list_node_s* head_p);
char Get_command(void);
int  Get_value(void);

/*-----------------------------------------------------------------*/
int main(int argc, char ** argv) {
	srand(time(NULL));
   struct list_node_s* head_p = NULL;  /* start with empty list */

   if (argc == 1) {
	   printf("Please include the thread count as argument.\n");
	   printf("-- Each thread will generate random values to\n");
	   printf("-- insert/delete into/from the list.\n");
	   return 0;
   }

   // allocate a bunch of threads, each thread will 
   // generate or delete random values from the linked list
   // all threads share the same queue
   // I will simulate this using random data, but could be
   // implemented for example in a multi-user system
   size_t thread_count = strtol(argv[1], NULL, 10);
#pragma omp parallel num_threads(thread_count)
   {
	   int  value;
		/* -----------------------
		 * Supported Operations
		 * -----------------------
		 * Insert(value, &head_p);
		 * Print(head_p);
    	 * Member(value, head_p);
         * Delete(value, &head_p);
		 * -------------------- */

	   // each thread will perform 10 operations on the queue
	   for (int i=1; i<=10; i++) {
		   int r = rand() % 4;

		   /* --- 1 in 4 operations will be a delete. --- */
		   if (r == 0) {
		       // delete a number between 0 and 19
		       value = rand() % 20;
		       // otherwise we can assume it's not the head element
		       Delete(value, &head_p);
		   } else if (r == 1) {
		       value = rand() % 20;
			   Member(value, head_p);

		   } else { // 2 of 4 operations will be an insert
			   // insert a number between 0 and 19
			   // value = rand() % 10;
			   value = rand() % 20;
			   Insert(value, &head_p);
		   }
	   }
   }

   printf("Threads are done modifying the list...\n");
   Print(head_p);
   Free_list(&head_p);
   return 0;
}

/*
 * Initializes the linked list, do this in a critical section
 * se we can guarantee only one thread performs the initialization.
 * If the head_pp is already initialized, this method returns 1,
 * otherwise return 0.
 */
int Init(int value, struct list_node_s ** head_pp) {
	int did_something = 1;

#pragma omp critical
	{
		// already initalized, or initialized by another thread
		if (*head_pp == NULL) { 
			*head_pp = malloc(sizeof(struct list_node_s));
			(*head_pp)->data = value;
			(*head_pp)->next = NULL;
			omp_init_lock(&(*head_pp)->lock);
			did_something = 0;
		}
	}

	// return 1 if we didn't do anything
	return did_something;
}

/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert value in correct numerical location 
 *             into list.  If value is already in the list, 
 *             print a message and return 
 * In arg:     value, the value to be inserted            
 * In/out arg: head_pp, a pointer to the head of the list pointer
 * Return val: 1 if value was inserted, 0 otherwise
 */            
void Insert(int value, struct list_node_s ** head_pp) {
   // see if we need to initialize the list,
   // if we need to, only allow one thread to
   // do it in a critical section, if it's
   // been initialized go ahead and insert the value
   if (Init(value, head_pp) == 0) {
	   // value initialized the list, nothing to do
	   return;
   }

	struct list_node_s * head_p = *head_pp;

   if (value < head_p->data) {
	   // if we need to update the head pointer
	   // do the insertion in a critical section
	   // that way only one thread can change
	   // the head at a time 
#pragma omp critical
	   {
			struct list_node_s * temp_p = malloc(sizeof(struct list_node_s));
			temp_p->data = value;
			temp_p->next = head_p;
			omp_init_lock(&temp_p->lock); // Initialize a lock for it.

			*head_pp = temp_p;
	   }

	   return;
   } else if (value == head_p->data) {
		printf("[%d] already in list.\n", value);
		return;
   }

   // otherwise we know that we at least have a head
   // and value goes somewhere after that head
   
	struct list_node_s * curr_p = head_p->next;
	struct list_node_s * pred_p = head_p;

	omp_set_lock(&pred_p->lock);
	while (curr_p) {
		omp_set_lock(&curr_p->lock);

		if (curr_p->data == value) {
			omp_unset_lock(&pred_p->lock);
			omp_unset_lock(&curr_p->lock);
			printf("[%d] already in list.\n", value);
			return;
		}

		if (curr_p->data > value) {
			// value goes after pred and before curr
			// keep both locked
			break;
		}

		// unset the lock on pred_p before updating it
		omp_unset_lock(&pred_p->lock);

		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	// Initialize the new item to be added to the list.
	// It's not actually in the list yet so we don't need to worry about locking it.
	struct list_node_s * temp_p = malloc(sizeof(struct list_node_s));
	temp_p->data = value;
	temp_p->next = curr_p;
	omp_init_lock(&temp_p->lock); // Initialize a lock for it.

	pred_p->next = temp_p;

	if (curr_p) { omp_unset_lock(&curr_p->lock); }
	omp_unset_lock(&pred_p->lock);
} 

/*-----------------------------------------------------------------*/
/* Function:    Delete
 * Purpose:     If value is in the list, delete it.  Otherwise, just
 *              return.
 * In arg:      value, the value to be deleted
 * In/out arg:  head_pp, pointer to the head of the list pointer
 * Return val:  1 if value is deleted, 0 otherwise
 */
int Delete(int value, struct list_node_s ** head_pp) {
	struct list_node_s * head_p = *head_pp;

	// nothing in least, nothing to delete
	if (head_p == NULL) {
		printf("Cannot Delete [%d]: Not in list.\n", value);
		return 0;
	}

	// if we need to remove the head, use a critical section
	// so only one thread can modify the root head pointer at a time
	if (head_p != NULL && head_p->data == value) { 
#pragma omp critical
		{
		struct list_node_s * tmp = head_p;
		*head_pp = tmp->next;
		omp_destroy_lock(&tmp->lock);
		free(tmp);
		}
		return 0;
	}

	// we know it's not the head, so check the remaining elements
	struct list_node_s * curr_p = head_p->next;
	struct list_node_s * pred_p = head_p;

	// we need to update pred to point to cur, so we need to lock it
	// we will also be removing cur (once found) so keep a lock on it too
	omp_set_lock(&pred_p->lock);
	while (curr_p) {
		omp_set_lock(&curr_p->lock);

		// is this the node we need to remove?
		if (curr_p->data == value) {
			// skip the current element in the linked list
			pred_p->next = curr_p->next;

			omp_unset_lock(&pred_p->lock);
			omp_unset_lock(&curr_p->lock);
			return 0;
		}

		// else keep searching
		omp_unset_lock(&pred_p->lock);
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	omp_unset_lock(&pred_p->lock);
	// don't need to unlock curr_p because curr_p = NULL
	printf("Cannot delete [%d] not in list.\n", value);
}  /* Delete */

/*-----------------------------------------------------------------*/
/* Function:  Print
 * Purpose:   Print the contents of the list
 * In arg:    head_p, pointer to the head of the list
 */
void Print(struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   printf("list = ");

   curr_p = head_p;
   while (curr_p) {
	  omp_set_lock(&curr_p->lock);
      printf("%d ", curr_p->data);

	  struct list_node_s * pred_p = curr_p;
      curr_p = curr_p->next;
	  omp_unset_lock(&pred_p->lock);
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
/* Function:    Member
 * Purpose:     Search list for value
 * In args:     value, the value to be searched for
 *              head_p, pointer to the head of the list  
 * Return val:  true, if value is in the list, false otherwise
 */
int Member(int value, struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   curr_p = head_p;
   while (curr_p) {
	  omp_set_lock(&curr_p->lock);

	  if (curr_p->data == value) {
		  omp_unset_lock(&curr_p->lock);
		  printf("%d is in the list\n", value);
		  return 1;
	  }

	  struct list_node_s * pred_p = curr_p;
      curr_p = curr_p->next;
	  omp_unset_lock(&pred_p->lock);
   }

   // if we reached the end without finding the value
   // it is not in the list
  printf("%d is not in the list\n", value);
  return 0;
}  /* Member */

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free the storage used by the list
 * In/out arg: head_pp, pointer to the head of the list pointer
 *               on input, NULL on return
 */
void Free_list(struct list_node_s** head_pp) {
	// Only one thread is going to do this.
   struct list_node_s* curr_p;
   struct list_node_s* succ_p;

   if (Is_empty(*head_pp)) return;
   curr_p = *head_pp; 
   succ_p = curr_p->next;
   while (succ_p != NULL) {
#     ifdef DEBUG
      printf("Freeing %d\n", curr_p->data);
#     endif
	  // Just need to destroy the lock for each node.
	  omp_destroy_lock(&curr_p->lock);
      free(curr_p);
      curr_p = succ_p;
      succ_p = curr_p->next;
   }
#  ifdef DEBUG
   printf("Freeing %d\n", curr_p->data);
#  endif
   free(curr_p);
   *head_pp = NULL;
}  /* Free_list */

/*-----------------------------------------------------------------*/
/* Function:    Is_empty
 * Purpose:     Determine whether the list is empty
 * In arg:      head_p:  head of list pointer
 * Return val:  true, if head_p is NULL, false otherwise
 */
int Is_empty(struct list_node_s* head_p) {
   if (head_p == NULL) {
      return 1;
   } else {
      return 0;
   }
}  /* Is_empty */

/*-----------------------------------------------------------------*/
/* Function:    Get_command
 * Purpose:     Get the next command (a single char) from stdin
 * Return val:  The next nonwhite char in stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command:  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------*/
/* Function;      Get_value
 * Purpose:       Get an int from stdin
 * Return value:  The int
 */
int  Get_value(void) {
   int val;

   printf("Please enter a value:  ");
   scanf("%d", &val);
   return val;
}  /* Get_value */
