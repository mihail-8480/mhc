#ifndef CASYNC_MH_QUEUE_H
#define CASYNC_MH_QUEUE_H
#include "mh.h"

// A datatype that holds elements and returns them first-in-first-out.
// Our implementation does only one allocation for the queue object.
typedef struct mh_queue mh_queue_t, *Queue;

// Put something in the queue (the element gets added).
MH_EXPORT void mh_queue(Queue queue, Any data);

// Take something from the queue (the element gets removed).
MH_EXPORT Any mh_queue_take(Queue queue);

// Create a new queue (must be destroyed with mh_queue_destroy).
MH_EXPORT Queue mh_queue_new(void);

// Destroy a queue.
MH_EXPORT void mh_queue_destroy(Queue queue);

#endif //CASYNC_MH_QUEUE_H
