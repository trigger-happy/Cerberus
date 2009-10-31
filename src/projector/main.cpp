#include <QApplication>
#include "ProjectorWindow.h"

int main ( int argc, char* argv[] )
{
		QApplication app ( argc, argv );
		ProjectorWindow pw;
		pw.show();
		return app.exec();
}
