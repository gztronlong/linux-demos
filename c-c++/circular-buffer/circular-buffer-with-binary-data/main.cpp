
#include "CircularBuffer.h"
#include <iostream>

// Thanks to: http://www.asawicki.info/news_1468_circular_buffer_of_raw_binary_data_in_c.html

using namespace std;

int main() {
    char tmpBuf[1024];
    char helloBuf[] = "hello world"; // 11

    CircularBuffer buf(1024);

    cout << "capacity: " << buf.capacity() << endl;
    cout << "size: " << buf.size() << endl;

    int nwrite = buf.write(helloBuf, 11);
    cout << "nwrite: " << nwrite << endl;

    nwrite = buf.write(tmpBuf, 1024);
    cout << "nwrite: " << nwrite << endl;

    int nread = buf.read(tmpBuf, 1020);
    cout << "nread: " << nread << endl;

    buf.clear();
    cout << "size: " << buf.size() << endl;

    nwrite = buf.write(tmpBuf, 50);
    cout << "nwrite: " << nwrite << endl;

    nread = buf.read(tmpBuf, 100);
    cout << "nread: " << nread << endl;

    return 0;
}
