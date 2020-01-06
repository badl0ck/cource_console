#include "auxilary.h"
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <QThread>

using namespace std;

int perElementFunc(const Task tsk)
    {
        /*СОРТИРОВКА ДВУМЕРНОГО МАССИВА*/
        int temp = 0;
           for (int m = 0; m < (tsk.endIndex - tsk.beginIndex + 1) * tsk.nElem - 1; m++)   //сдвиги очередных элементов в правильную позицию
               /*сдвиг элемента массива в правильную позицию*/
               for (int i = tsk.beginIndex; i <= tsk.endIndex; i++)
               {
                   QList<int> currentLineSorting = tsk.arr->at(i);
                   for (int j = 0; j<tsk.nElem - 1; j++){
                       /*АНАЛИЗ НА ПОСЛЕДНИЙ ЭЛЕМЕНТ МАССИВА*/
                           if (i == tsk.endIndex && j == tsk.nElem - 1){  //Если строка последняя и справа тупик, то ничего не делаем
                                   continue;
                           }
                       /*КОНЕЦ АНАЛИЗА НА ПОСЛЕДНЮЮ СТРОКУ*/

                           if (currentLineSorting.at(j) > currentLineSorting.at(j+1)){ //Если элемент не на своей позиции
                              temp = currentLineSorting.at(j);        //Обмен местами
                              currentLineSorting.replace(j, currentLineSorting.at(j+1));
                              currentLineSorting.replace(j+1, temp);

                              QThread::msleep(1);
                       }

                       tsk.arr->replace(i, currentLineSorting);
                  }
               }
          /*КОНЕЦ СОРТИРОВКИ ДВУМЕРНОГО МАССИВА*/

        return 0;
    }

Auxilary::Auxilary() : QObject()
{

}

void Auxilary::startRunning()
{
    cout << "Matrix size: \n"; // запросить количество элементов и потоков
    cin >> nElem;

    cout << "Count of threads: \n";
    int numThread;
    cin >> numThread;

    if (nElem > 0 && numThread > 0)
    {
        int elemPerThread = nElem / numThread; // количество строк на поток

        printf("%d threads\n", numThread);
        printf("Hello!\n");

        arr.clear();
        QList<int> current_line; // используется для заполнения, дальше не фигурирует

        for (int i = 0; i < nElem; i++)
        {
            current_line.clear();

            for (int j = 0; j < nElem; j++)
            {
                current_line.append(rand() % 100000 + 100);
            }

            arr.append(current_line);
        }

        // вывести матрицу в окне

        if (nElem < 10)
        {
            for (int i = 0; i < nElem; i++)
            {
                for (int j = 0; j < nElem; j++)
                {
                    printf("%d ", arr.at(i).at(j));
                }
                printf("\n");
            }
        }

        timeCheck.start();

        printf("Preparing tasks\n");

        for (int i = 0; i <= numThread - 1; i++)
        {
            Task tsk;
            tsk.beginIndex = i * elemPerThread; // номер первой строки куска

            if (i == numThread - 1) // последней строки куска
            {
                tsk.endIndex = nElem - 1;
            }
            else
            {
                tsk.endIndex = (i + 1) * elemPerThread - 1;
            }

            tsk.arr = &arr;
            tsk.nElem = nElem;
            tasks.append(tsk);
        }

        watcher = new QFutureWatcher<int>();
        connect(watcher, SIGNAL(progressValueChanged(int)), this, SLOT(progressValueChanged(int)));
        connect(watcher, SIGNAL(finished()), this, SLOT(finished()));
        future = QtConcurrent::mapped(tasks, perElementFunc);
        watcher->setFuture(future);

        printf("%d %d %d\n", future.progressMinimum(), future.progressValue(), future.progressMaximum());
    }
    else
    {
        printf("numThread and nElem must be greater than 0\n");
    }
}

void Auxilary::progressValueChanged(int v)
{
    printf("Progress: %d\n", v);
}

void Auxilary::finished()
{
    printf("Time elapsed: %d\n", timeCheck.elapsed());
    printf("-------------------------\n");

    QString outputLine = "";

    QList<int> temp; // сортировка
    for (int i = 0; i < nElem - 1; i++)
    {
        for (int j = 0; j < nElem - i - 1; j++)
        {
            if (arr.at(j).at(0) > arr.at(j + 1).at(0))
            {
                temp = arr.at(j);
                arr.replace(j, arr.at(j + 1));
                arr.replace(j + 1, temp);
            }
        }
    }

    if (nElem < 10)
    {
        for (int i = 0; i < nElem; i++)
        {
            outputLine.clear();
            for (int j = 0; j < nElem; j++)
            {
                printf("%d ", arr.at(i).at(j));
            }
            printf("\n");
        }
    }

    tasks.clear();
    arr.clear();
}
