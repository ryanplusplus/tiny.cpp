# tiny.cpp
[![Tests](https://github.com/ryanplusplus/tiny.cpp/actions/workflows/test.yml/badge.svg)](https://github.com/ryanplusplus/tiny.cpp/actions/workflows/test.yml)

`tiny.cpp` is an embedded development library designed to be enjoyable to use, even in a heapless environment. Its components are designed to be flexible, reusable, and have RAM and ROM usage. `tiny.cpp` can be used bare metal, with an RTOS, or integrated into an existing framework like Arduino or Mbed and can be used on all but the smallest microcontrollers.

`tiny.cpp` is a C++ implementation of [`tiny`](https://github.com/ryanplusplus/tiny).

## Design
### Simple
`tiny.cpp` favors simplicity over providing every possible feature. This makes it easier to learn, keeps code size and compile times down, and helps to ensure that the features that _are_ included are high impact.

### Event-Driven
`tiny.cpp` is designed for event-driven applications. It relies upon short RTCs (Run-To-Complete steps) and synchronous event propagation to enable "multi-tasking" without threading. This is roughly analogous to the JavaScript programming model.

### Flexible
`tiny.cpp` relies upon run-time polymorphism provided by interfaces (pure abstract classes) without inheritance. Interfaces make `tiny.cpp` flexible, portable, and testable.

## What's in the Box?
### HAL
`tiny.cpp` provides a basic HAL (Hardware Abstraction Layer) that includes interfaces for interacting with hardware like digital I/O, analog inputs, PWM channels, and serial communication protocols. Most of the core `tiny.cpp` components are hardware independent, but those that aren't use the HAL to be portable.

See [include/hal](include/tiny/hal) for a list of all interfaces.

### Data Structures
`tiny.cpp` provides heapless, arbitrarily-sized (intrusive) list and ring buffer data structures. See:
- [`List`](include/tiny/List.hpp)
- [`RingBuffer`](include/tiny/RingBuffer.hpp)

### Software Timers
Software timers allow applications to generate one-shot and periodic time-based events. See:
- [`Timer`](include/tiny/Timer.hpp)

### Events
`tiny.cpp` defines an abstract event type as well as some concrete event implementations. Events are basically fancy callbacks that event handlers to be registered dynamically. See:
- [`IEvent`](include/tiny/IEvent.hpp)
- [`Event`](include/tiny/Event.hpp)
- [`SingleSubscriberEvent`](include/tiny/SingleSubscriberEvent.hpp)

### Event Queues
Event queues allow work to be queued to another RTC or to move an between an interrupt context and a non-interrupt context. See:
- [`IEventQueue`](include/tiny/IEventQueue.hpp)
- [`EventQueue`](include/tiny/EventQueue.hpp)

### Messages Buses
`tiny.cpp`'s message bus interface allows components to broadcast events within an application. See:
- [`IMessageBus`](include/tiny/IMessageBus.hpp)
- [`MessageBus`](include/tiny/MessageBus.hpp)

### Key Value Stores
`tiny.cpp`'s key value store allows for data to be shared within a component in a structured way. Changes to values within a key value store generate data events that enable components to "watch" shared data. See:
- [`IKeyValueStore`](include/tiny/IKeyValueStore.hpp)
- [`RamKeyValueStore`](include/tiny/RamKeyValueStore.hpp)

### State Machines
`tiny.cpp` provides lightweight FSM (Finite State Machine) and HSM (Hierarchical State Machine) libraries. These model states as functions that are invoked when the state machine is signaled. State machines allow event-driven programs to be written without spaghetti code. See:
- [`Fsm`](include/tiny/Fsm.hpp)
- [`Hsm`](include/tiny/Hsm.hpp)

### Communication
`tiny.cpp` provides a simple library for point-to-point communication. It allows payloads to be sent between nodes in a network with data integrity protected by a CRC16.
- [`IComm`](include/tiny/IComm.hpp)
- [`Comm`](include/tiny/Comm.hpp)

## How Can I Use It?
Integrating `tiny.cpp` is simple: just put `include/` on the include path and build everything in `src/`.

To include `tiny.cpp` in a test build, add `test/include` to the include path and build the contents of `test/src`. This brings useful test doubles into your tests.

### Examples
#### Bare Metal
See [`ryanplusplus/atmega328p-tiny.cpp-starter-kit`](https://github.com/ryanplusplus/atmega328p-tiny.cpp-starter-kit) for an example of a bare metal `tiny.cpp` project using the Microchip ATmega328P. This is a simple project that sets up a blinking LED and includes several hardware drivers that use the `tiny.cpp` HAL.

## Development
Building and running tests requires MacOS or Linux and:
- A toolchain based on [`gcc`](https://gcc.gnu.org/) or [`clang`](https://clang.llvm.org/)
- [GNU `make`](https://www.gnu.org/software/make/)
- [CppUTest](http://cpputest.github.io/)
- [`rerun`](https://rerun.github.io/rerun/) (optional)

Build and run tests with:

```shell
$ make
```

Or use `rerun` to automatically build and run tests when source files change with:

```shell
$ rerun make
```
