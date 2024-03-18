фибоначчи :-
    repeat,
    writeln('Введите число: '),
    read(N),
    (N < 0 ->
        writeln('Программа завершена');
        fib(N, Result),
        format('~w~n', Result),
        fail
    ).

fib(0, 1).
fib(1, 1).
fib(N, Result) :-
    N > 1,
    N1 is N - 1,
    N2 is N - 2,
    fib(N1, F1),
    fib(N2, F2),
    Result is F1 + F2.
