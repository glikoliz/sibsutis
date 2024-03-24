(defvar mylist '(a b c 1 4 d b f a a 1 c a 4))
(defvar myset '(1 a c))

(defun count-elements (list set)
  (if (null list)
    0
    (+ (if (member (car list) set) 0 1)
       (count-elements (cdr list) set))))


(print (count-elements mylist myset))

