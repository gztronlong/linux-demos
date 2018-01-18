
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h>


struct entry {
    size_t frame_size;
    CIRCLEQ_ENTRY(entry) entries;
};

struct circular_buffer {
    size_t beg_index_;
    size_t end_index_;
    size_t size_;
    size_t capacity_;
    char *data_;
    CIRCLEQ_HEAD(circleq, entry) size_queue_;
    pthread_rwlock_t rw_lock_;
};

struct circular_buffer *circular_buffer_init(size_t capacity);
void circular_buffer_deinit(struct circular_buffer *buf);

size_t circular_buffer_size(struct circular_buffer *buf);
size_t circular_buffer_capacity(struct circular_buffer *buf);

void circular_buffer_clear(struct circular_buffer *buf);

bool circular_buffer_push(struct circular_buffer *buf, const char *data, size_t bytes);
bool circular_buffer_pop(struct circular_buffer *buf, char *data, size_t *bytes);


#ifdef __cplusplus
}
#endif

#endif
