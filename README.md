# SlomeScript

This is my own programming language:
in development since 31-1-2023

This is just for me to explore C++ a bit, and explore what goes into making a (interpreted) language.

In order to run your own script, and the provided test script. Compile the cpp file, and open the executable. By default it should open the "main.sls" file. you can edit this in the source code.
Edit the sls file and watch the magic happen! A documentation is not yet made, so study the structure if you want to make something cool

# NOTICE

This language right now is just a few days old, so it does not yet include crucial things like ARRAYS, stay tuned, I plan on adding these really soon.
<code>
Features
DO:
  &ensp;SAY: logs to console
  &ensp;SET: sets a variable (also includes math)
  &ensp;[function name]: executes a function
MAKE:
  &ensp;INT, DOUBLE, NUMBER, STRING, BOOL: make a variable
SKIP:
  &ensp;TO [location_name]: start reading from the location specified
    &ensp;UNLESS/WHEN: only skip if condition is true (conditions are incased by @)
  &ensp;LOCATION [location_name]: sets a point for a skip to skip to, skips can also skip backwards (allowing for loops etc) 
DEFINE:
  &ensp;FUNC: define a function (ENDFUNC to end a function)
</code>
This language does not like spaces! any parameters like logical operations are done like this @val==othervall||val>largeval@
I am gonna write some sort of documentation in the future!
This is open source because I do not care if anyone can edit, this project is made for me to practise c++.
