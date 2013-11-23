#include "huffmantree.h"
huffmantree* huffmantree_init();
huffmantree* huffmantree_init(char* string);
char* huffmantree_tostring();
int huffmantree_isempty(huffmantree* tree);
int huffmantree_isleaf(huffmantree* tree);
void huffmantree_free(huffmantree* tree);