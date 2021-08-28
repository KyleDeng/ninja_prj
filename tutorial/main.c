#include <stdio.h>
#include "hello.h"
#include "goodbye.h"

int main(int argc, char* argv[])
{
    const char* name = "Bob";
    hello(name);
    goodbye();

    printf("------main------\n");

    for(int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}
