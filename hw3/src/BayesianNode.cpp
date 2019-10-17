#include "BayesianNode.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>

BayesianNode::BayesianNode(char _node, vector<BayesianNode *> _parents, vector<float> _cdf):
    node(_node),
    parents(_parents),
    cdf(_cdf)
{
    if(cdf.size() != (unsigned int) (1 << parents.size()))
    {
        throw std::runtime_error("Cdf size must be 2^(size of parents)");
    }
}

float BayesianNode::getConditionalProb(vector<bool> truth)
{
    if(truth.size() != parents.size())
    {
        throw std::runtime_error("Truth vector size " + std::to_string(truth.size()) + " for node \'" + std::string(1, node) + "\' must be the same size as the number of parents");
    }

    int offset = 0;
    unsigned int truthCount = truth.size();
    for(unsigned int i = 0; i < truthCount; i++)
    {
        // set bit to one if the corresponding parent is true
        // shift needs to be reversed because the bools should match the nodes
        offset |= truth[i] << i;
    }

    return cdf[offset];
}

float BayesianNode::getProb(bool value, vector<BayesianNode *> given, vector<bool> values)
{
    if(given.size() != values.size())
    {
        throw std::runtime_error("Truth values should be the same size as the given nodes");
    }

    // if given nothing and the the node has no parents (terminal node)
    if(parents.size() == 0)
    {
        if(given.size() == 0)
        {
            // return the probability
            float termProb = cdf[0];
            if(!value)
            {
                termProb = 1 - termProb;
            }
            std::cout << "P(" << std::string(1, node) << " = " << value << ") = " << termProb << std::endl;
            return termProb;
        } else
        {
            // Bayes rule
            // currently assuming only one node given
            float bayesProb =  given[0]->getProb(values[0], {this}, {true}) * this->getProb(value, {}, {}) / given[0]->getProb(values[0], {}, {});
            std::cout << "P(" << std::string(1, node) << " = " << value << " | " << std::string(1, given[0]->node) << " = " << values[0] << ")" << std::endl;
            std::cout << "= ";
            std::cout << "P(" << std::string(1, given[0]->node) << " = " << values[0] << " | " << std::string(1, node) << " = " << value << ")";
            std::cout << " * P(" <<  std::string(1, node) << " = " << value << ")";
            std::cout << " / P(" <<  std::string(1, given[0]->node) << " = " << values[0] << ")";
            std::cout << std::endl;

            return bayesProb;
        }
        
    }

    vector<bool> truthVector(parents.size());
    vector<bool> givenIndices(parents.size());
    vector<BayesianNode * > givenNonParents = given;
    vector<bool> givenNonParentValues = values;
    vector<BayesianNode *> givenParents;
    vector<bool> givenParentValues;
    for(unsigned int i = 0; i < parents.size(); i++)
    {
        vector<BayesianNode *>::iterator it = std::find(given.begin(), given.end(), parents[i]);
        if(it != given.end())
        {
            // if this node is given:
            int givenIndex = std::distance(given.begin(), it);
            // push the given value
            truthVector[i] = given[givenIndex];
            givenIndices[i] = true;

            // if the given node is in the parents, remove it from the given non-parents
            vector<BayesianNode *>::iterator givenParentIt = std::find(givenNonParents.begin(), givenNonParents.end(), parents[i]);
            if(givenParentIt != givenNonParents.end())
            {
                int givenParentIndex = std::distance(givenNonParents.begin(), givenParentIt);
                givenNonParents.erase(givenParentIt);
                givenNonParentValues.erase(givenNonParentValues.begin() + givenParentIndex);
            }

            givenParents.push_back(parents[i]);
            givenParentValues.push_back(truthVector[i]);
        } else
        {
            // if the node is not given:
            // just push back false
            truthVector[i] = false;
            givenIndices[i] = false;
        }
    }

    std::string outputString = "";

    outputString += "P(" + std::string(1, node) + " = " + std::to_string(value);
    if(givenParents.size() > 0)
    {
        outputString += " | ";
    }
    for(unsigned int i = 0; i < givenParents.size(); i++)
    {
        outputString += std::string(1, givenParents[i]->node) + " = " + std::to_string(givenNonParentValues[i]);
        if(i < givenParents.size() - 1)
        {
            outputString += ", ";
        }
    }

    outputString += ")\n= ";

    float prob = 0;

    // for every combination of non-given parents
    unsigned int numSigs = 1 << (parents.size() - given.size() + givenNonParents.size());
    for(unsigned int binSig = 0; binSig < numSigs; binSig++)
    {

        outputString += "P(" + std::string(1, node) + " = " + std::to_string(value) + " | ";
        unsigned int sigBit = 0;
        for(unsigned int i = 0; i < truthVector.size(); i++)
        {
            if(!givenIndices[i])
            {
                // alternate the non-given truth value based on the bit the of the binary signature
                truthVector[i] = binSig & (1 << sigBit);
                sigBit++;
            }
            outputString += std::string(1, parents[i]->node) + " = " + std::to_string(truthVector[i]);
            if(i < truthVector.size() - 1)
            {
                outputString += ", ";
            }
        }

        outputString +=  ") ";

        // get the conditional probability for the given values
        float sigProb = getConditionalProb(truthVector);
        for(unsigned int i = 0; i < parents.size(); i++)
        {
            // if not given
            if(!givenIndices[i])
            {
                // multiply the by the probability of true or not true
                float parentProb = parents[i]->getProb(truthVector[i], givenNonParents, givenNonParentValues);

                sigProb *= parentProb;
                outputString +=  "* P(" + std::string(1, parents[i]->node) + " = " + std::to_string(truthVector[i]) + ") ";
            }
        }

        prob += sigProb;

        if(binSig < numSigs - 1)
        {
            outputString += "+ ";
        }
    }

    std::cout << outputString << std::endl;

    if(value)
    {
        return prob;
    } else
    {
        return 1 - prob;
    }
}