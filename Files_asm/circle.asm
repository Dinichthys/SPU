push 0
pop bx
push 0
pop cx

for_1:

    push cx
    push 10
    sub

    push cx
    push 10
    sub

    mul

    push bx
    push 10
    sub

    push bx
    push 10
    sub

    mul

    add

    push 50

;    sub
;
;    pop ex
;
;    push ex
;    push ex
;    mul
;
;    push 100

    jb not_circle:

        push 255
        jmp end_change_color:

    not_circle:
        push 0

    end_change_color:
    push bx
    push 20
    mul

    push cx

    add

    pop dx

    pop [dx]

    push cx
    push 1
    add

    pop cx

    push cx

    push 20

    ja for_1:

        push bx
        push 1
        add

        pop bx

        push 0

        pop cx

        push bx

        push 20

        ja for_1:

draw

hlt

