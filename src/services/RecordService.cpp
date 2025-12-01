#include "RecordService.h"
#include <QFile>
#include <QTextStream>
#include <QDir>

// CsvRecordService 实现
CsvRecordService::CsvRecordService(const QString& directory)
    : m_directory(directory)
{
    QDir dir(m_directory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

bool CsvRecordService::save(const RecordEntry& entry) {
    QString filePath = getFilePath(entry.timestamp);
    bool fileExists = QFile::exists(filePath);
    
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    // 新文件写入BOM和表头
    if (!fileExists) {
        out << QString::fromUtf8("\xef\xbb\xbf");  // UTF-8 BOM
        out << QString::fromUtf8("\xe6\x97\xb6\xe9\x97\xb4") << ","
            << QString::fromUtf8("\xe8\xae\xb0\xe5\xbd\x95\xe5\x86\x85\xe5\xae\xb9") << "\n";
    }
    
    // 写入数据
    QString timeStr = entry.timestamp.toString("HH:mm:ss");
    QString escapedContent = escapeCSV(entry.content);
    out << timeStr << "," << escapedContent << "\n";
    
    file.close();
    return true;
}

QString CsvRecordService::getFilePath(const QDateTime& date) const {
    QString dateStr = date.toString("yyyy-MM-dd");
    return m_directory + "/" + dateStr + ".csv";
}

QString CsvRecordService::escapeCSV(const QString& text) const {
    QString result = text;
    
    // 包含特殊字符时需要转义
    if (result.contains(",") || result.contains("\"") || result.contains("\n")) {
        result.replace("\"", "\"\"");
        result = "\"" + result + "\"";
    }
    
    return result;
}

// RecordManager 实现
RecordManager& RecordManager::instance() {
    static RecordManager instance;
    return instance;
}

void RecordManager::setService(std::unique_ptr<IRecordService> service) {
    m_service = std::move(service);
}

bool RecordManager::saveRecord(const QString& content) {
    if (!m_service) {
        return false;
    }
    
    RecordEntry entry(QDateTime::currentDateTime(), content);
    return m_service->save(entry);
}
