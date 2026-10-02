#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Game game(Side::BLACK, Timer::FIVE);
    // game.initBoard();
    //
    // game.board.print();
    // std::cout << std::endl;
    // game.board.display();

    MainWindow window;
    window.show();

    // QPushButton button("Hello world!", nullptr);
    // button.resize(100, 50);
    // button.show();
    return QApplication::exec();
}
