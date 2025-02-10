#include "whatsapp_message.hpp"
#include "spdlog_wrapper.hpp"

#include <QUuid>


namespace UTILS
{
WhatsappMessage::WhatsappMessage(Type type, const QString& text, const QDateTime& date_time, const QString& reply_uuid) :
    m_type(type),
    m_text(text),
    m_date_time(date_time),
    m_uuid(QUuid::createUuid().toString(QUuid::WithoutBraces)),
    m_reply_uuid(reply_uuid)
{
}

WhatsappMessage::WhatsappMessage(const WhatsappMessage& other) :
    m_type(other.m_type),
    m_text(other.m_text),
    m_date_time(other.m_date_time),
    m_uuid(other.m_uuid),
    m_reply_uuid(other.m_reply_uuid)
{
}

WhatsappMessage& WhatsappMessage::operator=(const WhatsappMessage& other)
{
    if (this != &other)
    {
        this->m_type       = other.m_type;
        this->m_text       = other.m_text;
        this->m_date_time  = other.m_date_time;
        this->m_uuid       = other.m_uuid;
        this->m_reply_uuid = other.m_reply_uuid;
    }
    return *this;
}

WhatsappMessage::Type WhatsappMessage::getType() const
{
    return this->m_type;
}

QString WhatsappMessage::getText() const
{
    return this->m_text;
}

QDateTime WhatsappMessage::getDateTime() const
{
    return this->m_date_time;
}

QString WhatsappMessage::getUUID() const
{
    return this->m_uuid;
}

QString WhatsappMessage::getReplyUUID() const
{
    return this->m_reply_uuid;
}

void WhatsappMessage::setType(Type type)
{
    this->m_type = type;
}

void WhatsappMessage::setText(const QString& text)
{
    this->m_text = text;
}

void WhatsappMessage::setDateTime(const QDateTime& date_time)
{
    this->m_date_time = date_time;
}

void WhatsappMessage::setReplyUUID(const QString& reply_uuid)
{
    this->m_reply_uuid = reply_uuid;
}

QString WhatsappMessage::typeToString() const
{
    return (this->m_type == Type::Inbound) ? tr("inbound_type") : tr("outbound_type");
}

bool WhatsappMessage::operator<(const WhatsappMessage& other) const
{
    return this->m_date_time < other.m_date_time;
}

void WhatsappMessage::printDebugInfo() const
{
    spdlog::info("================================================");
    spdlog::info(tr("uuid_name %0").arg(this->m_uuid));
    spdlog::info(tr("reply_uuid_name %0").arg(this->m_reply_uuid));
    spdlog::info(tr("type_name %0").arg(this->typeToString()));
    spdlog::info(tr("text_name %0").arg(this->m_text));
    spdlog::info(tr("date_time_name %0").arg(this->m_date_time.toString("dd-MM-yyyy hh:mm:ss")));
    spdlog::info("================================================");
}
}
