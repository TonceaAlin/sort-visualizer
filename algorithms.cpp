#include "algorithms.h"


Thread::Thread(int ms, int alg, int n, std::vector<double> columns, QObject* parent = nullptr) : QThread(parent)
{
	this->sortWith = alg;
	this->amount = n;
	this->columnsHeight = columns;
	this->sortDoneDelay = (amount >= 300 ? 2 : 5);
	this->sortDelay = ms;
}

void Thread::run()
{
	
	switch (this->sortWith)
	{
	case 0:
		BubbleSort();
		break;
	case 1:
		HeapSort();
		break;
	case 2:
		QuickSort(0, amount - 1);
		break;
	default:
		break;
	}

	sorted();
}

void Thread::swap(int n, int m)
{
	
	std::swap(columnsHeight[n], columnsHeight[m]);
	emit comparision(n, m);
}

void Thread::sorted()
{
	qDebug() << "balarie";
	for (auto i = 0; i < amount; i++)
	{
		emit sortDone(i);
		msleep(sortDoneDelay);

	}
	emit changeButtonStatus(2);
}

void Thread::isAccessToArray()
{
}

void Thread::BubbleSort()
{

	for(int i=0; i<amount; i++)
		for (auto j = 0; j < amount - 1; j++)
		{
			if (columnsHeight[j] > columnsHeight[j+1])
				swap(j, j + 1);

			msleep(sortDelay);
		}

}

void Thread::Heapify(int n, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && columnsHeight[left] > columnsHeight[largest])
		largest = left;

	if (right < n && columnsHeight[right] > columnsHeight[largest])
		largest = right;

	if (largest != i)
	{
		swap(i, largest);
		Heapify(n, largest);

		msleep(sortDelay);
	}
}

void Thread::HeapSort()
{
	auto n = this->amount;
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		Heapify(n, i);
	}

	for (int i = n - 1; i >= 0; i-- )
	{
		swap(0, i);
		msleep(sortDelay);
		Heapify(i, 0);
	}
}

int Thread::Partition(int begin, int end)
{	
	auto pivot = columnsHeight[end];
	auto i = (begin - 1);

	for (auto j = begin; j <= end-1; j++)
	{
		if (columnsHeight[j] <= pivot)
		{
			i++;
			swap(i, j);
		}

		msleep(sortDelay);
	}

	swap(i + 1, end);
	msleep(sortDelay);
	return(i + 1);
}

void Thread::QuickSort(int begin, int end)
{
	if (begin < end)
	{
		auto pi = Partition(begin, end);

		QuickSort(begin, pi - 1);
		QuickSort(pi + 1, end);
	}
}



