(defvar my-set1 '(2 5 3))
(defvar my-set2 '(3 2 5))
(defvar result nil)

(defun myfunc (set1 set2)
 (cond
  ((null set1) (null set2))
  ((member (car set1) set2) 
    (myfunc (cdr set1) (remove (car set1) set2))
  )
  )
)

(setq result (myfunc my-set1 my-set2))

(if result
    (format t "равны")
    (format t "не равны"))