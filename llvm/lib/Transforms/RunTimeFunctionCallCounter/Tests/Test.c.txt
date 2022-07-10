// Test.c

void Foo()
{
}

void Bar()
{
    Foo();
}

void Fez()
{
    Bar();
}

int main()
{
    Foo();
    Bar();
    Fez();

    for (int i = 0; i < 5; i++)
    {
        Foo();
    }

    return 0;
}
