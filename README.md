# Hermes

## Intention and motivation

Some IoT projects consist of multiple devices, which need to talk to each other.
These devices can be connected via various buses like UART, SPI, etc. This
library provides a functionality to help all these devices to talk to a
**master** device. The **master** is one of the devices which is controls other
devices in the IoT system.

Usually in a very simple case RPC(let say the commmunication is done via Serial)
is done this way:

```c++

// Slave side code on an Arduino Core based system
void loop()
{
    int cmd = Serial.read();
    switch(cmd)
    {
        case 1: { do_something(); break; }
        case 2: { do_something_else(); break; }
        default: { Serial.println("Unknown command"); }
    }
}
```

This approach works but it's:
   * Hard to extend
   * Unsecure
   * Ugly

This library intend to fix these problems and take care about security,
reconnection issues (when connection between devices are based TCP sockets(e.g)).

## Design

The typical system this library to be used in is a system with **1 master**
and **multiple** slave devices. Master can talk to slave devices using
protocols like UART, SPI, I2C. There is an example in `examples` directory,
for a case then slaves are in a chain, so some slaves can act as a
proxies, this makes easier to extends even more the possible
communication ways.


```uml

[Slave 1] <--UART--> [MASTER] <-- SPI --> [Slave 2] <--2.4Ghz(1)-->[Slave 3] <-- I2C-- > [Slave 3]...

2.4Ghz(1) - Let say this connection is done using nRF24L04 attached to the [Slave 2] ans [Slave 3]

```

## Installation

TBD

## Developement, extension, usage, etc

### Developement

The hermes library is a crossplatform library, but still development is mostly
done on an Ubuntu 22.04. For making developement systems similar and issues
more reproducible we use Docker. To copy the developement enviroment just run
`./scripts/run_docker.sh` script. Check `./scripts/run_docker.sh` and
`.scripts/Dockerfile.dev` if you want to create same env on a Windows machine.

### Documentation

To get documentation, just run `./scripts/gen_docs.sh` script. This will
generate HTML documentatoin for the project.

### Testing on hardware

At the moment there no much limitations on platforms, but functionality will be
checked on following platforms:

* Raspberry Pi Zero 2 (Debian Buster)
* Banana Pi Zero M2 (Debian Buster)
* ESP32 (IDF + FreeRTOS)
* Raspberry Pico (Basically almost every RP2040-based circuit should work)

Some basic scenarios can be found in `./examples/<platform>/<example>`


### Support

We will gladly approciate your help in any way you feel comfortable to commit.
Documentation, testing, developement, code review, etc.

### License

Please check `LICENSE`