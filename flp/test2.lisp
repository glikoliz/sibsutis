(defvar L '(1 2 3 -1 2 3 4 4 0 1))
(defun sum-triples (lst)
  (if (or (null lst) (null (cdr lst)) (null (cddr lst)))
      nil
      (cons (+ (car lst) (cadr lst) (caddr lst))
            (sum-triples (cdr lst)))))

(print (sum-triples L))
