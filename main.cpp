#include <QApplication>
#include "connection.h"
#include "tableeditor.h"

int main(int argc, char* argv[]){

    QApplication app(argc,argv);

    if(!createConnection()) //连接数据库
        return -1;

    TableEditor editor("person");
    editor.show();
    return app.exec();
}
