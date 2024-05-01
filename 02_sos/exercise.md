# S O S

Now that you have a reliable way to monitor the passage of time on your
controller, it's time to use that new feature to blink a pattern on the LED. To 
add a little more complexity, this exercise requires you to blink the morse 
code sentence "SOS".

Before starting the exercise, it will be helpful to give some names to pieces of
a morse code sentence. Let's look at the string "SOS":

    ... --- ...

The first letter (__character__) is the letter "S" which is made up of three
__symbols__ of the type __dot__. The second letter "O" is made up of three
__dash__ symbols. The spaces between symbols in this project are given the name
__symbol gap__. The spaces between letters, words, and sentences are called
__character gaps__,  __word gaps__, and __sentence gaps__. 

The morse code timings for the symbols and gaps is typically given in something
called __dot time__. For example, a given encoding might say a dot is 200ms,
a dash is 3 dot times, a character gap is 5 dot times, etc.


## Exercise

Write an application that continually blinks the morse code pattern for "SOS"
(with a sentence gap at the end). Use the table below for symbol and gap timing.

| Element       | Timing (ms or dot times) |
| ------------- | :----------------------: |
| dot           |  100 milliseconds        |
| dash          |  4 dot times             |
| symbol gap    |  1 dot times             |
| character gap |  5 dot times             |
| word gap      |  7 dot times             |
| sentence gap  |  15 dot times            |

