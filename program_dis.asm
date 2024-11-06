push 0.00000
pop bx
push 65.00000
pop [bx]
push bx
push 1.00000
add
pop bx
push 10000.00000
push bx
jb 18
draw
push 30.00000
pop [0]
push [1]
hlt
