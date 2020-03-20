#ifndef SWELL_H
#define SWELL_H

#include <QWidget>
#include <QPropertyAnimation>

namespace Ui {
class Swell;
}

class Swell : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QColor innerColor READ innerColor WRITE setInnerColor)

public:
    /**
     * @brief Constructor
     * @param parent Parent widget.
     */
    explicit Swell(QWidget *parent = 0);

    /**
     * @brief Sets the dimensions for this widget. This widget must be a square; therefore,
     * only one dimension is accepted.
     * @param dimension Represents the width and the height.
     */
    void setSize(int dimension = 0);

    /**
     * Accessor for state property.
     */
    bool state();

    /**
     * Accessor for innerColor property.
     */
    QColor innerColor();

    /** Event filter for catching mouse press event for either this or the inner button. */
    bool eventFilter(QObject *target, QEvent *event);

    /**
     * Destructor
     */
    ~Swell();

signals:
    /** Emitted when state property changes. */
    void stateChanged(bool);

private slots:
    /**
     * @brief Mutator for swollenState property.
     * @param newValue New value for swollenState property.
     */
    void setState(bool newValue);

    /**
     * @brief Mutator for innerColor property.
     * @param newValue New value for innerColor property.
     */
    void setInnerColor(QColor newValue);

    /**
     * @brief Catches when the animation finishes.
     */
    void animationFinished();

private:
    /**
     * @brief Sets the appearance of the inner button based on the state of this.
     * @param state State of this.
     */
    void resetInnerButton(bool state);

    /** Represents the swollen state. */
    bool m_state;
    /** Represents the color of the innerButton */
    QColor m_innerColor;
    /** Geometry Animation for the innerButton */
    QPropertyAnimation *geometryAnimation;
    /** Color Animation for the innerButton */
    QPropertyAnimation *colorAnimation;
    /** UI */
    Ui::Swell *ui;
};

#endif // SWELL_H
