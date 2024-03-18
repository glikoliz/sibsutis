(defvar my-list '(1 2 (2 3)))
(defvar result nil)
(print my-list)
(defun myfunc (lst)
    (cond 
    ((null lst) nil) ;если пустой
    ((null (cdr lst)) lst) ;если один элемент
    (t 
    (append (last lst) 
    (butlast (cdr lst)) 
    (list (car lst))))
    )
)
(setq result (list (myfunc my-list) (myfunc (list 1 2 3 4 5))))
(print result)
