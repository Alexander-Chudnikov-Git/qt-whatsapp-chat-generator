#ifndef DEFAULT_HPP
#define DEFAULT_HPP

#include <QStringList>
#include <QCoreApplication>
#include <QDir>

namespace UTILS
{
namespace DEFAULTS
{
constexpr auto d_project_name         = PROJECT_NAME;
constexpr auto d_project_version      = PROJECT_VERSION;
constexpr auto d_application_name     = PROJECT_APPLICATION_NAME;
constexpr auto d_organization_name    = PROJECT_ORGANIZATION_NAME;
constexpr auto d_organization_website = PROJECT_ORGANIZATION_WEBSITE;
constexpr auto d_developer_name       = PROJECT_DEVELOPER_NAME;
constexpr auto d_developer_email      = PROJECT_DEVELOPER_EMAIL;
constexpr auto d_compile_time         = COMPILE_TIME;
constexpr auto d_compile_id           = COMPILER_ID;
constexpr auto d_version              = COMPILER_VERSION;

constexpr auto d_window_width  = 640;
constexpr auto d_window_height = 480;

constexpr auto d_logger_name                = "global_logger";
constexpr auto d_logger_settings_manager    = "settings";
constexpr auto d_logger_translation_namager = "language";

constexpr auto d_translator_base_name   = "lang_";
constexpr auto d_translator_base_dir    = ":/i18n/";
constexpr auto d_translator_base_locale = "en_GB";
constexpr auto d_translator_base_format = ".qm";

const     auto        d_fake_image_path     = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("out");
constexpr auto        d_fake_image_prefix   = "chat_%1_%2.png";

constexpr auto        d_fake_chat_path      = "";
constexpr auto        d_fake_chat_generator = "https://fakedetail.com/fake-whatsapp-chat-generator";
const     QStringList d_fake_chat_xpath_ads = {"/html/iframe", "/html/body/div", "/html/body/div[5]"};

constexpr auto d_fake_chat_hide_header     = false;
constexpr auto d_fake_chat_hide_footer     = false;
constexpr auto d_fake_chat_hide_dark_theme = false;
constexpr auto d_fake_chat_hide_iphone     = false;
constexpr auto d_fake_chat_hide_percentage = false;
constexpr auto d_fake_chat_hide_payment    = false;
constexpr auto d_fake_chat_hide_dual_sim   = false;

const     QStringList d_fake_chat_networks = {"n5g", "n4g", "n3g", "n2g", "nwifi"};
constexpr auto        d_fake_chat_network  = "n4g";
constexpr auto        d_fake_chat_time     = "12:31 AM";
constexpr auto        d_fake_chat_battery  = "17";
constexpr auto        d_fake_chat_name     = "Group Chat";

constexpr auto d_fake_chat_delay       = 150;
constexpr auto d_fake_screenshot_delay = 150;
}
}
#endif // DEFAULT_HPP

