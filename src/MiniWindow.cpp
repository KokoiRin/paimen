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
    buttonLayout->setSpacing(8);
    
    m_startPauseButton = new QPushButton(QString::fromUtf8("\xe2\x96\xb6"));
    m_resetButton = new QPushButton(QString::fromUtf8("\xe2\x86\xbb"));
    m_expandButton = new QPushButton(QString::fromUtf8("\xe2\xac\x9c"));
    
    m_startPauseButton->setObjectName("controlButton");
    m_resetButton->setObjectName("controlButton");
    m_expandButton->setObjectName("expandButton");
    
    m_startPauseButton->setFixedSize(45, 35);  // 增大按钮
    m_resetButton->setFixedSize(45, 35);
    m_expandButton->setFixedSize(45, 35);
    
    m_startPauseButton->setToolTip(QString::fromUtf8("\xe5\xbc\x80\xe5\xa7\x8b/\xe6\x9a\x82\xe5\x81\x9c"));
    m_resetButton->setToolTip(QString::fromUtf8("\xe9\x87\x8d\xe7\xbd\xae"));
    m_expandButton->setToolTip(QString::fromUtf8("\xe5\xb1\x95\xe5\xbc\x80\xe7\xaa\x97\xe5\x8f\xa3"));
    
    buttonLayout->addWidget(m_startPauseButton);
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addWidget(m_expandButton);  // 直接排在一起
    buttonLayout->addStretch();  // 拉伸放在最后
    
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(m_startPauseButton, &QPushButton::clicked, this, &MiniWindow::onStartPauseClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MiniWindow::onResetClicked);
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
