(defvar my-set1 '(5 6 7 2 1))
(defvar my-set2 '(9 8 3 7 4 1))
(defvar result nil)

(defun myfunc (set1 set2)
(if (member (car set1) set2)
    (cons (car set1) (myfunc (cdr set1) (remove (car set1) set2)))
    (if (null set1)
    nil
    (myfunc (cdr set1) (remove (car set1) set2))
    )
)
)

(setq result (myfunc my-set1 my-set2))
(print result)
