in
pop bx
call factorial:
hlt


factorial:

    push 1
    pop cx

    for_factorial:

        push cx
        push cx
        mul
        out

        push cx+1
        pop cx

        push cx
        push bx

    jae for_factorial:

ret
