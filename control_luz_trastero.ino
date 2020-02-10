#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(10, 11); //(TX, RX)

#define ACTIVATION_PIN 13 // define pin 13 as control pin

long previousAutoSwitchOffMinutes = 0;
long previousAutoPulseMinutes = 0;
long autoSwitchOffMinutes = 60;
long preshButtonEvery = 1;
char light_status;

void setup()
{
    pinMode(ACTIVATION_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Bluetooth.begin(9600);
}

void preshHallwayButton()
{
    digitalWrite(ACTIVATION_PIN, HIGH);
    delay(200);
    digitalWrite(ACTIVATION_PIN, LOW);
}

void loop()
{

    unsigned long currentMinutes = millis() / 60000;

    // Simulates a button presh to avoid light turns off
    if (light_status == '1' && currentMinutes - previousAutoPulseMinutes >= preshButtonEvery)
    {
        previousAutoPulseMinutes = currentMinutes;
        preshHallwayButton();
        Bluetooth.println("Pulsador apretado.");
    }

    // Check if auto switch off time is reached
    if (light_status == '1' && currentMinutes - previousAutoSwitchOffMinutes >= autoSwitchOffMinutes)
    {
        previousAutoSwitchOffMinutes = currentMinutes;
        light_status = '0';
        Bluetooth.println("Luz apagada automáticamente tras " + String(autoSwitchOffMinutes) + " minutos encendida.");
    }

    if (Bluetooth.available())

    {
        light_status = (Bluetooth.read());

        if (light_status == '1')
        {
            preshHallwayButton();
            Bluetooth.println("Luz encendida.");
            previousAutoSwitchOffMinutes = currentMinutes;
        }

        if (light_status == '0')
        {
            Bluetooth.println("Luz apagada.");
        }

        if (light_status == '?')
        {
            Bluetooth.println("Envía '1' para ENCENDER la luz.");
            Bluetooth.println("Envía '0' para APAGAR la luz.");
        }
    }
}
