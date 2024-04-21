#ifndef SECOND_H
#define SECOND_H
#include <QThread>
#include <QWidget>
#include "ap3216c.h"
namespace Ui {
class second;
}
class WorkerThread;
class WorkerThread3;

class second : public QWidget
{
    Q_OBJECT

public:
    explicit second(QWidget *parent = nullptr);
    ~second();

private slots:
    void on_pushButton_clicked();

    void handleResults();
   void qiehuan();

private:
    Ui::second *ui;
    Ap3216c *ap3216c;
    WorkerThread *workerThread;
    WorkerThread3 *workerThread3;

};

class WorkerThread : public QThread
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

public:
    WorkerThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写run方法，继承QThread的类，只有run方法是在新的线程里 */
    void run() override {
//        readdata();
        
        emit b();

    }

signals:
    void b();

};
class WorkerThread3 : public QObject
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

public:
    WorkerThread3(QObject *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写run方法，继承QThread的类，只有run方法是在新的线程里 */
    void run()  {
//        readdata();

            emit c();

          }

signals:
    void c();

};

#endif // SECOND_H
