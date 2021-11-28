#include MH_QUEUE_IMPL
#include MH_ALLOC_IMPL

Queue mh_queue_new(void) {
    return mh_calloc(1, sizeof(mh_queue_t));
}

void mh_queue_destroy(Queue queue) {
    mh_assert(queue != MH_NULL);
    mh_free(queue);
}

Any mh_queue_take(Queue queue) {
    mh_assert(queue != MH_NULL);
    mh_queue_node_t *node = queue->first;
    if (node == MH_NULL) {
        mh_debug("Queue is empty, returning MH_NULL.", queue);
        return MH_NULL;
    }
    queue->first = queue->first->next;
    Any data = node->data;
    mh_free(node);
    return data;
}

void mh_queue(Queue queue, Any data) {
    mh_assert(queue != MH_NULL);
    mh_queue_node_t *node = mh_calloc(1, sizeof(mh_queue_node_t));
    node->data = data;
    if (queue->first == MH_NULL) {
        queue->first = node;
    } else {
        queue->current->next = node;
    }
    queue->current = node;
}
