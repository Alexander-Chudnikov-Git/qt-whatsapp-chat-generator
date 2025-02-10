#ifndef TRANSLATION_MANAGER_HPP
#define TRANSLATION_MANAGER_HPP

#include <QTranslator>
#include <QLocale>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>
#include <mutex>

class QMutex;

namespace UTILS
{
class TranslationManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TranslationManager)
public:
    static TranslationManager* instance();

    ~TranslationManager();

    bool loadTranslation(const QString& locale);
    void loadSystemTranslation();
    void loadDefaultTranslation();

    QStringList getAvailableTranslations() const;
    QString getCurrentLocale() const;

private:
    TranslationManager();

private:
    static TranslationManager* m_instance;
    static QMutex m_mutex;

    QString m_base_name;
    QString m_translations_dir;
    QString m_default_locale;
    QString m_default_format;

    QTranslator* m_current_translator;
    QString m_current_locale;
};
}

#endif // TRANSLATION_MANAGER_HPP
