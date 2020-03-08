#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node // creates a structure called node containing an int, two chars and 2 structures
{
	int modNo;
	char *module;
	char *year;
	char *semester;
	struct node* left;
	struct node* right;
} node;

typedef void (*callback)(node*);
node* create_node(char  **nodeData);
node* insertNode(node* root, char **nodeData);

node* read(node* root) {
	static const char filename[] = "pg03data.txt";
   	FILE *file = fopen ( filename, "r" );
   	if ( file != NULL )
  	{
  		char line [ 128 ]; 
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      		{
			char *array[4]; int i=0; // makes char a 4 length array 
     			char* token = strtok(line, ",");	// splits line into tokens
    			while (token != NULL) {  // while a token has a value
				array[i] = malloc(8*strlen(token+1)) ; // creates an array containing an 8 byte string pointing to the length of a token +1
				strcpy(array[i],token); // returns given byte element = to the i'th value of the array + a token

        			token = strtok(NULL, ","); // makes a token equal to null
				i++;

    			}
			root = insertNode(root, array); // makes root equal to a node contining root and the above array

      		}
      		fclose ( file );
   	}
   	else
   	{
      		perror ( filename ); // of the file didnt open
   	}
	return root;
}

int compare(int left, int right) // compares the values of left and right nodes and returns 1 r 0 depending on which one is bigger
{
	if (left>right)
	return 0;
	if (left < right)
	return 0;
	return 1;
}

node* insertNode(node* root, char **nodeData) {

  if (root == NULL)
  {
     root = create_node(nodeData); // if therre is no value in root, creates a node
  }
  else
    {
        int is_left  = 0;
        int r        = 0;
        node* cursor = root; // sets value of a node cursor to root
        node* prev   = NULL;

        while(cursor != NULL) // if its not null, turn it into a node
        {
	    prev = cursor;
	    if (atoi(nodeData[0])<cursor->modNo)   // converts the node data array into a integer and compares it to the value stored in the modNo cursor pointer, and if its bigger
            { // assigns it as a left node
                is_left = 1;
                cursor = cursor->left;
            }
            else // if its smaller, it assigns it as a right node
            {
                is_left = 0;
                cursor = cursor->right;
            }

        }
        if(is_left) // if it is left, creates a node
            prev->left = create_node(nodeData);
        else
            prev->right = create_node(nodeData); // if not creates a right node
     }
  return root;
}

node* create_node(char **nodeData)
{ 
    node *new_node = (node*)malloc(sizeof(node)); // creates a new node with a string allocated with the size of the ndoe in bytes
    if(new_node == NULL)
    {
        fprintf (stderr, "Out of memory!!! (create_node)\n"); // if the node cannot be crated, prints the following and stops the program
        exit(1);
    }
	new_node->modNo=atoi(nodeData[0]);

	
	new_node->module = malloc(8*strlen(nodeData[1])) ;
	strcpy(new_node->module,nodeData[1]);

	new_node->year = malloc(8*strlen(nodeData[2])) ; // creates a node pointing to year, allocating it memory equal to 8 pointing to the length of the node data array

	strcpy(new_node->year,nodeData[2]); //Copies the C string pointed by new_node into the array pointed by year

	new_node->semester = malloc(8*strlen(nodeData[3])) ; // the same but for semester
	strcpy(new_node->semester, nodeData[3]); //Copies the C string pointed by new_node into the array pointed by semester

    new_node->left = NULL; // points  new_node to assign null to left
    new_node->right = NULL; // the same but for right

    return new_node;
}

/*
    recursively remove all nodes of the tree
*/
void dispose(node *myNode)
{
    if(myNode != NULL)
    {
        dispose(myNode->left);
        dispose(myNode->right);
        free(myNode);
    }
}

void writefile(node* myNode) {

char* add = malloc(128); // creates a string 128 bytes long to store data in
char num[10];
sprintf(num, "%d", myNode -> modNo);
strcpy(add, num); // copies num by the pointed string
strcat(add,",");	// appends the destination source by adding ,
strcat(add, myNode -> module); // appends the destination source by adding a pointer to the value module
strcat(add, ","); // appends the destination soruce by assing a ,
strcat(add,myNode -> year); // appends the destination source by adding a pointer to the value year
strcat(add, ","); // appends the destination source by adding a ,
strcat(add,myNode -> semester); // appends the destination source by adding a pointer to the value semester
FILE *out = fopen("FinalData.txt", "a");  //makes out equal to the file final data generated by the previous lines of code
fprintf(out, "%s", add);
fclose(out);
}

// creates a system to travel through the nodes in the tree, testing for left  and right nodes and traversing into them when needed
void Traverse(node *myNode)
{
  if(myNode->left!=NULL)
   Traverse(myNode->left);

   printf("%d", myNode->modNo);
	writefile(myNode);
   
   ("%s,", myNode->module);
   printf("%s,", myNode->year);
   printf("%s", myNode->semester);


  if(myNode->right!=NULL)
   Traverse(myNode->right);

}
// method to display data from the tree
void display_tree(node* myNode)
{
    if (myNode == NULL)
        return;
    // display each node data
    printf("%d",myNode->modNo);
    if(myNode->left != NULL)
        printf(" (L:%d)", myNode->left->modNo); // if the node is set to be a left node, prints the value stored inside it
    if(myNode->right != NULL)
        printf(" (R:%d)", myNode->right->modNo); // if the node is set to be a right node, prints the value stored in it
   printf("%s,", myNode->module); // also prints the module name
   printf("%s,", myNode->year); // the module year
   printf("%s", myNode->semester); // and the module semester
    printf("\n");

    display_tree(myNode->left); // displays the left node
    display_tree(myNode->right); // displays the right node
}

int main( int argc, const char* argv[] )
{
	node* root = NULL;
	root = read(root);

	Traverse(root);
	display_tree(root);


	dispose(root);
	return 0;
}