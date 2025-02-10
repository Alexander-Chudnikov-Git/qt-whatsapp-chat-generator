#include "control_panel_widget.hpp"
#include "settings_manager.hpp"
#include "spdlog_wrapper.hpp"

#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QFormLayout>

namespace APP
{
ControlPanelWidget::ControlPanelWidget(QWidget *parent)
    : QWidget(parent)
{
    this->initialize();
}

ControlPanelWidget::~ControlPanelWidget()
{
}

void ControlPanelWidget::initialize()
{
    this->m_chat_parser    = new UTILS::WhatsappChatParser();
    this->m_chat_generator = new UTILS::WhatsappChatGenerator();

    this->setupUi();
    this->setupStyle();
    this->setupConnections();
}

void ControlPanelWidget::setupUi()
{
    // Layouts and Widgets
    m_main_layout = new QGridLayout();

    // File Selector
    m_file_selector_label = new QLabel(tr("file_label"));
    m_file_selector_line_edit = new QLineEdit();
    m_file_selector_button = new QPushButton(tr("browse_file_button"));
    m_file_selector_line_edit->setReadOnly(true);
    //m_file_selector_line_edit->setText(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_PATH).toString());


    // Checkboxes
    m_hide_header_checkbox     = new QCheckBox(tr("hide_header_checkbox"));
    m_hide_footer_checkbox     = new QCheckBox(tr("hide_footer_checkbox"));
    m_hide_dark_theme_checkbox = new QCheckBox(tr("hide_dark_theme_checkbox"));
    m_hide_iphone_checkbox     = new QCheckBox(tr("hide_iphone_checkbox"));
    m_hide_percentage_checkbox = new QCheckBox(tr("hide_percentage_checkbox"));
    m_hide_payment_checkbox    = new QCheckBox(tr("hide_payment_checkbox"));
    m_hide_dual_sim_checkbox   = new QCheckBox(tr("hide_dual_sim_checkbox"));

    m_hide_header_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_HEADER).toBool());
    m_hide_footer_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_FOOTER).toBool());
    m_hide_dark_theme_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_DARK_THEME).toBool());
    m_hide_iphone_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_IPHONE).toBool());
    m_hide_percentage_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_PERCENTAGE).toBool());
    m_hide_payment_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_PAYMENT).toBool());
    m_hide_dual_sim_checkbox->setChecked(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_DUAL_SIM).toBool());


    // Other Input Widgets
    m_network_combobox = new QComboBox();
    QStringList networks = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_NETWORKS).toStringList();
    m_network_combobox->addItems(networks);
    m_network_combobox->setCurrentText(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_NETWORK).toString());

    m_time_edit = new QTimeEdit();
    m_time_edit->setTime(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_TIME).toTime());

    m_battery_spinbox = new QSpinBox();
    m_battery_spinbox->setRange(0, 100);
    m_battery_spinbox->setSuffix("%");
    m_battery_spinbox->setValue(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_BATTERY).toInt());

    m_name_line_edit = new QLineEdit();
    m_name_line_edit->setText(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_NAME).toString());

    // Directory Selector
    m_directory_selector_label = new QLabel(tr("directory_label"));
    m_directory_selector_line_edit = new QLineEdit();
    m_directory_selector_button = new QPushButton(tr("browse_directory_button"));
    m_directory_selector_line_edit->setReadOnly(true);
    m_directory_selector_line_edit->setText(UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PATH).toString());

    // Bottom Buttons
    m_run_button = new QPushButton(tr("execute_push_button"));
    m_run_button->setEnabled(false); // Initially disable Run button

    // Group Boxes
    QGroupBox *file_selector_group = new QGroupBox(tr("input_file_group_box"));
    QGroupBox *directory_selector_group = new QGroupBox(tr("output_directory_group_box"));

    // Layouts for Group Boxes
    QVBoxLayout *file_selector_group_layout = new QVBoxLayout();
    QVBoxLayout *directory_selector_group_layout = new QVBoxLayout();

    // Layout for File Selector Row (Label, LineEdit, Button)
    QHBoxLayout *file_selector_row_layout = new QHBoxLayout();
    file_selector_row_layout->addWidget(m_file_selector_label);
    file_selector_row_layout->addWidget(m_file_selector_line_edit);
    file_selector_row_layout->addWidget(m_file_selector_button);

    // Layout for Directory Selector Row (Label, LineEdit, Button)
    QHBoxLayout *directory_selector_row_layout = new QHBoxLayout();
    directory_selector_row_layout->addWidget(m_directory_selector_label);
    directory_selector_row_layout->addWidget(m_directory_selector_line_edit);
    directory_selector_row_layout->addWidget(m_directory_selector_button);

    // Layout for Checkboxes (Vertical)
    QVBoxLayout *checkbox_layout = new QVBoxLayout();
    checkbox_layout->addWidget(m_hide_header_checkbox);
    checkbox_layout->addWidget(m_hide_footer_checkbox);
    checkbox_layout->addWidget(m_hide_dark_theme_checkbox);
    checkbox_layout->addWidget(m_hide_iphone_checkbox);
    checkbox_layout->addWidget(m_hide_percentage_checkbox);
    checkbox_layout->addWidget(m_hide_payment_checkbox);
    checkbox_layout->addWidget(m_hide_dual_sim_checkbox);

    // Layout for input widgets (Network, Time, Battery, Name)
    QFormLayout *input_form_layout = new QFormLayout();
    input_form_layout->addRow(tr("network_label:"), m_network_combobox);
    input_form_layout->addRow(tr("time_label:"), m_time_edit);
    input_form_layout->addRow(tr("battery_label:"), m_battery_spinbox);
    input_form_layout->addRow(tr("name_label:"), m_name_line_edit);


    // Button Layout (Horizontal)
    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->addWidget(m_run_button);
    button_layout->setAlignment(Qt::AlignRight);

    // Assemble Group Box Layouts
    file_selector_group_layout->addLayout(file_selector_row_layout);
    file_selector_group_layout->addLayout(checkbox_layout);
    file_selector_group->setLayout(file_selector_group_layout);

    directory_selector_group_layout->addLayout(directory_selector_row_layout);
    directory_selector_group->setLayout(directory_selector_group_layout);


    // Add widgets to the main layout (Grid Layout)
    m_main_layout->addWidget(file_selector_group,      0, 0);
    m_main_layout->addWidget(directory_selector_group, 1, 0);
    m_main_layout->addLayout(input_form_layout,       2, 0);
    m_main_layout->setRowStretch(2, 1);
    m_main_layout->addLayout(button_layout, 3, 0);


    setLayout(m_main_layout);
}

void ControlPanelWidget::setupConnections()
{
    connect(this->m_file_selector_button, &QPushButton::clicked, this, &ControlPanelWidget::onFileBrowseClicked);
    connect(this->m_directory_selector_button, &QPushButton::clicked, this, &ControlPanelWidget::onDirectoryBrowseClicked);
    connect(this->m_run_button, &QPushButton::clicked, this, &ControlPanelWidget::onRunClicked);
    connect(this->m_chat_parser, &UTILS::WhatsappChatParser::parsingStatus, this,  [this](const QString& message) { emit parsingStatus(message); });
    connect(this->m_chat_generator, &UTILS::WhatsappChatGenerator::generatorStatus, this,  [this](const QString& message) { emit parsingStatus(message); });

    connect(m_file_selector_line_edit, &QLineEdit::textChanged, this, [this](const QString &text)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_PATH, text);
    });
    connect(m_hide_header_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_HEADER, checked);
    });
    connect(m_hide_footer_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_FOOTER, checked);
    });
    connect(m_hide_dark_theme_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_DARK_THEME, checked);
    });
    connect(m_hide_iphone_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_IPHONE, checked);
    });
    connect(m_hide_percentage_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_PERCENTAGE, checked);
    });
    connect(m_hide_payment_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_PAYMENT, checked);
    });
    connect(m_hide_dual_sim_checkbox, &QCheckBox::toggled, this, [this](bool checked)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_HIDE_DUAL_SIM, checked);
    });

    // Network ComboBox
    connect(m_network_combobox, &QComboBox::currentTextChanged, this, [this](const QString &text)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_NETWORK, text);
    });

    // Time Edit
    connect(m_time_edit, &QTimeEdit::timeChanged, this, [this](const QTime &time)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_TIME, time);
    });

    // Battery SpinBox
    connect(m_battery_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_BATTERY, value);
    });

    // Name Line Edit
    connect(m_name_line_edit, &QLineEdit::textChanged, this, [this](const QString &text)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_NAME, text);
    });

    // Directory Selector Line Edit
    connect(m_directory_selector_line_edit, &QLineEdit::textChanged, this, [this](const QString &text)
    {
        UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PATH, text);
    });
}

void ControlPanelWidget::setupStyle()
{
    setStyleSheet("ControlPanelWidget { background: #181818; }"
                  "QLabel { color: #EEEEEE; }"
                  "QLineEdit { background-color: #282828; color: #EEEEEE; border: 1px solid #555555; }"
                  "QPushButton { background-color: #444444; color: #EEEEEE; border: 1px solid #555555; padding: 5px; }"
                  "QPushButton:hover { background-color: #555555; }"
                  "QGroupBox { color: #EEEEEE; border: 1px solid #555555; margin-top: 10px; }"
                  "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 5px; }");
}

void ControlPanelWidget::onFileBrowseClicked()
{
    auto default_path = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_PATH).toString();

    QString file_path = QFileDialog::getOpenFileName(this, tr("select_input_file_lable"), default_path, tr("xlsx_file_selector (*.xls *.xlsx)"));

    if (file_path.isEmpty())
    {
        spdlog::warn(tr("no_file_selected"));
        return;
    }

    QFileInfo file_info(file_path);
    QString directory_path = file_info.absolutePath();

    UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_CHAT_PATH, file_path);

    emit parsingStatus(tr("parsing_xlsx_message %0").arg(file_path));

    this->m_chat_parser->parseXlsx(file_path);
    this->m_chat_parser->printMessages();

    this->m_file_selector_line_edit->setText(file_path);
    m_run_button->setEnabled(true);
}

void ControlPanelWidget::onDirectoryBrowseClicked()
{
    auto default_path = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PATH).toString();

    QString directory_path = QFileDialog::getExistingDirectory(this, tr("select_directory_label"), default_path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (directory_path.isEmpty())
    {
        spdlog::warn(tr("no_directory_selected"));
        return;
    }

    UTILS::SettingsManager::instance()->setValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PATH, directory_path);

    emit parsingStatus(tr("directory_selected_message %0").arg(directory_path));

    this->m_directory_selector_line_edit->setText(directory_path);
}

void ControlPanelWidget::onRunClicked()
{
    UTILS::SettingsManager::instance()->saveChanges();

    // Input file path validation
    if (m_file_selector_line_edit->text().isEmpty())
    {
        m_run_button->setEnabled(false); // Consider if disabling the button is the desired behavior. Maybe just inform the user.
        spdlog::warn(tr("no_file_selected_run_check"));
        return;
    }

    // Parse chat messages
    auto results = this->m_chat_parser->getParseResult();
    if (results.isEmpty())
    {
        spdlog::warn(tr("no_messages_provided"));
        return;
    }

    setEnabled(false);

    this->m_chat_generator->showBrowser();
    bool generation_successful = this->m_chat_generator->generateChat(results);

    if (!generation_successful)
    {
        setEnabled(true);
        spdlog::warn(tr("error_generating_chat"));
        return;
    }

    QMetaObject::Connection error_connection;
    QMetaObject::Connection done_connection;

    error_connection = connect(this->m_chat_generator, &UTILS::WhatsappChatGenerator::generatorError, this, [this, done_connection]()
    {
        disconnect(done_connection);
        setEnabled(true);
    }, Qt::SingleShotConnection);

    done_connection = connect(this->m_chat_generator, &UTILS::WhatsappChatGenerator::generatorDone, this, [this, error_connection]()
    {
        disconnect(error_connection);
        this->m_chat_generator->hideBrowser();
        // Output directory and filename settings
        auto save_path       = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PATH).toString();
        auto filename_prefix = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_IMAGE_PREFIX).toString();
        auto file_path       = UTILS::SettingsManager::instance()->getValue(UTILS::SettingsManager::Setting::FAKE_CHAT_PATH).toString();
        QFileInfo file_info(file_path);
        QString file_name = file_info.fileName();


        if (save_path.isEmpty())
        {
            spdlog::warn(tr("error_save_directory_empty"));
            setEnabled(true);
            return;
        }

        QDir dir; // QDir object for directory operations

        // Check if directory exists, create if not
        if (!dir.exists(save_path))
        {
            spdlog::warn(tr("save_directory_does_not_exist %1, attempting to create").arg(save_path));
            if (!dir.mkpath(save_path)) // mkpath creates all parent directories if necessary
            {
                spdlog::error(tr("error_create_save_directory %1").arg(save_path));
                emit parsingStatus(tr("error_create_save_directory %1").arg(save_path));
                setEnabled(true);
                return; // Exit if directory creation fails
            }
            spdlog::info(tr("save_directory_created %1").arg(save_path));
        }

        // Get images to save (Now get it after generatorDone is emitted, might be redundant if results are already passed)
        auto images_to_save = this->m_chat_generator->getGeneratorResult();
        if (images_to_save.isEmpty())
        {
            spdlog::warn(tr("no_screenshots_provided"));
            emit parsingStatus(tr("no_screenshots_provided"));
            setEnabled(true);
            return;
        }

        // Save images
        emit parsingStatus(tr("status_saving_images_to %0").arg(save_path));
        spdlog::info(tr("status_saving_images_to %0").arg(save_path));

        int saved_count = 0;
        for (int i = 0; i < images_to_save.size(); ++i)
        {
            // Construct filename - Assuming filename_prefix is intended to be a format string
            QString filename = QString("%1/%2").arg(save_path).arg(filename_prefix.arg(file_name, QString::number(i)));
            // If filename_prefix is just a prefix and not a format string, use:
            // QString filename = QString("%1/%2%3.png").arg(save_path).arg(filename_prefix).arg(QString::number(i));


            if (images_to_save[i].save(filename, "PNG"))
            {
                saved_count++;
                spdlog::info(tr("save_image_info %0").arg(filename));
            }
            else
            {
                spdlog::error(tr("save_image_error %0").arg(filename));
                emit parsingStatus(tr("save_image_error %0").arg(filename));
            }
        }

        spdlog::info(tr("status_saved_images_count %0 %1").arg(saved_count).arg(images_to_save.size()));
        emit parsingStatus(tr("status_saved_images_count %0 %1").arg(saved_count).arg(images_to_save.size()));

        setEnabled(true);
    }, Qt::SingleShotConnection);

}
} // namespace APP
