(defvar my-list '(1 -2 -3 4 5 6 -7 8 9))
(defvar result nil)

(defun myfunc (lst)
    (if (null (cdr lst))
        (if (null (car lst))
            nil ;чет
            (cons (car lst) nil) ;нечет
        )
        (cons (+ (car lst) (car (last lst))) (myfunc (butlast (cdr lst))))
    )
)

(setq result (myfunc my-list))
(print result)