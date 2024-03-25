(defvar mylist '(5 1 2 1 3 5 5 7))

(defun delete-dupl (lst)
  (cond ((null lst) nil)
        ((member (car lst) (cdr lst)) 
         (delete-dupl (remove (car lst) (cdr lst))))
        (t (cons (car lst) (delete-dupl (cdr lst))))))

(print (delete-dupl mylist))
