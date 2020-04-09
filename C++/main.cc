//
// C++&Qt Version
//

#include "snake.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Snake snake;
    snake.show();
    return app.exec();
}
