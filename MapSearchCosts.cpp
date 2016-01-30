/**********************************************************************
***********************************************************************
** Zeke Long                                                         **
** 12/10/2015                                                        **
**                                                                   **
** Program that reads in a 2D text map, then searches for the best   **
** path to a destination coordinate using breadth-first, lowest-cost **
** and A* searches.  NOTE: The map's width, height, starting         **
** coordinate, and destination coordinate are the first six numbers  **
** in map.txt                                                        **
***********************************************************************
**********************************************************************/

#include <iostream>
#include <fstream>
#include "MapSearchCosts.h"
using namespace std; 

int width, height, startX, startY, goalX, goalY;

int main(){
	ifstream infile;
	char x;
	SearchTree T1;

	/******************************************************************
	*              Read and store map in a 2D array                   *
	******************************************************************/
	infile.open("map.txt");
	infile >> width >> height;        //get map size
	infile >> startX >> startY;       //get starting position
	infile >> goalX >> goalY;         //get goal position

	T1.map = new char* [height];      //allocate space in 2D array
	for(int i=0; i<height; i++)                    
		*(T1.map + i) = new char[width];

	for(int i=0; i<height; i++){       //store map in the 2D array
		for(int j=0; j<width; j++)
		{
		 infile >> x;
		 T1.map[i][j] = x;
		}
	}
	infile.close();

	/******************************************************************
	*       Create tree and search for path from start to goal        *
	******************************************************************/
	T1.AddNodes();                             //create the search tree
	cout << "\n*******ORIGINAL MAP*******\n";
	T1.PrintTree(T1.rootPtr, 0);
	T1.BreadthFirst();               
	T1.LowestCost();
	T1.A_Star();    
}

