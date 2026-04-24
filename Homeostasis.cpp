#include <vector>
#include <cmath>

class HomeostasisRule {
private:
    float targetFiringRate;
    float learningRate;
    float windowSize;
    std::vector<int> spikeHistory;
    int historyIndex;
    
public:
    HomeostasisRule(float _targetFiringRate = 0.1f, float _learningRate = 0.001f, 
                    float _windowSize = 1000.0f) {
        targetFiringRate = _targetFiringRate;
        learningRate = _learningRate;
        windowSize = _windowSize;
        spikeHistory.resize(static_cast<int>(_windowSize), 0);
        historyIndex = 0;
    }
    
    void recordSpike(int spike) {
        spikeHistory[historyIndex] = spike;
        historyIndex = (historyIndex + 1) % static_cast<int>(windowSize);
    }
    
    float getCurrentFiringRate() {
        int totalSpikes = 0;
        for (int spike : spikeHistory) {
            totalSpikes += spike;
        }
        return static_cast<float>(totalSpikes) / windowSize;
    }
    
    float computeWeightScaling() {
        float currentRate = getCurrentFiringRate();
        
        // Multiplicative scaling
        if (currentRate == 0.0f) currentRate = 0.0001f;  // Avoid division by zero
        
        float scalingFactor = std::pow(targetFiringRate / currentRate, learningRate);
        
        // Clip scaling factor to reasonable bounds
        if (scalingFactor > 1.5f) scalingFactor = 1.5f;
        if (scalingFactor < 0.67f) scalingFactor = 0.67f;
        
        return scalingFactor;
    }
    
    void setTargetFiringRate(float rate) {
        targetFiringRate = rate;
    }
    
    void setLearningRate(float lr) {
        learningRate = lr;
    }
};
