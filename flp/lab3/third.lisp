(defvar my-list nil)
(defvar result nil)

(defun myfunc (predicate lst)
  (if (null lst)
      nil
      (if (funcall predicate (car lst))
          (cons '* (cons (car lst) (myfunc predicate (cdr lst))))
          (cons (car lst) (myfunc predicate (cdr lst)))
      )
  )
)

(setq my-list '(1 -2 3 -4 5))
(setq result (myfunc (lambda (x) (>= x 0)) my-list))
(print result)

(setq my-list '(1 2 3 4 5))
(setq result (myfunc #'evenp my-list))
(print result)
