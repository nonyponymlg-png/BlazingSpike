#include "Synapse.cpp"
#include <vector>

class Brain {
private:
    int numNeurons;
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    std::vector<float> neuronInputs;
    
public:
    Brain(int _numNeurons) {
        numNeurons = _numNeurons;
        
        for (int i = 0; i < numNeurons; i++) {
            neurons.push_back(new Neuron(0.0, 1.0, 0.0, 0.9));
            neuronInputs.push_back(0.0);
        }
    }
    
    void connectNeurons(int neuronA, int neuronB, float weight) {
        if (neuronA >= 0 && neuronA < numNeurons && 
            neuronB >= 0 && neuronB < numNeurons) {
            synapses.push_back(new Synapse(neurons[neuronA], neurons[neuronB], weight));
        }
    }
    
    void update() {
        // Step 1: Update neurons (integrate inputs and check for spikes)
        for (int i = 0; i < numNeurons; i++) {
            neurons[i]->update(neuronInputs[i]);
            neuronInputs[i] = 0.0;  // Reset input for next cycle
        }
        
        // Step 2: Process spikes through synapses (while spike flag is still 1)
        for (Synapse* synapse : synapses) {
            synapse->update();
        }
        
        // Step 3: Reset neurons that spiked
        for (Neuron* neuron : neurons) {
            neuron->reset();
        }
    }
    
    void stimulateNeuron(int neuronIdx, float input) {
        if (neuronIdx >= 0 && neuronIdx < numNeurons) {
            neuronInputs[neuronIdx] += input;
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
