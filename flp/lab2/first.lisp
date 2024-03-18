(defvar my-list '(-1 d 6 -3 a 0))
(defvar mysymb '*)

(defun myfunc (symb lst)
  (if (null lst)
      nil
      (cons (if (and (numberp (car lst)) (>= (car lst) 0))
                (list (car lst) symb)
                (car lst))
            (myfunc symb (cdr lst))))
)
(setq my-list (myfunc mysymb my-list))
(print my-list)