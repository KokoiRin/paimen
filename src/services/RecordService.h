#ifndef RECORDSERVICE_H
#define RECORDSERVICE_H

#include <QString>
#include <QDateTime>
#include <memory>

// 记录数据模型
struct RecordEntry {
    QDateTime timestamp;
    QString content;
    
    RecordEntry(const QDateTime& time, const QString& text)
        : timestamp(time), content(text) {}
};

// 记录服务接口
class IRecordService {
public:
    virtual ~IRecordService() = default;
    virtual bool save(const RecordEntry& entry) = 0;
};

// CSV记录服务实现
class CsvRecordService : public IRecordService {
public:
    explicit CsvRecordService(const QString& directory);
    bool save(const RecordEntry& entry) override;
    
private:
    QString getFilePath(const QDateTime& date) const;
    QString escapeCSV(const QString& text) const;
    
    QString m_directory;
};

// 单例记录管理器
class RecordManager {
public:
    static RecordManager& instance();
    
    RecordManager(const RecordManager&) = delete;
    RecordManager& operator=(const RecordManager&) = delete;
    
    void setService(std::unique_ptr<IRecordService> service);
    bool saveRecord(const QString& content);
    
private:
    RecordManager() = default;
    ~RecordManager() = default;
    
    std::unique_ptr<IRecordService> m_service;
};

#endif // RECORDSERVICE_H
