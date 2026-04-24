#include <vector>
#include <cmath>

class RewardModulatedRule {
private:
    float learningRate;
    float rewardSignal;
    float tau_eligibility;  // Eligibility trace time constant
    std::vector<float> eligibilityTraces;
    
public:
    RewardModulatedRule(float _learningRate = 0.01f, float _tau_eligibility = 50.0f) {
        learningRate = _learningRate;
        tau_eligibility = _tau_eligibility;
        rewardSignal = 0.0f;
    }
    
    void setReward(float reward) {
        rewardSignal = reward;
    }
    
    float getReward() {
        return rewardSignal;
    }
    
    void decayEligibilityTraces() {
        float decayFactor = std::exp(-1.0f / tau_eligibility);
        for (float& trace : eligibilityTraces) {
            trace *= decayFactor;
        }
    }
    
    void markSynapseActive(int synapseIdx) {
        if (synapseIdx >= eligibilityTraces.size()) {
            eligibilityTraces.resize(synapseIdx + 1, 0.0f);
        }
        eligibilityTraces[synapseIdx] = 1.0f;  // Mark synapse as active
    }
    
    float computeWeightChange(int synapseIdx) {
        if (synapseIdx >= eligibilityTraces.size()) return 0.0f;
        
        // Weight change = learning rate * eligibility trace * reward signal
        return learningRate * eligibilityTraces[synapseIdx] * rewardSignal;
    }
    
    void resetEligibilityTraces() {
        for (float& trace : eligibilityTraces) {
            trace = 0.0f;
        }
    }
};
