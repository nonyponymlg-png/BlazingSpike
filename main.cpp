/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include "Neuron.cpp"
int main()
{
    Neuron myNeuron = Neuron(0.0,1.0,0.0,.9);
    Neuron myNeuron_1 = Neuron(0.0,1.0,0.0,.9);
    std::cout<<"Hello World";
    for(int i = 0; i < 1000; i++) {
        myNeuron.update(myNeuron_1.spike);
        myNeuron_1.update(i*.001);
        std::cout<<myNeuron.v;
        std::cout<<"\n";
    }

    return 0;
}