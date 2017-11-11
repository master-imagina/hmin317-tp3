#ifndef VECDIMINDICATOR_H
#define VECDIMINDICATOR_H

#include <QWidget>


class VectorDimIndicator : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit VectorDimIndicator(QWidget *parent = nullptr);
    VectorDimIndicator(const QString &text, const QColor &color,
                       QWidget *parent = nullptr);
    ~VectorDimIndicator();

    QString text() const;
    QColor color() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

Q_SIGNALS:
    void textChanged(const QString &);
    void colorChanged(const QColor &);

public Q_SLOTS:
    void setText(const QString &text);
    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_text;
    QColor m_color;
};

#endif // VECDIMINDICATOR_H
