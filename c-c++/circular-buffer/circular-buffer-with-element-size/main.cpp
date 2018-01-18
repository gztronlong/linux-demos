
#include "CircularBuffer.h"
#include <iostream>
#include <cstring>

// Thanks to: http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html

using namespace std;

int main() {
    char tmpBuf[1024];
    char helloBuf[] = "hello world"; // 11

    CircularBuffer buf(1024);

    cout << "capacity: " << buf.capacity() << endl;
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.push(helloBuf, 11);
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.push(tmpBuf, 1024);
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.push(tmpBuf, 1000);
    cout << "size: " << buf.size() << endl;

    cout << endl;
    size_t nread;
    buf.pop(tmpBuf, nread);
    cout << "nread: " << nread << endl;
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.clear();
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.push(tmpBuf, 50);
    cout << "size: " << buf.size() << endl;

    cout << endl;
    buf.pop(tmpBuf, nread);
    cout << "size: " << buf.size() << endl;
    cout << "nread: " << nread << endl;

    buf.clear();
    for (int i = 0; i < 10; i++) {
         buf.push(helloBuf, 11);
         buf.push(helloBuf, 5);
    }

    cout << endl;
    for (int i = 0; i < 20; i++) {
        buf.pop(tmpBuf, nread);
        tmpBuf[nread] = '\0';
        cout << "nread: " << nread << " string: " << tmpBuf << endl;
    }
    buf.pop(tmpBuf, nread);
    cout << "nread: " << nread << endl;

    return 0;
}
