in
in
in
pop dx
pop cx
pop bx

push bx
push 0

jne else_linear:

    call linear:

    hlt

else_linear:

    push cx   push cx   mul

    push -1   push 4    mul   push bx   mul   push dx  mul

    add

    pop dx

    push dx
    push 0

    ja no_roots_end:

        push dx

        sqrt
        pop dx

        push dx

        push -1   push cx   mul

        sub

        push 2    push bx   mul

        div

            pop [0]

        push dx

        push -1   push cx   mul

        add

        push 2    push bx   mul

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

no_root:
push 10
push 116
push 111
push 111
push 114
push 32
push 121
push 110
push 97
push 32
push 116
push 39
push 110
push 115
push 97
push 104
push 32
push 110
push 111
push 105
push 116
push 97
push 117
push 113
push 101
push 32
push 101
push 104
push 84
push 0
pop ex
for_no_root:
pop [ex]
push ex
push 1
add
pop ex
push ex
push 20
ja for_no_root:
draw
ret

all_root:
push 10
push 116
push 111
push 111
push 114
push 32
push 101
push 114
push 97
push 32
push 115
push 114
push 101
push 98
push 109
push 117
push 110
push 32
push 108
push 108
push 65
push 0
pop ex
for_all_root:
pop [ex]
push ex
push 1
add
pop ex
push ex
push 12
ja for_all_root:
draw
ret

one_root:
push 10
push 116
push 111
push 111
push 114
push 32
push 101
push 110
push 111
push 32
push 115
push 97
push 104
push 32
push 110
push 111
push 105
push 116
push 97
push 117
push 113
push 101
push 32
push 101
push 104
push 84
push 0
pop ex
for_one_root:
pop [ex]
push ex
push 1
add
pop ex
push ex
push 17
ja for_one_root:
draw
out
ret

