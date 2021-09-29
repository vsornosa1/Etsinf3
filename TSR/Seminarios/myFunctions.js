function fibo(n) {
    return (n < 2) ? 1 : fibo(n - 2) + fibo(n - 1)
}

function fact(n) {
    var rval = 1;
    for (var i = 2; i <= n; i++)
        rval = rval * i;
    return rval;
}