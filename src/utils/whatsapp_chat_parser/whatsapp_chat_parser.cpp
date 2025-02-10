#include "whatsapp_chat_parser.hpp"
#include "spdlog_wrapper.hpp"

#include <QRandomGenerator>

namespace UTILS
{
WhatsappChatParser::WhatsappChatParser(QObject *parent) :
    QObject(parent)
{
}

bool WhatsappChatParser::parseXlsx(const QString& file_path)
{
    this->m_messages.clear();

    QXlsx::Document xlsx(file_path);
    if (!xlsx.load())
    {
        emit parsingStatus(tr("failed_to_load_xlsx_error %0").arg(file_path));
        spdlog::error(tr("failed_to_load_xlsx_error %0").arg(file_path));
        return false;
    }

    int row = 1;
    while (true)
    {
        std::shared_ptr<QXlsx::Cell> cell_inbound  = xlsx.cellAt(row, 1);
        std::shared_ptr<QXlsx::Cell> cell_outbound = xlsx.cellAt(row, 2);
        std::shared_ptr<QXlsx::Cell> cell_date     = xlsx.cellAt(row, 3);

        if (!cell_inbound || cell_outbound->value().isNull())
        {
            break;
        }

        if (!this->parseRow(cell_inbound, cell_outbound, cell_date, row))
        {
            emit parsingStatus(tr("skipping_row_warning %0").arg(row));
            spdlog::warn(tr("skipping_row_warning %0").arg(row));
        }

        row++;
    }

    std::sort(this->m_messages.begin(), this->m_messages.end());
    return true;
}

QList<WhatsappMessage> WhatsappChatParser::getParseResult() const
{
    return this->m_messages;
}

void WhatsappChatParser::printMessages()
{
    emit parsingStatus(tr("total_messages_info %0").arg(this->m_messages.size()));
    spdlog::info(tr("total_messages_info %0").arg(this->m_messages.size()));
    for (const auto &message : this->m_messages)
    {
        message.printDebugInfo();
    }
}

bool WhatsappChatParser::parseRow(std::shared_ptr<QXlsx::Cell> cell_inbound, std::shared_ptr<QXlsx::Cell> cell_outbound, std::shared_ptr<QXlsx::Cell> cell_date, int row)
{
    QString inbound_text = this->readInboundText(cell_inbound);
    QString outbound_text = this->readOutboundText(cell_outbound);
    QDateTime date_time = this->readDateTime(cell_date, row);

    if (date_time.isNull())
    {
        return false;
    }

    QTime base_time(8, 0, 0);
    int seconds_in_day = base_time.secsTo(QTime(23, 0, 0));
    int random_seconds = QRandomGenerator::global()->bounded(seconds_in_day + 1);
    QTime random_inbound_time = base_time.addSecs(random_seconds);

    QDateTime inbound_date_time = QDateTime(date_time.date(), random_inbound_time);

    QString inbound_uuid;

    if (!inbound_text.isEmpty())
    {
        inbound_uuid = this->addMessage(WhatsappMessage::Type::Inbound, inbound_text, inbound_date_time);
    }
    else
    {
        emit parsingStatus(tr("no_inbound_message_warning"));
        spdlog::warn(tr("no_inbound_message_warning"));
        return false;
    }

    int random_outbound_delay = QRandomGenerator::global()->bounded(10, 3601);
    QDateTime outbound_date_time = inbound_date_time.addSecs(random_outbound_delay);

    if (!outbound_text.isEmpty())
    {
        this->addMessage(WhatsappMessage::Type::Outbound, outbound_text, outbound_date_time, inbound_uuid);
    }
    else
    {
        emit parsingStatus(tr("no_outbound_message_warning"));
        spdlog::warn(tr("no_outbound_message_warning")); // This means that there would be no answer
    }

    return true;
}

QString WhatsappChatParser::readInboundText(std::shared_ptr<QXlsx::Cell> cell)
{
    return cell->value().toString();
}

QString WhatsappChatParser::readOutboundText(std::shared_ptr<QXlsx::Cell> cell)
{
    return (cell && !cell->value().isNull()) ? cell->value().toString() : "";
}

QDateTime WhatsappChatParser::readDateTime(std::shared_ptr<QXlsx::Cell> cell, int row)
{
    QDateTime date_time;

    if (cell)
    {
        if (cell->isDateTime())
        {
            date_time = cell->dateTime().toDateTime();
        }
        else
        {
            QString cell_value = cell->value().toString();

            QStringList date_time_formats = {
                "dd.MM.yyyy",
                "dd-MM-yyyy",
                "dd/MM/yyyy",
                "yyyy.MM.dd",
                "yyyy-MM-dd",
                "yyyy/MM/dd",
                "MM.dd.yyyy",
                "MM-dd-yyyy",
                "MM/dd/yyyy",
                "yyyy.dd.MM",
                "yyyy-dd-MM",
                "yyyy/dd/MM"
            };

            for (const QString &format : date_time_formats)
            {
                date_time = QDateTime::fromString(cell_value, format);
                if (date_time.isValid())
                {
                    break;
                }
            }

            if (!date_time.isValid())
            {
                emit parsingStatus(tr("wrong_date_time_warning %0").arg(row));
                spdlog::warn(tr("wrong_date_time_warning %0").arg(row));
                date_time = QDateTime();
            }
        }
    }
    else
    {
        emit parsingStatus(tr("no_date_time_warning %0").arg(row));
        spdlog::warn(tr("no_date_time_warning $0").arg(row));
        date_time = QDateTime();
    }

    return date_time;
}

QString WhatsappChatParser::addMessage(WhatsappMessage::Type type, const QString& text, const QDateTime& date_time, const QString& reply_uuid)
{
    WhatsappMessage message(type, text, date_time, reply_uuid);
    this->m_messages.append(message);
    return message.getUUID();
}

}
