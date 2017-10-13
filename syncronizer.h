#include <QObject>
#include <QElapsedTimer>

class Syncronizer : public QObject {

    Q_OBJECT

    static const float delay = 3.0f;
    static Syncronizer * instance;

    QElapsedTimer timer;
    int value;

public:

    Syncronizer();

    static Syncronizer& getInstance();

public slots:

    void update();

signals:

    void firstChanged(int newValue);
    void secondChanged(int newValue);
    void thirdChanged(int newValue);
    void fourthChanged(int newValue);

};