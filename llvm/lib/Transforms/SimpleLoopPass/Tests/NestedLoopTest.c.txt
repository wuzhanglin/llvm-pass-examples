// NestedLoopTest.c

int a[1024][1024];
int b[1024][1024];

void NestedLoopTest()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j] = ((i - 3) * (i + 3)) % i;
        }
    }
}