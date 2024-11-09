in
pop bx
in
pop cx
in
pop dx

push bx
push 0

jne else_linear:

    call linear:

    hlt

else_linear:

    push cx
    push cx
    mul

    push -4
    push bx
    mul
    push dx
    mul

    add

    pop dx

    push dx
    push 0

    ja no_roots_end:

        push dx

        sqrt
        pop dx

        push 2
        push bx
        mul

        push dx

        push -1
        push cx
        mul

        sub

        div

            pop [0]

        push 2
        push bx
        mul

        push dx

        push -1
        push cx
        mul

        add

        div

            pop [1]

        push [1]
        push [0]

        je one_root_bx_n_0:

            push 2
            out

            push [0]
            out

            push [1]
            out
                hlt

        one_root_bx_n_0:

            push 1
            out

            push [0]
            out
                hlt

    no_roots_end:

        push 0
        out
            hlt

linear:

    push cx
    push 0

    jne else_linear_cx_0:

        push dx
        push 0

        jne else_linear_dx_0:

            push 777.777
            out

            jmp end_else_linear_dx_0:

        else_linear_dx_0:

            push 426.426
            out

        end_else_linear_dx_0:

            ret

    else_linear_cx_0:

        push 1
        out

        push cx   push dx   div

        push -1

        mul

        out
        ret
