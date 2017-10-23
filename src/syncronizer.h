#include <QObject>
#include <QElapsedTimer>

class Syncronizer : public QObject {

    Q_OBJECT

    static float delay;
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
