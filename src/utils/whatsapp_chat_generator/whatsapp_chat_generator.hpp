#ifndef WHATSAPP_CHAT_GENERATOR_HPP
#define WHATSAPP_CHAT_GENERATOR_HPP

#include "whatsapp_message.hpp"

#include <QObject>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QQueue>
#include <QList>
#include <QImage>

namespace UTILS
{
class WhatsappChatGenerator : public QObject
{
    Q_OBJECT
public:
    explicit WhatsappChatGenerator(QObject *parent = nullptr);

    bool generateChat(const QList<WhatsappMessage>& messages);
    QList<QImage> getGeneratorResult() const;

    void showBrowser();
    void hideBrowser();

signals:
    void generatorStatus(const QString& message);
    void generatorDone();
    void generatorError();

private slots:
    void takeScreenshot();
    void checkScrollPosition();

private:
    void loadWebsite();
    void processNextMessage();

    void removeClassElement(const QString& class_name);
    void removeIDElement(const QString& id_name);
    void removeXPathElement(const QString& xpath_name);

    void clickXPathElement(const QString& xpath_name);

    void setLocalStorageItem(const QString &key, const QString &value);

    // Custom JS stuff
    void deleteMessage(const QString& message_id);
    void createMessage(const WhatsappMessage& message);
    void hideHeader();
    void showHeader();
    void hidePercentage();
    void showPercentage();
    void hideDoubleSim();
    void showDoubleSim();
    void hideMoney();
    void showMoney();
    void hideFooter();
    void showFooter();
    void setDarkTheme();
    void setLightTheme();
    void setIphoneLayout();
    void setAndroidLayout();
    void setTimeString(const QString& time);
    void setPowerString(const QString& power);
    void setNetworkString(const QString& network);
    void setTitleString(const QString& title);

private:
    QWebEngineView *m_web_view;
    QWebEnginePage *m_web_page;

    QQueue<WhatsappMessage> m_messages;
    QList<QImage> m_chat_screenshots;

    int m_currentMessageIndex;

    QString m_last_date;
};
} // namespace UTILS

#endif // WHATSAPP_CHAT_GENERATOR_HPP

