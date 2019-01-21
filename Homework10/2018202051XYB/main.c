#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zlib.h"
#include "splay.h"
#include "compression.h"

int main(int argc, char** argv)
{
    initialize_empty_tree();
    build_tree_from_raw_file();
    // check_dfs(root);
    save_tree_to_bin_file();
    destruct_tree();

    load_tree_from_bin_file();
    // check_dfs(root);
    destruct_tree();
    return 0;
}

