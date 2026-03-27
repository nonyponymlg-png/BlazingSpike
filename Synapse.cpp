class Synapse {
    Neuron* n1, n2;
    float weight = 0;
    Synapse(Neuron* _n1, Neuron* _n2, float _weight) {
        n1=_n1;
        n2=_n2;
        weight=_weight;
    }
    update() {
        
    }
}