#include "Brain.cpp"
#include <iostream>

int main() {
    // Create a simple brain with 10 neurons
    Brain brain(10);
    
    // Connect neurons with random weights
    brain.connectNeurons(0, 1, 0.5f);
    brain.connectNeurons(1, 2, 0.3f);
    brain.connectNeurons(2, 3, 0.4f);
    
    // Enable learning rules
    brain.enableSTDPLearning(0.01f);
    brain.enableRewardLearning(0.01f);
    brain.enableHomeostasis(0.1f, 0.001f);
    
    // Run simulation
    for (int step = 0; step < 1000; step++) {
        // Input stimulation
        if (step % 10 == 0) {
            brain.stimulateNeuron(0, 2.0f);
        }
        
        // Set reward signal for learning
        if (step > 500) {
            brain.setReward(0.1f);  // Positive reward
        } else {
            brain.setReward(-0.05f);  // Negative reward
        }
        
        // Update the brain
        brain.update();
        
        // Print diagnostics every 100 steps
        if (step % 100 == 0) {
            std::cout << "Step " << step << ": ";
            std::cout << "Neuron 0 V=" << brain.getNeuronVoltage(0) << ", ";
            std::cout << "Synapse 0 W=" << brain.getSynapseWeight(0) << std::endl;
        }
    }
    
    return 0;
}
