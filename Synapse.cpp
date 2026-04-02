#include "Neuron.cpp"

class Synapse {
    Neuron* n1;
    Neuron* n2;
    float weight = 0;
    
public:
    Synapse(Neuron* _n1, Neuron* _n2, float _weight) {
        n1 = _n1;
        n2 = _n2;
        weight = _weight;
    }
    
    void update() {
        if (n1->spike) {
            n2->v += weight * n1->spike;
        }
    }
    
    void setWeight(float newWeight) {
        weight = newWeight;
    }
    
    float getWeight() {
        return weight;
    }
};
