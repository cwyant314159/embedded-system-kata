# Hello, Morse!

It's time to put an embedded spin on the classical "Hello, World!" application
using morse code.

## Exercise

Write an application that continually blinks the morse code pattern for:

    Hello, Morse!

The table below describes the required behavior for ASCII characters

| ASCII characater (or hex) | Interpretation      |
| ------------------------- | ------------------- |
| a-z, A-Z, 0-9,            | standard morse code |
| . ? !                     | sentence gap        |
| 0x20 \n \t                | word gap            |
| all other ASCII codes     | ignore              |
