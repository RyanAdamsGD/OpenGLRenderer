#ifndef GRID_H
#define GRID_H
#include "glm\glm.hpp"
#include "MyTypeDefs.h"
#include <memory.h>

template<class T>
class Grid
{
	struct Cell
	{
		uint size;
		T** items;
		void addItem(T* item);
		Cell() :size(0),items(NULL) {}
	};
	Cell* cells;
	uint nx,ny,nz;
	float dx,dy,dz;
	glm::vec3 min, max;
	uint totalItemCount;
	ushort* sortedCellIndicies;

	void sortGrid();
	int computeIndex(const glm::vec3& point)const;
	//ToDo::finish implementing the sort then move this to public
	T** getSortedCellData()const;
public:
	uint numOfCells;

	Grid():cells(NULL),min(0,0,0),max(0,0,0),sortedCellIndicies(NULL),totalItemCount(0){}
	Grid(const glm::vec3& min, const glm::vec3& max, float xGridSpacing, float yGridSpacing, float zGridSpacing);
	Grid(const glm::vec3& min, const glm::vec3& max, uint xGridCount, uint yGridCount, uint zGridCount);
	~Grid(void);
	Grid(const Grid& rhs);
	Grid<T>& operator =(const Grid& rhs);

	void setUpCells();
	///returns false if item is outside the current grid bounds
	bool addItem(T* item, const glm::vec3& position);
	bool addItem(T* item, uint index);
	void clearGrid();
	void clearCellData();

	T** getCellData(const glm::vec3& point, uint& size)const;
	T** getCellData(uint index, uint& size)const;

	glm::vec3 getMin() {return min;}
	glm::vec3 getMax() {return max;}
};

template <class T>
Grid<T>::Grid(const Grid& rhs)
	:numOfCells(rhs.numOfCells),nx(rhs.nx),ny(rhs.ny),nz(rhs.nz),min(rhs.min),max(rhs.max),dx(rhs.dx),
	dy(rhs.dy),dz(rhs.dz),totalItemCount(rhs.totalItemCount)
{
	if(cells != NULL)
	{
		clearCellData();
		delete [] cells;
		cells = NULL;
	}
	if(sortedCellIndicies != NULL)
	{
		delete [] sortedCellIndicies;
	}
	cells = new Cell[numOfCells];
	sortedCellIndicies = new ushort[numOfCells];
	memcpy(sortedCellIndicies,rhs.sortedCellIndicies,sizeof(ushort) * numOfCells);
	memcpy(cells,rhs.cells,sizeof(Cell) * numOfCells);
}

template <class T>
Grid<T>& Grid<T>::operator =(const Grid& rhs)
{
	nx = rhs.nx;
	ny = rhs.ny;
	nz = rhs.nz;
	dx = rhs.dx;
	dy = rhs.dy;
	dz = rhs.dz;

	totalItemCount = rhs.totalItemCount;

	numOfCells = rhs.numOfCells;
	min = rhs.min;
	max = rhs.max;
	if(cells != NULL)
	{
		clearCellData();
		delete [] cells;
		cells = NULL;
	}
	if(sortedCellIndicies != NULL)
	{
		delete [] sortedCellIndicies;
	}
	cells = new Cell[numOfCells];
	sortedCellIndicies = new ushort[numOfCells];
	memcpy(sortedCellIndicies,rhs.sortedCellIndicies,sizeof(ushort) * numOfCells);
	memcpy(cells,rhs.cells,sizeof(Cell) * numOfCells);
	return *this;
}

template <class T>
bool Grid<T>::addItem(T* item, uint index)
{
	if(numOfCells < index)
		return false;
	cells[index].addItem(item);
	totalItemCount++;
	return true;
}

template <class T>
bool Grid<T>::addItem(T* item, const glm::vec3& position)
{
	if(numOfCells == 0)
		return false;
	int index = computeIndex(position);
	if(index<0 || index > numOfCells)
		return false;

	cells[index].addItem(item);
	totalItemCount++;
	return true;
}

template <class T>
void Grid<T>::Cell::addItem(T* item)
{
	if(size == 0)
	{
		items = new T*[1];
		items[0] = item;
		size++;
		return;
	}
	
	T** newItems = new T*[size+1];
	memcpy(newItems,items,size * sizeof(T*));
	delete [] items;
	items = newItems;
	items[size++] = item;
}

template <class T>
Grid<T>::Grid(const glm::vec3& min, const glm::vec3& max, float xGridSpacing, float yGridSpacing, float zGridSpacing)
	:min(min),max(max),cells(NULL),totalItemCount(0)
{
	nx = max.x - min.x / xGridSpacing + 1;
	ny = max.y - min.y / yGridSpacing + 1;
	nz = max.z - min.z / zGridSpacing + 1;
	
	dx = (1.0f / (max.x - min.x)) * nx;
	dy = (1.0f / (max.y - min.y)) * ny;
	dz = (1.0f / (max.z - min.z)) * nz;
	setUpCells();
	sortedCellIndicies = new ushort[numOfCells];
	sortGrid();
}

template <class T>
Grid<T>::Grid(const glm::vec3& min, const glm::vec3& max, uint xGridCount, uint yGridCount, uint zGridCount)
	:min(min), max(max),cells(NULL),nx(xGridCount),ny(yGridCount),nz(zGridCount),totalItemCount(0)
{
	assert(xGridCount > 0 && yGridCount > 0 && zGridCount > 0);
	
	dx = (1.0f / (max.x - min.x)) * nx;
	dy = (1.0f / (max.y - min.y)) * ny;
	dz = (1.0f / (max.z - min.z)) * nz;
	setUpCells();
	sortedCellIndicies = new ushort[numOfCells];
	sortGrid();
}

template <class T>
Grid<T>::~Grid(void)
{
	if(cells != NULL)
	{
		delete [] cells;
		cells = NULL;
	}

	if(sortedCellIndicies != NULL)
	{
		delete [] sortedCellIndicies;
		sortedCellIndicies = NULL;
	}
}

template <class T>
void Grid<T>::setUpCells()
{
	numOfCells = nx * ny * nz;
	if(cells != NULL)
	{
		delete [] cells;
		cells = NULL;
	}
	cells = new Cell[numOfCells];
}

template <class T>
void Grid<T>::clearGrid()
{
	clearCellData();
	if(cells != NULL)
	{
		delete [] cells;
		cells = NULL;
	}
	numOfCells = 0;
}

template <class T>
void Grid<T>::clearCellData()
{
	for(uint i=0;i<numOfCells;i++)
	{
		if(cells[i].items != NULL)
		{
			delete [] cells[i].items;
			cells[i].items = NULL;
		}
		cells[i].size = 0;
	}
}

template <class T>
int Grid<T>::computeIndex(const glm::vec3& point)const
{
	int ix = (point.x - min.x) * dx;
	int iy = (point.y - min.y) * dy;
	int iz = (point.z - min.z) * dz;

	//return -1 if point falls outside the grid
	if(ix < 0 || iy < 0 || iz < 0 || ix > nx - 1 || iy > ny -1 || iz > nz -1)
		return -1;

	return ix + nx * iy + nx * ny * iz;
}

template <class T>
T** Grid<T>::getCellData(const glm::vec3& point, uint& size)const
{
	uint index = computeIndex(point);
	if(index < 0 || index >= numOfCells)
		return NULL;
	Cell& cellData = cells[index];
	size = cellData.size;
	return cellData.items;
}

template <class T>
T** Grid<T>::getCellData(uint index, uint& size)const
{
	if(index < 0 || index >= numOfCells)
		return NULL;
	Cell& cellData = cells[index];
	size = cellData.size;
	return cellData.items;
}

template <class T>
void Grid<T>::sortGrid()
{

}

template <class T>
T** Grid<T>::getSortedCellData()const
{
	//make sure there is data to return
	if(totalItemCount == 0)
		return NULL;

	T** sortedData = new T*[totalItemCount];
	uint currentOffset = 0;
	uint size;
	for(uint i=0;i<numOfCells;i++)
	{
		Cell& cellData = cells[sortedCellIndicies[i]];
		//check if there is data in the cell
		if(cellData.size != 0)
		{
			//copy all the cells items into the output array
			size = sizeof(T*) * cellData.size;
			memcpy(sortedData + currentOffset,cellData.items,size);
			currentOffset += size;
		}
	}

	return sortedData;
}
#endif