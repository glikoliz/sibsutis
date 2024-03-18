(defvar my-list '(((1 (2 *)) 3) 4))
(defvar result nil)

(defun myfunc (lst)
  (cond
    ((atom lst) nil)
    ((equal (car lst) '*)
     '*)
    (t
     (or (myfunc (car lst)) (myfunc (cdr lst))))
    )
)

(setq result (myfunc my-list))
(print result)
