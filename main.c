#include <stdio.h>
#include <stdlib.h>

/***********STRUCTS***********/
typedef struct AVL_TREE_NODE_s *AVL_TREE_NODE;
typedef struct AVL_TREE_NODE_s
{
    int data;
    int height;
    AVL_TREE_NODE left;
    AVL_TREE_NODE right;
} AVL_TREE_NODE_t[1];

/***********FUNCTIONS***********/
AVL_TREE_NODE create(int data)
{ // Creates a Avl tree node
    AVL_TREE_NODE new_node = (AVL_TREE_NODE)malloc(sizeof(AVL_TREE_NODE));

    if (new_node != NULL)
    {
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    else
    { // If memory don't allocated to the new node
        printf("\nMemory can't be allocated\n");
        return NULL;
    }
    return new_node;
}

int height(AVL_TREE_NODE root)
{
    int lh, rh;
    if (root == NULL)
    { // if tree is empty then height is 0
        return 0;
    }

    if (root->left == NULL)
    { // height adjustment for left
        lh = 0;
    }
    else
    {
        lh = 1 + root->left->height;
    }

    if (root->right == NULL)
    { // height adjustment for right
        rh = 0;
    }
    else
    {
        rh = 1 + root->right->height;
    }

    if (lh > rh)
    { // return the biggest height (left or right)
        return (lh);
    }
    else
    {
        return (rh);
    }
}

int search(AVL_TREE_NODE root, int key)
{                     // Search algorithm (based on binary search) to find if a specific node exists or not
    if (root == NULL) // function looks so clear without brackets so i didn't use them unlike other functions
        return 0;

    if (root->data == key)
        return 1;

    if (key > root->data)
        search(root->right, key);

    else
        search(root->left, key);
}

AVL_TREE_NODE rotate_left(AVL_TREE_NODE root)
{ // Left rotation function
    AVL_TREE_NODE right_child = root->right;
    root->right = right_child->left;
    right_child->left = root;

    // Update the heights of the nodes
    root->height = height(root);
    right_child->height = height(right_child);

    // Return the new node after rotation
    return right_child;
}

AVL_TREE_NODE rotate_right(AVL_TREE_NODE root)
{ // Right rotation function
    AVL_TREE_NODE left_child = root->left;
    root->left = left_child->right;
    left_child->right = root;

    // Update the heights of the nodes
    root->height = height(root);
    left_child->height = height(left_child);

    // Return the new node after rotation
    return left_child;
}

int balance_factor(AVL_TREE_NODE root)
{ // Calculates the balance factor of a node
    int lh, rh;
    if (root == NULL)
    {
        return 0;
    }
    if (root->left == NULL)
    { // height adjustment for left
        lh = 0;
    }
    else
    {
        lh = 1 + root->left->height;
    }
    if (root->right == NULL)
    { // height adjustment for right
        rh = 0;
    }
    else
    {
        rh = 1 + root->right->height;
    }
    // return the difference (balance factor)
    return lh - rh;
}

AVL_TREE_NODE insert(AVL_TREE_NODE root, int data)
{

    if (root == NULL)
    { // If tree has no nodes then we need to create a root
        AVL_TREE_NODE new_node = create(data);
        if (new_node == NULL)
        {
            return NULL;
        }
        root = new_node;
    }
    else if (data > root->data)
    { // Insert the new node to the right
        root->right = insert(root->right, data);

        if (balance_factor(root) < -1)
        { // If tree is unbalanced (bf < -1) we need to rotate it
            if (data > root->right->data)
            {
                // Right subtree is too deep on the right side of the current node
                // We need Left rotation
                root = rotate_left(root);
            }
            else
            {
                // Right subtree is too deep on the left side of the current node
                // We need RL rotation
                root->right = rotate_right(root->right);
                root = rotate_left(root);
            }
        }
    }
    else
    { // Insert the new node to the left
        root->left = insert(root->left, data);

        if (balance_factor(root) > 1)
        { // If tree is unbalanced (bf > 1) we need to rotate it
            if (data < root->left->data)
            {
                // Left subtree is too deep on the left side of the current node
                // We need Right rotation
                root = rotate_right(root);
            }
            else
            {
                // Left subtree is too deep on the right side of the current node
                // We need LR rotation
                root->left = rotate_left(root->left);
                root = rotate_right(root);
            }
        }
    }
    root->height = height(root); // Update the height of the inserted node
    return root;
}

AVL_TREE_NODE delete(AVL_TREE_NODE root, int x)
{
    AVL_TREE_NODE temp = NULL;

    /*
     * I solved the empty tree problem at the menu because
     * there is no need to go into function unnecessarily
     * but i wanted to write the condition anyway now it's mostly symbolic
     */
    if (root == NULL)
    {
        return NULL;
    }

    if (x > root->data)
    { // If the deleted node's data is bigger than current one then we need to go to right
        root->right = delete (root->right, x);

        if (balance_factor(root) > 1)
        { // If the tree is unbalanced (bf > 1) we need to rotate it

            if (balance_factor(root->left) >= 0)
            {
                // Left subtree is too deep on the left side of the current node
                // We need Right rotation
                root = rotate_right(root);
            }
            else
            {
                // Left subtree is too deep on the right side of the current node
                // We need LR rotation
                root->left = rotate_left(root->left);
                root = rotate_right(root);
            }
        }
    } // End of if condition

    else if (x < root->data)
    { // If the deleted node's data is smaller than current one then we need to go to left
        root->left = delete (root->left, x);
        if (balance_factor(root) < -1)
        { // If the tree is unbalanced (bf < -1) we need to rotate it

            if (balance_factor(root->right) <= 0)
            {
                // Right subtree is too deep on the right side of the current node
                // We need Left rotation
                root = rotate_left(root);
            }
            else
            {
                // Right subtree is too deep on the left side of the current node
                // We need RL rotation
                root->right = rotate_right(root->right);
                root = rotate_left(root);
            }
        }
    } // End of else if condition

    else
    { // If we found the node that we need to delete then let's do it

        if (root->right != NULL)
        { // Check for if there is a right child if there is find inorder successor
            temp = root->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            // Replace data of current with inorder successor
            root->data = temp->data;

            // Delete succesor from right subtree recursively
            root->right = delete (root->right, temp->data);

            if (balance_factor(root) > 1)
            { // If tree is unbalanced we need (bf > 1)
                if (balance_factor(root->left) >= 0)
                {
                    // Left subtree is too deep on the left side of the current node
                    // We need Right rotation
                    root = rotate_right(root);
                }
                else
                {
                    // Left subtree is too deep on the right side of the current node
                    // We need LR rotation
                    root->left = rotate_left(root->left);
                    root = rotate_right(root);
                }
            }
        }
        else
        { // If there is no right child then delete the current by returning it's left child
            return (root->left);
        }
    } // End of outer else condition
    root->height = height(root);
    return (root);
}

void preorder(AVL_TREE_NODE root)
{ // Preorder Traversal (node, left, right)
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}
void inorder(AVL_TREE_NODE root)
{ // Inorder Traversal (left, node, right)
    if (root == NULL)
    {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

AVL_TREE_NODE freeAvl(AVL_TREE_NODE root)
{ // Function to clear the memory before program terminated (postorder to traverse all nodes before the root)
    if (root != NULL)
    {
        freeAvl(root->left);
        freeAvl(root->right);
        free(root);
    }
    return NULL;
}

int IsInt()
{ // Function to check if the input is valid (integer) or not
    double input;
    int validInput;
    do
    {
        validInput = 0;
        while (scanf("%lf", &input) != 1 || (int)input != input || getchar() != '\n')
        {
            printf("Invalid input!!! Please enter an integer:\n");
            while (getchar() != '\n')
                ; // Clear input from memory
        }
        validInput = 1;
    } while (!validInput);

    return (int)input;
}

/*************MAIN FUNCTION*************/
int main()
{
    int choice;
    AVL_TREE_NODE root;

    while (1)
    { // MAIN MENU LOOP
        printf("---------------\nAVL Tree - Menu\n---------------"
               "\n1. Insert"
               "\n2. Delete"
               "\n3. Update"
               "\n4. Traverse the tree in Preorder"
               "\n5. Traverse the tree in Inorder"
               "\n6. Exit\n");

        printf("\nPlease enter your choice\n");
        choice = IsInt();

        if (choice == 6)
        { // Exit Code
            break;
        }
        switch (choice)
        {
        case 1: // Submenu for Insert
            while (1)
            {
                int value;
                char ans;

                printf("Enter the value for the new node\n");
                value = IsInt();
                if (search(root, value) == 1)
                { // Check for duplicate
                    printf("You cannot insert same value more than once!\n");
                }
                else
                {
                    root = insert(root, value);
                    printf("Insert Completed\n");
                }

                printf("Do you want to continue? (Y/N)\n");
                scanf(" %c", &ans);

                if (ans == 'N' || ans == 'n')
                {
                    printf("Returning to Main Menu...\n");
                    fflush(stdin); // Clear input stream to prevent unexpected results
                    break;
                }
                else if (ans == 'Y' || ans == 'y')
                {
                    fflush(stdin);
                    continue;
                }
                else
                {
                    fflush(stdin);
                    printf("Invalid Answer!!! Returning to Main Menu...\n");
                    break;
                }
            }
            break;

        case 2: // Submenu for Delete
            while (1)
            {
                int value;
                char ans;
                printf("Enter the node value to be deleted\n");
                value = IsInt();

                if (root == NULL || search(root, value) != 1)
                {
                    printf("Node does not exists!\n");
                }
                else
                {
                    root = delete (root, value);
                    printf("Delete Completed\n");
                }

                printf("Do you want to continue? (Y/N)\n");
                scanf(" %c", &ans);

                if (ans == 'N' || ans == 'n')
                {
                    printf("Returning to Main Menu...\n");
                    fflush(stdin); // Clear input stream to prevent unexpected results
                    break;
                }
                else if (ans == 'Y' || ans == 'y')
                {
                    fflush(stdin);
                    continue;
                }
                else
                {
                    fflush(stdin);
                    printf("Invalid Answer!!! Returning to Main Menu...\n");
                    break;
                }
            }
            break;

        case 3: // Submenu for Update
            while (1)
            {
                int old_value, new_value;
                char ans;

                printf("Enter the node value to be updated\n");
                old_value = IsInt();

                printf("What will be the new value?\n");
                new_value = IsInt();

                if (root == NULL || search(root, old_value) != 1)
                {
                    printf("Node does not exists!\n");
                }
                else
                {
                    root = delete (root, old_value);
                    root = insert(root, new_value);
                    printf("Update Completed\n");
                }

                printf("Do you want to continue? (Y/N)\n ");
                scanf(" %c", &ans);

                if (ans == 'N' || ans == 'n')
                {
                    printf("Returning to Main Menu...\n");
                    fflush(stdin); // Clear input stream to prevent unexpected results
                    break;
                }
                else if (ans == 'Y' || ans == 'y')
                {
                    fflush(stdin);
                    continue;
                }
                else
                {
                    fflush(stdin);
                    printf("Invalid answer!!! Returning to Main Menu...\n");
                    break;
                }
            }
            break;

        case 4: // Submenu for Preorder
            if (root != NULL)
            {
                printf("Preorder Traversal of the AVL Tree:\n");
                preorder(root);
                printf("\n\n");
                while (1)
                {
                    char wait;
                    printf("Press any key to go back to the main menu\n");
                    scanf("%c", &wait);
                    fflush(stdin);
                    break;
                }
                break;
            }
            else
            {
                printf("Avl Tree is empty... Returning to main menu\n");
                break;
            }

        case 5: // Submenu for Inorder
            if (root != NULL)
            {
                printf("Inorder Traversal of the AVL Tree:\n");
                inorder(root);
                printf("\n\n");
                while (1)
                {
                    char wait;
                    printf("Press any key to go back to the main menu\n");
                    scanf("%c", &wait);
                    fflush(stdin);
                    break;
                }
                break;
            }
            else
            {
                printf("Avl Tree is empty... Returning to main menu\n");
                break;
            }

        default: // Warning message if choice > 6 and choice < 0
            printf("Invalid Input!!! Please enter an integer between 1 and 6.\n");
            break;
        } // End of switch
    } // End of main menu loop

    root = freeAvl(root);
    printf("\nMemory Cleared, Program is Terminating...\n");
    return 0;
}
