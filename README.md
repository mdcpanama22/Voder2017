# Voder2017

Digital recreation of the Voder 1939 machine into the new keyboard friendly Voder 2017.

Realtime keyboard speech synthesis system.

# What

The **Voder** (*Voice Operating Demonstrator*) was made by Homer Dudley at Bell Telephone Laboratory
in 1937-1938. The Voder was a system capable of creating recognizable speech without
human vocal cords entered into the procedure at any point.

**Voder Diagram**

![](https://i1.wp.com/120years.net/wordpress/wp-content/uploads/voder01jpg.jpg)

The system consisted of a "buzzer" tube and a "hissing" tube which were two sound generators.
And a series of keys which could switch between the two sounds and modulate those sounds into 
other sounds. As well as produce the "stop" consonants ('d', 'k', 'p'). There was also a pedal 
to change pitch during words, and a knob to make the voice sounds male or female by changing the pitch.

**Control system of the Voder**

![](https://i0.wp.com/120years.net/wordpress/wp-content/uploads/voder03.jpg?w=683)

**Video demonstrating the Voder**

[![](https://img.youtube.com/vi/5hyI_dM5cGo/0.jpg)](https://www.youtube.com/watch?v=5hyI_dM5cGo)

Using the Voder required training as it required precise timing and accuracy to
use effectively. The best operator was Helen Harper, who had a year of constant practice
and trained approximately 20 other operators. The Voder had performances at the 1939
New York World's Fair and in San Francisco's Golden Gate International Exposition in late 1940.
After the exhibitions the machine disappeared as it was a "proof-of-concept", and had no commercial value.


# How

Using C++ and The Synthesis Toolkit (STK). STK is an open source audio signal processing
and algorithmic synthesis classes. STK allows for extremely simple and portable realtime audio
processing. More information can be found on the [STK Website](https://ccrma.stanford.edu/software/stk/)

# Why

The Voder has an important place in history as one of the first speech synthesis systems.
A lot could be learned from recreating the system in the 21st century 80 years after it's inception.
It can be used for text-to-speech without the need of any sound recordings.

# How to use

#### Running the Voder

Pull and type make

#### Installing STK by itself

[Download Page](https://ccrma.stanford.edu/software/stk/download.html)

**Install required packages**

```sudo apt install automake autoconf libasound2 libasound2-dev```

**Move into STK folder**

```
autoconf
./configure
make
```

# Who

If you have any questions contact us: gurnys@rpi.edu

## License

Voder2017 is licensed under GPL v3.0 which can be viewed in [LICENSE](LICENSE.txt)
