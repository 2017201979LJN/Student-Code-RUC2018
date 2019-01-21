#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zlib.h"
#include "splay.h"
#include "compression.h"

// Read data to build original tree
void read_data(FILE *fp){
    int n = 0;
    int i = 0;
    double d = 0.0;
    char c;
    fscanf(fp, "%d", &n);	
	for(i = 1; i <= n; i++){
		fscanf(fp, "%lf %c", &d, &c);
		insert_node(d, c);
    }
    return ;
}


void build_tree_from_raw_file(){
    FILE* fin = fopen("data.in", "r");
    read_data(fin);
    fclose(fin);
    return ;
}

void dfs_tree_save(char *buffer, struct tree_node *p){
    if (p == NULL) {
        sprintf(buffer + strlen(buffer), "0;");
        return ;
    }
    sprintf(buffer + strlen(buffer), "1;%.3lf;%c;", p->val, p->c);
    dfs_tree_save(buffer, p->ch[0]);
    dfs_tree_save(buffer, p->ch[1]);
    return ;
}

int save_tree_to_bin_file()
{
    // Create buffers
    char *raw_buf = malloc(sizeof(char) * FILE_BUFFER),  // Temporary write
         *cmp_buf = malloc(sizeof(char) * FILE_BUFFER);  // Compress
    strcpy(raw_buf, "");
    strcpy(cmp_buf, "");
    // Dump tree to source buffer
    dfs_tree_save(raw_buf, root);
    // Compress buffer
    unsigned long int dest_len = FILE_BUFFER,
                      src_len = strlen(raw_buf);
    int ret_res = compress2(cmp_buf, &dest_len, (const unsigned char*)raw_buf,
                            src_len, Z_BEST_COMPRESSION);
    if (ret_res != Z_OK) {
        printf("Error while compressing data.\n");
        free(raw_buf);
        free(cmp_buf);
        return 1;
    }
    // Write buffer to file
    FILE* fout = fopen("compressed.sv", "wb");
    printf("Finish: %d, %d\nCompression Ratio: %.2lf%%\n",
            (int)src_len, (int)dest_len,
            100.0 * (double)dest_len / (double)src_len);
    fwrite(cmp_buf, sizeof(char), dest_len, fout);
    fclose(fout);
    // Finalize
    free(raw_buf);
    free(cmp_buf);
    return 0;
}



struct tree_node* restore_node(char* buffer, int *pointer, struct tree_node *parent){
    int is_valid_node = 0;
    int scan_len = 0;
    sscanf(buffer + *pointer, "%d;%n", &is_valid_node, &scan_len);
    *pointer += scan_len;
    if (!is_valid_node)
        return NULL;
    // Is not null node, read node data
    struct tree_node *p = malloc(sizeof(struct tree_node));
    sscanf(buffer + *pointer, "%lf;%c;%n", &p->val, &p->c, &scan_len);
    *pointer += scan_len;
    p->parent = parent;
    p->ch[0] = restore_node(buffer, pointer, p);
    p->ch[1] = restore_node(buffer, pointer, p);
    p->size = 1 + (p->ch[0] ? p->ch[0]->size : 0) +
              (p->ch[1] ? p->ch[1]->size : 0);
    return p;
}

int load_tree_from_bin_file(){
    // Create buffers
    char *cmp_buf = malloc(sizeof(char) * FILE_BUFFER),  // Compressed data
         *raw_buf = malloc(sizeof(char) * FILE_BUFFER);  // Actual data read
    strcpy(cmp_buf, "");
    strcpy(raw_buf, "");
    // Read file into buffer
    unsigned long int src_len = FILE_BUFFER,
                      dest_len = FILE_BUFFER;
    FILE* fin = fopen("compressed.sv", "rb");
    fread(cmp_buf, sizeof(char), FILE_BUFFER, fin);
    fclose(fin);
    // Decompressing file
	int ret_res = uncompress(raw_buf, &dest_len, (const unsigned char*)cmp_buf,src_len);
    if (ret_res != Z_OK) {
        printf("Error while compressing data.\n");
        free(raw_buf);
        free(cmp_buf);
        return 1;
    }
    // Construct tree
    int pointer = 0;
    root = restore_node(raw_buf, &pointer, NULL);
    // Finalize
    free(raw_buf);
    free(cmp_buf);
    return 0;
}


