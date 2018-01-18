
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <queue>
#include <stdlib.h>
#include <pthread.h>

class CircularBuffer {
    public:
        CircularBuffer(size_t capacity);
        ~CircularBuffer();

        size_t size() const;
        size_t capacity() const;
        void clear();
        bool push(const char *data, size_t bytes);
        bool pop(char *data, size_t &bytes);

    private:
        size_t beg_index_;
        size_t end_index_;
        size_t size_;
        size_t capacity_;
        char *data_;
        std::queue<size_t> element_size_queue;
        pthread_rwlock_t rw_lock_;
};

#endif
