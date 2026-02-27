#include "nyObj.h"

int main() {
    const char *prog = \
    "a = 1+3; b = a; print(b);";

    NyRuntime interpretator;
    runtime_init(&interpretator);

    runtime_exec(&interpretator, prog);

    runtime_destroy(&interpretator);

    return 0;
}
