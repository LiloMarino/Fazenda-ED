#include "radialtree.h"
#include "geo.h"
#include <string.h>
#include <stdio.h>

int main()
{
    RadialTree t = newRadialTree(8, 0.1);

    freeRadialTree(&t, true);
}