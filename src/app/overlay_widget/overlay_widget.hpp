#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QWidget>

namespace APP
{
class OverlayWidget : public QWidget
{
	Q_OBJECT

public:
	explicit OverlayWidget(QWidget *parent = nullptr);

	void setOverlayWidget(QWidget *widget);
	void showOverlay();
	void hideOverlay();
	bool isOverlayVisible() const;

protected:
	void resizeEvent(QResizeEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	QWidget		*m_overlay_widget;
	bool		 m_overlay_visible;
	QVBoxLayout *m_layout;
};
} // namespace APP
#endif // OVERLAYWIDGET_H
