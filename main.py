#!/usr/bin/env python

import numpy as np

np.set_printoptions(10)

A = [
	[ 6.26,  0.96,  1.11,   1.24,  0.24  ],
	[ 1.24,  4.16,  1.30,  -1.63, -1.38  ],
	[ 0.83, -2.46,  5.44,   2.10, -0.917 ],
	[ 1.24,  0.16,  2.10,   6.10, 14.    ],
	[ 0.24,  0.72,  1.183, -7.,    7.    ]
]
B = [ 2.10, 0., -3.01, -6.44, 0. ]

a = np.array(A)
b = np.array(B)
r = np.linalg.solve(a, b)

print(r)