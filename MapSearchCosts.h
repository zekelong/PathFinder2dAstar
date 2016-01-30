/********************************************************************* 
**********************************************************************
** Zeke Long                                                        **
** 12/10/2015                                                       **
**                                                                  **
** Header file for MapSearchCosts.cpp and MethodsMapSearchCosts.cpp **
** that provides classes                                            **   
**********************************************************************
*********************************************************************/

#include<iostream>
using namespace std;

#define R 1
#define f 2
#define F 4
#define h 5
#define r 7
#define M 9
#define W 0


class SearchTree
{
    private:
	struct TreeNode{
		int row;           //row location on map
		int column;        //column location on map
		int checked;       //flag for searches
		int cost;          //cost to move into node
		int totalCost;     //cost from start node
		int manhatDist;    //heuristic variables for A* search
		int xDist;
		int yDist;  
		TreeNode* Parent;
		TreeNode* Par1;
		TreeNode* Par2;
		TreeNode* Ptr1;
		TreeNode* Ptr2;
	};
	typedef struct TreeNode* TreePtr;
 
	
	public:
	SearchTree(){
		rootPtr = NULL;
	}

	~SearchTree(){
		TreeDelete(rootPtr);
	}

	bool IsEmpty(){
		return (rootPtr == NULL);
	}
	
	TreePtr rootPtr, tmpPtr;
	TreePtr parent1, parent2;     //temporary holder variables
	char** map;                   //initialize array (used for display)

	int BreadthFirst();
	int LowestCost();
	int A_Star();
	int GetHeuristicVal(TreePtr& t);
	void AddNodes();    
	TreePtr& FindStart(TreePtr& t);
	int TreeDelete(TreePtr& t);    
	void PrintTree(TreePtr& t, int tmpCount);
	void MarkCurrentPath(TreePtr& t);

	/**********************************************************************
	*                         Nested Queue Class                          *
	**********************************************************************/
	class Queue{
		public:
		struct QueueNode{
			SearchTree::TreePtr dataPtr;
			QueueNode* next;
		};
		typedef QueueNode* QueuePtr;

		Queue(){		//constructor
			count = 0;
			head = tail = tmp = NULL;
		}

		~Queue(){		//destructor
			QueuePtr p;
			if(head != NULL){
				while(head->next != NULL){
					p = head;
					head = head->next;
					p->next = NULL;
					delete p;
				}
			}
		}

		int IsEmpty(){
			return (count == 0);
		}

		QueuePtr head, tail, tmp;	//pointer variables
		int count;		        //queue count

		int Enqueue(TreePtr& t);
		int PriorityEnqueue(TreePtr& t);
		TreePtr& Dequeue();
	};	

};


