#!/usr/bin/python

import random, sys
from optparse import OptionParser

class shuf:
    # initialization
    def __init__(self, input_name, numlines, low, high,
                 yes_range, repeat):
        self.input_name = input_name
        self.repeat = repeat
        self.yes_range = yes_range

        # if number range create list of numbers in range
        # if - or empty take stdin or splits file into lines
        if yes_range:
            self.input_text = []
            for i in range(low, high):
                self.input_text.append(i)
            self.input_text.append(high)
        else:
            if input_name == "-" or input_name == "":
                self.input_text = sys.stdin.readlines()
            else:
                f = open(input_name, 'r')
                self.input_text = f.readlines()
                f.close()

        # number of lines, sets to lines in file if not repeat        
        self.numlines = numlines
        if self.numlines > len(self.input_text) and not repeat:
            self.numlines = len(self.input_text)

        # randomizes the order of the list
        self.permutation = self.input_text
        random.shuffle(self.permutation)
        
    def display(self):
        # displays the entire file either once, numlines or forever
        if self.repeat:
            if self.numlines == sys.maxsize:
                while True:
                    sys.stdout.write(random.choice(self.permutation))
            else:
                for i in range(self.numlines):
                    sys.stdout.write(random.choice(self.permutation))
        else:
            if self.yes_range:
                for i in range(self.numlines):
                    sys.stdout.write(str(self.permutation[i])+ "\n")
            else:
                for i in range(self.numlines):
                    sys.stdout.write(self.permutation[i])

def main():
    version_msg = "%prog 2.0"

    usage_msg = """%prog [OPTION]... FILE
       or: %prog -i LO-HI [OPTION]...

Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input."""

    # creates the parser with three options
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--head-count",
                      action="store", dest="numlines", default=sys.maxsize,
                      help="output at most COUNT lines")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")
    parser.add_option("-i", "--input-range",
                      action="store", dest="inrange", default="",
                      help="treat each number LO through HI as an input line")
    options, args = parser.parse_args(sys.argv[1:])

    # try and except lines to check if option entries are valid
    try:
        numlines = int(options.numlines)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    # in range option, checks if valid range
    if options.inrange:
        yes_range = True
        try:
            inrange = options.inrange
            inrange.find('-')
            range_n = inrange.split('-')
            low = int(range_n[0])
            high = int(range_n[1])
        except:
            parser.error("invalid RANGE: {0}".
                     format(options.inrange))
        if (len(range_n) != 2 or (high - low) < 0 or low < 0
            or high < 0):
            parser.error("invalid input range: {0}".
                     format(options.inrange))
    else:
        yes_range = False
        low = 0
        high = 1
    # repeat option
    try:
        repeat = bool(options.repeat)
    except:
        parser.error("invalid OPTION: {0}".
                     format(options.repeat))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    # creates the input file name
    if yes_range:
        input_file = False
        if len(args) != 0:
            parser.error("extra argument: {0}".
                         format(args))
    else:
        if len(args) == 0:
            input_file = ""
        elif len(args) == 1:
            input_file = args[0]
        else:
            input_file = False
            parser.error("extra argument: {0}".
                         format(args))
    # use the shuf class to instantiate and
    # display function to display
    # OS error if invalid
    try:
        generator = shuf(input_file, numlines, low, high,
                         yes_range, repeat)
        generator.display()
    except OSError as err:
        parser.error("OS error{0}: ".
                     format(err))

if __name__ == "__main__":
    main()
