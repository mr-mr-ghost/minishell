#include <stdio.h>
#include <stdlib.h>

// Define the different types of AST nodes
typedef enum {
	NODE_TYPE_OPERATOR,
	NODE_TYPE_IDENTIFIER
} NodeType;

// Structure for an AST node
typedef struct ASTNode {
	NodeType type;
	union {
		char operator;		// For operators like +, *, etc.
		char identifier;	// For variables like a, b, c, etc.
	} value;
	struct ASTNode* left;
	struct ASTNode* right;
} ASTNode;

// Function to create an operator node
ASTNode* createOperatorNode(char operator, ASTNode* left, ASTNode* right) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = NODE_TYPE_OPERATOR;
	node->value.operator = operator;
	node->left = left;
	node->right = right;
	return node;
}

// Function to create an identifier node
ASTNode* createIdentifierNode(char identifier) {
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = NODE_TYPE_IDENTIFIER;
	node->value.identifier = identifier;
	node->left = NULL;
	node->right = NULL;
	return node;
}

// Function to print the AST in a readable format
void printAST(ASTNode* node) {
	if (node == NULL) {
		return;
	}

	// Print the left child
	if (node->left != NULL) {
		printf("(");
		printAST(node->left);
	}

	// Print the current node
	if (node->type == NODE_TYPE_OPERATOR) {
		printf(" %c ", node->value.operator);
	} else if (node->type == NODE_TYPE_IDENTIFIER) {
		printf("%c", node->value.identifier);
	}

	// Print the right child
	if (node->right != NULL) {
		printAST(node->right);
		printf(")");
	}
}

int	main(void)
{
	// Example: Constructing the AST for the expression "a + b * c"

	// Leaf nodes for identifiers
	ASTNode* a = createIdentifierNode('a');
	ASTNode* b = createIdentifierNode('b');
	ASTNode* c = createIdentifierNode('c');

	// Sub-tree for "b * c"
	ASTNode* multiply = createOperatorNode('*', b, c);

	// Root node for the entire expression "a + (b * c)"
	ASTNode* add = createOperatorNode('+', a, multiply);

	// Print the AST
	printf("AST for the expression 'a + b * c':\n");
	printAST(add);
	printf("\n");

	// Free the allocated memory
	free(a);
	free(b);
	free(c);
	free(multiply);
	free(add);
	return (0);
}
