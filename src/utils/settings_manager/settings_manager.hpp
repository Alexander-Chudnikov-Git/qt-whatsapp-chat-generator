#ifndef SETTINGS_MANAGER_HPP
#define SETTINGS_MANAGER_HPP

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QMap>
#include <QCoreApplication>
#include <QDebug>
#include <QPair>
#include <QMutex>

namespace UTILS
{
class SettingsManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(SettingsManager)
public:
    enum Setting
    {
        WINDOW_RECT,
        TRANSLATION_LANG,

        FAKE_IMAGE_PATH,
        FAKE_IMAGE_PREFIX,

        FAKE_CHAT_PATH,
        FAKE_CHAT_WEBSITE,
        FAKE_CHAT_ADS,
        
        FAKE_CHAT_DELAY,
        FAKE_SCREENSHOT_DELAY,

        FAKE_CHAT_HIDE_HEADER,
        FAKE_CHAT_HIDE_FOOTER,
        FAKE_CHAT_HIDE_DARK_THEME,
        FAKE_CHAT_HIDE_IPHONE,
        FAKE_CHAT_HIDE_PERCENTAGE,
        FAKE_CHAT_HIDE_PAYMENT,
        FAKE_CHAT_HIDE_DUAL_SIM,

        FAKE_CHAT_NETWORKS,
        FAKE_CHAT_NETWORK,
        FAKE_CHAT_TIME,
        FAKE_CHAT_BATTERY,
        FAKE_CHAT_NAME,

        COUNT
    };

private:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager() override;

    void loadSettings();
    void saveSettings();

private:
    static SettingsManager* m_instance;
    static QMutex m_mutex;
    QMap<SettingsManager::Setting, QPair<QString, QVariant>> m_settings_lookup;

    QSettings* m_settings;
    QMap<QString, QVariant> m_changed_settings;

public:
    static SettingsManager* instance();

    QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    QVariant getValue(Setting setting, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    void setValue(Setting setting, const QVariant &value);
    bool isChanged(const QString& key) const;
    bool isChanged(Setting setting) const;
    void clearChanges();
    void saveChanges();
	void discardChanges();

signals:
    void settingChanged(const QString &key, const QVariant &value);
    void settingsChanged();
    void settingsLoaded();
    void settingsSaved();

public slots:
    void onApplicationAboutToQuit();
};
}

#endif // SETTINGS_MANAGER_HPP

