#include <iostream>


#define compute_address(top, left, width) ((((top) * (width)) + (left)) * (3)) 
#define get_red(top, left, width) (((compute_address((top), (left), (width))) + (0)))
#define get_blue(top, left, width) (((compute_address((top), (left), (width))) + (1)))
#define get_green(top, left, width) (((compute_address((top), (left), (width))) + (2)))

int main (int argc, char *argv[])
{
    
}