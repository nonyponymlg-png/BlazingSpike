#include "Synapse.cpp"
#include "RewardLearning.cpp"
#include "Homeostasis.cpp"
#include <vector>

class Brain {
private:
    int numNeurons;
    std::vector<Neuron*> neurons;
    std::vector<Synapse*> synapses;
    std::vector<float> neuronInputs;
    
    // Learning rules
    RewardModulatedRule* rewardLearning = nullptr;
    std::vector<HomeostasisRule*> homeostasisRules;
    
    float simulationTime = 0.0f;
    
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
    
    void enableSTDPLearning(float learningRate = 0.01f) {
        for (Synapse* syn : synapses) {
            syn->enableSTDP(learningRate);
        }
    }
    
    void enableRewardLearning(float learningRate = 0.01f) {
        if (!rewardLearning) {
            rewardLearning = new RewardModulatedRule(learningRate);
        }
    }
    
    void enableHomeostasis(float targetFiringRate = 0.1f, float learningRate = 0.001f) {
        homeostasisRules.clear();
        for (int i = 0; i < numNeurons; i++) {
            homeostasisRules.push_back(new HomeostasisRule(targetFiringRate, learningRate));
        }
    }
    
    void setReward(float reward) {
        if (rewardLearning) {
            rewardLearning->setReward(reward);
        }
    }
    
    void update() {
        // Step 1: Update neurons (integrate inputs and check for spikes)
        for (int i = 0; i < numNeurons; i++) {
            neurons[i]->update(neuronInputs[i]);
            neuronInputs[i] = 0.0;  // Reset input for next cycle
        }
        
        // Step 2: Process spikes through synapses
        for (int i = 0; i < synapses.size(); i++) {
            synapses[i]->update();
        }
        
        // Step 3: Apply learning rules
        // STDP learning (happens during synapse update)
        for (Synapse* syn : synapses) {
            if (dynamic_cast<Synapse*>(syn)) {
                syn->updateLearning();
            }
        }
        
        // Reward-modulated learning
        if (rewardLearning) {
            rewardLearning->decayEligibilityTraces();
            
            for (int i = 0; i < synapses.size(); i++) {
                // Check if synapse should be marked active (pre-synaptic spike)
                if (synapses[i]->getWeight() != 0) {  // Simplified activation check
                    rewardLearning->markSynapseActive(i);
                    float weightChange = rewardLearning->computeWeightChange(i);
                    float newWeight = synapses[i]->getWeight() + weightChange;
                    synapses[i]->setWeight(newWeight);
                }
            }
        }
        
        // Homeostatic plasticity
        if (!homeostasisRules.empty()) {
            for (int i = 0; i < numNeurons; i++) {
                if (i < homeostasisRules.size()) {
                    homeostasisRules[i]->recordSpike(neurons[i]->spike);
                }
            }
            
            // Periodically apply homeostatic scaling (every 100 steps)
            if (static_cast<int>(simulationTime) % 100 == 0) {
                for (Synapse* syn : synapses) {
                    float scale = homeostasisRules[0]->computeWeightScaling();
                    syn->setWeight(syn->getWeight() * scale);
                }
            }
        }
        
        // Step 4: Reset neurons that spiked
        for (Neuron* neuron : neurons) {
            neuron->reset();
        }
        
        simulationTime += 1.0f;
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
    
    float getSynapseWeight(int synapseIdx) {
        if (synapseIdx >= 0 && synapseIdx < synapses.size()) {
            return synapses[synapseIdx]->getWeight();
        }
        return 0.0f;
    }
    
    int getNumNeurons() {
        return numNeurons;
    }
    
    int getNumSynapses() {
        return synapses.size();
    }
    
    float getSimulationTime() {
        return simulationTime;
    }

    ~Brain() {
        for (Neuron* n : neurons) {
            delete n;
        }
        for (Synapse* s : synapses) {
            delete s;
        }
        if (rewardLearning) delete rewardLearning;
        for (HomeostasisRule* h : homeostasisRules) {
            delete h;
        }
    }
};
