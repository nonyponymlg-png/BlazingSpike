#include "Neuron.cpp"
#include "Synapse.cpp"
#include <vector>

class Brain {
private:
    int numNeurons;
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    
public:
    Brain(int _numNeurons) {
        numNeurons = _numNeurons;
        
        for (int i = 0; i < numNeurons; i++) {
            neurons.push_back(new Neuron(0.0, 1.0, 0.0, 0.9));
        }
    }
    
    void connectNeurons(int neuronA, int neuronB, float weight) {
        if (neuronA >= 0 && neuronA < numNeurons && 
            neuronB >= 0 && neuronB < numNeurons) {
            synapses.push_back(new Synapse(neurons[neuronA], neurons[neuronB], weight));
        }
    }
    
    void update() {
        for (Synapse* synapse : synapses) {
            synapse->update();
        }
        
        for (Neuron* neuron : neurons) {
            neuron->update(0.0);  // External input can be added here
        }
    }
    
    void stimulateNeuron(int neuronIdx, float input) {
        if (neuronIdx >= 0 && neuronIdx < numNeurons) {
            neurons[neuronIdx]->v += input;
        }
    }
    
    int getNeuronSpike(int neuronIdx) {
        if (neuronIdx >= 0 && neuronIdx < numNeurons) {
            return neurons[neuronIdx]->spike;
        }
        return 0;
    }
    
    float getNeuronVoltage(int neuronIdx) {
        if (neuronIdx >= 0 && neuronIdx < numNeurons) {
            return neurons[neuronIdx]->v;
        }
        return 0.0;
    }
    
    int getNumNeurons() {
        return numNeurons;
    }
    
    int getNumSynapses() {
        return synapses.size();
    }

    ~Brain() {
        for (Neuron* n : neurons) {
            delete n;
        }
        for (Synapse* s : synapses) {
            delete s;
        }
    }
};
