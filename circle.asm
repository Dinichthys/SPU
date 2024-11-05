push 0
pop bx
push 0
pop cx

for_1:

    push cx  push 49  sub

    push cx  push 49  sub

    mul

    push bx

    push 49  sub

    push 2

    mul

    push bx

    push 49  sub

    push 2

    mul

    mul

    add

    push 2500

    sub

    pop ex

    push ex  push ex  mul

    push 1500

    jb not_circle:

        push 35

        push bx  push 100  mul  push cx  add

        pop dx

        pop [dx]

        jmp end_push_symbol:

    not_circle:

        push 32

        push bx  push 100  mul  push cx  add

        pop dx

        pop [dx]

    end_push_symbol:

    push cx  push 1  add

    pop cx

    push cx

    push 100

    ja for_1:

        push bx  push 1  add

        pop bx

        push 0

        pop cx

        push bx

        push 100

        ja for_1:

draw

hlt

