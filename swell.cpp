#include <swell.h>
#include <ui_swell.h>

#include <iostream>

using namespace std;

int animationDuration = 1200; //Milliseconds
int threshold = 5; //Pixels
QString innerButtonSS = "#innerButton { border: 1px solid black; border-radius: 5px; background-color: black; }";
QRect smallCircleRect = QRect(10, 10, 10, 10); //Size of the innerButton in the off state
QRect bigCircleRect = QRect(0, 0, 30, 30); //Size of the innerButton in the on state
QColor smallCircleColor = QColor(0, 0, 0); //Color of the innerButton in the off state
QColor bigCircleColor = QColor(150, 150, 255); //Color of the innerButton in the on state

Swell::Swell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Swell),
    m_state(false),
    m_innerColor(QColor(0, 0, 0))
{
    ui->setupUi(this);

    //Reparent the inner button, because we can't do so in the designer.
    ui->innerButton->setParent(ui->outerButton);

    //Install event filters to catch mouse events.
    ui->outerButton->installEventFilter(this);
    ui->innerButton->installEventFilter(this);

    //Property Animations
    geometryAnimation = new QPropertyAnimation(ui->innerButton, "geometry");
    geometryAnimation->setDuration(animationDuration);
    geometryAnimation->setStartValue(smallCircleRect);
    geometryAnimation->setEndValue(bigCircleRect);
    connect(geometryAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));

    colorAnimation = new QPropertyAnimation(this, "innerColor");
    colorAnimation->setDuration(animationDuration);
    colorAnimation->setStartValue(smallCircleColor);
    colorAnimation->setEndValue(bigCircleColor);
}

bool Swell::state()
{
    return m_state;
}

void Swell::setState(bool newValue)
{
    if(m_state != newValue)
    {
        m_state = newValue;
        emit stateChanged(m_state);

        geometryAnimation->setStartValue(m_state ? bigCircleRect : smallCircleRect);
        geometryAnimation->setEndValue(m_state ? smallCircleRect : bigCircleRect);
        colorAnimation->setStartValue(m_state ? bigCircleColor : smallCircleColor);
        colorAnimation->setEndValue(m_state ? smallCircleColor : bigCircleColor);
    }
    resetInnerButton(m_state);
}

void Swell::resetInnerButton(bool state)
{
    ui->innerButton->setGeometry(state ? bigCircleRect : smallCircleRect);
    setInnerColor(state ? bigCircleColor : smallCircleColor);
}

QColor Swell::innerColor()
{
    return m_innerColor;
}

void Swell::setInnerColor(QColor newValue)
{
    if(m_innerColor != newValue)
    {
        m_innerColor = newValue;

        //Set the color and border-radius for the innerButton
        QString colorString = "rgb(" + QString::number(m_innerColor.red()) +
                "," + QString::number(m_innerColor.green()) +
                "," + QString::number(m_innerColor.blue()) + ")";
        ui->innerButton->setStyleSheet(innerButtonSS + "#innerButton { border-radius: " +
                                       QString::number(ui->innerButton->width()/2) +
                                       "; background-color: " + colorString + "; }");
    }
}

void Swell::animationFinished() {
    bool state = false;
    if(m_state)
    {
        state = !(ui->innerButton->width() - threshold <= smallCircleRect.width());
    }
    else
    {
        state = ui->innerButton->width() + threshold >= bigCircleRect.width();
    }
    setState(state);
}

bool Swell::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        geometryAnimation->start();
        colorAnimation->start();
        return true;
    } else if(event->type() == QEvent::MouseButtonRelease) {
        geometryAnimation->stop();
        colorAnimation->stop();
        animationFinished();
        return true;
    } else {
        return QWidget::eventFilter(target, event);
    }
}

Swell::~Swell()
{
    delete ui;
}
