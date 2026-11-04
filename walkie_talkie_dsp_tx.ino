
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <math.h>

// Set 1 for Board A, 2 for Board B
#define MY_ID 1

// Your existing NRF24L01 connections
#define CE_PIN    4
#define CSN_PIN   5
#define SCK_PIN   12
#define MOSI_PIN  11
#define MISO_PIN  13

// Your existing PTT connection
#define PTT_PIN 1

RF24 radio(CE_PIN, CSN_PIN);

const byte NODE1[6] = "NODE1";
const byte NODE2[6] = "NODE2";

// 27-byte NRF24L01 payload
struct __attribute__((packed)) Packet {
  uint8_t sender;
  uint16_t counter;
  int16_t raw[6];
  int16_t filtered[6];
};

const float FS = 100.0f;
const float F1 = 5.0f;
const float F2 = 25.0f;
const float FC = 10.0f;
const float PI_F = 3.14159265f;

float alpha;
float filterState = 0.0f;

uint32_t sampleIndex = 0;
uint16_t packetCounter = 0;

unsigned long lastTransmit = 0;
bool previousPTT = false;

const unsigned long TX_INTERVAL = 60; // 6 samples / 60 ms

// Generate a test signal
int16_t generateSignal() {
  float t = (float)sampleIndex / FS;

  float x =
      10000.0f * sinf(2.0f * PI_F * F1 * t) +
       5000.0f * sinf(2.0f * PI_F * F2 * t);

  sampleIndex++;
  return (int16_t)x;
}

// First-order IIR low-pass filter
int16_t applyFilter(int16_t input) {
  filterState += alpha * ((float)input - filterState);
  return (int16_t)filterState;
}

// Print only numeric data for Serial Plotter
void plotSamples(const int16_t *raw, const int16_t *filtered) {
  for (int i = 0; i < 6; i++) {
    Serial.print("Raw:");
    Serial.print(raw[i]);
    Serial.print("\tFiltered:");
    Serial.println(filtered[i]);
  }
}

void transmitPacket() {
  Packet packet = {};
  packet.sender = MY_ID;
  packet.counter = packetCounter++;

  // Generate six samples and filter them
  for (int i = 0; i < 6; i++) {
    packet.raw[i] = generateSignal();
    packet.filtered[i] = applyFilter(packet.raw[i]);
  }

  radio.stopListening();
  bool success = radio.write(&packet, sizeof(packet));
  radio.startListening();

  // Plot the local DSP signals even if radio transmission fails
  plotSamples(packet.raw, packet.filtered);

  // Avoid printing text while Serial Plotter is running.
  (void)success;
}

void receivePackets() {
  while (radio.available()) {
    Packet packet = {};

    if (radio.getPayloadSize() == sizeof(Packet)) {
      radio.read(&packet, sizeof(packet));

      if (packet.sender != MY_ID) {
        // Plot received raw and filtered data
        plotSamples(packet.raw, packet.filtered);
      }
    } else {
      // Discard a payload of unexpected size
      uint8_t discard[32];
      radio.read(&discard, sizeof(discard));
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(PTT_PIN, INPUT_PULLDOWN);

  // alpha = dt / (RC + dt)
  float dt = 1.0f / FS;
  float rc = 1.0f / (2.0f * PI_F * FC);
  alpha = dt / (rc + dt);

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);

  if (!radio.begin()) {
    // No text output: keep the plotter stream numeric.
    while (true) {
      delay(1000);
    }
  }

  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);
  radio.setPayloadSize(sizeof(Packet));

  if (MY_ID == 1) {
    radio.openWritingPipe(NODE2);
    radio.openReadingPipe(1, NODE1);
  } else {
    radio.openWritingPipe(NODE1);
    radio.openReadingPipe(1, NODE2);
  }

  radio.startListening();
}

void loop() {
  bool pttPressed = digitalRead(PTT_PIN) == HIGH;

  if (pttPressed) {
    if (!previousPTT) {
      lastTransmit = millis() - TX_INTERVAL;
    }

    if (millis() - lastTransmit >= TX_INTERVAL) {
      lastTransmit = millis();
      transmitPacket();
    }
  } else {
    receivePackets();
  }

  previousPTT = pttPressed;
  delay(1);
}
