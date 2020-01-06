#include "auxilary.h"
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <QThread>

using namespace std;

int perElementFunc(const Task tsk)
    {
        int i, j, swap_count = 0, cols_nmbr = 0, temp = 0;
        for (i = tsk.beginIndex; i < tsk.endIndex+1; i++)
        {
            temp = 0;
            swap_count = 0;
            cols_nmbr = tsk.nElem;
            QList<int> currentLineSorting = tsk.arr->at(i);
            for (j = 0; j < cols_nmbr; j++)
            {
                if (j + 1 != cols_nmbr && currentLineSorting.at(j) > currentLineSorting.at(j + 1))
                {
                    temp = currentLineSorting.at(j);
                    currentLineSorting.replace(j, currentLineSorting.at(j+1));
                    currentLineSorting.replace(j+1, temp);
                    swap_count++;
                }
                if (j + 1 == cols_nmbr && swap_count > 0)
                {
                    cols_nmbr--;
                    swap_count = 0;
                    j = -1;
                }
            }
            tsk.arr->replace(i, currentLineSorting);
        }
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
        //printf("%d %d %d\n", future.progressMinimum(), future.progressValue(), future.progressMaximum());
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
    QString outputLine = "";
    QList<int> temp; // сортировка
    int r = 0;
    int swapCount = 0;
    int size = nElem;
    while (r < size)
    {
        if (r + 1 != size && arr.at(r).at(0) > arr.at(r + 1).at(0)) {
            temp = arr.at(r);
            arr.replace(r, arr.at(r + 1));
            arr.replace(r + 1, temp);
            swapCount++;
        }
        if (r + 1 == size && swapCount > 0) {
            size--;
            swapCount = 0;
            r = 0;
        }
        else { r++; }
    }
    printf("Time elapsed: %d\n", timeCheck.elapsed());
    printf("-------------------------\n");
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
