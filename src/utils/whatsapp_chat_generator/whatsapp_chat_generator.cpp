#include "whatsapp_chat_generator.hpp"
#include "settings_manager.hpp"
#include "spdlog_wrapper.hpp"

#include <QTimer>
#include <QPainter>
#include <QScreen>
#include <QThread>


namespace UTILS
{

WhatsappChatGenerator::WhatsappChatGenerator(QObject *parent) :
    QObject(parent),
    m_web_view(new QWebEngineView),
    m_web_page(new QWebEnginePage)
{
    this->m_web_view->setPage(m_web_page);
    this->m_web_view->resize(800, 600);

    // Load website once
    connect(m_web_page, &QWebEnginePage::loadFinished, this, [this](bool success)
    {
        QTimer::singleShot(500, this, [this, success]()
        {
            if (success)
            {
                setLocalStorageItem("introjsTourShown", "true");
                emit generatorStatus(tr("webpage_loaded_info"));
                spdlog::info(tr("webpage_loaded_info"));

                auto website_ads = SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_ADS).toStringList();

                for (const auto& ad_xpath : website_ads)
                {
                    removeXPathElement(ad_xpath);
                }

                QTimer::singleShot(3000, this, [this]()
                {
                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_HEADER).toBool())
                    {
                        hideHeader();
                    }
                    else
                    {
                        showHeader();
                    }

                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_FOOTER).toBool())
                    {
                        hideFooter();
                    }
                    else
                    {
                        showFooter();
                    }

                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_IPHONE).toBool())
                    {
                        setAndroidLayout();
                    }
                    else
                    {
                        setIphoneLayout();
                    }

                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_PERCENTAGE).toBool())
                    {
                        hidePercentage();
                    }
                    else
                    {
                        showPercentage();
                    }

                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_PAYMENT).toBool())
                    {
                        hideMoney();
                    }
                    else
                    {
                        showMoney();
                    }

                    if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_DUAL_SIM).toBool())
                    {
                        hideDoubleSim();
                    }
                    else
                    {
                        showDoubleSim();
                    }

                    setNetworkString(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_NETWORK).toString());
                    setTimeString(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_TIME).toTime().toString("H:mm"));
                    setPowerString(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_BATTERY).toString());
                    setTitleString(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_NAME).toString());

                    deleteMessage("2");
                    deleteMessage("1");
                    processNextMessage();
                    removeXPathElement("/html/body/main/div[1]/div/div[3]/div/div[1]/div[5]/div[2]/div/div/div[2]/div[1]/p");
                });
            }
            else
            {
                emit generatorStatus(tr("webpage_failed_to_load_info"));
                spdlog::warn(tr("webpage_failed_to_load_info"));
            }
        });
    });
}

void WhatsappChatGenerator::showBrowser()
{
    this->m_web_view->show();
}

void WhatsappChatGenerator::hideBrowser()
{
    this->m_web_view->hide();
}

bool WhatsappChatGenerator::generateChat(const QList<WhatsappMessage>& messages)
{
    if (messages.isEmpty())
    {
        emit generatorStatus(tr("no_messages_processed_info"));
        spdlog::warn(tr("no_messages_processed_info"));
        return false;
    }

    this->m_messages = QQueue<WhatsappMessage>(messages);
    this->m_chat_screenshots.clear();
    loadWebsite();
    return true;
}

void WhatsappChatGenerator::loadWebsite()
{
    auto website_url = SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_WEBSITE).toString();
    emit generatorStatus(tr("loading_website_info %1").arg(website_url));
    spdlog::info(tr("loading_website_info %1").arg(website_url));
    this->m_web_page->load(QUrl(website_url));
}

void WhatsappChatGenerator::processNextMessage()
{
    if (m_messages.isEmpty())
    {
        emit generatorStatus(tr("messages_processed_info"));
        spdlog::info(tr("messages_processed_info"));

        if (SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_HIDE_DARK_THEME).toBool())
        {
            setLightTheme();
        }
        else
        {
            setDarkTheme();
        }

        QTimer::singleShot(500, this, [this]()
        {
            clickXPathElement("//*[@id=\"preview\"]");
            QTimer::singleShot(500, this, [this]()
            {
                takeScreenshot();
            });
        });
        return;
    }

    const WhatsappMessage msg = m_messages.dequeue();

    createMessage(msg);
    QTimer::singleShot(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_CHAT_DELAY).toInt(), this, &WhatsappChatGenerator::processNextMessage);
}


void WhatsappChatGenerator::takeScreenshot()
{
    QString jsCode = R"(
        (function()
        {
            let element = document.evaluate('//*[@id="preview-modal-content"]', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;

            if (!element)
            {
                return { error: 'Chat container element not found' };
            }

            let rect = element.getBoundingClientRect();
            return {
                x: rect.x,
                y: rect.y,
                width: rect.width,
                height: rect.height
            };
        })();
    )";

    this->m_web_page->runJavaScript(
        jsCode,
        [this](const QVariant &result)
        {
            if (!result.isValid())
            {
                spdlog::error(tr("takescreenshot_js_invalid_result"));
                emit generatorError();
                return;
            }

            if (result.typeId() == QMetaType::QVariantMap)
            {
                QVariantMap resultMap = result.toMap();

                if (resultMap.contains("error"))
                {
                    spdlog::error(tr("takescreenshot_js_error %1").arg(resultMap["error"].toString()));
                    emit generatorError();
                    return;
                }

                if (!resultMap.contains("x") || !resultMap.contains("y") || !resultMap.contains("width") || !resultMap.contains("height"))
                {
                    spdlog::error(tr("takescreenshot_js_missing_dims"));
                    emit generatorError();
                    return;
                }

                bool okX, okY, okWidth, okHeight;
                int x = resultMap["x"].toInt(&okX);
                int y = resultMap["y"].toInt(&okY);
                int width = resultMap["width"].toInt(&okWidth);
                int height = resultMap["height"].toInt(&okHeight);

                if (!okX || !okY || !okWidth || !okHeight)
                {
                    spdlog::error(tr("takescreenshot_js_dims_not_int"));
                    return;
                }


                QRect rect(x, y, width, height);

                if (rect.width() <= 0 || rect.height() <= 0)
                {
                    spdlog::error(tr("invalid_chat_dims %1 %2").arg(rect.width(), rect.height()));
                    emit generatorError();
                    return;
                }

                QPixmap fullScreenshot = this->m_web_view->grab();
                if (fullScreenshot.isNull())
                {
                    spdlog::error(tr("failed_grab_screenshot"));
                    emit generatorError();
                    return;
                }

                QImage cropped = fullScreenshot.toImage().copy(rect);
                if (cropped.isNull())
                {
                    spdlog::error(tr("failed_crop_screenshot"));
                    emit generatorError();
                    return;
                }

                this->m_chat_screenshots.append(cropped);
                emit generatorStatus(tr("screenshot_taken_info"));
                spdlog::info(tr("screenshot_taken_info"));

                QTimer::singleShot(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_SCREENSHOT_DELAY).toInt(), this, &WhatsappChatGenerator::checkScrollPosition);
            }
            else
            {
                spdlog::error(tr("takescreenshot_js_unexpected_type %1").arg(result.typeName()));
                emit generatorError();
            }
        }
    );
}

void WhatsappChatGenerator::checkScrollPosition()
{
    spdlog::info(tr("checking_scroll_pos"));
    QString jsCode = R"(
            (function()
            {
                let element = document.evaluate('/html/body/main/div[2]/div/div/div[2]/div/div/div/div[2]', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
                if (!element)
                {
                    return { scrolled: false, error: 'Scrollable chat element not found' };
                }

                let maxScroll = element.scrollHeight - element.clientHeight;
                let currentScroll = element.scrollTop;

                if (currentScroll < maxScroll)
                {
                    const scrollAmount = Math.min(element.clientHeight * 0.9, maxScroll - currentScroll);
                    element.scrollBy(0, scrollAmount);
                    return { scrolled: true, currentScroll: element.scrollTop, maxScroll: maxScroll };
                }
                else
                {
                    return { scrolled: false, currentScroll: currentScroll, maxScroll: maxScroll };
                }
            })();
        )";

    this->m_web_page->runJavaScript(
        jsCode,
        [this](const QVariant &result)
        {
            if (!result.isValid())
            {
                spdlog::error(tr("checkscroll_js_invalid_result"));
                emit generatorStatus(tr("screenshots_processing_error"));
                return;
            }

            if (result.typeId() == QMetaType::QVariantMap)
            {
                QVariantMap resultMap = result.toMap();

                if (resultMap.contains("error"))
                {
                    spdlog::error(tr("checkscroll_js_error %1").arg(resultMap["error"].toString()));
                    emit generatorStatus(tr("screenshots_processing_error"));
                    return;
                }

                if (resultMap.contains("scrolled") && resultMap["scrolled"].toBool())
                {
                    spdlog::info(tr("scrolled_taking_next_screenshot"));
                    QTimer::singleShot(SettingsManager::instance()->getValue(SettingsManager::Setting::FAKE_SCREENSHOT_DELAY).toInt(), this, &WhatsappChatGenerator::takeScreenshot);
                }
                else
                {
                    spdlog::info(tr("screenshots_processed"));
                    emit generatorStatus(tr("screenshots_processed_info"));
                    spdlog::info(tr("screenshots_processed_info"));
                    emit generatorDone();
                }
            }
            else
            {
                spdlog::error(tr("checkscroll_js_unexpected_type %1").arg(result.typeName()));
                emit generatorStatus(tr("screenshots_processing_error"));
            }
        }
    );
}

void WhatsappChatGenerator::removeClassElement(const QString& class_name)
{
    emit generatorStatus(tr("class_removed_info %1").arg(class_name));
    spdlog::info(tr("class_removed_info %1").arg(class_name));

    QString script = QString("document.querySelector('.%1')?.remove();").arg(class_name);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::removeIDElement(const QString& id_name)
{
    emit generatorStatus(tr("id_removed_info %1").arg(id_name));
    spdlog::info(tr("id_removed_info %1").arg(id_name));

    QString script = QString("document.querySelector('#%1')?.remove();").arg(id_name);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::removeXPathElement(const QString& xpath_name)
{
    emit generatorStatus(tr("xpath_removed_info %1").arg(xpath_name));
    spdlog::info(tr("xpath_removed_info %1").arg(xpath_name));

    QString script = QString(R"(
        (function()
        {
            function removeElement()
            {
                let element = document.evaluate('%1', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
                if (element)
                {
                    element.remove();
                }
            }

            removeElement();

            let observer = new MutationObserver((mutations) =>
            {
                mutations.forEach(() => removeElement());
            });

            observer.observe(document.body, { childList: true, subtree: true });
        })();
    )").arg(xpath_name);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::clickXPathElement(const QString& xpath_name)
{
    emit generatorStatus(tr("xpath_clicked_info %1").arg(xpath_name));
    spdlog::info(tr("xpath_clicked_info %1").arg(xpath_name));

    QString script = QString(R"(
        (function()
        {
            function clickElement()
            {
                let element = document.evaluate('%1', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
                if (element)
                {
                    element.click();
                }
            }

            clickElement();
        })();
    )").arg(xpath_name);

    this->m_web_page->runJavaScript(script);

}

void WhatsappChatGenerator::setLocalStorageItem(const QString &key, const QString &value)
{
    emit generatorStatus(tr("storage_item_set_info %1 %2").arg(key, value));
    spdlog::info(tr("storage_item_set_info %1 %2").arg(key, value));

    QString script = QString(R"(
        (function()
        {
            localStorage.setItem('%1', '%2');
        })();
    )").arg(key, value);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::deleteMessage(const QString& message_id)
{
    emit generatorStatus(tr("delete_message_info %1").arg(message_id));
    spdlog::info(tr("delete_message_info %1").arg(message_id));

    QString script = QString(R"(
            $('#message-box' + %1).remove();
    )").arg(message_id);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::createMessage(const WhatsappMessage& message)
{
    emit generatorStatus(tr("add_message_info %1").arg(message.getUUID()));
    spdlog::info(tr("add_message_info %1").arg(message.getUUID()));

    QString date = message.getDateTime().toString("d MMMM yyyy");

    if (date == this->m_last_date)
    {
        date = "";
    }
    else
    {
        this->m_last_date = date;
    }

    QString script = QString(R"(
        (function()
        {
            $messageSendType = "%5";
            $messageText = "%1";
            $messageTime = "%2";
            $messageImage = '';
            $messageColor = '#000000';
            $senderName = '%3';
            msgdis = '';
            msg_option = '';
            $messageStatus = 'read';
            $messageBoxId = '';
            $messageDate = '%4';
            $flag = 0;

            if (msg_option.trim() != "disappearing")
            {
                if ($messageText.trim() == "" && $messageImage == "")
                {
                    $(".message-input").addClass("error");
                    $flag++;
                }
                else
                {
                    $(".message-input").removeClass("error");
                }

                if ($messageTime.trim() == "")
                {
                    $(".message-time-input").addClass("error");
                    $flag++;
                }
                else
                {
                    $(".message-time-input").removeClass("error");
                }

                if ($messageDate != "")
                {
                    if ($messageDate.match(date_regex))
                    {
                        $messageDate = $messageDate;
                        $(".message-date-input").removeClass("error");
                        $(".error-msg").html("").hide();
                    }
                    else
                    {
                        if ($("#chat_date").val() == "otherdate")
                        {
                            $messageDate = "";
                            $(".error-msg").html('<ul><li>The date format should be like this: ("26 November 2003")</li><li>Make sure the first letters of "Today," "Yesterday," and "Month Name" are capitalized.</li></ul>').show();
                            $(".message-date-input").addClass("error");
                            $flag++;
                        }
                    }
                }
            }
            console.log($flag);
            if ($flag == 0)
            {
                $messageSendType = $messageSendType == "Receive" ? "receive" : "sent";
                $replyHtml = "";

                if ($(".temp-reply-box").html().trim() != "")
                {
                    $replyHtml = $(".temp-reply-box").html();
                }

                var disnum = Number($(".messagedisappearing-box-num").val());
                $data = {msg_option: msg_option, msgdis: msgdis, disnum: disnum, message_box_id: $messageBoxId, message_text: $messageText, message_time: $messageTime, message_send_type: $messageSendType, message_status: $messageStatus, sender_name: $senderName, message_image: $messageImage, message_date: $messageDate, message_color: $messageColor, reply_html: $replyHtml};
                var str = getMessageBoxHtml($data);
                var item = $(str).hide();

                if ($messageBoxId == "")
                {
                    $(".whatsapp-body").append(item);
                    if ($(".whatsapp-body .remove").length > 0)
                    {
                        $(".whatsapp-body .remove").fadeTo(250, 0, function ()
                        {
                            $(".whatsapp-body .remove").remove();
                            item.fadeIn(500);
                        });
                    }
                    else
                    {
                        item.fadeIn(500);
                    }
                }
                else
                {
                    $messageSendTypeCls = $messageSendType == "receive" ? "received" : "sent";
                    $("#message-box" + $messageBoxId).removeClass("message-received message-sent").addClass("message-" + $messageSendTypeCls);
                    $("#message-box" + $messageBoxId).html(item);
                    item.fadeIn(500);
                }
            }

            $(".message-input,.hidden-message-image,.sender-name-input,.message-date-input,.message-image-input,.message-box-id").val("");
            $(".date-label.new").removeClass("new");
            jQuery(".fileinput-new,.message-status-main-div").show();
            jQuery(".file-exists,.sender-name-main-div").hide();
            jQuery(".file-preview,.temp-reply-box").html("");
            $('input[name="message-send-type"]').prop("checked", false);
            $("#sent,#message_sent").prop("checked", true);
            $(".close-btn").click();
        })();
    )"
    ).arg(message.getText(), message.getDateTime().toString("HH:mm"), "", date, (message.getType() == WhatsappMessage::Type::Inbound) ? "Receive" : "Sent");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::hideHeader()
{
    emit generatorStatus(tr("hide_header_info"));
    spdlog::info(tr("hide_header_info"));

    QString script = QString(R"(
            $(".android-header").hide();
            $(".ios-header").hide()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::showHeader()
{
    emit generatorStatus(tr("show_header_info"));
    spdlog::info(tr("show_header_info"));

    QString script = QString(R"(
            $(".android-header").show();
            $(".ios-header").show()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::hideFooter()
{
    emit generatorStatus(tr("hide_footer_info"));
    spdlog::info(tr("hide_footer_info"));

    QString script = QString(R"(
            $(".all_footer_wp").hide()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::hideDoubleSim()
{
    emit generatorStatus(tr("hide_doublesim_info"));
    spdlog::info(tr("hide_doublesim_info"));

    QString script = QString(R"(
            $(".network-two").removeClass('hide')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::showDoubleSim()
{
    emit generatorStatus(tr("show_doublesim_info"));
    spdlog::info(tr("show_doublesim_info"));

    QString script = QString(R"(
            $(".network-two").addClass('hide')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::hidePercentage()
{
    emit generatorStatus(tr("hide_percentage_info"));
    spdlog::info(tr("hide_percentage_info"));

    QString script = QString(R"(
            $(".battery-container").removeClass('hide-percentage')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::showPercentage()
{
    emit generatorStatus(tr("show_percentage_info"));
    spdlog::info(tr("show_percentage_info"));

    QString script = QString(R"(
            $(".battery-container").addClass('hide-percentage')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::hideMoney()
{
    emit generatorStatus(tr("hide_money_info"));
    spdlog::info(tr("hide_money_info"));

    QString script = QString(R"(
            $(".moneyicon").hide()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::showMoney()
{
    emit generatorStatus(tr("show_money_info"));
    spdlog::info(tr("show_money_info"));

    QString script = QString(R"(
            $(".moneyicon").show()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::showFooter()
{
    emit generatorStatus(tr("show_footer_info"));
    spdlog::info(tr("show_footer_info"));

    QString script = QString(R"(
            $(".all_footer_wp").show()
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setDarkTheme()
{
    emit generatorStatus(tr("set_dark_theme_info"));
    spdlog::info(tr("set_dark_theme_info"));

    QString script = QString(R"(
            jQuery(".wa-theme").addClass('dark-theme');
            $('.msg-arrow-left').css('fill', '#202c33');
            $('.msg-arrow-right').css('fill', '#005c4b')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setLightTheme()
{
    emit generatorStatus(tr("set_light_theme_info"));
    spdlog::info(tr("set_light_theme_info"));

    QString script = QString(R"(
            jQuery(".wa-theme").removeClass('dark-theme');
            $('.msg-arrow-left').css('fill', '#ffffff');
            $('.msg-arrow-right').css('fill', '#e6ffda')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setIphoneLayout()
{
    emit generatorStatus(tr("set_iphone_layout_info"));
    spdlog::info(tr("set_iphone_layout_info"));

    QString script = QString(R"(
            jQuery(".android-header.white-content").hide();
            jQuery(".whatsapp-text-img.received-carat").hide();
            jQuery(".whatsapp-text-img.sent-carat").hide();
            jQuery(".iphone-bottom,.iphone-date,.iphone-date-text").show();
            jQuery(".whatapp-android-footer,.date,.date-text").hide();
            jQuery(".wp-android").hide();
            jQuery(".wp-ios").show();
            jQuery(".whatapp-ios-footer").show();
            jQuery(".whatsapp-body").addClass('whatsapp-body-ios');
            jQuery(".chat-timer-wrap").addClass('chat-timer-ios');
            $(".battery-status").text($('#Battery_percent').val())
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setAndroidLayout()
{
    emit generatorStatus(tr("set_android_layout_info"));
    spdlog::info(tr("set_android_layout_info"));

    QString script = QString(R"(
            jQuery(".android-header.white-content,.whatsapp-text-img.received-carat").show();
            jQuery(".whatsapp-text-img.sent-carat").show();
            jQuery(".iphone-bottom,.iphone-date,.iphone-date-text").hide();
            jQuery(".whatapp-android-footer,.date-text").show();
            jQuery(".wp-android").show();
            jQuery(".wp-ios").hide();
            jQuery(".whatapp-ios-footer").hide();
            jQuery(".chat-timer-wrap").removeClass('chat-timer-ios');
            jQuery(".whatsapp-body").removeClass('whatsapp-body-ios');
            $(".battery-status").text($('#Battery_percent').val() + '%')
    )");

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setTimeString(const QString& time_string)
{
    emit generatorStatus(tr("set_time_info %1").arg(time_string));
    spdlog::info(tr("set_time_info %1").arg(time_string));

    QString script = QString(R"(
            $(".status-time").text("%1");
    )").arg(time_string);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setPowerString(const QString& power)
{
    emit generatorStatus(tr("set_power_info %1").arg(power));
    spdlog::info(tr("set_power_info %1").arg(power));

    QString script = QString(R"(
            var count = %1;
            if (count > 100) count = 100;
            if (count < 0) count = 0;
            $("#Battery_percent").val(count);
            $(".battery-level").removeClass("red");

            if (count <= 15)
            {
                $(".battery-level").addClass("red");
            }

            $(".battery-level").css("width", count + "%");

            if ($("input#iphone_wp").is(":checked"))
            {
                $(".battery-status").text(count);
            }
            else
            {
                $(".battery-status").text(count + "%");
            }
    )").arg(power);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setNetworkString(const QString& network)
{
    emit generatorStatus(tr("set_network_info %1").arg(network));
    spdlog::info(tr("set_network_info %1").arg(network));

    QString script = QString(R"(
            $('.show-network').removeClass('n5g n4g n3g n2g nwifi');
            $('.show-network').addClass('%1')
    )").arg(network);

    this->m_web_page->runJavaScript(script);
}

void WhatsappChatGenerator::setTitleString(const QString& title)
{
    emit generatorStatus(tr("set_title_info %1").arg(title));
    spdlog::info(tr("set_title_info %1").arg(title));

    QString script = QString(R"(
        element = document.evaluate('/html/body/main/div[1]/div/div[3]/div/div[1]/div[5]/div[2]/div/div/div[1]/div[1]/div[2]/div/div[2]/div[2]/div[1]/p/span', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;

        if (element)
        {
            element.textContent = "%1";
        }

        element = document.evaluate('/html/body/main/div[1]/div/div[3]/div/div[1]/div[5]/div[2]/div/div/div[1]/div[2]/div[2]/div/div[3]/div[2]/div[1]/p/span', document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;

        if (element)
        {
            element.textContent = "%1";
        }
    )").arg(title);

    this->m_web_page->runJavaScript(script);
}

QList<QImage> WhatsappChatGenerator::getGeneratorResult() const
{
    return m_chat_screenshots;
}

} // namespace UTILS

