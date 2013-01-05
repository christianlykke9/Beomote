Beomote
=======

Bang &amp; Olufsen Remote Library for Arduino

This library decodes the infrared signal from a Bang &amp; Olufsen remote using a plasma protected Bang &amp; Olufsen infrared eye.

Supported devices
=======

The following remotes have been tested and works with the library:

- Bang &amp; Olufsen Beo4
- Bang &amp; Olufsen Beolink 1000


Connection
=======

The library uses Timer1 meaning that the infrared data cable must be connected to either pin 3 or 10.

Connector layout of the Bang &amp; Olufsen infrared eye:

1. Serial clock - SCL
2. Serial data - SDA
3. IR receive
4. 5V
5. Ground
