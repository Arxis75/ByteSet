#include <data/ByteSet.h>

int main(int argc, char *argv[])
{
    ByteSet<4> b1;
    b1 = ByteSet<4>("0x646973637634646973637635");
    cout << hex << int(b1[2]) << endl;
    return 0;
}
