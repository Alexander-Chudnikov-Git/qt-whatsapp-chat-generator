#include "main_window.hpp"
#include "control_panel_widget.hpp"
#include "console_panel_widget.hpp"
#include "settings_manager.hpp"

#include <QLabel>
#include <QSplitter>
#include <QGridLayout>
#include <QSizePolicy>
#include <QTimer>


namespace APP
{
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_move_resize_timer(new QTimer(this))
{
    this->initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{
    this->m_move_resize_timer->setSingleShot(true);
    this->m_move_resize_timer->setInterval(500);

    QRect window_rect = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::WINDOW_RECT).toRect();

#if defined(SYSTEM_IS_TILING) && SYSTEM_IS_TILING == 1
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    setFixedSize(window_rect->width(), window_rect->height());
    move(window_rect->x(), window_rect->y());
#else
    setGeometry(window_rect);
#endif

    this->setupUi();
    this->setupStyle();
    this->setupConnections();
}

void MainWindow::setupUi()
{
    auto layout_wrapper = new QWidget();
    this->m_main_layout = new QGridLayout();
    this->m_splitter    = new QSplitter(Qt::Horizontal);

    this->m_control_panel = new ControlPanelWidget();
    this->m_console_panel = new ConsolePanelWidget();

    this->m_main_layout->addWidget(this->m_splitter, 0, 0, 1, 1);

    this->m_splitter->addWidget(this->m_control_panel);
    this->m_splitter->addWidget(this->m_console_panel);

    this->m_splitter->setStretchFactor(0, 1);
    this->m_splitter->setStretchFactor(1, 2);

    layout_wrapper->setLayout(this->m_main_layout);
    this->setCentralWidget(layout_wrapper);
}

void MainWindow::setupConnections()
{
    connect(this->m_control_panel, &ControlPanelWidget::parsingStatus, this->m_console_panel,  &ConsolePanelWidget::addLine);
    connect(this->m_move_resize_timer, &QTimer::timeout, this, &MainWindow::saveWindowRect);
}

void MainWindow::setupStyle()
{
    setStyleSheet("* { background: #181818; color: #ffffff; }");
}

void MainWindow::saveWindowRect()
{
    UTILS::SettingsManager::instance()->saveChanges();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);

    QRect window_rect = this->geometry();
    UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::WINDOW_RECT, window_rect);

    this->m_move_resize_timer->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QRect window_rect = this->geometry();
    UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::WINDOW_RECT, window_rect);

    this->m_move_resize_timer->start();
}

}
