// Test.c

int globalInt = 0;
short globalShort = 1;
const char* globalString = "This is a global string";

int Foo(int a)
{
    int b;

    if (a > 33)
    {
        b = 66;
    }
    else
    {
        b = 77;
    }

    return b;
}

int Bar(int a, int b)
{
    return a + b;
}

int Bead(int a, int b)
{
    return a * b;
}
