#ifndef BAYESIAN_NODE_H_
#define BAYESIAN_NODE_H_

#include <vector>

using std::vector;

class BayesianNode
{
    public:
        BayesianNode(char _node, vector<BayesianNode *> _parents, vector<float> _cdf);
        vector<BayesianNode *> parents;
        vector<float> cdf;
        char node;
        float getConditionalProb(vector<bool> truth);
        float getProb(bool value, vector<BayesianNode *> given, vector<bool> values);
};

#endif