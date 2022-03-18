/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b);
//
/////////////////////////////////////////////////

Node* CopyNode(Node* node);
int TreeSize(const Node* node) const;
int leafCount(const Node* node) const;
//void Wender(PNG)
Node* RenderHelper(Node* node, PNG& png) const;
void KILL(Node* node);
void mirror(Node* subRoot);

#endif