#include<iostream>
#include <QDir>
#include <string>
using namespace std;

int main() {
    QDir dir("");
    string s;
    cin >> s;
    QString* ss = new QString("resources/go/hello.txt");
    bool cnt = dir.mkdir(  QString("resources/hello") );

    QFile file("resources/logo.png");
    bool cnt2 = file.copy( "resources/hello/logo.png" );
    //QDir::setCurrent("/resources");
    //file.setFileName("readme.txt");
    //QDir::setCurrent("/home");
    //file.open(QIODevice::ReadOnly);

    cout << cnt << " " << cnt2 << endl;
    return 0;
}

