/**************************************************************************************

░█▀█░█▀▄░█▀█░█░█░█▀█░░░█▀▀░▀█▀░█▀█░█▀█░█░░░█▀▀░░░█▀█░█▀█░▀█▀░░░▀▀▄░▀▀▄
░█▀▀░█▀▄░█░█░▀▄▀░█▀█░░░█▀▀░░█░░█░█░█▀█░█░░░█▀▀░░░█▀█░█▀▀░░█░░░░▄▀░░▄▀░
░▀░░░▀░▀░▀▀▀░░▀░░▀░▀░░░▀░░░▀▀▀░▀░▀░▀░▀░▀▀▀░▀▀▀░░░▀░▀░▀░░░▀▀▀░░░▀▀▀░▀▀▀
░█▀▄░█▀█░█▀▄░▀█▀░█▀█░░░█▀█░█▄█░█▀█░█▀▄░█▀█░█▀▄░▀█▀
░█░█░█▀█░█▀▄░░█░░█░█░░░█▀█░█░█░█▀█░█░█░█░█░█▀▄░░█░
░▀▀░░▀░▀░▀░▀░▀▀▀░▀▀▀░░░▀░▀░▀░▀░▀░▀░▀▀░░▀▀▀░▀░▀░▀▀▀

***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/******* CONSTANTS ***********/
#define DIM_BUFFER_K 5
#define MAX_DIM_BUFFER 1000
#define BASE_10 10
#define ASCII_TABLE_SIZE 124
/*****************************/

/**
 * This struct represents a single node of BST
 * char str*: string of length k
 * node *left: pointer to left subtree (contains only nodes with str lesser than the node’s str)
 * node *right: pointer to right subtree (contains only nodes with str bigger than the node’s str)
 * node *p: pointer to father node
 * node *next: pointer to the next node who respects the constraints learned so far
 */
struct node{
    char *str;
    struct node *left;
    struct node *right;
    struct node *next;
};

/**
 * This struct is used to create a node for the list
 * int val: integer value of the node
 * struct nodeList* next: pointer to the next node of the list
 */
typedef struct nodeList{
    int val;
    struct nodeList *next;
} nodeList;

/******* GLOBAL VARIABLES ************************************************************************/
int numberOfAddedNodes = 0; // numberOfAddedNodes to create array to fill node->next
struct node* globalPointerToFirstAdmissibleNode; // address of first element of admissible List
int isFindGlobalPointer; // used to find globalPointer for the first time
int k; //length of all string

/******* LIST METHODS ****************************************************************************/

/**
 * add method is used to insert new element into the list
 * @param x: value of new element to insert
 * @param head: pointer to the first element of the list (head of the list)
 * @return: the pointer of the new list with the new element inside it
*/
nodeList* add(int x, nodeList** head) {

    if((*head) != NULL) {
        nodeList* mom = (*head);
        (*head) = (nodeList*)malloc(sizeof(nodeList));
        (*head) -> val = x;
        (*head) -> next = mom;
    } else {
        (*head) = (nodeList*)malloc(sizeof(nodeList));
        (*head) -> val = x;
        (*head) -> next = NULL;
    }

    return (*head);

}

/**
 * printList is the method used to print all the elements of the list
 * @param head: pointer to the list
*/
void printList(struct node *head) {

    if (head != NULL) {

        printf("Head -> ");

        while(head != NULL){

            if(head->next != -1) {
                printf("%s -> ", head->str);
                head = head -> next;
            } else {
                printf("questo:%s", head->str);
                head = NULL;
            }
        }

        printf("NULL\n");
    } else {
        printf("empty list\n");
    }
}

/******* BST METHODS *******************************************************************************/

/**
 * stringComparison is used to determine the bigger string
 * @param s1 first string
 * @param s2 second string
 * @return 1 if s1 > s2, 0 if s1 < s2, -1 if error occurred
 */
int stringComparison(char** firstStr, char** secondStr){
    char str1[k + 1];
    char str2[k + 1];
    strncpy(str1, *firstStr, k);
    strncpy(str2, *secondStr, k);

    int index;
    //the lengths of s1 and s2 are always equals
    for(index = 0; index < k; index++){
        if(str1[index] > str2[index])
            return 1;
        else if(str2[index] > str1[index])
            return 0;
    }
    return -1;
}

/**
 * treeSearch is the method used to find an element inside the tree
 * @param root the address of the root of the tree
 * @param str string to find inside the tree
 * @return NULL if str is not inside the tree or the address of the node
 */
struct node* treeSearch(struct node** root, char** str){
    if(*root == NULL)
        return NULL;
    //strncmp returns 0 if the strings are equals
    if(!strncmp(*str, (*root)->str, k))
        return *root;
    //if str < x->str
    if(!stringComparison(str, &(*root)->str))
        return treeSearch(&(*root)->left, str);
    else
        return treeSearch(&(*root)->right, str);
}

/**
 * inorderTreeWalk is used to visit each node of the tree
 * @param currentNode address of the root of the tree
 */
void inorderTreeWalk(struct node** currentNode){
    if (*currentNode != NULL) {
        inorderTreeWalk(&(*currentNode) -> left);
        //printf("%s , %ld \n", (*currentNode) -> str, (*currentNode));
        printf("%s, address = %ld, next = %ld\n", (*currentNode) -> str, (*currentNode), (*currentNode)->next);
        inorderTreeWalk(&(*currentNode) -> right);
    }

}

/**
 * initializeNextField is used to initialize each next field to the next node
 * @param currentNode address of the root of the tree
 * @param predecessor to save the content of the last visited node, the first assignment
 * predecessor is the address of the less node
 * N.B: before use this method is mandatory find the minimum node
 */

void initializeNextField(struct node** currentNode, struct node** predecessor){

    if (*currentNode != NULL) {

        initializeNextField(&(*currentNode) -> left, predecessor);

        (*predecessor)->next = *currentNode;
        *predecessor = *currentNode;
        //printf("curNode = %s (add = %d), address NEXT = %d\n",(*currentNode)->str, (*currentNode), (*currentNode)->next);

        initializeNextField(&(*currentNode)->right, predecessor);
    }
    return;
}

/**
 * updateNextField is used to update each next field to the next admissible node
 * we will call it when we will add string to bst ("+inserisci_inizio ... fine")
 * @param currentNode address of the root of the tree
 * @param predecessor to save the content of the last visited node
 * N.B: before use this method is mandatory find the minimum node and set isFindGlobalPointer to 0
 */
void updateNextField(struct node** currentNode, struct node** predecessor){

    if (*currentNode != NULL) {

        updateNextField(&(*currentNode) -> left, predecessor);
        struct node *momNode = *currentNode;

        if(momNode->next != -1) {

            if (!isFindGlobalPointer) {
                globalPointerToFirstAdmissibleNode = momNode;
                isFindGlobalPointer = 1;
                *predecessor = *currentNode;
            } else {
                (*predecessor)->next = *currentNode;
                *predecessor = *currentNode;
            }
        }
        updateNextField(&(*currentNode) -> right, predecessor);

    }

}

/**
 * deleteNode is used to change the next field of the node to delete
 * @param currentNode the start pointer to the first admissible string
 * @param predecessor the info of the last node
 * @param strToDelete string to delete
 */
void deleteNode(struct node** currentNode, struct node** predecessor, char* strToDelete){

    if(*currentNode != NULL){

        if(!strncmp((*currentNode)->str, strToDelete, k)){

            //if current node is the root of the tree and i equal to str to delete
            struct node *momNode = *currentNode;

            if(*currentNode == globalPointerToFirstAdmissibleNode){
                globalPointerToFirstAdmissibleNode = (*currentNode)->next;
                momNode->next = -1;
            }else{
                (*predecessor)->next = (*currentNode)->next;
                momNode->next = -1;
            }
            return;
        }

        deleteNode(&(*currentNode)->next, currentNode, strToDelete);
    }

}

/**
 * treeMinimum to find the minimum element of the tree
 * the passage of the node is for value not for reference
 * if it was for reference the mainTree will changed
 * @param node main BST for value
 * @return the value of the minimum element
 */
struct node* treeMinimum(struct node* node){
    while((node)->left != NULL)
        node = (node)->left;
    return node;
}

/**
 * treeMax is used to find the greater element of the tree
 * the passage of the node is for value not for reference
 * if it was for reference the mainTree will changed
 * @param node main BST for value
 * @return the value of the minimum element
 */
struct node* treeMax(struct node* node){
    while((node)->right != NULL)
        node = (node)->right;
    return node;
}

/**
 * treeInsert is used to add a new node inside the tree
 * @param root address of the tree's root
 * @param strToInsert new string to add to the tree
 * @param nextValue is 0 if the word respect constraints or 1 if it doesn't
 * @return the address of the tree's root
 */
struct node* treeInsert(struct node** root, char* strToInsert, int nextValue){

    numberOfAddedNodes++;
    char *str = (char*)malloc(sizeof(char)*(k+1));
    strncpy(str, strToInsert, k);
    str[k] = '\0';
    struct node* nodeToInsert = (struct node*)malloc(sizeof(struct node));
    nodeToInsert->str = str;

    nodeToInsert->right = NULL;
    nodeToInsert->left = NULL;

    struct node* fatherOfNodeToInsert = NULL;
    struct node* iterableNode = *root;

    while(iterableNode != NULL){

        fatherOfNodeToInsert = iterableNode;

        //if ( iterableNode->str > nodeToInsert->str )
        if(stringComparison(&iterableNode->str, &nodeToInsert->str))
            iterableNode = iterableNode->left;
        else
            iterableNode = iterableNode->right;

    }
    //we are arrived in the position where the new node will be placed
    //nodeToInsert->p = fatherOfNodeToInsert;

    if(fatherOfNodeToInsert == NULL){
        return nodeToInsert;
        //if nodeToInsert is less than father
    }else if(stringComparison(&fatherOfNodeToInsert->str, &nodeToInsert->str)){
        fatherOfNodeToInsert->left = nodeToInsert;
        //if nodeToInsert is greater than father
    }else {
        fatherOfNodeToInsert->right = nodeToInsert;
    }

    return *root;
}

/******* GAME METHODS *******************************************************************************/

/**
 * stampeFiltrate is the method used to slide the compatible words
 * from the global pointer to the end of the linked list
 */
void stampeFiltrate(){

    struct node* momNodeForstampeFiltrate;
    momNodeForstampeFiltrate = globalPointerToFirstAdmissibleNode;

    while(momNodeForstampeFiltrate != NULL){
        // char momStr = (char*)malloc(sizeof(char)*(k+1));
        // strncpy(momStr, momNodeForstampeFiltrate->str, k+1);
        printf("%s\n", momNodeForstampeFiltrate->str);
        momNodeForstampeFiltrate = momNodeForstampeFiltrate->next;

    }
}

/**
 * dimLinkedList
*/
int dimLinkedList() {
    //TOD: si puo ottimizzare salvando la dimensione della lunghezza
    struct node* head = NULL;
    head = globalPointerToFirstAdmissibleNode;
    int count = 0;
    while(head != NULL){
        count++;
        head = head -> next;
    }
    return count;
}

/**
 * singleCheck used to add string when it reads "inserisci_inizio"
 * This method check the read word with constraints and
 * @return 0 if it respect the constraints or 1 if it doesn't
*/
int singleCheck(int noCharBroadcast[], int minOccChar[], int perfectOccChar[], int yesCharIdx[], nodeList* noCharIdx[], char* buf) {

    char *str = (char*)malloc(sizeof(char)*(k+1));
    strncpy(str, buf, k);
    str[k] = '\0';
    //printf("single check: %s\n", str);

    int instances[ASCII_TABLE_SIZE];
    int stop = 0;
    int i, idx;
    nodeList* curList = NULL;

    //initialize the array
    for(i = 0; i < ASCII_TABLE_SIZE; i++){
        instances[i] = 0;
    }
        
    // find instances of each char of str
    for(i = 0; i < k; i++){
        instances[(int)str[i]]++;
    }
        

    /*------------------------------*/

    //check yesChar[]
    for(i = 0; i < k && !stop; i++)
        if(yesCharIdx[i] != -1)
            if(yesCharIdx[i] != (int)str[i]){
                stop = 1;
            }
                

    //check minOccChar
    for(i = 0; i < ASCII_TABLE_SIZE && !stop; i++){
        if(minOccChar[i] != 0)
            if(instances[i] < minOccChar[i]){
                stop = 1;
            }
    }

    //check perfectOccChar[] A
    for(i = 0; i < ASCII_TABLE_SIZE && !stop; i++){
        if(perfectOccChar[i] != -1)
            if(instances[i] != perfectOccChar[i]){
                stop = 1;
                // printf("&=%d,(%c) %d(%c) \n", perfectOccChar[i], i, instances[i], i);
            }
    }

    //check noCharIdx
    for(i = 0; i < k && !stop; i++){
        curList = noCharIdx[i];

        while(curList != NULL && !stop) {

            if(curList->val == str[i])
                stop = 1;
            curList = curList -> next;

        }
    }

    //check noCharBroadcast
    for(i = 0; i < k && !stop; i++){
        idx = (int)str[i];
        if(noCharBroadcast[idx] == 1)
            stop = 1;
    }
    return stop;
}

/**
 * checkLinkedList is used to filter the linked list of admissible words
 * for each word that doesn't respect constraints delete function will be called
 */
void checkLinkedList(int noCharBroadcast[], int minOccChar[], int perfectOccChar[], int yesCharIdx[], nodeList* noCharIdx[]) {

    struct node* curNode = globalPointerToFirstAdmissibleNode;
    int stop;
    struct node* predecessor = NULL;
    struct node* mom = NULL;
    while(curNode != NULL) {

        stop = singleCheck(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx, curNode->str);
        //stop = 1;
        if(stop) {

            if(curNode == globalPointerToFirstAdmissibleNode) {

                globalPointerToFirstAdmissibleNode = curNode->next;
                mom = curNode;
                curNode = curNode->next;
                mom->next = -1;

            } else {

                predecessor->next = curNode->next;
                mom = curNode;
                curNode = curNode->next;
                mom->next = -1;

            }
        } else {
            predecessor = curNode;
            curNode = curNode->next;
        }
    }
    //printList(globalPointerToFirstAdmissibleNode);
}

/**
 * match method is used to print result string in output
 * and learn the new constraints born form the comparison
 * @param rString first string r (reference)
 * @param pString second string p (it change every time)
 */
void match(char* rString, char* pString, int noCharBroadcast[], int yesCharIdx[],
           nodeList* noCharIdx[], int minOccChar[], int perfectOccChar[]){

    int index;
    int minOccCharCur[ASCII_TABLE_SIZE];
    int i;
    int rN[ASCII_TABLE_SIZE]; //occurrences of each character of r
    int rC[ASCII_TABLE_SIZE]; //number of match between r and p of each character
    int rX[ASCII_TABLE_SIZE]; //number of occurrences of non match characters

    //initialize the arrays
    for(i = 0; i < ASCII_TABLE_SIZE; i++) {
        rN[i] = 0;
        rC[i] = 0;
        rX[i] = 0;
        minOccCharCur[i] = 0;
    }

    char *res = (char*)malloc(sizeof(char)*(k+1));
    res[k] = '\0';
    //initialize rN and rC and find the '+'
    for(i = 0; i < k; i++) {
        index = (int)(rString[i]);
        rN[index]++;
        if(pString[i] == rString[i]) {
            res[i] = '+';
            rC[index]++;
        }
    }

    for(i = 0; i < k; i++) {

        int asciiVal = (int)pString[i];
        //printf("ascci = %d, pstring = %s, i = %d\n", asciiVal, pString, i);
        if(res[i] == '+') {
            yesCharIdx[i] = asciiVal;
            minOccCharCur[asciiVal]++;
            // if(minOccCharCur[asciiVal] > 0)
            //     minOccCharCur[asciiVal]--;
        }
        else if(rN[asciiVal] == 0) {
            //character not present in r
            //if(rN[(int)pString[i] == 0): res[i] = '/'
            res[i] = '/';
            noCharBroadcast[asciiVal] = 1;
        } else if(rN[asciiVal] - rC[asciiVal] > rX[asciiVal]) {
            //character present in r but not in the correct position
            //if(rN - rC > rX): res[i] = '|'
            res[i] = '|';
            rX[asciiVal]++;
            minOccCharCur[asciiVal]++;
            add(asciiVal, &noCharIdx[i]);

        } else if(rN[asciiVal] - rC[asciiVal] <= rX[asciiVal]) {
            //if(rN - rC <= rX): res[i] = '/'
            res[i] = '/';
            add(asciiVal, &noCharIdx[i]);
            perfectOccChar[asciiVal] = rN[asciiVal];
        }
    }
    for(i = 0; i< ASCII_TABLE_SIZE; i++)
        if(minOccCharCur[i] > minOccChar[i])
            minOccChar[i] = minOccCharCur[i];
            
    //printf("%s (p: %s , r: %s), %d, %d\n", res, pString, rString, minOccChar[109], perfectOccChar[109]);
    printf("%s\n", res);

}

void initializeConstraintsStruct(int noCharBroadcast[], int minOccChar[], int perfectOccChar[],
                                 int yesCharIdx[], nodeList* noCharIdx[]) {

    for(int i = 0; i < k; i++) {
        yesCharIdx[i] = -1;
        noCharIdx[i] = NULL;
    }

    for(int i = 0; i < ASCII_TABLE_SIZE; i++) {
        noCharBroadcast[i] = -1;
        minOccChar[i] = 0;
        perfectOccChar[i] = -1;
    }
}

/**
 * Main App to tests
 * Pay attention! In the online tester
 * is mandatory assign the return value to a variable
 * @return 0
 */
int main() {
    clock_t t;
    t = clock();
    //LOCAL VARIABLES:
    char bufferK[DIM_BUFFER_K]; //buffer to read k
    char buffer[MAX_DIM_BUFFER]; //buffer to read each line of input
    int firstPhase; //when this variable is 1 we are in the first phase
    int addString; //when this variable is 1 we are in the second phase
    struct node* root = NULL; //root of BST
    int firstString; //to check if it is the first world to add to bst
    struct node* predecessor = NULL; //to fill next field
    int n; //n is the max number of comparison for match with r
    int endGame; //if it is 1 => EOF
    char *rString; //word of reference
    char *pString; //word to compare
    int newMatch; //when is 1 is starting a new match
    int isFirststampeFiltrate = 1; //1 if stampe filtrate hasn't been read yet
    int stopPrintKO = 0;
    //constraints:
    int noCharBroadcast[ASCII_TABLE_SIZE];
    int minOccChar[ASCII_TABLE_SIZE];
    int perfectOccChar[ASCII_TABLE_SIZE];
    //END LOCAL VARIABLES DECLARATIONS



    //FIRST PHASE: read K and the list of admissible words, when it reads "+nuova_partita" the SECOND phase begins
    firstPhase = 1;
    addString = 0;
    firstString = 1;

    k = strtol(fgets(bufferK, DIM_BUFFER_K, stdin), NULL, BASE_10);

    //LOCAL VARIABLES DEPENDENT FROM K:
    int yesCharIdx[k];
    nodeList* noCharIdx[k];
    //END LOCAL VARIABLES DECLARATIONS


    //while the input is != from "+nuova_partita" and NULL
    while ((fgets(buffer, MAX_DIM_BUFFER, stdin)) != NULL && firstPhase) {

        //"+nuova_partita" => second phase
        if (buffer[0] == '+' && buffer[1] == 'n') {
            if(isFirststampeFiltrate) {
                isFirststampeFiltrate = 0;
                predecessor = treeMinimum(root);
                globalPointerToFirstAdmissibleNode = predecessor;
                initializeNextField(&root, &predecessor);
                treeMax(root)->next = 0;
                //printf("GlobalPointer = %s, address = %d\n", (globalPointerToFirstAdmissibleNode)->str, globalPointerToFirstAdmissibleNode);
                //inorderTreeWalk(&root);
            }
            firstPhase = 0;
            initializeConstraintsStruct(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx);

            //"+stampa_filtrate", print out all the compatible words, from next
        } else if(buffer[0] == '+' && buffer[1] == 's') {

            //if it is the first time that we read "stampe filtrate"
            // we initialize the next field of each node, no constraints yet
            if(isFirststampeFiltrate) {
                isFirststampeFiltrate = 0;
                predecessor = treeMinimum(root);
                globalPointerToFirstAdmissibleNode = predecessor;
                initializeNextField(&root, &predecessor);
                treeMax(root)->next = 0;
                //printf("GlobalPointer = %s, address = %d\n", (globalPointerToFirstAdmissibleNode)->str, globalPointerToFirstAdmissibleNode);
                //inorderTreeWalk(&root);
            }
            stampeFiltrate();

            //"+inserisci_inizio" begins the insert of new admissible words
        } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[16] == 'o') {
            addString = 1;

            //"+inserisci_fine" end of insert
        } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[14] == 'e') {
            addString = 0;

            //add string to BST, after reading "+inserisci_inizio"
        } else if (addString) {
            //list of new words

            if (firstString) {
                root = treeInsert(&root, buffer, 0);
                firstString = 0;
            } else
                treeInsert(&root, buffer, 0);

            //list of words to add to BST, first part of the programme
        } else {

            if(firstString) {
                root = treeInsert(&root, buffer, 0);
                firstString = 0;
            } else
                treeInsert(&root, buffer, 0);
        }
    }

    /* ------------------------------------------------------ */

    //SECOND PHASE: every time i reads "+nuova_partita"
    endGame = 0;

    while (!endGame) {

        addString = 0;
        newMatch = 0;//if new match is read => new match will be 1

        rString = (char*)malloc(sizeof(char)*(k + 1)); //reference string
        strncpy(rString, buffer, k); //I already have it in the buffer since the last comparison
        rString[k] = '\0';
        n = strtol(fgets(bufferK, DIM_BUFFER_K, stdin), NULL, BASE_10);
        stopPrintKO = 0;

        while ((fgets( buffer, MAX_DIM_BUFFER, stdin)) != NULL && !newMatch) {

            //"+nuova_partita" => newMatch = 1
            if (buffer[0] == '+' && buffer[1] == 'n') {

                initializeConstraintsStruct(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx);
                //printf("GP = %d,  %s\n", globalPointerToFirstAdmissibleNode, globalPointerToFirstAdmissibleNode->str);
                predecessor = treeMinimum(root);
                globalPointerToFirstAdmissibleNode = predecessor;
                initializeNextField(&root, &predecessor);
                treeMax(root)->next = 0;
                //printf("GlobalPointer = %s, address = %d\n", (globalPointerToFirstAdmissibleNode)->str, globalPointerToFirstAdmissibleNode);
                //inorderTreeWalk(&root);
                newMatch = 1;

                //"+inserisci_inizio" begins the insert of new admissible words
            } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[16] == 'o'){
                addString = 1;

                //"+inserisci_fine" end of insert
            } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[14] == 'e'){

                addString = 0;
                isFindGlobalPointer = 0;
                predecessor = treeMinimum(root);
                updateNextField(&root, &predecessor);

                //"+stampa_filtrate", print out all the compatible words, from next
            } else if (buffer[0] == '+' && buffer[1] == 's') {

                checkLinkedList(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx);
                stampeFiltrate();

                //between "+inserisci_inizio" and "+inserisci_fine"
            } else if (addString){
                //TODO: if it is the first string, is it possible?
                treeInsert(&root, buffer, singleCheck(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx, buffer));

                //comparison between p and r MATCH
            } else {

                pString = (char*)malloc(sizeof(char)*(k+1));
                strncpy(pString, buffer, k);
                pString[k] = '\0';

                if(treeSearch(&root, &pString) != 0){// 0 or NULL TODO
                    if(!strncmp(rString, pString, k))
                        printf("ok\n");
                    else {
                        match(rString, pString, noCharBroadcast, yesCharIdx, noCharIdx, minOccChar, perfectOccChar);
                        checkLinkedList(noCharBroadcast, minOccChar, perfectOccChar, yesCharIdx, noCharIdx);
                        printf("%d\n", dimLinkedList());
                        n--;
                    }
                } else
                    printf("not_exists\n");

            }
        }
        if(!newMatch)
            endGame = 1;

        //KO (we reached n comparison)
        if(n == 0 && !stopPrintKO) {
            printf("ko\n");
            stopPrintKO = 1;
        }
    }



/**
 * TODO:
 * 1) trovare bug con test3
 * 2) testare fortissimo e trovare altri bug e risolverli
 * Una volta che passo upto18 del gruppo:
 * 3) cambiare strutture dati per vincoli con liste ovunque
 * 4) schiaffare codice su valgrind, vedere bottle neck e ottimizzare
 */








//proba










    //inorderTreeWalk(&root);
/*



    //initialize list
    //initialization of global pointer must stay here because the value of predecessor will change

    predecessor = treeMinimum(root);
    globalPointerToFirstAdmissibleNode = predecessor;
    initializeNextField(&root, &predecessor);

    //update list
    //isFindGlobalPointer = 0;
    //predecessor = treeMinimum(root);
    //updateNextField(&root, &predecessor);
    //printf("numbers of nodes = %d", numberOfAddedNodes);
    //inorderTreeWalk(&root);
*/
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("TIME =  %f seconds to execute \n", time_taken);
    return 0;
}

