#pragma once

#include <QtWidgets/QMainWindow>
#include <qwidget.h>
#include "ui_trail.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QListWidgetItem>
#include <QObject>
#include <QScrollBar>
#include <QtWidgets>

#include "algorithms.h"

namespace Ui {
    class trail;
}
class Thread;

class trail : public QMainWindow
{
    Q_OBJECT

public:
    explicit trail(QWidget *parent = nullptr);
    ~trail();

private slots:

    void sortButton_clicked();
    void on_algorithmSelection_itemClicked(QListWidgetItem*);
    void sort_done(int);
    void resetColumns(int, int);

    void sortButtonStatus(int);
    void on_comparison(int, int);

private:
    Ui::trailClass *ui;
    QGraphicsScene *scene;
    Thread *runningThread;

    void columnsSet(int, int);
    void threadUpdate(int, int);

    std::vector<QGraphicsRectItem*> columns;
    std::vector<double> columnsHeight;
    QMap<int, QString> algorithmList;


    int amountOfColumns;
    int sortDelay;
    int appState;
    int comparisions;
    int algorithmSelector;

    double columnsWidth;
    double sceneWidth;
    double sceneHeight;
};
