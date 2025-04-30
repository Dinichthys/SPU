push 2
pop bx

push 4
pop cx

jmp root:

root:
push cx
out

push bx
out

hlt
