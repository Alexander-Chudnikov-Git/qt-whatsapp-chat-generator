#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>


class QGridLayout;
class QSplitter;

namespace APP
{
class ControlPanelWidget;
class ConsolePanelWidget;
}

namespace APP
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initialize();
    void setupUi();
    void setupStyle();
    void setupConnections();
    
private:
    QGridLayout *m_main_layout;
    QSplitter *m_splitter;

    ControlPanelWidget *m_control_panel;
    ConsolePanelWidget *m_console_panel;
};
}
#endif // MAINWINDOW_HPP
