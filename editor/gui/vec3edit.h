#ifndef VEC3EDIT_H
#define VEC3EDIT_H

#include <QVector3D>
#include <QWidget>

class QDoubleSpinBox;


class Vec3DEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QVector3D value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)
    Q_PROPERTY(float xMin READ xMin WRITE setXMin NOTIFY xMinChanged)
    Q_PROPERTY(float xMax READ xMax WRITE setXMax NOTIFY xMaxChanged)
    Q_PROPERTY(float yMin READ yMin WRITE setYMin NOTIFY yMinChanged)
    Q_PROPERTY(float yMax READ yMax WRITE setYMax NOTIFY yMaxChanged)
    Q_PROPERTY(float zMin READ zMin WRITE setZMin NOTIFY zMinChanged)
    Q_PROPERTY(float zMax READ zMax WRITE setZMax NOTIFY zMaxChanged)
    Q_PROPERTY(float stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged)

public:
    explicit Vec3DEdit(QWidget *parent = nullptr);
    ~Vec3DEdit();

    QVector3D value() const;

    int decimals() const;

    float xMin() const;
    float xMax() const;

    float yMin() const;
    float yMax() const;

    float zMin() const;
    float zMax() const;

    float stepSize() const;

Q_SIGNALS:
    void valueChanged(const QVector3D &);

    void decimalsChanged(int);

    void xMinChanged(float);
    void xMaxChanged(float);

    void yMinChanged(float);
    void yMaxChanged(float);

    void zMinChanged(float);
    void zMaxChanged(float);

    void stepSizeChanged(float);

public Q_SLOTS:
    void setValue(const QVector3D &val);

    void setDecimals(int prec);

    void setXMin(float min);
    void setXMax(float max);

    void setYMin(float min);
    void setYMax(float max);

    void setZMin(float min);
    void setZMax(float max);

    void setMin(float min);
    void setMax(float max);

    void setStepSize(float step);

private:
    QDoubleSpinBox *m_xSpinBox;
    QDoubleSpinBox *m_ySpinBox;
    QDoubleSpinBox *m_zSpinBox;
};

#endif // VEC3EDIT_H
