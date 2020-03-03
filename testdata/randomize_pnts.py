#!/usr/bin/python

import random
import sys
import getopt


def main(argv):
    output_file_path = ""
    input_size = 10
    help_msg = "randomize.py -o <output_file> -s <input_size>"

    try:
        opts, args = getopt.getopt(argv, "ho:s:", ["ofile=", "input_size="])
    except getopt.GetoptError:
        print help_msg
        sys.exit(2)

    for opt, arg in opts:
        if opt == "-h":
            print help_msg
            sys.exit()
        elif opt in ("-o", "--ofile"):
            output_file_path = arg
        elif opt in ("-s", "--input_size"):
            input_size = int(arg)

    gen_file(output_file_path, input_size)


def gen_file(output_file_path, input_size):
    file = open(output_file_path, "w+")

    for i in range(input_size):
        x = random.uniform(0, 1)
        y = random.uniform(0, 1)
        file.write("{} {}\n".format(x, y))


if __name__ == "__main__":
    main(sys.argv[1:])
