#pragma once
#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtWidgets>

class Thread : public QThread
{
	Q_OBJECT
public:
	Thread(int, int, int, std::vector<double>, QObject*);
signals:
	void sortDone(int);
	void comparision(int, int);
	void changeButtonStatus(int);
protected:
	void run() override;

private:
	std::vector<double> columnsHeight;
	int sortWith;
	int amount;
	int sortDoneDelay;
	int sortDelay;

	void swap(int, int);
	void sorted();
	void isAccessToArray();

	void BubbleSort();

	void Heapify(int n, int i);
	void HeapSort();

	int Partition(int begin, int end);
	void QuickSort(int begin, int end);
};

#endif // THREAD_H