#include "MainWindow.h"
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_timer(new PomodoroTimer(this))
{
    // 启用双缓冲减少闪烁
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground, false);
    
    setupUI();
    applyStyles();
    
    connect(m_timer, &PomodoroTimer::timeChanged, this, &MainWindow::onTimeChanged);
    connect(m_timer, &PomodoroTimer::stateChanged, this, &MainWindow::onStateChanged);
    connect(m_timer, &PomodoroTimer::pomodoroCompleted, this, &MainWindow::onPomodoroCompleted);
    
    // 初始化显示
    onTimeChanged(m_timer->remainingSeconds());
    onStateChanged(m_timer->state());
}

void MainWindow::setupUI() {
    setWindowTitle(QString::fromUtf8("\xe7\x95\xaa\xe8\x8c\x84\xe9\x92\x9f - \xe6\x97\xb6\xe9\x97\xb4\xe7\xae\xa1\xe7\x90\x86\xe5\xb7\xa5\xe5\x85\xb7"));
    setFixedSize(400, 500);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题
    QLabel *titleLabel = new QLabel(QString::fromUtf8("\xf0\x9f\x8d\x85 \xe7\x95\xaa\xe8\x8c\x84\xe9\x92\x9f"));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    
    // 时间显示
    m_timeLabel = new QLabel("25:00");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setObjectName("timeLabel");
    m_timeLabel->setFixedHeight(120);  // 固定高度避免重叠
    QFont timeFont = m_timeLabel->font();
    timeFont.setPointSize(56);
    timeFont.setBold(true);
    m_timeLabel->setFont(timeFont);
    
    // 状态标签
    m_statusLabel = new QLabel(QString::fromUtf8("\xe5\x87\x86\xe5\xa4\x87\xe5\xbc\x80\xe5\xa7\x8b"));
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setObjectName("statusLabel");
    QFont statusFont = m_statusLabel->font();
    statusFont.setPointSize(14);
    m_statusLabel->setFont(statusFont);
    
    // 控制按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    m_startButton = new QPushButton(QString::fromUtf8("\xe5\xbc\x80\xe5\xa7\x8b"));
    m_pauseButton = new QPushButton(QString::fromUtf8("\xe6\x9a\x82\xe5\x81\x9c"));
    m_resetButton = new QPushButton(QString::fromUtf8("\xe9\x87\x8d\xe7\xbd\xae"));
    
    m_startButton->setObjectName("startButton");
    m_pauseButton->setObjectName("pauseButton");
    m_resetButton->setObjectName("resetButton");
    
    m_startButton->setMinimumHeight(50);
    m_pauseButton->setMinimumHeight(50);
    m_resetButton->setMinimumHeight(50);
    
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(m_pauseButton);
    buttonLayout->addWidget(m_resetButton);
    
    // 迷你模式按钮
    m_miniModeButton = new QPushButton(QString::fromUtf8("\xe5\x88\x87\xe6\x8d\xa2\xe5\x88\xb0\xe8\xbf\xb7\xe4\xbd\xa0\xe6\xa8\xa1\xe5\xbc\x8f"));
    m_miniModeButton->setObjectName("miniModeButton");
    m_miniModeButton->setMinimumHeight(40);
    
    // 添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_timeLabel);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_miniModeButton);
    mainLayout->addStretch();
    
    // 连接信号
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(m_miniModeButton, &QPushButton::clicked, this, &MainWindow::onMiniModeClicked);
}

void MainWindow::applyStyles() {
    setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
        }
        
        #titleLabel {
            color: white;
            margin-bottom: 10px;
        }
        
        #timeLabel {
            color: white;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            padding: 20px 30px;
            margin: 10px 20px;
            min-height: 100px;
        }
        
        #statusLabel {
            color: rgba(255, 255, 255, 0.9);
            margin-top: 10px;
        }
        
        QPushButton {
            background-color: rgba(255, 255, 255, 0.2);
            color: white;
            border: 2px solid rgba(255, 255, 255, 0.3);
            border-radius: 10px;
            font-size: 14px;
            font-weight: bold;
            padding: 10px;
        }
        
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.3);
            border: 2px solid rgba(255, 255, 255, 0.5);
        }
        
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.15);
        }
        
        #startButton {
            background-color: rgba(72, 187, 120, 0.8);
        }
        
        #startButton:hover {
            background-color: rgba(72, 187, 120, 1);
        }
        
        #pauseButton {
            background-color: rgba(237, 137, 54, 0.8);
        }
        
        #pauseButton:hover {
            background-color: rgba(237, 137, 54, 1);
        }
        
        #resetButton {
            background-color: rgba(245, 101, 101, 0.8);
        }
        
        #resetButton:hover {
            background-color: rgba(245, 101, 101, 1);
        }
        
        #miniModeButton {
            background-color: rgba(66, 153, 225, 0.8);
        }
        
        #miniModeButton:hover {
            background-color: rgba(66, 153, 225, 1);
        }
    )");
}

void MainWindow::onTimeChanged(int remainingSeconds) {
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    m_timeLabel->setText(QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::onStateChanged(PomodoroTimer::State state) {
    switch (state) {
    case PomodoroTimer::Idle:
        m_statusLabel->setText(QString::fromUtf8("\xe5\x87\x86\xe5\xa4\x87\xe5\xbc\x80\xe5\xa7\x8b"));
        m_startButton->setEnabled(true);
        m_pauseButton->setEnabled(false);
        break;
    case PomodoroTimer::Running:
        m_statusLabel->setText(QString::fromUtf8("\xe4\xb8\x93\xe6\xb3\xa8\xe4\xb8\xad..."));
        m_startButton->setEnabled(false);
        m_pauseButton->setEnabled(true);
        break;
    case PomodoroTimer::Paused:
        m_statusLabel->setText(QString::fromUtf8("\xe5\xb7\xb2\xe6\x9a\x82\xe5\x81\x9c"));
        m_startButton->setEnabled(true);
        m_pauseButton->setEnabled(false);
        break;
    }
}

void MainWindow::onPomodoroCompleted() {
    QString title = QString::fromUtf8("\xe7\x95\xaa\xe8\x8c\x84\xe9\x92\x9f");
    QString message = QString::fromUtf8("\xf0\x9f\x8e\x89 \xe6\x81\xad\xe5\x96\x9c\xef\xbc\x81\xe4\xbd\xa0\xe5\xae\x8c\xe6\x88\x90\xe4\xba\x86\xe4\xb8\x80\xe4\xb8\xaa\xe7\x95\xaa\xe8\x8c\x84\xe9\x92\x9f\xef\xbc\x81\n\n\xe4\xbc\x91\xe6\x81\xaf\xe4\xb8\x80\xe4\xb8\x8b\xe5\x90\xa7\xef\xbd\x9e");
    QMessageBox::information(this, title, message);
    m_statusLabel->setText(QString::fromUtf8("\xe5\xae\x8c\xe6\x88\x90\xef\xbc\x81"));
}

void MainWindow::onStartClicked() {
    m_timer->start();
}

void MainWindow::onPauseClicked() {
    m_timer->pause();
}

void MainWindow::onResetClicked() {
    m_timer->reset();
}

void MainWindow::onMiniModeClicked() {
    emit switchToMiniMode();
}
