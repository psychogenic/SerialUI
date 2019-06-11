/*
 * RingBuffer.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Pat Deegan
 *
 *  Part of the SerialUI project.
 *  Copyright (C) 2019 Pat Deegan, https://psychogenic.com
 *  More information on licensing and usage at 
 *  https://devicedruid.com/
 */

#ifndef SERIALUI_SRC_RINGBUFFER_H_
#define SERIALUI_SRC_RINGBUFFER_H_
#include <cstring>
#include <sys/types.h>


#ifndef MIN
#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
#endif

/*
#include <mutex>
#include <thread>

#include <iostream>
*/

#define RINGBUG_MUTEX_TYPE			volatile bool
#define RINGBUG_DEFINE_MUTEX(var)		RINGBUG_MUTEX_TYPE var

#define RINGBUG_DEFINE_MUTEXLOCK(var)		RINGBUG_MUTEX_TYPE & var
//define RINGBUG_DEFINE_MUTEX(var)		std::mutex var


#define RINGBUG_MUTEX_INIT(var)		var(false)


/*
#define RINGBUF_LOG_UNLOCKING()	RINGBUF_DEBUG("Unlocking")
// define RINGBUF_LOCKBUF_MUTEX(m) std::unique_lock<std::mutex> lock1(m)

#define RINGBUF_LOCKBUF_MUTEX(m) while (m) { asm("nop");} m = true
#define RINGBUF_UNLOCKBUF_MUTEX(m)		m = false
// #define RINGBUF_LOCKBUF_MUTEX(m) LockAndLog lock1(m)


#define LOCKTHISBUF_MUTEX()	RINGBUF_LOCKBUF_MUTEX(bufmutex)

*/









#define RINGBUF_LOG_UNLOCKING()	RINGBUF_DEBUG("Unlocking")
// define RINGBUF_LOCKBUF_MUTEX(m) std::unique_lock<std::mutex> lock1(m)

#define RINGBUF_LOCKBUF_MUTEX(m) 		m = true
#define RINGBUF_UNLOCKBUF_MUTEX(m)		m = false
// #define RINGBUF_LOCKBUF_MUTEX(m) LockAndLog lock1(m)


#define LOCKTHISBUF_MUTEX()	RINGBUF_LOCKBUF_MUTEX(bufmutex)






#define RINGBUF_DEBUG(...)
namespace SerialUI {


class LockAndLog {
public:
	LockAndLog(RINGBUG_MUTEX_TYPE & m) : locky(m) {
		RINGBUF_LOCKBUF_MUTEX(m);
		// locky = new std::unique_lock<std::mutex>(m);


	}
	~LockAndLog() {
		RINGBUF_DEBUG("R ");
		RINGBUF_UNLOCKBUF_MUTEX(locky);
	}
	RINGBUG_DEFINE_MUTEXLOCK(locky);
	// std::unique_lock<std::mutex> * locky;
};



template<typename T> class RingBuffer {
public:
    /**
      * create a RingBuffer with space for up to size elements.
      */
    explicit RingBuffer(size_t size)
            : size(size)
            , begin(0)
            , end(0)
            , wrap(false)
    		, RINGBUG_MUTEX_INIT(bufmutex)
    {
        buffer = new T[size];
    }

    /**
      * copy constructor
      */
    RingBuffer(const RingBuffer<T> & rb) : wrap(false)
    {
    	RINGBUF_LOCKBUF_MUTEX(rb.bufmutex);
        this(rb.size);
        begin = rb.begin;
        end = rb.end;
        memcpy(buffer, rb.buffer, sizeof(T) * size);
    }

    /**
      * destructor
      */
    ~RingBuffer()
    {
        delete[] buffer;
    }
    size_t addItem(const T & data) {
    	return write(&(data), 1);
    }

    size_t addItem(const T * data) {
    	return write(data, 1);
    }



    size_t write(const T * data, size_t n)
    {
        LOCKTHISBUF_MUTEX();
        n = MIN(n, numFreeSlots());

        if (n == 0) {
            return n;
        }

        const size_t first_chunk = MIN(n, size - end);
        memcpy(buffer + end, data, first_chunk * sizeof(T));
        end = (end + first_chunk) % size;

        if (first_chunk < n) {
            const size_t second_chunk = n - first_chunk;
            memcpy(buffer + end, data + first_chunk, second_chunk * sizeof(T));
            end = (end + second_chunk) % size;
        }

        if (begin == end) {
            wrap = true;
        }

        return n;
    }

    bool peekNext(T* dest) {

        LOCKTHISBUF_MUTEX();
    	size_t n = numSlotsUsed();
    	if (n == 0) {
    		return false;
    	}

        memcpy(dest, buffer + begin, sizeof(T));
    	return true;
    }

    size_t readNext(T* dest) {
    	return read(dest, 1);
    }

    size_t read(T * dest, size_t n)
    {

        LOCKTHISBUF_MUTEX();
        n = MIN(n, numSlotsUsed());

        if (n == 0) {
            return n;
        }

        if (wrap) {
            wrap = false;
        }

        const size_t first_chunk = MIN(n, size - begin);
        memcpy(dest, buffer + begin, first_chunk * sizeof(T));
        begin = (begin + first_chunk) % size;

        if (first_chunk < n) {
            const size_t second_chunk = n - first_chunk;
            memcpy(dest + first_chunk, buffer + begin, second_chunk * sizeof(T));
            begin = (begin + second_chunk) % size;
        }
        return n;
    }

    size_t getOccupied() {

        LOCKTHISBUF_MUTEX();
        return numSlotsUsed();
    }

    size_t getFree() {
        LOCKTHISBUF_MUTEX();
        return numFreeSlots();
    }

    void clear() {
    	T dummyDest;
    	while (getOccupied()) {
    		readNext(&dummyDest);

    	}
    }
private:
    inline size_t numFreeSlots() {
        return size - numSlotsUsed();
    }
    inline size_t numSlotsUsed() {

        if (end == begin) {
            return wrap ? size : 0;
        } else if (end > begin) {
            return end - begin;
        } else {
            return size + end - begin;
        }
    }
    T * buffer;
    size_t size;
    size_t begin;
    size_t end;
    bool wrap;
    RINGBUG_DEFINE_MUTEX(bufmutex);
};

} /* namespace ggk */

#endif /* GOBBLEDEGOOK_SRC_RINGBUFFER_H_ */
