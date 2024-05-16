#!/usr/bin/env python

""" Microcontroller loader script

The loader script is responsible for interacting with the programmer hardware of
the microcontroller. This script supports 3 actions:

1. flashing the image to the controller
2. verifying the image on the controller matches a given input file
3. erasing the entire flash on the controller
"""

import argparse
import os
import subprocess
import sys

from dataclasses import dataclass

@dataclass
class CliArgs:
    """Container class for command line parameters"""
    hexfile: str
    verify: bool
    erase: bool


    def __init__(self):
        parser = argparse.ArgumentParser(
            description='Microcontroller application loader utility.',
        )

        parser.add_argument('-v', '--verify', action='store_true',
            help='Verify the image on the controller. Ignored if erase specified')
        
        parser.add_argument('-e', '--erase',  action='store_true',
            help='Erase the controller.')

        parser.add_argument('hexfile', nargs='?')

        args = parser.parse_args()

        self.hexfile = args.hexfile
        self.verify  = args.verify
        self.erase   = args.erase

if __name__ == "__main__":
    cli_arg = CliArgs()

    cmd = [
        'avrdude',              # programmer utility
        '-c', 'atmelice_isp',   # Atmel ICE programmer connection
        '-p', 'm328p',          # ATmega328p part
        '-v'                    # verbose output
    ]

    if cli_arg.erase:
        cmd.append('-e')
    else:
        
        if not os.path.exists(cli_arg.hexfile):
            sys.exit(f'Cannot find hexfile: {cli_arg.hexfile}')
        
        op = 'v' if cli_arg.verify else 'w'

        cmd.extend(['-U', f'flash:{op}:{cli_arg.hexfile}:i'])

    print(f'Running command: {" ".join(cmd)}')
    sys.exit(subprocess.run(cmd).returncode)