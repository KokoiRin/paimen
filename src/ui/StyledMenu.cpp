#include "StyledMenu.h"
#include <QAction>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPainterPath>
#include <QApplication>

// MenuStyle 实现
MenuStyle::MenuStyle(QStyle *style) : QProxyStyle(style) {}

void MenuStyle::drawControl(ControlElement element, const QStyleOption *option,
                           QPainter *painter, const QWidget *widget) const {
    if (element == CE_MenuItem) {
        const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
        if (!menuItem) {
            QProxyStyle::drawControl(element, option, painter, widget);
            return;
        }
        
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        
        // 绘制菜单项背景
        QRect itemRect = option->rect.adjusted(4, 2, -4, -2);
        
        if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
            // 绘制分隔线
            painter->setPen(QPen(QColor(255, 255, 255, 40), 1));
            int y = itemRect.center().y();
            painter->drawLine(itemRect.left() + 10, y, itemRect.right() - 10, y);
        } else {
            // 绘制普通菜单项
            QPainterPath path;
            path.addRoundedRect(itemRect, 6, 6);
            
            if (option->state & State_Selected) {
                // 悬停状态 - 使用深紫色调
                painter->fillPath(path, QColor(138, 43, 226, 100));  // 深紫色高亮
                
                // 边框
                painter->setPen(QPen(QColor(186, 85, 211, 150), 1.5));  // 中紫色边框
                painter->drawPath(path);
            } else if (option->state & State_Sunken) {
                // 按下状态
                painter->fillPath(path, QColor(138, 43, 226, 150));
            } else {
                // 正常状态 - 透明
                painter->fillPath(path, Qt::transparent);
            }
            
            // 绘制文字
            painter->setPen(Qt::white);
            QFont font = painter->font();
            font.setBold(true);
            font.setPointSize(10);
            painter->setFont(font);
            
            QRect textRect = itemRect.adjusted(20, 0, -20, 0);
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, menuItem->text);
        }
        
        painter->restore();
        return;
    }
    
    QProxyStyle::drawControl(element, option, painter, widget);
}

void MenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                             QPainter *painter, const QWidget *widget) const {
    if (element == PE_PanelMenu) {
        // 不绘制默认的菜单面板，让StyledMenu的paintEvent处理
        return;
    }
    
    if (element == PE_FrameMenu) {
        // 不绘制默认的菜单边框
        return;
    }
    
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QSize MenuStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                                 const QSize &size, const QWidget *widget) const {
    QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);
    
    if (type == CT_MenuItem) {
        const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
        if (menuItem && menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
            return QSize(newSize.width(), 12);
        }
        // 增加菜单项高度和宽度
        newSize.setHeight(qMax(36, newSize.height()));
        newSize.setWidth(newSize.width() + 40);
    }
    
    return newSize;
}

int MenuStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                          const QWidget *widget) const {
    if (metric == PM_SmallIconSize) {
        return 16;
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

// StyledMenu 实现
StyledMenu::StyledMenu(QWidget *parent) : QMenu(parent), m_menuStyle(nullptr) {
    setupStyle();
}

StyledMenu::StyledMenu(const QString &title, QWidget *parent) 
    : QMenu(title, parent), m_menuStyle(nullptr) {
    setupStyle();
}

StyledMenu::~StyledMenu() {
    if (m_menuStyle) {
        delete m_menuStyle;
    }
}

QAction* StyledMenu::addStyledAction(const QString &icon, const QString &text) {
    QString actionText = icon + " " + text;
    QAction *action = addAction(actionText);
    return action;
}

void StyledMenu::setupStyle() {
    // 设置窗口标志
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground, true);  // 确保背景被绘制
    
    // 应用自定义样式
    m_menuStyle = new MenuStyle();
    setStyle(m_menuStyle);
    
    // 设置边距
    setContentsMargins(8, 8, 8, 8);
}

void StyledMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制菜单背景
    QRect menuRect = rect();
    QPainterPath path;
    path.addRoundedRect(menuRect, 10, 10);
    
    // 紫蓝渐变背景 - 与记录窗口一致
    QLinearGradient gradient(0, 0, menuRect.width(), menuRect.height());
    gradient.setColorAt(0, QColor(0x66, 0x7e, 0xea));   // #667eea
    gradient.setColorAt(1, QColor(0x76, 0x4b, 0xa2));   // #764ba2
    painter.fillPath(path, gradient);
    
    // 绘制边框
    painter.setPen(QPen(QColor(255, 255, 255, 100), 2));  // 半透明白色边框
    painter.drawPath(path);
    
    // 添加微妙的内阴影，增加深度
    QPainterPath innerPath;
    innerPath.addRoundedRect(menuRect.adjusted(2, 2, -2, -2), 8, 8);
    painter.setPen(QPen(QColor(0, 0, 0, 30), 1));
    painter.drawPath(innerPath);
    
    // 调用基类绘制菜单项
    QMenu::paintEvent(event);
}
