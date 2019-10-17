#include <iostream>

#include "BayesianNode.h"
#include <vector>

using std::vector;


int main(void)
{
    vector<BayesianNode *> nodes;
    BayesianNode * b = new BayesianNode('B', {}, {0.01});
    nodes.push_back(b);
    BayesianNode * c = new BayesianNode('C', {}, {0.001});
    nodes.push_back(c);
    BayesianNode * a = new BayesianNode('A', {c, b}, {0.01, 0.5, 0.9, 0.99});
    nodes.push_back(a);
    BayesianNode * d = new BayesianNode('D', {a}, {0.2, 0.5});
    nodes.push_back(d);

    std::cout << "p(A | B = 0, C = 1) = " << a->getConditionalProb({true, false}) << std::endl;
    std::cout << "P(A|B)" << std::endl << "----------" << std::endl;
    std::cout << a->getProb(true, {b}, {true}) << std::endl << std::endl;
    std::cout << "P(A)" << std::endl << "----------" << std::endl;
    std::cout <<  a->getProb(true, {}, {}) << std::endl << std::endl;
    std::cout << "P(B|A)" << std::endl << "----------" << std::endl;
    std::cout << b->getProb(true, {a}, {true}) << std::endl << std::endl;

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
        nodes[i] = NULL;
    }
}