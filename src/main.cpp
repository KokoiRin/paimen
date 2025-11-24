#include <QApplication>
#include <QScreen>
#include "MainWindow.h"
#include "MiniWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow *mainWindow = new MainWindow();
    MiniWindow *miniWindow = new MiniWindow();
    
    // 获取屏幕尺寸
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    // 窗口切换逻辑
    QObject::connect(mainWindow, &MainWindow::switchToMiniMode, [=]() {
        mainWindow->hide();
        miniWindow->show();
        miniWindow->move(
            screenGeometry.width() - miniWindow->width() - 20,
            20
        );
    });
    
    QObject::connect(miniWindow, &MiniWindow::switchToNormalMode, [=]() {
        miniWindow->hide();
        mainWindow->show();
    });
    
    // 默认显示主窗口
    mainWindow->show();
    
    return app.exec();
}
