#ifndef _COMPRESSION_H
#define _COMPRESSION_H

#define FILE_BUFFER 2097152
/* 1;-10000.000;c; 15 chars for present node
 * 0; 2 chars for empty node
 * 15*100000 maximum, buffer open to 2097152
 */

void read_data(FILE *fp);

void build_tree_from_raw_file();

void dfs_tree_save(char *buffer, struct tree_node *p);

int save_tree_to_bin_file();

struct tree_node* restore_node(char *buffer, int *pointer, struct tree_node *parent);

int load_tree_from_bin_file();

#endif
