# Voder2017

Digital recreation of the Voder 1939 machine into the new keyboard friendly Voder 2017

# What

The **Voder** (*Voice Operating Demonstrator*) was made by Homer Dudley at Bell Telephone Laboratory
in 1937-1938. The Voder was a system capable of creating recognizable speech without
human vocal cords entered into the procedure at any point.

![](https://i1.wp.com/120years.net/wordpress/wp-content/uploads/voder01jpg.jpg)

Voder Diagram

The system consisted of a "buzzer" tube and a "hissing" tube which were two sound generators.
And a series of keys which could switch between the two sounds and also produce the 
"stop" consonants ('d', 'k', 'p'). There was also a pedal to change pitch during words,
and a knob to make the voice sounds male or female by changing the pitch.

Control system of the Voder

![](https://i0.wp.com/120years.net/wordpress/wp-content/uploads/voder03.jpg?w=683)

**Video demonstrating the Voder**

[![](https://img.youtube.com/vi/5hyI_dM5cGo/0.jpg)](https://www.youtube.com/watch?v=5hyI_dM5cGo)

Using the Voder required training as it required precise timing and accuracy to
use effectively. The best operator was Helen Harper, who had a year of constant practice
and trained approximately 20 other operators. The Voder had performances at the 1939
New York World's Fair and in San Francisco's Golden Gate International Exposition in late 1940.
After the exhibitions the machine disappeared as it was a "proof-of-concept", and had no commercial value.


# How

We are using The Synthesis Toolkit (STK) with C++ to create the Voder.
It is an open source realtime sound sythesis library.
https://ccrma.stanford.edu/software/stk/



# Why

To see what we can learn from reimplementing a historic system

# How to use
stk requires libasound2 and libasound2-dev in order to configure using automake  
commands:  
sudo apt install automake  
sudo apt-get install libasound2  
sudo apt-get install libasound2-dev  
then   
./configure   
while in the stk-4.6.0 directory.  



TBD -- currently under progress
