//This project implements a BST to put in the main project of API 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CONSTANTS:
#define DIM_BUFFER_K 5
#define MAX_DIM_BUFFER 1000
#define BASE_10 10
#define ASCII_TABLE_SIZE 124

/**
 * This struct represents a single node of BST
 * char str* is the string of length k
 * node *left is the pointer to left subtree (contains only nodes with str lesser than the node's str)
 * node *right is the pointer to right subtree (contains only nodes with str bigger than the node's str)
 * node *p is the pointer to father node
 * node *next is the pointer to the next node who respects the constraints learned so far
 */
struct node{
    char *str;
    struct node *left;
    struct node *right;
    //struct node *p;
    struct node *next;
};

/**
 * List
 */
typedef struct nodeList{
    
    int val;
    struct nodeList* next;

} nodeList;

//GLOBAL VARIABLES:
int numberOfAddedNodes = 0; // numberOfAddedNodes to create array for fill node->next
struct node* globalPointerToFirstAdmissibleNode; //address of first element of admissible List
int isFindGlobalPointer; //TODO
int k; //length of all string

//LIST METHODS:

nodeList* add(int x, nodeList* testa) {

    if(testa != NULL){
        nodeList* succ = testa;
        testa = (nodeList*)malloc(sizeof(node));
        testa -> val = x;
        testa -> next = succ;
    }
    else {
        testa = (nodeList*)malloc(sizeof(node));
        testa -> val = x;
        testa -> next = NULL;
    }
    return testa;
}

void printList(nodeList *testa) {

    if (testa != NULL) {

        printf("TESTA -> ");

        while(testa != NULL){

            printf("%d -> ", testa->val);
            testa = testa -> next;
        }

        printf("NULL\n");
    } else {
        printf("Lista vuota\n");
    }
}


//BST METHODS:

/**
 * stringComparison is used to determine the bigger string
 * @param s1 first string
 * @param s2 second string
 * @return 1 if s1 > s2, 0 if s1 < s2, -1 if error occurred
 */
int stringComparison(char** firstStr, char** secondStr){
    char str1[k + 1];
    char str2[k + 1];
    strcpy(str1, *firstStr);
    strcpy(str2, *secondStr);

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
    //strcmp returns 0 if the strings are equals
    if(!strcmp(*str, (*root)->str))
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

    if (*currentNode != NULL && (*currentNode)->str != (*predecessor)->str) {

        initializeNextField(&(*currentNode) -> left, predecessor);

        (*predecessor)->next = *currentNode;
        *predecessor = *currentNode;

        initializeNextField(&(*currentNode) -> right, predecessor);

    }

}

/**
 * updateNextField is used to update each next field to the next admissible node
 * @param currentNode address of the root of the tree
 * @param predecessor to save the content of the last visited node
 * N.B: before use this method is mandatory find the minimum node and set isFindGlobalPointer to 0
 */
void updateNextField(struct node** currentNode, struct node** predecessor){

    if (*currentNode != NULL) {

        updateNextField(&(*currentNode) -> left, predecessor);
        struct node *momNode = *currentNode;

        //153 line generates a warning: 
        //progetto.c:152:26: warning: comparison between pointer and integer
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
 * deleteNode is used to change the next field between the node to delete
 * @param currentNode the start pointer to the first admissible string
 * @param predecessor the info of the last node
 * @param strToDelete string to delete
 */
void deleteNode(struct node** currentNode, struct node** predecessor, char* strToDelete){

    if(*currentNode != NULL){

        if(!strcmp((*currentNode)->str, strToDelete)){
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
 * treeInsert is used to add a new node inside the tree
 * @param root address of the tree's root
 * @param strToInsert new string to add to the tree
 * @return the address of the tree's root
 * TODO: change type of strToInsert from char* to char** in the main project
 */
struct node* treeInsert(struct node** root, char* strToInsert){

    numberOfAddedNodes++;
    char *str = malloc(sizeof (char) * k + 1);
    strncpy(str, strToInsert, k);
    struct node* nodeToInsert = (struct node*)malloc(sizeof(struct node));
    nodeToInsert->str = str;
    nodeToInsert->right = NULL;
    nodeToInsert->left = NULL;
    //nodeToInsert->p = NULL;

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
//END OF BST METHODS

/**
 * stampeFiltrate is the method used to slide the compatible words
 * from the global pointer to the end of the linked list
 */
void stampeFiltrate(){

    struct node* momNodeForStampeFiltrate;
    momNodeForStampeFiltrate = globalPointerToFirstAdmissibleNode;
    while(momNodeForStampeFiltrate != NULL){
        //printf("%s\n", momNodeForStampeFiltrate->str);
        momNodeForStampeFiltrate = momNodeForStampeFiltrate->next;
    }
}

/**
 * match method to print result string in output
 * and learn the new constraints born form the comparison
 * @param rString first string r (reference)
 * @param pString second string p (it change every time)
 */
void match(char* rString, char* pString, int *noCharBroadcast, int *yesCharIdx,
           int *noCharIdx, int *minOccChar, int *perfectOccChar){

    int index;

    //printf("Match: rif = %sp = %s", rString, pString);
    int i;
    int rN[ASCII_TABLE_SIZE]; //occurrences of each character of r
    int rC[ASCII_TABLE_SIZE]; //number of match between r and p of each character
    int rX[ASCII_TABLE_SIZE]; //number of occurrences of non match characters

    //initialize the arrays
    for(i = 0; i < ASCII_TABLE_SIZE; i++) {
        rN[i] = 0;
        rC[i] = 0;
        rX[i] = 0;
    }

    char *res = malloc(sizeof(char) * (k + 1));



    //initialize rN and rC and find the '+'
    for(i = 0; i < k; i++) {
        index = (int)(rString[i]);
        rN[index]++;
        if(pString[i] == rString[i]) {
            res[i] = '+';
            rC[index]++;
            yesCharIdx[index]++;
        }
    }

    for(i = 0; i < k; i++) {

        int indexOfChar = (int)pString[i];

        if(res[i] == '+')
            continue;
        else if(rN[indexOfChar] == 0) {
            //character not present in r
            //if(rN[(int)pString[i] == 0): res[i] = '/'
            res[i] = '/';

        } else if(rN[indexOfChar] - rC[indexOfChar] > rX[indexOfChar]) {
            //character present in r but not in the correct position
            //if(rN - rC > rX): res[i] = '|'
            res[i] = '|';
            rX[indexOfChar]++;

        } else if(rN[indexOfChar] - rC[indexOfChar] <= rX[indexOfChar]) {
            //if(rN - rC <= rX): res[i] = '/'
            res[i] = '/';
            rX[indexOfChar]++;
        }
    }

    printf("res = %s\n", res);

}

/**
 * Main App to tests
 * Pay attention! In the online tester
 * is mandatory assign the return value to a variable
 * @return 0
 */
int main() {
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
    int isFirstStampeFiltrate = 1; //1 if stampe filtrate hasn't been read yet
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
    nodeList* yesCharIdx[k];
    nodeList* noCharIdx[k];
    //int yesCharIdx[ASCII_TABLE_SIZE];
    //END LOCAL VARIABLES DECLARATIONS


    //while the input is != from "+nuova_partita" and NULL
    while ((fgets(buffer, MAX_DIM_BUFFER, stdin)) != NULL && firstPhase) {

        //"+nuova_partita" => second phase
        if (buffer[0] == '+' && buffer[1] == 'n') {
            firstPhase = 0;

        //"+stampa_filtrate", print out all the compatible words, from next
        } else if(buffer[0] == '+' && buffer[1] == 's') {

            //if it is the first time that we read "stampe filtrate"
            // we initialize the next field of each node, no constraints yet
            if(isFirstStampeFiltrate) {
                isFirstStampeFiltrate = 0;
                predecessor = treeMinimum(root);
                globalPointerToFirstAdmissibleNode = predecessor;
                initializeNextField(&root, &predecessor);
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
                root = treeInsert(&root, buffer);
                firstString = 0;
            } else
                treeInsert(&root, buffer);

        //list of words to add to BST, first part of the programme
        } else {

            if(firstString) {
                root = treeInsert(&root, buffer);
                firstString = 0;
            } else
                treeInsert(&root, buffer);
        }
    }

    /* ------------------------------------------------------ */

    //SECOND PHASE: every time i reads "+nuova_partita"
    endGame = 0;

    while (!endGame) {

        addString = 0;
        newMatch = 0;//if new match is read => new match will be 1

        rString = malloc(sizeof(char) * (k + 1)); //reference string
        strncpy(rString, buffer, k); //I already have it in the buffer since the last comparison
        n = strtol(fgets(bufferK, DIM_BUFFER_K, stdin), NULL, BASE_10);

        while ((fgets( buffer, MAX_DIM_BUFFER, stdin)) != NULL && !newMatch) {

            //"+nuova_partita" => newMatch = 1
            if (buffer[0] == '+' && buffer[1] == 'n') {
                newMatch = 1;
                initializeConstraintsStruct();

            //"+inserisci_inizio" begins the insert of new admissible words
            } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[16] == 'o'){
                addString = 1;

            //"+inserisci_fine" end of insert
            } else if (buffer[0] == '+' && buffer[1] == 'i' && buffer[14] == 'e'){
                addString = 0;

            //"+stampa_filtrate", print out all the compatible words, from next
            } else if (buffer[0] == '+' && buffer[1] == 's') {

                //if it is the first time that we read "stampe filtrate"
                // we initialize the next field of each node
                //But we have to consider the constraints learned so far (fin'ora)
                if(isFirstStampeFiltrate) {
                    isFirstStampeFiltrate = 0;
                    predecessor = treeMinimum(root);
                    globalPointerToFirstAdmissibleNode = predecessor;
                    initializeNextField(&root, &predecessor);
                }
                stampeFiltrate();
                //printf("\n\n");

            //if we are between "inserisci_inizio" and "inserisci_fine"
            } else if (addString){

                treeInsert(&root, buffer);

            //comparison between p and r
            } else {

                pString = malloc(sizeof(char) * (k + 1));
                strncpy(pString, buffer, k);

                if(treeSearch(&root, &pString) != NULL)
                    if(!strcmp(rString, pString))
                        printf("ok\n");
                    else
                        match(rString, pString, noCharBroadcast, yesCharIdx, noCharIdx, minOccChar, perfectOccChar);
               else
                   printf("not_exists\n");

            }
        }
        if(!newMatch)
            endGame = 1;

    }



/**
 * TODO:
 * 1) creare su un altro progetto le strutture per i vincoli in 
 * 3) aggiornare linked list ogni comparison
 * 3) aggiornare linked list ogni comparison
 * 2) mettere controllo su n max confronti
 * 5) fare update lista quando aggiungiamo parole e aggiornare linked list verificando se le nuove parole soddisfano vincoli
 * 6) ottimizzare
 * 7) valutare se allocare memoria a blocchi
 */



















    //inorderTreeWalk(&root);


    predecessor = treeMinimum(root);

    //initialize list
    //initialization of global pointer must stay here because the value of predecessor will change
    globalPointerToFirstAdmissibleNode = predecessor;
    initializeNextField(&root, &predecessor);

    //isFindGlobalPointer = 0;
    //predecessor = treeMinimum(root);
    //updateNextField(&root, &predecessor);ù
    //printf("numbers of nodes = %d", numberOfAddedNodes);
    //inorderTreeWalk(&root);

    return 0;
}


