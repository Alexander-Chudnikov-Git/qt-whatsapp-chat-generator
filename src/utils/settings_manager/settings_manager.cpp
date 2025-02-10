#include "settings_manager.hpp"
#include "spdlog_wrapper.hpp"
#include "defaults.hpp"


namespace UTILS
{
SettingsManager* SettingsManager::m_instance = nullptr;
QMutex           SettingsManager::m_mutex;
const QMap<SettingsManager::Setting, QPair<QString, QVariant>> SettingsManager::m_settings_lookup =
{
    {
        SettingsManager::Setting::WINDOW_RECT,
        {"window_rect", QRect(0, 0, DEFAULTS::d_window_width, DEFAULTS::d_window_height)}
    },
    {
        SettingsManager::Setting::TRANSLATION_LANG,
        {"translation_lang", DEFAULTS::d_translator_base_locale}
    },
    {
        SettingsManager::Setting::FAKE_IMAGE_PATH,
        {"fake_image_path", DEFAULTS::d_fake_image_path}
    },
    {
        SettingsManager::Setting::FAKE_IMAGE_PREFIX,
        {"fake_image_prefix", DEFAULTS::d_fake_image_prefix}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_PATH,
        {"fake_chat_path", DEFAULTS::d_fake_chat_path}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_WEBSITE,
        {"fake_chat_website", DEFAULTS::d_fake_chat_generator}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_ADS,
        {"fake_chat_ads", DEFAULTS::d_fake_chat_xpath_ads}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_DELAY,
        {"fake_chat_delay", DEFAULTS::d_fake_chat_delay}
    },
    {
        SettingsManager::Setting::FAKE_SCREENSHOT_DELAY,
        {"fake_screenshot_delay", DEFAULTS::d_fake_screenshot_delay}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_HEADER,
        {"fake_chat_hide_header", DEFAULTS::d_fake_chat_hide_header}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_FOOTER,
        {"fake_chat_hide_footer", DEFAULTS::d_fake_chat_hide_footer}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_DARK_THEME,
        {"fake_chat_hide_dark_theme", DEFAULTS::d_fake_chat_hide_dark_theme}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_IPHONE,
        {"fake_chat_hide_iphone", DEFAULTS::d_fake_chat_hide_iphone}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_PERCENTAGE,
        {"fake_chat_hide_percentage", DEFAULTS::d_fake_chat_hide_percentage}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_PAYMENT,
        {"fake_chat_hide_payment", DEFAULTS::d_fake_chat_hide_payment}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_HIDE_DUAL_SIM,
        {"fake_chat_hide_dual_sim", DEFAULTS::d_fake_chat_hide_dual_sim}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_NETWORKS,
        {"fake_chat_networks", DEFAULTS::d_fake_chat_networks}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_NETWORK,
        {"fake_chat_network", DEFAULTS::d_fake_chat_network}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_TIME,
        {"fake_chat_time", DEFAULTS::d_fake_chat_time}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_BATTERY,
        {"fake_chat_battery", DEFAULTS::d_fake_chat_battery}
    },
    {
        SettingsManager::Setting::FAKE_CHAT_NAME,
        {"fake_chat_name", DEFAULTS::d_fake_chat_name}
    },
    {
        SettingsManager::Setting::COUNT,
        {"", QVariant()}
    }
};

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
{
    QMutexLocker locker(&this->m_mutex);

    QCoreApplication::setOrganizationName(DEFAULTS::d_organization_name);
    QCoreApplication::setOrganizationDomain(DEFAULTS::d_organization_website);
    QCoreApplication::setApplicationName(DEFAULTS::d_project_name);

    this->m_settings = new QSettings(this);
    this->m_settings->setDefaultFormat(QSettings::NativeFormat);
    this->loadSettings();


    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &SettingsManager::onApplicationAboutToQuit);
}

SettingsManager::~SettingsManager()
{
    this->saveChanges();
}

SettingsManager* SettingsManager::instance()
{
    if (m_instance == nullptr)
    {
        SPD_INFO_CLASS(DEFAULTS::d_logger_settings_manager, "Initialization complete");
        m_instance = new SettingsManager();
    }
    return m_instance;
}

void SettingsManager::loadSettings()
{
    for (const auto& pair : SettingsManager::m_settings_lookup)
    {
        QString setting_name = pair.first;
        QVariant default_value = pair.second;

        if (setting_name.isEmpty())
        {
            continue;
        }

        SPD_INFO_CLASS(DEFAULTS::d_logger_settings_manager, QString("| Loading setting: %1").arg(setting_name));

        if (!this->m_settings->contains(setting_name))
        {
            if (default_value.isValid())
            {
                this->m_settings->setValue(setting_name, default_value);
                SPD_INFO_CLASS(DEFAULTS::d_logger_settings_manager, QString("| Setting default value for %1: %2").arg(setting_name).arg(default_value.toString()));
            }
            else
            {
                SPD_WARN_CLASS(DEFAULTS::d_logger_settings_manager, QString("| No default value defined for setting: %1").arg(setting_name));
            }
        }
    }

    this->m_changed_settings.clear();

    SPD_INFO_CLASS(DEFAULTS::d_logger_settings_manager, "Settings loaded");

    emit settingsLoaded();
}

void SettingsManager::saveSettings()
{
    QMutexLocker locker(&this->m_mutex);

    this->m_settings->sync();

    SPD_INFO_CLASS(DEFAULTS::d_logger_settings_manager, "Settings saved");

    emit settingsSaved();
}

QVariant SettingsManager::getValue(const QString &key, const QVariant &default_value) const
{
    QMutexLocker locker(&this->m_mutex);

    if (this->m_changed_settings.contains(key))
    {
        return this->m_changed_settings.value(key);
    }
    return m_settings->value(key, default_value);
}

QVariant SettingsManager::getValue(Setting setting, const QVariant &defaultValue) const
{
    return this->getValue(this->m_settings_lookup.value(setting).first, defaultValue);
}

void SettingsManager::setValue(const QString &key, const QVariant &value)
{
    QMutexLocker locker(&this->m_mutex);

    QVariant old_value = this->m_settings->value(key);

    if (old_value != value)
    {
        this->m_changed_settings[key] = value;
        emit settingChanged(key, value);
        emit settingsChanged();
    }
}

void SettingsManager::setValue(Setting setting, const QVariant &value)
{
    return this->setValue(this->m_settings_lookup.value(setting).first, value);
}

bool SettingsManager::isChanged(const QString& key) const
{
	QMutexLocker locker(&this->m_mutex);

	return this->m_changed_settings.contains(key);
}

bool SettingsManager::isChanged(Setting setting) const
{
    return this->isChanged(this->m_settings_lookup.value(setting).first);
}

void SettingsManager::clearChanges()
{
    QMutexLocker locker(&this->m_mutex);

    this->m_changed_settings.clear();
}

void SettingsManager::saveChanges()
{
    QMutexLocker locker(&this->m_mutex);

    for (auto it = this->m_changed_settings.begin(); it != this->m_changed_settings.end(); ++it)
    {
        this->m_settings->setValue(it.key(), it.value());
    }

    this->m_changed_settings.clear();
    this->m_settings->sync();
}

void SettingsManager::discardChanges()
{
    QMutexLocker locker(&this->m_mutex);

    this->m_changed_settings.clear();
    this->loadSettings();
}

void SettingsManager::onApplicationAboutToQuit()
{
    this->saveChanges();
}
}
