посчитать_нули_и_единицы([], 0).
посчитать_нули_и_единицы([1|T], Diff) :-
    посчитать_нули_и_единицы(T, SubDiff),
    Diff is SubDiff + 1, !.
посчитать_нули_и_единицы([0|T], Diff) :-
    посчитать_нули_и_единицы(T, SubDiff),
    Diff is SubDiff - 1, !.
посчитать_нули_и_единицы([_|T], Diff) :-
    посчитать_нули_и_единицы(T, Diff).

старт(L, Res) :-
    посчитать_нули_и_единицы(L, Diff),
    Res is abs(Diff).