class Neuron {
    // holy tuff
public:
    float v_rest, v_thresh, v_reset, v_leak, v = 0;
    int spike = 0;
    
    Neuron(float rest, float thresh, float reset, float leak) {
        v_rest = rest;
        v_thresh = thresh;
        v_reset = reset;
        v_leak = leak;
        v = reset;
    }
    
    void update(float input_sum) {
        v += input_sum - v_leak * (v - v_rest);
        if (v >= v_thresh) {
            spike = 1;
        } else {
            spike = 0;
        }
    }
    
    void reset() {
        // Call this AFTER synapses have processed the spike
        if (spike) {
            v = v_reset;
            spike = 0;  // Clear the spike flag for next cycle
        }
    }
};
