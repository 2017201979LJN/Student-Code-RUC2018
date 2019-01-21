#include <stdio.h>
#include <string.h>

#include "bitio.h"
#include "zip.h"

void print_usage(char* path)
{
    fprintf(stderr, "Usage: %s { zip | unzip } filename1 [filename2]\n", path);
}

int main(int argc, char** argv)
{

    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "zip") == 0) {
        printf("%d\n", zipfile(argv[2], argv[3]));
    } else if (strcmp(argv[1], "unzip") == 0) {
        printf("%d\n", unzipfile(argv[2]));
    } else {
        print_usage(argv[0]);
        return 1;
    }

    // BFILE* fout = bfopen("QAQ", "wb");

    // for (unsigned char s = 0, ch; s < 64; s++) {
    //     ch = s << 3;
    //     bwrite(fout, &ch, 5);
    // }

    // bfclose(fout);

    // fout = bfopen("QAQ", "rb");
    // unsigned char bufc = 0;

    // for (int i = 0; i < 64; i++) {
    //     bread(fout, &bufc, 1);
    //     printf("%d", (int)(bufc >> 7));
    //     bread(fout, &bufc, 1);
    //     printf("%d", (int)(bufc >> 7));
    //     bread(fout, &bufc, 1);
    //     printf("%d", (int)(bufc >> 7));
    //     bread(fout, &bufc, 1);
    //     printf("%d", (int)(bufc >> 7));
    //     bread(fout, &bufc, 1);
    //     printf("%d", (int)(bufc >> 7));
    //     puts("");
    // }

    // bfclose(fout);

    unsigned char ch;

    return 0;
}
