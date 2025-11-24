#include "MiniWindow.h"
#include <QHBoxLayout>
#include <QFont>
#include <QApplication>

MiniWindow::MiniWindow(QWidget *parent)
    : QWidget(parent)
    , m_timer(new PomodoroTimer(this))
    , m_dragging(false)
{
    setupUI();
    applyStyles();
    
    // 设置窗口属性：无边框、置顶、工具窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    // 启用硬件加速提升性能
    setAttribute(Qt::WA_NativeWindow);
    
    connect(m_timer, &PomodoroTimer::timeChanged, this, &MiniWindow::onTimeChanged);
    connect(m_timer, &PomodoroTimer::stateChanged, this, &MiniWindow::onStateChanged);
    connect(m_timer, &PomodoroTimer::pomodoroCompleted, this, &MiniWindow::onPomodoroCompleted);
    
    // 初始化显示
    onTimeChanged(m_timer->remainingSeconds());
    onStateChanged(m_timer->state());
}

void MiniWindow::setupUI() {
    setFixedSize(320, 120);  // 增大窗口确保显示完整
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    
    // 顶部：时间和状态指示器
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    m_statusIndicator = new QLabel(QString::fromUtf8("\xe2\x97\x8f"));
    m_statusIndicator->setObjectName("statusIndicator");
    m_statusIndicator->setFixedWidth(30);  // 固定宽度
    QFont indicatorFont = m_statusIndicator->font();
    indicatorFont.setPointSize(18);  // 适中的大小
    m_statusIndicator->setFont(indicatorFont);
    
    m_timeLabel = new QLabel("25:00");
    m_timeLabel->setObjectName("timeLabel");
    m_timeLabel->setMinimumWidth(120);  // 减小宽度，更紧凑
    m_timeLabel->setMaximumWidth(140);  // 限制最大宽度
    QFont timeFont = m_timeLabel->font();
    timeFont.setPointSize(28);  // 减小字体以适应窗口
    timeFont.setBold(true);
    m_timeLabel->setFont(timeFont);
    
    topLayout->addWidget(m_statusIndicator);
    topLayout->addWidget(m_timeLabel, 1);  // 添加拉伸因子
    topLayout->addStretch();
    
    // 底部：控制按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(6);  // 减小间距以容纳4个按钮
    
    m_startPauseButton = new QPushButton(QString::fromUtf8("\xe2\x96\xb6"));
    m_resetButton = new QPushButton(QString::fromUtf8("\xe2\x86\xbb"));
    m_recordButton = new QPushButton(QString::fromUtf8("\xe2\x9c\x8f"));  // 笔记符号
    m_expandButton = new QPushButton(QString::fromUtf8("\xe2\xac\x9c"));
    
    m_startPauseButton->setObjectName("controlButton");
    m_resetButton->setObjectName("controlButton");
    m_recordButton->setObjectName("recordButton");
    m_expandButton->setObjectName("expandButton");
    
    m_startPauseButton->setFixedSize(42, 35);  // 稍微缩小以容纳4个按钮
    m_resetButton->setFixedSize(42, 35);
    m_recordButton->setFixedSize(42, 35);
    m_expandButton->setFixedSize(42, 35);
    
    m_startPauseButton->setToolTip(QString::fromUtf8("\xe5\xbc\x80\xe5\xa7\x8b/\xe6\x9a\x82\xe5\x81\x9c"));
    m_resetButton->setToolTip(QString::fromUtf8("\xe9\x87\x8d\xe7\xbd\xae"));
    m_recordButton->setToolTip(QString::fromUtf8("\xe8\xae\xb0\xe5\xbd\x95\xe5\xbd\x93\xe5\x89\x8d\xe6\x97\xb6\xe9\x97\xb4"));
    m_expandButton->setToolTip(QString::fromUtf8("\xe5\xb1\x95\xe5\xbc\x80\xe7\xaa\x97\xe5\x8f\xa3"));
    
    buttonLayout->addWidget(m_startPauseButton);
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addWidget(m_recordButton);  // 添加记录按钮在重置和展开之间
    buttonLayout->addWidget(m_expandButton);
    buttonLayout->addStretch();  // 拉伸放在最后
    
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(m_startPauseButton, &QPushButton::clicked, this, &MiniWindow::onStartPauseClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MiniWindow::onResetClicked);
    connect(m_recordButton, &QPushButton::clicked, this, &MiniWindow::onRecordClicked);
    connect(m_expandButton, &QPushButton::clicked, this, &MiniWindow::onExpandClicked);
}

void MiniWindow::applyStyles() {
    setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #FF3E5C, stop:0.3 #FF6B35, stop:0.6 #4ECDC4, stop:1 #3D5AFE);
            border-radius: 18px;
            border: 3px solid rgba(255, 255, 255, 0.6);
            color: white;
        }
        
        #timeLabel {
            color: #FFFFFF;
            background: rgba(0, 0, 0, 0.4);
            border-radius: 8px;
            padding: 4px 10px;
            font-weight: bold;
        }
        
        #statusIndicator {
            color: #FFEB3B;
            background: transparent;
            border: none;
            font-weight: bold;
        }
        
        QPushButton {
            background-color: rgba(255, 255, 255, 0.5);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.7);
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.75);
            border: 2px solid white;
        }
        
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.35);
        }
        
        #controlButton {
            font-size: 18px;
            background-color: rgba(76, 175, 80, 0.7);
        }
        
        #controlButton:hover {
            background-color: rgba(76, 175, 80, 0.9);
        }
        
        #recordButton {
            font-size: 16px;
            background-color: rgba(33, 150, 243, 0.7);
        }
        
        #recordButton:hover {
            background-color: rgba(33, 150, 243, 0.9);
        }
        
        #expandButton {
            font-size: 14px;
            background-color: rgba(255, 152, 0, 0.7);
        }
        
        #expandButton:hover {
            background-color: rgba(255, 152, 0, 0.9);
        }
    )");
}

void MiniWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MiniWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MiniWindow::onTimeChanged(int remainingSeconds) {
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    m_timeLabel->setText(QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void MiniWindow::onStateChanged(PomodoroTimer::State state) {
    switch (state) {
    case PomodoroTimer::Idle:
        m_statusIndicator->setStyleSheet("color: #BDBDBD; text-shadow: 0 0 10px #FFFFFF;");
        m_startPauseButton->setText(QString::fromUtf8("\xe2\x96\xb6"));
        m_startPauseButton->setToolTip(QString::fromUtf8("\xe5\xbc\x80\xe5\xa7\x8b"));
        break;
    case PomodoroTimer::Running:
        m_statusIndicator->setStyleSheet("color: #00E676; text-shadow: 0 0 20px #00E676;");
        m_startPauseButton->setText(QString::fromUtf8("\xe2\x8f\xb8"));
        m_startPauseButton->setToolTip(QString::fromUtf8("\xe6\x9a\x82\xe5\x81\x9c"));
        break;
    case PomodoroTimer::Paused:
        m_statusIndicator->setStyleSheet("color: #FFD600; text-shadow: 0 0 20px #FFD600;");
        m_startPauseButton->setText(QString::fromUtf8("\xe2\x96\xb6"));
        m_startPauseButton->setToolTip(QString::fromUtf8("\xe7\xbb\xa7\xe7\xbb\xad"));
        break;
    }
}

void MiniWindow::onPomodoroCompleted() {
    // 闪烁效果提示完成
    m_statusIndicator->setStyleSheet("color: #FF1744; text-shadow: 0 0 25px #FF1744, 0 0 35px #FF1744;");
}

void MiniWindow::onStartPauseClicked() {
    if (m_timer->state() == PomodoroTimer::Running) {
        m_timer->pause();
    } else {
        m_timer->start();
    }
}

void MiniWindow::onResetClicked() {
    m_timer->reset();
}

void MiniWindow::onExpandClicked() {
    emit switchToNormalMode();
}

void MiniWindow::onRecordClicked() {
    // 获取当前时间（24小时制）
    QString currentTime = QDateTime::currentDateTime().toString("HH:mm:ss");
    
    // 创建自定义对话框
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(QString::fromUtf8("\xe8\xae\xb0\xe5\xbd\x95\xe6\x97\xb6\xe9\x97\xb4"));
    dialog->setFixedSize(400, 250);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    
    // 设置对话框样式
    dialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #667eea, stop:1 #764ba2);
            border-radius: 10px;
        }
        QLabel {
            color: white;
            font-size: 14px;
            background: transparent;
        }
        #timeLabel {
            font-size: 18px;
            font-weight: bold;
            color: #FFE66D;
        }
        QTextEdit {
            background-color: rgba(255, 255, 255, 0.9);
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 8px;
            padding: 8px;
            font-size: 13px;
            color: #333;
        }
        QTextEdit:focus {
            border: 2px solid #FFE66D;
        }
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.9);
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 13px;
            color: #333;
            min-height: 30px;
        }
        QLineEdit:focus {
            border: 2px solid #FFE66D;
        }
        QPushButton {
            background-color: rgba(255, 255, 255, 0.2);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 14px;
            font-weight: bold;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.35);
            border: 2px solid white;
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.15);
        }
        #okButton {
            background-color: rgba(76, 175, 80, 0.8);
        }
        #okButton:hover {
            background-color: rgba(76, 175, 80, 1);
        }
        #cancelButton {
            background-color: rgba(245, 101, 101, 0.8);
        }
        #cancelButton:hover {
            background-color: rgba(245, 101, 101, 1);
        }
    )");
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);
    
    // 时间显示标签
    QLabel *timeLabel = new QLabel(QString::fromUtf8("\xe2\x8f\xb0 ") + currentTime);
    timeLabel->setObjectName("timeLabel");
    timeLabel->setAlignment(Qt::AlignCenter);
    
    // 提示标签
    QLabel *hintLabel = new QLabel(QString::fromUtf8("\xe8\xaf\xb7\xe8\xbe\x93\xe5\x85\xa5\xe4\xbd\xa0\xe5\x81\x9a\xe4\xba\x86\xe4\xbb\x80\xe4\xb9\x88\xef\xbc\x9a"));
    
    // 文本输入框（使用QLineEdit单行，支持回车保存）
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText(QString::fromUtf8("\xe4\xbe\x8b\xe5\xa6\x82\xef\xbc\x9a\xe5\xae\x8c\xe6\x88\x90\xe4\xba\x86\xe7\xac\xac\xe4\xb8\x80\xe4\xb8\xaa\xe7\x95\xaa\xe8\x8c\x84\xe9\x92\x9f\xe2\x80\xa6"));
    
    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    QPushButton *okButton = new QPushButton(QString::fromUtf8("\xe2\x9c\x93 \xe4\xbf\x9d\xe5\xad\x98"));
    QPushButton *cancelButton = new QPushButton(QString::fromUtf8("\xe2\x9c\x95 \xe5\x8f\x96\xe6\xb6\x88"));
    
    okButton->setObjectName("okButton");
    cancelButton->setObjectName("cancelButton");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    
    // 添加到主布局
    layout->addWidget(timeLabel);
    layout->addWidget(hintLabel);
    layout->addWidget(lineEdit);
    layout->addSpacing(10);
    layout->addLayout(buttonLayout);
    
    // 连接信号 - 回车键直接保存
    connect(lineEdit, &QLineEdit::returnPressed, dialog, &QDialog::accept);
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    
    // 显示对话框
    lineEdit->setFocus();
    if (dialog->exec() == QDialog::Accepted) {
        QString text = lineEdit->text().trimmed();
        if (!text.isEmpty()) {
            saveToExcel(currentTime, text);
        }
    }
    
    dialog->deleteLater();
}

void MiniWindow::saveToExcel(const QString &time, const QString &content) {
    // 获取当前日期作为文件名
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString fileName = dateStr + ".csv";  // 使用CSV格式，Excel可以直接打开
    
    // 确保在可执行文件同目录下
    QString filePath = QDir::currentPath() + "/" + fileName;
    
    bool fileExists = QFile::exists(filePath);
    QFile file(filePath);
    
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);  // Qt6使用setEncoding
        
        // 如果是新文件，写入表头
        if (!fileExists) {
            out << QString::fromUtf8("\xef\xbb\xbf");  // UTF-8 BOM，让Excel正确识别编码
            out << QString::fromUtf8("\xe6\x97\xb6\xe9\x97\xb4") << ","
                << QString::fromUtf8("\xe8\xae\xb0\xe5\xbd\x95\xe5\x86\x85\xe5\xae\xb9") << "\n";
        }
        
        // 写入数据，处理逗号和换行符
        QString escapedContent = content;
        if (escapedContent.contains(",") || escapedContent.contains("\"") || escapedContent.contains("\n")) {
            escapedContent.replace("\"", "\"\"");  // 转义引号
            escapedContent = "\"" + escapedContent + "\"";  // 用引号包裹
        }
        
        out << time << "," << escapedContent << "\n";
        file.close();
    }
}
