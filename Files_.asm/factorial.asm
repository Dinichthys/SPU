in
pop bx
call factorial:

push cx
out

hlt


factorial:

    push bx
    push 1

    jb complete:

        push 1
        pop cx
        ret

    complete:

    push 1
    push bx
    sub

    pop bx

    call factorial:

    push bx+1
    pop bx

    push cx
    push bx
    mul

    pop cx

ret
