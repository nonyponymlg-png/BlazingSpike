#include <iostream>
#include <iomanip>
#include "Brain.cpp"

int main()
{
    std::cout << "=== BlazingSpike SNN Demo ===" << std::endl << std::endl;
    
    // Create a brain with 5 neurons
    Brain myBrain(5);
    std::cout << "Created brain with " << myBrain.getNumNeurons() << " neurons" << std::endl;
    
    // Create a network topology with synapses
    // Connect neurons in a chain-like structure with varying weights
    myBrain.connectNeurons(0, 1, 0.5);   // Neuron 0 -> Neuron 1 (weight 0.5)
    myBrain.connectNeurons(1, 2, 0.6);   // Neuron 1 -> Neuron 2 (weight 0.6)
    myBrain.connectNeurons(2, 3, 0.7);   // Neuron 2 -> Neuron 3 (weight 0.7)
    myBrain.connectNeurons(3, 4, 0.8);   // Neuron 3 -> Neuron 4 (weight 0.8)
    myBrain.connectNeurons(0, 3, 0.4);   // Neuron 0 -> Neuron 3 (shortcut)
    myBrain.connectNeurons(1, 4, 0.5);   // Neuron 1 -> Neuron 4 (shortcut)
    
    std::cout << "Created " << myBrain.getNumSynapses() << " synapses" << std::endl;
    std::cout << std::endl;
    
    // Simulate the network for 100 time steps
    std::cout << "Simulating network activity:" << std::endl;
    std::cout << "Time | Input | N0_V   | N0_Sp | N1_V   | N1_Sp | N2_V   | N2_Sp | N3_V   | N3_Sp | N4_V   | N4_Sp" << std::endl;
    std::cout << std::string(120, '-') << std::endl;
    
    for (int t = 0; t < 100; t++) {
        // Provide pulsed input to neuron 0 (every 20 time steps)
        float externalInput = (t % 20 == 0) ? 0.8 : 0.0;
        
        if (externalInput > 0) {
            myBrain.stimulateNeuron(0, externalInput);
        }
        
        // Update the entire brain (process spikes through synapses, update neurons)
        myBrain.update();
        
        // Display network state every 10 time steps
        if (t % 10 == 0) {
            std::cout << std::setw(4) << t << " | ";
            std::cout << std::setw(5) << std::fixed << std::setprecision(2) << externalInput << " | ";
            
            for (int n = 0; n < 5; n++) {
                std::cout << std::setw(6) << std::fixed << std::setprecision(2) 
                         << myBrain.getNeuronVoltage(n) << " | ";
                std::cout << std::setw(5) << myBrain.getNeuronSpike(n) << " | ";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "=== Simulation Complete ===" << std::endl;
    std::cout << "Final neuron voltages:" << std::endl;
    for (int i = 0; i < myBrain.getNumNeurons(); i++) {
        std::cout << "  Neuron " << i << ": V = " << std::fixed << std::setprecision(3) 
                 << myBrain.getNeuronVoltage(i) << " mV, Spike: " 
                 << myBrain.getNeuronSpike(i) << std::endl;
    }
    
    return 0;
}
