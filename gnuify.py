#!/bin/python3

import sys
import datetime


def main():
    """
    Main function.
    """
    # process command line arguments:
    argc = len(sys.argv)
    if argc < 2:  # not enough arguments
        print("usage: ./gnuify <infile> [<outfile>]")
    else:
        infile_name = sys.argv[1]
        if argc > 2:  # outfile (and possibly more stuff) given
            outfile_name = sys.argv[2]
        else:  # use default
            now = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
            outfile_name = now + "_" + infile_name.split(".")[0] + "_" + ".dat"
        print("infile:  '{}'".format(infile_name))
        print("outfile: '{}'".format(outfile_name))

    # read input file, filter and write output file:
    try:
        with open(infile_name, "r") as infile:
            with open(outfile_name, "w") as outfile:
                for line in infile:
                    keys, val = line.strip().split("=")
                    keys = keys.split("/")
                    if len(keys) == 2:  # 2d data
                        print("keys: {}, val: {}".format(keys, val))
                        outfile.write("{x}, {y}, {v}\n".format(
                            x=keys[0], y=keys[1], v=val)
                        )
    except Exception as e:
        print(e)


if __name__ == "__main__":
     main()
