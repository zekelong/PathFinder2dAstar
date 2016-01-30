/***************************************************************************** 
******************************************************************************
*** Zeke Long                                                              *** 
*** 12/10/2015                                                             ***
***                                                                        ***
***                    Methods for MapSearchCosts.h                        *** 
******************************************************************************
*****************************************************************************/

#include <iostream>
#include <ctime>
#include <math.h>
#include <cstdlib>
#include "MapSearchCosts.h"
using namespace std;

extern int width;
extern int height;
extern int startX;
extern int startY;
extern int goalX;
extern int goalY;
int numCellsExplored;

/*****************************************************************************
**                          Create the search tree                          **
*****************************************************************************/
void SearchTree::AddNodes(){
	int tmpCount = 0;

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			TreePtr newPtr = new TreeNode;    //ALLOCATE NEW TREENODE
			newPtr->row = i;
			newPtr->column = j;
			newPtr->checked = 0;
			newPtr->manhatDist = 9999999;
			newPtr->xDist = 9999999;
			newPtr->yDist = 9999999;
			newPtr->totalCost = 9999999;      //cost of path from start node
			newPtr->Parent = NULL;
			newPtr->Par1 = parent1;
			newPtr->Par2 = parent2; //(will be changed to NULL if in column 0)
			newPtr->Ptr1 = NULL;
			newPtr->Ptr2 = NULL;

			switch(map[i][j]){            //store the cost
				case 'R': newPtr->cost = R;
						  break;
				case 'f': newPtr->cost = f;
						  break;
				case 'F': newPtr->cost = F;
						  break;
				case 'h': newPtr->cost = h;
						  break;
				case 'r': newPtr->cost = r;
						  break;
				case 'M': newPtr->cost = M;
						  break;
				case 'W': newPtr->cost = W;
						  break;
				default:  newPtr->cost = 100;
			}

			if(rootPtr == NULL)
				rootPtr = newPtr;
			else{        
				/**************************************************************
				*                   Find and assign Pointers                  *
				**************************************************************/
				tmpPtr = parent1;                    
				while(tmpPtr){
					if(newPtr->column == tmpPtr->column){
						newPtr->Par1 = tmpPtr;    //assign correct Par1 
						break;
					}
					tmpPtr = tmpPtr->Par2; 
				}
			}

			parent2 = newPtr;   //make parent2 the preceding node in the row

			/*************************************************************
			*        make newPtr->Par(1 or 2) point to newPtr            *
			*************************************************************/
			if(newPtr->Par1)
				newPtr->Par1->Ptr1 = newPtr;

			if(newPtr->Par2)
				newPtr->Par2->Ptr2 = newPtr;

			tmpPtr = newPtr;
		}
		// make parent1 the last item of each row 
		//(it will be corrected in next iteration of the loop)
		parent1 = tmpPtr;         
	}
}


/*****************************************************************************
**                      Perform Breadth First Search                        **
*****************************************************************************/
int SearchTree::BreadthFirst(){
	time_t timeBFS = clock();
	numCellsExplored = 0;          //ensures explored cell count starts at zero 
	TreePtr t;
	Queue Q1;

	t = FindStart(rootPtr);          //find start node in tree
	if(t != NULL){
		t->totalCost = 0;             //start node has zero cost
		Q1.Enqueue(t);
		//search for goal until queue is empty
		while( (! Q1.IsEmpty()) && (t = Q1.Dequeue()) ){                           
			t->checked = 1;

			/****************************************************
			*             Check if goal is reached              *
			****************************************************/
			//if goal is reached
			if( (t->row == goalY)&&(t->column == goalX) ){ 
				timeBFS = clock() - timeBFS;      //record time for BFS search
				MarkCurrentPath(t);                            //mark the path
				cout << "BREADTH-FIRST SEARCH\n";
				cout << "Number of explored cells: " << numCellsExplored << endl;
				cout << "Time of search: " << timeBFS << endl;
				cout << "Total path cost: " << t->totalCost << endl;
				PrintTree(rootPtr, 0);                     //print path
				return 1;
			}
			else{                            //enqueue Pointers if not goal
				if( (t->Ptr1)&&(t->Ptr1->checked == 0)&&(t->Ptr1->cost != W) ){
					t->Ptr1->totalCost = t->Ptr1->cost + t->totalCost;
					t->Ptr1->Parent = t;
					Q1.Enqueue(t->Ptr1);
				}

				if( (t->Ptr2)&&(t->Ptr2->checked == 0)&&
					(t->column < (width-1))&&(t->Ptr2->cost != W) ){
					t->Ptr2->totalCost = t->Ptr2->cost + t->totalCost;
					t->Ptr2->Parent = t;
					Q1.Enqueue(t->Ptr2);
				}          

				if( (t->Par2)&&(t->Par2->checked == 0)&&
					(t->column > 0)&&(t->Par2->cost != W) ){
					t->Par2->totalCost = t->Par2->cost + t->totalCost;
					t->Par2->Parent = t;
					Q1.Enqueue(t->Par2);
				}

				if( (t->Par1)&&(t->Par1->checked == 0)&&
					(t->column > 0)&&(t->Par1->cost != W) ){
					t->Par1->totalCost = t->Par1->cost + t->totalCost;
					t->Par1->Parent = t;
					Q1.Enqueue(t->Par1);
				}
			}
		}
	}
}


/*****************************************************************************
**                      Performs Lowest Cost Search                         **
*****************************************************************************/
int SearchTree::LowestCost(){
	time_t timeLC = clock();
	numCellsExplored = 0;    //ensures explored cell count starts at zero 
	TreePtr t;
	Queue Q2;

	t = FindStart(rootPtr);                     //find start node in tree
	if(t != NULL){
		t->totalCost = 0;                  //start node has zero cost
		Q2.Enqueue(t);
		while(! Q2.IsEmpty()){          //search for goal until queue is empty         
			t = Q2.Dequeue();

			/****************************************************************
			*                  Check if goal is reached                     *
			****************************************************************/
			if( (t->row == goalY)&&(t->column == goalX) ){  //GOAL IS REACHED
				timeLC = clock() - timeLC;        //record time of search
				MarkCurrentPath(t);                       //mark the path
				cout << "LOWEST COST SEARCH\n";
				cout << "Number of explored cells: " << 
					numCellsExplored << endl;
				cout << "Time for search: " << timeLC << endl;
				cout << "Total path cost: " << t->totalCost << endl;
				PrintTree(rootPtr, 0);     //print path 
				return 1;
			}
			else{         
				//CHECK FIRST POINTER
				if( (t->Ptr1)&&(t->row < (height-1))&&(t->Ptr1->cost != W)&&
					(t->Ptr1->checked != 1) ){
					if(t->totalCost + t->Ptr1->cost < t->Ptr1->totalCost){
						t->Ptr1->totalCost = t->totalCost + t->Ptr1->cost;
						t->Ptr1->Parent = t;
					}
					Q2.Enqueue(t->Ptr1);
				}

				//CHECK SECOND POINTER
				if( (t->Ptr2)&&(t->column < (width-1))&&
					(t->Ptr2->cost != W)&&(t->Ptr2->checked != 1) ){
					if(t->totalCost + t->Ptr2->cost < t->Ptr2->totalCost){
						t->Ptr2->totalCost = t->totalCost + t->Ptr2->cost;
						t->Ptr2->Parent = t;
					}
					Q2.Enqueue(t->Ptr2);
				}

				//CHECK THIRD POINTER
				if( (t->Par2)&&(t->column > 0)&&
					(t->Par2->cost != W)&&(t->Par2->checked != 1) ){
					if(t->totalCost + t->Par2->cost < t->Par2->totalCost){
						t->Par2->totalCost = t->totalCost + t->Par2->cost;
						t->Par2->Parent = t;
					}
					Q2.Enqueue(t->Par2);
				}

				//CHECK FOURTH POINTER
				if( (t->Par1)&&(t->row > 0)&&
					(t->Par1->cost != W)&&(t->Par1->checked != 1) ){
					if(t->totalCost + t->Par1->cost < t->Par1->totalCost){
						t->Par1->totalCost = t->totalCost + t->Par1->cost;
						t->Par1->Parent = t;
					}
					Q2.Enqueue(t->Par1);
				}
			} //end of else-statement

			t->checked = 1;          //t cannot be visited again
		}//end of while loop
	}
}


/*****************************************************************************
**                          Performs A* Search                              **
*****************************************************************************/
int SearchTree::A_Star(){
	time_t timeAStar = clock();
	numCellsExplored = 0;             //explored cell count starts at zero 
	TreePtr t;
	Queue Q3;

	t = FindStart(rootPtr);            //find start node in tree
	if(t != NULL){
		t->totalCost = 0;          //start node has zero cost
		Q3.PriorityEnqueue(t);
		while(! Q3.IsEmpty()){     //search for goal until queue is empty           
			t = Q3.Dequeue();

			/****************************************************************
			*                    CHECK IF GOAL IS REACHED                   *
			****************************************************************/
			if(t->manhatDist == 0){                         //GOAL IS REACHED
				timeAStar = clock() - timeAStar;
				MarkCurrentPath(t);        //mark the path
				cout << "A* SEARCH\n";
				cout << "Number of explored cells: " << numCellsExplored << endl;
				cout << "Time of search: " << timeAStar << endl;
				cout << "Total path cost: " << t->totalCost << endl;
				PrintTree(rootPtr, 0);     //print path 
				return 1;
			}
			else{         
				//CHECK FIRST POINTER
				if( (t->Ptr1)&&(t->row < (height-1))&&(t->Ptr1->cost != W)&&
					(t->Ptr1->checked != 1) ){
					if(t->totalCost + t->Ptr1->cost < t->Ptr1->totalCost){
						t->Ptr1->totalCost = t->totalCost + t->Ptr1->cost;
						t->Ptr1->Parent = t;
					}
					GetHeuristicVal(t->Ptr1);
					Q3.PriorityEnqueue(t->Ptr1);
				}

				//CHECK SECOND POINTER
				if( (t->Ptr2)&&(t->column < (width-1))&&(t->Ptr2->cost != W)&&
					(t->Ptr2->checked != 1) ){
					if(t->totalCost + t->Ptr2->cost < t->Ptr2->totalCost){
						t->Ptr2->totalCost = t->totalCost + t->Ptr2->cost;
						t->Ptr2->Parent = t;
					}
					GetHeuristicVal(t->Ptr2);
					Q3.PriorityEnqueue(t->Ptr2);
				}

				//CHECK THIRD POINTER
				if( (t->Par2)&&(t->column > 0)&&(t->Par2->cost != W)&&
					(t->Par2->checked != 1) ){
					if(t->totalCost + t->Par2->cost < t->Par2->totalCost){
						t->Par2->totalCost = t->totalCost + t->Par2->cost;
						t->Par2->Parent = t;
					}
					GetHeuristicVal(t->Par2);
					Q3.PriorityEnqueue(t->Par2);
				}

				//CHECK FOURTH POINTER
				if( (t->Par1)&&(t->row > 0)&&(t->Par1->cost != W)&&
					(t->Par1->checked != 1) ){
					if(t->totalCost + t->Par1->cost < t->Par1->totalCost){
						t->Par1->totalCost = t->totalCost + t->Par1->cost;
						t->Par1->Parent = t;
					}
					GetHeuristicVal(t->Par1);
					Q3.PriorityEnqueue(t->Par1);
				}
			}                        //end of else-statement

			t->checked = 1;          //t cannot be visited again
		 
		}//end of while loop
	}
}


/*****************************************************************************
**        Assigns heuristic values to TreePtr nodes (for A* search)         **
*****************************************************************************/
int SearchTree::GetHeuristicVal(TreePtr& t){
	int distX = abs(goalX - t->column);
	int distY = abs(goalY - t->row);

	t->manhatDist = distX + distY; 
	t->xDist = distX;
	t->yDist = distY;
}


/*****************************************************************************
**                  Adds a node onto the queue (FIFO)                      **
*****************************************************************************/
int SearchTree::Queue::Enqueue(TreePtr& t){
	if(t){
		count++;  
		numCellsExplored++;            //increment explored cell count
		QueuePtr n = new QueueNode;

		n->dataPtr = t;               //assign values to new QueueNode
		n->next = NULL;

		if(head == NULL){             //add to front if queue is empty
			head = n;
			tail = head;
			return 1;
		}
		else{                             //else add to end of queue 
			tail->next = n;
			tail = tail->next;
		}   
	}   
}


/*****************************************************************************
**           Adds a node into the A* queue (Priority Queue)                 **
*****************************************************************************/
int SearchTree::Queue::PriorityEnqueue(TreePtr& t)
{
	if(t != NULL){
		count++;  
		numCellsExplored++;           //increment explored cell count
		QueuePtr n, prev, curr;

		n = new QueueNode;
		n->dataPtr = t;               //assign values to new QueueNode
		n->next = NULL;

		if(head == NULL){             //add to front if queue is empty
			head = n;
			tail = head;
		}
		else{
//			if(n->dataPtr->manhatDist <= head->dataPtr->manhatDist)
//			if(n->dataPtr->xDist <= head->dataPtr->xDist)
			if(n->dataPtr->yDist <= head->dataPtr->yDist){
				n->next = head;      
				head = n;
			}   
			else{         //walk the queue to find correct position
				curr = head;
				while(curr != NULL) {
					prev = curr;

//					if(n->dataPtr->manhatDist <= curr->dataPtr->manhatDist)
//					if(n->dataPtr->xDist <= curr->dataPtr->xDist)
					if(n->dataPtr->yDist <= curr->dataPtr->yDist){
						n->next = curr;
						prev->next = n;
						return 1;
					}
					curr = curr->next;
				}
			}
		}  
	} 
}


/*****************************************************************************
**                    Delete node from beginning of queue                   **
*****************************************************************************/
SearchTree::TreePtr& SearchTree::Queue::Dequeue(){
	if(head != NULL){
		QueuePtr p;
		p = head;             
		head = head->next;        
		if(head == tail)
			tail = head; 
		count--;
		return p->dataPtr;
	}
}


/*****************************************************************************
**              Find and return starting node in search tree                **
*****************************************************************************/
SearchTree::TreePtr& SearchTree::FindStart(TreePtr& t){
	if(t){
		if( (t->row == startY)&&(t->column == startX) )
			return t;
		FindStart(t->Ptr2);
	}
} 


/*****************************************************************************
**               Mark the current path from the start node                  **
*****************************************************************************/
void SearchTree::MarkCurrentPath(TreePtr& t){
	if(t){
		t->checked = 2;
		MarkCurrentPath(t->Parent);
	}
} 


/*****************************************************************************
**                 Print the map, showing the current path                  **
*****************************************************************************/
void SearchTree::PrintTree(TreePtr& t, int tmpCount){
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			if( (t->row == i)&&(t->column == j) ){      //print map
				if( (t->checked == 0) || (t->cost == W) )  
					cout << map[i][j]; 
				else if(t->checked == 1) 
					cout << "#";
//					cout << "\033[33m" << map[i][j] << "\033[0m";
				else if(t->checked == 2)   
					cout << " ";
//					cout << "\033[37;40m" << map[i][j] << "\033[0m";

				t->checked = 0;             //restore original value
				tmpCount++;
				if( (tmpCount == width) || (t->Ptr2 == NULL) ){
					cout << endl;
					tmpCount = 0;
				}
			}
			if( (i==(height-1)) && (j==(width-1)) )
				if( (t->row==(height-1)) && (t->column==(width-1)) )
					cout << "\n\n";
		}
	}
	if(t->Ptr2) 
		PrintTree(t->Ptr2, tmpCount);     
}


/*****************************************************************************
**               Method to delete all nodes in SearchTree                   **
*****************************************************************************/
int SearchTree::TreeDelete(TreePtr& t){
	if(t != NULL){
		TreeDelete(t->Ptr1);
		TreeDelete(t->Ptr2);
		t->Par1 = NULL;    delete t->Par1;
		t->Par2 = NULL;    delete t->Par2;
		t->Ptr1 = NULL;    delete t->Ptr1;
		t->Ptr2 = NULL;    delete t->Ptr2;
	}
	else
		return 0;
}



