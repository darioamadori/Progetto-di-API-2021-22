/**
 * Project Name: WordChecker
 * Author: Amadori Dario
 * Date: 2022
 * Course: Algorithms and Principles of Computer Engineering
 * Institution: Politecnico di Milano
 * Academic Year: 2022
 * Description:
 *  The aim of this year's project is to develop a system named WordChecker, designed to
 *  validate the correspondence between the letters of two words of equal length. This system
 *  operates on words composed of lowercase and uppercase alphabetic characters (a-z, A-Z),
 *  numeric digits (0-9), and the symbols '-' (dash) and '_' (underscore).
 *
 *  WordChecker functions by reading from standard input a series of instructions and data,
 *  including a value 'k' representing the length of the words to be processed, and an arbitrary
 *  sequence of admissible words of length 'k', ensuring no duplicates. Following the input of
 *  admissible words, the system reads sequences of "matches", marked by the command '+new_match',
 *  each involving a reference word of length 'k' (assumed to be among the admissible words),
 *  a maximum number 'n' of words to be compared with the reference, and a sequence of words
 *  of length 'k' to compare against the reference word.
 *
 *  Additionally, the system can receive commands '+insert_start' and '+insert_end' to add new
 *  admissible words of length 'k' to the pool, with the assumption of no duplicate entries.
 *  The '+print_filtered' command may appear to trigger specific output actions during or between
 *  matches.
 *
 * Key Features:
 *  - Validates letter correspondence between words of equal length.
 *  - Manages an expandable set of admissible words.
 *  - Processes sequences of matches with reference and comparison words.
 *  - Allows dynamic addition of admissible words to the system.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/******* CONSTANTS ***********/
#define DIM_BUFFER_K 5
#define MAX_DIM_BUFFER 1000
#define BASE_10 10
#define ASCII_TABLE_SIZE 124

/******* STRUCT ************************************************************************/
/**
 * This struct represents a single node of BST
 * char str*: string of length k
 * node *left: pointer to left subtree (contains only nodes with str lesser than the node’s str)
 * node *right: pointer to right subtree (contains only nodes with str bigger than the node’s str)
 * node *next: pointer to the next node who respects the constraints learned so far
 */
struct node {
    char *str;
    struct node *left;
    struct node *right;
    struct node *next;
};

/**
 * nodeDict is used to store value with key and value (constraints)
 * char key: the letter or number or - , _
 * int val: the number of occurences of key
 * node *right
 * node *left
 */
struct nodeDict {
    char key;
    int val;
    struct nodeDict *left;
    struct nodeDict *right;
};

/**
 * simpleNode is used to store value with only key
 * char key: the letter or number or - , _
 * node *right
 * node *left
 */
struct simpleNode {
    char key;
    struct simpleNode *left;
    struct simpleNode *right;
};


/******* GLOBAL VARIABLES *********************************************************************************/
int numberOfAddedNodes = 0; // numberOfAddedNodes to create array to fill node->next
struct node* globalPointerToFirstAdmissibleNode; // address of first element of admissible List
int isFindGlobalPointer; // used to find globalPointer for the first time
int k; // length of all string
int stop; // stop is 1 when the word doesn't respect constraints any more

/******* SIMPLE NODE METHODS *******************************************************************************/
/**
 * myStrnCmp is my version of strncmp() of <string.h>
 * @param str1 first string
 * @param str2 second string
 * @return 0 if the the strings are equal, 1 if they aren't 
*/
int myStrnCmp(char* str1, char* str2) {

    for (int i = 0; i < k; i++) {
        if (str1[i] != str2[i])
            return 1;
    }

    return 0;

}

/**
 * simpleNodeSearch
 * @param str1 first string
 * @param str2 second string
 * @return 0 if the the strings are equal, 1 if they aren't 
*/
struct simpleNode* simpleNodeSearch(struct simpleNode* simpleNode, char charKey) {

    if(simpleNode == NULL)
        return NULL;

    if(simpleNode->key == charKey)
        return simpleNode;

    if(charKey < simpleNode->key)
        return simpleNodeSearch(simpleNode->left, charKey);
    else
        return simpleNodeSearch(simpleNode->right, charKey);

}

struct simpleNode* simpleNodeInsert(struct simpleNode** root, char keyChar){

    struct simpleNode* nodeToInsert = (struct simpleNode*)malloc(sizeof(struct simpleNode));
    nodeToInsert->key = keyChar;

    nodeToInsert->right = NULL;
    nodeToInsert->left = NULL;

    struct simpleNode* fatherOfNodeToInsert = NULL;
    struct simpleNode* iterableNode = *root;

    while(iterableNode != NULL){

        fatherOfNodeToInsert = iterableNode;

        if(iterableNode->key == nodeToInsert->key)
            return *root;
        else if(iterableNode->key > nodeToInsert->key)
            iterableNode = iterableNode->left;
        else
            iterableNode = iterableNode->right;

    }

    if (fatherOfNodeToInsert == NULL) {
        return nodeToInsert;
        //if nodeToInsert is less than father
    } else if (nodeToInsert->key < fatherOfNodeToInsert->key) {
        fatherOfNodeToInsert->left = nodeToInsert;
        //if nodeToInsert is greater than father
    } else {
        fatherOfNodeToInsert->right = nodeToInsert;
    }

    return *root;
}

void deallocateSimpleNode(struct simpleNode* node) {

    if(node != NULL) {
        deallocateSimpleNode(node->left);
        free(node);
        deallocateSimpleNode(node->right);
    }
}

/******* BST METHODS *******************************************************************************/

/**
 * stringComparison is used to determine the bigger string
 * @param s1 first string
 * @param s2 second string
 * @return 1 if s1 > s2, 0 if s1 < s2, -1 if error occurred
 */
int stringComparison(char* str1, char* str2){
    // char str1[k + 1];
    // char str2[k + 1];

    // strncpy(str1, *firstStr, k);
    // strncpy(str2, *secondStr, k);

    int i;
    //the lengths of s1 and s2 are always equals
    for(i = 0; i < k; i++){

        if (str1[i] > str2[i])
            return 1;
        else if (str2[i] > str1[i])
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
struct node* treeSearch(struct node** root, char* str){
    if(*root == NULL)
        return NULL;
    //strncmp returns 0 if the strings are equals
    if(!myStrnCmp(str, (*root)->str))
        return *root;
    //if str < x->str
    if(!stringComparison(str, (*root)->str))
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

        if(momNode->next != (struct node*)-1) {

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

    if(nextValue == 1)
        nodeToInsert->next = (struct node*)-1;

    nodeToInsert->right = NULL;
    nodeToInsert->left = NULL;

    struct node* fatherOfNodeToInsert = NULL;
    struct node* iterableNode = *root;

    while(iterableNode != NULL){

        fatherOfNodeToInsert = iterableNode;

        //if ( iterableNode->str > nodeToInsert->str )
        if(stringComparison(iterableNode->str, nodeToInsert->str))
            iterableNode = iterableNode->left;
        else
            iterableNode = iterableNode->right;

    }
    //we are arrived in the position where the new node will be placed
    //nodeToInsert->p = fatherOfNodeToInsert;

    if(fatherOfNodeToInsert == NULL){
        return nodeToInsert;
        //if nodeToInsert is less than father
    }else if(stringComparison(fatherOfNodeToInsert->str, nodeToInsert->str)){
        fatherOfNodeToInsert->left = nodeToInsert;
        //if nodeToInsert is greater than father
    }else {
        fatherOfNodeToInsert->right = nodeToInsert;
    }

    return *root;
}

/******* DICTIONARY METHODS *******************************************************************************/

struct nodeDict* dictInsert(struct nodeDict** root, char keyChar, int number){

    struct nodeDict* nodeToInsert = (struct nodeDict*)malloc(sizeof(struct nodeDict));
    nodeToInsert->key = keyChar;
    nodeToInsert->val = number;

    nodeToInsert->right = NULL;
    nodeToInsert->left = NULL;

    struct nodeDict* fatherOfNodeToInsert = NULL;
    struct nodeDict* iterableNode = (*root);

    while(iterableNode != NULL){

        fatherOfNodeToInsert = iterableNode;

        if(iterableNode->key == nodeToInsert->key){

            iterableNode->val = number;
            return *root;

        } else if(iterableNode->key > nodeToInsert->key)
            iterableNode = iterableNode->left;
        else
            iterableNode = iterableNode->right;

    }

    if (fatherOfNodeToInsert == NULL) {
        return nodeToInsert;
        //if nodeToInsert is less than father
    } else if (nodeToInsert->key < fatherOfNodeToInsert->key) {
        fatherOfNodeToInsert->left = nodeToInsert;
        //if nodeToInsert is greater than father
    } else {
        fatherOfNodeToInsert->right = nodeToInsert;
    }

    return *root;
}

int dictSearch(struct nodeDict* root, char charKey){

    if(root == NULL)
        return (int)NULL;

    if(root->key == charKey)
        return root->val;

    if(charKey < root->key)
        return dictSearch(root->left, charKey);
    else
        return dictSearch(root->right, charKey);

}


void deallocateDict(struct nodeDict* node) {

    if(node != NULL) {
        deallocateDict(node->left);
        free(node);
        deallocateDict(node->right);
    }
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

void walkPerfectOccChar(struct nodeDict** curNode, char* str){

    if ((*curNode) != NULL && stop == 0) {

        walkPerfectOccChar(&(*curNode)->left, str);
        int keyChar = (*curNode)->key;

        int valChar = (*curNode)->val;
        int cont = 0;
        for (int i = 0; i < k; i++)
            if(str[i] == keyChar)
                cont++;

        if (cont != valChar) 
            stop = 1;
        
        if (stop == 0)
            walkPerfectOccChar(&(*curNode)-> right, str);
    }
}

void walkMinOccChar(struct nodeDict** curNode, char* str){

    if ((*curNode) != NULL && stop == 0) {

        walkMinOccChar(&(*curNode)->left, str);
        int keyChar = (*curNode)->key;

        int valChar = (*curNode)->val;
        int cont = 0;
        for (int i = 0; i < k; i++)
            if(str[i] == keyChar)
                cont++;

        if (cont < valChar){
            stop = 1;
        }
        
        if (stop == 0)
            walkMinOccChar(&(*curNode)-> right, str);
    }
}


int singleCheck(struct nodeDict** minOccChar, struct simpleNode** noCharBroadcast, 
    struct nodeDict** perfectOccChar, int yesCharIdx[], struct simpleNode* noCharIdx[], char* str) {

    int i;
    char curChar;
    stop = 0;
    // nodeList* curList = NULL;

    /*------------------------------------
    Constraints (from stronger to weaker)
    try to chage this order to do better times TODO
    1) yesCharIdx
    2) perfectOccChar
    3) minOccChar
    4) noCharBroadcast
    5) noCharIdx
    ------------------------------------*/

    // yesCharIdx[]
    for (i = 0; i < k && !stop; i++) {

        curChar = str[i];
        if (yesCharIdx[i] != -1 && yesCharIdx[i] != (int)curChar)
            return 1;

    }

    // perfectOccChar[]
    walkPerfectOccChar(perfectOccChar, str);
    if (stop == 1)
        return 1;
    

    // noCharIdx[]
    for (i = 0; i < k && !stop; i++) {
        
        curChar = str[i];
        if (simpleNodeSearch(noCharIdx[i], curChar) != NULL)
            return 1;

    }

    // minOccChar[]
    walkMinOccChar(minOccChar, str);
    if (stop == 1)
        return 1;
        

    // noCharBroadcast
    for (i = 0; i < k && !stop; i++) {

        curChar = str[i];
        if (simpleNodeSearch(*noCharBroadcast, curChar) != NULL)
            return 1;
    }

    return 0;
}


/**
 * match method is used to print result string in output
 * and learn the new constraints born form the comparison
 * @param rString first string r (reference)
 * @param pString second string p (it change every time)
 */
void matchAndCheck(int rN[], int rC[], int rX[], int minOccCharCur[], char* rString, char* pString, struct nodeDict** minOccChar, struct simpleNode** noCharBroadcast, 
    struct nodeDict** perfectOccChar, int yesCharIdx[], struct simpleNode* noCharIdx[]){


    // struct nodeDict* rN; // occurrences of each character of r
    // struct nodeDict* rC; // number of match between r and p of each character
    // struct nodeDict* rX; // number of occurrences of non match characters

    int asciiVal;
    int asciiVal2;
    int i;

    //initialize the arrays
    for(i = 0; i < k; i++) {
        asciiVal = (int)rString[i];
        rN[asciiVal] = 0;
        rC[asciiVal] = 0;
        rX[asciiVal] = 0;
        minOccCharCur[asciiVal] = 0;
        asciiVal2 = (int)pString[i];
        rN[asciiVal2] = 0;
        rC[asciiVal2] = 0; 
        rX[asciiVal2] = 0;      
        minOccCharCur[asciiVal2] = 0;
    }

    //string to print out
    char *res = (char*)malloc(sizeof(char)*(k+1));
    res[k] = '\0';

    //initialize rN and rC and find the '+'
    for(i = 0; i < k; i++) {
        asciiVal = (int)rString[i];
        rN[asciiVal]++;
        if(pString[i] == rString[i]) {
            res[i] = '+';
            rC[asciiVal]++;
        }
    }

    for(i = 0; i < k; i++) {

        asciiVal = (int)pString[i];

        if(res[i] == '+') {
            
            yesCharIdx[i] = asciiVal;
            minOccCharCur[asciiVal]++;

        } else if(rN[asciiVal] == 0) {

            //character not present in r
            // '/'  '/'  '/'  '/'  '/'  '/'
            res[i] = '/';

            *noCharBroadcast = simpleNodeInsert(noCharBroadcast, pString[i]);
                
            // noCharBroadcast[asciiVal] = 1;

        } else if(rN[asciiVal] - rC[asciiVal] > rX[asciiVal]) {
            //character present in r but not in the correct position
            //  '|' '|' '|' '|' '|' '|' '|' 
            res[i] = '|';
            rX[asciiVal]++;
            minOccCharCur[asciiVal]++;
            noCharIdx[i] = simpleNodeInsert(&noCharIdx[i], pString[i]);
            //add(asciiVal, &noCharIdx[i]);

        } else if(rN[asciiVal] - rC[asciiVal] <= rX[asciiVal]) {
            //  '/' '/' '/' '/' '/' '/' '/'
            res[i] = '/';
            noCharIdx[i] = simpleNodeInsert(&noCharIdx[i], pString[i]);
            //add(asciiVal, &noCharIdx[i]);
            *perfectOccChar = dictInsert(perfectOccChar, pString[i], rN[asciiVal]);
        }
    }

    int momCur;

    for (i = 0; i < k; i++) {
        momCur = dictSearch(*minOccChar, pString[i]);
        asciiVal = (int)pString[i];
        if (momCur == (int)NULL && minOccCharCur[asciiVal] > 0)
            *minOccChar = dictInsert(minOccChar, pString[i], minOccCharCur[asciiVal]);
        else
            if(minOccCharCur[asciiVal] > momCur)
                *minOccChar = dictInsert(minOccChar, pString[i], minOccCharCur[asciiVal]);
    }

    for (i = 0; i < k; i++)
        putchar(res[i]);
    putchar('\n');

    /********************** CHECK LINKED LIST *********************************/

    struct node* curNode = globalPointerToFirstAdmissibleNode;
    struct node* predecessor = NULL;
    struct node* mom = NULL;

    while(curNode != NULL) {

        stop = singleCheck(minOccChar, noCharBroadcast, perfectOccChar, yesCharIdx, noCharIdx, curNode->str);

        if(stop) {

            if(curNode == globalPointerToFirstAdmissibleNode) {

                globalPointerToFirstAdmissibleNode = curNode->next;
                mom = curNode;
                curNode = curNode->next;
                mom->next = (struct node*)-1;

            } else {

                predecessor->next = curNode->next;
                mom = curNode;
                curNode = curNode->next;
                mom->next = (struct node*)-1;

            }
        } else {
            predecessor = curNode;
            curNode = curNode->next;
        }
    }

}

void initializeConstraintsStruct(struct nodeDict** minOccChar, struct simpleNode** noCharBroadcast, struct nodeDict** perfectOccChar,
    int yesCharIdx[], struct simpleNode* noCharIdx[]) {

    deallocateDict(*minOccChar);
    deallocateSimpleNode(*noCharBroadcast);
    deallocateDict(*perfectOccChar);

    for (int i = 0; i < k; i++) {
        yesCharIdx[i] = -1;
       deallocateSimpleNode(noCharIdx[i]);
        noCharIdx[i] = NULL;
    }

    *minOccChar = NULL;
    *noCharBroadcast = NULL;
    *perfectOccChar = NULL;

}

/**
 * Main App to tests
 * Pay attention! In the online tester
 * is mandatory assign the return value to a variable
 * @return 0
 */
int main() {

    // TODO: delete it
    clock_t t;
    t = clock();

    //LOCAL VARIABLES:
    char bufferK[DIM_BUFFER_K]; //buffer to read k
    char buffer[MAX_DIM_BUFFER]; //buffer to read each line of input
    int firstPhase; //when this variable is 1 we are in the first phase
    int addString; //when this variable is 1 we are in the second phase
    struct node* root = NULL; //root of BST
    struct node* predecessor = NULL; //to fill next field
    int n; //n is the max number of comparison for match with r
    int endGame; //if it is 1 => EOF
    char *rString; //word of reference
    // char *pString; //word to compare
    int newMatch; //when is 1 is starting a new match
    int isFirststampeFiltrate = 1; //1 if stampe filtrate hasn't been read yet
    int stopPrintKO = 0;
    //constraints:
    struct nodeDict* minOccChar = NULL;
    struct simpleNode* noCharBroadcast = NULL;
    struct nodeDict* perfectOccChar = NULL;
    int rN[ASCII_TABLE_SIZE]; //occurrences of each character of r
    int rC[ASCII_TABLE_SIZE]; //number of match between r and p of each character
    int rX[ASCII_TABLE_SIZE]; //number of occurrences of non match characters
    int minOccCharCur[ASCII_TABLE_SIZE]; // to find the min occurences for this comparison
    //END LOCAL VARIABLES DECLARATIONS



    //FIRST PHASE: read K and the list of admissible words, when it reads "+nuova_partita" the SECOND phase begins
    firstPhase = 1;
    addString = 0;

    k = strtol(fgets(bufferK, DIM_BUFFER_K, stdin), NULL, BASE_10);

    //LOCAL VARIABLES DEPENDENT FROM K:
    int yesCharIdx[k];
    struct simpleNode* noCharIdx[k];
    for (int idx = 0; idx < k; idx++) 
        noCharIdx[idx] = NULL;
    //END LOCAL VARIABLES DECLARATIONS

    //while the input is != from "+nuova_partita" and NULL
    while ((fgets(buffer, MAX_DIM_BUFFER, stdin)) != NULL && firstPhase) {

        if(buffer[0] == '+') {

            //"+nuova_partita" => second phase
            if (buffer[1] == 'n') {
                if(isFirststampeFiltrate) {
                    isFirststampeFiltrate = 0;
                    predecessor = treeMinimum(root);
                    globalPointerToFirstAdmissibleNode = predecessor;
                    initializeNextField(&root, &predecessor);
                    treeMax(root)->next = 0;
                }
                firstPhase = 0;
                initializeConstraintsStruct(&minOccChar, &noCharBroadcast, &perfectOccChar, yesCharIdx, noCharIdx);

                //"+stampa_filtrate", print out all the compatible words, from next
            } else if(buffer[1] == 's') {

                //if it is the first time that we read "stampe filtrate"
                // we initialize the next field of each node, no constraints yet
                if(isFirststampeFiltrate) {
                    isFirststampeFiltrate = 0;
                    predecessor = treeMinimum(root);
                    globalPointerToFirstAdmissibleNode = predecessor;
                    initializeNextField(&root, &predecessor);
                    treeMax(root)->next = 0;
                }
                stampeFiltrate();

                //"+inserisci_inizio" begins the insert of new admissible words
            } else if (buffer[1] == 'i' && buffer[16] == 'o') {
                addString = 1;

                // "+inserisci_fine" end of insert
                // (buffer[1] == 'i' && buffer[14] == 'e')
            } else {
                addString = 0;

                //add string to BST, after reading "+inserisci_inizio"
            } 

        } else if (addString) {
            //list of new words

           root = treeInsert(&root, buffer, 0);

            //list of words to add to BST, first part of the programme
        } else {

            root = treeInsert(&root, buffer, 0);
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

            if (buffer[0] == '+') {

                //"+nuova_partita" => newMatch = 1
                if (buffer[1] == 'n') {

                    initializeConstraintsStruct(&minOccChar, &noCharBroadcast, &perfectOccChar, yesCharIdx, noCharIdx);
                    predecessor = treeMinimum(root);
                    globalPointerToFirstAdmissibleNode = predecessor;
                    initializeNextField(&root, &predecessor);
                    treeMax(root)->next = 0;
                    newMatch = 1;

                //"+stampa_filtrate", print out all the compatible words, from next   
                } else if (buffer[1] == 's') {

                    stampeFiltrate();

                //"+inserisci_inizio" begins the insert of new admissible words
                } else if (buffer[1] == 'i' && buffer[16] == 'o'){
                    addString = 1;

                    //"+inserisci_fine" end of insert
                } else {

                    addString = 0;
                    isFindGlobalPointer = 0;
                    predecessor = treeMinimum(root);
                    updateNextField(&root, &predecessor);

                }
            //between "+inserisci_inizio" and "+inserisci_fine"
            } else if (addString){

                treeInsert(&root, buffer, singleCheck(&minOccChar, &noCharBroadcast, &perfectOccChar, yesCharIdx, noCharIdx, buffer));

                //comparison between p and r MATCH
            } else {

                if(treeSearch(&root, buffer) != 0){// 0 or NULL TODO

                    if (!myStrnCmp(rString, buffer)) {
                        putchar('o');
                        putchar('k');
                        putchar('\n');
                    } else {
                        matchAndCheck(rN, rC, rX, minOccCharCur, rString, buffer, &minOccChar, &noCharBroadcast, &perfectOccChar, yesCharIdx, noCharIdx);
                        printf("%d\n", dimLinkedList());
                        n--;
                    }
                } else {
                    putchar('n');
                    putchar('o');
                    putchar('t');
                    putchar('_');
                    putchar('e');
                    putchar('x');
                    putchar('i');
                    putchar('s');
                    putchar('t');
                    putchar('s');
                    putchar('\n');
                }

            }
        }
        if(!newMatch)
            endGame = 1;

        //KO (we reached n comparison)
        if(n == 0 && !stopPrintKO) {
            putchar('k');
            putchar('o');
            putchar('\n');
            stopPrintKO = 1;
        }
    }
    //calculate execution time
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("TIME =  %f seconds to execute \n", time_taken);
    return 0;
}


