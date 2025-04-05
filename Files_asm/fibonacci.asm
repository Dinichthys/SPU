in
pop dx

call fibonacci:

push cx
out

hlt

fibonacci:

    push dx
    push 3

    jbe complete:
    push 1
    pop bx
    push 1
    pop cx

    ret

    complete:

    push 1
    push dx
    sub

    pop dx

    call fibonacci:

    push bx
    push cx
    add
    pop fx

    push cx
    pop bx

    push fx
    pop cx

ret


