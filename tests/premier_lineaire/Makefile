list.txt: A
	echo "Finished"

A: B
	./premier `echo 19*200000000/20 |bc` `echo 20*200000000/20-1 |bc` >> list.txt

B: C
	./premier `echo 18*200000000/20 |bc` `echo 19*200000000/20-1 |bc` >> list.txt
C: D
	./premier `echo 17*200000000/20 |bc` `echo 18*200000000/20-1 |bc` >> list.txt
D: E
	./premier `echo 16*200000000/20 |bc` `echo 17*200000000/20-1 |bc` >> list.txt
E: F
	./premier `echo 15*200000000/20 |bc` `echo 16*200000000/20-1 |bc` >> list.txt
F: G
	./premier `echo 14*200000000/20 |bc` `echo 15*200000000/20-1 |bc` >> list.txt
G: H
	./premier `echo 13*200000000/20 |bc` `echo 14*200000000/20-1 |bc` >> list.txt
H: I
	./premier `echo 12*200000000/20 |bc` `echo 13*200000000/20-1 |bc` >> list.txt
I: J
	./premier `echo 11*200000000/20 |bc` `echo 12*200000000/20-1 |bc` >> list.txt
J: K
	./premier `echo 10*200000000/20 |bc` `echo 11*200000000/20-1 |bc` >> list.txt
K: L
	./premier `echo 9*200000000/20 |bc` `echo 10*200000000/20-1 |bc` >> list.txt
L: M
	./premier `echo 8*200000000/20 |bc` `echo 9*200000000/20-1 |bc` >> list.txt
M: N
	./premier `echo 7*200000000/20 |bc` `echo 8*200000000/20-1 |bc` >> list.txt
N: O
	./premier `echo 6*200000000/20 |bc` `echo 7*200000000/20-1 |bc` >> list.txt
O: P
	./premier `echo 5*200000000/20 |bc` `echo 6*200000000/20-1 |bc` >> list.txt
P: Q
	./premier `echo 4*200000000/20 |bc` `echo 5*200000000/20-1 |bc` >> list.txt

Q: R
	./premier `echo 3*200000000/20 |bc` `echo 4*200000000/20-1 |bc` >> list.txt

R: S
	./premier `echo 2*200000000/20 |bc` `echo 3*200000000/20-1 |bc` >> list.txt

S: T
	./premier `echo 1*200000000/20 |bc` `echo 2*200000000/20-1 |bc` >> list.txt

T:
	./premier 2 `echo 1*200000000/20-1 |bc` > list.txt