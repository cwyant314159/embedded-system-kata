# Sentence Statistics

This is the final exercise. We are going to combine everything we know about
timing, morse code, and UARTs into one final project.

## Exercise

Write an application that takes in a new line terminated string from the UART
and blinks out the morse code representation on the LED. The list below
summarizes the requirements of the application:

- The application should limit the string length to 40 characters.

- Characters above 40 should be ignored.

- The application should echo characters as they are typed for better user 
experience.

- The application does not have to handle the arrow keys, backspace, or delete.

- Only alphanumeric characters, white space, and sentence terminating
punctuation should count against the 40 character limit. All other characters
(non-printable, comma, etc.) should be ignored.

- Strings are terminated with new lines (`\n`). Carriage returns (`\r`) and
null terminators (`\0`) should be ignored.

- The application should not blink out the morse code until the new line
character is received.

- The application should not encode the new line character as a white space gap
in morse code.

- The user should not send a new string while the current string is still
blinking out the LED. If a new string is sent to the application while another
string is still blinking, the message 
`"\n\rERROR: Encoding already in progress!\n\r"` should be transmitted out the
UART.
