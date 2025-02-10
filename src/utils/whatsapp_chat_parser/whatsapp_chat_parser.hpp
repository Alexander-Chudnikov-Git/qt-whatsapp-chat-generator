#ifndef WHATSAPP_CHAT_PARSER_HPP
#define WHATSAPP_CHAT_PARSER_HPP

#include "whatsapp_message.hpp"

#include <xlsxdocument.h>
#include <QString>
#include <QDateTime>
#include <QList>

namespace UTILS
{
class WhatsappChatParser : public QObject
{
    Q_OBJECT
public:
    explicit WhatsappChatParser(QObject *parent = nullptr);

    bool parseXlsx(const QString& file_path);

    QList<WhatsappMessage> getParseResult() const;

    void printMessages();

signals:
    void parsingStatus(const QString& message);

private:
    bool parseRow(std::shared_ptr<QXlsx::Cell> cell_inbound, std::shared_ptr<QXlsx::Cell> cell_outbound, std::shared_ptr<QXlsx::Cell> cell_date, int row);

    QString   readInboundText(std::shared_ptr<QXlsx::Cell> cell);
    QString   readOutboundText(std::shared_ptr<QXlsx::Cell> cell);
    QDateTime readDateTime(std::shared_ptr<QXlsx::Cell> cell, int row);

    QString addMessage(WhatsappMessage::Type type, const QString& text, const QDateTime& date_time, const QString& reply_uuid = "");

private:
    QList<WhatsappMessage> m_messages;

};
}

#endif // WHATSAPP_CHAT_PARSER_HPP
