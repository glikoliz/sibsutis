(defvar my-cons nil)
(setq my-cons (cons 1 (cons (cons 2 (cons 3 nil)) nil)))
(print my-cons)

(defvar my-list nil)
(setq my-list (list 1 '(2 3)))
(print my-list)
(terpri)