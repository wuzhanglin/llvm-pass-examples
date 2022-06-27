// SimpleLoopTest.c

int array[1024];

void SimpleLoopTest()
{
    for (int i = 0; i < 100; i++)
    {
        array[i] = i;
    }

    for (int i = 101; i < 200; i++)
    {
        array[i] = array[i] * i;
    }
}
