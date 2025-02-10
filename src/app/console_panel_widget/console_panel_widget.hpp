#ifndef CONSOLE_PANEL_WIDGET_HPP
#define CONSOLE_PANEL_WIDGET_HPP

#include <QWidget>
#include <QPlainTextEdit>
#include <QGridLayout>

namespace APP
{
class ConsolePanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsolePanelWidget(QWidget *parent = nullptr);
    ~ConsolePanelWidget();

    void removeFirstLine();
    void removeLastLine();
    void clearConsole();

    void setMaxLines(int max_lines);
    int maxLines() const;

public slots:
    void addLine(const QString &text);

private:
    void initialize();
    void setupUi();
    void setupStyle();
    void setupConnections();
    void enforceMaxLines();

private:
    QGridLayout *m_main_layout;
    QPlainTextEdit *m_console;
    int m_max_lines;
};
} // namespace APP

#endif // CONSOLE_PANEL_WIDGET_HPP

