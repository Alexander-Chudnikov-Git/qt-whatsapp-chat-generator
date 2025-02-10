#include "translation_manager.hpp"
#include "settings_manager.hpp"
#include "main_window.hpp"
#include "spdlog_wrapper.hpp"

#include <QApplication>
#include <QDirIterator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load settings
    UTILS::SettingsManager::instance();
    // Load translations
    UTILS::TranslationManager::instance();

    // Log messages
    spdlog::info(QObject::tr("application_init"));

    app.setQuitOnLastWindowClosed(true);

    // Create main window
    auto window = std::make_unique<APP::MainWindow>(nullptr);
    window->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);

    // Center the window on the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screen_geometry = screen->geometry();
        int width = qMax(640, screen_geometry.width() / 4);
        int height = qMax(480, screen_geometry.height() / 4);
        int x = (screen_geometry.width() - width) / 2;
        int y = (screen_geometry.height() - height) / 2;

        window->setFixedSize(width, height);
        window->move(x, y);
    }

    window->show();
    return app.exec();
}
