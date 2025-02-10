#include "main_window.hpp"
#include "control_panel_widget.hpp"
#include "console_panel_widget.hpp"
#include "spdlog_wrapper.hpp"

#include <QLabel>
#include <QSplitter>
#include <QGridLayout>
#include <QSizePolicy>


namespace APP
{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{
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
}

void MainWindow::setupStyle()
{
    setStyleSheet("MainWindow { background: #181818; }");
}
}
