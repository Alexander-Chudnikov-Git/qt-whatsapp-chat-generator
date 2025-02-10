#include "overlay_widget.hpp"
#include "spdlog_wrapper.hpp"

#include <QPainter>

namespace APP
{
OverlayWidget::OverlayWidget(QWidget *parent) :
    QWidget(parent),
    m_overlay_widget(nullptr),
    m_overlay_visible(false)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);

    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignCenter);
    setLayout(m_layout);
}

void OverlayWidget::setOverlayWidget(QWidget *widget)
{
    if (m_overlay_widget)
    {
        m_layout->removeWidget(m_overlay_widget);
        m_overlay_widget->hide();
        m_overlay_widget->setParent(nullptr);
    }

    m_overlay_widget = widget;

    if (m_overlay_widget)
    {
        m_overlay_widget->setAttribute(Qt::WA_TranslucentBackground, false);
        m_layout->addWidget(m_overlay_widget);
        m_overlay_widget->setParent(this);
        if (m_overlay_visible)
        {
             m_overlay_widget->show();
        }
    }
}

void OverlayWidget::showOverlay()
{
    if (m_overlay_widget)
    {
        m_overlay_visible = true;
        //Make parent respond to mouse clicks
        parentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        //Make overlay not respond to mouse clicks
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        //Force main window to redraw, so overlay will be painted
        parentWidget()->update();
        show();
        m_overlay_widget->show();
    }
}

void OverlayWidget::hideOverlay()
{
    if (m_overlay_widget)
    {
        m_overlay_visible = false;
        parentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        hide();
        m_overlay_widget->hide();
    }
}

bool OverlayWidget::isOverlayVisible() const
{
    return m_overlay_visible;
}

void OverlayWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void OverlayWidget::paintEvent(QPaintEvent *event)
{
    if (m_overlay_visible)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, 128));
    }
    QWidget::paintEvent(event);
}
}
