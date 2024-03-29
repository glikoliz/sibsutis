обратить_слова(ИсходныйФайл, РезультирующийФайл) :-
    open(ИсходныйФайл, read, ВходнойПоток),
    open(РезультирующийФайл, write, ВыходнойПоток),
    обработать_строки(ВходнойПоток, ВыходнойПоток),
    close(ВходнойПоток),
    close(ВыходнойПоток).

обработать_строки(ВходнойПоток, ВыходнойПоток) :-
    read_line_to_string(ВходнойПоток, Строка),
    (Строка \== end_of_file ->
        обратить_и_записать(Строка, ВыходнойПоток),
        обработать_строки(ВходнойПоток, ВыходнойПоток)
    ; true
    ).

обратить_и_записать(Строка, ВыходнойПоток) :-
    разделить_и_обратить(Строка, ОбратнаяСтрока),
    write(ВыходнойПоток, ОбратнаяСтрока),
    nl(ВыходнойПоток).

разделить_и_обратить(Строка, ОбратнаяСтрока) :-
    перевернуть_строку(Строка, Перевернутая_строка),
    split_string(Перевернутая_строка, " ", "", Перевернутые_слова),
    reverse(Перевернутые_слова, Результат),
    atomic_list_concat(Результат, " ", ОбратнаяСтрока).

перевернуть_строку(Str,Y) :- string_codes(Str, X), reverse(X, Xr), string_codes(Y,Xr).