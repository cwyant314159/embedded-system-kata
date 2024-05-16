# Sentence Statistics

We now have inbound (RX) and outbound (TX) UART traffic on our processor. Let's
put both of those together into a new app that does something a little more
interesting than echo'ing back to your terminal.

## Exercise

Write an application that takes in a new line terminated string from the UART.
When the new line is received, output the following statistics on the UART
transmitter:

- number of letters
- number of vowels
- number of digits
- number of whitespace characters (including the new line but not NULL)
- number of punctuation characters

Format the output as follows:

    =================
    Letters    : N
    Vowels     : N
    Digits     : N
    Whitespace : N
    Punctuation: N

The counts should clamp to a maximum value of 255. If the number of elements of
a given type exceeds 255, append a plus sign ('+') to the count. For example, 
in the output below, there were 500 digits sent to the processor:

    =================
    Letters    : 11
    Vowels     : 3
    Digits     : 255+
    Whitespace : 1
    Punctuation: 0

### Bonus

Toggle the LED every 500ms without blocking the UART.
