#include <cmath>
#include <deque>

class STDPRule {
private:
    float learningRate;
    float tau_plus;   // Time constant for LTP
    float tau_minus;  // Time constant for LTD
    float A_plus;     // Amplitude for LTP
    float A_minus;    // Amplitude for LTD
    
    std::deque<float> preSpikeTimes;   // Store pre-synaptic spike times
    std::deque<float> postSpikeTimes;  // Store post-synaptic spike times
    
public:
    STDPRule(float _learningRate = 0.01f, float _tau_plus = 20.0f, 
             float _tau_minus = 20.0f, float _A_plus = 0.01f, float _A_minus = 0.01f) {
        learningRate = _learningRate;
        tau_plus = _tau_plus;
        tau_minus = _tau_minus;
        A_plus = _A_plus;
        A_minus = _A_minus;
    }
    
    void recordPreSpike(float currentTime) {
        preSpikeTimes.push_back(currentTime);
        
        // Remove old pre-spike times (older than 5*tau_plus)
        while (!preSpikeTimes.empty() && currentTime - preSpikeTimes.front() > 5 * tau_plus) {
            preSpikeTimes.pop_front();
        }
    }
    
    void recordPostSpike(float currentTime) {
        postSpikeTimes.push_back(currentTime);
        
        // Remove old post-spike times
        while (!postSpikeTimes.empty() && currentTime - postSpikeTimes.front() > 5 * tau_minus) {
            postSpikeTimes.pop_front();
        }
    }
    
    float computeWeightChange(float currentTime) {
        float weightChange = 0.0f;
        
        // Check all pre-spike events against recent post-spike event
        if (!postSpikeTimes.empty()) {
            float lastPostSpikeTime = postSpikeTimes.back();
            
            for (float preTime : preSpikeTimes) {
                float dt = lastPostSpikeTime - preTime;  // dt > 0 means post after pre (LTP)
                
                if (dt > 0) {
                    // LTP: post-synaptic spike after pre-synaptic spike
                    weightChange += A_plus * std::exp(-dt / tau_plus);
                } else if (dt < 0) {
                    // LTD: post-synaptic spike before pre-synaptic spike
                    weightChange -= A_minus * std::exp(dt / tau_minus);
                }
            }
        }
        
        // Check all post-spike events against recent pre-spike event
        if (!preSpikeTimes.empty()) {
            float lastPreSpikeTime = preSpikeTimes.back();
            
            for (float postTime : postSpikeTimes) {
                float dt = postTime - lastPreSpikeTime;
                
                if (dt > 0 && dt <= tau_plus) {  // Avoid double counting
                    weightChange += A_plus * std::exp(-dt / tau_plus);
                }
            }
        }
        
        return learningRate * weightChange;
    }
    
    void setLearningRate(float lr) {
        learningRate = lr;
    }
};
