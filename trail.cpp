#include "trail.h"

#include <random>
#include <algorithm>

trail::trail(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::trailClass)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene(this);
    this->ui->cellsToSort->setScene(scene);
    this->ui->cellsToSort->verticalScrollBar()->blockSignals(true);
    this->ui->cellsToSort->horizontalScrollBar()->blockSignals(true);

    this->appState = -1;// -> app Started
    sceneHeight = this->ui->cellsToSort->size().height();
    sceneWidth = this->ui->cellsToSort->size().width();
    this->comparisions = 0;

    for (int i = 0; i < this->ui->algorithmList->count(); i++)
    {
        QListWidgetItem* item = this->ui->algorithmList->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 50));
        algorithmList.insert(i, item->text());
    }

    columnsSet(1, 100);
    QObject::connect(ui->algorithmList, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(on_algorithmSelection_itemClicked(QListWidgetItem*)));
    QObject::connect(ui->sortButton, &QPushButton::clicked, this, &trail::sortButton_clicked);
    //QObject::connect(ui->algorithmList, &QListWidget::itemSelectionChanged, this, &trail::on_algorithmSelection_itemClicked);
}



void trail::columnsSet(int ms, int n)
{
    ui->comparisonsLabel->setNum(0);
    this->amountOfColumns = n;
    this->columnsWidth = this->sceneWidth / amountOfColumns;
    this->sortDelay = ms;

    //setting up columns;
    columns.resize(static_cast<unsigned>(amountOfColumns));

    //setting up columns height
    double increment = this->sceneHeight / this->amountOfColumns;
    for (auto i = increment; i <= sceneHeight; i += increment)
        this->columnsHeight.push_back(i);

    //randomize the arrays
    std::random_device random_d;
    std::mt19937 random_e(random_d());
    std::shuffle(this->columnsHeight.begin(), this->columnsHeight.end(), random_e);

    //applying the columns to the scene
    auto j = 0;
    auto k = 0.0;
    for (auto &c : this->columns)
    {
        c = new QGraphicsRectItem;
        c->setRect(k, (sceneHeight - columnsHeight[j]), columnsWidth, columnsHeight[j]);
        c->setBrush(QBrush(QColor(235,52, 52, 255)));


        if (amountOfColumns <= 200)
            c->setPen(QPen(Qt::black, 2));
        else if(amountOfColumns>200 && amountOfColumns<= 300)
            c->setPen(QPen(Qt::black, 1));
        else
            c->setPen(Qt::NoPen);

        this->scene->addItem(c);

        j++;
        k += columnsWidth;

    }
    
}

trail::~trail()
{
    this->runningThread->terminate();
    delete ui;
}

void trail::on_algorithmSelection_itemClicked(QListWidgetItem* item1)
{
    if (appState == -1)
    {

        this->ui->sortButton->setEnabled(true);
        appState = 0;
        sortButtonStatus(appState);

    }
    if (appState == 0)
    {
        
        this->algorithmSelector = this->algorithmList.key(item1->text());
    }
}

void trail::sort_done(int n)
{
    columns[n]->setBrush(QBrush(QColor(0, 200, 0)));
}

void trail::resetColumns(int ms, int n)
{
    for (auto& c : this->columns)
    {
        this->scene->removeItem(c);
    }

    this->columnsHeight.clear();
    columnsSet(ms, n);
    threadUpdate(sortDelay, algorithmSelector);

}

void trail::sortButtonStatus(int state)
{
    QString buttonText;
    switch(state) {
    case 0:
        buttonText = "sort";
        ui->algorithmList->setEnabled(true);
        break;
    case 1:
        buttonText = "cancel";
        ui->algorithmList->setDisabled(true);
        break;
    case 2:
        buttonText = "new sort";
        ui->sortButton->setEnabled(true);
        break;
    
    }

    this->appState = state;
    ui->sortButton->setText(buttonText);
}



void trail::threadUpdate(int ms, int key)
{
    
    this->runningThread = new Thread(ms, key, static_cast<int>(amountOfColumns), columnsHeight, this); 

    connect(runningThread, SIGNAL(comparision(int, int)), this, SLOT(on_comparison(int, int)));
    connect(runningThread, SIGNAL(sortDone(int)), this, SLOT(sort_done(int)));
    connect(runningThread, SIGNAL(changeButtonStatus(int)), this, SLOT(sortButtonStatus(int)));
    

}

void trail::on_comparison(int n, int k)
{
    auto nRect = columns[n]->rect();
    auto kRect = columns[k]->rect();
    auto nColumnPos = nRect.left();
    auto kColumnPos = kRect.left();

    nRect.moveLeft(kColumnPos);
    kRect.moveLeft(nColumnPos);

    columns[n]->setRect(nRect);
    columns[k]->setRect(kRect);

    std::swap(columns[n], columns[k]);

    this->comparisions++;
    ui->comparisonsLabel->setNum(comparisions);
}

void trail::sortButton_clicked()
{
    
    switch (this->appState)
    {
    case 0:
        this->comparisions = 0;
        sortButtonStatus(1);
        threadUpdate(sortDelay, algorithmSelector);
        runningThread->start();
        break;
    case 1:
        runningThread->terminate();
        sortButtonStatus(2);
        break;
    case 2:
        for (auto& p : columns)
            scene->removeItem(p);

        columnsHeight.clear();
        columnsSet(sortDelay, static_cast<int>(amountOfColumns));
        sortButtonStatus(0);
        break;

    default:
        break;
    }
}
