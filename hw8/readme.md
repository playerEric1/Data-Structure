A rope implementation using C++.

A rope is a binary tree where each leaf (end node) holds a string and a length (weight), and each node further up the tree holds the sum of the lengths of all the leaves in its left subtree. A node with two children thus divides the whole string into two parts: the left subtree stores the first part of the string, the right subtree stores the second part of the string, and a node's weight is the length of the first part.
