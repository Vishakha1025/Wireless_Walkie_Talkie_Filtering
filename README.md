# Wireless Walkie-Talkie Filtering Using DSP

## Project Overview

This project demonstrates wireless Digital Signal Processing (DSP) using ESP32-S3 microcontrollers and NRF24L01+ RF transceiver modules. It implements an IIR (Infinite Impulse Response) low-pass filter to process input signals and wirelessly transmit both raw and filtered data between devices.

The system demonstrates how digital filtering can reduce high-frequency components from a signal while enabling wireless data communication.

## Objectives

- To understand the fundamentals of Digital Signal Processing.
- To implement an IIR low-pass filter using ESP32-S3.
- To study raw and filtered signal characteristics.
- To establish wireless communication using NRF24L01+.
- To visualize signal filtering using Arduino Serial Plotter.

## Hardware Components

- ESP32-S3 development boards
- NRF24L01+ 2.4 GHz RF transceiver modules
- Connecting wires
- USB cable and power supply

## Software and Tools

- Arduino IDE
- Arduino Serial Plotter
- Embedded C/C++

## Working Principle

1. The ESP32-S3 generates input test signals consisting of different frequency components.
2. The generated signal is processed using a first-order IIR low-pass filter.
3. The filter reduces high-frequency components while retaining lower-frequency components.
4. The ESP32-S3 packages the raw and filtered samples into data packets.
5. NRF24L01+ transmits the packets wirelessly to the receiving device.
6. The received signal data can be monitored and visualized using Arduino Serial Plotter.

## DSP Technique Used

**IIR Low-Pass Filter**

The project uses a first-order IIR low-pass filter implemented using the following difference equation:

`y[n] = y[n-1] + α(x[n] - y[n-1])`

Where:

- `x[n]` = Current input sample
- `y[n]` = Current filtered output
- `y[n-1]` = Previous filtered output
- `α` = Filter coefficient

The filter is implemented using an exponential smoothing approach.

## Wireless Communication

- Microcontroller: ESP32-S3
- RF module: NRF24L01+
- Frequency band: 2.4 GHz
- Communication interface: SPI
- Data transmission: Wireless packet communication

## Signal Visualization

The Arduino Serial Plotter is used to visualize and compare raw input signals with filtered output signals.

The comparison demonstrates the effect of low-pass filtering on signals containing different frequency components.

## Applications

- Wireless signal processing
- Digital communication systems
- Embedded DSP applications
- Real-time signal monitoring
- Educational DSP experiments

## Limitations

- The current implementation uses generated test signals rather than live microphone input.
- Filter performance depends on sampling frequency and filter coefficient.
- Wireless communication can be affected by interference and transmission range.
- The current implementation demonstrates signal filtering and wireless data transfer, not complete voice transmission.

## Future Scope

- Integrating microphone-based real-time audio input.
- Implementing advanced digital filters.
- Improving wireless communication reliability.
- Adding real-time audio processing and playback.
- Optimizing power consumption and system performance.

## Project Team

- Vishakha Kirange
- Sanket Pawar
- Pranjal Sawant

## Related Project

[Digital Walkie-Talkie](https://github.com/Vishakha1025/Digital-Walkie-Talkie) – ESP32-S3-based wireless voice communication project using NRF24L01+.

---

**Project Domain:** Digital Signal Processing | Wireless Communication | Embedded Systems
