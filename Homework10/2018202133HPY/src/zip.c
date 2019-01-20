#include "zip.h"

#include <math.h>
#include <stdio.h>

#include "bitio.h"
#include "tree.h"

void ziptree_raw(tree_node_t* pnode, BFILE* out)
{
    if (pnode == NULL)
        return;

    unsigned char ch = pnode->data.c;
    unsigned int uinteger = pnode->data.d;

    if (ch >= 'a')
        ch = 26 + ch - 'a';
    else
        ch = ch - 'A';

    ch = ch << (8 - ZIP_CHAR_BIT_WIDTH);

    uinteger = uinteger << ((sizeof(unsigned int) * 8) - ZIP_INT_BIT_WIDTH);

    BWRITE_PACK_32(out, &uinteger, ZIP_INT_BIT_WIDTH);
    bwrite(out, &ch, ZIP_CHAR_BIT_WIDTH);

    ziptree_raw(pnode->ls, out);
    ziptree_raw(pnode->rs, out);
}

void unziptree_raw(tree_node_t** ppnode, BFILE* in)
{
    int count;
    bread(in, &count, 32);

    unsigned char ch;
    unsigned int uinteger;
    tree_data_t data;

    for (int i = 0; i < count; i++) {
        BREAD_PACK_32(in, &uinteger, ZIP_INT_BIT_WIDTH);
        bread(in, &ch, ZIP_CHAR_BIT_WIDTH);

        uinteger = uinteger >> ((sizeof(unsigned int) * 8) - ZIP_INT_BIT_WIDTH);

        ch = ch >> (8 - ZIP_CHAR_BIT_WIDTH);
        if (ch >= 26)
            ch = 'a' + ch - 26;
        else
            ch = 'A' + ch;

        data.c = ch;
        data.d = uinteger;

        tree_insert_node(ppnode, data);
    }
}

int zipfile(const char* input, const char* output)
{
    FILE* fin = fopen(input, "r");
    BFILE* bfout = bfopen(output, "wb");

    int count;
    double tmp;
    char buf[16];
    tree_node_t* tree = NULL;
    tree_data_t data;

    fscanf(fin, "%d", &count);

    for (int i = 0; i < count; i++) {
        fscanf(fin, "%lf%s", &tmp, buf);

        data.c = buf[0];
        data.d = (int)round(tmp * ZIP_DOUBLE_SCALE) + ZIP_INT_OFFSET;

        tree_insert_node(&tree, data);
    }

    int hashcode = tree_size_hash(tree);

    bwrite(bfout, &(tree->siz), 32);

    ziptree_raw(tree, bfout);

    fclose(fin);
    bfclose(bfout);

    return hashcode;
}

int unzipfile(const char* input)
{
    BFILE* bfin = bfopen(input, "rb");

    tree_node_t* tree = NULL;

    unziptree_raw(&tree, bfin);

    int hashcode = tree_size_hash(tree);

    bfclose(bfin);

    return hashcode;
}
