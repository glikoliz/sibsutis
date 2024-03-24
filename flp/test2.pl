старт([], []).
старт([_], []).
старт([_,_], []).
старт([X,Y,Z|Rest], [Sum|Result]) :-
    Sum is X + Y + Z,
    старт([Y,Z|Rest], Result),
    !.