enter example

minimize
 x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10 + x11 + x12 +
 z0 + z1 + z2 + z3 + z4 + z5 + z6 + z7 + z8 + z9 + z10 + z11 + z12 +
 y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8 + y9 + y10 + y11 + y12

subject to 
x0 + y0 + x3 + y3 + x9 + y9 -2 w0 = 1
x1 + y1 + x4 + y4 + x9 + y9 + x10 + y10 -2 w1 = 1
x2 + y2 + x5 + y5 + x10 + y10 -2 w2 = 0
x3 + y3 + x6 + y6 + x11 + y11 -2 w3 = 1
x4 + y4 + x7 + y7 + x11 + y11 + x12 + y12 -2 w4 = 1
x5 + y5 + x8 + y8 + x12 + y12 -2 w5 = 1
z0 + y0 + z1 + y1 + z9 + y9 -2 w6 = 1
z1 + y1 + z2 + y2 + z10 + y10 -2 w7 = 1
z3 + y3 + z4 + y4 + z9 + y9 + z11 + y11 -2 w8 = 0
z4 + y4 + z5 + y5 + z10 + y10 + z12 + y12 -2 w9 = 0
z6 + y6 + z7 + y7 + z11 + y11 -2 w10 = 0
z7 + y7 + z8 + y8 + z12 + y12 -2 w11 = 0


bound

general
w0
w1
w2
w3
w4
w5
w6
w7
w8
w9
w10
w11


binary

x0
x1
x2
x3
x4
x5
x6
x7
x8
x9
x10
x11
x12
y0
y1
y2
y3
y4
y5
y6
y7
y8
y9
y10
y11
y12
z0
z1
z2
z3
z4
z5
z6
z7
z8
z9
z10
z11
z12


end