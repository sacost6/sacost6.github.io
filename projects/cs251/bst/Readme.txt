


Name:

Simon Acosta 

Please confirm that you compiled your solution with test cases exercising ALL
functions using g++ -std=c++11.  Confirm this statement by typing YES below.
(If you did not do this, compilation errors may result in an overall grade of
zero!)



YES!!!!


Describe what augmentations to the bst data structures you made to complete the 
project -- i.e., what types / data members did you change and why?


I added an int size variable to the bst_nodes to keep track of the number of nodes that each subtree carried. 
This was added in order to meet the runtime requirements for geq and leq functions and made it easier to keep 
track of the number of nodes greater than or less than the target values. 




-----------------------------------------------
Which functions did you need to modify as a result of the augmentations from the previous
question?  



I modified insert and remove to check for a violation of the size-balancing property, 
if a violation occurred the tree would be rebalanced at the node closest to the root 
that results in the violation after the insert or deletion. 





-----------------------------------------------
For each function from the previous question, how did you ensure that the (assymptotic) runtime 
remained the same?



I made sure to only use the expensive rebalancing function if a violation occurred for the property. 
I kept track of number of the nodes in each subtree to check for violations in constant runtime.




-----------------------------------------------
For each of the assigned functions, tell us how far you got using the choices 0-5 and
answer the given questions.  


0:  didn't get started
1:  started, but far from working
2:  have implementation but only works for simple cases
3:  finished and I think it works most of the time but not sure the runtime req is met. 
4:  finished and I think it works most of the time and I'm sure my design leads to 
       the correct runtime (even if I still have a few bugs).
5:  finished and confident on correctness and runtime requirements


to_vector level of completion:  5 


-----------------------------------------------
get_ith level of completion:  4  

    How did you ensure O(h) runtime?

    ANSWER:
I used the size variable in the node struct to 
keep from having to traverse each individual node and have O(N) runtime 
and keeping balanced trees. 
-----------------------------------------------
num_geq level of completion:  4  

    How did you ensure O(h) runtime?

    ANSWER:
By using the size bookkeeping variable to keep from having to 
individually count each node with a value greater than the target
and keeping balanced trees.
-----------------------------------------------
num_leq level of completion: 4

    How did you ensure O(h) runtime?

    ANSWER:
By using the size bookkeeping variable to keep from having to 
individually count each node with a value less than the
target and keeping balanced trees.
-----------------------------------------------
num_range level of completion:  4

    How did you ensure O(h) runtime?

    ANSWER:
I’m not too confident on the runtime, the code traverses through most of the
nodes and counts each time a value is equal to the target. 
-----------------------------------------------
Implementation of size-balanced criteria according to 
the given guidelines (including bst_sb_work):

    Level of completion: 5


Write a few sentences describing how you tested your solutions.  Are there
any tests that in retrospect you wish you'd done?


Solutions were tested using different size inputs, such as in order numbers sets of 1-20 and random sets of 15+ integers. 
I made sure that the height was near log(base_2)(N) where N is number of total nodes in a tree. I also made sure none of the properties were
violated during insertion and deletion. I wish I would have extensively checked edge cases for the functions and checked functions if they are used
back to back.
 






