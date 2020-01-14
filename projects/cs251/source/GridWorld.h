
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;


class GridWorld {

  private:
  int population; //Hold population of world
  int rows; // Holds number of rows
  int columns; // Holds number of columns
  int repeat;
  int count; // holds the number of people in vector
  
  /* node to hold the members of a district */
  struct node {
    int idNumber; // hold ID number of person
    node *next;
    node *prev;
  }
  
  /* struct to hold a district's informatino */
  struct district {
    node * front; // front end of the list (lowest "Seniority")
    node * back; // back end of the list (highest "Seniority")
    int dPop; // population of a district
  }
  
  district deadpool; // holds district of dead people
  district ** world; // holds the 2D array used to hold together districts, creating a "world"
  
  /*struct to hold a person */
  struct person {
    int r, c; // Used to locate the person's district
    bool alive; // Used to tell if the person is alive
    node *xptr; // Used to find the node of the person
  }
  vector<people> peopleList; // holds vector containing each person, indexed by ID
  
   /* * * * * * * * * * * * * * * * * * * * * *
   * Function to push a node on to a district *
   *                                          *
   *                                          *
   *                                          *
   * * * * * * * * * * * * * * * * * * * * * */

  node * pushNode(int r, int c, int ID) {
    node *baby = new Node;
    baby->idNumber = ID;
    
    /* check if district is empty, if it is then assign node as front/back ptrs */
    if (world[r-1][c-1].front == nullptr || world[r-1][c-1].back == nullptr) {
      world[r-1][c-1].front = world[r-1][c-1].back = baby;
      baby->next = nullptr;
      baby->prev = nullptr;
      
      return baby;
    }
    
    /* if district is not empty: set next pointer to front of district,
     assign baby as front of district, and increment district population */
    baby->prev = nullptr;
    baby->next = world[r-1][c-1].front;
    world[r-1][c-1].front = baby;
    
    return baby;
    
  }
  
   /* * * * * * * * * * * * * * * * * * * * * *
   * Function to check deadpool               *
   *                                          *
   *                                          *
   *                                          *
   * * * * * * * * * * * * * * * * * * * * * */
  int checkDeadpool(node *baby) {
    if(deadpool.front == nullptr || deadpool.back == nullptr) {
      return -1;
    }

    return deadpool.back->idNumber;
  }
  
  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols)   {
      int i = 0;
      rows = nrows; // set the entered rows/columns
      columns = ncols;
      
      /* dynamically allocate space for 2D district array */
      grid = new district*[nrows];
      for(i = 0; i < nrows; i++) {
        grid[i] = new district[ncols];
      }
      
      /* set deadpools to empty */
      deadpool.dPop = 0;
      deadpool.front = nullptr;
      deadpool.back = nullptr;
      
      population = count = 0;
    }
  
    ~GridWorld(){
      int i;
      /* delete rows of grid 2D array */
      for(i = 0; i < rows; i++) {
        delete grid[i];
      }
      /* delete grid */
      delete grid;
    }

    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    bool birth(int row, int col, int &id){
      
      /* if the entered row or column is out of range, return false */
      if( row > rows - 1 || col > columns - 1) { return false; }
      
      /* increment the populations district and world district*/
      population++;
      world[row-1][col-1].dPop++;
      /* check if the deadpool is empty, if not
         then reuse the last node in the list */
      if(checkDeadpool() != -1) {
        int index = checkDeadpool() // get the id from the last node in deadpool
        
        Node *recycledNode = deadpool.back; //get a pointer to the back of the deadpool
        
        peopleList[index].r = row; // update the person's information in the person vector
        peopleList[index].c = column;
        peopleList[index].alive = true;
        peopleList[index].xptr = recycledNode;
        
        recycledNode->prev = nullptr; //set prev to null because this is the first node in the district now
        recycledNode->next = world[r-1][c-1].front; //set the next pointer equal to the front of the list
        world[r-1][c-1].front = recycledNode; //set the front of the district list to this new person
        
        node * tmp = recycledNode->prev; // remove the last node from the deadpool list
        if(tmp != nullptr) {
          tmp->next = nullptr;
          deadpool.back = tmp;
          return true;
        }
        deadpool.back = nullptr;
        deadpool.front = nullptr;
        return true;
      }
      /* If the code gets to this point, we need to create a new person/node */
      person baby; // create a new person and set their new information
      baby.r = row;
      baby.c = col;
      baby.alive = true;
      baby.xptr = pushNode(row, col, count); // get their node address from pushNode funtion
      
      peopleList.push_back(baby); // set the new person to their ID'd location in the people vector
      
      return true;
    }

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    bool death(int personID){
      /* check if id is within bounds */
      if(personID >= count || personID < 0) {
        return false;
      }
      /* check if person is dead */
      if(!peopleList[personID].alive) {
        return false;
      }
      /* get info from peoples list */
      node *target = peopleList[personID].xptr;
      int r = peopleList[personID].r;
      int c = peopleList[personID].c;
      /* check if the target is the only person in district */
      if(world[r][c].front == target && world[r][c].back == target) {
        world[r][c].front = world[r][c].back = nullptr;
      }
      /* check if the target is in the front */
      else if(world[r][c].front == target && world[r][c].back != target) {
        node * temp = target->next;
        temp->prev = nullptr;
        world[r][c].front = temp;
      }
      /* check if the target is in the back */
      else if(world[r][c].front != target && world[r][c].back == target) {
        node * temp = target->prev;
        temp->next = nullptr;
        world[r][c].back = temp;
      }
      /* the target is in the middle of the list */
      else {
        node * p1 = target->prev;
        node * p2 = target>next;
        p1->next = p2;
        p2->prev = p1;
      }
      /* Update bookkeeping */
      world[r][c].dPop--;
      population--;
      peopleList[userID].alive = false;
      // add the dead person to the deadpool 
      if(deadpool.front == nullptr) {
        deadpool.front = deadpool.back = target;
        deadpool.dPop++;
        return true;
      }
      else {
        node * p1 = deadpool.front;
        p1->prev = target;
        target->prev = nullptr;
        deadpool.front = target;
        deadpool.dPop++;
        return true;
      }

      return false;
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    bool whereis(int id, int &row, int &col)const{
      
      if(count <= id || id < 0) { return false }
      if(peopleList[id].alive)
        row = peopleList[id].r;
        column peopleList[id].c;
        return true;
    }
      
      return false ;
    }

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    bool move(int id, int targetRow, int targetCol){
      return false;
    }

    std::vector<int> * members(int row, int col)const{

      return nullptr;
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    int population()const{
      return 0;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    int population(int row, int col)const{
      return world[row][col];
    }

    /*
     * function: num_rows
     * description:  returns number of rows in world
     */
    int num_rows()const {
      return 0;
    }

    /*
     * function: num_cols
     * description:  returns number of columns in world
     */
    int num_cols()const {
      return 0;
    }



};

#endif
