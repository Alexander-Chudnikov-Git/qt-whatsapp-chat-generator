#ifndef WHATSAPP_MESSAGE_HPP
#define WHATSAPP_MESSAGE_HPP

#include <QString>
#include <QDateTime>
#include <QVariant>


namespace UTILS
{
class WhatsappMessage : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        Inbound,
        Outbound
    };

    WhatsappMessage(Type type = Type::Inbound, const QString& text = "", const QDateTime& date_time = QDateTime(), const QString& reply_uuid = "");
    WhatsappMessage(const WhatsappMessage& other);

    WhatsappMessage& operator=(const WhatsappMessage& other);

    Type getType() const;
    QString getText() const;
    QDateTime getDateTime() const;
    QString getUUID() const;
    QString getReplyUUID() const;

    void setType(Type type);
    void setText(const QString& text);
    void setDateTime(const QDateTime& date_time);
    void setReplyUUID(const QString& reply_uuid);

    QString typeToString() const;

    bool operator<(const WhatsappMessage& other) const;

    void printDebugInfo() const;

private:
    Type m_type;
    QString m_text;
    QDateTime m_date_time;

    QString m_uuid;
    QString m_reply_uuid;
};
}

#endif // WHATSAPP_MESSAGE_HPP
