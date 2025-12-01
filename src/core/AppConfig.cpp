#include "AppConfig.h"
#include <QDir>

AppConfig& AppConfig::instance() {
    static AppConfig instance;
    return instance;
}

AppConfig::AppConfig() 
    : m_settings(std::make_unique<QSettings>("PomodoroTimer", "Settings"))
    , m_pomodoroDuration(25)
    , m_shortBreakDuration(5)
    , m_longBreakDuration(15)
    , m_autoStartMiniMode(false)
    , m_recordDirectory(QDir::currentPath())
{
    load();
}

void AppConfig::setPomodoroDuration(int minutes) {
    m_pomodoroDuration = minutes;
    save();
}

void AppConfig::setShortBreakDuration(int minutes) {
    m_shortBreakDuration = minutes;
    save();
}

void AppConfig::setLongBreakDuration(int minutes) {
    m_longBreakDuration = minutes;
    save();
}

void AppConfig::setAutoStartMiniMode(bool enabled) {
    m_autoStartMiniMode = enabled;
    save();
}

void AppConfig::setRecordDirectory(const QString& dir) {
    m_recordDirectory = dir;
    save();
}

void AppConfig::load() {
    m_pomodoroDuration = m_settings->value("timer/pomodoroDuration", 25).toInt();
    m_shortBreakDuration = m_settings->value("timer/shortBreakDuration", 5).toInt();
    m_longBreakDuration = m_settings->value("timer/longBreakDuration", 15).toInt();
    m_autoStartMiniMode = m_settings->value("window/autoStartMiniMode", false).toBool();
    m_recordDirectory = m_settings->value("record/directory", QDir::currentPath()).toString();
}

void AppConfig::save() {
    m_settings->setValue("timer/pomodoroDuration", m_pomodoroDuration);
    m_settings->setValue("timer/shortBreakDuration", m_shortBreakDuration);
    m_settings->setValue("timer/longBreakDuration", m_longBreakDuration);
    m_settings->setValue("window/autoStartMiniMode", m_autoStartMiniMode);
    m_settings->setValue("record/directory", m_recordDirectory);
    m_settings->sync();
}
