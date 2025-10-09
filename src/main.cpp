//#include <UI/dialog.h>
//#include <QApplication>
#include <data/ByteSet.h>
#include <data/Tools.h>

int main(int argc, char *argv[])
{
    ByteSet b1("0x0ABCDEF");
    cout << hex << b1.asString() << endl << endl;
    ByteSet<1> b2("0x0ABCDEF");
    cout << hex << b2.asString() << endl << endl;
    ByteSet b3("0b000101010111100110111101111", Bin);
    cout << hex << b3.asString(Bin) << endl << endl;
    ByteSet<1> b4("0b000101010111100110111101111", Bin);
    cout << hex << b4.asString(Bin) << endl << endl;
    ByteSet b5("discv5", UTF8);
    cout << hex << b5.asString(UTF8) << endl << endl;
    ByteSet<1> b6("discv5", UTF8);
    cout << hex << b6.asString(UTF8) << endl << endl;

    /*QApplication a(argc, argv);
    Dialog d;
    d.show();
    return a.exec();*/
    return 0;
}
