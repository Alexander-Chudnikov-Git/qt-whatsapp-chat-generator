#include "console_panel_widget.hpp"

#include <QScrollBar>

namespace APP
{

ConsolePanelWidget::ConsolePanelWidget(QWidget *parent) :
    QWidget(parent),
    m_max_lines(100)
{
    initialize();
}

ConsolePanelWidget::~ConsolePanelWidget() = default;

void ConsolePanelWidget::initialize()
{
    setupUi();
    setupStyle();
    setupConnections();
}

void ConsolePanelWidget::setupUi()
{
    m_main_layout = new QGridLayout(this);
    m_console = new QPlainTextEdit(this);

    m_console->setReadOnly(true);
    m_console->setWordWrapMode(QTextOption::NoWrap);

    m_main_layout->addWidget(m_console);
    setLayout(m_main_layout);
}

void ConsolePanelWidget::setupStyle()
{
    m_console->setStyleSheet("QPlainTextEdit { background-color: black; color: white; font-family: monospace; }");
}

void ConsolePanelWidget::setupConnections()
{
    // No additional connections needed for now
}

void ConsolePanelWidget::addLine(const QString &text)
{
    m_console->appendPlainText(text);
    enforceMaxLines();

    // Auto-scroll to bottom
    QTextCursor cursor = m_console->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfBlock);
    m_console->setTextCursor(cursor);
}

void ConsolePanelWidget::removeFirstLine()
{
    QString content = m_console->toPlainText();
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    if (!lines.isEmpty())
    {
        lines.removeFirst();
        m_console->setPlainText(lines.join('\n'));
    }
}

void ConsolePanelWidget::removeLastLine()
{
    QString content = m_console->toPlainText();
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    if (!lines.isEmpty())
    {
        lines.removeLast();
        m_console->setPlainText(lines.join('\n'));
    }
}

void ConsolePanelWidget::clearConsole()
{
    m_console->clear();
}

void ConsolePanelWidget::setMaxLines(int max_lines)
{
    m_max_lines = max_lines;
    enforceMaxLines();
}

int ConsolePanelWidget::maxLines() const
{
    return m_max_lines;
}

void ConsolePanelWidget::enforceMaxLines()
{
    QString content = m_console->toPlainText();
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    while (lines.size() > m_max_lines)
    {
        lines.removeFirst();
    }

    m_console->setPlainText(lines.join('\n'));
}

} // namespace APP

