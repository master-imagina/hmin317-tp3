#ifndef EVENTHOOK_H
#define EVENTHOOK_H

#include <unordered_map>

#include <QObject>

class QKeyEvent;


class EventHook : public QObject
{
    Q_OBJECT

public:
    explicit EventHook(QObject *parent = nullptr);
    ~EventHook() = default;

    bool keyIsPressed(int key) const;

    void clear();

    bool eventFilter(QObject *obj, QEvent *e) override;

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    std::unordered_map<int, bool> m_keyTable;
};

#endif // EVENTHOOK_H
