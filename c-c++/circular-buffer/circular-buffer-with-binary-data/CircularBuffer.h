
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdlib.h>
#include <pthread.h>

class CircularBuffer {
    public:
        CircularBuffer(size_t capacity);
        ~CircularBuffer();

        size_t size() const;
        size_t capacity() const;
        void clear();
        // Return number of bytes written.
        size_t write(const char *data, size_t bytes);
        // Return number of bytes read.
        size_t read(char *data, size_t bytes);

    private:
        size_t beg_index_;
        size_t end_index_;
        size_t size_;
        size_t capacity_;
        char *data_;
        pthread_rwlock_t rw_lock_;
};

#endif
