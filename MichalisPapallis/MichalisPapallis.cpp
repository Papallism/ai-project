/*
* Michalis Papallis
* U184N0546
* 
* COMP-405 Artificial Intelligence
* Programming Project
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constant for the total number of rocks that exist in the problem
const int NUMBER_OF_ROCKS = 7;

// Enumeration with the 3 possible states that a rock can have
enum Rock
{
    Brown, Empty, Green
};

// Enumeration with the 4 valid operations that can be performed
enum Operation
{
    MoveRight, JumpRight, MoveLeft, JumpLeft
};

// Structure to represent the states of the problem at every iteration
struct Node
{
	Node* parent;
	Node* next;
	Rock state[NUMBER_OF_ROCKS]; // An array of type enum Rock to represent the 7 rocks and what is on each rock
    Rock frogThatMovedColor;
    int frogThatMovedIndex;
    Operation performedOperation;

	Node();
	Node(Node* parentNode, Rock generatedState[], Rock frogColor, int frogIndex, Operation operation);
};

// Implementation of a Linked List with pointers using simplified operations according to the problem
class LinkedList
{
public:
	void insert(Node* nodeToInsert);
	void remove();
	bool isEmpty();
    bool alreadyExists(Rock stateArrayToCheck[]);

	LinkedList();
	LinkedList(Node* rootNode);

	Node* first;
	Node* last;
};

// Constants for the initial and goal state
const Rock initialState[NUMBER_OF_ROCKS] = { Brown, Brown, Brown, Empty, Green, Green, Green };
const Rock goalState[NUMBER_OF_ROCKS] = { Green, Green, Green, Empty, Brown, Brown, Brown };

// Function signatures
bool isGoal(Node* nodeToCheck);
int locateEmptyRock(Rock stateArray[]);
void copyPreviousState(Rock previousStateArray[], Rock newStateArray[]);
void generateChildren(Node* parentNode, LinkedList* openList, LinkedList* closedList);
string rockEnumToString(Rock rockState);
string operationEnumToString(Operation performedOperation);

int main()
{
    Node* root = new Node();
    Node* currentNode = new Node();

    LinkedList* open = new LinkedList(root);
    LinkedList* closed = new LinkedList();

    while (!open->isEmpty())
    {
        currentNode = open->first;
        open->remove();

        if (isGoal(currentNode))
        {
            cout << "Solution found" << endl;
            break;
        }
        else
        {
            closed->insert(currentNode);
            generateChildren(currentNode, open, closed);
        }
    }

    // Traverse the closed list to count the expanded nodes
    int expandedNodes = 0;
    Node* temp = closed->first;
    while (temp != nullptr) {
        expandedNodes++;
        temp = temp->next;
    }
    cout << endl << "Expanded nodes: " << expandedNodes << endl;

    // Backtrack from the goal until the root
    cout << endl << "Jumps as follows:" << endl;
    temp = currentNode;
    vector<string> solutionSteps;
    while (temp->parent != nullptr)
    {
        // Convert enums and index to string, concatenate, and store in vector
        solutionSteps.push_back(rockEnumToString(temp->frogThatMovedColor)
                                + to_string(temp->frogThatMovedIndex + 1)
                                + operationEnumToString(temp->performedOperation));

        temp = temp->parent;
    }
    // Print the vector in reverse order to show the steps in the correct order
    for (int i = solutionSteps.size() - 1; i >= 0; i--)
    {
        cout << solutionSteps[i] << endl;
    }

    // TODO: Open and Closed lists in report

    //cout << "\nClosed List:" << endl;
    //temp = closed->first;
    //while (temp != nullptr) {
    //    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    //    {
    //        cout << temp->state[i];
    //    }
    //    cout << endl;
    //    temp = temp->next;
    //}

    return 0;
}

// Function to convert Rock enums to string
string rockEnumToString(Rock rockState)
{
    string rockEnums[] = { "Brown frog on place ", "Empty frog on place ", "Green frog on place " };
    return rockEnums[rockState];
}

// Function to convert Operation enums to string
string operationEnumToString(Operation performedOperation)
{
    string operationEnums[] = { " moves right", " jumps right", " moves left", " jumps left" };
    return operationEnums[performedOperation];
}

// Function to check if a state is the goal state
bool isGoal(Node* nodeToCheck)
{
    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    {
        if (goalState[i] != nodeToCheck->state[i])
        {
            return false;
        }
    }

    return true;
}

// Function to locate the index of the empty rock in a state
int locateEmptyRock(Rock stateArray[])
{
    int emptyRockIndex = NULL;

    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    {
        if (stateArray[i] == Rock::Empty)
        {
            emptyRockIndex = i;
            break;
        }
    }

    return emptyRockIndex;
}

// Function to copy the state of the previous node to a new array
void copyPreviousState(Rock previousStateArray[], Rock newStateArray[])
{
    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    {
        newStateArray[i] = previousStateArray[i];
    }
}

// Function to generate the children of a node and add them in the OPEN list
void generateChildren(Node* parentNode, LinkedList* openList, LinkedList* closedList)
{
    int emptyRockIndex = locateEmptyRock(parentNode->state);
    Rock generatedState[NUMBER_OF_ROCKS];

	// Move Right operation
	if (emptyRockIndex > 0 && parentNode->state[emptyRockIndex - 1] == Brown)
	{
        copyPreviousState(parentNode->state, generatedState);
        generatedState[emptyRockIndex] = Brown;
        generatedState[emptyRockIndex - 1] = Empty;
        if (!openList->alreadyExists(generatedState) && !closedList->alreadyExists(generatedState))
        {
            Node* generatedNode = new Node(parentNode, generatedState, Brown, emptyRockIndex - 1, MoveRight);
            openList->insert(generatedNode);
        }
	}
    // Jump Right operation
    if (emptyRockIndex > 1 && parentNode->state[emptyRockIndex - 2] == Brown)
    {
        copyPreviousState(parentNode->state, generatedState);
        generatedState[emptyRockIndex] = Brown;
        generatedState[emptyRockIndex - 2] = Empty;
        if (!openList->alreadyExists(generatedState) && !closedList->alreadyExists(generatedState))
        {
            Node* generatedNode = new Node(parentNode, generatedState, Brown, emptyRockIndex - 2, JumpRight);
            openList->insert(generatedNode);
        }
    }
    // Move Left operation
    if (emptyRockIndex < 6 && parentNode->state[emptyRockIndex + 1] == Green)
    {
        copyPreviousState(parentNode->state, generatedState);
        generatedState[emptyRockIndex] = Green;
        generatedState[emptyRockIndex + 1] = Empty;
        if (!openList->alreadyExists(generatedState) && !closedList->alreadyExists(generatedState))
        {
            Node* generatedNode = new Node(parentNode, generatedState, Green, emptyRockIndex + 1, MoveLeft);
            openList->insert(generatedNode);
        }
    }
    // Jump Left operation
    if (emptyRockIndex < 5 && parentNode->state[emptyRockIndex + 2] == Green)
    {
        copyPreviousState(parentNode->state, generatedState);
        generatedState[emptyRockIndex] = Green;
        generatedState[emptyRockIndex + 2] = Empty;
        if (!openList->alreadyExists(generatedState) && !closedList->alreadyExists(generatedState))
        {
            Node* generatedNode = new Node(parentNode, generatedState, Green, emptyRockIndex + 2, JumpLeft);
            openList->insert(generatedNode);
        }
    }
}

// Node Constructor for the root, creating the initial state
Node::Node()
{
    parent = nullptr;
    next = nullptr;
    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    {
        state[i] = initialState[i];
    }
}

// Node Constructor for all states except the root, setting a pointer to the parent Node
Node::Node(Node* parentNode, Rock generatedState[], Rock frogColor, int frogIndex, Operation operation)
{
    parent = parentNode;
	next = nullptr;
    for (int i = 0; i < NUMBER_OF_ROCKS; i++)
    {
        state[i] = generatedState[i];
    }
    frogThatMovedColor = frogColor;
    frogThatMovedIndex = frogIndex;
    performedOperation = operation;
}

// Operation to insert a node in the linked list
void LinkedList::insert(Node* nodeToInsert)
{
    if (first == nullptr && last == nullptr)
    {
        first = last = nodeToInsert;
        nodeToInsert->next = nullptr;
    }
    else
    {
        last->next = nodeToInsert;
        last = nodeToInsert;
        nodeToInsert->next = nullptr;
    }
}

// Operation to remove a node from the linked list
void LinkedList::remove()
{
    if (first->next != nullptr)
    {
        Node* temp = first;
        first = first->next;
        temp->next = nullptr;
    }
    else
    {
        first = nullptr;
        last = nullptr;
    }
}

// Operation to check if a linked list is empty
bool LinkedList::isEmpty()
{
    return first == nullptr && last == nullptr;
}

// Operation to check if a node already exists in the linked list
bool LinkedList::alreadyExists(Rock stateArrayToCheck[])
{
    Node* temp = first;
    int index = 0;

    while (temp != nullptr)
    {
        while (temp->state[index] == stateArrayToCheck[index])
        {
            index++;
            if (index == NUMBER_OF_ROCKS)
            {
                return true;
            }
        }

        temp = temp->next;
        index = 0;
    }

    return false;
}

// Linked List Constructor to create an empty linked list
LinkedList::LinkedList()
{
    first = nullptr;
    last = nullptr;
}

// Linked List Constructor to create a linked list with a node as the root
LinkedList::LinkedList(Node* rootNode)
{
    first = rootNode;
    last = rootNode;
}