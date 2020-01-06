#include <QFutureWatcher>
#include <QList>
#include <QObject>
#include <QTime>
#include <QtWidgets/QWidget>

#ifndef AUXILARY_H
#define AUXILARY_H

struct Task
{
    int beginIndex; // первая строка выделенного куска массива
    int endIndex; // последняя строка
    QList<QList<int>> *arr; // указатель на матрицу
    int nElem; // количество элементов в куске
};

class Auxilary : public QObject
{
    Q_OBJECT
public:
    Auxilary();

signals:

public slots:
    void startRunning();
private slots:
    void progressValueChanged(int v);
    void finished();

private:
    QFutureWatcher<int> *watcher;
    QTime timeCheck;
    QList<QList<int>> arr;
    QList<Task> tasks;
    QFuture<int> future;
    int nElem;
};

#endif // AUXILARY_H
