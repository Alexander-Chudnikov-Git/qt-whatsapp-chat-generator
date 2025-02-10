#ifndef CONTROL_PANEL_WIDGET_HPP
#define CONTROL_PANEL_WIDGET_HPP

#include "whatsapp_chat_parser.hpp"
#include "whatsapp_chat_generator.hpp"

#include <QWidget>


class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QTimeEdit;
class QSpinBox;
class QComboBox;
class QCheckBox;

namespace APP
{
class ControlPanelWidget : public QWidget
{
    Q_OBJECT

public:
    ControlPanelWidget(QWidget *parent = nullptr);
    ~ControlPanelWidget();

private:
    void initialize();
    void setupUi();
    void setupStyle();
    void setupConnections();

private slots:
    void onFileBrowseClicked();
    void onDirectoryBrowseClicked();
    void onRunClicked();

signals:
    void parsingStatus(const QString& message);

private:
    QGridLayout *m_main_layout;

    QLabel      *m_file_selector_label;
    QLineEdit   *m_file_selector_line_edit;
    QPushButton *m_file_selector_button;

    QCheckBox *m_hide_header_checkbox;
    QCheckBox *m_hide_footer_checkbox;
    QCheckBox *m_hide_dark_theme_checkbox;
    QCheckBox *m_hide_iphone_checkbox;
    QCheckBox *m_hide_percentage_checkbox;
    QCheckBox *m_hide_payment_checkbox;
    QCheckBox *m_hide_dual_sim_checkbox;

    QComboBox *m_network_combobox;
    QTimeEdit *m_time_edit;
    QSpinBox  *m_battery_spinbox;
    QLineEdit *m_name_line_edit;

    QLabel      *m_directory_selector_label;
    QLineEdit   *m_directory_selector_line_edit;
    QPushButton *m_directory_selector_button;

    QPushButton *m_run_button;

    UTILS::WhatsappChatParser    *m_chat_parser;
    UTILS::WhatsappChatGenerator *m_chat_generator;
};
}
#endif // CONTROL_PANEL_WIDGET_HPP
