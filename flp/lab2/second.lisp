(defvar my-list1 '(1 2 3 4 5 6 7 8))
(defvar my-list2 '(a b c d f))
(defvar result nil)

(defun myfunc (lst1 lst2)
  (if (or (null lst1) (null lst2))
      (append lst1 lst2)
      (cons (car lst1) (cons (car lst2) (myfunc(cdr lst1) (cdr lst2))))))

(setq result (myfunc my-list1 my-list2))
(print result)