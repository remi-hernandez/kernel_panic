int
atoi(const char * str)
{
    int r = 0;
    int s = 1;
    int l = 0;
    int i;
    int j;

    for (i = 0; str[i] == '-' || str[i] == '+'; ++i)
        if (str[i] == '-')
            s *= -1;
    for (j = i; str[j] >= '0' && str[j] <= '9'; ++j)
        l += 1;
    for (; l; --l, ++i)
        r = r * 10 + (str[i] - '0');

    return (r * s);
}
