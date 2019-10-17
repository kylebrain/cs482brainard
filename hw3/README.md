## Complilation
```bash
make
./hw3
```

## Creating the network
To create a node initialize it with the parents and the conditional probabilities in a binary table\
Parents are inputted first with the lowest order bit\
The indices of conditional probabilities correspond with the truth values of the parents

For example

| Index | P(A) |
|----|------|
| 00 | 0.01 |
| 01 | 0.5  |
| 10 | 0.9  |
| 11 | 0.99 |

B is the higher order bit\
C is the lower order bit

```c++
BayesianNode * a = new BayesianNode('A', {c, b}, {0.01, 0.5, 0.9, 0.99});
```

## Making querys
To make querys call getProb()\
getProb() takes in the value of the object and the given values\
Given values are given as a list of given nodes and a list of indice matched truth values for those given nodes

For example\
P(A|B)

```c++
a->getProb(true, {b}, {true});
```

## Limitations
For any query the there can only be one node to get the probability\
For example, you cannot get P(A, B | C)

Furthermore, when making a query in which the given node is a child of the query node, there can only be one given\
For example, you cannot get P(B | A, D) with the provided network
