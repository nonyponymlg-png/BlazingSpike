#include "Neuron.cpp"
#include "STDP.cpp"

class Synapse {
    Neuron* n1;
    Neuron* n2;
    float weight = 0;
    STDPRule* stdpRule = nullptr;
    float minWeight = -1.0f;
    float maxWeight = 1.0f;
    float timeStep = 0.0f;
    
public:
    Synapse(Neuron* _n1, Neuron* _n2, float _weight) {
        n1 = _n1;
        n2 = _n2;
        weight = _weight;
    }
    
    void enableSTDP(float learningRate = 0.01f) {
        stdpRule = new STDPRule(learningRate);
    }
    
    void update() {
        if (n1->spike) {
            n2->v += weight * n1->spike;
            
            // Record pre-synaptic spike for STDP
            if (stdpRule) {
                stdpRule->recordPreSpike(timeStep);
            }
        }
    }
    
    void updateLearning() {
        if (stdpRule && n2->spike) {
            stdpRule->recordPostSpike(timeStep);
            
            // Apply weight change
            float weightChange = stdpRule->computeWeightChange(timeStep);
            weight += weightChange;
            
            // Clip weights to bounds
            if (weight > maxWeight) weight = maxWeight;
            if (weight < minWeight) weight = minWeight;
        }
        
        timeStep += 1.0f;  // Increment time
    }
    
    void setWeight(float newWeight) {
        weight = newWeight;
    }
    
    float getWeight() {
        return weight;
    }
    
    void setWeightBounds(float minW, float maxW) {
        minWeight = minW;
        maxWeight = maxW;
    }
    
    ~Synapse() {
        if (stdpRule) delete stdpRule;
    }
};
