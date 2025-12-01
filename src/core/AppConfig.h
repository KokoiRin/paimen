#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>
#include <QSettings>
#include <memory>

// 单例模式 - 应用配置管理
class AppConfig {
public:
    static AppConfig& instance();
    
    // 删除拷贝构造和赋值操作
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;
    
    // 番茄钟配置
    int pomodoroDuration() const { return m_pomodoroDuration; }
    void setPomodoroDuration(int minutes);
    
    int shortBreakDuration() const { return m_shortBreakDuration; }
    void setShortBreakDuration(int minutes);
    
    int longBreakDuration() const { return m_longBreakDuration; }
    void setLongBreakDuration(int minutes);
    
    // 窗口配置
    bool autoStartMiniMode() const { return m_autoStartMiniMode; }
    void setAutoStartMiniMode(bool enabled);
    
    // 记录配置
    QString recordDirectory() const { return m_recordDirectory; }
    void setRecordDirectory(const QString& dir);
    
    // 加载和保存配置
    void load();
    void save();
    
private:
    AppConfig();
    ~AppConfig() = default;
    
    std::unique_ptr<QSettings> m_settings;
    
    int m_pomodoroDuration;
    int m_shortBreakDuration;
    int m_longBreakDuration;
    bool m_autoStartMiniMode;
    QString m_recordDirectory;
};

#endif // APPCONFIG_H
