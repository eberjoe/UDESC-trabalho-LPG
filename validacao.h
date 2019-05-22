#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int ValidaFloat(float *x) { // Parâmetro por referência permite a manipulação do valor
    char buffer[100], *endptr;
    fgets(buffer, sizeof(buffer), stdin);
    *x = strtof(buffer, &endptr);
    if ((*endptr == '\0') || (isspace(*endptr) != 0)) {
        return 1;
    }
    return 0;
}